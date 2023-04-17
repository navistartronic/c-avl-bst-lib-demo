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

static char *RCSid[] = { "$Id: rebalance.c,v 2.2 1999/01/27 01:17:06 roger Exp $" };


void rbal(t_node ** treeroot, t_node * a, t_node * f, t_node * q, t_node * b, int d)
{
 /*******************************************************************************
  *  A private library function that will rbal the AVL tree to a balanced state 
  *  after  an insertion is performed and leaves the tree unbalanced.
  *
  *  Input Parameters
  *  =================
  *  a          : Pointer to pointer (t_node) to the nearest ancestor
  *               with a balance factor equal to +1 or -1.
  *  b          : Pointer to pointer (t_node) to a's left or right
  *               subtree depending on whether the new node was inserted in
  *               a.left or a.right subtree.
  *  d          : Flag for which side of the subtree of a the new node was
  *               inserted on.
  *                 d = +1: right side; d = -1: left side.
  *  f          : Pointer to pointer (t_node) to the parent of a.
  *  q          : Pointer to pointer (t_node) to the parent of new node.
  *  treeroot   : Pointer to pointer (t_node) to the current root tree.
  *
  *  Output Parameters
  *  =================
  *  None. Tree is rbald to a balanced state via rotations.
  *
  *  Global Variables
  *  =================
  *  None.
  *******************************************************************************/

    t_node *c;

    /* Upon Entry:
     *  d = +1 implies a left imbalance in the tree:  H(l)-H(r) = +1
     *  d = -1 implies a right imbalance in the tree: H(l)-H(r) = -1
     *  a is pointing to the nearest ancestor with a balance factor + or - 1
     *  f is a's parent node or NULL
     *  b is pointing to a's left subtree (d = +1) or a's right subtree (d = -1)
     *  c replaces a as the parent node of the subtree being rotated 
     */

    if (d == +1)
	/* Left imbalance H(l)-H(r) = +1 */
	if (b->tn_bf == +1) {
	    /* LL ROTATION */
#ifdef DEBUG_SHOWREBALANCE
	    printf(">> LL << rotation\n");
#endif
	    /* LINKS */
	    a->tn_llink = b->tn_rlink;
	    b->tn_rlink = a;
	    b->tn_ulink = a->tn_ulink;
	    a->tn_ulink = b;
	    if (a->tn_llink != NULL)
		a->tn_llink->tn_ulink = a;
	    /* TAGS */
	    b->tn_tag = a->tn_tag;
	    a->tn_tag = RIGHT_SON;
	    if (a->tn_llink != NULL)
		a->tn_llink->tn_tag = LEFT_SON;
	    /* BALANCE FACTORS */
	    a->tn_bf = 0;
	    b->tn_bf = 0;
	} else {
	    /* LR ROTATION */
	    /* LINKS */
	    c = b->tn_rlink;
	    b->tn_rlink = c->tn_llink;
	    a->tn_llink = c->tn_rlink;
	    c->tn_llink = b;	/* c is now new root of subtree */
	    c->tn_rlink = a;

	    switch (c->tn_bf) {	/* readjust affected balance factors */

		/* LR(b) */
	    case +1:		/* UPLINKS */
#ifdef DEBUG_SHOWREBALANCE
		printf(">> LR(b) << rotation\n");
#endif
		c->tn_ulink = a->tn_ulink;
		b->tn_ulink = c;
		a->tn_ulink = c;
		b->tn_rlink->tn_ulink = b;
		if (a->tn_llink != NULL)
		    a->tn_llink->tn_ulink = a;
		/* TAGS */
		c->tn_tag = a->tn_tag;
		a->tn_tag = RIGHT_SON;
		b->tn_rlink->tn_tag = RIGHT_SON;
		if (a->tn_llink != NULL)
		    a->tn_llink->tn_tag = LEFT_SON;
		/* BALANCE FACTORS */
		a->tn_bf = -1;
		b->tn_bf = 0;
		break;
		/* LR(a) */
	    case 0:		/* UPLINKS */
#ifdef DEBUG_SHOWREBALANCE
		printf(">> LR(a) << rotation\n");
#endif
		c->tn_ulink = a->tn_ulink;
		b->tn_ulink = c;
		a->tn_ulink = c;
		/* TAGS */
		c->tn_tag = a->tn_tag;
		a->tn_tag = RIGHT_SON;
		/* BALANCE FACTORS */
		a->tn_bf = 0;
		b->tn_bf = 0;
		break;
		/* LR(c) */
	    case -1:		/* UPLINKS */
#ifdef DEBUG_SHOWREBALANCE
		printf(">> LR(c) << rotation\n");
#endif
		c->tn_ulink = a->tn_ulink;
		b->tn_ulink = c;
		a->tn_ulink = c;
		a->tn_llink->tn_ulink = a;
		if (b->tn_rlink != NULL)
		    b->tn_rlink->tn_ulink = b;
		/* TAGS */
		c->tn_tag = a->tn_tag;
		a->tn_tag = RIGHT_SON;
		a->tn_llink->tn_tag = LEFT_SON;
		if (b->tn_rlink != NULL)
		    b->tn_rlink->tn_tag = RIGHT_SON;
		/* BALANCE FACTORS */
		a->tn_bf = 0;
		b->tn_bf = 1;
		break;
	    }			/* switch */

	    c->tn_bf = 0;
	    b = c;		/* copy pointer for wrap up at the end */

	} /* else */
    else			/* d = -1 */
    /* right imbalance H(l)-H(r) = -1 */ if (b->tn_bf == -1) {
	/* RR ROTATION */
#ifdef DEBUG_SHOWREBALANCE
	printf(">> RR << rotation\n");
#endif
	/* LINKS */
	a->tn_rlink = b->tn_llink;
	b->tn_llink = a;
	/* UP LINKS */
	b->tn_ulink = a->tn_ulink;
	a->tn_ulink = b;
	if (a->tn_rlink != NULL)
	    a->tn_rlink->tn_ulink = a;
	/* TAGS */
	b->tn_tag = a->tn_tag;
	a->tn_tag = LEFT_SON;
	if (a->tn_rlink != NULL)
	    a->tn_rlink->tn_tag = RIGHT_SON;
	/* BALANCE FACTORS */
	a->tn_bf = 0;
	b->tn_bf = 0;
    } else {
	/* RL ROTATION */
	/* LINKS */
	c = b->tn_llink;
	b->tn_llink = c->tn_rlink;
	a->tn_rlink = c->tn_llink;
	c->tn_rlink = b;	/* c is now new parent of subtree */
	c->tn_llink = a;

	switch (c->tn_bf) {	/* readjust affected balance factors */
	    /* RL(c) */
	case +1:		/* UP LINKS */
#ifdef DEBUG_SHOWREBALANCE
	    printf(">> RL(c) << rotation\n");
#endif
	    c->tn_ulink = a->tn_ulink;
	    a->tn_ulink = c;
	    b->tn_ulink = c;
	    a->tn_rlink->tn_ulink = a;
	    if (b->tn_llink != NULL)
		b->tn_llink->tn_ulink = b;
	    /* TAGS */
	    c->tn_tag = a->tn_tag;
	    a->tn_tag = LEFT_SON;
	    a->tn_rlink->tn_tag = RIGHT_SON;
	    if (b->tn_llink != NULL)
		b->tn_llink->tn_tag = LEFT_SON;
	    /* BALANCE FACTORS */
	    a->tn_bf = 0;
	    b->tn_bf = -1;
	    break;
	    /* RL(a) */
	case 0:		/* UP LINKS */
#ifdef DEBUG_SHOWREBALANCE
	    printf(">> RL(a) << rotation\n");
#endif
	    c->tn_ulink = a->tn_ulink;
	    a->tn_ulink = c;
	    b->tn_ulink = c;
	    /* TAGS */
	    c->tn_tag = a->tn_tag;
	    a->tn_tag = LEFT_SON;
	    /* BALANCE FACTORS */
	    a->tn_bf = 0;
	    b->tn_bf = 0;
	    break;
	    /* RL(b) */
	case -1:		/* UP LINKS */
#ifdef DEBUG_SHOWREBALANCE
	    printf(">> RL(b) << rotation\n");
#endif
	    c->tn_ulink = a->tn_ulink;
	    a->tn_ulink = c;
	    b->tn_ulink = c;
	    b->tn_llink->tn_ulink = b;
	    if (a->tn_rlink != NULL)
		a->tn_rlink->tn_ulink = a;
	    /* TAGS */
	    c->tn_tag = a->tn_tag;
	    a->tn_tag = LEFT_SON;
	    b->tn_llink->tn_tag = LEFT_SON;
	    if (a->tn_rlink != NULL)
		a->tn_rlink->tn_tag = RIGHT_SON;
	    /* BALANCE FACTORS */
	    a->tn_bf = +1;
	    b->tn_bf = 0;
	    break;

	}			/* switch */

	c->tn_bf = 0;
	b = c;			/* copy pointer for wrap up at the end */

    }				/* else */

    /* wrap up */
    if (f == NULL)
	*treeroot = b;		/* new tree root */
    else if (a == f->tn_llink)
	f->tn_llink = b;	/* finish linking from f to new f.left  LL */
    else if (a == f->tn_rlink)
	f->tn_rlink = b;	/* finish linking from f to new f.right LL */
}
