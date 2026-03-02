<?php
namespace IPP\Student;

use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\OperandValueException;
use IPP\Student\Exception\ValueException;


class IDivInstruction extends AbstractInstruction {

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        if (count($args) !== 3) {
            throw new \InvalidArgumentException("IDivInstruction requires exactly three arguments.");
        }
    }

    public function execute(): void {
        // Získá hodnotu operandu, ať už je literál nebo proměnná
        $operand1 = $this->resolveOperandValue($this->args[0], $this->frames[0]);
        $operand2 = $this->resolveOperandValue($this->args[1], $this->frames[1]);

        if ($operand2 == 0) {
            throw new OperandValueException("Dělení nulou v IDIV instrukci.");
        }


        // Provedení celočíselného dělení
        $result = intdiv($operand1, $operand2);

        // Uložení výsledku do cílové proměnné
        $this->frames[2]->setVariableValue($this->args[2]['value'], $result);
    }


    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue(array $operand, Frame $frame): int {
        // Kontroluje, zda je operand typu var nebo literál (např. int)
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