import sys
from parser import Parser
from node import SymbolTable

def main():
    if len(sys.argv)!=2:
        print("Uso: python main.py <arquivo.pesca>")
        sys.exit(1)

    code = open(sys.argv[1],encoding='utf8').read()
    tree = Parser(code).parse()
    st = SymbolTable()
    tree.evaluate(st)

if __name__=='__main__':
    main()
