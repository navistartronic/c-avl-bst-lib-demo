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

#define  UNBALANCED  TRUE

static char *RCSid[] = { "$Id: put.c,v 2.2 1999/01/27 02:24:25 roger Exp $" };

extern int bst_errno;


/* bst_put: insert a new node into the tree */
Boolean bst_put(char *tname, void *pl)
{
 /*******************************************************************************
  *  A user acccessible function that inserts a new node into the tree from the
  *  user. User supplies a tree node and we copy their data into the newly create
  *  internal tree node here.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to define.
  *  pl         : Pointer to a tree node users Leaf area.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : User node added to tree.
  *  FALSE      : Tree not defined, node mismatch, or malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    int d;
    t_header *ph;
    t_node *pcopy, *pn, *pr, *a, *f, *q, *b;
    t_header *find_header(char *);

    extern void *tallocm(MallocTypes mkind, ...);
    extern void bst_stat(char *tname);
    extern t_node *find_node(t_node * treeroot, void *keyrecord, int (*th_ucf) (void *, void *), t_node ** a, t_node ** f,
			     t_node ** q);
    extern Boolean put_node(t_header * ph, t_node * pcopy, int (*th_ucf) (), t_node * a, t_node * q, t_node ** b, int *d);
    extern void rbal(t_node ** treeroot, t_node * a, t_node * f, t_node * q, t_node * b, int d);

    bst_errno = BST_ERR_RESET;

    /* Check if the tree is defined: */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (FALSE);
    }

    /* Set the pointer from the users data area to the header of the node: */
    pn = ((t_node *) pl) - 1;

    /* Check if this node then belongs to this tree: */
    if (ph->th_id != pn->tn_id) {
	bst_errno = BST_ERR_TREE_NODE_MISMATCH;
	return (FALSE);
    }

    /* Search tree and set pointers for place of insertion: */
    if (find_node(ph->th_root, pl, ph->th_ucf, &a, &f, &q) != NULL) {
	bst_errno = BST_ERR_DUPLICATE_KEY;
	return (FALSE);
    }

    /* Make an exact copy of the structure the user is inserting; this will then become */
    /* the node that is actually placed in the tree:                                    */
    if ((pcopy = (t_node *) tallocm(T_NODE, ph)) == NULL)
	return (FALSE);
    memcpy(pcopy, pn, sizeof(t_node) + ph->th_usiz);
#ifdef DEBUG_MALLAC_USAGE
    printf(">>> memcpy FROM LOCATION 0x%-5x TO LOCATION 0x%-5x; %i BYTES <<<\n", pn, pcopy, sizeof(t_node) + ph->th_usiz);
#endif

    /* Link in the the copy node and rebalance the tree if necessary: */
    if (put_node(ph, pcopy, ph->th_ucf, a, q, &b, &d) == UNBALANCED)
	if (ph->th_bsttype == AVL)
	    rbal(&ph->th_root, a, f, q, b, d);
    ph->th_ncnt++;

    /* *_stat are left in for development purposes only; it verifies the condition of */
    /* the tree by traversing the whole tree checking for accuracy:                   */
    if (ph->th_bsttype == AVL && ph->th_stat)
	bst_stat(tname);

    /* Successful node insertion: */
    return (TRUE);
}
