/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_changedir.h
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
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
/* Unique built-in command: change directory.
*  It returns ERROR if something is wrong, CD otherwise. 1st argument: pathname. */
int mycd(char*);
/* It checks if there's a change directory request in the parsed command line,
*  it returns OK if there's "cd" as first argument in any command, execute it if valid,
*  ERROR otherwise. 1st argument: command list, 2nd argument: number of command(s). */
int checkcd(char***, const int);
