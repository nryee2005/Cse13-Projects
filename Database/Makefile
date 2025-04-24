CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: database customers_lib_test

database: database.o customers_lib.o
	$(CC) database.o customers_lib.o -lm -o database

customers_lib_test: customers_lib_test.o customers_lib.o
	$(CC) customers_lib_test.o customers_lib.o -lm -o customers_lib_test

customers_lib_test.o: customers_lib_test.c customers_lib.h
	$(CC) $(CFLAGS) -c customers_lib_test.c 

customers_lib.o: customers_lib.c customers_lib.h
	$(CC) $(CFLAGS) -c customers_lib.c 

database.o: database.c customers_lib.h
	$(CC) $(CFLAGS) -c database.c 


clean: 
	rm -f database.o customers_lib.o customers_lib_test.o
	rm -f database customers_lib_test

format:
	clang-format -i -style=file *.[ch]




