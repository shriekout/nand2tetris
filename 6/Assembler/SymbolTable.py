class SymbolTable:
    def __init__(self):
        self.symbol_table = {
            "SP": 0,
            "LCL": 1,
            "ARG": 2,
            "THIS": 3,
            "THAT": 4,
            "SCREEN": 16384,
            "KBD": 24576,
        }
        
        for i in range(16):
            self.symbol_table[f"R{i}"] = i

    def addEntry(self, symbol, address):
        self.symbol_table[symbol] = address

    def contains(self, symbol):
        return symbol in self.symbol_table

    def getAddress(self, symbol):
        return self.symbol_table[symbol]
    
    def __str__(self):
        result = ""
        for symbol, address in self.symbol_table.items():
            result += f"{symbol}: {address}\n"
            
        return result
    
if __name__ == "__main__":
    s = SymbolTable()
    print(s)