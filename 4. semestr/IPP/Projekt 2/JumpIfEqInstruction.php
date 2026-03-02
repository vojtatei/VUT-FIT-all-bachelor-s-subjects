<?php
namespace IPP\Student;

use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\SemanticException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Exception\VariableAccessException;

class JumpIfEqInstruction extends AbstractInstruction {
    private string $label;
    private mixed $operand1;
    private mixed $operand2;
    public static bool $shouldJump = false;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        $this->label = $args[0]; // Štítek pro skok
        $this->operand1 = $args[1]; // První operand pro porovnání
        $this->operand2 = $args[2]; // Druhý operand pro porovnání
        self::$shouldJump = false;
    }

    public function execute(): void {
        $frameForOperand1 = $this->frames[1];
        $frameForOperand2 = $this->frames[2];

        if (!$this->frames[0]->isLabelDefined($this->label)) {
            throw new SemanticException("Štítek '{$this->label}' není definován.");
        }

        $operand1Value = $this->resolveOperandValue($this->operand1, $frameForOperand1);
        $operand2Value = $this->resolveOperandValue($this->operand2, $frameForOperand2);

        if ($this->operand1['type'] === 'var' && $operand1Value === null) {
            throw new ValueException("Proměnná '{$this->operand1['value']}' není inicializovaná.");
        }
        if ($this->operand2['type'] === 'var' && $operand2Value === null) {
            throw new ValueException("Proměnná '{$this->operand2['value']}' není inicializovaná.");
        }


        if ($operand1Value === null || $operand2Value === null) {
            // Jeden z operandů je 'nil', není třeba nic dalšího kontrolovat
        } else {
            // Zkontrolujte typy hodnot
            if (gettype($operand1Value) !== gettype($operand2Value)) {
                throw new OperandTypeException("Operandy nejsou stejného typu.");
            }
        }

        if ($operand1Value === $operand2Value) {
            $labelIndex = $this->frames[0]->getLabelIndex($this->label);
            $this->frames[0]->setInstructionPointer($labelIndex);
            self::$shouldJump = true;
        } else {
            self::$shouldJump = false;
        }
    }

    private function resolveOperandValue(mixed $operand, Frame $frame) : mixed {
        if (is_array($operand)) {
            if ($operand['type'] === 'var') {
                if (!$frame->isVariableDefined($operand['value'])) {
                    throw new VariableAccessException("Proměnná '{$operand['value']}' není definována.");
                }
                return $frame->getVariableValue($operand['value']);
            } else {
                return $operand['value']; // Operand je literál a má již definovaný typ
            }
        } else {
            return $operand; // Operand není pole, takže se předpokládá, že je to literál
        }
    }
}