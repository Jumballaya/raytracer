######
#
#
#
######
RM := rm

PATH_D = bin
PATH_I = include
PATH_S = src

MAIN_SRC = main.cpp
API_SRC = api.cpp
OBJS = $(wildcard src/*.cpp)
CC = g++
C_FLAGS = -w
L_FLAGS =
INC = -I./$(PATH_I)

BINARY = raytrace

build: $(MAIN_SRC) $(OBJS)
	mkdir -p $(PATH_D)
	$(CC) $^ $(C_FLAGS) $(L_FLAGS) -o $(PATH_D)/$(BINARY) $(INC)

api: $(API_SRC) $(OBJS)
	mkdir -p $(PATH_D)
	$(CC) $^ $(C_FLAGS) $(L_FLAGS) -o $(PATH_D)/api $(INC)

clean:
	rm -rf $(PATH_D)

.PHONY: build api clean
