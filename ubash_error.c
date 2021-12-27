/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_error.c
 *
 *  Copyright (c) 2020-2021, Mattia Cacciatore <cacciatore1995@hotmail.it>
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
 */
#include "ubash_error.h"
#include "ubash_functions.h"
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
void prntfail(int type_err, char* err, char* fun){ /* err --> error; fun --> function; */
	switch(type_err){
		case FUN:
			fprintf(stderr, "Error %s failed in %s\n", err, fun);
			break;
		case SYS:
			fprintf(stderr, "Error %s failed in %s\n", err, fun);
			perror(NULL);
			break;
		case SIG:
			fprintf(stderr, "Process terminated by signal: %s\n", err);
			break;
		case FUN_VAL_IN:
			fprintf(stderr, "Command(s) with only blank and/or TAB spaces. Invalid command(s).\n");
			break;
		case FUN_PARS_ONE:
			fprintf(stderr, "Error in %s.", fun);
			fprintf(stderr, "Too many arguments for command. Limit is: %i.\n", MAX_ARG_CMD);
			break;
		case FUN_PARS_TWO:
			fprintf(stderr, "Error in %s.", fun);
			fprintf(stderr, "Too many commands. Limit is: %i.\n", MAX_CMD);
			break;
		case FUN_EXP:
			fprintf(stderr, "Error in %s. Invalid environment variable after $.\n", fun);
			break;
		case FUN_VAL_IO:
			fprintf(stderr, "Error in %s. Invalid I/O redirection.\n", fun);
			break;
		case FUN_EXEC:
			fprintf(stderr, "Error in %s. Command is too long. Length limit is: %i.\n", fun,
				_POSIX_ARG_MAX);
			break;
		case FUN_CD:
			fprintf(stderr, "Error in %s. %s doesn't exist.\n", fun, err);
			break;
		case FUN_CHECK_ONE:
			fprintf(stderr, "Error in %s. cd in pipe with other command(s).\n", fun);
			break;
		case FUN_CHECK_TWO:
			fprintf(stderr, "Error in %s. Incorrect number of argument(s) for cd.\n", fun);
			break;
		case FUN_CHECK_THR:
			fprintf(stderr, "Error in %s. I/O redirection after cd.\n", fun);
			break;
		case FUN_HIST:
			fprintf(stderr, "Error in %s. History list unavailable.\n", fun);
			break;
		default:
			break;
	}
}