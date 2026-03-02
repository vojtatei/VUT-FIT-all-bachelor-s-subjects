<?php

namespace IPP\Student;

use DOMElement;
use IPP\Student\Exception\InvalidSource;


class InstructionValidator
{
    public function validateZeroArgs(DOMElement $instructionElement, string $opcode): void
{
    $arg1Elements = $instructionElement->getElementsByTagName('arg1');
    $arg2Elements = $instructionElement->getElementsByTagName('arg2');
    $arg3Elements = $instructionElement->getElementsByTagName('arg3');

    if ($arg1Elements->length > 0 || $arg2Elements->length > 0 || $arg3Elements->length > 0) {
        throw new InvalidSource("Instrukce '$opcode' nesmí obsahovat žádné argumenty.");
    }
}

    public function validateOneArg(DOMElement $instructionElement, string $opcode): void
    {
        $arg1Length = $instructionElement->getElementsByTagName('arg1')->length;
        $arg2Length = $instructionElement->getElementsByTagName('arg2')->length;

        if ($arg1Length !== 1 || $arg2Length > 0) {
            throw new InvalidSource("Instrukce '$opcode' musí obsahovat právě jeden argument 'arg1'.");
        }
    }

    public function validateTwoArgs(DOMElement $instructionElement, string $opcode): void
    {
        $arg1Length = $instructionElement->getElementsByTagName('arg1')->length;
        $arg2Length = $instructionElement->getElementsByTagName('arg2')->length;
        $arg3Length = $instructionElement->getElementsByTagName('arg3')->length; // Přidáno pro kontrolu

        if ($arg1Length !== 1 || $arg2Length !== 1 || $arg3Length > 0) { 
            throw new InvalidSource("Instrukce '$opcode' musí obsahovat právě dva argumenty 'arg1' a 'arg2', a nesmí obsahovat 'arg3'.");
        }
    }

    public function validateThreeArgs(DOMElement $instructionElement, string $opcode): void
    {
        $arg1Length = $instructionElement->getElementsByTagName('arg1')->length;
        $arg2Length = $instructionElement->getElementsByTagName('arg2')->length;
        $arg3Length = $instructionElement->getElementsByTagName('arg3')->length;
        $arg4Length = $instructionElement->getElementsByTagName('arg4')->length;

        if ($arg1Length !== 1 ||
            $arg2Length !== 1 ||
            $arg3Length !== 1 ||
            $arg4Length > 0) {
            throw new InvalidSource("Instrukce '$opcode' musí obsahovat právě tři argumenty 'arg1', 'arg2' a 'arg3'.");
        }
    }
}
