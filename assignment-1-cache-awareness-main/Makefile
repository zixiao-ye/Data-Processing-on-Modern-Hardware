CLFAGS = -Iinclude -O2 -Werror -Wpedantic
SRC := src

SRCF := $(addprefix $(SRC)/, CuTest.c prng.c arrper.c cache_access.c )
SRCN := $(SRCF) $(addprefix $(SRC)/, main.c)
SRCT := $(SRCF) $(addprefix $(SRC)/, tester.c)

all: normal tester

normal: $(SRCN)
	$(CC) $(CLFAGS) -o assignment $(SRCN) -lm

tester: $(SRCT)
	$(CC) $(CLFAGS) -g -o tester $(SRCT) -lm

debug: $(SRCN)
	$(CC) $(CLFAGS) -g -o assignment $(SRCN) -lm

.PHONY: clean
clean:
	rm assignment tester