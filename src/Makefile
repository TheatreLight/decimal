CC=gcc
CFLAGS=
LIBS=

OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit 
	OPEN=xdg-open
else
    LIBS=-lcheck -lm
	OPEN=open
endif

all: s21_decimal.a test gcov_report

s21_decimal.a:
	$(CC) $(CFLAGS) -c s21_decimal.c
	ar rcs libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a

test:
	$(CC) $(CFLAGS) s21_test.c s21_decimal.c -o s21_test.out $(LIBS)
	make run

gcov_report:
	$(CC) $(CFLAGS) -o s21_test.out --coverage s21_decimal.c s21_test.c $(LIBS)
	make run
	lcov -t "s21_test" -o test.info -c -d .
	genhtml -o report test.info

clean:
	rm -rf *.o *.a *.out *.gcda *.gcno *.info *.dSYM *.info report

miniverter:
	sudo chmod +x ../materials/build/./run.sh
	sudo ../materials/build/./run.sh

run:
	./s21_test.out

check: 
	cp ../materials/linters/CPPLINT.cfg ./CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --quiet  --extension=c *.c *.h
	cppcheck --quiet --enable=all --suppress=missingIncludeSystem *.c *.h

leaks: clean test
	CK_FORK=no leaks --atExit --  ./s21_test.out

valgrind:
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./s21_test.out
