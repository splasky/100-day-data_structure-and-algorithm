CC=gcc
CFLAGS= -O3 -Werror -Wall -Wextra -Isrc/ -rdynamic
LDFLAGS= -ldl $(OPTLIBS)
PREFIX ?= /usr/local/

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/test_*.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))


TARGET=build/libmylib.a
DYNAMIC=build/libmylib.so

# The Target Build
all: $(TARGET) tests $(DYNAMIC)

dev: CFLAGS += -g
dev: CFLAGS := $(filter-out -O3,$(CFLAGS))
dev: $(TARGET) tests valgrind

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(DYNAMIC): build $(OBJECTS$)
	$(CC) -shared -fPIC -o $@ $(OBJECTS)

install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

build:
	@mkdir -p build

# The Unit Tests
.PHONY: tests clean valgrind
tests: LDLIBS +=-L./build -lmylib
tests: $(TESTS)
	python ./tests/runttests.py

valgrind:
	VALGRIND="valgrind --log-file=valgrind-%p.log"

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
