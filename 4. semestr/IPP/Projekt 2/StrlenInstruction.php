<?php

namespace IPP\Student;

use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\SemanticException;
use IPP\Student\Exception\ValueException;


class StrlenInstruction extends AbstractInstruction
{
    protected string $varName;

    /** @var mixed[] $stringOperand */
    protected array $stringOperand;

    public function __construct(array $args, int $order, array $frames)
    {
        parent::__construct($args, $order, $frames);
        $this->varName = $args[0];
        $this->stringOperand = $args[1];
    }

    public function execute(): void
    {
        
        $string = $this->resolveOperand($this->stringOperand['value'], $this->stringOperand['type'], $this->frames[1]);

        if ($string === null) {
            throw new ValueException("Operand pro STRLEN musí být string");
        }

        if (!is_string($string)) {
            throw new OperandTypeException("Operand pro STRLEN musí být string");
        }

        $length = strlen($string);
        $this->frames[0]->setVariableValue($this->varName, $length);
    }

    /**
     * @param mixed $operandValue Value of the operand.
     * @param string $operandType Type of the operand.
     * @param Frame $frame Frame in which to resolve the operand.
     * @return mixed The resolved value of the operand.
     */
    private function resolveOperand($operandValue, $operandType, $frame)
    {
    if ($operandType === 'var') {
        // Pokud je operand proměnná, získáme její hodnotu
        $value = $frame->getVariableValue($operandValue);
        // Kontrola, zda získaná hodnota je string, pokud ne, vyhodíme výjimku
        if (!$frame->isVariableDefined($operandValue)) {
            throw new SemanticException("Promenná '$operandValue' není definovaná.");
        }
        if (!$frame->isVariableInitialized($operandValue)) {
            throw new ValueException("Proměnná '$operandValue' není definovaná.");
        }
        if ($value === '') { 
            throw new OperandTypeException("Operand pro STRLEN nemůže být nil");
        }
        return $value;
    } elseif ($operandType === 'string') {
        return $operandValue;
    } else {
        // Pro jakýkoli jiný typ operandu vyhodíme výjimku
        throw new OperandTypeException("Operand musí být string nebo proměnná se stringem");
    }
}
}
