<?php
namespace IPP\Student;

use IPP\Student\Exception\VariableAccessException;
use IPP\Student\Frame;
use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\ValueException;


class MoveInstruction extends AbstractInstruction {
    private string $targetVariableName;
    private string $sourceSymbol;

    /**
     * @param string[] $args
     */

     public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        $this->targetVariableName = $args[0];
        $this->sourceSymbol = $args[1];
    }

    public function execute(): void {
        // Kontrola existence cílové proměnné
        $frameForTargetVar = $this->frames[0];
        $frameForSourceSymbol = $this->frames[1];


        if (!$frameForTargetVar->isVariableDefined($this->targetVariableName)) {
            throw new VariableAccessException("Cílová proměnná '{$this->targetVariableName}' neexistuje.");
        }


        $sourceValue = $this->getSourceSymbolValue($this->sourceSymbol, $frameForSourceSymbol);
        $frameForTargetVar->setVariableValue($this->targetVariableName, $sourceValue);

    }

    private function getSourceSymbolValue(string $symbol, Frame $frame): mixed {
        if ($frame->isArgumentVariable($symbol)) {
            if (!$frame->isVariableDefined($symbol)) {
                throw new VariableAccessException("Zdrojová proměnná '$symbol' není definována.");
            }
            if (!$frame->isVariableInitialized($symbol)) {
                throw new ValueException("Zdrojová proměnná '$symbol' není inicializována.");; 
            }
            return $frame->getVariableValue($symbol);
        } else {
            // Kontrola pro nil
            if ($symbol === 'nil') {
                return '';
            }
            // Kontrola pro booleovské hodnoty
            if ($symbol === 'true') {
                return true;
            }
            if ($symbol === 'false') {
                return false;
            }
            // Kontrola pro číselné hodnoty
            if (is_numeric($symbol)) {
                // Rozhodnout mezi int a float
                return strpos($symbol, '.') === false ? intval($symbol) : floatval($symbol);
            }
            // Výchozí: považujte symbol za řetězec
            return $symbol;
        }
    }
}
