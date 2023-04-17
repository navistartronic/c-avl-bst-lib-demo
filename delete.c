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

static char *RCSid[] = { "$Id: delete.c,v 2.1 1999/01/02 16:55:13 roger Exp $" };

extern t_header *t_head;
extern int bst_errno;


/* bst_delete: delete tree header and free it */
Boolean bst_delete(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that deletes an AVL or BST search tree and 
  *  removes it from the link list of trees pointed to by t_head.
  *  To delete a complete tree, the tree is traversed freeing all nodes, then any
  *  pointers to char in the header record are free, and then finally the tree
  *  header record itself is freed.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the tree to delete.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Tree deleted sucessfully.
  *  FALSE      : Tree does not exist.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;

    t_header *find_header(char *);
    void tdispose(t_header *);

    bst_errno = BST_ERR_RESET;

    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (FALSE);
    }

    tdispose(ph);

    return (TRUE);
}
