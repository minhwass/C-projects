program: main.o Producer.o Consumer.o
	gcc -o program main.c Producer.c Consumer.c
main.o: main.c header.h Producer.c Consumer.c
	gcc -c main.c Producer.c Consumer.c
Producer.o: header.h Producer.c
	gcc -c Producer.c header.h
Consumer.o: header.h Consumer.c
	gcc -c Consumer.c header.h
