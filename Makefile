CFLAGS = -I. -g

mypas: mypas.o lexer.o parser.o keywords.o symtab.o errors.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o
	
mostlyclean: clean
	$(RM) mypas *~
