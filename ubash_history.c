/*
 *  microbash - SETI course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2020-2021 - Italy
 *
 *  ubash_history.c
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
#include "ubash_history.h"
#include "ubash_error.h"
/*------------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------*/
void prnt_hstry(){
	register HIST_ENTRY** my_history_list;

	my_history_list = history_list();
	if(my_history_list != NULL){
		for (int i = 0; my_history_list[i] != NULL; ++i)
			fprintf (stdout, "%d: %s\n", (i + 1), my_history_list[i]->line);
	}
	else
		prntfail(FUN_HIST, NULL, "prnt__hstry");
}
