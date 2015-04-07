CC		 = gcc
SRCHOME  = src
BIN      = bin
EXEC	 = solver
SOURCES  = $(SRCHOME)/solver.c $(SRCHOME)/stack.c
OBJECTS	 = $(SOURCES:.c=.o)
INCLUDES = includes
CFLAGS   = -Wall -ansi -I $(INCLUDES)/

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN)/$@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(SRCHOME)/*.o
	rm $(BIN)/solver

