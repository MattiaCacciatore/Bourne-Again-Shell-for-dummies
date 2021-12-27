/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_functions.h
 *
 *  Copyright (c) 2020-2021, Mattia Cacciatore <cacciatore1995@hotmail.it>
 *  Copyright (c) 2020-2021, Gabriele Nicora <marscina2015@gmail.com>
 *  Copyright (c) 2020-2021, Giovanni Bosi <vamp2058@gmail.com>
 *  All rights reserved.
 *
 *  ubash is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with ubash.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  A special thanks to Gabriele Nicora, a very dear friend. Thanks for your tips and your support.
 */
#include <stdio.h>
#include <stdlib.h>            /* calloc. */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>            /* strtok_r, strchr, NULL. */
#include <unistd.h>            /* getcwd. */
#include <limits.h>            /* e.g. LONG_MAX, _POSIX_ARG_MAX.*/
#include <readline/history.h>  /* Allowed by Prof. Lagorio Giovanni. */
/*------------------------------------------------------------------------------------------------------------------*/
// GLOBAL VARIABLES & STRUCTURES
/*------------------------------------------------------------------------------------------------------------------*/
#ifndef EOT
	#define EOT 0x04
#endif
/*---------------------------------------------------CODES----------------------------------------------------------*/
#define ERROR -1
#define OK     0
#define EXEC   1
#define CMD    2
#define CD     3
/*---------------------------------------------------COLORS---------------------------------------------------------*/
#define BOLD_BLUE     "\x1b[1m\x1b[34m"
#define BOLD_GREEN    "\x1b[1m\x1b[32m"
#define OFF           "\x1b[0m"
/*--------------------------------------------------CONSTANTS-------------------------------------------------------*/
static const int DEF_INP_BUF = 1024;     /* Default input buffer: 1 kB. */
static const int MIN_BUF = 1024;         /* Min size of a buffer: 1 kB. */
static const int MAX_ARG_CMD = 256;      /* Max number of arguments per command. */
static const int MAX_CMD = 256;          /* Max number of commands per input. */

struct commandinfo{
	char*** cmd_list;                /* List of command(s). */
	int n_cmd;                       /* Number of command(s). */
	int* fdpipe_list;                /* List of pipe(s). */
};
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
/* It prints prompt on stdout. Return: OK if it succeeds, ERROR if an error has occurred. */
int printprompt();
/* It reads input from stdin and stores it into a heap-buffer string. It returns a pointer to the string
*  allocated with malloc, NULL if it fails. */
char* getinput();
/* "parser line" - It parses the string, spliting it by "|" then by " \t" into a list of list of command(s).
*   It returns a pointer to the list, NULL if it fails. */
char*** parsline(char*);
/* "count arguments" - It counts and returns the number of argument(s) in the 1st argument (list of command(s)). */
int cntarg(const char** const);
/* "count commands" - It counts and returns the number of command(s) in the 1st argument. */
int cntcmd(const char*** const);
/* "expand macros" - It parses the string looking for macros like "$<something>", then it expands them.
*  It returns ERROR if something fails, OK otherwise. 1st argument: string. */
int expmacros(char**);
/* "valid input/output" - It parses the command (1st argument), looking for environment variables
*   and I/O redirections. It expands macros and stores into 2nd and 3rd argument the pointers to
*   file input (2nd - redirection of input from file) and file output (3rd argument - redirection of
*   output on file). It returns OK if it succeeds, ERROR otherwise. */
int valio(char**, char**, char**, const int, const int);
/* It prepares pipe(s) for commands, storing the fd(s) into int* array.
*  It returns a pointer to this array. 1st argument: number of command(s). */
int* createpipe(int*);
/* "input/output command" - It executes I/O redirection on the pipe(s).
*  1st argument: file descriptor list.
*  2nd argument: input file (NULL if there's no input redirection).
*  3rd argument: output file (NULL if there's no output redirection).
*  4th argument: number of command in the list of command(s).
*  5th argument: number of command(s).
*  It returns OK if it succeeds, ERROR otherwise. */
int iocmd(const int* const, const char** const, const char** const, const int, const int);
/* "execute command" - It executes the command stored in the 1st argument. Command must be legit. */
void execcmd(char**);
/* It waits all child processes, terminate them if one of them is terminated by a signal. 
*  1st argument: number of child processes to be waited. */
void waitall(int);
/* It closes all file descriptors opened early. */
void closeallfd(const int, const int* const);
/* "resetinfo" - It frees commandinfo and the two strings for the pathfile, setting everything to NULL or zero. */
void rstinfo(struct commandinfo*, char**, char**);
/* "resetline" - It frees string allocated with malloc/realloc. It returns the pointer (null-ed). */
char* rstline(char*);
/* "check pointer" - It checks if a pointer is NULL. Return ERROR if it's NULL, OK otherwise. */
int checkp(void*);
