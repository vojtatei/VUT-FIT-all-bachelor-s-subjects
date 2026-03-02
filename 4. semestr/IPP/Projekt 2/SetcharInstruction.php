<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\StringOperationException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Exception\VariableAccessException;
use IPP\Student\Frame;


class SetCharInstruction extends AbstractInstruction {

     /**
     * @var array{'type': string, 'value': mixed} $variable
     */
    private array $variable;

     /**
     * @var array{'type': int, 'value': mixed} $index
     */
    private array $index;

     /**
     * @var array{'type': string, 'value': mixed} $newChar
     */
    private array $newChar;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);

        $this->variable = $args[0];
        $this->index = $args[1];
        $this->newChar = $args[2];
    }

    public function execute(): void {
        $frame = $this->frames[0];

        $varName = $this->variable['value'];
        $index = $this->resolveValue($this->index, $frame);
        $newChar = $this->resolveValue($this->newChar, $frame);

        if (!$frame->isVariableDefined($varName)) {
            throw new VariableAccessException("Proměnná '{$varName}' není definována.");
        }

        $stringValue = $frame->getVariableValue($varName);
        
        // Kontrola, zda je hodnota proměnné null, což odpovídá hodnotě nil v IPPcode24
        if ($stringValue === null) {
            throw new ValueException("Hodnota proměnné '{$varName}' nemůže být nil pro operaci SETCHAR.");
        }

        if (!is_string($stringValue)) {
            throw new OperandTypeException("Hodnota proměnné '{$varName}' není řetězec.");
        }

        if (!is_int($index)) {
            throw new OperandTypeException("Index '{$index}' je mimo rozsah řetězce.");
        }

        if  ($index < 0 || $index >= strlen($stringValue)) {
            throw new StringOperationException("Index '{$index}' je mimo rozsah řetězce.");
        }
        
        if (!is_string($newChar))
        {
            throw new OperandTypeException("Nový znak pro SETCHAR musí být řetězec.");
        }

        if ($newChar === '') {
            throw new StringOperationException("Nový znak pro SETCHAR nesmí být prázdný.");
        }

        $stringValue[$index] = $newChar[0];
        $frame->setVariableValue($varName, $stringValue);
    }

    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveValue(array $operand, Frame $frame) : mixed {
        if ($operand['type'] === 'var') {
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                throw new ValueException("Hodnota proměnné '{$operand['value']}' není inicializovaná.");
            }
            return $value;
        } elseif ($operand['type'] === 'int') {
            return intval($operand['value']);
        } elseif ($operand['type'] === 'string') {
            return $operand['value'];
        } else {
            throw new OperandTypeException("Nepodporovaný typ operandu: {$operand['type']}.");
        }
    }
}
