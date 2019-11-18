all:
	gcc  protocolo.c  application.c  files.c  -lrt  -lm  -Wall  -g  -o  writer
	gcc  protocolo.c  application.c  files.c  -lrt  -lm  -Wall  -g  -o  reader/reader

clean:
	rm writer
	rm reader

valgrind: 
	valgrind --leak-check=yes -v ./writer
	valgrind --leak-check=yes -v ./reader/reader

rerun:
	make clean all valgrind
