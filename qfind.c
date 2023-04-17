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

static char *RCSid[] = { "$Id: qfind.c,v 2.2 1999/01/02 22:27:15 roger Exp $" };


/* qfind: search tree w/o setting any pointers for specified key */
Boolean qfind(t_header * ph, t_node * tnode)
{
 /*******************************************************************************
  *  An internal library function that does a quick search a bst tree to see if 
  *  the key contained in 'knode' is present in the tree and if so qfind returns 
  *  TRUE else FALSE. qfind does not set any pointers on it's search through the 
  *  tree as does fnode does.
  *
  *  Input Parameters
  *  =================
  *  ph         : Pointer to the header record for the tree.
  *  knode      : Node containing the key to find in the bst tree.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : A new tree was created.
  *  FALSE      : Tree is already defined or malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; *NOT* reset on entry.
  *******************************************************************************/

    t_node *pn;
    register int cmpresult;

    /* initialize */
    pn = ph->th_root;

    /* search ... */
    while (pn != NULL) {
	cmpresult = ph->th_ucf(tnode + 1, pn + 1);
	if (cmpresult < 0)
	    pn = pn->tn_llink;
	else if (cmpresult > 0)
	    pn = pn->tn_rlink;
	else
	    return (TRUE);
    }

    /* no luck ... */
    return (FALSE);
}
