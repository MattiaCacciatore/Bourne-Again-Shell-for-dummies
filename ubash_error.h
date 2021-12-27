/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_error.h
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
enum{FUN, SYS, SIG, FUN_VAL_IN, FUN_PARS_ONE, FUN_PARS_TWO, FUN_EXP, FUN_VAL_IO, FUN_EXEC, FUN_CD, FUN_CHECK_ONE,
     FUN_CHECK_TWO, FUN_CHECK_THR, FUN_HIST};
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
/* It prints the error on stderr. 1st argument: the escape int code. 
*  2nd argument a string that indicates the fail reason.
*  3rd argument a string that indicates where the error occured. */
void prntfail(int, char*, char*);