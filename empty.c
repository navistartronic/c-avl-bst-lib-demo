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

static char *RCSid[] = { "$Id: empty.c,v 2.1 1999/01/02 16:56:09 roger Exp $" };

extern int bst_errno;


/* bst_empty: check if tree has any nodes in it */
Boolean bst_empty(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that creates a new AVL or BST search tree.
  *  Creates a new tree header node, initializes it and adds it to the global
  *  link list pointed to by t_head.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to check on.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Tree is emtpy.
  *  FALSE      : Tree not defined or has at least one node in it.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;
    t_header *find_header(char *);

    bst_errno = BST_ERR_RESET;

    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (TRUE);
    }

    if (ph->th_root == EMPTY_TREE)
	return (TRUE);
    else
	return (FALSE);
}
