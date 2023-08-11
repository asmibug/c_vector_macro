# CPPFLAGS to generate Makefiles *.d for each compiled source file
CPPFLAGS := -MMD -MP
CFLAGS := -std=c17 -Wpedantic -g3 \
-Wall -Wextra -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 \
-Wundef -Wshadow -Wdouble-promotion -Wconversion -Wno-sign-conversion \
-Wfloat-equal -Wswitch-default \
-fsanitize=address,undefined

default: test

test: test.exe
	./test.exe
	@echo "ALL TESTS PASSED"

test.exe: test.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	-rm *.o *.d *.exe

.PHONY: test clean
.DELETE_ON_ERROR:

-include $(wildcard *.d)
