<?php

namespace IPP\Student;


use DOMDocument;
use DOMXPath;
use DOMElement;
use IPP\Student\Exception\InvalidSource;

class XMLProcessor
{
    private DOMXPath $xpath;
    private Frame $interpreter;

    public function __construct(DOMDocument $domDocument, Frame $interpreter)
    {
        $this->xpath = new DOMXPath($domDocument);
        $this->interpreter = $interpreter;
    }

    public function validateRootElement(): void
    {
        $rootElement = $this->xpath->query('/program');
        if ($rootElement->length === 0) {
            throw new InvalidSource("Nesprávný kořenový element, očekává se <program>.");
        }
    }

    /**
     * @return DOMElement[] Array of instruction elements indexed by their order.
     */
    public function getInstructions(): array
    {
        $instructionElements = $this->xpath->query('/program/*');
        $instructions = [];
        if ($instructionElements === false || $instructionElements->length === 0) {
            return $instructions; 
        }

        foreach ($instructionElements as $element) {
            /** @var DOMElement $element */
            if ($element->nodeName !== 'instruction') {
                throw new InvalidSource("Nalezen neplatný element: " . $element->nodeName);
            }
            if (!$element instanceof DOMElement) continue;
            $order = intval($element->getAttribute('order'));

            if ($order <= 0) {
                throw new InvalidSource("Atribut 'order' instrukce musí být kladné číslo, zjištěno: $order.");
            }
            if (array_key_exists($order, $instructions)) {
                throw new InvalidSource("Duplicitní 'order' hodnota: $order");
            }
            $instructions[$order] = $element;
            $opcode = $element->getAttribute('opcode');

            if (!$opcode) {
                throw new InvalidSource("Instrukce nemá 'opcode' atribut.");
            }
    
            $validArgNames = ['arg1', 'arg2', 'arg3']; // seznam platných názvů argumentů
            $args = $element->childNodes;
            foreach ($args as $arg) {
                /** @var DOMElement $arg */
                if ($arg->nodeType !== XML_ELEMENT_NODE) {
                    continue; // Přeskočit, pokud to není element
                }
                $argName = $arg->nodeName;
                if (!in_array($argName, $validArgNames)) {
                    throw new InvalidSource("Neplatný element argumentu: $argName");
                }
            }
    
            if ($opcode === "LABEL") {
                $labelName = trim($element->getElementsByTagName('arg1')->item(0)->nodeValue);
                $this->interpreter->registerLabel($labelName, $order);
            }
        }
        ksort($instructions); // Zajistí, že instrukce jsou seřazené podle 'order'
        return $instructions;
    }
}
