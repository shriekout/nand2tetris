import sys

A_COMMAND = "A_COMMAND"
C_COMMAND = "C_COMMAND"
L_COMMAND = "L_COMMAND"

class Parser:
    def __init__(self, file):
        try:
            with open(file, "r") as f:
                self.code_lines = [
                    line.split('//')[0].strip()
                    for line in f
                    if line.strip() and not line.strip().startswith('//')
                ]
                
            self.current_index = -1
            self.current_command = None
        except  FileNotFoundError:
            print(f"File '{file}' not Found.")
            sys.exit()
            
    def hasMoreLines(self):
        return self.current_index + 1 < len(self.code_lines)
    
    def advance(self):
        self.current_index += 1
        self.current_command = self.code_lines[self.current_index]
        
    def instructionType(self):
        if self.current_command.startswith('@'):
            return A_COMMAND
        elif (self.current_command.startswith('(') and self.current_command.endswith(')')):
            return L_COMMAND
        else:
            return C_COMMAND
        
    def symbol(self):
        if self.instructionType() == A_COMMAND:
            return self.current_command[1:]
        elif self.instructionType() == L_COMMAND:
            return self.current_command[1:-1]
        else:
            return None
        
    def comp(self):
        if self.instructionType() != C_COMMAND:
            return None
        
        if '=' in self.current_command:
            comp_part = self.current_command.split('=')[1]
        else:
            comp_part = self.current_command
            
        if ';' in comp_part:
            comp_part = comp_part.split(';')[0]
            
        return comp_part
    
    def dest(self):
        if (self.instructionType() == C_COMMAND and '=' in self.current_command):
            return self.current_command.split('=')[0]
        else:
            return None
        
    def jump(self):
        if (self.instructionType() == C_COMMAND and ';' in self.current_command):
            return self.current_command.split(';')[-1]
        else:
            return None
        
    def reset(self):
        self.current_index = -1
        self.current_command = None

    def print_lines(self):
        while self.hasMoreLines():
            self.advance()
            print(f"Command: {self.current_command}")
            print(f"Type: {self.instructionType()}")
            print(f"Symbol: {self.symbol()}")
            print(f"Dest: {self.dest()}")
            print(f"Comp: {self.comp()}")
            print(f"Jump: {self.jump()}")
            print("-" * 30)

if __name__ == "__main__":
    if len(sys.argv) >= 2:
        file = sys.argv[1]
    else:
        print("Usage: Parser.py filename.")
        sys.exit()
        
    parser = Parser(file)
    parser.print_lines()