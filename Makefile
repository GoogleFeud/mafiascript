calc:
	bison -d ./src/Parser/parser.y -o src/Parser/parser.tab.c
	flex  -o src/Parser/parser.lex.c ./src/Parser/parser.l
	g++  -std=c++17 src/Parser/parser.lex.c src/Parser/parser.tab.c src/main.cpp -o dist

clean:
	rm -rf calc
