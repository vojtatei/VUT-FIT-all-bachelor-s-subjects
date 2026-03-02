import sys
import xml.etree.ElementTree as ET
from xml.dom import minidom
from syntax import InvalidVariableFormatError, InvalidTypeError
from scanner import Scanner
from syntax import SyntaxAnalyzer

def create_xml_instruction(order, instruction):
    opcode = instruction['opcode']
    args = instruction['args']
    # Vytvoření elementu instrukce
    instruction_elem = ET.Element('instruction', order=str(order), opcode=opcode)

    # Přidání argumentů pokud existují
    for i, arg in enumerate(args, start=1):
        arg_type, arg_value = get_arg_type_and_value(arg, opcode)
        # Speciální úprava pro LABEL instrukci, kde se typ nesmí převést na 'type_here'
        if opcode == 'LABEL' and arg_type == 'type_here':
            arg_type = 'label'  # Nastavení správného typu pro LABEL
        arg_elem = ET.SubElement(instruction_elem, f'arg{i}', type=arg_type)
        arg_elem.text = str(arg_value)

    # Pro instrukce bez argumentů (jako je RETURN) nevložíme žádné argumenty
    if not args and opcode not in ['AND', 'OR', 'NOT']:
        # Vytvoření stringové reprezentace elementu s explicitním uzavíracím tagem
        instruction_str = ET.tostring(instruction_elem, 'unicode')
        # Přidání explicitního uzavíracího tagu
        instruction_str = instruction_str.replace('/>', '>\n    </instruction>')
        # Parsing stringu zpět do Elementu pro zachování kompatibility s ostatním kódem
        instruction_elem = ET.fromstring(instruction_str)
        
    return instruction_elem

def get_arg_type_and_value(arg, opcode=None):  # Přidáme volitelný argument opcode
    # Rozpoznání a vrácení typu a hodnoty argumentu
    if arg.startswith('GF@') or arg.startswith('LF@') or arg.startswith('TF@'):
        return 'var', arg
    elif arg.startswith('int@'):
        _, value = arg.split('@', 1)
        return 'int', value
    elif arg.startswith('bool@'):
        # Převedení booleovské hodnoty na malá písmena
        _, value = arg.split('@', 1)
        value = value.lower()
        if value in ['true', 'false']:
            return 'bool', value
        else:
            # Vyhodit chybu, pokud není hodnota 'true' nebo 'false'
            raise ValueError("Invalid boolean value: {}".format(value))
    elif arg.startswith('string@'):
        _, value = arg.split('@', 1)
        return 'string', value
    elif arg.startswith('nil@'):
        return 'nil', 'nil'
    elif opcode == 'READ' and arg in ['int', 'string', 'bool']:
        # Speciální případ pro instrukci READ, kde druhý argument je typ
        return 'type', arg
    elif arg[0].isalpha():
        # Assuming any argument starting with an alphabet character is a label
        return 'label', arg
    else:
        # Pro neznámé typy vrátí zástupný typ a hodnotu
        return 'type_here', arg

def print_help():
    help_text = """
    Usage: parse.py [OPTIONS]
    Options:
      --help            Show this help message and exit.
      <input_file>      Specify the input file with IPPcode24 source code to parse.

    The script reads IPPcode24 source code, checks lexical and syntactic correctness, 
    and outputs XML representation of the code to the standard output.
    """
    print(help_text)
    sys.exit(0)

def prettify_xml(element):
    rough_string = ET.tostring(element, 'unicode')
    reparsed = minidom.parseString(rough_string.encode('utf-8'))
    pretty_xml = reparsed.toprettyxml(indent="    ")
    xml_declaration = '<?xml version="1.0" encoding="UTF-8"?>\n'
    pretty_xml_with_correct_declaration = xml_declaration + pretty_xml.split('\n', 1)[1]
    
    return pretty_xml_with_correct_declaration



def main():
    try:
        # Přečte celý vstup ze standardního vstupu
        input_data = sys.stdin.read()
        scanner = Scanner(input_data)
        lines = scanner.get_lines()
        
        analyzer = SyntaxAnalyzer(lines)
        instructions = analyzer.parse_lines()

        program = ET.Element('program', language='IPPcode24')
        for order, instruction in enumerate(instructions, 1):
            if instruction['opcode'].lower() == 'read':
                _, type_arg = instruction['args']
                if type_arg not in ['int', 'bool', 'string']:
                    raise InvalidTypeError(f"Invalid type for READ instruction: {type_arg}")
            program.append(create_xml_instruction(order, instruction))
            

        # Získání hezky naformátovaného XML jako řetězce
        pretty_xml = prettify_xml(program)
        # Výpis hezky naformátovaného XML na standardní výstup
        sys.stdout.write(pretty_xml)

    except InvalidVariableFormatError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(23)  # Specifický chybový kód pro chybný formát proměnné

    except Exception as e:
        print(f"Internal error: {e}", file=sys.stderr)
        sys.exit(99)

if __name__ == '__main__':
    if len(sys.argv) == 2 and sys.argv[1] == '--help':
        print_help()
    elif len(sys.argv) == 1:
        main()
    else:
        print("Error: Incorrect number of arguments.", file=sys.stderr)
        sys.exit(10)
