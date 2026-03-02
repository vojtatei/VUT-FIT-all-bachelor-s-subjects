import sys
import re

class SyntaxAnalyzer:
    def __init__(self, lines):
        self.lines = lines
        self.instructions_definition = {
            'MOVE': ['var', 'symb'],
            'CREATEFRAME': [],
            'PUSHFRAME': [],
            'POPFRAME': [],
            'DEFVAR': ['var'],
            'CALL': ['label'],
            'RETURN': [],
            'PUSHS': ['symb'],
            'POPS': ['var'],
            'ADD': ['var', 'symb', 'symb'],
            'SUB': ['var', 'symb', 'symb'],
            'MUL': ['var', 'symb', 'symb'],
            'IDIV': ['var', 'symb', 'symb'],
            'LT': ['var', 'symb', 'symb'],
            'GT': ['var', 'symb', 'symb'],
            'EQ': ['var', 'symb', 'symb'],
            'AND': ['var', 'symb', 'symb'],
            'OR': ['var', 'symb', 'symb'],
            'NOT': ['var', 'symb'],
            'INT2CHAR': ['var', 'symb'],
            'STRI2INT': ['var', 'symb', 'symb'],
            'READ': ['var', 'type'],
            'WRITE': ['symb'],
            'CONCAT': ['var', 'symb', 'symb'],
            'STRLEN': ['var', 'symb'],
            'GETCHAR': ['var', 'symb', 'symb'],
            'SETCHAR': ['var', 'symb', 'symb'],
            'TYPE': ['var', 'symb'],
            'LABEL': ['label'],
            'JUMP': ['label'],
            'JUMPIFEQ': ['label', 'symb', 'symb'],
            'JUMPIFNEQ': ['label', 'symb', 'symb'],
            'EXIT': ['symb'],
            'DPRINT': ['symb'],
            'BREAK': [],
        }

    def parse_lines(self):
        instructions = []
        for line in self.lines:
            # Odstranění komentářů z řádku
            line, _, _ = line.partition('#')  # Rozdělí řádek na část před # a ignoruje zbytek
            parts = line.split(maxsplit=1)
            if parts:
                opcode = parts[0].upper()
                if opcode in self.instructions_definition:
                    args = parts[1].split() if len(parts) > 1 else []
                    # Kontrola shody počtu argumentů s definicí
                    if len(args) != len(self.instructions_definition[opcode]):
                        print(f"Invalid number of arguments for {opcode}: expected {len(self.instructions_definition[opcode])}, got {len(args)}")
                        sys.exit(23)
                    if opcode in ['JUMP', 'JUMPIFEQ', 'JUMPIFNEQ', 'LABEL', 'CALL']:
                        label = args[0]
                        if not self.is_valid_label(label):
                            sys.exit(23)
                        if not self.validate_jump_args(opcode, args):
                            sys.exit(23)
                    instruction = {'opcode': opcode, 'args': args}
                    self.check_syntax(instruction)  # Kontrola syntaxe instrukce
                    instructions.append(instruction)
                else:
                    sys.exit(23)
        return instructions

    def check_syntax(self, instruction):
        opcode = instruction['opcode']
        args = instruction['args']
        if opcode in ['JUMP', 'JUMPIFEQ', 'JUMPIFNEQ']:
            if not self.validate_jump_args(opcode, args):
                sys.exit(23)
        expected_arg_types = self.instructions_definition[opcode]

        for arg, expected_type in zip(args, expected_arg_types):
            if expected_type == 'var' and not self.is_valid_variable(arg):
                raise InvalidVariableFormatError(f"Invalid variable format: {arg}")
            elif expected_type == 'symb':
                if not self.is_valid_symbol(arg):
                    sys.exit(23)
                # Přidání další logiky pro kontrolu správného formátu booleovských hodnot
                if arg.startswith('bool@'):
                    bool_value = arg.split('@')[1].lower()
                    if bool_value not in ['true', 'false']:
                        raise ValueError(f"Invalid boolean format: {arg}")
            elif opcode == 'READ' and expected_type == 'type':
                if arg.lower() not in ['int', 'bool', 'string']:
                    raise InvalidTypeError(f"Invalid type for READ instruction: {arg}")
    
    def process_argument(arg):
        arg_type, arg_value = arg.split('@', 1)
        if arg_type == 'int':
            # Podpora hexadecimálních čísel
            if arg_value.startswith('-'):
                # Pro záporná hexadecimální čísla
                sign = -1
                arg_value = arg_value[1:]
            else:
                sign = 1
            if arg_value.startswith('0x'):
                # Odstraní prefix '0x' pro hexadecimální čísla
                base = 16
                arg_value = arg_value[2:]
            elif arg_value.startswith('0o'):
                # Odstraní prefix '0o' pro oktalová čísla
                base = 8
                arg_value = arg_value[2:]
                if not all(c in '01234567' for c in arg_value):
                    sys.exit(23)
            else:
                base = 10
            try:
                # Převod hexadecimálního nebo dekadického celého čísla na int
                int_value = sign * int(arg_value, base)
            except ValueError:
                raise ValueError(f"Invalid integer value: {arg_value}")
            return arg_type, str(int_value)  # Vrátí hodnotu jako řetězec
        else:
        # Zpracování ostatních typů argumentů zůstává beze změny
            return arg_type, arg_value
        
    def validate_jump_args(self, opcode, args):
        if opcode in ['JUMPIFEQ', 'JUMPIFNEQ']:
            if len(args) != 3 or not self.is_valid_label(args[0]) or not all(self.is_valid_symbol(arg) for arg in args[1:]):
                return False
        elif opcode == 'JUMP' and (len(args) != 1 or not self.is_valid_label(args[0])):
            return False
        return True
    
    @staticmethod
    def is_valid_label(label):
    # Štítky nesmí obsahovat '@', '/', '\', a nesmí začínat klíčovými slovy pro typy
        invalid_start = ('bool@', 'int@', 'string@', 'nil@', 'float@')
        if any(label.startswith(start) for start in invalid_start) or '@' in label or '/' in label or '\\' in label:
            return False
        # Kontrola, zda se jedná o platný identifikátor štítku podle specifikace IPPcode24
        return re.match(r'^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$', label) is not None

    @staticmethod
    def is_valid_variable(arg):
        if not arg.startswith(('GF@', 'LF@', 'TF@')):
            return False
        var_name = arg.split('@', 1)[1]
        return re.match(r'^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$', var_name) is not None

    @staticmethod
    def is_valid_symbol(arg):
        if SyntaxAnalyzer.is_valid_variable(arg):
            return True
        # Rozpoznání celočíselných literálů a hexadecimálních čísel
        if arg.startswith('int@'):
            value = arg[4:]  # Odstranění prefixu 'int@'
            try:
                # Kontrola pro oktalové hodnoty
                if value.startswith('0o') or value.startswith('0O'):
                    int(value, 8)  # Pokus o převod hodnoty na oktalové číslo
                elif value.startswith('-0o') or value.startswith('-0O'):
                    int(value[1:], 8)  # Pokus o převod hodnoty na záporné oktalové číslo
                elif value.startswith('0x') or value.startswith('-0x'):
                    int(value, 16)  # Pokus o převod hodnoty na hexadecimální číslo
                else:
                    int(value)  # Pokus o převod hodnoty na celé číslo
                return True
            except ValueError:
               sys.exit(23)
        # Rozpoznání reálných čísel
        elif arg.startswith('bool@'):
            # Kontrola pro booleovské hodnoty
            value = arg[5:]  # Odstranění prefixu 'bool@'
            return value.lower() in ['true', 'false']
        elif arg.startswith('string@'):
            value = arg[7:]  # Odstranění prefixu 'string@'
        # Kontrola, zda hodnota je prázdný řetězec nebo platný řetězec
            if value == '' or SyntaxAnalyzer.is_valid_string_literal(value):
                return True
            else:
                return False
        elif arg.startswith('nil@'):
            return arg == 'nil@nil'
        
        else:
            try:
                float(arg)
                return True
            except ValueError:
                return False
    
    @staticmethod
    def is_valid_string_literal(value):
        # Kontrola, zda řetězec obsahuje pouze povolené znaky a escape sekvence
        if not re.match(r'^([^\s#\\]|(\\[0-9]{3}))*$', value):
            return False

        # Kontrola, zda všechny escape sekvence odpovídají platným Unicode znakům
        escape_sequences = re.findall(r'\\([0-9]{3})', value)
        for seq in escape_sequences:
            if int(seq) > 255: 
                return False

        return True
    
class InvalidVariableFormatError(Exception):
    pass

class InvalidTypeError(InvalidVariableFormatError):
    pass
