<?php


namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\ValueException;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\OperandValueException;


class ExitInstruction extends AbstractInstruction {

    /**
     * @var mixed The value to be used for exit.
     */
    private $exitValue;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        if (count($args) !== 1) {
            throw new OperandTypeException("EXIT instruction expects exactly one argument.");
        }
        $this->exitValue = $args[0];
    }

    public function execute(): void {
        $exitCode = $this->resolveOperandValue($this->exitValue, $this->frames[0]);

        if ($exitCode === null) {
            throw new ValueException("Nenastavená hodnota");
        }

        if (!is_int($exitCode) || $exitCode < 0 || $exitCode > 9) {
            throw new OperandValueException("Exit code for EXIT must be an integer between 0 and 9.");
        }

        // Ukončení interpretace s daným návratovým kódem
        exit($exitCode);
    }

    /**
     * Resolves the value of an operand.
     *
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to retrieve the value.
     */
    private function resolveOperandValue(array $operand, Frame $frame) : mixed {
        if ($operand['type'] === 'var') {
            return $frame->getVariableValue($operand['value']);
        } elseif ($operand['type'] === 'int') {
            return intval($operand['value']);
        } else {
            throw new OperandTypeException("Unsupported operand type for EXIT.");
        }
    }
}
