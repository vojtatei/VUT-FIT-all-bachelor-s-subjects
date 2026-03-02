<?php
namespace IPP\Student;

use IPP\Student\Exception\SemanticException;
use IPP\Student\Exception\FrameAccessException;
use IPP\Student\Exception\ValueException;
use IPP\Student\Exception\VariableAccessException;


class Frame {

    /**
     * @var Variable[] $variables 
     */
    private array $variables = [];

    /**
     * @var array<int> $callStack
     */
    private array $callStack = [];
    private int $instructionPointer = 1;
    private ?Frame $temporaryFrame = null;

    /**
 * @var array<string, int> $labels Map of label names to instruction indexes.
 */
    private array $labels = [];
    

    public function defineVar(string $name): void {

        if ($this->isVariableDefined($name)) {
            throw new SemanticException("Proměnná '$name' již byla definována.");
        }
        
        $this->variables[$name] = new Variable($name);
    }

    public function reset(): void {
        $this->variables = [];
        $this->instructionPointer = 1;
        $this->labels = [];
    }


    public function isVariableDefined(string $name): bool {
        $cleanName = trim($name);
        return array_key_exists($cleanName, $this->variables);
    }

    public function isArgumentVariable(string $argument): bool {
        return preg_match('/^(GF|LF|TF)@/', $argument) === 1;
    }
    
    public function getVariableValue(string $name): mixed {
        $cleanName = trim($name);
       // echo "je? $name\n";
        if (!$this->isVariableDefined($cleanName)) {
            throw new VariableAccessException("Proměnná '$cleanName' není definována.");
        }
        return $this->variables[$cleanName]->getValue();
    }

    public function setVariableValueFrame(string $name, mixed $value): void {
        $cleanName = trim($name);
        if (!$this->isVariableDefined($cleanName)) { 
            throw new FrameAccessException("Proměnná '$cleanName' není definována");
        }
        $this->variables[$cleanName]->setValue($value);
    }
    
    public function setVariableValue(string $name, mixed $value): void {
        $cleanName = trim($name);
        if (!$this->isVariableDefined($cleanName)) {
            throw new VariableAccessException("Proměnná '$cleanName' není definována");
        }
        $this->variables[$cleanName]->setValue($value);
    }
    

    public function getVariable(string $name): Variable {
        if ($this->isArgumentVariable($name) && !$this->isVariableDefined($name)) {
            throw new VariableAccessException("Proměnná '$name' není definována.");
        }
        return $this->variables[$name];
    }

    public function registerLabel(string $labelName, int $instructionIndex): void {
        if (isset($this->labels[$labelName])) {
            throw new SemanticException("Štítek '$labelName' už byl dříve definován.");
        }
        $this->labels[$labelName] = $instructionIndex;
    }

    public function getLabelIndex(string $labelName): int {
        
        if (!isset($this->labels[$labelName])) {
            throw new SemanticException("Štítek '$labelName' není definován.");
        }
        return $this->labels[$labelName];
    }

    public function setInstructionPointer(int $instructionIndex): void {
        $this->instructionPointer = $instructionIndex;
        
        
    }

    public function getInstructionPointer() : int {
        return $this->instructionPointer;
    }

    public function defineLabel(string $label, int $instructionIndex) : void {
        if (array_key_exists($label, $this->labels)) {
            
            throw new SemanticException("Štítek '$label' již existuje.");
        }
        $this->labels[$label] = $instructionIndex;
    }

    public function getInstructionIndexForLabel(string $label): int {
        if (!isset($this->labels[$label])) {
            throw new SemanticException("Štítek '{$label}' není definován.");
        }
        return $this->labels[$label];
    }

    public function isLabelDefined(string $labelName): bool {

        $isDefined = isset($this->labels[$labelName]);
        if (!$isDefined) {

        }
        return $isDefined;
    }

    public function pushCallStack(int $returnPosition) : void {
        array_push($this->callStack, $returnPosition);
    }

    public function popCallStack() : int {
        if (empty($this->callStack)) {
            throw new ValueException("Zásobník volání je prázdný.");
        }
        return array_pop($this->callStack);
    }

 
    public function popCallStackItem() : mixed {
        if (empty($this->callStack)) {
            throw new ValueException("Zásobník volání je prázdný.");
        }
        return array_pop($this->callStack);
    }


    public function initializeTemporaryFrame(): void {
        $this->temporaryFrame = new Frame();
    }

    public function isTemporaryFrameInitialized(): bool {
        return $this->temporaryFrame !== null;
    }

    // Metoda pro kontrolu, zda je proměnná inicializovaná.
    public function isVariableInitialized(string $name): bool
    {
        // Kontrolujeme, zda proměnná existuje a není null.
        return isset($this->variables[$name]) && $this->variables[$name]->isInitialized();
    }

    public function isVariableInitialized2(string $name): bool {
    // Kontrola existence proměnné a její hodnoty
    return isset($this->variables[$name]) && $this->variables[$name]->getValue() !== null;
}


    /**
     * Sets the variables for the frame
     *
     * @param array<string, Variable> $variables The variables to set
     * @return void
     */
    public function setVariables(array $variables): void {
        $this->variables = $variables;
    }
    
    /**
     * Gets all the variables in the frame
     *
     * @return array<string, Variable> The variables in the frame
     */
    public function getVariables(): array {
        return $this->variables;
    }

    public function undefineVar(string $name): void {
        $cleanName = trim($name);
        if ($this->isVariableDefined($cleanName)) {
            unset($this->variables[$cleanName]);
        } else {
            throw new SemanticException("Proměnná '$cleanName' není definována, nelze zrušit definici.");
        }
    }

    public function isVariableSet(string $variableName): bool {

        return isset($this->variables[$variableName]) && $this->variables[$variableName] !== null;
    }

}