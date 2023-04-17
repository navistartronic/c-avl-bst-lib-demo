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


/* put_node: insert a node into the tree. */
Boolean put_node(t_header * ph, t_node * pcopy, int (*th_ucf) (), t_node * a, t_node * q, t_node ** b, int *d)
{
 /*******************************************************************************
  *  A private library function that inserts a tree node into the tree returning
  *  the balance of the tree.
  *
  *  Input Parameters
  *  =================
  *    pcopy: Pointer (t_node) to the new node to insert.
  *    th_ucf  : Pointer to user written compare function.
  *    a    : Pointer to pointer (t_node) to last node with tn_bf **
  *           equal to +1 or -1 closest to the new node pcopy.
  *    q    : Pointer to pointer (t_node) to the parent node of the
  *           new node pcopy.
  *
  *  Output Parameters
  *  =================
  *  Function name returns insertion result:
  *  TRUE       : Tree is unbalanced.
  *  FALSE      : Tree is still balanced after the insertion.
  *
  *  Global Variables
  *  =================
  *  none
  *******************************************************************************/

    Boolean unbalanced;
    t_node *p, *pn;

    if (ph->th_root == NULL) {	/* empty tree - special case      */
	ph->th_root = pcopy;	/* new node becomes the root node */
	pcopy->tn_ulink = NULL;
	pcopy->tn_tag = ROOT;
	pcopy->tn_bf = 0;
	return (FALSE);		/* new node successfully added    */
    }

    /* Link the new node into the tree by linking it to its parent node pointed to by q: */
    pcopy->tn_ulink = q;

    if (th_ucf(pcopy + 1, q + 1) < 0) {
	q->tn_llink = pcopy;
	pcopy->tn_tag = LEFT_SON;
    } else {
	q->tn_rlink = pcopy;
	pcopy->tn_tag = RIGHT_SON;
    }

    /* Set *p to the head of the path from *a to *q to adjust those balance   */
    /* factors on that path and set flag d to which side of the subtree the   */
    /* new node was inserted on:                                              */
    if (th_ucf(pcopy + 1, a + 1) > 0) {
	p = a->tn_rlink;	/* head of path starts in a.right subtree     */
	*b = p;			/* b is an additional pointer                 */
	*d = -1;		/* new node is inserted in right subtree of a */
    } else {
	p = a->tn_llink;	/* head of path starts in a.left subtree      */
	*b = p;
	*d = +1;		/* new node is inserted in left subtree of a  */
    }

    /* Trace down the path from a to q, adjusting each node tn_bf     */
    /* whether the new node was inserted in the left or right subtree         */
    while (p != pcopy) {
	if (th_ucf(pcopy + 1, p + 1) < 0) {
	    p->tn_bf = +1;
	    p = p->tn_llink;
	} else {
	    p->tn_bf = -1;
	    p = p->tn_rlink;
	}
    }

    /* Now after insertion, check if tree is unbalanced: */
    unbalanced = TRUE;

    /* The new node was inserted into the perfectly balanced subtree a,   */
    /* so now either H(l)-H(r) = -1 or H(l)-H(r) = +1                     */
    if (a->tn_bf == 0) {
	a->tn_bf = *d;		/* d = -1, left side; d = +1, right side  */
	unbalanced = FALSE;
    }

    /* The new node was inserted into the opposite side of subtree a,  */
    /* making the subtree a now perfectly balanced: H(l)-H(r) = 0      */
    if (a->tn_bf + *d == 0) {
	a->tn_bf = 0;
	unbalanced = FALSE;
    }
    return (unbalanced);
}
