import GeneratedParser

--data SyntaxType = PLiteral String | PNumber String | POperator String | PNode String [SyntaxType] deriving (Eq,Ord,Show)

import Data.Char
import Data.List
import Debug.Trace
import qualified Data.Map as Map
import qualified Data.Maybe as Maybe

data CharType = Digit Char | Whitespace Char | Normal Char | Special Char

skipChars::String->[CharType]
skipChars []        = []
skipChars ('\n':xs) = classifyChars xs
skipChars (_:xs)    = skipChars xs

classifyChars::String -> [CharType]
classifyChars [] = []
classifyChars ('#':xs)      = skipChars xs
classifyChars (x:xs)
    | x `elem` "(){}[]=.+-,@&|" = Special x:classifyChars xs
    | isDigit x                 = Digit x:classifyChars xs
    | isSpace x                 = Whitespace x:classifyChars xs
    | otherwise                 = Normal x:classifyChars xs

tokenize::[CharType]->[SyntaxType]
tokenize [] = []
tokenize (Special '.':Special '.':xs) = PLiteral "..":tokenize xs
tokenize (Special a:xs)               = PLiteral [a] :tokenize xs
tokenize (Whitespace _:xs)            = tokenize xs
tokenize (Digit a:Digit b:xs)         = case tokenize (Digit b:xs) of (PNumber n:ns) -> PNumber (a:n):ns
tokenize (Digit a:xs)                 = PNumber [a]:tokenize xs
tokenize (Normal a:Digit  b:xs)       = case tokenize (Digit  b:xs) of (PNumber n:ns) -> PLiteral (a:n):ns
tokenize (Normal a:Normal b:xs)       = case tokenize (Normal b:xs) of (PLiteral n:ns) -> PLiteral (a:n):ns
tokenize (Normal a:xs)                = PLiteral [a]:tokenize xs

parse::[SyntaxType]->[[SyntaxType]]->[[SyntaxType]]
parse xs     [] = error ("Syntax error before "++show xs)
parse []     ys = ys
parse (x:xs) ys = parse xs [z|y<-ys,z<-match (x:y)]

unbox::[[SyntaxType]]->[SyntaxType]
unbox [x] = x

removeInvisibles::[SyntaxType]->[SyntaxType]
removeInvisibles ((PNode "#" [cont]):xs)     = removeInvisibles [cont] ++ removeInvisibles xs
removeInvisibles ((PNode ('@':str) cont):xs) = removeInvisibles cont ++ removeInvisibles xs
removeInvisibles ((PNode      str  cont):xs) = (PNode str $ removeInvisibles cont):removeInvisibles xs
removeInvisibles (x:xs)                      = x:removeInvisibles xs
removeInvisibles []                          = []

{-- Starting here it gets specific to the grammar --}

collectSpecifications::[SyntaxType]->Map.Map String SyntaxType
collectSpecifications ((PNode "specification" [PLiteral n, c]):xs) = Map.insert n c $ collectSpecifications xs
collectSpecifications [] = Map.empty

simplifyCalc1::Map.Map String Int->SyntaxType->SyntaxType
simplifyCalc1 vars (PNode "baseconst"      [PNumber  n]) = (PNode "baseconst"                         [PNumber n])
simplifyCalc1 vars (PNode "addconst" [base, PNumber  n]) = (PNode "addconst" [simplifyCalc1 vars base, PNumber n])
simplifyCalc1 vars (PNode "subconst" [base, PNumber  n]) = (PNode "subconst" [simplifyCalc1 vars base, PNumber n])
simplifyCalc1 vars (PNode "addvar"   [base, PLiteral s]) =
    case Map.lookup s vars of (Just n) -> (PNode "addconst" [simplifyCalc1 vars base, PNumber $ show n])
                              Nothing  -> (PNode "addvar"   [simplifyCalc1 vars base, PLiteral s])
simplifyCalc1 vars (PNode "subvar"   [base, PLiteral s]) =
    case Map.lookup s vars of (Just n) -> (PNode "subconst" [simplifyCalc1 vars base, PNumber $ show n])
                              Nothing  -> (PNode "subvar"   [simplifyCalc1 vars base, PLiteral s])
simplifyCalc1 vars (PNode "basevar"        [PLiteral s]) =
    case Map.lookup s vars of (Just n) -> (PNode "baseconst" [PNumber $ show n])
                              Nothing  -> (PNode "basevar"   [PLiteral s])

stradd::String->String->String
stradd n m = show $ ((read::(String->Int)) n) + ((read::(String->Int)) m)
strsub::String->String->String
strsub n m = show $ ((read::(String->Int)) n) - ((read::(String->Int)) m)

simplifyCalc2::SyntaxType->SyntaxType
simplifyCalc2 (PNode "addconst" [node, PNumber m]) =
    case simplifyCalc2 node of (PNode "baseconst" [PNumber n]) -> PNode "baseconst"        [PNumber $ stradd n m]
                               other                           -> PNode "addconst"  [other, PNumber            m]
simplifyCalc2 (PNode "subconst" [node, PNumber m]) =
    case simplifyCalc2 node of (PNode "baseconst" [PNumber n]) -> PNode "baseconst"        [PNumber $ strsub n m]
                               other                           -> PNode "subconst"  [other, PNumber            m]
simplifyCalc2 other = other

simplifyCalc::Map.Map String Int->SyntaxType->SyntaxType
simplifyCalc vars syntax = simplifyCalc2 $ simplifyCalc1 vars syntax

finalizeCalc::SyntaxType->Maybe Int
finalizeCalc (PNode "baseconst"       [PNumber n]) = Just      ((read::(String->Int)) n)
finalizeCalc (PNode "addconst"  [base, PNumber n]) = fmap ((+) ((read::(String->Int)) n)) $ finalizeCalc base
finalizeCalc (PNode "subconst"  [base, PNumber n]) = fmap (flip (-) ((read::(String->Int)) n)) $ finalizeCalc base
finalizeCalc                                     _ = Nothing

evaluateCalc::Map.Map String Int->SyntaxType->Maybe Int
evaluateCalc vars syntax = finalizeCalc $ simplifyCalc vars syntax

reconfigVariables::Map.Map String Int->[SyntaxType]->Maybe (Map.Map String Int)
reconfigVariables vars (PLiteral n:node:xs) = fmap nextstep $ reconfigVariables vars xs
    where nextstep = case evaluateCalc vars node of {(Just i) -> Map.insert n i; Nothing -> id}
reconfigVariables vars [] = Just Map.empty

simplify1::Map.Map String SyntaxType->Map.Map String Int->SyntaxType->Maybe SyntaxType
simplify1 specs vars (PNode "specification" [PLiteral n, cont]) = do
    recursion <- simplify1 specs vars cont
    return $ PNode "specification" [PLiteral n, recursion]
simplify1 specs vars (PNode "inheritance" [PLiteral n]) = do
    spec <- Map.lookup n specs
    simplify1 specs vars spec
simplify1 specs vars (PNode "inheritance" (PLiteral n:xs)) = do
    spec    <- Map.lookup n specs
    newvars <- reconfigVariables vars xs
    simplify1 specs newvars spec
simplify1 specs vars (PNode "forall" [cont, (PLiteral s), start, end]) = do
    startindex <- (evaluateCalc vars start)
    endindex   <- (evaluateCalc vars end)
    unrolled   <- sequence [simplify1 specs (Map.insert s i vars) cont | i<-[startindex..endindex-1]]
    return $ PNode "conjunction" unrolled
simplify1 specs vars (PNode "forsome" [cont, (PLiteral s), start, end]) = do
    startindex <- evaluateCalc vars start
    endindex   <- evaluateCalc vars end
    unrolled   <- sequence [simplify1 specs (Map.insert s i vars) cont | i<-[startindex..endindex-1]]
    return $ PNode "disjunction" unrolled
simplify1 specs vars (PNode "forone" [cont, (PLiteral s), start]) = do
    startindex <- evaluateCalc vars start
    simplify1 specs (Map.insert s startindex vars) cont
simplify1 specs vars (PNode "default" [cont, (PLiteral s), indexExp]) = do
    i <- case (Map.lookup s vars) of {(Just i) -> (Just i); Nothing -> evaluateCalc vars indexExp}
    simplify1 specs (Map.insert s i vars) cont
simplify1 specs vars (PNode "if" [idx1, idx2, cont1, cont2]) = do
    left  <- evaluateCalc vars idx1
    right <- evaluateCalc vars idx2
    simplify1 specs vars $ if left == right then cont1 else cont2
simplify1 specs vars (PNode "slotindex" [slot, idx]) = do
    return $ PNode "slotindex" [slot, simplifyCalc vars idx]
simplify1 specs vars (PNode s c) = do
    recurs <- sequence $ map (simplify1 specs vars) c
    return $ PNode s recurs
simplify1 specs vars other = Just other

applyRename::Map.Map SyntaxType SyntaxType->SyntaxType->Maybe SyntaxType
applyRename dict (PNode "slotbase"   xs)     = Map.lookup (PNode "slotbase" xs) dict
applyRename dict (PNode "slotmember" (x:xs)) =
    case Map.lookup (PNode "slotmember" (x:xs)) dict of (Just n) -> Just n
                                                        Nothing  -> case applyRename dict x of (Just n) -> Just (PNode "slotmember" (n:xs))
                                                                                               Nothing  -> Nothing

applyRename dict (PNode "slotindex" (x:xs)) =
    case Map.lookup (PNode "slotindex" (x:xs)) dict of (Just n) -> Just n
                                                       Nothing  -> case applyRename dict x of (Just n) -> Just (PNode "slotindex" (n:xs))
                                                                                              Nothing  -> Nothing

applyRename dict (PNode "slotrange" (x:xs)) =
    case Map.lookup (PNode "slotrange" (x:xs)) dict of (Just n) -> Just n
                                                       Nothing  -> case applyRename dict x of (Just n) -> Just (PNode "slotrange" (n:xs))
                                                                                              Nothing  -> Nothing

attachBase::Maybe SyntaxType->SyntaxType->SyntaxType
attachBase Nothing                                 right   = right
attachBase (Just left) (PNode "slotbase"          [right]) = PNode "slotmember" [left, right]
attachBase (Just left) (PNode "slotmember" [middle,right]) = PNode "slotmember" [attachBase (Just left) middle, right]
attachBase (Just left) (PNode "slotindex"  [middle,right]) = PNode "slotindex"  [attachBase (Just left) middle, right]
attachBase (Just left) (PNode "slotrange"  [middle,r1,r2]) = PNode "slotrange"  [attachBase (Just left) middle, r1,r2]

applySlotTransform::(Map.Map SyntaxType SyntaxType,Maybe SyntaxType)->SyntaxType->SyntaxType
applySlotTransform (dict,base) (PNode "slotbase" const) = Maybe.fromMaybe notindict indict
    where notindict = (attachBase base $ PNode "slotbase" const)
          indict    = (applyRename dict (PNode "slotbase" const))
applySlotTransform (dict,base) (PNode "slotmember" const) = Maybe.fromMaybe notindict indict
    where notindict = (attachBase base $ PNode "slotmember" const)
          indict    = (applyRename dict (PNode "slotmember" const))
applySlotTransform (dict,base) (PNode "slotindex" const) = Maybe.fromMaybe notindict indict
    where notindict = (attachBase base $ PNode "slotindex" const)
          indict    = (applyRename dict (PNode "slotindex" const))
applySlotTransform (dict,base) (PNode "slotrange" const) = Maybe.fromMaybe notindict indict
    where notindict = (attachBase base $ PNode "slotrange" const)
          indict    = (applyRename dict (PNode "slotrange" const))
applySlotTransform dict (PNode str const) = (PNode str $ map (applySlotTransform dict) const)
applySlotTransform dict other = other

buildSlotTransformSet::[SyntaxType]->(Map.Map SyntaxType SyntaxType,Maybe SyntaxType)
buildSlotTransformSet []       = (Map.empty, Nothing)
buildSlotTransformSet [base]   = (Map.empty, Just base)
buildSlotTransformSet (x:y:xs) = (Map.insert y x dict, base)
    where (dict,base) = buildSlotTransformSet xs

simplify2::SyntaxType->SyntaxType
simplify2 (PNode "rename" (x:xs)) = applySlotTransform (buildSlotTransformSet xs) $ simplify2  x
simplify2 (PNode str        cont) = (PNode str $ map simplify2 cont)
simplify2 other = other

prettyprint::SyntaxType->String
prettyprint (PLiteral s)     = show s
prettyprint (PNumber  n)     = show $ (read::(String->Int)) n
prettyprint (PNode str cont) = ("("++intercalate ", " (show str:map prettyprint cont)++")")

main = do
    contents   <- getContents
    let parsed  = parse (tokenize $ classifyChars contents) [[PNode "#" []]]
    let cleaned = init $ removeInvisibles $ unbox $ parsed
    let simpler1 = Maybe.mapMaybe (simplify1 (collectSpecifications cleaned) Map.empty) cleaned
    let simpler2 = map simplify2 simpler1
    putStrLn $ "("++intercalate ", " (map prettyprint simpler2)++")"