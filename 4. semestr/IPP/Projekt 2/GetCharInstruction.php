<?php

namespace IPP\Student;

use IPP\Student\AbstractInstruction;
use IPP\Student\Exception\OperandTypeException;
use IPP\Student\Exception\StringOperationException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Frame;


class GetCharInstruction extends AbstractInstruction {
     /**
     * @var array{type: string, value: mixed} The target variable where the result will be stored.
     */
    private array $variable;

    /**
     * @var array{type: string, value: mixed} The string or variable containing the string.
     */
    private array $string;


     /**
     * @var array{type: string, value: mixed} The position of the character in the string.
     */
    private array $position;

    public function __construct(array $args, int $order, array $frames) {
        parent::__construct($args, $order, $frames);
        $this->variable = $args[0]; // Cílová proměnná
        $this->string = $args[1]; // Řetězec nebo proměnná obsahující řetězec
        $this->position = $args[2]; // Pozice znaku v řetězci
    }

    public function execute(): void {
        $string = $this->resolveValue($this->string, $this->frames[1]);
        $position = $this->resolveValue($this->position, $this->frames[2]);

        if ($string === null || $position === null) {
            throw new ValueException("Operandy pro GETCHAR nejsou nastaveny.");
        }


        if (!is_string($string) || !is_int($position)) {
            throw new OperandTypeException("Operandy GETCHAR musí být řetězec a celé číslo.");
        }

        if ($position < 0 || $position >= strlen($string)) {
            throw new StringOperationException("Indexace mimo řetězec vede na chybu 58.");
        }


        $char = $string[$position]; // Získá znak z řetězce na dané pozici
        $this->frames[0]->setVariableValue($this->variable['value'], $char);
    }

    /**
     * @param mixed[] $operand
     * @param Frame $frame
     * @return mixed
     */
    private function resolveValue(array $operand, Frame $frame) : mixed {
        // Kontroluje, zda je operand typu var nebo literál 
        if ($operand['type'] === 'var') {
            // Získání hodnoty proměnné
            
            return $frame->getVariableValue($operand['value']);
        } else {
            // Pokud operand není proměnná, předpokládáme, že je to literál a vrátíme ho přímo
            return $operand['value'];
        }
    }
}
