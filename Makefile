all:
	gcc -std=gnu11 -ggdb -Og -Wall -Wextra -Werror -pedantic -fsanitize=address ubash_main_enhanced.c ubash_functions.c ubash_changedir.c ubash_history.c ubash_error.c -o ubash -lreadline
