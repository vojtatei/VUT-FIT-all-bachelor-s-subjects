<?php

namespace IPP\Student;

use IPP\Core\Instruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\SemanticException;
use IPP\Student\Exception\StringOperationException;
use IPP\Student\Exception\ValueException;


class Int2CharInstruction extends AbstractInstruction {

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
    }

    public function execute() : void {
        // Získání hodnoty operandu, který je předáván instrukci
        $operandValue = $this->getOperandValue($this->args['symb'], $this->frames[1]);

        if ($operandValue === null) {
            throw new ValueException("Hodnota operandu není nastavena.");
        }

        // Zkontrolovat, zda je hodnota v platném rozsahu pro Unicode
        if (!is_int($operandValue) || $operandValue < 0 || $operandValue > 1114111) {
            throw new StringOperationException("Neplatná hodnota pro převod INT2CHAR: Hodnota musí být celé číslo v rozsahu 0 až 1114111.");
        }

        // Převod hodnoty na znak
        $char = chr($operandValue);

        // Přiřazení znaku do proměnné
        $this->frames[0]->setVariableValue($this->args['var']['value'], $char); 
    }

    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function getOperandValue(array $operand, Frame $frame): mixed {
        // Pokud je operand typu 'var', získáme jeho hodnotu z rámce
        if ($operand['type'] === 'var') {
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                return null; // Pokud proměnná není nastavena, vraťte null.
            }
            if (!is_int($value)) {
                throw new OperandTypeException("Hodnota proměnné {$operand['value']} není celé číslo.");
            }
            return $value;
        } elseif ($operand['type'] === 'int') {
            // Pokud je operand typu 'int', ověříme, že hodnota je celé číslo
            if (filter_var($operand['value'], FILTER_VALIDATE_INT) === false) {
                throw new OperandTypeException("Hodnota {$operand['value']} není platné celé číslo.");
            }
            return intval($operand['value']);
        } else {
            throw new OperandTypeException("INT2CHAR očekává operand typu 'int' nebo 'var'.");
        }
    }
}

