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

static char *RCSid[] = { "$Id: tident.c,v 2.1 1999/01/02 17:08:17 roger Exp $" };

extern int bst_errno;


/* bst_ident: check if two trees are identical in contents */
Boolean bst_ident(char *t1, char *t2)
{
 /*******************************************************************************
  *  A user acccessible function that that checks if two trees are identical in 
  *  structure. If two trees are identical, then TRUE is returned else FALSE is 
  *  returned.
  *
  *  For two trees to be identical they must:
  *   i. Both be defined
  *  ii. They must both have the same node size.
  * iii. They must both have the same number of nodes in each tree
  *  iv. Each key in tree #1 must be in tree #2
  *   v. The postion of the key in tree #1 must be in the *same* position in
  *      tree #2.
  *
  * Any key missing/mispositioned in tree #2 breaks the test and returns FALSE
  *
  * To determine if two trees are of the same "family", the user's structure is
  * is compared in each tree by examining the value stored in ph->th_usiz. If
  * these two values are equal, it is assumed that the trees are of the same
  * family or structure. And it is entirely possible that although the 2 values
  * are equal, the physical make up of the two structures are entirely different
  * altogether. In this case a segmentaion fault can occur.
  *
  *  Input Parameters
  *  =================
  *  t1         : String name of tree #1 to compare
  *  t2         : String name of tree #2 to compare
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  FALSE       :if a node is missing or different keys in the two nodes
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph1, *ph2;
    t_header *find_header(char *);	/* to retrieve the tree header record */

    Boolean twalk(TWalkOps op, Traversals order, ...);

    /* Initialization */
    bst_errno = BST_ERR_RESET;

    /* Verify the length of the 1st tree name: */
    if (strlen(t1) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN_T1;
	return (FALSE);
    }

    /* Check if 1st tree is defined: */
    if ((ph1 = (t_header *) find_header(t1)) == NULL) {
	bst_errno = BST_ERR_FIRST_TREE_UNDEF;
	return (FALSE);
    }

    /* Verify the length of the 2nd tree name: */
    if (strlen(t2) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN_T2;
	return (FALSE);
    }

    /* Check if 2nd tree is defined: */
    if ((ph2 = (t_header *) find_header(t2)) == NULL) {
	bst_errno = BST_ERR_SECOND_TREE_UNDEF;
	return (FALSE);
    }

    /* Check if the users data size is the same for both trees. If so, there is still no  */
    /* guarentee that the structures are really the same -- which can cause a segmentaion */
    /* fault to occur:                                                                    */
    if (ph1->th_usiz != ph2->th_usiz) {
	bst_errno = BST_ERR_TREES_NOT_SAME_FAMILY;
	return (FALSE);
    }

    /* Check if the number of nodes in each tree are the same as an easy check:        */
    if (ph1->th_ncnt != ph2->th_ncnt)
	return (FALSE);

    /* It is possible to have one tree an AVL and the other a BST and yet identical */

    /* Make the comparison call: */
    return (twalk(IDENT, PREORDER, ph1, ph2));
}
