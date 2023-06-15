CLFAGS = -Iinclude -O2

all: vector tester
SRCV := $(addprefix src/, main.c vectorization.c prng.c helpers.c)
SRCT := $(addprefix src/, tester.c vectorization.c prng.c helpers.c CuTest.c)

vector: $(SRCV)
	$(CC) $(CLFAGS) $(SRCV) -o vector -lm

tester: $(SRCT)
	$(CC) $(CLFAGS) $(SRCT) -o tester -lm

debug: $(SRCV)
	$(CC) $(CLFAGS) -g $(SRCV) -o vector -lm

.PHONY: clean
clean:
	rm vector tester