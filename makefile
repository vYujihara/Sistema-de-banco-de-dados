CC = gcc
CFLAGS = -Wall  -std=c99  -g

objs = escalona.o equivalente.o libEscalona.o seriabilidade.o

all: escalona

escalona: $(objs)
	$(CC) $(CFLAGS) $(objs) -o escalona
escalona.o: escalona.c libEscalona.h equivalente.h seriabilidade.h
	$(CC) $(CFLAGS) -c escalona.c
equivalente.o: equivalente.c equivalente.h libEscalona.h seriabilidade.h
	$(CC) $(CFLAGS) -c equivalente.c
seriabilidade.o: seriabilidade.c seriabilidade.h equivalente.h libEscalona.h
	$(CC) $(CFLAGS) -c seriabilidade.c
libEscalona.o: libEscalona.c seriabilidade.h equivalente.h libEscalona.h
	$(CC) $(CFLAGS) -c libEscalona.c
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f escalona
