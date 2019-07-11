CFLAGS = -fPIC -Wall -Wextra
LDFLAGS = -shared
LDADD = -ldl

LIB = libhsm.so
OBJ = hsm.o
TEST = test.c
TESTO = test

.PHONY: all clean

all: $(LIB)

clean:
	$(RM) $(LIB) $(OBJ) $(TESTO)
	
test:
	cc $(TEST) -o $(TESTO)

$(LIB): $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $(LDADD) $^
