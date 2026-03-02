<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Frame;


class OrInstruction extends AbstractInstruction  {
    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
    }

    public function execute(): void {
        // Předpokládáme, že operand1 a operand2 jsou typu bool
        $operand1 = $this->resolveOperandValue($this->args[1], $this->frames[1]);
        $operand2 = $this->resolveOperandValue($this->args[2], $this->frames[2]);

        // Aplikujeme logickou operaci AND
        $result = $operand1 || $operand2;

        // Uložíme výsledek do cílové proměnné
        $this->frames[0]->setVariableValue($this->args[0]['value'], $result);
    }

    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue(array $operand, Frame $frame) : bool {
        if ($operand['type'] === 'var') {
            // Operand je proměnná, získáme její hodnotu z rámce
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                throw new ValueException("Proměnná '{$operand['value']}' není definována.");
            }
            if (!is_bool($value)) {
                throw new OperandTypeException("Operand {$operand['value']} není typu bool.");
            }
            return $value;
        } elseif ($operand['type'] === 'bool') {
            // Operand je přímá bool hodnota
            return filter_var($operand['value'], FILTER_VALIDATE_BOOLEAN);  
        } else {
            // Operand není ani proměnná typu bool ani bool literál
            throw new OperandTypeException("Operand {$operand['type']} není podporovaného typu pro OR instrukci.");
        }
    }
}
