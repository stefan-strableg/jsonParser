CC      = g++
LD 		= g++
CFLAGS  = -Wall -g
LDFLAGS = 

IDIR = inc
INC = jsonParser.hpp string.hpp
INC_PATH = $(addprefix $(IDIR)/, $(INC))

ODIR = obj
OBJ = main.o jsonParser.o string.o
OBJ_PATH = $(addprefix $(ODIR)/, $(OBJ))

BDIR = bin
BIN = main.exe
BIN_PATH = $(addprefix $(BDIR)/, $(BIN))

SDIR = src

main: $(OBJ_PATH) $(INC_PATH)
	$(LD) $(LDFLAGS) -o bin/main.exe $(OBJ_PATH) 

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	del /f /q bin\* obj\*