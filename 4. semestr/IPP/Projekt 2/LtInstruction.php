<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Frame;


class LtInstruction extends AbstractInstruction {
    /**
     * @var array{'type': string, 'value': mixed} $var
     */
    private array $var;

    /**
     * @var array{'type': string, 'value': mixed} $symb1
     */
    private array $symb1;

    /**
     * @var array{'type': string, 'value': mixed} $symb2
     */
    private array $symb2;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);

        // Argumenty jsou ve správném pořadí podle specifikace IPPcode24
        $this->var = $args[0];
        $this->symb1 = $args[1];
        $this->symb2 = $args[2];
    }

    public function execute(): void {
        $operand1 = $this->resolveOperandValue($this->symb1, $this->frames[1]);
        $operand2 = $this->resolveOperandValue($this->symb2, $this->frames[1]);

        // Kontrola, že oba operandy jsou stejného typu
        if (gettype($operand1) !== gettype($operand2)) {
            throw new OperandTypeException("Operandy musí být stejného typu pro porovnání v GT.");
        }

        // Vykonání porovnání a uložení výsledku do proměnné
        $result = $operand1 < $operand2; // Přímo porovnává hodnoty
        $this->frames[0]->setVariableValue($this->var['value'], $result);
    }

     /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue(array $operand, Frame $frame): mixed {
        if ($operand['type'] === 'var') {
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                throw new ValueException("Proměnná '{$operand['value']}' není definována.");
            }
            if (is_string($value)) {
                $value = StringDecoder::decodeEscapeSequences($value);
            }
            return $value;
        } elseif ($operand['type'] === 'bool') {
            return filter_var($operand['value'], FILTER_VALIDATE_BOOLEAN);
        } elseif ($operand['type'] === 'int') {
            return intval($operand['value']);
        } elseif ($operand['type'] === 'string') {
            return $operand['value'];
        } else {
            throw new OperandTypeException("Nepodporovaný typ pro GT instrukci.");
        }
    }
}
