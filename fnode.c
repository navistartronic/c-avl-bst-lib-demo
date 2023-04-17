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

static char *RCSid[] = { "$Id: fnode.c,v 2.2 1999/01/18 03:20:52 roger Exp $" };

extern int bst_errno;


/* find_node: search the tree for the given node */
t_node *find_node(t_node * treeroot, void *keyrecord, int (*th_ucf) (void *, void *), t_node ** a, t_node ** f, t_node ** q)
{
 /*******************************************************************************
  *  An internal library function that finds and returns the desired node in the tree.
  *
  *  Input Parameters
  *  =================
  *  treeroot   : Pointer to the root (t_node) of the AVL tree to search.
  *               (Found by function find_header which passes the root)
  *  keyrecord  : Target key to find in the AVL tree. Structure def. unknown.
  *  th_ucf     : Pointer to the user written compare function used to make
  *               the comparisions for this tree.(From find_header).
  *
  *  Output Parameters
  *  =================
  *  a          : Pointer to pointer (t_node) to the last node with a
  *               balance factor NOT equal to zero.
  *  f          : Pointer to pointer (t_node) to the parent node of 'a'
  *  q          : Pointer to pointer (t_node) to the parent of the new
  *               node if not found and to insert a new node.
  *  Function name returns copy of found node or NULL.
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *  bst_errno  : Global error variable; *NOT* reset on entry.
  *******************************************************************************/

    int cmpresult;
    t_node *p;

    *f = NULL;			/* f is pointer to father of a */
    p = treeroot;		/* p leads the way thru tree */
    *q = NULL;			/* q follows p around */
    *a = treeroot;		/* a is pointer to last node with bf + or - 1 */

    /* scan down through the tree searching for the desired key while making */
    /* note of where the last node with a balance factor of +1 or -1 is,     */
    /* just in case the next operation is a key insertion.                   */

    while (p != NULL) {
	if (p->tn_bf != 0) {
	    *a = p;		/* last node with bf = + or - 1 */
	    *f = *q;		/* f is the parent node of a */
	}
	cmpresult = th_ucf(keyrecord, p + 1);	/* make key comparison call */

	if (cmpresult < 0) {	/* move down through left subtree */
	    *q = p;
	    p = p->tn_llink;
	} else if (cmpresult > 0) {	/* move down through right subtree */
	    *q = p;
	    p = p->tn_rlink;
	} else			/* found it */
	    return p;		/* p points the header part of the node */
    }

    bst_errno = BST_ERR_KEY_NOT_FOUND;

    return (NULL);		/* key not in tree */
}
