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

static char *RCSid[] = { "$Id: get.c,v 2.2 1999/01/18 03:41:38 roger Exp $" };

extern int bst_errno;
extern t_header *find_header(char *);
extern t_node *find_node(t_node *, void *, int (*)(), t_node **, t_node **, t_node **);
extern void *tallocm(MallocTypes mkind, ...);


/* bst_get: search and return a copy of the node with specified key to user */
void *bst_get(char *tname, void *kname)
{
 /*******************************************************************************
  *  A user acccessible function searches the tree for the specified node & key.
  *  If the desired key found in the tree, an exact copy of the node is made and 
  *  that node is returned to the user. This copy of the node returned is 
  *  pointing (cast) to the users data area of node. 
  *
  *  Upon exit, if found, bst_get is pointing to the users data area
  *  which is of type t_node, which will then be cast to the proper
  *  type by the user in the assignment statement: leafptr = bst_get(...)
  *
  *  Input Parameters
  *  =================
  *  tname : Name of tree to search.
  *  kname : User defined structure which contains the key to search for.
  *
  *  Output Parameters
  *  =================
  *  Function name returns point to copy of found node or NULL.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;
    t_node *a, *f, *q, *pn, *pcopy;


    bst_errno = BST_ERR_RESET;

    /* check if tree even exists */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (NULL);		/* tree not defined */
    }

    /* cast pointer from users data part to header node part of node */
    pn = ((t_node *) kname) - 1;	/* cast pointer from leaf type to header type */

    /* check if the node even belongs to this tree */
    if (ph->th_id != pn->tn_id) {
	bst_errno = BST_ERR_TREE_NODE_MISMATCH;
	return (NULL);
    }

    /* find the node in the tree returning a pointer to it */
    if ((pn = find_node(ph->th_root, kname, ph->th_ucf, &a, &f, &q)) == NULL) {
	bst_errno = BST_ERR_KEY_NOT_FOUND;
	return (NULL);
    }

    /* make copy of found node to return to user */
    if ((pcopy = (t_node *) tallocm(T_NODE, ph)) == NULL) {
	bst_errno = BST_ERR_MALLOC;
	return (NULL);
    }
    memcpy(pcopy, pn, sizeof(t_node) + ph->th_usiz);

#ifdef DEBUG_MALLAC_USAGE
    printf(">>> memcpy FROM LOCATION 0x%-5x TO LOCATION 0x%-5x; %i BYTES <<<\n", pn, pcopy, sizeof(t_node) + ph->th_usiz);
#endif

    return (pcopy + 1);		/* point from header part to users data area */
}
