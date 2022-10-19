BIN   = ./puca
SRC   = src/puca.c

CC     = cc
COPTS  = -g -O3
LIBS   = -lpthread -lm

install : all

all : $(BIN)

clean:
	rm -f $(BIN)

$(BIN): 
	$(CC) $(SRC) $(COPTS) $(LIBS) -o $(BIN) 

