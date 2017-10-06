#!/usr/bin/pypy

grammar = """
# ::= <specification>

specification ::= Constraint <s> <constraint> End

constraint ::= <grouping> | <collect> | <rename> | <rebase> | <atom> | <GeneralizedDominance> | '(' <constraint> ')'

grouping    ::= <conjunction> | <disjunction> | <inheritance> | <forall> | <forsome> | <forone> | <if> | <default>
inheritance ::= <rawinherit> | <arginherit>
rawinherit  ::= inherits <s>
arginherit  ::= <rawinherit> '(' <s> = <calculation> { , <s> = <calculation> } ')'
conjunction ::= '(' <constraint> and <constraint> { and <constraint> } ')'
disjunction ::= '(' <constraint> or <constraint> { or <constraint> } ')'
forall      ::= <constraint> for all <s> = <calculation> .. <calculation>
forsome     ::= <constraint> for some <s> = <calculation> .. <calculation>
forone      ::= <constraint> for <s> = <calculation>
default     ::= <constraint> for <s> = <calculation> if not otherwise specified
if          ::= if <calculation> = <calculation> then <constraint> else <constraint> endif
collect     ::= collect <s> <n> <constraint>
rename      ::= <grouping> with <slot> as <slot> { and <slot> as <slot> }
rebase      ::= <grouping> at <slot> | <rename> at <slot>

slot       ::= '{' <openslot> '}' | <slottuple>
openslot   ::= <slotbase> | <slotindex> | <slotrange> | <slotmember> | <slottuple>
slotbase   ::= <s>
slotindex  ::= <openslot> '[' <calculation> ']'
slotrange  ::= <openslot> '[' <calculation> .. <calculation> ']'
slotmember ::= <openslot> . <s>
slottuple  ::= '{' <openslot> , { <openslot> , } <openslot> '}'

calculation ::= <basevar> | <baseconst> | <addvar> | <addconst> | <subvar> | <subconst>
basevar     ::= <s>
baseconst   ::= <n>
addvar      ::= <calculation> + <s>
addconst    ::= <calculation> + <n>
subvar      ::= <calculation> - <s>
subconst    ::= <calculation> - <n>

atom ::= <ConstraintIntegerType> | <ConstraintFloatType> | <ConstraintPointerType> \
         | <ConstraintIntZero> | <ConstraintFloatZero> \
         | <ConstraintUnused>       | <ConstraintNotNumericConstant> | <ConstraintConstant> \
         | <ConstraintPreexecution> | <ConstraintArgument>           | <ConstraintInstruction> \
         | <ConstraintStoreInst>  | <ConstraintLoadInst>   | <ConstraintReturnInst> | <ConstraintBranchInst> \
         | <ConstraintAddInst>    | <ConstraintSubInst>    | <ConstraintMulInst>    | <ConstraintFAddInst>   \
         | <ConstraintFSubInst>   | <ConstraintFMulInst>   | <ConstraintFDivInst>   | <ConstraintBitOrInst>  \
         | <ConstraintLShiftInst> | <ConstraintSelectInst> | <ConstraintSExtInst>   | <ConstraintZExtInst>   \
         | <ConstraintGEPInst>    | <ConstraintICmpInst> \
         | <ConstraintSame> | <ConstraintDistinct> \
         | <ConstraintDFGEdge> | <ConstraintCFGEdge> | <ConstraintCDGEdge> | <ConstraintPDGEdge> \
         | <ConstraintFirstOperand> | <ConstraintSecondOperand> \
         | <ConstraintThirdOperand> | <ConstraintFourthOperand> \
         | <ConstraintDFGDominate>          | <ConstraintDFGPostdom>          \
         | <ConstraintDFGDominateStrict>    | <ConstraintDFGPostdomStrict>    \
         | <ConstraintCFGDominate>          | <ConstraintCFGPostdom>          \
         | <ConstraintCFGDominateStrict>    | <ConstraintCFGPostdomStrict>    \
         | <ConstraintPDGDominate>          | <ConstraintPDGPostdom>          \
         | <ConstraintPDGDominateStrict>    | <ConstraintPDGPostdomStrict>    \
         | <ConstraintDFGNotDominate>       | <ConstraintDFGNotPostdom>       \
         | <ConstraintDFGNotDominateStrict> | <ConstraintDFGNotPostdomStrict> \
         | <ConstraintCFGNotDominate>       | <ConstraintCFGNotPostdom>       \
         | <ConstraintCFGNotDominateStrict> | <ConstraintCFGNotPostdomStrict> \
         | <ConstraintPDGNotDominate>       | <ConstraintPDGNotPostdom>       \
         | <ConstraintPDGNotDominateStrict> | <ConstraintPDGNotPostdomStrict> \
         | <ConstraintIncomingValue> \
         | <ConstraintDFGBlocked> | <ConstraintCFGBlocked> | <ConstraintPDGBlocked>

ConstraintIntegerType ::= <slot> is an integer
ConstraintFloatType   ::= <slot> is a float
ConstraintPointerType ::= <slot> is a pointer

ConstraintIntZero   ::= <slot> is integer zero
ConstraintFloatZero ::= <slot> is floating point zero

ConstraintUnused             ::= <slot> is unused
ConstraintNotNumericConstant ::= <slot> is not a numeric constant
ConstraintConstant           ::= <slot> is a constant
ConstraintPreexecution       ::= <slot> is preexecution
ConstraintArgument           ::= <slot> is an argument
ConstraintInstruction        ::= <slot> is an instruction

ConstraintStoreInst  ::= <slot> is store instruction
ConstraintLoadInst   ::= <slot> is load instruction
ConstraintReturnInst ::= <slot> is return instruction
ConstraintBranchInst ::= <slot> is branch instruction
ConstraintAddInst    ::= <slot> is add instruction
ConstraintSubInst    ::= <slot> is sub instruction
ConstraintMulInst    ::= <slot> is mul instruction
ConstraintFAddInst   ::= <slot> is fadd instruction
ConstraintFSubInst   ::= <slot> is fsub instruction
ConstraintFMulInst   ::= <slot> is fmul instruction
ConstraintFDivInst   ::= <slot> is fdiv instruction
ConstraintBitOrInst  ::= <slot> is bitor instruction
ConstraintLShiftInst ::= <slot> is lshift instruction
ConstraintSelectInst ::= <slot> is select instruction
ConstraintSExtInst   ::= <slot> is sext instruction
ConstraintZExtInst   ::= <slot> is zext instruction
ConstraintGEPInst    ::= <slot> is gep instruction
ConstraintICmpInst   ::= <slot> is icmp instruction

ConstraintSame     ::= <slot> is the same as <slot>
ConstraintDistinct ::= <slot> is not the same as <slot>

ConstraintDFGEdge ::= <slot> has data flow to <slot>
ConstraintCFGEdge ::= <slot> has control flow to <slot>
ConstraintCDGEdge ::= <slot> has control dominance to <slot>
ConstraintPDGEdge ::= <slot> has dependence edge to <slot>

ConstraintFirstOperand  ::= <slot> is first argument of <slot>
ConstraintSecondOperand ::= <slot> is second argument of <slot>
ConstraintThirdOperand  ::= <slot> is third argument of <slot>
ConstraintFourthOperand ::= <slot> is fourth argument of <slot>

ConstraintDFGDominate       ::= <slot> data flow dominates <slot>
ConstraintDFGPostdom        ::= <slot> data flow post dominates <slot>
ConstraintDFGDominateStrict ::= <slot> strictly data flow dominates <slot>
ConstraintDFGPostdomStrict  ::= <slot> strictly data flow post dominates <slot>

ConstraintCFGDominate       ::= <slot> control flow dominates <slot>
ConstraintCFGPostdom        ::= <slot> control flow post dominates <slot>
ConstraintCFGDominateStrict ::= <slot> strictly control flow dominates <slot>
ConstraintCFGPostdomStrict  ::= <slot> strictly control flow post dominates <slot>

ConstraintPDGDominate       ::= <slot> dominates <slot>
ConstraintPDGPostdom        ::= <slot> post dominates <slot>
ConstraintPDGDominateStrict ::= <slot> strictly dominates <slot>
ConstraintPDGPostdomStrict  ::= <slot> strictly post dominates <slot>

ConstraintDFGNotDominate       ::= <slot> does not data flow dominate <slot>
ConstraintDFGNotPostdom        ::= <slot> does not data flow post dominate <slot>
ConstraintDFGNotDominateStrict ::= <slot> does not strictly data flow dominate <slot>
ConstraintDFGNotPostdomStrict  ::= <slot> does not strictly data flow post dominate <slot>

ConstraintCFGNotDominate       ::= <slot> does not control flow dominate <slot>
ConstraintCFGNotPostdom        ::= <slot> does not control flow post dominate <slot>
ConstraintCFGNotDominateStrict ::= <slot> does not strictly control flow dominate <slot>
ConstraintCFGNotPostdomStrict  ::= <slot> does not strictly control flow post dominate <slot>

ConstraintPDGNotDominate       ::= <slot> does not dominate <slot>
ConstraintPDGNotPostdom        ::= <slot> does not post dominate <slot>
ConstraintPDGNotDominateStrict ::= <slot> does not strictly dominate <slot>
ConstraintPDGNotPostdomStrict  ::= <slot> does not strictly post dominate <slot>

ConstraintIncomingValue ::= <slot> reaches phi node <slot> from <slot>

ConstraintDFGBlocked ::= all data flow from <slot> to <slot> passes through <slot>
ConstraintCFGBlocked ::= all control flow from <slot> to <slot> passes through <slot>
ConstraintPDGBlocked ::= all flow from <slot> to <slot> passes through <slot>

GeneralizedDominance ::= all flow from <slot> or any origin to any of <slot> passes through at least one of <slot>
"""

def token_match(token, rule):
    if rule.startswith("<") and rule.endswith(">"):
        if rule == "<s>":   return type(token) is str
        elif rule == "<n>": return type(token) is int
        else:               return type(token) is tuple and token[0] == rule[1:-1]
    else:                   return type(token) is str and token == rule

def full_match(stack, rules, lookahead):
    if lookahead and (len(stack) < len(rules) + 1 or not any(token_match(stack[-len(rules)-1], l) for l in lookahead)):
        return False
    elif len(stack) < len(rules) or not all(token_match(stack[-1-i], rules[-1-i]) for i in range(len(rules))):
        return False
    else:
        return True

def list_possibles(stack, grammar, lookahead_rules):
    possibles    = []
    not_dead_end = all(type(s) == tuple and s[0]=="#" for s in stack)

    for name, rule in grammar:
        for branch in rule:
            if full_match(stack, branch, lookahead_rules[name]):
                new_element = (name,)+ tuple(a for a,b in zip(stack[-len(branch):],branch) if b[:1]+b[-1:] == "<>")
                possibles += list_possibles(stack[:-len(branch)] + [new_element], grammar, lookahead_rules)
            if not not_dead_end and any(full_match(stack, branch[:part], lookahead_rules[name]) for part in range(1, len(branch))):
                not_dead_end = True

    return possibles + [stack] if not_dead_end else possibles

def strip_invisibles(syntax_list):
    return_syntax_list = ()

    for syntax in syntax_list:
        if type(syntax) == tuple:
            if syntax[0] in ["#", "conjunctionprefix", "constraint", "disjunctionprefix", "renameprefix", "slot", 
                             "openslot", "slottupleprefix", "calculation", "arginheritprefix", "rawinherit",
                             "arginherit", "openslot2", "grouping"]:
                return_syntax_list = return_syntax_list + strip_invisibles(syntax[1:])
            else:
                return_syntax_list = return_syntax_list + (syntax[:1] + strip_invisibles(syntax[1:]),)
        else:
            return_syntax_list = return_syntax_list + (syntax,)
    return return_syntax_list

def parse(code, grammartext):
    grammar = []

    for name,rule in ((s.strip() for s in line.split("::=")) for line in grammartext.split('\n') if line):
        if "{ " in rule and " }" in rule:
            prefix = rule.split("{ ")[0]
            core   = rule.split("{ ")[1].split(" }")[0]
            suffix = rule.split(" }")[1]
            rule   = "<"+name+"prefix> "+suffix

            grammar.append((name+"prefix", prefix+" | <"+name+"prefix> "+core))

        grammar.append((name, rule))

    grammar = [(name, rule.replace("'", "").split(" | ")) for name,rule in grammar]
    grammar = [(name, [[token for token in branch.split(' ') if token] for branch in rule]) for name,rule in grammar]

    lookahead_rules = {}

    for name,rule in grammar:

        old_aliases = []
        new_aliases = [name]
        while new_aliases:
            old_aliases = old_aliases + new_aliases
            new_aliases = []
            for othername, otherrule in grammar:
                if othername not in new_aliases and othername not in old_aliases:
                    for branch in otherrule:
                        if branch[0][0] + branch[0][-1] == "<>" and branch[0][1:-1] in old_aliases:
                            new_aliases.append(othername)

        if "#" in old_aliases:
            lookahead_rules[name] = []
        else:
            predecessors = []

            for othername, otherrule in grammar:
                for branch in otherrule:
                    for i,tokenrule in enumerate(branch[1:]):
                        if tokenrule[0]+tokenrule[-1] == "<>" and tokenrule[1:-1] in old_aliases:
                            predecessors.append(branch[i])

            if "<s>" in predecessors or "<n>" in predecessors:
                predecessors = []

            lookahead_rules[name] = predecessors

    code = " ".join(line.split("#")[0] for line in code.split("\n"))
    code = "".join(" "+c+" " if c in "(){}[]=.+-," else c for c in code)
    code = code.replace(".  .", "..")

    possibles = [[]]

    debug_tokens = []
    for token in [int(token) if token.isdigit() else token for token in code.split() if token]:
        possibles = sum([list_possibles(pos+[token], grammar, lookahead_rules) for pos in possibles], [])

        debug_tokens = debug_tokens[-7:]+[str(token)]

        if not possibles:
            raise Exception("Syntax error at \""+" ".join(debug_tokens)+"\"")

    possibles = [pos for pos in possibles if all(type(s) == tuple and s[0] == "#" for s in pos)]

    if len(possibles) == 1:
        return strip_invisibles(possibles[0])

def partial_evaluator(syntax, handler, *extras):
    handler_result = handler(syntax, *extras)
    if handler_result:
        return handler_result
    return tuple(partial_evaluator(s, handler, *extras) if type(s) is tuple else s for s in syntax)

def rebase(prefix, suffix):
    if not prefix:
        return suffix
    elif suffix[0] == "slotrange":
        return (suffix[0], rebase(prefix, suffix[1]), suffix[2], suffix[3])
    elif suffix[0] in ["slotmember", "slotindex"]:
        return (suffix[0], rebase(prefix, suffix[1]), suffix[2])
    elif suffix[0] == "slotbase":
        return ("slotmember", prefix, suffix[1])
    raise Exception("Error, \"" + suffix[0] + "\" is not allowed in suffix.")

def evaluate_remove_rename_rebase(syntax, renames={}, prefix=None):
    if syntax[0] in ["rebase", "rename"]:
        child      = syntax[1]  if syntax[0] == "rename" else syntax[1][1] if syntax[1][0] == "rename" else syntax[1]
        newprefix  = None       if syntax[0] == "rename" else syntax[-1]
        renamevars = syntax[2:] if syntax[0] == "rename" else syntax[1][2:] if syntax[1][0] == "rename" else []
        newrenames = dict(zip(renamevars[1::2], renamevars[0::2]))
        
        return partial_evaluator(partial_evaluator(child, evaluate_remove_rename_rebase, newrenames, newprefix),
                                                          evaluate_remove_rename_rebase, renames, prefix)

    elif syntax[0] in ["slotmember", "slotindex"]:
        if syntax in renames:
            return renames[syntax]
        else:
            return (syntax[0], partial_evaluator(syntax[1], evaluate_remove_rename_rebase, renames, prefix), syntax[2])

    elif syntax[0] == "slotbase":
        if syntax in renames:
            return renames[syntax]
        else:
            return rebase(prefix, syntax)

def evaluate_remove_for_with(syntax, specs, vardict={}, collectvars=[]):
    if syntax[0] == "inheritance":
        new_vardict = {}
        for i in range(2,len(syntax),2):
            rangevalue = partial_evaluator(syntax[i+1], evaluate_remove_for_with, specs, vardict, collectvars)

            if rangevalue[0] == "baseconst":
                new_vardict[syntax[i]] = rangevalue[1]
            else:
                raise Exception("Free variables remain in for assignment.")

        if not new_vardict:
            new_vardict = vardict

        return partial_evaluator(specs[syntax[1]], evaluate_remove_for_with, specs, new_vardict, collectvars)

    elif syntax[0] in ["forall", "forsome"]:
        rangestart = partial_evaluator(syntax[3], evaluate_remove_for_with, specs, vardict, collectvars)
        rangestop  = partial_evaluator(syntax[4], evaluate_remove_for_with, specs, vardict, collectvars)

        if rangestart[0] == rangestop[0] == "baseconst":

            branches = []

            for i in range(rangestart[1],rangestop[1]):

                new_vardict = dict(vardict)
                new_vardict[syntax[2]] = i
                branches.append(partial_evaluator(syntax[1], evaluate_remove_for_with, specs, new_vardict, collectvars))

            return ("conjunction" if syntax[0] == "forall" else "disjunction",)+tuple(branches)
        raise Exception("Free variables remain in for loop range start.")

    elif syntax[0] == "forone":
        rangevalue = partial_evaluator(syntax[3], evaluate_remove_for_with, specs, vardict, collectvars)

        if rangevalue[0] == "baseconst":

            new_vardict = dict(vardict)
            new_vardict[syntax[2]] = rangevalue[1]
            return partial_evaluator(syntax[1], evaluate_remove_for_with, specs, new_vardict, collectvars)
        raise Exception("Free variables remain in for assignment.")

    elif syntax[0] == "if":
        leftvalue  = partial_evaluator(syntax[1], evaluate_remove_for_with, specs, vardict, collectvars)
        rightvalue = partial_evaluator(syntax[2], evaluate_remove_for_with, specs, vardict, collectvars)

        if leftvalue[0] == "baseconst" and rightvalue[0] == "baseconst":
            if leftvalue[1] == rightvalue[1]:
                return partial_evaluator(syntax[3], evaluate_remove_for_with, specs, vardict, collectvars)
            else:
                return partial_evaluator(syntax[4], evaluate_remove_for_with, specs,vardict, collectvars)
        raise Exception("Free variables remain in conditional.")

    elif syntax[0] == "default":
        defaultvalue = partial_evaluator(syntax[3], evaluate_remove_for_with, specs, vardict, collectvars)

        if syntax[2] in vardict or syntax[2] in collectvars:
            return partial_evaluator(syntax[1], evaluate_remove_for_with, specs, vardict, collectvars)
        elif defaultvalue[0] == "baseconst":
            new_vardict = dict(vardict)
            new_vardict[syntax[2]] = defaultvalue[1]
            return partial_evaluator(syntax[1], evaluate_remove_for_with, specs, new_vardict, collectvars)
        raise Exception("Free variables remain in default value.")

    elif syntax[0] == "collect":
        new_collectvars = collectvars[:] + [syntax[1]]
        return syntax[:3] + (partial_evaluator(syntax[3], evaluate_remove_for_with, specs, vardict, new_collectvars),)

    elif syntax[0] == "basevar":
        if syntax[1] in vardict:
            return ("baseconst", vardict[syntax[1]])
        elif syntax[1] in collectvars:
            return syntax
        raise Exception("Free variable \""+syntax[1]+"\" remain in static calculation.")

    elif syntax[0] in ["addvar", "subvar"]:
        if syntax[2] in vardict:
            return evaluate_remove_for_with((syntax[0][:3]+"const", syntax[1], vardict[syntax[2]]), specs, vardict)
        raise Exception("Free variables remain in static calculation of "+syntax[0]+".")

    elif syntax[0] in ["addconst", "subconst"]:
        leftvalue = evaluate_remove_for_with(syntax[1], specs, vardict)

        if leftvalue[0] == "baseconst":
            return (leftvalue[0], leftvalue[1] + syntax[2] * {"add":+1,"sub":-1}[syntax[0][:-5]])
        else:
            raise Exception("Free variables remain in static calculation.")

def evaluate_remove_trivials(syntax):
    if syntax[0] in ["conjunction", "disjunction"]:
        is_trivial = False
        result     = syntax[:1]
        for s in [partial_evaluator(s, evaluate_remove_trivials) for s in syntax[1:]]:
            if s[0] == {"con":"false","dis":"true"}[syntax[0][:3]]:
                is_trivial = True
            elif s[0] != {"con":"true","dis":"false"}[syntax[0][:3]]:
                result = result + (s,)
        if is_trivial:
            return ({"con":"false","dis":"true"}[syntax[0][:3]],)
        elif len(result) == 1:
            return ({"con":"true","dis":"false"}[syntax[0][:3]],)
        else:
            return result if len(result) > 2 else result[1]

def evaluate_flatten_connectives(syntax):
    if syntax[0] in ["conjunction", "disjunction"]:
        result = syntax[:1]

        for child in (partial_evaluator(s, evaluate_flatten_connectives) for s in syntax[1:]):
            if child[0] == syntax[0]:
                result = result + child[1:]
            else:
                result = result + (child,)
        return result

def optimize_delay_aliases(syntax, slotlist):
    if syntax[0] == "conjunction":
        aliases = [s[1][1:] for s in syntax[1:] if s[0] == "atom" and s[1][0] == "ConstraintSame"]
        aliases = [(generate_cpp_slot(a),generate_cpp_slot(b)) for a,b in aliases]
        sys.stderr.write(str(aliases)+"\n")

def indent_code(prefix, code):
    current_indent = 0
    while code[:1] == " " and current_indent < len(prefix):
        code = code[1:]
        current_indent += 1

    return prefix + code.replace("\n", "\n"+" "*(len(prefix) - current_indent))

def generate_cpp_type(syntax):
    if syntax[0] in ["conjunction", "disjunction"]:
        return {"conjunction":"ConstraintAnd", "disjunction":"ConstraintOr"}[syntax[0]]
    elif syntax[0] == "atom":
        return syntax[1][0]
    raise Exception("Error, \"" + syntax[0] + "\" is not allowed in type generator.")

def generate_cpp_slot(syntax):
    if syntax[0] == "slotbase":
        return syntax[1]
    elif syntax[0] == "slotmember":
        return generate_cpp_slot(syntax[1]) + "." + syntax[2]
    elif syntax[0] == "slotindex" and syntax[2][0] in ["baseconst", "basevar"]:
        return generate_cpp_slot(syntax[1]) + "[" + str(syntax[2][1]) + "]"
    raise Exception("Error, \"" + syntax[0] + "\" is not allowed in single slot.")

def generate_cpp_slotlist(syntax):
    if syntax[0] == "slotbase":
        return (syntax[1],)
    elif syntax[0] == "slotmember":
        return tuple(prefix+"."+syntax[2] for prefix in generate_cpp_slotlist(syntax[1]))
    elif syntax[0] == "slotindex" and syntax[2][0] in ["baseconst", "basevar"]:
        return tuple(prefix+"["+str(syntax[2][1])+"]" for prefix in generate_cpp_slotlist(syntax[1]))
    elif syntax[0] == "slotrange" and syntax[2][0] == "baseconst" and syntax[3][0] == "baseconst":
        return tuple(prefix+"["+str(i)+"]" for prefix in generate_cpp_slotlist(syntax[1]) for i in range(syntax[2][1], syntax[3][1]))
    elif syntax[0] == "slottuple":
        return sum((generate_cpp_slotlist(s) for s in syntax[1:]), ())
    raise Exception("Error, \"" + syntax[0] + "\" is not allowed in slot list.")

def code_generation_core(syntax, counter):
    if syntax[0] == "atom":
        if len(syntax[1]) > 2:
            if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
                code = "auto atom{} = make_shared<Backend{}>(array<unsigned,3>{{{{0,1,1}}}}, wrap);\n".format(counter[0], syntax[1][0][10:])
            elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
                code = "auto atom{} = make_shared<Backend{}>(array<unsigned,3>{{{{1,1,1}}}}, wrap);\n".format(counter[0], syntax[1][0][10:])
            else:
                code = "auto atom{} = make_shared<Backend{}>(wrap);\n".format(counter[0], syntax[1][0][10:])
            counter[0] += 1
        else:
            code = ""

        slots = [generate_cpp_slot(s) for s in syntax[1][1:2]+syntax[1][3:1:-1]]

        if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
            result = {slot:("MultiVectorSelector<Backend{},{}>".format(syntax[1][0][10:], i+1), "atom{}, 0".format(counter[0]-1)) for i,slot in enumerate(slots)}
        elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
            result = {slot:("MultiVectorSelector<Backend{},{}>".format(syntax[1][0][10:], i), "atom{}, 0".format(counter[0]-1)) for i,slot in enumerate(slots)}
        elif len(syntax[1]) == 2:
            result = {slot:("Backend{}".format(syntax[1][0][10:]), "wrap") for i,slot in enumerate(slots)}
        else:
            result = {slot:("ScalarSelector<Backend{},{}>".format(syntax[1][0][10:], i), "atom{}".format(counter[0]-1)) for i,slot in enumerate(slots)}

        return slots,result,code

    elif syntax[0] == "GeneralizedDominance":
        slotlists   = [generate_cpp_slotlist(s) for s in syntax[1:2]+syntax[3:1:-1]]
        code        = "auto atom{} = make_shared<BackendPDGDominate>(array<unsigned,3>{{{{{},{},{}}}}}, wrap);\n".format(counter[0], len(slotlists[0]), len(slotlists[1]), len(slotlists[2]))
        slots       = [slot for slotlist in slotlists for slot in slotlist]
        slots       = [slot for n,slot in enumerate(slots) if slot not in slots[:n]]
        result      = {slot:("MultiVectorSelector<BackendPDGDominate,{}>".format(j), "atom{}, {}".format(counter[0], i))
                       for j,slotlist in enumerate(slotlists) for i,slot in enumerate(slotlist)}
        counter[0] += 1
        return slots,result,code

    elif syntax[0] in ["conjunction", "disjunction"]:
        part_results = [code_generation_core(s, counter) for s in syntax[1:]]
        code         = "".join([part[2] for part in part_results])
        slots        = [slot for part in part_results for slot in part[0]]
        slots        = [slot for n,slot in enumerate(slots) if slot not in slots[:n]]
        result       = {slot:[part[1][slot] for part in part_results if slot in part[1]] for slot in slots}

        if syntax[0] == "conjunction":
            result = {slot:result[slot][0] if len(result[slot]) == 1 else
                           ("BackendAnd<"+",".join(a for a,b in result[slot])+">", ", ".join("{"+b+"}" for a,b in result[slot]))
                      for slot in slots}

        if syntax[0] == "disjunction":
            choices        = max([0]+[len(result[slot]) for slot in slots])
            templateparams = ",".join("tuple<"+",".join(a for a,b in result[slot])+">" for slot in slots)
            classname      = "BackendOr<{},{}>".format(choices, templateparams)
            constructargs  = ", ".join("tuple<"+",".join(a[0] for a in result[slot])+">{"+", ".join("{"+a[1]+"}" for a in result[slot])+"}" for slot in slots)
            code          += "auto atom{} = make_shared<{}>({});\n".format(counter[0], classname, constructargs)
            result         = {slot:("ScalarSelector<{},{}>".format(classname, n), "atom{}".format(counter[0])) for n,slot in enumerate(slots)}
            counter[0]    += 1

        return slots,result,code

    elif syntax[0] == "collect":
        part_slots, part_result, code = code_generation_core(syntax[3], counter)

        local_slots_idx  = [i for i,slot in enumerate(part_slots) if "["+syntax[1]+"]"     in slot]
        global_slots_idx = [i for i,slot in enumerate(part_slots) if "["+syntax[1]+"]" not in slot]

        local_parts  = [part_result[part_slots[idx]] for idx in local_slots_idx]
        global_parts = [part_result[part_slots[idx]] for idx in global_slots_idx]

        code += "\n".join(["vector<unique_ptr<SolverAtom>> globals{};".format(counter[0])]
                         +["globals{}.emplace_back(unique_ptr<SolverAtom>(new {}({})));".format(counter[0], part[0], part[1]) for part in global_parts]
                         +["vector<unique_ptr<SolverAtom>> locals{};".format(counter[0])]
                         +["locals{}.emplace_back(unique_ptr<SolverAtom>(new {}({})));".format(counter[0], part[0], part[1]) for part in local_parts]
                         +["auto atom{} = make_shared<BackendCollect>(array<unsigned,2>{{{{{},{}}}}}, move(globals{}), move(locals{}));".format(counter[0], len(global_parts), len(local_parts), counter[0], counter[0])])+"\n"

        local_slots  = [part_slots[idx].replace("["+syntax[1]+"]", "["+str(n)+"]") for n in range(syntax[2]) for idx in local_slots_idx]
        global_slots = [part_slots[idx] for idx in global_slots_idx]

        result      = {slot:("MultiVectorSelector<BackendCollect,0>", "atom{}, {}".format(counter[0], i)) for i,slot in enumerate(global_slots)}
        result.update({slot:("MultiVectorSelector<BackendCollect,1>", "atom{}, {}".format(counter[0], i)) for i,slot in enumerate(local_slots)})
        counter[0] += 1

        return global_slots+local_slots, result, code

    raise Exception("Error, \"" + syntax[0] + "\" is not allowed in atoms collection.")

def generate_fast_cpp_specification(syntax, specs):
    constr = partial_evaluator(syntax[2], evaluate_remove_for_with, specs)
    constr = partial_evaluator(constr,    evaluate_remove_rename_rebase)
    constr = partial_evaluator(constr,    evaluate_remove_trivials)
    constr = partial_evaluator(constr,    evaluate_flatten_connectives)
#   constr = partial_evaluator(constr, optimize_delay_aliases, generate_cpp_slotlist(constr))

    slots, result, code = code_generation_core(constr, [0])

    return "\n".join(["vector<Solution> Detect{}(llvm::Function& function, unsigned max_solutions)".format(syntax[1])]
                    +["{"]
                    +["    FunctionWrap wrap(function);"]
                    +[indent_code("    ", code.rstrip())]
                    +["    vector<pair<string,unique_ptr<SolverAtom>>> constraint;"]
                    +["    constraint.emplace_back(\"{}\", unique_ptr<SolverAtom>(new {}({})));".format(slot, result[slot][0], result[slot][1]) for slot in slots]
                    +["    return Solution::Find(move(constraint), function, max_solutions);"]
                    +["}"])

def generate_cpp_code(syntax_list):
    includes  = ["IdiomSpecifications","BackendSpecializations", "BackendDirectClasses", "BackendSelectors", "Solution"]
    specs     = {spec[1] : spec[2] for spec in syntax_list}
    whitelist = ["Distributive", "HoistSelect", "AXPYn", "GEMM", "GEMV", "AXPY",
                 "DOT", "SPMV", "Reduction", "Histo", "Stencil", "StencilPlus"]

    return "\n".join(["#include \"llvm/Constraints/{}.hpp\"".format(s) for s in includes]
                    +["using namespace std;"]
                    +["#pragma GCC optimize (\"O0\")"]
                    +["#pragma clang optimize off"]
                    +[generate_fast_cpp_specification(syntax, specs) for syntax in syntax_list if syntax[1] in whitelist])

def print_syntax_tree(syntax, indent=0):
    if type(syntax) is str or type(syntax) is int:
        print("| "*indent+str(syntax))
    elif type(syntax) is tuple and type(syntax[0]) is str:
        print("| "*indent+str(syntax[0]))
        for s in syntax[1:]:
            print_syntax_tree(s, indent+1)
    elif type(syntax) is tuple:
        for s in syntax:
            print_syntax_tree(s, indent+1)

import sys

print(generate_cpp_code(parse(sys.stdin.read(), grammar)))
