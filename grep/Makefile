FLAG=-std=c11 -Wall -Wextra -Werror
CHECK=valgrind --tool=memcheck --leak-check=yes
FILE = grep_tests/test1.txt
FILE2 = grep_tests/test2.txt


all: s21_grep

s21_grep: clean
	gcc $(FLAGS) s21_grep.c -o s21_grep

clean:
	rm -f s21_grep

clang-format:
	clang-format --style=Google -i *.c

valgrind: 
	$(CHECK) ./s21_grep Lorem $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -i lorem $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -i LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -e LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -v lorem $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -n LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -c LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -l LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -h LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -s LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -f grep_tests/patterns.txt $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -o LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -iv LOREM $(FILE) $(FILE2)
	$(CHECK) ./s21_grep -in LOREM $(FILE) $(FILE2)
