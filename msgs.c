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

#define BASE   100		/* lower bound on the error numbers for indexing */
#define UPPER  125		/* last used error number in list of error msgs. */

t_header *find_header(char *);

static char *RCSid[] = { "$Id: msgs.c,v 2.2 1999/01/13 02:48:01 roger Exp $" };


/* bst_errmsg: return the corresponding error message */
char *bst_errmsg(int n)
{
 /*******************************************************************************
  *  A user acccessible function that returns the corresponding error message.
  *  Creates a new tree header node, initializes it and adds it to the global
  *  link list pointed to by t_head.
  *
  *  Input Parameters
  *  =================
  *  n          : Error code number
  *
  *  Output Parameters
  *  =================
  *  Function name returns corresponding error message:
  *
  *  Global Variables
  *  =================
  *  none.
  *******************************************************************************/

    static char *bst_errmsgs[] = {
	/* 100 */ "no error",
	/* 101 */ "specified tree not defined with bst_create",
	/* 102 */ "node specified is owned by a different tree",
	/* 103 */ "malloc unable to make memory allocation",
	/* 104 */ "size of user data area/structure is zero",
	/* 105 */ "calloc unable to make memory allocation",
	/* 106 */ "internal routine stack is full",
	/* 107 */ "internal routine stack is empty",
	/* 108 */ "avl tree is out of balance",
	/* 109 */ "no user written printing function specified",
	/* 110 */ "no user written compare function specified",
	/* 111 */ "tree is already defined",
	/* 112 */ "compare function cannot be the same as the print function",
	/* 113 */ "specified key not in tree",
	/* 114 */ "duplicate key -- key already in tree",
	/* 115 */ "tn_tag bits are incorrect",
	/* 116 */ "tree name too short",
	/* 117 */ "copy to tree is already defined - can't overwrite it",
	/* 118 */ "copy from tree is not defined",
	/* 119 */ "copy counts mismatch",
	/* 120 */ "compare tree #1 is not defined",
	/* 121 */ "compare tree #2 is not defined",
	/* 122 */ "trying to compare 2 trees with different data structures",
	/* 123 */ "tree name too short for first tree parameter",
	/* 124 */ "tree name too short for second tree parameter",
	/* 125 */ "undefined error number"
    };

    return (n < BASE || n > UPPER) ? bst_errmsgs[UPPER - BASE] : bst_errmsgs[n - BASE];
}
