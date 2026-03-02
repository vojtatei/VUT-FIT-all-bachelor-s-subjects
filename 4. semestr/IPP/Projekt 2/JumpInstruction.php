<?php
namespace IPP\Student;

use IPP\Student\Exception\SemanticException;


class JumpInstruction extends AbstractInstruction {
    public string $label;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        $this->label = $args[0];
    }
    

    public function execute(): void {
        $globalFrame = $this->frames[0];
        if (!$globalFrame->isLabelDefined($this->label)) {
            throw new SemanticException("Štítek '{$this->label}' není definován.");
        }
        
        $instructionIndex = $globalFrame->getLabelIndex($this->label);
        $globalFrame->setInstructionPointer($instructionIndex);
    }
}
