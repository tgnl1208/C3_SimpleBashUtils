CC = gcc
FLAGS = -Wall -Wextra -Werror
CHECK = valgrind --tool=memcheck --leak-check=yes


s21_cat: clean 
	$(CC) $(FLAGS) s21_cat.c -o s21_cat


clean:
	rm -f s21_cat

clang-format: 
	clang-format --style=Google -i *.c

valgrind:
	$(CHECK) ./s21_cat $(FILE)
	$(CHECK) ./s21_cat -b $(FILE)
	$(CHECK) ./s21_cat -n $(FILE)
	$(CHECK) ./s21_cat -s $(FILE)
	$(CHECK) ./s21_cat -e $(FILE)
	$(CHECK) ./s21_cat -E $(FILE)
	$(CHECK) ./s21_cat -t $(FILE)
	$(CHECK) ./s21_cat -T $(FILE)
	$(CHECK) ./s21_cat -v $(FILE)
	$(CHECK) ./s21_cat -bn $(FILE)
	$(CHECK) ./s21_cat -nse $(FILE)
	$(CHECK) ./s21_cat --number-nonblank $(FILE)
	$(CHECK) ./s21_cat --number $(FILE)
	$(CHECK) ./s21_cat --squeeze-blank $(FILE)