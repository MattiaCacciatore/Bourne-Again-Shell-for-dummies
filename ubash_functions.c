/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_functions.c
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
#include "ubash_functions.h"
#include "ubash_history.h"
#include "ubash_error.h"
/*------------------------------------------------------------------------------------------------------------------*/
// HELPER FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
/* "checkexit" - Check if the string is "exit". It returns a pointer to the input string,
*  NULL if there's a escape sequence. 2nd argument: ctrl-d from keyboard. */
char* checkex(char* input, const int ctrlD){ /* User wants to exit from shell. */
	if(checkp(input) == OK && (strcmp(input, "exit") == 0 || ctrlD == OK)) input = rstline(input);
	return input;
}
/*------------------------------------------------------------------------------------------------------------------*/
/* "valid input" - It parses the 1st argument returning ERROR if the command is empty, OK otherwise. */
int valinput(const char* const arg){
	unsigned int i = 0;
	for(; i < strlen(arg); ++i)
		if(arg[i] != ' ' && arg[i] != '\t') break;

	if(i == strlen(arg)){ /* Command with only blank and/or tab space(s). */
		prntfail(FUN_VAL_IN, NULL, NULL);
		return ERROR;
	}
	return OK; /* Else. */
}
/*------------------------------------------------------------------------------------------------------------------*/
/* "resetlist" - It frees the list of string(s) allocated with malloc/realloc. It returns the pointer (null-ed).*/
char** rstlist(char** cmd){
	if(checkp(cmd) == ERROR) return NULL;

	for(char** p = cmd; *p != NULL; ++p) *p = rstline(*p);

	free(cmd);
	return cmd = NULL;
}
/*------------------------------------------------------------------------------------------------------------------*/
/* "resetall" - It frees the list of lists of string(s) allocated with malloc/realloc. It returns the pointer (null-ed).*/
char*** rstall(char*** cmdlist){
	if(checkp(cmdlist) == ERROR)
		return NULL;

	for(char*** p = cmdlist; *p != NULL; ++p)
		*p = rstlist(*p);
	free(cmdlist);
	return cmdlist = NULL;
}
/*------------------------------------------------------------------------------------------------------------------*/
/* "checkstatus" - It checks the child's exitstatus. It returns ERROR if a child has been terminated
*  by a signal or something else, OK otherwise. */
int ckstatus(const int status){
	if(WIFEXITED(status))
		return OK;
	else if(WIFSIGNALED(status)){
		int signo = WTERMSIG(status);
		prntfail(SIG, strsignal(signo) , NULL);
	}
	return ERROR;
}
/*------------------------------------------------------------------------------------------------------------------*/
/* "checkhistory" - It checks if the user wants to see the command history. It prints the history and returns OK if
*  the 1st argument matches "history", ERROR otherwise. */
int check_hstry(const char* cmd){
	if(strcmp(cmd, "history") == 0){ /* Valid command, request for command history on stdout. */
		prnt_hstry();
		return OK;
	}
	return ERROR;
}
/*------------------------------------------------------------------------------------------------------------------*/
char** prepare_cmd(char** cmd){
	int n_arg = cntarg((const char** const)cmd);
	int n_io_arg = 0; /* Number of arguments with I/O redirection. */
	for(char** tmp = cmd; *tmp != NULL; ++tmp){
		if(*tmp[0] == '<' || *tmp[0] == '>'){
			*tmp = rstline(*tmp); /* Deleting I/O redirection argument(s). */
			++n_io_arg;           /* Counting them. */
		}
	}
	char** tmp = (char**) calloc((n_arg - n_io_arg), sizeof(char*));
	if(checkp(tmp) == ERROR){
		prntfail(FUN, "calloc", "execcmd");
		return NULL;
	}

	for(int i = 0, j = 0; i < (n_arg - n_io_arg); ++j){ /* Preparing command to be executed. */
		if(cmd[j] != NULL){
			tmp[i] = cmd[j];
			cmd[j] = NULL;
			++i;
		}
	}

	for(int i = 0; i < (n_arg - n_io_arg); ++i) cmd[i] = tmp[i];

	free(tmp);
	return cmd;
}
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
int printprompt(){
	/* The uniform system limit for the length of an entire file name including the terminating null character 
	*  (PATH_MAX). Defined in "limits.h". */
	char* pathname = NULL;
	pathname = getcwd(pathname, PATH_MAX); /* Syscall. */
	if(checkp(pathname) == ERROR){
		prntfail(SYS, "getcwd", "printprompt");
		return ERROR;
	}
	else{
		char* user = NULL;
		user = getenv("USER");
		if(checkp(user) == OK){
            		fprintf(stdout, BOLD_GREEN);
			fprintf(stdout, "%s", user);
		}
        	fprintf(stdout, BOLD_BLUE);
        	fprintf(stdout, "%s", pathname);
        	fprintf(stdout, OFF);
        	fprintf(stdout, "$ ");
		pathname = rstline(pathname); /* Very important in order to avoid memory leak. */
		return OK;
	}
}
/*------------------------------------------------------------------------------------------------------------------*/
char* getinput(){
	int cap = DEF_INP_BUF, ctrlD = ERROR;                     /* Capacity and control-D. */
	char* inpbuf = (char*) calloc(DEF_INP_BUF, sizeof(char)); /* Input buffer. */
	if(checkp(inpbuf) == ERROR){
		prntfail(FUN, "calloc", "getinput");
		return inpbuf = NULL;
	}

	for(int i = 0;; ++i){
		if(i >= cap){ /* Limit buffer reached, trying to expand it. */
			cap *= 2;
			/* If realloc fails, it returns a NULL pointer leaving unchanged the pointer in input
			*  and the heap allocated with malloc previously. */
			char* tmp = (char*)realloc(inpbuf, cap * sizeof(char));
			if(checkp(tmp) == ERROR){
				prntfail(FUN, "realloc", "getinput");
				inpbuf = rstline(inpbuf);
				return inpbuf;
			}
			inpbuf = tmp;
			tmp = NULL;
		}
		/* fgetc because fgets considers as end of the string only '\n' or EOF that can cause some issues 
		*  with some standard library function that use '\0' aka string-terminator as parameter. */
		inpbuf[i] = fgetc(stdin);
		if(inpbuf[i] == '\n'){
			inpbuf[i] = '\0';
			break;
		}
		else if(inpbuf[i] == '\0') break; /* Commands inserted. */
		else if(inpbuf[i] == EOF || inpbuf[i] == EOT){
			/* In the C Standard Library, the value (macro) EOF indicate that an end-of-file 
			*  condition has occurred. In ASCII and Unicode, the character EOT is encoded at 
			*  U+0004 <control-0004>, it can be referred to as Ctrl-D. */
			if(i == 0){ /* Ctrl-D from keyboard as first character. */
				ctrlD = OK;
				break;
			}
			else
				--i;
		}
	}
	inpbuf = checkex(inpbuf, ctrlD);
	return inpbuf;
}
/*------------------------------------------------------------------------------------------------------------------*/
char*** parsline(char* input){
	char*** cmdlist = NULL;
	if(checkp(input) == ERROR) return cmdlist;

	cmdlist = (char***) calloc(MAX_CMD, sizeof(char**));
	if(checkp(cmdlist) == ERROR){
		prntfail(FUN, "calloc", "parsline");
		cmdlist = NULL;
	}
	else{
		char* tmp = input,* cmd = NULL,* str_s = NULL,* arg = NULL,* str_s_2 = NULL;
		int narg = 0, ncmd = 0;

		for(; ncmd < MAX_CMD; ++ncmd){
			cmd = strtok_r(tmp, "|", &str_s);
			if(checkp(cmd) == ERROR) break; /* Command parsing ends. */

			if(valinput(cmd) == ERROR){ /* Checking invalid command(s). */
				cmdlist = rstall(cmdlist);
				break;
			}
//-------------------------------ARGUMENT(S) PARSING-----------------------------------------------------------------
			char** arglist = (char**) calloc(MAX_ARG_CMD, sizeof(char*));
			if(checkp(arglist) == ERROR){
				prntfail(FUN, "calloc", "parsline");
				cmdlist = rstall(cmdlist);
				break;
			}

			for(narg = 0; narg < MAX_ARG_CMD; ++narg){
				/* Previously it splits command line by " " (space) then by "\t" (TAB). */
				arg = strtok_r(cmd, " \t", &str_s_2);
				if(checkp(arg) == ERROR) break; /* Argument parsing ends. */

				arglist[narg] = strdup(arg);
				if(checkp(arglist[narg]) == ERROR){
					prntfail(SYS, "strdup", "parsline");
					cmdlist = rstall(cmdlist);
					return cmdlist;
				}
				cmd = NULL;
			}

			if(narg == MAX_ARG_CMD){
				prntfail(FUN_PARS_ONE, NULL, "parsline");
				cmdlist = rstall(cmdlist);
				break;
			}
//------------------------------------------------------------------------------------------------------------------
			cmdlist[ncmd] = arglist;
			if(checkp(cmdlist[ncmd]) == ERROR){
				cmdlist = rstall(cmdlist);
				break;
			}
			tmp = NULL;
		}

		if(ncmd == MAX_CMD){
			prntfail(FUN_PARS_TWO, NULL, "parsline");
			cmdlist = rstall(cmdlist);
		}
	}
	return cmdlist;
}
/*------------------------------------------------------------------------------------------------------------------*/
int cntarg(const char** const cmd){
	int narg = 0;
	if(checkp(cmd) == OK)
		for(; cmd[narg] != NULL; ++narg){}
	return narg;
}
/*------------------------------------------------------------------------------------------------------------------*/
int cntcmd(const char*** const cmdlist){
	int ncmd = 0;
	if(checkp(cmdlist) == OK)
		for(; cmdlist[ncmd] != NULL; ++ncmd){}
	return ncmd;
}
/*------------------------------------------------------------------------------------------------------------------*/
int expmacros(char** arg){
	if(checkp(*arg) == ERROR) return ERROR;
	/* Variable expansion, $ must be at the start of argument. 
	*  strlen() > 1 because "echo $A" --> nothing, "echo $" --> "$". */
	if(strncmp(*arg, "$", 1) == 0 && strlen(*arg) > 1){
		char* var = *arg;
		var = getenv(++var); /* E.g. "$USER" --> "USER" --> "mammamia". */
		if(checkp(var) == ERROR){
			prntfail(FUN_EXP, NULL, "expmacros");
			return ERROR;
		}
		*arg = strdup(var); /* Replacing expanded variable. */
	}
	return OK;
}
/*------------------------------------------------------------------------------------------------------------------*/
int valio(char** command, char** file_in, char** file_out, const int cmd_n, const int cmd_q){
	/* cmd_n = number of command(s). cmd_q = quantity of command(s). */
	if(checkp(command) == ERROR || cmd_n < 0 || cmd_q < 1) return ERROR;

	char** arg = command;
	int n_arg = cntarg((const char** const)command);
	for(int i = 0; *arg != NULL; ++arg, ++i){ /* Looking for environment variables and I/O redirections. */
		if(expmacros(&command[i]) == ERROR) return ERROR;
		if(command[i][0] == '<' || command[i][0] == '>'){ /* Found I/O redirection. */
			/* I/O redirection in a middle command/argument, not valid.
			*  [cmd_n == 0 first command, cmd_n + 1 == cmd_q last command].
			*  < only allowed for the first command, > only allowed for the last command. */
			if((cmd_n != 0 && cmd_n + 1 != cmd_q) || (i != 0 && i + 1 != n_arg) ||
			    (command[i][0] == '<' && cmd_n != 0) || (command[i][0] == '>' && cmd_n + 1 != cmd_q))
				goto errorIO;

			for(int j = 1; command[i][j] != '\0' && command[i][j] != '\t'; ++j)
				if(command[i][j] == '<' || command[i][j] == '>') goto errorIO; /* E.g. ">foo>bar". */

			if(command[i][0] == '<'){ /* file_in(put) --> first command. */
				*file_in = command[i];
				++*file_in;
				*file_in = strdup(*file_in);
				if(checkp(*file_in) == ERROR) goto errorSTR;
			}
			else if(command[i][0] == '>'){ /* file_out(put) --> last command. */
				*file_out = command[i];
				++*file_out;
				*file_out = strdup(*file_out);
				if(checkp(*file_out) == ERROR) goto errorSTR;
			}
		}
	}
	return OK;
//------------------------------------NECESSARY---------------------------------------
errorIO:
	prntfail(FUN_VAL_IO, NULL, "valio");
	return ERROR;
errorSTR:
	prntfail(SYS, "strdup", "valio");
	return ERROR;
}
/*------------------------------------------------------------------------------------------------------------------*/
int* createpipe(int* n_cmd){
	int* fdpipe_list = NULL;
	if(*n_cmd > 1){
		fdpipe_list = (int*) calloc((*n_cmd - 1) * 2, sizeof(int));
		if(checkp(fdpipe_list) == ERROR){
			prntfail(FUN, "calloc", "createpipe");
			*n_cmd = 0;
			fdpipe_list = NULL;
		}
	}

	int pipefd[2];
	for(int i = 0; i < *n_cmd - 1; ++i){
		if(pipe(pipefd) < 0){
			prntfail(SYS, "pipe", "createpipe");
			*n_cmd = 0;
			break;
		}
		/* pipe --> [Read/stdin] ==== [Write/stdout] --> man pipe
		*                 0                  1 */
		fdpipe_list[(i * 2) + 1] = pipefd[0]; /* fds_list[0] = WRITE */
		fdpipe_list[i * 2] = pipefd[1];       /* fds_list[1] = READ */
	}
	return fdpipe_list;
}
/*------------------------------------------------------------------------------------------------------------------*/
int iocmd(const int* const pfd_list, const char** const fn_in, const char** const fn_out, const int cmd_n, const int cmd_q){
	if(cmd_q < 2 && *fn_in == NULL && *fn_out == NULL) return OK; /*  If there's only 1 command and no I/O on file. */

	if(*fn_in != NULL && cmd_n == 0){ /* Input redirection from file. Legit. */
		/* Opening file *fn_in in "read only" mode. */
		const int fd_file_in = open(*fn_in, O_RDONLY);
		if(fd_file_in < 0) goto errorOPEN;
		if(dup2(fd_file_in, STDIN_FILENO) < 0) goto errorDUP;
		if(close(fd_file_in) < 0) goto errorCLOSE;
	}

	if(*fn_out != NULL && (cmd_n + 1) == cmd_q){ /* Output redirection on file. Legit. */
		/* Opening file *fn_out in "write only" mode. Setting 0740 permissions by default. */
		const int fd_file_out = open(*fn_out, O_WRONLY | O_CREAT, S_IRWXU + S_IRGRP + S_IWGRP);
		if(fd_file_out < 0) goto errorOPEN;
		if(dup2(fd_file_out, STDOUT_FILENO) < 0) goto errorDUP;
		if(close(fd_file_out) < 0) goto errorCLOSE;
	}

	if(cmd_n > 0 && *fn_in == NULL) /* Input (STDIN) redirection. */
		if(dup2(pfd_list[(cmd_n * 2) - 1], STDIN_FILENO) < 0) goto errorDUP;

	if((cmd_n + 1) < cmd_q && *fn_out == NULL) /* Output (STDOUT) redirection. */
		if(dup2(pfd_list[cmd_n * 2], STDOUT_FILENO) < 0) goto errorDUP;

	for(int i = 0; i < (cmd_q - 1) * 2; ++i) /* Closing all unnecessary fds. */
		if(close(pfd_list[i]) < 0) goto errorCLOSE;

	return OK;
//---------------------------------NECESSARY---------------------------------
errorDUP:
	prntfail(SYS, "dup2", "iocmd");
	return ERROR;
errorCLOSE:
	prntfail(SYS, "close", "iocmd");
	return ERROR;
errorOPEN:
	prntfail(SYS, "open", "iocmd");
	return ERROR;
}
/*------------------------------------------------------------------------------------------------------------------*/
void execcmd(char** cmd){
	if(checkp(cmd) == ERROR || check_hstry(*cmd) == OK) return;
	/* At this point every command is legit. Now it's time to remove any I/O redirection argument. */
	cmd = prepare_cmd(cmd);
	unsigned int dim = _POSIX_ARG_MAX;
	if(dim < 1024) dim = 1024;
	char cmdpath[dim];
	if(strlen(cmd[0]) > dim){
		prntfail(FUN_EXEC, NULL, "execcmd");
		return;
	}
	sprintf(cmdpath, "%s", cmd[0]);
	if(execvp(cmdpath, cmd) < 0) prntfail(SYS, "exec", "execcmd");
	/* CPU executes this command only if exec have failed. */
	return;
}
/*------------------------------------------------------------------------------------------------------------------*/
void waitall(int ncmd){
	int status = 0;
	for(int i = 0; i < ncmd; ++i){
		if(wait(&status) != -1){
			/* It checks child's exitstatus, terminating the others if one of them has been
			*  terminated by a signal. */
			if(ckstatus(status) == ERROR){
				signal(SIGTERM, SIG_IGN); /* The parent process ignores SIGTERM. */
				kill(-getpid(), SIGTERM);
				signal(SIGTERM, SIG_DFL);
				wait(0);
			}
		}
	}
}
/*------------------------------------------------------------------------------------------------------------------*/
void closeallfd(const int cmd_q, const int* const pfd_list){
	/* Freeing fds in pipe(s). */
	for(int i = 0; i < cmd_q * 2; ++i){
		if(close(pfd_list[i]) < 0){
			prntfail(SYS, "close", "closeallfd");
			break;
		}
	}
}
/*------------------------------------------------------------------------------------------------------------------*/
void rstinfo(struct commandinfo* mycmdlist, char** file_in, char** file_out){
	if(checkp(mycmdlist->cmd_list) == OK) mycmdlist->cmd_list = rstall(mycmdlist->cmd_list);
	if(checkp(mycmdlist->fdpipe_list) == OK){
		free(mycmdlist->fdpipe_list);
		mycmdlist->fdpipe_list = NULL;
	}
	mycmdlist->n_cmd = 0;

	if(checkp(file_in) == OK) *file_in = rstline(*file_in);
	if(checkp(file_out) == OK) *file_out = rstline(*file_out);
}
/*------------------------------------------------------------------------------------------------------------------*/
char* rstline(char* arg){
	if(checkp(arg) == OK) free(arg); /* It frees string allocated with malloc/realloc. */
	return arg = NULL;
}
/*------------------------------------------------------------------------------------------------------------------*/
int checkp(void* p){
	if(p == NULL) return ERROR;
	else return OK;
}
