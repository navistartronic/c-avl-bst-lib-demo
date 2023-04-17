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

static char *RCSid[] = { "$Id: tdispose.c,v 2.1 1999/01/02 17:06:57 roger Exp $" };

extern t_header *t_head;


/* tdispose: delete a tree and all its nodes */
void tdispose(t_header * ph)
{
 /*******************************************************************************
  *  A private library function that deletes an entire tree and all its nodes.
  *
  *  Input Parameters
  *  =================
  *  ph         : Pointer to the tree header record to delete and remove
  *
  *  Output Parameters
  *  =================
  *  None.
  *
  *  Global Variables
  *  =================
  *  None.
  *******************************************************************************/

    t_header *p, *q;
    t_node *pn, *qn;
    Boolean found;

    extern void tfreem(MallocTypes mkind, ...);
    extern Boolean twalk(TWalkOps op, Traversals order, ...);


    /* Traverse the tree freeing all nodes: */
    twalk(DELETE, POSTORDER, ph->th_root);

    //printf("tdispose: tree nodes freed\n");
    /* Traverse the list of free nodes in the header, freeing all nodes: */
    for (pn = ph->th_flist; pn != EMPTY_LIST;) {
	qn = pn;
	pn = pn->tn_ulink;
	tfreem(T_NODE, FREE, qn);
    }

    //printf("tdispose: free list in header freed\n");
    /* Find the header record position in the list of defined trees: */
    for (p = t_head, q = NULL, found = FALSE; p != NULL && !found;)
	if (strcmp(p->th_name, ph->th_name) == IDENTICAL)
	    found = TRUE;
	else {
	    q = p;
	    p = p->th_link;
	}

    /* Unlink the tree record from the list of defined trees: */
    if (q == NULL)
	t_head = p->th_link;
    else
	q->th_link = p->th_link;

    /* Free the tree name and then the header record itself: */
    /*      free(ph->th_name);  */
    tfreem(T_HEADER, p);
    //printf("tdispose: header record freed\n");
}
