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

t_header *find_header(char *);

static char *RCSid[] = { "$Id$" };

extern int bst_errno;


/* bst_print: print out a node using a user supplied function */
void bst_print(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that will graphically  print out the users tree.
  *  Only line spacing and tabbing is done on output.  When the root node is 
  *  encountered a slash is placed in front to symbolize the root node.  Also 
  *  the tree prints with the right side of the tree at the top of screen and 
  *  works its way down the screen until the left edge of the tree is displayed.   
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to print.
  *
  *  Output Parameters
  *  =================
  *  none.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    Boolean done, end_of_right_branch, try_going_right, time_to_go_left;
    int depth;
    t_node *p;
    t_header *ph;

    depth = 0;
    bst_errno = BST_ERR_RESET;

    printf(">>> using new print tree module (print.c) <<<\n");

    /* check if tree is even defined */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return;
    }

    /* now check if user has passed a printing function for this tree */
    if (ph->th_upf == NULL) {
	bst_errno = BST_ERR_NO_UPF_GIVEN;
	return;
    }

    /* any nodes in tree to traverse? */
    if (ph->th_root == NULL) {
	ph->th_upf(NULL, -1);
	return;
    }

    p = ph->th_root;
    done = FALSE;
    while (!done) {
	end_of_right_branch = FALSE;
	while (!end_of_right_branch) {
	    if (p->tn_rlink != NULL) {
		p = p->tn_rlink;
		depth++;
	    } else
		end_of_right_branch = TRUE;
	}
	try_going_right = FALSE;
	while (!try_going_right && !done) {
	    ph->th_upf(((t_node *) p) + 1, depth);
	    /* printf("%i;depth = %i\n",p->data,depth); */
	    if (p->tn_llink != NULL) {
		p = p->tn_llink;
		depth++;
		try_going_right = TRUE;
	    }
	    if (!try_going_right) {
		time_to_go_left = FALSE;
		while (!done && !time_to_go_left) {
		    if (!done) {
			depth--;
			if (p->tn_tag == ROOT)
			    done = TRUE;
			else if (p->tn_tag == RIGHT_SON) {
			    time_to_go_left = TRUE;
			    try_going_right = FALSE;
			}
			p = p->tn_ulink;
		    }
		}
	    }
	}
    }
}
