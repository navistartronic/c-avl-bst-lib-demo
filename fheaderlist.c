/*
+------------------------------------------------------------------+
| Copyright (C) 2023 Roger P. Johnson, roger0080@netscape.net      |
|                                                                  |
| This file is part of libbst.                                     |
|                                                                  |
| libbst is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU Lessor General Public License as   |
| published by the Free Software Foundation, either version 3 of   |
| the License, or (at your option) any later version.              |
|                                                                  |
| libbst is distributed in the hope that it will be useful, but    |
| WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the     |
| GNU Lessor General Public License for more details.              |
|                                                                  |
| You should have received a copy of the GNU Lessor General Public |
| License along with libbst. If not, see www.gnu.org/licenses.     |
+------------------------------------------------------------------+
*/

#ifndef BST_HDR
#include "bst.h"
#endif

static char *RCSid[] = { "$Id$" };

extern t_header *t_head;
extern int bst_errno;


/* find_header_list: return a dymaincally created array of defined tree names */
void find_header_list(char ***stringArr, int *arrSize)
{
 /*******************************************************************************
  *  A user accessible function that returns that returns a dynamically created 
  *  array of  tree names as it scans through the linked list of tree headers.
  * 
  *  The caller of this routine needs to at some point loop through the array of
  *  tree names and free each one. Then free the pointer to the array itself.
  *
  *  Input Parameters
  *  =================
  *
  *  Output Parameters
  *  =================
  *  stringArr  : Pointer to array of pointers to strings.
  *  arrSize    : Number of strings (index) in array.
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *  bst_errno  : Global error variable; *NOT* reset on entry.
  *******************************************************************************/

    /* Creating a dynamically created array of tree names adopted from:
     *    Detailed:   https://cboard.cprogramming.com/c-programming/172507-dynamicaly-allocating-memory-unknown-number-strings-unknown-sizes-c.html
     *    Basic idea: https://cboard.cprogramming.com/c-programming/130826-dynamicly-resize-array-strings.html
     * 
     *    Using newer gcc 11.3.0 pukes at: realloc(): invalid pointer (core dumped)
     *    Worked fine when only one tree defined; two or more looping and realloc() would fail.
     *    Older gcc 3.4.3 and Sun Studio 12 worked fine.
     *    Fix/hint suggested by Daniel Fischer commenting about uninitialized after realloc(); needed to add initialization to NULL
     *    https://stackoverflow.com/questions/15731703/c-glibc-realloc-invalid-pointer
     */

    t_header *ph;
    char p_buff[MAX_TREE_NAME_LEN + 1];
    char **storage;
    int size;
    int i;

    size = 0;
    storage = (char **) malloc((size + 1) * sizeof(char *));
    storage[size] = (char *) malloc(sizeof(char));

    for (ph = t_head; ph != NULL; ph = ph->th_link) {
	memset(p_buff, '\0', MAX_TREE_NAME_LEN + 1);
	strncpy(p_buff, ph->th_name, MAX_TREE_NAME_LEN);

	storage[size] = (char *) realloc(storage[size], (strlen(p_buff) + 1) * sizeof(char));
	strcpy(storage[size], p_buff);

	size++;
	storage = (char **) realloc(storage, (size + 1) * sizeof(char *));
        storage[size] = NULL;   /* critical, older gcc & Sun Studio did not need this else realloc(): invalid pointer (core dumped)  */
    }

    /*
     * for(i = 0; i < size; i++) {
     *     printf("loop: storage[%d] = [%s]\n", i, storage[i]);
     * }
     */

    *arrSize = size;
    *stringArr = storage;
}
