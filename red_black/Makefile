.PHONY:all clean
CC=gcc
RELEASE=0
ifeq ($(RELEASE), 1)
CFLAGS= -Wall -fPIC
OPTIMIZE=-O2 -fno-strict-aliasing
else
CFLAGS= -g3 -Wall -fPIC
OPTIMIZE=
endif
CPPFLAGS += $(INCLUDE)
SRC =$(wildcard *.c)
OBJ =$(patsubst %.c, %.o, $(SRC))
TARGET = rb_tree_test
all:$(TARGET) 
$(TARGET):$(OBJ)
	$(CC) -o $@ $^ -lm
%.o:%.c
	$(CC) -o $@ $(MARCO) $(OPTIMIZE) $(MACRO) $(CFLAGS) $(CPPFLAGS) -fstack-protector-all -c $<
clean:
	rm -f *.o
	rm $(TARGET)

