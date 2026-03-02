<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Frame;


class AddInstruction extends AbstractInstruction {


    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
    }

    public function execute(): void {
        $operand1 = $this->resolveOperandValue($this->args[0], $this->frames[0]);
        $operand2 = $this->resolveOperandValue($this->args[1], $this->frames[1]);

        $result = $operand1 + $operand2;
        $this->frames[2]->setVariableValue($this->args[2]['value'], $result);
    }
        
    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     * @return int The resolved integer value of the operand.
     */
    private function resolveOperandValue($operand, Frame $frame): int {
        if (!is_array($operand) || !isset($operand['type']) || !isset($operand['value'])) {
            throw new OperandTypeException("Operand musí být pole s klíči 'type' a 'value'.");
        }
    
        if ($operand['type'] === 'var') {
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                throw new ValueException("Proměnná '{$operand['value']}' není definována.");
            }
            if (!is_int($value)) {
                throw new OperandTypeException("Hodnota proměnné '{$operand['value']}' musí být typu int.");
            }
            return $value;
        } elseif ($operand['type'] === 'int') {
            return intval($operand['value']);
        } else {
            throw new OperandTypeException("Neplatný typ operandu: {$operand['type']}.");
        }
    }
}