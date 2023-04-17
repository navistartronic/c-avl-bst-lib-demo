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

static char *RCSid[] = { "$Id: defined.c,v 2.1 1999/01/02 16:54:51 roger Exp $" };

extern int bst_errno;


/* bst_defined: check if tree name is created or not */
Boolean bst_defined(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that checks if the named tree is defined or not.
  *
  *  Input Parameters
  *  =================
  *  tname      : String tree name to check on
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Tree is defined.
  *  FALSE      : Tree is not defined.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *find_header(char *);

    bst_errno = BST_ERR_RESET;

    if (find_header(tname) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (FALSE);
    } else
	return (TRUE);
}
