
tuner: tuner.o filter.o libfft.o note.o
	g++ -o tuner filter.o libfft.o note.o tuner.o -lm -lportaudio -lmraa -lupm-i2clcd

tuner.o: tuner.cc
	g++ -c tuner.cc

filter.o: filter.c
	g++ -c filter.c

libfft.o: libfft.c
	g++ -c libfft.c

note.o: note.c
	g++ -c note.c

install: tuner
	cp tuner /bin/

uninstall:
	rm /bin/tuner

.PHONY: clean
clean:
	rm -f *.o tuner

