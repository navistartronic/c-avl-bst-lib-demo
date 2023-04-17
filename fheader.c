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

static char *RCSid[] = { "$Id: fheader.c,v 2.1 1999/01/02 16:56:23 roger Exp $" };

extern t_header *t_head;
extern int bst_errno;


/* find_header: search the link list of defined trees for this named tree */
t_header *find_header(char *tname)
{
 /*******************************************************************************
  *  An internal library function that searches the linked list of trees for
  *  the specified tree name.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to search for.
  *
  *  Output Parameters
  *  =================
  *  Function name returns point to tree header record or NULL.
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *  bst_errno  : Global error variable; *NOT* reset on entry.
  *******************************************************************************/

    t_header *ph;

    /* Verify the length of the copy to tree name: */
    if (strlen(tname) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN;
	return (TREE_NOT_DEFINED);
    }

    /* Search the linked list for the specified tree: */
    for (ph = t_head; ph != NULL; ph = ph->th_link)
	if (strcmp(ph->th_name, tname) == IDENTICAL)
	    return (ph);

    return (TREE_NOT_DEFINED);
}
