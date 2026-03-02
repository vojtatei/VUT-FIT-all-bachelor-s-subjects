<?php

namespace IPP\Student;


use IPP\Student\Frame;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\ValueException;


class ConcatInstruction extends AbstractInstruction {

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
    }

    public function execute(): void {
        // Předpokládáme, že $args[0] je cílová proměnná, $args[1] a $args[2] jsou operandy
        $operand1 = $this->resolveOperandValue($this->args[1], $this->frames[1]);
        $operand2 = $this->resolveOperandValue($this->args[2], $this->frames[2]);
        
        $result = $operand1 . $operand2;

        // Zde nastavíme výsledek do cílové proměnné v rámci $frames[0]
        $this->frames[0]->setVariableValue($this->args[0]['value'], $result);
    }
     
    /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue($operand, Frame $frame) : mixed{
         // Kontroluje, zda je operand typu var nebo literál (např. int)
 
         if (!is_array($operand) || !isset($operand['type']) || !isset($operand['value'])) {
             throw new \InvalidArgumentException("Operand musí být pole s klíči 'type' a 'value'.");
         }
 
 
         if ($operand['type'] === 'var') {
             $value = $frame->getVariableValue($operand['value']);
             if ($value === null) {
                 throw new ValueException("Proměnná '{$operand['value']}' není definována.");
             }
             if (!is_string($value)) {
                throw new OperandTypeException("Hodnota proměnné '{$operand['value']}' musí být typu string.");
            }
             return $value;
         } elseif ($operand['type'] === 'string') {
             // Přímé vrácení číselného literálu
 
             return $operand['value'];
         } else {
             throw new OperandTypeException("Neplatný typ operandu: {$operand['type']}.");
         }
     }
 }