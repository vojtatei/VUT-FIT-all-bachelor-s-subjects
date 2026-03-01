import unittest
from swift_lexer import SwiftLexer  # Předpokládáme, že máte implementovaný lexer pro Swift

def test_let_token():
    code = "let"
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'LET'

def test_identifier_token():
    code = "x"
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'IDENTIFIER'

def test_equal_token():
    code = "="
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'EQUAL'

def test_integer_token():
    code = "42"
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'INTEGER'

def test_semicolon_token():
    code = ";"
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'SEMICOLON'

def test_invalid_token():
    code = "13f"
    lexer = SwiftLexer(code)

    assert lexer.get_next_token() == 'ERROR'  # Očekáváme chybový token

if __name__ == '__main__':
    unittest.main()
