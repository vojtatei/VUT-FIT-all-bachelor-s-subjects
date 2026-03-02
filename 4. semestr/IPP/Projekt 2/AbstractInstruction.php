<?php

namespace IPP\Student;



abstract class AbstractInstruction {
    /**
     * @var mixed[] $args Arguments for the instruction
     */
    protected array $args;

    /**
     * @var int $order Order of the instruction
     */
    protected int $order;

    /**
     * @var Frame[] $frames The frames associated with the instruction
     */
    protected array $frames;

    /**
     * Constructor for abstract instruction.
     * 
     * @param mixed[] $args Arguments for the instruction
     * @param int $order Order of the instruction
     * @param Frame[] $frames Array of frames associated with the instruction
     */
    public function __construct(array $args, int $order, array $frames) {
        $this->args = $args;
        $this->order = $order;
        $this->frames = $frames;
    }

    /**
     * Get the order of the instruction.
     * 
     * @return int The order of the instruction
     */
    public function getOrder(): int {
        return $this->order;
    }

    /**
     * Execute the instruction.
     */
    abstract public function execute(): void;
}
