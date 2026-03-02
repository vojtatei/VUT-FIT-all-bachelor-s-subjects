<?php

namespace IPP\Student;


use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\XMLException;
use IPP\Student\Exception\InvalidSource;
use IPP\Student\Exception\SemanticException;
use IPP\Student\Exception\VariableAccessException;
use IPP\Student\InstructionValidator;
use IPP\Core\ReturnCode;;
use DOMDocument;
use DOMXPath;
use DOMElement;
use IPP\Student\Exception\FrameAccessException;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\StringOperationException;
use IPP\Student\Frame;
use IPP\Student\DefvarInstruction;
use IPP\Student\Exception\OperandValueException;
use IPP\Student\Exception\ValueException;
use IPP\Student\XMLProcessor;


class Interpreter extends AbstractInterpreter
{

    public Frame $globalFrame;
    private InstructionValidator $validator;

    /**
     * @var Frame[] Stack of frames.
     */

    private array $frameStack = [];
    private ?Frame $localFrame = null; 
    private ?Frame $tempFrame = null;

    /**
     * @var mixed[] Data stack holding various types of values.
     */
    private array $dataStack = []; 

    private function initializeInterpreter(): void {
        $this->globalFrame = new Frame();
        $this->validator = new InstructionValidator();
}

public function execute(): int {
    // Inicializace interpreteru a resetování globálního rámce
    $this->initializeInterpreter();
    $this->globalFrame->reset();

    $xmlProcessor = new XMLProcessor($this->source->getDOMDocument(), $this->globalFrame);
    $xmlProcessor->validateRootElement();
    $instructions = $xmlProcessor->getInstructions();
        
        // Získání a seřazení všech hodnot 'order' pro snadnější navigaci
        $instructionOrders = array_keys($instructions);
        sort($instructionOrders);

        // Nastavení aktuálního 'order' na první instrukci nebo na defaultní hodnotu
        $currentOrder = $this->globalFrame->getInstructionPointer();
        if (!in_array($currentOrder, $instructionOrders)) {
            $currentOrder = reset($instructionOrders);
        }

        // Hlavní smyčka zpracovávání instrukcí
        while (isset($instructions[$currentOrder])) {
            $instructionElement = $instructions[$currentOrder];
            $opcode = $instructionElement->getAttribute('opcode');

        

            // Zpracování aktuální instrukce podle jejího operačního kódu
            $this->processInstruction($opcode, $instructionElement);

            // Logika pro skoky; pokud byl vyvolán skok, nastaví se 'currentOrder' na novou hodnotu
            if (JumpIfEqInstruction::$shouldJump || $opcode === "JUMP" || JumpIfNeqInstruction::$shouldJump) {
                $currentOrder = $this->globalFrame->getInstructionPointer();
                JumpIfEqInstruction::$shouldJump = false; // Reset příznaku skoku
                JumpIfNeqInstruction::$shouldJump = false; 
            } elseif ($opcode === "CALL") {
                $labelName = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $labelPosition = $this->globalFrame->getLabelIndex($labelName);
                $this->globalFrame->pushCallStack($currentOrder + 1); // Uložíme následující pozici (pro návratovou instrukci)
                $currentOrder = $labelPosition; // Skočíme na pozici štítku
            } elseif ($opcode === "RETURN") {
                // Zde použijeme popCallStack pro návrat na pozici po CALL
                $currentOrder = $this->globalFrame->popCallStack(); // Tato metoda by měla vyjmout hodnotu ze zásobníku a vrátit ji
            } else {
                // Hledání další instrukce k zpracování podle seřazeného seznamu 'order'
                $nextOrderIndex = array_search($currentOrder, $instructionOrders) + 1;
                $currentOrder = isset($instructionOrders[$nextOrderIndex]) ? $instructionOrders[$nextOrderIndex] : null;
            }

            // Pokud neexistuje další instrukce, ukončí se smyčka
            if ($currentOrder === null) break;
        }
        return ReturnCode::OK;
    }

    private function isTemporaryFrameInitialized(): bool {
        return $this->tempFrame !== null;
    }


    private function prepareArgument(DOMElement $node) : mixed {
        $value = trim($node->nodeValue);
        $type = $node->getAttribute('type');
        switch ($type) {
            case 'var':
                return ['type' => 'var', 'value' => $value];
            case 'label':
                // Pro štítek nemusíme kontrolovat hodnotu, protože je to identifikátor, nikoli hodnota
                return ['type' => 'label', 'value' => $value];
            case 'string':
                return ['type' => 'string', 'value' => $value];
            case 'int':
                if (filter_var($value, FILTER_VALIDATE_INT) === false) {
                    throw new InvalidSource("Očekávána celočíselná hodnota pro 'int', ale zjištěno: '$value'.");
                }
                return ['type' => 'int', 'value' => intval($value)];
            case 'bool': // Přidána kontrola pro boolean
                if ($value === 'true' || $value === 'false') {
                    return ['type' => 'bool', 'value' => $value === 'true'];
                } else {
                    throw new OperandTypeException("Očekávána booleovská hodnota pro 'bool', ale zjištěno: '$value'.");
                }
            case 'nil':
                return ['type' => 'nil', 'value' => null]; 
            
            case 'symb': // případně jiný název, pokud je použit v XML
                if (is_numeric($value)) {
                    return ['type' => 'int', 'value' => intval($value)];
                } else {
                    throw new OperandTypeException("Očekáván celočíselný typ pro 'symb': $value.");
                }
            default:
                throw new OperandTypeException("Neznámý typ operandu: $type.");
        }
    }

    private function createFrame(): void {
        $this->tempFrame = new Frame();
    }

    private function pushFrame(): void {
        if ($this->tempFrame === null) {
            throw new FrameAccessException("Dočasný rámec není inicializován.");
        }
    
        // Přejmenování proměnných z TF@ na LF@ v tempFrame
        foreach ($this->tempFrame->getVariables() as $varName => $variable) {
            $newVarName = str_replace('TF@', 'LF@', $varName);
            $this->tempFrame->defineVar($newVarName);
            $this->tempFrame->setVariableValue($newVarName, $variable->getValue());
            $this->tempFrame->undefineVar($varName);
        }
    
        // Uložení tempFrame jako nového localFrame a jeho přidání do zásobníku
        array_push($this->frameStack, $this->tempFrame);
        $this->localFrame = $this->tempFrame;
    
        // Reset tempFrame pro další použití
        $this->tempFrame = null;
    }
    private function popFrame(): void {
        if (empty($this->frameStack)) {
            throw new FrameAccessException("Zásobník rámců je prázdný, nelze provést POPFRAME.");
        }
    
        // Odebrání LF ze zásobníku a nastavení jej jako nový TF
        $this->tempFrame = array_pop($this->frameStack);
    
        // Přejmenování proměnných z LF@ na TF@
        $variables = $this->tempFrame->getVariables();
        foreach ($variables as $varName => $variable) {
            $newVarName = str_replace('LF@', 'TF@', $varName);
            $this->tempFrame->undefineVar($varName);
            $this->tempFrame->defineVar($newVarName);
            $this->tempFrame->setVariableValue($newVarName, $variable->getValue());
        }
    
        // Aktualizace LF podle aktuálního stavu zásobníku
        $this->localFrame = !empty($this->frameStack) ? end($this->frameStack) : null;
    }


        private function resolveFrameForVariable(mixed $variableName): Frame {
            // Zjistí prefix proměnné
            if (!preg_match('/^(TF|LF|GF)@/', $variableName)) {
                return $this->globalFrame;
            }
            $framePrefix = explode('@', $variableName)[0];
        
            // Vrátí odpovídající rámec na základě prefixu
            switch ($framePrefix) {
                case 'TF':
                    if ($this->tempFrame === null) {
                        throw new FrameAccessException("Dočasný rámec TF není inicializován.");
                    }
                    return $this->tempFrame;
        
                case 'LF':
                    if ($this->localFrame === null) {
                        throw new FrameAccessException("Lokální rámec LF není inicializován nebo není aktivní.");
                    }
                    return $this->localFrame;
        
                case 'GF':
                    return $this->globalFrame;
        
                default:
                    throw new FrameAccessException("Neznámý rámec pro proměnnou: $variableName");
            }
        }
        


    private function processInstruction(string $opcode, DOMElement $instructionElement): void {
        
        switch (strtoupper($opcode)) {
            case 'CREATEFRAME':
                // implementace instrukce CREATEFRAME
                $this->createFrame();
                $this->validator->validateZeroArgs($instructionElement, $opcode);
                break;
            case 'PUSHFRAME':
                // implementace instrukce PUSHFRAME
                $this->pushFrame();
                $this->validator->validateZeroArgs($instructionElement, $opcode);
                break;

            case 'POPFRAME':
                $this->popFrame();
                $this->validator->validateZeroArgs($instructionElement, $opcode);
                break;

            case 'PUSHS':
                // Získání argumentu
                $arg1 = $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0));
                $this->validator->validateOneArg($instructionElement, $opcode);
            
                // Kontrola, zda je argument typu proměnná a zda je definován a inicializován
                if ($arg1['type'] === 'var') {
                    $frame = $this->resolveFrameForVariable($arg1['value']);
                    if (!$frame->isVariableDefined($arg1['value'])) {
                        throw new VariableAccessException("Proměnná '{$arg1['value']}' není definována.");
                    }
                    if ($frame->isVariableInitialized($arg1['value'])) {
                        $valueToPush = $frame->getVariableValue($arg1['value']);
                    } else {
                        throw new ValueException("Proměnná '{$arg1['value']}' není inicializovaná.");
                    }
                } else {
                    // Pokud argument není typu proměnná, předpokládá se, že je to literál nebo hodnota
                    $valueToPush = $arg1['value'];
                }
            
                // Přidání hodnoty na zásobník
                array_push($this->dataStack, $valueToPush);
                break;

            case 'INT2CHAR':
                $varArg = $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0));
                $symbArg = $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0));
                $this->validator->validateTwoArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
            
                // Získání rámců pro každý argument
                $frameForVar = $this->resolveFrameForVariable($varArg['value']);
                $frameForSymb = $this->resolveFrameForVariable($symbArg['value']);
            
                $int2CharInstruction = new Int2CharInstruction(
                    ['var' => $varArg, 'symb' => $symbArg],
                    $order,
                    [$frameForVar, $frameForSymb]
                );
                $int2CharInstruction->execute();
                break;

            case 'DPRINT':
                // Vytvoření a spuštění instrukce DPRINT
                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $this->validator->validateOneArg($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                $frame = $this->resolveFrameForVariable($arg1);
                
                // Získání hodnoty proměnné nebo literálu
                $variableValue = $frame->isVariableDefined($arg1) 
                    ? $frame->getVariableValue($arg1)
                    : $arg1; // Přímý literál, pokud není proměnná
                
                            // Převod hodnoty na string pro výpis
                $variableValueStr = is_bool($variableValue) 
                ? ($variableValue ? 'true' : 'false') 
                : (string)$variableValue;

                // Výpis hodnoty na stderr
                $this->stderr->writeString($variableValueStr . "\n");
                break;
                
            case 'POPS':
                if (empty($this->dataStack)) 
                {
                    throw new ValueException("Datový zasobnik je prázdný, nelze prověst POPS.");
                }
                $varName = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $this->validator->validateOneArg($instructionElement, $opcode);
                $value = array_pop($this->dataStack);
                $this->globalFrame->setVariableValue($varName, $value);
                break;

            case 'DEFVAR':

                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $this->validator->validateOneArg($instructionElement, $opcode);
                $args = [$arg1];
                $order = intval($instructionElement->getAttribute('order'));
                
                list($frame, $variable) = explode('@', $arg1, 2);
                
                if ($frame === 'TF' && !$this->isTemporaryFrameInitialized()) {
                    throw new FrameAccessException("Dočasný rámec TF nebyl inicializován.");
                }
                
                $frames = [$this->resolveFrameForVariable($arg1)];
            
                $defvarInstruction = new DefvarInstruction($args, $order, $frames);
                $defvarInstruction->execute();
                break;
            case 'MOVE':
                
                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $arg2 = trim($instructionElement->getElementsByTagName('arg2')->item(0)->nodeValue);
                $this->validator->validateTwoArgs($instructionElement, $opcode);
                $args = [$arg1, $arg2];
                $order = intval($instructionElement->getAttribute('order'));
                // Získání rámců pro každý argument
                $frameForTargetVar = $this->resolveFrameForVariable($arg1);
                $frameForSourceSymbol = $this->resolveFrameForVariable($arg2);

                $moveInstruction = new MoveInstruction($args, $order, [$frameForTargetVar, $frameForSourceSymbol]);
                $moveInstruction->execute();
                break;
            
            case 'GETCHAR':
                
                $targetVarNode = $instructionElement->getElementsByTagName('arg1')->item(0);
                $sourceStringNode = $instructionElement->getElementsByTagName('arg2')->item(0);
                $indexNode = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);
            
                if ($targetVarNode === null || $sourceStringNode === null || $indexNode === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci GETCHAR.");
                }
            
                $order = intval($instructionElement->getAttribute('order'));
            
                // Příprava argumentů pro instrukci
                $targetArg = $this->prepareArgument($targetVarNode);
                $sourceStringArg = $this->prepareArgument($sourceStringNode);
                $indexArg = $this->prepareArgument($indexNode);

                // Určení rámců pro každý operand
                $targetFrame = $this->resolveFrameForVariable($targetArg['value']);
                $sourceStringFrame = $this->resolveFrameForVariable($sourceStringArg['value']);
                $indexFrame = $this->resolveFrameForVariable($indexArg['value']);
                
                // Vytvoření pole rámců
                $frames = [
                    $targetFrame,
                    $sourceStringFrame,
                    $indexFrame
                ];

                // Vytvoření instance GetCharInstruction s příslušnými rámci
                $getCharInstruction = new GetCharInstruction([$targetArg, $sourceStringArg, $indexArg], $order, $frames);
                $getCharInstruction->execute();
                break;

            case 'STRI2INT':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
            
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];

                $stri2intInstruction = new Stri2intInstruction($args, $order, $frames);
                $stri2intInstruction->execute();
                break;


            case 'SETCHAR':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));

                // Dekódování escape sekvencí pro argumenty, které jsou typu string
                foreach ($args as &$arg) {
                    if ($arg['type'] === 'string') {
                        $arg['value'] = StringDecoder::decodeEscapeSequences($arg['value']);
                    }
                }
                unset($arg); // Zrušení reference
            
                $frames = [$this->resolveFrameForVariable($args[0]['value'])];
                $setCharInstruction = new SetCharInstruction($args, $order, $frames);
                $setCharInstruction->execute();
                break;
                    

            case 'ADD':
                $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg2Node = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg3Node = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                if ($arg1Node === null || $arg2Node === null || $arg3Node === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci ADD.");
                }
                
                $order = intval($instructionElement->getAttribute('order'));
                
                // Použití metody prepareArgument pro získání argumentů ve správném formátu
                $arg1 = $this->prepareArgument($arg1Node); // Cílová proměnná pro výsledek
                $arg2 = $this->prepareArgument($arg2Node);
                $arg3 = $this->prepareArgument($arg3Node);

                $frames = [
                    $this->resolveFrameForVariable($arg2['value']),
                    $this->resolveFrameForVariable($arg3['value']),
                    $this->resolveFrameForVariable($arg1['value']),
                ];
                
                // Instance AddInstruction a její spuštění
                $addInstruction = new AddInstruction([$arg2, $arg3, $arg1], $order, $frames);
                $addInstruction->execute();
                break;

            case 'SUB':
                $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg2Node = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg3Node = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                
                if ($arg1Node === null || $arg2Node === null || $arg3Node === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci ADD.");
                }
                
                $order = intval($instructionElement->getAttribute('order'));
                
                // Použití metody prepareArgument pro získání argumentů ve správném formátu
                $arg1 = $this->prepareArgument($arg1Node); // Cílová proměnná pro výsledek
                $arg2 = $this->prepareArgument($arg2Node);
                $arg3 = $this->prepareArgument($arg3Node);
                
                $frameForArg1 = $this->resolveFrameForVariable($arg1['value']);
                $frameForArg2 = $this->resolveFrameForVariable($arg2['value']);
                $frameForArg3 = $this->resolveFrameForVariable($arg3['value']);
                
                // Instance AddInstruction a její spuštění
                $addInstruction = new SubInstruction([$arg2, $arg3, $arg1], $order,[$frameForArg2, $frameForArg3, $frameForArg1]);
                $addInstruction->execute();
                
                break;

            case 'GT':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);

                // Dekódování escape sekvencí pro argumenty, které jsou typu string
                foreach ($args as &$arg) {
                    if ($arg['type'] === 'string') {
                        $arg['value'] = StringDecoder::decodeEscapeSequences($arg['value']);
                    }
                }
                unset($arg); // Zrušení reference
            
                $order = intval($instructionElement->getAttribute('order'));
            
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];
                
                $gtInstruction = new GtInstruction($args, $order, $frames);
                $gtInstruction->execute();
                break;

            case 'LT':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);

                // Dekódování escape sekvencí pro argumenty, které jsou typu string
                foreach ($args as &$arg) {
                    if ($arg['type'] === 'string') {
                        $arg['value'] = StringDecoder::decodeEscapeSequences($arg['value']);
                    }
                }
                unset($arg); // Zrušení reference
            
                $order = intval($instructionElement->getAttribute('order'));
            
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];
                
                $gtInstruction = new LtInstruction($args, $order, $frames);
                $gtInstruction->execute();
                break;


            case 'EQ':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);

                // Dekódování escape sekvencí pro argumenty, které jsou typu string
                foreach ($args as &$arg) {
                    if ($arg['type'] === 'string') {
                        $arg['value'] = StringDecoder::decodeEscapeSequences($arg['value']);
                    }
                }
                unset($arg); // Zrušení reference
            
                $order = intval($instructionElement->getAttribute('order'));
            
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];
                
                $gtInstruction = new EqInstruction($args, $order, $frames);
                $gtInstruction->execute();
                break;
                         
            case 'MUL':
                    $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                    $arg2Node = $instructionElement->getElementsByTagName('arg2')->item(0);
                    $arg3Node = $instructionElement->getElementsByTagName('arg3')->item(0);
                    $this->validator->validateThreeArgs($instructionElement, $opcode);
                    
                    if ($arg1Node === null || $arg2Node === null || $arg3Node === null) {
                        throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci ADD.");
                    }
                    
                    $order = intval($instructionElement->getAttribute('order'));
                    
                    // Použití metody prepareArgument pro získání argumentů ve správném formátu
                    $arg1 = $this->prepareArgument($arg1Node); // Cílová proměnná pro výsledek
                    $arg2 = $this->prepareArgument($arg2Node);
                    $arg3 = $this->prepareArgument($arg3Node);
                    
                    $frames = [
                        $this->resolveFrameForVariable($arg2['value']),
                        $this->resolveFrameForVariable($arg3['value']),
                        $this->resolveFrameForVariable($arg1['value']),
                    ];
                    
                    $mulInstruction = new MulInstruction([$arg2, $arg3, $arg1], $order, $frames);
                    $mulInstruction->execute();
                    
                    break;

            case 'WRITE':
                // Get the value of the first argument
                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);

                $frame = $this->resolveFrameForVariable($arg1);
                $this->validator->validateOneArg($instructionElement, $opcode);

                if ($frame->isArgumentVariable($arg1)) {
                    // Kontrola, zda je argument proměnná
                    $frame = $this->resolveFrameForVariable($arg1);
                    if (!$frame->isVariableDefined($arg1)) {
                        throw new VariableAccessException("Proměnná '$arg1' není definována.");
                    }
                    $variableValue = $frame->getVariableValue($arg1);
                    if ($variableValue === null) {
                        throw new ValueException("Proměnná '$arg1' není inicializovaná (nemá přiřazenou hodnotu).");
                    }
                } else {
                    // Pokud argument není proměnná, zpracujte ho jako literál
                    $variableValue = $arg1 === 'nil' ? null : $arg1;
                }
            
                if (is_string($variableValue)) {
                    $variableValue = StringDecoder::decodeEscapeSequences($variableValue);
                }

                if (is_int($variableValue)) {
                    $this->stdout->writeInt($variableValue);
                } elseif (is_bool($variableValue)) {
                    $this->stdout->writeBool($variableValue);
                } elseif (is_string($variableValue)) {
                    $this->stdout->writeString($variableValue);
                } elseif ($variableValue === null) {
                    $this->stdout->writeString(''); // Výpis 'nil' jako prázdný řetězec
                } else {
                    
                }
                break;
                    
            
            case 'JUMP':
                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $this->validator->validateOneArg($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                
                // Globální rámec je nyní předán jako pole rámců, předpokládáme, že skoky jsou vždy ve vztahu k globálnímu rámci
                $jumpInstruction = new JumpInstruction([$arg1], $order, [$this->globalFrame]);
                $jumpInstruction->execute();
                break;
            case 'LABEL':
                
                $labelName = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
        
                $this->validator->validateOneArg($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));

                // Zkontrolujte, zda štítek již existuje
                if (!$this->globalFrame->isLabelDefined($labelName)) {
                    // Pokud štítek neexistuje, zaregistrujte ho
                    $this->globalFrame->registerLabel($labelName, $order);
                } else {
                  
                }
                break;

            case 'IDIV':
                $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg2Node = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg3Node = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);
               
                if ($arg1Node === null || $arg2Node === null || $arg3Node === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci ADD.");
                }
                
                $order = intval($instructionElement->getAttribute('order'));

                $arg1 = $this->prepareArgument($arg1Node); // Cílová proměnná pro výsledek
                $arg2 = $this->prepareArgument($arg2Node);
                $arg3 = $this->prepareArgument($arg3Node);

                            // Získání rámců pro každý argument
                $frameForArg1 = $this->resolveFrameForVariable($arg1['value']);
                $frameForArg2 = $this->resolveFrameForVariable($arg2['value']);
                $frameForArg3 = $this->resolveFrameForVariable($arg3['value']);

                $idivInstruction = new IDivInstruction([$arg2, $arg3, $arg1], $order, [$frameForArg2, $frameForArg3, $frameForArg1]);
                $idivInstruction->execute();
                break;

            case 'JUMPIFEQ':
                $labelElement = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg1Element = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg2Element = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);

                if ($labelElement === null || $arg1Element === null || $arg2Element === null) {
                    throw new SemanticException("Chybí jeden nebo více argumentů pro instrukci JUMPIFEQ.");
                }

                $label = $this->prepareArgument($labelElement)['value'];;
                $arg1 = $this->prepareArgument($arg1Element);
                $arg2 = $this->prepareArgument($arg2Element);

                if ($arg1['type'] === 'string') {
                    $arg1['value'] = StringDecoder::decodeEscapeSequences($arg1['value']);
                }
                if ($arg2['type'] === 'string') {
                    $arg2['value'] = StringDecoder::decodeEscapeSequences($arg2['value']);
                }

                $order = intval($instructionElement->getAttribute('order'));

                $frameForLabel = $this->globalFrame; // Předpokládáme, že štítek je v globálním rámci
                $frameForOperand1 = $this->resolveFrameForVariable($arg1['value']);
                $frameForOperand2 = $this->resolveFrameForVariable($arg2['value']);

                $jumpIfEqInstruction = new JumpIfEqInstruction(
                    [$label, $arg1, $arg2],  
                    $order, 
                    [$frameForLabel, $frameForOperand1, $frameForOperand2]
                );
                $jumpIfEqInstruction->execute();
                break;

            case 'JUMPIFNEQ':
                $labelElement = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg1Element = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg2Element = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);

                if ($labelElement === null || $arg1Element === null || $arg2Element === null) {
                    throw new SemanticException("Chybí jeden nebo více argumentů pro instrukci JUMPIFEQ.");
                }

                $label = $this->prepareArgument($labelElement)['value'];;
                $arg1 = $this->prepareArgument($arg1Element);
                $arg2 = $this->prepareArgument($arg2Element);
                $order = intval($instructionElement->getAttribute('order'));


                if ($arg1['type'] === 'string') {
                    $arg1['value'] = StringDecoder::decodeEscapeSequences($arg1['value']);
                }
                if ($arg2['type'] === 'string') {
                    $arg2['value'] = StringDecoder::decodeEscapeSequences($arg2['value']);
                }

                $frameForLabel = $this->globalFrame; // Předpokládáme, že štítek je v globálním rámci
                $frameForOperand1 = $this->resolveFrameForVariable($arg1['value']);
                $frameForOperand2 = $this->resolveFrameForVariable($arg2['value']);

                $jumpIfNeqInstruction = new JumpIfNeqInstruction(
                    [$label, $arg1, $arg2],  
                    $order, 
                    [$frameForLabel, $frameForOperand1, $frameForOperand2]
                );
                $jumpIfNeqInstruction->execute();
                break;
            
            case 'CONCAT':
                $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                $arg2Node = $instructionElement->getElementsByTagName('arg2')->item(0);
                $arg3Node = $instructionElement->getElementsByTagName('arg3')->item(0);
                $this->validator->validateThreeArgs($instructionElement, $opcode);
            
                if ($arg1Node === null || $arg2Node === null || $arg3Node === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci ADD.");
                }
                
                $order = intval($instructionElement->getAttribute('order'));
            
                // Výsledek bude v arg1, takže získáme typ a hodnotu pro arg2 a arg3
                /** @var DOMElement $arg2Node */
                $arg2Type = $arg2Node->getAttribute('type');
                $arg2Value = trim($arg2Node->nodeValue);
                /** @var DOMElement $arg3Node */
                $arg3Type = $arg3Node->getAttribute('type');
                $arg3Value = trim($arg3Node->nodeValue);
                $resultVarName = trim($arg1Node->nodeValue); // Výsledná proměnná (kde se uloží součet) je arg1
            
                // Příprava operandů pro sčítání
                $arg1 = ['type' => 'var', 'value' => $resultVarName];
                $arg2 = ['type' => $arg2Type, 'value' => $arg2Value];
                $arg3 = ['type' => $arg3Type, 'value' => $arg3Value];
                
                $frames = [
                    $this->resolveFrameForVariable($resultVarName),
                    $this->resolveFrameForVariable($arg2['value']),
                    $this->resolveFrameForVariable($arg3['value'])
                ];
            
                $concatInstruction = new ConcatInstruction([$arg1, $arg2, $arg3], $order, $frames);
                $concatInstruction->execute();
            
                break;

            case 'READ':
                // Získání hodnoty prvního argumentu
                $arg1 = trim($instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue);
                $arg2 = trim($instructionElement->getElementsByTagName('arg2')->item(0)->nodeValue);
                $this->validator->validateTwoArgs($instructionElement, $opcode);
            
                // Získání rámců pro proměnné
                $frame = $this->resolveFrameForVariable($arg1);
            
                // Kontrola, zda je proměnná definována
                if (!$frame->isVariableDefined($arg1)) {
                    throw new VariableAccessException("Proměnná '$arg1' není definována.");
                }
            
                // Čtení hodnoty podle zadaného typu
                $typedValue = null;
                switch ($arg2) {
                    case 'int':
                        $typedValue = $this->input->readInt();
                        break;
                    case 'bool':
                        $typedValue = $this->input->readBool();
                        break;
                    case 'string':
                        $typedValue = $this->input->readString();
                        break;
                    default:
                        throw new OperandTypeException("Nepodporovaný typ pro instrukci READ: {$arg2}.");
                }
            
                // Kontrola, zda je hodnota načtena správně
                if ($typedValue === false) {
                    $typedValue = '';
                }
            
                // Přiřazení načtené hodnoty k proměnné
                $frame->setVariableValue($arg1, $typedValue);
                break;

            case 'STRLEN':
                $arg1Node = $instructionElement->getElementsByTagName('arg1')->item(0);
                $stringOperandNode = $instructionElement->getElementsByTagName('arg2')->item(0);
                $this->validator->validateTwoArgs($instructionElement, $opcode);

                if ($arg1Node === null || $stringOperandNode === null) {
                    throw new OperandTypeException("Chybí jeden nebo více argumentů pro instrukci STRLEN.");
                }

                $order = intval($instructionElement->getAttribute('order'));

                $varName = trim($arg1Node->nodeValue);
                $stringOperand = trim($stringOperandNode->nodeValue);
                /** @var DOMElement $stringOperandNode */
                $stringOperandType = $stringOperandNode->getAttribute('type');

                $stringOperand = ['type' => $stringOperandType, 'value' => $stringOperand];

                $frames = [$this->resolveFrameForVariable($varName), $this->resolveFrameForVariable($stringOperand['value'])];

                $strlenInstruction = new StrlenInstruction([$varName, $stringOperand], $order, $frames);
                $strlenInstruction->execute();
                break;

            case 'CALL':
                $labelNodeValue = $instructionElement->getElementsByTagName('arg1')->item(0)->nodeValue;
                $currentOrder = intval($instructionElement->getAttribute('order'));
                
                // Zde byste získali potřebné rámce, předpokládáme, že používáte správné rámce pro štítek
                $framesForCall = [$this->resolveFrameForVariable($labelNodeValue)];
            
                // Příprava argumentů pro CallInstruction
                $argsForCall = [
                    ['type' => 'label', 'value' => $labelNodeValue]
                ];
            
                // Vytvoření instance CallInstruction a její spuštění
                $callInstruction = new CallInstruction($argsForCall, $currentOrder, $framesForCall);
                $callInstruction->execute();
                break;
            case 'RETURN':
                // Ověření, že instrukce nemá žádné argumenty
                $this->validator->validateZeroArgs($instructionElement, $opcode);
        
                
                $returnPosition = $this->globalFrame->popCallStackItem();
                // Nastavení ukazatele na vyjmutou pozici
                $this->globalFrame->setInstructionPointer($returnPosition);
                break;

            case 'TYPE':
                $varArg = $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0));
                $symbArg = $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0));
                $this->validator->validateTwoArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                $frameForVar = $this->resolveFrameForVariable($varArg['value']);
                $frameForSymb = $this->resolveFrameForVariable($symbArg['value']);
        
                $typeInstruction = new TypeInstruction([$varArg, $symbArg], $order, [$frameForVar, $frameForSymb]);
                $typeInstruction->execute();
                break;


            case 'EXIT':
                $symbArg = $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0));
                $this->validator->validateOneArg($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                // Získání rámců pro argument
                $frameForSymb = $this->resolveFrameForVariable($symbArg['value']);
        
                // Vytvoření instance ExitInstruction a její spuštění
                $exitInstruction = new ExitInstruction([$symbArg], $order, [$frameForSymb]);
                $exitInstruction->execute();
                break;

            case 'AND':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];
             
                $andInstruction = new AndInstruction($args, $order, $frames);
                $andInstruction->execute();           
                break;

            case 'OR':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg3')->item(0))
                ];
                $this->validator->validateThreeArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                    $this->resolveFrameForVariable($args[2]['value'])
                ];
                
                $orInstruction = new OrInstruction($args, $order, $frames);
                $orInstruction->execute();
                break;

            case 'NOT':
                $args = [
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg1')->item(0)),
                    $this->prepareArgument($instructionElement->getElementsByTagName('arg2')->item(0)),
                ];
                $this->validator->validateTwoArgs($instructionElement, $opcode);
                $order = intval($instructionElement->getAttribute('order'));
                $frames = [
                    $this->resolveFrameForVariable($args[0]['value']),
                    $this->resolveFrameForVariable($args[1]['value']),
                ];
                
                $notInstruction = new NotInstruction($args, $order, $frames);
                $notInstruction->execute();
                break; 

            default:
                throw new InvalidSource("Neznámý operační kód '$opcode'.");
        }
    }
}
