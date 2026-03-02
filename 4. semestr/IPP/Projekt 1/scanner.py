import sys
import re

class Scanner:
    def __init__(self, data):
        self.data = data

    def get_lines(self):
        lines = self.data.splitlines()
        processed_lines = []
        header_found = False

        # Regulární výraz pro identifikaci hlavičky a odstranění komentářů
        header_pattern = re.compile(r'^\.IPPcode24', re.IGNORECASE)
        comment_pattern = re.compile(r'#.*')

        for line in lines:
            # Odstranění komentářů
            line = comment_pattern.sub('', line).strip()
            
            # Kontrola hlavičky, pokud ještě nebyla nalezena
            if not header_found and header_pattern.match(line):
                if header_found:
                    sys.exit(23) #mooooc hlaviček
                header_found = True
                continue  # Přeskočení hlavičky

            elif not header_found and line:
                sys.exit(21)
            
            elif header_found and line:
                processed_lines.append(line)

        if not header_found:
            sys.exit(21)

        return processed_lines