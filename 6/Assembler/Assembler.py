import sys
from Parser import Parser
from Code import Code

def toBinary(n, bits=16):
    return format(n, '0{}b'.format(bits))

if len(sys.argv) >= 2:
    asm_file = sys.argv[1]
else:
    print("Usage: Parser.py filename.")
    sys.exit()

hack = []
hack_file = asm_file.replace('.asm', '.hack')

parser = Parser(asm_file)

while parser.hasMoreLines():
    parser.advance()
    cmd_type = parser.instructionType()
    
    if cmd_type == "A_COMMAND":
        symbol = parser.symbol()        
        if symbol.isdigit():
            addr = int(symbol)
            binary_code = '0' + toBinary(addr, 15)
        else:
            print(f"Error: Symbol '{symbol}' not supported yet.")
            sys.exit()        
    elif cmd_type == "L_COMMAND":
        # 아직 기호테이블은 해석 못함
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
        