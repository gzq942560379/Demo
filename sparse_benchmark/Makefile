
include make.inc

BIN_DIR = bin
LIB_DIR = lib
INC_DIR = include
SRC_DIR = src
EXAMPLE_DIR = example

LIB_NAME = sparsebench
LIBA = $(LIB_DIR)/lib$(LIB_NAME).a

.PHONY: all lib example

all : lib example

LIB_SRC_C = $(shell find $(SRC_DIR) -name *.c)
LIB_SRC_CPP = $(shell find $(SRC_DIR) -name *.cpp)

EXAMPLE_SRC_C = $(shell find $(EXAMPLE_DIR) -name *.c)
EXAMPLE_SRC_CPP = $(shell find $(EXAMPLE_DIR) -name *.cpp)

LIB_OBJ_C = $(patsubst %.c, %.o, $(LIB_SRC_C))
LIB_OBJ_CPP = $(patsubst %.cpp, %.o, $(LIB_SRC_CPP))

EXAMPLE_OBJ_C = $(patsubst %.c, %.o, $(EXAMPLE_SRC_C))
EXAMPLE_OBJ_CPP = $(patsubst %.cpp, %.o, $(EXAMPLE_SRC_CPP))

OBJ_CPP = $(LIB_OBJ_CPP) $(EXAMPLE_OBJ_CPP)
OBJ_C = $(LIB_OBJ_C) $(EXAMPLE_OBJ_C)

LIB_OBJ = $(LIB_OBJ_C) $(LIB_OBJ_CPP)
EXAMPLE_OBJ = $(EXAMPLE_OBJ_C) $(EXAMPLE_OBJ_CPP)

OBJ_ALL = $(OBJ_CPP) $(OBJ_C)

EXAMPLE_BIN = $(patsubst %.o, $(BIN_DIR)/%, $(notdir $(EXAMPLE_OBJ)))

example : $(EXAMPLE_BIN) lib

lib : $(LIBA)

$(OBJ_CPP) : %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -I$(INC_DIR) -o $@ $<

$(OBJ_C) : %.o : %.c
	$(CC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

$(LIBA) : $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

$(EXAMPLE_BIN) : $(BIN_DIR)/% : $(EXAMPLE_DIR)/%.o $(LIBA)
	$(CXX) $(LINKFLAGS) -o $@ $< $(LIBA)

clean:
	rm -f $(EXAMPLE_BIN)
	rm -f $(OBJ_ALL)
	rm -f $(LIBA)
