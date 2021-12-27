/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_main_enhanced.c
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
#include "ubash_changedir.h"
#include "ubash_history.h"
#include "ubash_error.h"
/*----------------------------------------------------------------------------------------------------------------*/
// MAIN
/*----------------------------------------------------------------------------------------------------------------*/
int main(){
/*-------------------------------------------INITIALIZATION-------------------------------------------------------*/
	/* Struct that stores commands info. */
	struct commandinfo mycmdlist;
	mycmdlist.cmd_list = NULL;
	mycmdlist.fdpipe_list = NULL;
	int exitstatus = OK, ncmds = 0;
	char* input = NULL,* file_in = NULL,* file_out = NULL;
	pid_t proc_pid;

	using_history();

	for(;;){
/*--------------------------------------------PRINT PROMPT--------------------------------------------------------*/
		if(printprompt() == ERROR){
			exitstatus = ERROR;
			break;
		}
/*---------------------------------------------READ INPUT---------------------------------------------------------*/
		input = getinput();
		if(checkp(input) == ERROR){
			input = rstline(input);
			break;
		}
		add_history(input);
/*--------------------------------------------PARSING INPUT-------------------------------------------------------*/
		mycmdlist.cmd_list = parsline(input);
		input = rstline(input);
		mycmdlist.n_cmd = cntcmd((const char*** const)mycmdlist.cmd_list);
/*--------------------------------------------CD CHEKING----------------------------------------------------------*/
		if(checkcd(mycmdlist.cmd_list, mycmdlist.n_cmd) != OK){
			rstinfo(&mycmdlist, &file_in, &file_out);
			continue;
		}
/*-----------------------------------------PREPARING PIPE(S)------------------------------------------------------*/
		mycmdlist.fdpipe_list = createpipe(&mycmdlist.n_cmd);
/*----------------------------------------FORK, CHECK AND EXEC----------------------------------------------------*/
		/* It creates as many processes as the number of command(s). */
		for(int n_cmd = 0; n_cmd < mycmdlist.n_cmd; ++n_cmd){
			proc_pid = fork();
			if(proc_pid < 0){
				prntfail(SYS, "fork", "main"); /* Defined in ubash_error.h */
				rstinfo(&mycmdlist, &file_in, &file_out);
				clear_history();
                		return EXIT_FAILURE;
  			}
			else if(proc_pid == 0){ /* Child process(es). */
				if(valio(mycmdlist.cmd_list[n_cmd], &file_in, &file_out, n_cmd, mycmdlist.n_cmd) == OK
 			           && iocmd(mycmdlist.fdpipe_list, (const char** const)&file_in,
				      (const char** const)&file_out, n_cmd, mycmdlist.n_cmd) == OK)
						execcmd(mycmdlist.cmd_list[n_cmd]);
				/* Here only if exec or I/O redirection fails. */
				rstinfo(&mycmdlist, &file_in, &file_out);
				return EXIT_FAILURE;
  			}
		}
/*-------------------------------------------RESET AND FREES------------------------------------------------------*/
		closeallfd(mycmdlist.n_cmd - 1, mycmdlist.fdpipe_list); /* n_cmd - 1 = n_pipe */
		ncmds = mycmdlist.n_cmd;
		rstinfo(&mycmdlist, &file_in, &file_out);
/*-----------------------------------------------WAIT(S)----------------------------------------------------------*/
		waitall(ncmds);
	}
	clear_history();
	if(exitstatus < 0)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}