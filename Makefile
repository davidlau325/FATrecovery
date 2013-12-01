CC=gcc
CFLAGS=
DEPS=basic.h recovery.h entry.h
OBJ=main.o basic.o recovery.o

%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run:$(OBJ)
	$(CC) -o recover $^ $(CFLAGS)

.PHONY:clean
clean:
	@rm *.o || (echo "No build file found")
	@rm recover || (echo "No executable found")
	@echo "Build file clean!"
