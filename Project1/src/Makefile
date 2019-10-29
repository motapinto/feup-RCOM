all:
	gcc  protocolo.c  application.c  files.c  -lrt  -lm  -Wall  -g  -o  writer
	gcc  protocolo.c  application.c  files.c  -lrt  -lm  -Wall  -g  -o  reader

clean:
	rm writer

valgrind: 
	valgrind --leak-check=yes -v ./writer
	valgrind --leak-check=yes -v ./reader

rerun:
	make clean all valgrind
