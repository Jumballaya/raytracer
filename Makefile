######
#
#
#
######
RM := rm

PATH_D = bin
PATH_I = include
PATH_S = src

OBJS = $(wildcard main.cpp src/*.cpp)
CC = g++
C_FLAGS = -w
L_FLAGS =
INC = -I./$(PATH_I)

BINARY = raytrace

all: $(OBJS)
	mkdir -p $(PATH_D)
	$(CC) $^ $(C_FLAGS) $(L_FLAGS) -o $(PATH_D)/$(BINARY) $(INC)

clean:
	rm -rf $(PATH_D)
