/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  uchangedir.c
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
#include "ubash_functions.h"
#include "ubash_changedir.h"
#include "ubash_error.h"
/*------------------------------------------------------------------------------------------------------------------*/
int mycd(char* pathname){
	if(checkp(pathname) == ERROR) return ERROR;

	if(strcmp("cd", pathname) == 0){ /* input: "cd". */
		pathname = getenv("HOME");
		if(checkp(pathname) == ERROR){
			prntfail(FUN_CD, "HOME", "mycd");
			return ERROR;
		}
	}
	
	if(chdir(pathname) < 0) prntfail(SYS, "chdir", "mycd");
	return CD;
}
/*------------------------------------------------------------------------------------------------------------------*/
int checkcd(char*** cmdlist, const int ncmd){
	for(int i = 0; i < ncmd; ++i){
		/* Found cd command. It can be only the first argument. */
		if(strcmp("cd", cmdlist[i][0]) == 0){
			if(ncmd > 1){
				prntfail(FUN_CHECK_ONE, NULL, "checkcd");
				return ERROR;
			}
			
			int narg = cntarg((const char** const)cmdlist[i]);
			if(narg > 2){
				prntfail(FUN_CHECK_TWO, NULL, "checkcd");
				return ERROR;
			}
			else{ /* "cd" with 0 or 1 argument. */
				if(narg == 1) return mycd(cmdlist[0][0]); /* Legit cd. */
				else if(*cmdlist[0][1] == '<' || *cmdlist[0][1] == '>'){
					/* 2nd argument: I/O redirection --> INVALID. */
					prntfail(FUN_CHECK_THR, NULL, "checkcd");
					return ERROR;
				}
				else{ /* Legit cd. */
					if(expmacros(&cmdlist[0][1]) == ERROR) return ERROR;
					else return mycd(cmdlist[0][1]);
				}
			}

		}
	}
	return OK;
}