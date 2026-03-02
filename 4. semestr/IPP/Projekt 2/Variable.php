<?php
namespace IPP\Student;



class Variable {
    private string $name;
    private mixed $value;

    public function __construct(string $name) {
        $this->name = $name;
        $this->value = null;
    }

    public function setValue(mixed $value): void {
        // Nastaví hodnotu proměnné
        $this->value = $value;
    }
    
    public function getValue(): mixed {
        // Vrátí hodnotu proměnné
        return $this->value;
    }

    public function getName(): string {
        // Vrátí název proměnné
        return $this->name;
    }

    public function isInitialized(): bool {
        return $this->value !== null;
    }
}