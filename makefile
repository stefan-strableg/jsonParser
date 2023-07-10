CC      = g++
LD 		= g++
CFLAGS  = -Wall -g -std=c++20
LDFLAGS = 

IDIR = inc
INC = JsonInterface.hpp JsonArray.hpp JsonObject.hpp JsonValue.hpp string.hpp
INC_PATH = $(addprefix $(IDIR)/, $(INC))

ODIR = obj
OBJ = JsonInterface.o JsonArray.o JsonObject.o JsonValue.o main.o string.o
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