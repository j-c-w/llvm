#!/usr/bin/pypy

whitelist = ["Distributive", "HoistSelect", "AXPYn", "GEMM", "GEMV", "AXPY",
             "DOT", "SPMV", "Reduction", "Histo", "Stencil", "StencilPlus", "Experiment", "SCoP", "IfBlock", "IfBlock2"]

grammar = "[('#', ['<specification>']), ('@233', ['Constraint']), ('@234', ['<@233>', '<s>']), ('@235', ['<@234>', '<constraint>']), ('specification', ['<@235>', 'End']), ('constraint', ['<grouping>']), ('constraint', ['<collect>']), ('constraint', ['<rename>']), ('constraint', ['<rebase>']), ('constraint', ['<atom>']), ('constraint', ['<GeneralizedDominance>']), ('constraint', ['<GeneralizedSame>']), ('@236', ['(']), ('@237', ['<@236>', '<constraint>']), ('constraint', ['<@237>', ')']), ('grouping', ['<conjunction>']), ('grouping', ['<disjunction>']), ('grouping', ['<inheritance>']), ('grouping', ['<forall>']), ('grouping', ['<forsome>']), ('grouping', ['<forone>']), ('grouping', ['<if>']), ('grouping', ['<default>']), ('inheritance', ['<rawinherit>']), ('inheritance', ['<arginherit>']), ('@238', ['include']), ('rawinherit', ['<@238>', '<s>']), ('@239', ['<rawinherit>', '(']), ('@240', ['<@239>', '<s>']), ('@241', ['<@240>', '=']), ('@arginheritbase', ['<@241>', '<calculation>']), ('@242', ['<@arginheritbase>', ',']), ('@243', ['<@242>', '<s>']), ('@244', ['<@243>', '=']), ('@arginheritcont', ['<@244>', '<calculation>']), ('@245', ['<@arginheritcont>', ',']), ('@246', ['<@245>', '<s>']), ('@247', ['<@246>', '=']), ('@arginheritcont', ['<@247>', '<calculation>']), ('arginherit', ['<@arginheritcont>', ')']), ('arginherit', ['<@arginheritbase>', ')']), ('@250', ['<@237>', 'and']), ('@conjunctionbase', ['<@250>', '<constraint>']), ('@251', ['<@conjunctionbase>', 'and']), ('@conjunctioncont', ['<@251>', '<constraint>']), ('@252', ['<@conjunctioncont>', 'and']), ('@conjunctioncont', ['<@252>', '<constraint>']), ('conjunction', ['<@conjunctioncont>', ')']), ('conjunction', ['<@conjunctionbase>', ')']), ('@255', ['<@237>', 'or']), ('@disjunctionbase', ['<@255>', '<constraint>']), ('@256', ['<@disjunctionbase>', 'or']), ('@disjunctioncont', ['<@256>', '<constraint>']), ('@257', ['<@disjunctioncont>', 'or']), ('@disjunctioncont', ['<@257>', '<constraint>']), ('disjunction', ['<@disjunctioncont>', ')']), ('disjunction', ['<@disjunctionbase>', ')']), ('@258', ['<constraint>', 'for']), ('@259', ['<@258>', 'all']), ('@260', ['<@259>', '<s>']), ('@261', ['<@260>', '=']), ('@262', ['<@261>', '<calculation>']), ('@263', ['<@262>', '..']), ('forall', ['<@263>', '<calculation>']), ('@265', ['<@258>', 'some']), ('@266', ['<@265>', '<s>']), ('@267', ['<@266>', '=']), ('@268', ['<@267>', '<calculation>']), ('@269', ['<@268>', '..']), ('forsome', ['<@269>', '<calculation>']), ('@271', ['<@258>', '<s>']), ('@272', ['<@271>', '=']), ('forone', ['<@272>', '<calculation>']), ('@276', ['<@272>', '<calculation>']), ('@277', ['<@276>', 'if']), ('@278', ['<@277>', 'not']), ('@279', ['<@278>', 'otherwise']), ('default', ['<@279>', 'specified']), ('@280', ['if']), ('@281', ['<@280>', '<calculation>']), ('@282', ['<@281>', '=']), ('@283', ['<@282>', '<calculation>']), ('@284', ['<@283>', 'then']), ('@285', ['<@284>', '<constraint>']), ('@286', ['<@285>', 'else']), ('@287', ['<@286>', '<constraint>']), ('if', ['<@287>', 'endif']), ('@288', ['collect']), ('@289', ['<@288>', '<s>']), ('@290', ['<@289>', '<n>']), ('collect', ['<@290>', '<constraint>']), ('@291', ['<grouping>', 'with']), ('@292', ['<@291>', '<slot>']), ('@293', ['<@292>', 'as']), ('@renamebase', ['<@293>', '<slot>']), ('@294', ['<@renamebase>', 'and']), ('@295', ['<@294>', '<slot>']), ('@296', ['<@295>', 'as']), ('@renamecont', ['<@296>', '<slot>']), ('@297', ['<@renamecont>', 'and']), ('@298', ['<@297>', '<slot>']), ('@299', ['<@298>', 'as']), ('@renamecont', ['<@299>', '<slot>']), ('rename', ['<@renamecont>']), ('rename', ['<@renamebase>']), ('@300', ['<grouping>', 'at']), ('rebase', ['<@300>', '<slot>']), ('@301', ['<rename>', 'at']), ('rebase', ['<@301>', '<slot>']), ('@302', ['{']), ('@303', ['<@302>', '<openslot>']), ('slot', ['<@303>', '}']), ('slot', ['<slottuple>']), ('openslot', ['<slotbase>']), ('openslot', ['<slotindex>']), ('openslot', ['<slotrange>']), ('openslot', ['<slotmember>']), ('openslot', ['<slottuple>']), ('slotbase', ['<s>']), ('@304', ['<openslot>', '[']), ('@305', ['<@304>', '<calculation>']), ('slotindex', ['<@305>', ']']), ('@308', ['<@305>', '..']), ('@309', ['<@308>', '<calculation>']), ('slotrange', ['<@309>', ']']), ('@310', ['<openslot>', '.']), ('slotmember', ['<@310>', '<s>']), ('@slottuplebase', ['<@303>', ',']), ('@313', ['<@slottuplebase>', '<openslot>']), ('@slottuplecont', ['<@313>', ',']), ('@314', ['<@slottuplecont>', '<openslot>']), ('@slottuplecont', ['<@314>', ',']), ('slottuple', ['<@314>', '}']), ('slottuple', ['<@313>', '}']), ('calculation', ['<basevar>']), ('calculation', ['<baseconst>']), ('calculation', ['<addvar>']), ('calculation', ['<addconst>']), ('calculation', ['<subvar>']), ('calculation', ['<subconst>']), ('basevar', ['<s>']), ('baseconst', ['<n>']), ('@317', ['<calculation>', '+']), ('addvar', ['<@317>', '<s>']), ('addconst', ['<@317>', '<n>']), ('@319', ['<calculation>', '-']), ('subvar', ['<@319>', '<s>']), ('subconst', ['<@319>', '<n>']), ('atom', ['<ConstraintIntegerType>']), ('atom', ['<ConstraintFloatType>']), ('atom', ['<ConstraintVectorType>']), ('atom', ['<ConstraintPointerType>']), ('atom', ['<ConstraintIntZero>']), ('atom', ['<ConstraintFloatZero>']), ('atom', ['<ConstraintUnused>']), ('atom', ['<ConstraintNotNumericConstant>']), ('atom', ['<ConstraintConstant>']), ('atom', ['<ConstraintPreexecution>']), ('atom', ['<ConstraintArgument>']), ('atom', ['<ConstraintInstruction>']), ('atom', ['<ConstraintStoreInst>']), ('atom', ['<ConstraintLoadInst>']), ('atom', ['<ConstraintReturnInst>']), ('atom', ['<ConstraintBranchInst>']), ('atom', ['<ConstraintAddInst>']), ('atom', ['<ConstraintSubInst>']), ('atom', ['<ConstraintMulInst>']), ('atom', ['<ConstraintFAddInst>']), ('atom', ['<ConstraintFSubInst>']), ('atom', ['<ConstraintFMulInst>']), ('atom', ['<ConstraintFDivInst>']), ('atom', ['<ConstraintBitOrInst>']), ('atom', ['<ConstraintBitAndInst>']), ('atom', ['<ConstraintLShiftInst>']), ('atom', ['<ConstraintSelectInst>']), ('atom', ['<ConstraintSExtInst>']), ('atom', ['<ConstraintZExtInst>']), ('atom', ['<ConstraintGEPInst>']), ('atom', ['<ConstraintICmpInst>']), ('atom', ['<ConstraintCallInst>']), ('atom', ['<ConstraintShufflevectorInst>']), ('atom', ['<ConstraintInsertelementInst>']), ('atom', ['<ConstraintSame>']), ('atom', ['<ConstraintDistinct>']), ('atom', ['<ConstraintDFGEdge>']), ('atom', ['<ConstraintCFGEdge>']), ('atom', ['<ConstraintCDGEdge>']), ('atom', ['<ConstraintPDGEdge>']), ('atom', ['<ConstraintFirstOperand>']), ('atom', ['<ConstraintSecondOperand>']), ('atom', ['<ConstraintThirdOperand>']), ('atom', ['<ConstraintFourthOperand>']), ('atom', ['<ConstraintFirstSuccessor>']), ('atom', ['<ConstraintSecondSuccessor>']), ('atom', ['<ConstraintThirdSuccessor>']), ('atom', ['<ConstraintFourthSuccessor>']), ('atom', ['<ConstraintDFGDominate>']), ('atom', ['<ConstraintDFGPostdom>']), ('atom', ['<ConstraintDFGDominateStrict>']), ('atom', ['<ConstraintDFGPostdomStrict>']), ('atom', ['<ConstraintCFGDominate>']), ('atom', ['<ConstraintCFGPostdom>']), ('atom', ['<ConstraintCFGDominateStrict>']), ('atom', ['<ConstraintCFGPostdomStrict>']), ('atom', ['<ConstraintPDGDominate>']), ('atom', ['<ConstraintPDGPostdom>']), ('atom', ['<ConstraintPDGDominateStrict>']), ('atom', ['<ConstraintPDGPostdomStrict>']), ('atom', ['<ConstraintDFGNotDominate>']), ('atom', ['<ConstraintDFGNotPostdom>']), ('atom', ['<ConstraintDFGNotDominateStrict>']), ('atom', ['<ConstraintDFGNotPostdomStrict>']), ('atom', ['<ConstraintCFGNotDominate>']), ('atom', ['<ConstraintCFGNotPostdom>']), ('atom', ['<ConstraintCFGNotDominateStrict>']), ('atom', ['<ConstraintCFGNotPostdomStrict>']), ('atom', ['<ConstraintPDGNotDominate>']), ('atom', ['<ConstraintPDGNotPostdom>']), ('atom', ['<ConstraintPDGNotDominateStrict>']), ('atom', ['<ConstraintPDGNotPostdomStrict>']), ('atom', ['<ConstraintIncomingValue>']), ('atom', ['<ConstraintDFGBlocked>']), ('atom', ['<ConstraintCFGBlocked>']), ('atom', ['<ConstraintPDGBlocked>']), ('atom', ['<ConstraintFunctionAttribute>']), ('@321', ['<slot>', 'is']), ('@322', ['<@321>', 'an']), ('ConstraintIntegerType', ['<@322>', 'integer']), ('@324', ['<@321>', 'a']), ('ConstraintFloatType', ['<@324>', 'float']), ('ConstraintVectorType', ['<@324>', 'vector']), ('ConstraintPointerType', ['<@324>', 'pointer']), ('@330', ['<@321>', 'integer']), ('ConstraintIntZero', ['<@330>', 'zero']), ('@332', ['<@321>', 'floating']), ('@333', ['<@332>', 'point']), ('ConstraintFloatZero', ['<@333>', 'zero']), ('ConstraintUnused', ['<@321>', 'unused']), ('@336', ['<@321>', 'not']), ('@337', ['<@336>', 'a']), ('@338', ['<@337>', 'numeric']), ('ConstraintNotNumericConstant', ['<@338>', 'constant']), ('ConstraintConstant', ['<@324>', 'constant']), ('ConstraintPreexecution', ['<@321>', 'preexecution']), ('ConstraintArgument', ['<@322>', 'argument']), ('ConstraintInstruction', ['<@322>', 'instruction']), ('@347', ['<@321>', 'store']), ('ConstraintStoreInst', ['<@347>', 'instruction']), ('@349', ['<@321>', 'load']), ('ConstraintLoadInst', ['<@349>', 'instruction']), ('@351', ['<@321>', 'return']), ('ConstraintReturnInst', ['<@351>', 'instruction']), ('@353', ['<@321>', 'branch']), ('ConstraintBranchInst', ['<@353>', 'instruction']), ('@355', ['<@321>', 'add']), ('ConstraintAddInst', ['<@355>', 'instruction']), ('@357', ['<@321>', 'sub']), ('ConstraintSubInst', ['<@357>', 'instruction']), ('@359', ['<@321>', 'mul']), ('ConstraintMulInst', ['<@359>', 'instruction']), ('@361', ['<@321>', 'fadd']), ('ConstraintFAddInst', ['<@361>', 'instruction']), ('@363', ['<@321>', 'fsub']), ('ConstraintFSubInst', ['<@363>', 'instruction']), ('@365', ['<@321>', 'fmul']), ('ConstraintFMulInst', ['<@365>', 'instruction']), ('@367', ['<@321>', 'fdiv']), ('ConstraintFDivInst', ['<@367>', 'instruction']), ('@369', ['<@321>', 'bitor']), ('ConstraintBitOrInst', ['<@369>', 'instruction']), ('@371', ['<@321>', 'bitand']), ('ConstraintBitAndInst', ['<@371>', 'instruction']), ('@373', ['<@321>', 'lshift']), ('ConstraintLShiftInst', ['<@373>', 'instruction']), ('@375', ['<@321>', 'select']), ('ConstraintSelectInst', ['<@375>', 'instruction']), ('@377', ['<@321>', 'sext']), ('ConstraintSExtInst', ['<@377>', 'instruction']), ('@379', ['<@321>', 'zext']), ('ConstraintZExtInst', ['<@379>', 'instruction']), ('@381', ['<@321>', 'gep']), ('ConstraintGEPInst', ['<@381>', 'instruction']), ('@383', ['<@321>', 'icmp']), ('ConstraintICmpInst', ['<@383>', 'instruction']), ('@385', ['<@321>', 'call']), ('ConstraintCallInst', ['<@385>', 'instruction']), ('@387', ['<@321>', 'shufflevector']), ('ConstraintShufflevectorInst', ['<@387>', 'instruction']), ('@389', ['<@321>', 'insertelement']), ('ConstraintInsertelementInst', ['<@389>', 'instruction']), ('@391', ['<@321>', 'the']), ('@392', ['<@391>', 'same']), ('@393', ['<@392>', 'as']), ('ConstraintSame', ['<@393>', '<slot>']), ('@396', ['<@336>', 'the']), ('@397', ['<@396>', 'same']), ('@398', ['<@397>', 'as']), ('ConstraintDistinct', ['<@398>', '<slot>']), ('@399', ['<slot>', 'has']), ('@400', ['<@399>', 'data']), ('@401', ['<@400>', 'flow']), ('@402', ['<@401>', 'to']), ('ConstraintDFGEdge', ['<@402>', '<slot>']), ('@404', ['<@399>', 'control']), ('@405', ['<@404>', 'flow']), ('@406', ['<@405>', 'to']), ('ConstraintCFGEdge', ['<@406>', '<slot>']), ('@409', ['<@404>', 'dominance']), ('@410', ['<@409>', 'to']), ('ConstraintCDGEdge', ['<@410>', '<slot>']), ('@412', ['<@399>', 'dependence']), ('@413', ['<@412>', 'edge']), ('@414', ['<@413>', 'to']), ('ConstraintPDGEdge', ['<@414>', '<slot>']), ('@416', ['<@321>', 'first']), ('@417', ['<@416>', 'argument']), ('@418', ['<@417>', 'of']), ('ConstraintFirstOperand', ['<@418>', '<slot>']), ('@420', ['<@321>', 'second']), ('@421', ['<@420>', 'argument']), ('@422', ['<@421>', 'of']), ('ConstraintSecondOperand', ['<@422>', '<slot>']), ('@424', ['<@321>', 'third']), ('@425', ['<@424>', 'argument']), ('@426', ['<@425>', 'of']), ('ConstraintThirdOperand', ['<@426>', '<slot>']), ('@428', ['<@321>', 'fourth']), ('@429', ['<@428>', 'argument']), ('@430', ['<@429>', 'of']), ('ConstraintFourthOperand', ['<@430>', '<slot>']), ('@433', ['<@416>', 'successor']), ('@434', ['<@433>', 'of']), ('ConstraintFirstSuccessor', ['<@434>', '<slot>']), ('@437', ['<@420>', 'successor']), ('@438', ['<@437>', 'of']), ('ConstraintSecondSuccessor', ['<@438>', '<slot>']), ('@441', ['<@424>', 'successor']), ('@442', ['<@441>', 'of']), ('ConstraintThirdSuccessor', ['<@442>', '<slot>']), ('@445', ['<@428>', 'successor']), ('@446', ['<@445>', 'of']), ('ConstraintFourthSuccessor', ['<@446>', '<slot>']), ('@447', ['<slot>', 'data']), ('@448', ['<@447>', 'flow']), ('@449', ['<@448>', 'dominates']), ('ConstraintDFGDominate', ['<@449>', '<slot>']), ('@452', ['<@448>', 'post']), ('@453', ['<@452>', 'dominates']), ('ConstraintDFGPostdom', ['<@453>', '<slot>']), ('@454', ['<slot>', 'strictly']), ('@455', ['<@454>', 'data']), ('@456', ['<@455>', 'flow']), ('@457', ['<@456>', 'dominates']), ('ConstraintDFGDominateStrict', ['<@457>', '<slot>']), ('@461', ['<@456>', 'post']), ('@462', ['<@461>', 'dominates']), ('ConstraintDFGPostdomStrict', ['<@462>', '<slot>']), ('@463', ['<slot>', 'control']), ('@464', ['<@463>', 'flow']), ('@465', ['<@464>', 'dominates']), ('ConstraintCFGDominate', ['<@465>', '<slot>']), ('@468', ['<@464>', 'post']), ('@469', ['<@468>', 'dominates']), ('ConstraintCFGPostdom', ['<@469>', '<slot>']), ('@471', ['<@454>', 'control']), ('@472', ['<@471>', 'flow']), ('@473', ['<@472>', 'dominates']), ('ConstraintCFGDominateStrict', ['<@473>', '<slot>']), ('@477', ['<@472>', 'post']), ('@478', ['<@477>', 'dominates']), ('ConstraintCFGPostdomStrict', ['<@478>', '<slot>']), ('@479', ['<slot>', 'dominates']), ('ConstraintPDGDominate', ['<@479>', '<slot>']), ('@480', ['<slot>', 'post']), ('@481', ['<@480>', 'dominates']), ('ConstraintPDGPostdom', ['<@481>', '<slot>']), ('@483', ['<@454>', 'dominates']), ('ConstraintPDGDominateStrict', ['<@483>', '<slot>']), ('@485', ['<@454>', 'post']), ('@486', ['<@485>', 'dominates']), ('ConstraintPDGPostdomStrict', ['<@486>', '<slot>']), ('@487', ['<slot>', 'does']), ('@488', ['<@487>', 'not']), ('@489', ['<@488>', 'data']), ('@490', ['<@489>', 'flow']), ('@491', ['<@490>', 'dominate']), ('ConstraintDFGNotDominate', ['<@491>', '<slot>']), ('@496', ['<@490>', 'post']), ('@497', ['<@496>', 'dominate']), ('ConstraintDFGNotPostdom', ['<@497>', '<slot>']), ('@500', ['<@488>', 'strictly']), ('@501', ['<@500>', 'data']), ('@502', ['<@501>', 'flow']), ('@503', ['<@502>', 'dominate']), ('ConstraintDFGNotDominateStrict', ['<@503>', '<slot>']), ('@509', ['<@502>', 'post']), ('@510', ['<@509>', 'dominate']), ('ConstraintDFGNotPostdomStrict', ['<@510>', '<slot>']), ('@513', ['<@488>', 'control']), ('@514', ['<@513>', 'flow']), ('@515', ['<@514>', 'dominate']), ('ConstraintCFGNotDominate', ['<@515>', '<slot>']), ('@520', ['<@514>', 'post']), ('@521', ['<@520>', 'dominate']), ('ConstraintCFGNotPostdom', ['<@521>', '<slot>']), ('@525', ['<@500>', 'control']), ('@526', ['<@525>', 'flow']), ('@527', ['<@526>', 'dominate']), ('ConstraintCFGNotDominateStrict', ['<@527>', '<slot>']), ('@533', ['<@526>', 'post']), ('@534', ['<@533>', 'dominate']), ('ConstraintCFGNotPostdomStrict', ['<@534>', '<slot>']), ('@537', ['<@488>', 'dominate']), ('ConstraintPDGNotDominate', ['<@537>', '<slot>']), ('@540', ['<@488>', 'post']), ('@541', ['<@540>', 'dominate']), ('ConstraintPDGNotPostdom', ['<@541>', '<slot>']), ('@545', ['<@500>', 'dominate']), ('ConstraintPDGNotDominateStrict', ['<@545>', '<slot>']), ('@549', ['<@500>', 'post']), ('@550', ['<@549>', 'dominate']), ('ConstraintPDGNotPostdomStrict', ['<@550>', '<slot>']), ('@551', ['<slot>', 'reaches']), ('@552', ['<@551>', 'phi']), ('@553', ['<@552>', 'node']), ('@554', ['<@553>', '<slot>']), ('@555', ['<@554>', 'from']), ('ConstraintIncomingValue', ['<@555>', '<slot>']), ('@556', ['all']), ('@557', ['<@556>', 'data']), ('@558', ['<@557>', 'flow']), ('@559', ['<@558>', 'from']), ('@560', ['<@559>', '<slot>']), ('@561', ['<@560>', 'to']), ('@562', ['<@561>', '<slot>']), ('@563', ['<@562>', 'passes']), ('@564', ['<@563>', 'through']), ('ConstraintDFGBlocked', ['<@564>', '<slot>']), ('@566', ['<@556>', 'control']), ('@567', ['<@566>', 'flow']), ('@568', ['<@567>', 'from']), ('@569', ['<@568>', '<slot>']), ('@570', ['<@569>', 'to']), ('@571', ['<@570>', '<slot>']), ('@572', ['<@571>', 'passes']), ('@573', ['<@572>', 'through']), ('ConstraintCFGBlocked', ['<@573>', '<slot>']), ('@575', ['<@556>', 'flow']), ('@576', ['<@575>', 'from']), ('@577', ['<@576>', '<slot>']), ('@578', ['<@577>', 'to']), ('@579', ['<@578>', '<slot>']), ('@580', ['<@579>', 'passes']), ('@581', ['<@580>', 'through']), ('ConstraintPDGBlocked', ['<@581>', '<slot>']), ('@583', ['<@399>', 'attribute']), ('ConstraintFunctionAttribute', ['<@583>', 'pure']), ('@587', ['<@392>', 'set']), ('@588', ['<@587>', 'as']), ('GeneralizedSame', ['<@588>', '<slot>']), ('@593', ['<@577>', 'or']), ('@594', ['<@593>', 'any']), ('@595', ['<@594>', 'origin']), ('@596', ['<@595>', 'to']), ('@597', ['<@596>', 'any']), ('@598', ['<@597>', 'of']), ('@599', ['<@598>', '<slot>']), ('@600', ['<@599>', 'passes']), ('@601', ['<@600>', 'through']), ('@602', ['<@601>', 'at']), ('@603', ['<@602>', 'least']), ('@604', ['<@603>', 'one']), ('@605', ['<@604>', 'of']), ('GeneralizedDominance', ['<@605>', '<slot>'])]"

def token_match(token, rule):
    if rule.startswith("<") and rule.endswith(">"):
        if rule == "<s>":   return type(token) is str
        elif rule == "<n>": return type(token) is int
        else:               return type(token) is tuple and token[0] == rule[1:-1]
    else:                   return type(token) is str and token == rule

def list_possibles(stack, grammar, lookahead_rules):
    possibles    = []
    not_dead_end = all(type(s) == tuple and s[0]=="#" for s in stack)

    for name, rule in grammar:
        if len(rule) == 1 and lookahead_rules[name]:
            if len(stack) >=2 and token_match(stack[-1], rule[0]) and stack[-2][0] in lookahead_rules[name]:
                new_element = (name,) + ((stack[-1],) if rule[-1][:1]+rule[-1][-1:] == "<>" else tuple())
                possibles += list_possibles(stack[:-1] + [new_element], grammar, lookahead_rules)
        elif len(rule) == 1:
            if token_match(stack[-1], rule[0]):
                new_element = (name,) + ((stack[-1],) if rule[-1][:1]+rule[-1][-1:] == "<>" else tuple())
                possibles += list_possibles(stack[:-1] + [new_element], grammar, lookahead_rules)
        elif len(rule) == 2:
            if len(stack) >=2 and stack[-2][0] == rule[0][1:-1] and token_match(stack[-1], rule[1]):
                new_element = (name,stack[-2]) + ((stack[-1],) if rule[-1][:1]+rule[-1][-1:] == "<>" else tuple())
                possibles += list_possibles(stack[:-2] + [new_element], grammar, lookahead_rules)
            if not not_dead_end and token_match(stack[-1], rule[0]):
                not_dead_end = True

    return possibles + [stack] if not_dead_end else possibles

def postprocess(syntax):
    invisibles = ["#", "constraint", "slot", "openslot", "calculation", "rawinherit", "arginherit", "grouping"]

    if type(syntax) == tuple:
        result = tuple(s for a in map(postprocess, syntax) for s in a)
        return result[1:] if result[0] in invisibles or "@" in result[0] else (result,)
    else:
        return (syntax,)

def iterate_to_fixpoint(function, start, *extra):
    start, result = None, start
    while result != start:
        start, result = result, function(result, *extra)
    return result

def collect_aliases(aliases, grammar):
    return aliases | {"<{}>".format(name) for name,rule in grammar if rule[0] in aliases}

def parse(code, grammartext):
    grammar=eval(grammartext)

    aliases = {name:iterate_to_fixpoint(collect_aliases, {"<{}>".format(name)}, grammar) for name,rules in grammar}

    lookahead_rules = {name:set() for name,rule in grammar}
    for name,rule in grammar:
        if rule in [["<s>"], ["<n>"]]:
            precursors = [rule[i][1:-1] for _,rule in grammar for i,b in enumerate(rule[1:]) if b in aliases[name]]
            lookahead_rules[name] = lookahead_rules[name]|set(precursors)

    code = " ".join(line.split("#")[0] for line in code.split("\n"))
    code = "".join(" "+c+" " if c in "(){}[]=.+-," else c for c in code).replace(".  .", "..")

    possibles = [[]]

    debug_tokens = []
    for token in [int(token) if token.isdigit() else token for token in code.split() if token]:
        possibles = sum([list_possibles(pos+[token], grammar, lookahead_rules) for pos in possibles], [])

        debug_tokens = debug_tokens[-7:]+[str(token)]

        if not possibles:
            raise Exception("Syntax error at \""+" ".join(debug_tokens)+"\"")

    possibles = [pos for pos in possibles if all(type(s) == tuple and s[0] == "#" for s in pos)]

    if len(possibles) == 1:
        return tuple(s for a in map(postprocess, possibles[0]) for s in a)

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

def evaluate_bisect_connectives(syntax):
    if syntax[0] in ["conjunction", "disjunction"]:
        if len(syntax[1:]) == 0:
            return ({"con":"false","dis":"true"}[syntax[0][:3]],)
        elif len(syntax[1:]) == 1:
            return partial_evaluator(syntax[1], evaluate_bisect_connectives)
        elif len(syntax[1:]) == 2:
            return syntax[:1]+(partial_evaluator(syntax[1], evaluate_bisect_connectives),
                               partial_evaluator(syntax[2], evaluate_bisect_connectives))
        else:
            return syntax[:1]+(partial_evaluator(syntax[1], evaluate_bisect_connectives),
                               partial_evaluator(syntax[:1]+syntax[2:], evaluate_bisect_connectives))

def replace_variables(syntax, replaces):
    if syntax[0] in ["slotbase", "slotmember", "slotindex"]:
        return replaces[syntax] if syntax in replaces else syntax

def optimize_delay_aliases(syntax, slotlist):
    if syntax[0] == "conjunction":
        replaces = {}
        for a,b in (s[1][1:] for s in syntax[1:] if s[0] == "atom" and s[1][0] == "ConstraintSame"):
            aflat, bflat = generate_cpp_slot(a), generate_cpp_slot(b)
            if aflat in slotlist and bflat in slotlist:
                if slotlist.index(aflat) < slotlist.index(bflat):
                    replaces[b] = a
                if slotlist.index(bflat) < slotlist.index(aflat):
                    replaces[a] = b

        return syntax[:1] + tuple(s if s[0] == "atom" and s[1][0] == "ConstraintSame" else
                                  partial_evaluator(
                                  partial_evaluator(s, replace_variables,      replaces),
                                                       optimize_delay_aliases, slotlist) for s in syntax[1:])

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

def getatom(counter, typename):
    if typename in counter:
        result = "atom{}_[{}]".format(counter[typename][0], counter[typename][1])
        counter[typename] = (counter[typename][0], counter[typename][1]+1)
    else:
        result = "atom{}_[{}]".format(len(counter), 0)
        counter[typename] = (len(counter), 1)
    return result;

def prune_types(slots,result,code,counter):
    for slot in slots:
        if result[slot][0].startswith("remove_reference"):
            atom  = getatom(counter, "IndirSolverAtom")
            code += "{} = {{{}}};\n".format(atom, result[slot][1])
            result[slot] = ("IndirSolverAtom", atom)
    return slots,result,code

def code_generation_core(syntax, counter):
    if syntax[0] == "atom":
        classname = "Backend{}".format(syntax[1][0][10:])
        if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
            atom = getatom(counter, "my_shared_ptr<{}>".format(classname))
            code = "{} = {{{{0,1,1}}, wrap}};\n".format(atom)
        elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
            atom = getatom(counter, "my_shared_ptr<{}>".format(classname))
            code = "{} = {{{{1,1,1}}, wrap}};\n".format(atom)
        elif len(syntax[1]) > 2:
            atom = getatom(counter, "my_shared_ptr<{}>".format(classname))
            code = "{} = {{wrap}};\n".format(atom)
        else:
            if classname not in counter:
                atom = getatom(counter, classname)
                code = "{} = {{wrap}};\n".format(atom)
            else:
                atom = "atom{}_[0]".format(counter[classname][0])
                code = ""

        slots = [generate_cpp_slot(s) for s in syntax[1][1:2]+syntax[1][3:1:-1]]

        if syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] in ["Dominat","Postdom"]:
            result = {slot:("MultiVectorSelector<Backend{},{}>".format(syntax[1][0][10:], i+1), "{}, <[0]>".format(atom)) for i,slot in enumerate(slots)}
        elif syntax[1][0][10:13] in ["DFG","CFG","PDG"] and syntax[1][0][13:20] == "Blocked":
            result = {slot:("MultiVectorSelector<Backend{},{}>".format(syntax[1][0][10:], i), "{}, <[0]>".format(atom)) for i,slot in enumerate(slots)}
        elif len(syntax[1]) == 2:
            result = {slot:("Backend{}".format(syntax[1][0][10:]), atom) for i,slot in enumerate(slots)}
        else:
            result = {slot:("ScalarSelector<Backend{},{}>".format(syntax[1][0][10:], i), atom) for i,slot in enumerate(slots)}

        return slots,result,code

    elif syntax[0] == "GeneralizedDominance":
        atom        = getatom(counter, "my_shared_ptr<BackendPDGDominate>")
        slotlists   = [generate_cpp_slotlist(s) for s in syntax[1:2]+syntax[3:1:-1]]
        code        = "{} = {{{{{},{},{}}}, wrap}};\n".format(atom, len(slotlists[0]), len(slotlists[1]), len(slotlists[2]))
        slots       = [slot for slotlist in slotlists for slot in slotlist]
        slots       = [slot for n,slot in enumerate(slots) if slot not in slots[:n]]
        result      = {slot:("MultiVectorSelector<BackendPDGDominate,{}>".format(j), "{}, <[{}]>".format(atom, i))
                       for j,slotlist in enumerate(slotlists) for i,slot in enumerate(slotlist)}
        return slots,result,code

    elif syntax[0] == "GeneralizedSame":
        atom        = getatom(counter, "my_shared_ptr<BackendSameSets>")
        slotlists   = [generate_cpp_slotlist(s) for s in syntax[1:2]+syntax[3:1:-1]]
        code        = "{} = {{{{{}}}}};\n".format(atom, len(slotlists[0]))
        slots       = [slot for slotlist in slotlists for slot in slotlist]
        slots       = [slot for n,slot in enumerate(slots) if slot not in slots[:n]]
        result      = {slot:("MultiVectorSelector<BackendSameSets,{}>".format(j), "{}, <[{}]>".format(atom, i))
                       for j,slotlist in enumerate(slotlists) for i,slot in enumerate(slotlist)}
        return slots,result,code

    elif syntax[0] in ["conjunction", "disjunction"]:
        part_results = [code_generation_core(s, counter) for s in syntax[1:]]
        code         = "".join([part[2] for part in part_results])
        slots        = [slot for part in part_results for slot in part[0]]
        slots        = [slot for n,slot in enumerate(slots) if slot not in slots[:n]]
        result       = {slot:[part[1][slot] for part in part_results if slot in part[1]] for slot in slots}

        if syntax[0] == "conjunction":
            for slot in slots:
                newresult = result[slot][0]

                for a,b in result[slot][1:]:
                    classname    = indent_code("BackendAnd<", newresult[0]+",\n"+a+">")
                    atom         = getatom(counter, classname)
                    code        += "{} = {{{}}};\n".format(atom, "{"+newresult[1]+"}, {"+b+"}")
                    classname    = "remove_reference<decltype({}[0])>::type".format(atom[:atom.index("[")])
                    newresult    = (classname, atom)

                result[slot] = newresult

        if syntax[0] == "disjunction":
            choices       = max([0]+[len(result[slot]) for slot in slots])
            sliced_result = [{slot:result[slot][i] for slot in slots} for i in range(choices)]

            newresult = sliced_result[0]

            for slic in sliced_result[1:]:
                shared_data  = getatom(counter, "my_shared_ptr<pair<unsigned,unsigned>>")
                code        += "{} = {{0,0}};\n".format(shared_data);

                for slot in slots:
                    classname  = indent_code("BackendOr<", newresult[slot][0]+",\n"+slic[slot][0]+">")
                    atom       = getatom(counter, classname)
                    code      += "{} = {{{}}};\n".format(atom, "{"+newresult[slot][1]+"}, {"+slic[slot][1]+"}, {"+shared_data+"}")
                    classname  = "remove_reference<decltype({}[0])>::type".format(atom[:atom.index("[")])
                    newresult[slot] = (classname, atom)

            result = newresult

        return prune_types(slots,result,code,counter)

    elif syntax[0] == "collect":
        atom = getatom(counter, "my_shared_ptr<BackendCollect>")

        part_slots, part_result, code = code_generation_core(syntax[3], counter)

        local_slots_idx  = [i for i,slot in enumerate(part_slots) if "["+syntax[1]+"]"     in slot]
        global_slots_idx = [i for i,slot in enumerate(part_slots) if "["+syntax[1]+"]" not in slot]

        local_parts  = [part_result[part_slots[idx]] for idx in local_slots_idx]
        global_parts = [part_result[part_slots[idx]] for idx in global_slots_idx]

        mergedatom = atom[4:atom.index("[")] + atom[atom.index("[")+1:atom.index("]")] + "_"

        code += "\n".join(["vector<unique_ptr<SolverAtom>> globals{};".format(mergedatom)]
                         +["globals{}.emplace_back(unique_ptr<SolverAtom>(new {}({})));".format(mergedatom, part[0], part[1]) for part in global_parts]
                         +["vector<unique_ptr<SolverAtom>> locals{};".format(mergedatom)]
                         +["locals{}.emplace_back(unique_ptr<SolverAtom>(new {}({})));".format(mergedatom, part[0], part[1]) for part in local_parts]
                         +["{} = {{{{{{{},{}}}}}, move(globals{}), move(locals{})}};".format(atom, len(global_parts), len(local_parts), mergedatom, mergedatom)])+"\n"

        local_slots  = [part_slots[idx].replace("["+syntax[1]+"]", "["+str(n)+"]") for n in range(syntax[2]) for idx in local_slots_idx]
        global_slots = [part_slots[idx] for idx in global_slots_idx]

        result      = {slot:("MultiVectorSelector<BackendCollect,0>", "{}, <[{}]>".format(atom, i)) for i,slot in enumerate(global_slots)}
        result.update({slot:("MultiVectorSelector<BackendCollect,1>", "{}, <[{}]>".format(atom, i)) for i,slot in enumerate(local_slots)})

        return prune_types(global_slots+local_slots,result,code,counter)
#        return global_slots+local_slots, result, code

    raise Exception("Error, \"" + syntax[0] + "\" is not allowed in atoms collection.")

def postprocess_copyconstructions(code):
    atomic_def_lines = [line for line in code.split("\n") if line.startswith("atom")]
    code_atomic_defs = [(line,line.split("[")[0]+line.split(" = ")[1]) for line in atomic_def_lines]
    duplication_dict = {}

    for n,line in enumerate(code_atomic_defs):
        for oldline in code_atomic_defs[:n]:
            if oldline[1:] == line[1:]:
                duplication_dict[line[0]] = line[0].split(" = ")[0] + " = " + oldline[0].split(" = ")[0] + ";"
                break

    return "\n".join([duplication_dict[line] if line in duplication_dict else line for line in code.split("\n")])

def check_differences(first, second, third, fourth):
    diff1 = [y-x for line1,line2 in zip(first,second) for x,y in zip(line1,line2)]
    diff2 = [y-x for line1,line2 in zip(third,fourth) for x,y in zip(line1,line2)]

    return all([x==y for x,y in zip(diff1,diff2)])

# This does not actually check that indizes are a linear progression for more than two repetitions!
def loop_block(block, depth=0):
    if depth > 4: return block
    stripindizes = [line[::2] for line in block]
    onlyindizes  = [[int(n) for n in line[1::2]] for line in block]

    repeatstart  = 0
    repeatsize   = 1
    repeatamount = 1
    codesaved    = 0

    for n in range(len(stripindizes)):
        for m in range(1,1000):
            if stripindizes[n:n+m] == stripindizes[n+m:n+2*m]:
                newrepeatamount = 2
                for k in range(2,100):
                    if (stripindizes[n+(k-1)*m:n+k*m] == stripindizes[n+k*m:n+(k+1)*m] and
                        check_differences(onlyindizes[n+(k-2)*m:n+(k-1)*m], onlyindizes[n+(k-1)*m:n+(k+0)*m],
                                          onlyindizes[n+(k-1)*m:n+(k+0)*m], onlyindizes[n+(k+0)*m:n+(k+1)*m])):
                        newrepeatamount += 1
                    else:
                        break
                if (newrepeatamount-1)*m > codesaved:
                    repeatstart  = n
                    repeatsize   = m
                    repeatamount = newrepeatamount
                    codesaved    = (newrepeatamount-1)*m

    if codesaved:
        block_before = block[:repeatstart]
        block_after  = block[repeatstart+repeatamount*repeatsize:]

        modified_block = block[repeatstart:repeatstart+repeatsize]

        for k,line in enumerate(modified_block):
            for i in range(2,len(line),2):
                difference = int(block[repeatstart+repeatsize+k][i-1])-int(block[repeatstart+k][i-1])
                if difference > 1:
                    line[i] = "+{}*{}{}".format("ijklm"[depth], difference, line[i])
                elif difference < -1:
                    line[i] = "-{}*{}{}".format("ijklm"[depth], -difference, line[i])
                elif difference == 1:
                    line[i] = "+{}{}".format("ijklm"[depth], line[i])
                elif difference == -1:
                    line[i] = "-{}{}".format("ijklm"[depth], line[i])

        return (loop_block(block_before, depth)
               +[["for(unsigned {0} = 0; {0} < {1}; {0}++) {{".format("ijklm"[depth], repeatamount)]]
               +[["    "+line[0]]+line[1:] for line in loop_block(modified_block, depth+1)]
               +[["}"]]
               +loop_block(block_after, depth))

    return block

def postprocess_add_loops_one_block(block):
    block = [[part for word in line.split(']') for part in word.split('[')] for line in block]
    block = [line if len(line) <= 1 else [line[0]+"["]+line[1:-1]+["]"+line[-1]] for line in block]
    for line in block:
        if len(line) > 1:
            line[2:-2:2] = ["]"+e+"[" for e in line[2:-2:2]]
    block = loop_block(block)
    return ["".join(line) for line in block]

def postprocess_add_loops(code):
    grouped = itertools.groupby(code.split('\n'), (lambda x: x.startswith("atom")))
    grouped = [postprocess_add_loops_one_block(list(b)) if a else list(b) for a,b in grouped]
    return "\n".join(line for group in grouped for line in group)

def generate_fast_cpp_specification(syntax, specs):
    constr = partial_evaluator(syntax[2], evaluate_remove_for_with, specs)
    constr = partial_evaluator(constr,    evaluate_remove_rename_rebase)
    constr = partial_evaluator(constr,    evaluate_remove_trivials)
    constr = partial_evaluator(constr,    evaluate_flatten_connectives)
    constr = partial_evaluator(constr,    evaluate_bisect_connectives)
    constr = partial_evaluator(constr,    evaluate_remove_trivials)

    atom_counter = {}
    slots, result, code = code_generation_core(constr, atom_counter)

    constr = partial_evaluator(constr, optimize_delay_aliases, slots)

    atom_counter = {}
    slots2, result, code = code_generation_core(constr, atom_counter)

    code = postprocess_copyconstructions(code)
    code = postprocess_add_loops(code)

    code = "\n".join(["{} atom{}_[{}];".format(typename, atom_counter[typename][0], atom_counter[typename][1]) for typename in atom_counter]
                    +[code.rstrip()]
                    +["vector<pair<string,unique_ptr<SolverAtom>>> constraint({});".format(len(slots))]
                    +["constraint[{}] = make_pair(\"{}\", unique_ptr<SolverAtom>(new {}({})));".format(n, slot, result[slot][0], result[slot][1]) for n,slot in enumerate(slots)])

    code = code.replace("<[", "").replace("]>", "")

    return "\n".join(["vector<Solution> Detect{}(llvm::Function& function, unsigned max_solutions)".format(syntax[1])]
                    +["{"]
                    +["    FunctionWrap wrap(function);"]
                    +[indent_code("    ", code)]
                    +["    auto result = Solution::Find(move(constraint), function, max_solutions);"]
                    +["    return result;"]
                    +["}"])

def generate_cpp_code(syntax_list):
    includes  = ["IdiomSpecifications","BackendSpecializations", "BackendDirectClasses", "BackendSelectors", "Solution"]
    specs     = {spec[1] : spec[2] for spec in syntax_list}

    return "\n".join(["#include \"llvm/Constraints/{}.hpp\"".format(s) for s in includes]
                    +["using namespace std;"]
                    +["#pragma GCC optimize (\"O0\")"]
                    +["#pragma clang optimize off"]
                    +["template<typename T>"]
                    +["class my_shared_ptr : public shared_ptr<T>"]
                    +["{"]
                    +["public:"]
                    +["    my_shared_ptr() = default;"]
                    +["    my_shared_ptr<T>& operator=(T t) { shared_ptr<T>::operator=(make_shared<T>(move(t))); return *this; }"]
                    +["    my_shared_ptr<T>& operator=(const my_shared_ptr<T>& t) { return *this = *t; }"]
                    +["};"]
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
import itertools

sys.setrecursionlimit(10000)

print(generate_cpp_code(parse(sys.stdin.read(), grammar)))
