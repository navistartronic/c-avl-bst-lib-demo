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

/* private library modules include this file for use of libbst.a */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  BST_HDR    1		/* so only one include of this file */

#define  VERSION_ID          "Revision 1"
#define  TREE_NOT_DEFINED    NULL
#define  EMPTY_TREE          NULL
#define  EMPTY_LIST          NULL
#define  IDENTICAL           0
#define  MISSING             NULL
#define  MAX_FLIST           (unsigned int) 128
#define  MIN_TREE_NAME_LEN   1
#define  MAX_TREE_NAME_LEN   128
#define  MAX_ID_LEN          32

#include "typedefs.h"
#include "struct.h"
#include "errno.h"
