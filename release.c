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

static char *RCSid[] = { "$Id: release.c,v 2.2 1999/01/13 01:03:22 roger Exp $" };


/* bst_release: release a users work node previous retreived from bst_get(). */
Boolean bst_release(char *tname, void *pl)
{
 /*******************************************************************************
  *  A user acccessible function that  puts back a node no longer needed, into 
  *  the list of free nodes (reusable nodes) for that tree. Then the users 
  *  pointer is set to null to prevent the user from referencing the node 
  *  anymore.                     
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to define.
  *  pl         : Pointer to the node to return to the tree free list. Note
  *               that it points to the users data area and not to the header
  *               part of the node. This must be cast back to the header part
  *               of the node first.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Node returned back to the tree_header.
  *  FALSE      : Tree not defined or malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;
    t_node *pn;

    extern t_header *find_header(char *);
    extern int bst_errno;
    extern void tfreem(MallocTypes mkind, ...);

    bst_errno = BST_ERR_RESET;

    /* check if tree is defined */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (FALSE);	/* tree not defined */
    }

    /* cast the pointer back to the header part of the node from pointing */
    /* at the users data area:                                            */
    pn = ((t_node *) pl) - 1;

    /* check if passed node belongs to this tree */

    if (ph->th_id != pn->tn_id) {
	bst_errno = BST_ERR_TREE_NODE_MISMATCH;
	return (FALSE);
    }

    /* now add it to the free list in the header record:                  */
    tfreem(T_NODE, CHAIN, ph, pn);

    /* set the users pointer to NULL so they may no longer point to a valid */
    /* address anymore. Else they would still be using the node in the      */
    /* free list yet.                                                       */
    pl = NULL;			/* "fix" users pointer so they cannot dereference it no more */

    return (TRUE);
}
