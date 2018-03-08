#!/usr/bin/pypy

import sys

#print(sys.stdin.read())

class FreeVariableException(Exception):
    pass

class SlotNameGenerator:
    def __init__(self, variables={}):
        self.variables = variables
        self.hashvalue = hash(tuple(sorted(self.variables.items())))

    def __repr__(self):
        return repr(tuple(sorted(self.variables.items())))

    def __eq__(self, other):
        return self.hashvalue == other.hashvalue

    def inherit(self, syntax):
        if len(syntax) == 2:
            return self
        else:
            newvariables = {syntax[i]:self.evaluate_index(syntax[i+1]) for i in range(2,len(syntax)-1,2)}
            return SlotNameGenerator(newvariables)

    def default(self, syntax):
        if syntax[2] in self.variables:
            return self
        else:
            newvariables = self.variables.copy()
            newvariables[syntax[2]] = self.evaluate_index(syntax[3])
            return SlotNameGenerator(newvariables) 

    def derive(self, name, index):
        newvariables = self.variables.copy()
        newvariables[name] = index
        return SlotNameGenerator(newvariables) 

    def __hash__(self):
        return self.hashvalue

    def evaluate_index(self, syntax):
        if syntax[0] == "baseconst":
            return syntax[1]
        elif syntax[0] == "basevar":
            if syntax[1] in self.variables:
                return self.variables[syntax[1]]
            else:
                raise FreeVariableException("{} not in {}".format(syntax[1], self.variables))
        elif syntax[0] == "addconst":
            return self.evaluate_index(syntax[1])+syntax[2]
        elif syntax[0] == "addvar":
            if syntax[2] in self.variables:
                return self.evaluate_index(syntax[1])+self.variables[syntax[2]]
            else:
                raise FreeVariableException("{} not in {}".format(syntax[2], self.variables))
        elif syntax[0] == "subconst":
            return self.evaluate_index(syntax[1])-syntax[2]
        elif syntax[0] == "subvar":
            if syntax[2] in self.variables:
                return self.evaluate_index(syntax[1])-self.variables[syntax[2]]
            else:
                raise FreeVariableException("{} not in {}".format(syntax[2], self.variables))
        raise Exception("Error, \"{}\" is not allowed in index calculation.".format(syntax[0]))

    def evaluate_single(self, syntax):
        if syntax[0] == "slotbase":
            return syntax[1]
        elif syntax[0] == "slotmember":
            return self.evaluate_single(syntax[1]) + "." + syntax[2]
        elif syntax[0] == "slotindex":
            return self.evaluate_single(syntax[1]) + "[{}]".format(self.evaluate_index(syntax[2]))
        raise Exception("Error, \"{}\" is not allowed in variable name.".format(syntax[0]))

    def evaluate_list(self, syntax):
        if syntax[0] == "slottuple":
            return [t for s in syntax[1:] for t in self.evaluate_list(s)]
        elif syntax[0] == "slotrange":
            rangestart = self.evaluate_index(syntax[2])
            rangeend   = self.evaluate_index(syntax[3])
            return [self.evaluate_single(syntax[1]) + "[{}]".format(i) for i in range(rangestart, rangeend)]
        elif syntax[0] == "slotbase":
            return [syntax[1]]
        elif syntax[0] == "slotmember":
            return [s + "." + syntax[2] for s in self.evaluate_list(syntax[1])]
        elif syntax[0] == "slotindex":
            return [s + "[{}]".format(self.evaluate_index(syntax[2])) for s in self.evaluate_list(syntax[1])]
        raise Exception("Error, \"{}\" is not allowed in variable name list.".format(syntax[0]))

class SlotCollector:
    def __init__(self, specs):
        self.specs = specs
        self.cache = {}

    def __call__(self, name, slotnamegen=SlotNameGenerator()):
        hashable = (name,slotnamegen)
        if hashable in self.cache:
            return self.cache[hashable]
        else:
            temp = self.evaluate(self.specs[name], slotnamegen)
            self.cache[hashable] = temp
            return temp

    def filter(self, slots):
        tempset  = set()
        filtered = []

        for elem in slots:
            if elem not in tempset:
                filtered.append(elem)
                tempset.add(elem)

        return filtered

    def evaluate(self, syntax, slotnamegen):
        if syntax[0] == "atom":
            return [slotnamegen.evaluate_single(s) for s in syntax[1][1:2]+syntax[1][3:1:-1]]
        elif syntax[0] == "ConstraintOpcode":
            return [slotnamegen.evaluate_single(syntax[1])]
        elif syntax[0] in ["GeneralizedDominance", "GeneralizedSame"]:
            return [slot for s in syntax[1:2]+syntax[3:1:-1] for slot in slotnamegen.evaluate_list(s)]
        elif syntax[0] in ["conjunction", "disjunction"]:
            return self.filter([s for b in syntax[1:]
                                  for s in self.evaluate(b, slotnamegen)])
        elif syntax[0] == "collect":
            return self.filter([s for i in range(syntax[2])
                                  for s in self.evaluate(syntax[3], slotnamegen.derive(syntax[1], i))])
        elif syntax[0] in ["forall", "forsome"]:
            return self.filter([s for i in range(slotnamegen.evaluate_index(syntax[3]), slotnamegen.evaluate_index(syntax[4]))
                                  for s in self.evaluate(syntax[1], slotnamegen.derive(syntax[2], i))])
        elif syntax[0] == "if":
            if slotnamegen.evaluate_index(syntax[1]) == slotnamegen.evaluate_index(syntax[2]):
                return self.evaluate(syntax[3], slotnamegen)
            else:
                return self.evaluate(syntax[4], slotnamegen)
        elif syntax[0] == "default":
            return self.evaluate(syntax[1], slotnamegen.default(syntax))
        elif syntax[0] == "rename":
            if (len(syntax) % 2) == 1:
                base = slotnamegen.evaluate_single(syntax[-1])+"."
            else:
                base = ""
            slotdict = {slotnamegen.evaluate_single(syntax[i+1]):slotnamegen.evaluate_single(syntax[i]) for i in range(2,len(syntax)-1,2)}
            return [slotdict[slot] if slot in slotdict else base+slot for slot in self.evaluate(syntax[1], slotnamegen)]
        elif syntax[0] == "inheritance":
            return self(syntax[1], slotnamegen.inherit(syntax))
        raise Exception("Error, \"{}\" is not allowed in constraint.".format(syntax[0]))

class ConstraintGenerator:
    def __init__(self, specs):
        self.specs       = specs
        self.cache       = {}
        self.slotcollect = SlotCollector(specs)

    def __call__(self, syntax, slotnamegen=SlotNameGenerator()):
        if syntax[0] == "atom":
            classname = "Backend{}".format(syntax[1][0][10:])
            if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
                code =  "my_shared_ptr<{}> constraint = {{{{0,1,1}}, wrap}};".format(classname)
            elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
                code =  "my_shared_ptr<{}> constraint = {{{{1,1,1}}, wrap}};".format(classname)
            elif len(syntax[1]) > 2:
                code =  "my_shared_ptr<{}> constraint = {{wrap}};".format(classname)
            else:
                code = "{} constraint = {{wrap}};".format(classname)

            if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
                values = [("MultiVectorSelector<{},{}>".format(classname, i), "constraint, {0}") for i in [1,2]]
            elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
                values = [("MultiVectorSelector<{},{}>".format(classname, i), "constraint, {0}") for i in [0,1,2]]
            elif len(syntax[1]) == 2:
                values = [(classname, "constraint")]
            else:
                values = [("ScalarSelector<{},{}>".format(classname, i), "constraint") for i in range(len(syntax[1])-1)]

            return code,values

        elif syntax[0] == "ConstraintOpcode":
            opcode = syntax[2][:1].upper()+syntax[2][1:]
            if opcode[:3] == "Gep": opcode = "GEP"+opcode[3:]
            if opcode[-2:] == "or": opcode = opcode[:-2]+"Or"
            if opcode[-3:] == "and": opcode = opcode[:-3]+"And"
            if opcode[-3:] == "add": opcode = opcode[:-3]+"Add"
            if opcode[-3:] == "sub": opcode = opcode[:-3]+"Sub"
            if opcode[-3:] == "mul": opcode = opcode[:-3]+"Mul"
            if opcode[-3:] == "div": opcode = opcode[:-3]+"Div"
            if opcode[-3:] == "cmp": opcode = opcode[:-3]+"Cmp"
            if opcode[-3:] == "ext": opcode = opcode[:-3]+"Ext"
            if opcode[-4:] == "cast": opcode = opcode[:-4]+"Cast"
            if opcode[-5:] == "shift": opcode = opcode[:-5]+"Shift"
            if opcode[-6:] == "vector": opcode = opcode[:-6]+"Vector"
            if opcode[-7:] == "element": opcode = opcode[:-7]+"Element"

            classname = "Backend{}Inst".format(opcode)
            code      = "{} constraint = {{wrap}};".format(classname)
            values    = [(classname, "constraint")]
            return code,values


        elif syntax[0] == "GeneralizedDominance":
            classname = "BackendPDGDominate"
            slotlists = [slotnamegen.evaluate_list(s) for s in syntax[1:2]+syntax[3:1:-1]]
            code      = "my_shared_ptr<{}> constraint = {{{{{},{},{}}}, wrap}};".format(classname,  len(slotlists[0]), len(slotlists[1]), len(slotlists[2]))
            values    = [("MultiVectorSelector<classname,{}>".format(classname,i), "constraint, {{{}}}".format(j))
                         for i in [0,1,2] for j in range(len(slotlists[i]))]
            return code,values

        elif syntax[0] == "GeneralizedSame":
            classname = "BackendSameSets"
            slotlists = [slotnamegen.evaluate_list(s) for s in syntax[1:2]+syntax[3:1:-1]] # SLOTLISTS MUST BE SAME LENGTH BUT I DON'T CHECK!!!
            code      = "my_shared_ptr<{}> constraint = {{{{{}}}, wrap}};".format(classname,  len(slotlists[0]))
            values    = [("MultiVectorSelector<classname,{}>".format(classname,i), "constraint, {{{}}}".format(j))
                         for i in [0,1] for j in range(len(slotlists[i]))]
            return code,values

        elif syntax[0] == "conjunction":
            globalslots    = self.slotcollect.evaluate(syntax, slotnamegen)
            globalslotdict = {name:nr for (nr,name) in enumerate(globalslots)}
            assignments    = [[] for s in globalslots]
            revassignments = []

            for branch,slotlist in enumerate(syntax[1:]):
                localrevassignments = []
                for n,slot in enumerate(self.slotcollect.evaluate(slotlist, slotnamegen)):
                    localrevassignments.append((globalslotdict[slot], len(assignments[globalslotdict[slot]])))
                    assignments[globalslotdict[slot]].append((branch,n))
                revassignments.append(localrevassignments)

            localresults = [self(s) for s in syntax[1:]]
            localcodes   = [branch[0] for branch in localresults]
            localtypes   = [[value[0] for value in branch[1]] for branch in localresults]
            localexpr    = [[value[1] for value in branch[1]] for branch in localresults]

            code = []
            for i,perslotass in enumerate(assignments):
                for j,ass in enumerate(perslotass):
                    code.append("{} temp{}_{};".format(localtypes[ass[0]][ass[1]], i, j))

            for i,branchcode in enumerate(localcodes):
                code.append("{ "+branchcode.replace("\n", "\n  "))

                for j,expr in enumerate(localexpr[i]):
                    revass = revassignments[i][j]
                    code.append("  temp{}_{} = decltype(temp{}_{})({});".format(revass[0], revass[1], revass[0], revass[1], expr))

                code.append("}")

            values = [("<type>", "<expr") for slot in self.slotcollect.evaluate(syntax, slotnamegen)]

            return "\n".join(code),values
        else:
            return "// No codegen yet for {}".format(syntax[0]), [("<type>", "<expr") for slot in self.slotcollect.evaluate(syntax, slotnamegen)]


def generate_fast_cpp_specification(syntax, collector, generator):

    sys.stderr.write("Generate {}:\n".format(syntax[1]))
    try:
        variablecomments = ["    // {}".format(name) for name in collector(syntax[1])]
        code = "\n".join(["IdiomSpecification Detect{}(llvm::Function& function, unsigned max_solutions)".format(syntax[1])]
                        +["{"]
                        +["    FunctionWrap wrap(function);"]
         #               +["    // {}".format(name) for name in collector(syntax[1])]
                        +["    "+generator(syntax[2])[0].replace("\n", "\n    ")]
                        +["    auto result = Solution::Find(move(constraint), function, max_solutions);"]
                        +["    return result;"]
                        +["}"])
        return code
    except FreeVariableException as exc:
        sys.stderr.write("    failed with {}\n".format(exc))
        return "// {} is parameterized: {}".format(syntax[1], exc)

def generate_cpp_code(syntax_list):
    includes  = ["BackendSpecializations", "BackendDirectClasses", "BackendSelectors"]
    specs     = {spec[1] : spec[2] for spec in syntax_list}

    collector = SlotCollector(specs)
    generator = ConstraintGenerator(specs)

    return "\n".join(["#include \"llvm/CAnDLSolver/{}.hpp\"".format(s) for s in includes]
                    +["#include \"llvm/CAnDLParser/Solution.hpp\""]
                    +["using namespace std;"]
                    +["#pragma GCC optimize (\"O0\")"]
                    +["#pragma clang optimize off"]
                    +["using IdiomSpecification = vector<Solution>;"]
                    +["template<typename T>"]
                    +["class my_shared_ptr : public shared_ptr<T>"]
                    +["{"]
                    +["public:"]
                    +["    my_shared_ptr() = default;"]
                    +["    my_shared_ptr<T>& operator=(T t) { shared_ptr<T>::operator=(make_shared<T>(move(t))); return *this; }"]
                    +["    my_shared_ptr<T>& operator=(const my_shared_ptr<T>& t) { return *this = *t; }"]
                    +["};"]
                    +[generate_fast_cpp_specification(syntax, collector, generator) for syntax in syntax_list])

print(generate_cpp_code(eval(sys.stdin.read())))


