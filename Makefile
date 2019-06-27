CFLAGS = -fPIC -Wall -Wextra
LDFLAGS = -shared
LDADD = -ldl

LIB = libhsm.so
OBJ = hsm.o

.PHONY: all clean

all: $(LIB)

clean:
        $(RM) $(LIB) $(OBJ)

$(LIB): $(OBJ)
        $(LD) -o $@ $(LDFLAGS) $(LDADD) $^