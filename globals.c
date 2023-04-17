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

 /*******************************************************************************
  *  This module contains all variables and data structures used by the bst lib
  *  routines. All data here is accessible to both the library routines and quite
  *  possibly the user if they can deduce these variables.
  *
  *  Global Variables
  *  =================
  *  bst_errno : This global varible contains the last error number that occured
  *              in the routines
  *  t_head    : This global variable points to the head of defined bst tree
  *******************************************************************************/

#ifndef BST_HDR
#include "bst.h"
#endif

t_header *t_head = NULL;	/* global list of defined bst trees */
int bst_errno = BST_ERR_RESET;	/* global error var of last user op */

static char *RCSid[] = { "$Id: globals.c,v 1.5 1999/01/18 04:33:02 roger Exp roger $" };

static char *RCScopyright[] = {
    "$Id: +------------------------------------------------------------------+ $",
    "$Id: | Copyright (C) 2023 Roger P. Johnson, roger0080@netscape.net      | $",
    "$Id: |                                                                  | $",
    "$Id: | This file is part of libbst.                                     | $",
    "$Id: |                                                                  | $",
    "$Id: | libbst is free software: you can redistribute it and/or modify   | $",
    "$Id: | it under the terms of the GNU Lessor General Public License as   | $",
    "$Id: | published by the Free Software Foundation, either version 3 of   | $",
    "$Id: | the License, or (at your option) any later version.              | $",
    "$Id: |                                                                  | $",
    "$Id: | libbst is distributed in the hope that it will be useful, but    | $",
    "$Id: | WITHOUT ANY WARRANTY; without even the implied warranty of       | $",
    "$Id: | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the     | $",
    "$Id: | GNU Lessor General Public License for more details.              | $",
    "$Id: |                                                                  | $",
    "$Id: | You should have received a copy of the GNU Lessor General Public | $",
    "$Id: | License along with libbst. If not, see www.gnu.org/licenses.     | $",
    "$Id: +------------------------------------------------------------------+ $"
};
