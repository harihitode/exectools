all: icount

icount: icount.c
	${CC} icount.c -o icount

.PHONY: clean

clean:
	rm -rf icount
