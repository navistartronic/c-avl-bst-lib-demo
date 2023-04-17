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

static char *RCSid[] = { "$Id: tcp.c,v 2.1 1999/01/02 17:06:31 roger Exp $" };

extern int bst_errno;


/* bst_copy: make a copy of an existing tree */
Boolean bst_copy(char *from, char *to)
{
 /*******************************************************************************
  *  A user acccessible function that creates an exact copy of an existing
  *  tree.
  *
  *  Input Parameters
  *  =================
  *  from       : Copy source tree name.
  *  to         : Copy destination tree name.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : A new tree was created.
  *  FALSE      : Tree is already defined, not defined, or malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;
    t_header *find_header(char *);	/* to retrieve the tree header record */

    Boolean twalk(TWalkOps op, Traversals order, ...);


    /* Initialization */
    bst_errno = BST_ERR_RESET;

    /* Verify the length of the 'from' tree name: */
    if (strlen(from) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN_T1;
	return (FALSE);
    }

    /* Verify that the copy 'from' tree is defined: */
    if ((ph = (t_header *) find_header(from)) == NULL) {
	bst_errno = BST_ERR_COPY_FROM_NON_EXISTANT;
	return (FALSE);
    }

    /* Verify the length of the 'to' tree name: */
    if (strlen(to) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN_T2;
	return (FALSE);
    }

    /* Verify that the copy 'to' tree does *not* exist: */
    if (find_header(to) != NULL) {
	bst_errno = BST_ERR_COPY_TO_DEFINED;
	return (FALSE);
    }

    /* Copy the tree: */
    return (twalk(COPY, PREORDER, ph, to));
}
