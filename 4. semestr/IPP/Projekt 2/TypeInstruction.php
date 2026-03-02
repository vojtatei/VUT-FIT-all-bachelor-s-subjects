<?php

namespace IPP\Student;



class TypeInstruction extends AbstractInstruction {
    /**
     * @var array{'type': string, 'value': mixed} $var
     */
    private array $var;

    /**
     * @var array{'type': string, 'value': mixed} $symb
     */
    private array $symb;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        $this->var = $args[0];
        $this->symb = $args[1];
    }

    public function execute(): void {
        // Získáme hodnotu symb operandu
        $symbValue = $this->resolveOperandValue($this->symb, $this->frames[1]);

        
        // Zjistíme typ operandu na základě jeho hodnoty.
        $type = $this->determineType($symbValue);
    
        // Uložíme zjištěný typ do proměnné
        $this->frames[0]->setVariableValue($this->var['value'], $type);
    }
    
    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue(array $operand, Frame $frame): mixed {
        // Pokud je operand typu var, zjistíme, zda je proměnná inicializovaná
        if ($operand['type'] === 'var') {
            // Získání hodnoty proměnné
            return $frame->getVariableValue($operand['value']);
        } else {
            // Pokud je operand literál, vrátíme jeho hodnotu přímo
            return $operand['value'];
        }
    }
    
    private function determineType(mixed $value): string {
        if ($value === null) {
            return ''; // Prázdný řetězec pro nil hodnotu
        } elseif (is_int($value)) {
            return 'int';
        } elseif (is_bool($value)) {
            return 'bool';
        } elseif (is_string($value)) {
            return 'string';
        }
        return ''; 
    }

}
