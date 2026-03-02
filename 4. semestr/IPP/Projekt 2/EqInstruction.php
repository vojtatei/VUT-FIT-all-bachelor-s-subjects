<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Frame;


class EqInstruction extends AbstractInstruction {


    /**
     * @var array{'type': string, 'value': mixed} $var
     */
    private array $var;

    /**
     * @var array{'type': string, 'value': mixed} $symb1
     */
    private array $symb1;

    /**
     * @var array{'type': string, 'value': mixed} $symb2
     */
    private array $symb2;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);

        // Argumenty jsou ve správném pořadí podle specifikace IPPcode24
        $this->var = $args[0];
        $this->symb1 = $args[1];
        $this->symb2 = $args[2];
    }

    public function execute(): void {
        $operand1 = $this->resolveOperandValue($this->symb1, $this->frames[1]);
        $operand2 = $this->resolveOperandValue($this->symb2, $this->frames[1]);
    
        // Normalizace 'nil' literálů na null pro konzistentní typové porovnání
        $operand1 = $operand1 === '' ? null : $operand1;
        $operand2 = $operand2 === '' ? null : $operand2;
    
        if (($operand1 === null && $operand2 !== null) || ($operand1 !== null && $operand2 === null)) {
            $result = false; // Pokud je jeden operand null a druhý není, výsledek je vždy false
        } else if ($operand1 === null && $operand2 === null) {
            $result = true; // Pokud jsou oba operandy null, výsledek je true
        } else if (gettype($operand1) !== gettype($operand2)) {
            throw new OperandTypeException("Operandy musí být stejného typu nebo oba 'nil' pro operaci EQ.");
        } else {
            // Provádíme porovnání, pokud jsou typy stejné a nejsou null
            $result = $operand1 == $operand2;
        }
    
        $this->frames[0]->setVariableValue($this->var['value'], $result);
    }

     /**
     * @param mixed[] $operand The operand to resolve.
     * @param Frame $frame The frame from which to get the variable's value.
     */
    private function resolveOperandValue(array $operand, Frame $frame): mixed {
        if ($operand['type'] === 'var') {
            $value = $frame->getVariableValue($operand['value']);
            if ($value === null) {
                throw new ValueException("Proměnná '{$operand['value']}' není definována.");
            }
            if (is_string($value)) {
                $value = StringDecoder::decodeEscapeSequences($value);
            }
            return $value;
        } else {
            //echo $operand['type'];
            return $operand['value'] === 'nil' ? '' : $operand['value'];
        }
    }
}
