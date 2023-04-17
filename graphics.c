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


/* gheader: graphically show the header node */
void gheader(t_header * ph)
{
 /*******************************************************************************
  *  A user acccessible function that graphs the tree header node.
  *
  *  Input Parameters
  *  =================
  *  ph         : Pointer to this tree header record.
  *
  *  Output Parameters
  *  =================
  *  Graph printed to stdout.
  *
  *  Global Variables
  *  =================
  *  none.
  *******************************************************************************/

    printf("               +-------------+\n");
    printf("               | M A L L O C |\n");
    printf("               +-------------+  <- (0x%-5x)\n", ph);
    printf("               |  t_header   |\n");
    printf("               |(%3i bytes)  |\n", sizeof(t_header));
    printf("               |             |\n");
    printf("               |     .       |\n");
    printf("               |     .       |\n");
    printf("               |     .       |\n");
    printf("               +-------------+\n");
}

/* gheader: graphically show the tree node */
void gnode(t_header * ph, t_node * pn)
{
 /*******************************************************************************
  *  A user acccessible function that graphs the tree leaf node.
  *
  *  Input Parameters
  *  =================
  *  ph         : Pointer to this tree header record.
  *  pn         : Pointer to this tree node.
  *
  *  Output Parameters
  *  =================
  *  Graph printed to stdout.
  *
  *  Global Variables
  *  =================
  *  none.
  *******************************************************************************/

    printf("               +-------------+\n");
    printf("               | M A L L O C |\n");
    printf("               +-------------+  <- (0x%-5x)\n", pn);
    printf("               |   t_node    |\n");
    printf("               | (%3i bytes) |\n", sizeof(t_node));
    printf("               |      .      |\n");
    printf("               +-------------+  <- (0x%-5x)\n", pn + 1);
    printf("               |   leaf.h    |\n");
    printf("               |  user data  |\n");
    printf("               | (%3i bytes) |\n", ph->th_usiz);
    printf("               |      .      |\n");
    printf("               |      .      |\n");
    printf("               |      .      |\n");
    printf("               +-------------+\n");
}
