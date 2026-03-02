<?php

namespace IPP\Student;

use IPP\Student\Exception\SemanticException;


class DefvarInstruction extends AbstractInstruction {

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
    }

    public function execute(): void {
        // Proměnná pro definici
        $varName = $this->args[0];
        // Získání příslušného rámu pro proměnnou
        $frame = $this->frames[0]; 

        try {
            $frame->defineVar($varName);

       }
       catch (SemanticException $e)
       {
           throw new SemanticException($e->getMessage());
       }
    }
}

