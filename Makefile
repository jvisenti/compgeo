##debug 
#CFLAGS = -g
##release
CFLAGS = -O3 -DNDEBUG

CFLAGS+= -Wall

CC=gcc $(CFLAGS)

SOURCES=main.c

SRC_PATH=src
OBJ_PATH=obj

OBJ_FILES=$(patsubst %.c,$(OBJ_PATH)/%.o,$(SOURCES))

PROGS=MotionPlanner

.PHONY: all clean
all: $(PROGS)

$(PROGS): $(OBJ_FILES)
	$(CC) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $< -o $@

clean:	
	$(RM) $(OBJ_PATH)/*.o $(PROGS)


