<?php

namespace IPP\Student;

use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\StringOperationException;
use IPP\Student\Exception\ValueException;


class Stri2intInstruction extends AbstractInstruction {
    /**
     * @var array{'type': string, 'value': mixed} $targetVar
     */
    private array $targetVar;

    /**
     * @var array{'type': string, 'value': mixed} $sourceString
     */
    private array $sourceString;

    /**
     * @var array{'type': string, 'value': mixed} $charIndex
     */
    private array $charIndex;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);

        $this->targetVar = $args[0];
        $this->sourceString = $args[1];
        $this->charIndex = $args[2];
    }

    public function execute(): void {
        $frame = $this->frames[0];
        $string = $this->resolveOperandValue($this->sourceString, $frame);
        $index = $this->resolveOperandValue($this->charIndex, $frame);

        if ($this->sourceString['type'] !== 'string') {
            throw new OperandTypeException("Druhý operand instrukce STRI2INT musí být řetězec.");
        }
    
        if ($this->charIndex['type'] !== 'int') {
            throw new OperandTypeException("Třetí operand instrukce STRI2INT musí být celé číslo.");
        }

        if (!is_string($string) || !is_int($index)) {
            throw new OperandTypeException("Operandy pro STR2INT musí být řetězec a celé číslo.");
        }

        if ($index < 0 || $index >= mb_strlen($string)) {
            throw new StringOperationException("Indexace mimo řetězec vede na chybu 58.");
        }

        $ordValue = mb_ord($string[$index]);
        $frame->setVariableValue($this->targetVar['value'], $ordValue);
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
            if ($operand === $this->sourceString && !is_string($value)) {
                throw new OperandTypeException("Operand musí být řetězec.");
            } elseif ($operand === $this->charIndex && !is_int($value)) {
                throw new OperandTypeException("Operand musí být celé číslo.");
            }
            return $value;
        } else {
            if ($operand === $this->charIndex) {
                return intval($operand['value']);
            }
            return $operand['value'];
        }
    }
}
