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

extern int bst_errno;


/* bst_alloc: allocate a tree node back to the user */
void *bst_alloc(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that returns a node for the user to use.
  *  This node may be a new one generated from malloc, or a used one from a 
  *  linked list of reusable nodes for that particular tree. This linked list
  *  of reusable nodes is pointed to by 'th_flist' in the tree header record.
  *  If no resusable node exist in the th_flist, a new node is created via 
  *  malloc. Before returning a pointer to the  users data area, the users 
  *  data area is zero filled, erasing the previous contents.         
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the to get a node from.
  *
  *  Output Parameters
  *  =================
  *  Function name returns zero-filled tree node.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_node *pn;
    t_header *ph;

    extern void *tallocm(MallocTypes mkind, ...);
    extern t_header *find_header(char *);

    bst_errno = BST_ERR_RESET;

    /* check if tree is defined */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (TREE_NOT_DEFINED);	/* tree not defined */
    }

    /* check if any nodes for this tree is available from the th_flist */
    /* if not, make up a new one                                       */
    if ((pn = (t_node *) tallocm(T_NODE, ph)) == NULL)
	return (pn);

    /* Initialize header node */
    pn->tn_llink = NULL;
    pn->tn_rlink = NULL;
    pn->tn_ulink = NULL;
    pn->tn_id = ph->th_id;	/* mark the tree owner of this node ! */
    pn->tn_bf = 0;

    return (void *) (pn + 1);	/* points to the users data area */
}
