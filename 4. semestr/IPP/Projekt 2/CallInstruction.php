<?php

namespace IPP\Student;

use IPP\Student\Exception\InvalidSource;
use IPP\Student\Exception\SemanticException;


class CallInstruction extends AbstractInstruction {
    private string $label;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);

        if (!isset($args[0]) || !$args[0]['value']) {
            throw new InvalidSource("Není zadán štítek pro instrukci CALL.");
        }

        $this->label = $args[0]['value'];
    }

    public function execute(): void {
        // Získání správného rámce pro label
        $frame = $this->frames[0]; 
        
        // Uložení návratové adresy (pozice následující instrukce)
        $returnPosition = $this->order + 1;
        $frame->pushCallStack($returnPosition);

        // Skok na zadaný štítek
        if (!$frame->isLabelDefined($this->label)) {
            throw new SemanticException("Štítek '{$this->label}' nebyl nalezen.");
        }

        $position = $frame->getLabelIndex($this->label);
        $frame->setInstructionPointer($position);
    }
}
