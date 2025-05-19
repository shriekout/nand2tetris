import sys
from Parser import Parser
from Code import Code
from SymbolTable import SymbolTable

def toBinary(n, bits=16):
    return format(n, '0{}b'.format(bits))

if len(sys.argv) >= 2:
    asm_file = sys.argv[1]
else:
    print("Usage: Parser.py filename.")
    sys.exit()

hack = []
hack_file = asm_file.replace('.asm', '.hack')

symbols = SymbolTable()
parser = Parser(asm_file)

# 1 Pass
rom_address = 0

while parser.hasMoreLines():
    parser.advance()
    cmd_type = parser.instructionType()
    
    if cmd_type == "L_COMMAND" and not symbols.contains(parser.symbol()):
        symbols.addEntry(parser.symbol(), rom_address)
    elif cmd_type in ("A_COMMAND", "C_COMMAND"):
        rom_address += 1

# 2 Pass
parser.reset()
next_variable_address = 16

while parser.hasMoreLines():
    parser.advance()
    cmd_type = parser.instructionType()
    
    if cmd_type == "A_COMMAND":
        symbol = parser.symbol()        
        if symbol.isdigit():
            addr = int(symbol)
        else:
            if not symbols.contains(symbol):
                symbols.addEntry(symbol, next_variable_address)
                next_variable_address += 1
            addr = symbols.getAddress(symbol)
        binary_code = '0' + toBinary(addr, 15)
    elif cmd_type == "L_COMMAND":
        continue
    elif cmd_type == "C_COMMAND":
        comp_bits = Code.comp(parser.comp())
        dest_bits = Code.dest(parser.dest())
        jump_bits = Code.jump(parser.jump())
        binary_code = '111' + comp_bits + dest_bits + jump_bits
        
    hack.append(binary_code)

with open(hack_file, 'w') as f_out:
    for line in hack:
        print(line)
        f_out.write(line + '\n')
        
print(f"Assembly completed: {hack_file}")
