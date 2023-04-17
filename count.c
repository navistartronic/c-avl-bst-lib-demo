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

#define UNDEFINED_COUNT -1

static char *RCSid[] = { "$Id: count.c,v 2.1 1999/01/02 16:54:19 roger Exp $" };

extern int bst_errno;


/* bst_count: return the number of nodes in the tree */
int bst_count(char *tname)
{
 /*******************************************************************************
  *  A user accessible function that returns the current number of nodes in 
  *  the specified tree. No tree traversal takes place as the count is stored 
  *  in the tree header record.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of tree
  *
  *  Output Parameters
  *  =================
  *  Function name returns number of nodes in tree
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
	return (UNDEFINED_COUNT);
    } else
	return (ph->th_ncnt);
}
