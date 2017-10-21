CFLAGS = -O2 -Wall -Wextra -Isrc/ -rdynamic
DCFLAGS =
LDFLAGS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local/

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/test_*.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))


TARGET=build/libmylib.a

# The Target Build
all: $(TARGET) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
tests: LDLIBS +=-L./build -lmylib
tests: $(TESTS)
	python ./tests/runttests.py

valgrind:
	VALGRIND="valgrind --log-file=valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
