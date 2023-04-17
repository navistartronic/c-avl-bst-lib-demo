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

t_header *find_header(char *);
static t_header *ph;

static char *RCSid[] = { "$Id$" };

extern int bst_errno;


/* bst_rprint: DO NOT USE; OLD CODE; INTERNAL KNOWLEDGE EXPOSED; USE FOR R&D ONLY */
/*             USERS SHOULD USE bst_print() INSTEAD */
void bst_rprint(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that graphically prints out the tree as it is traversed.
  *  Only line spacing and tabbing is done on output.  When the root node is 
  *  encountered a slash is placed in front to symbolize the root node.  Also 
  *  the tree prints with the right side of the tree at the top of screen and 
  *  works its way down the screen until the left edge of the tree is displayed.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to traverse and print.
  *
  *  Output Parameters
  *  =================
  *  None.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    int depth;

    void inorderprint(t_node *, int *, int);

    printf(">>> recursive printing <<<\n");
    bst_errno = BST_ERR_RESET;

    depth = -1;

    /* check if tree is even defined */

    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return;
    }
    /* now check if user has passed a printing function for this tree */

    if (ph->th_upf == NULL) {
	bst_errno = BST_ERR_NO_UPF_GIVEN;
	return;
    }
    if (ph->th_root == NULL)
	ph->th_upf(NULL, -1);
    else
	inorderprint(ph->th_root, &depth, ph->th_usiz);
}


/* inorderprint: traverses the tree inorder only it works on the right    */
void inorderprint(t_node * p, int *k, int th_usiz)
{
    /* side of the tree (top of screen) and works its way to    */
    /* the left (bottom of screen)                              */

    /* p is pointer to current node */
    /* k is the level or depth at previous node in above level */
    /* th_usiz is the size of the users data area  */

    int j;			/* j is the for-next var for indenting the node */
    t_node *pcopy;		/* pointer to a copy of the user node in tree */

    extern void *tallocm(MallocTypes mkind, ...);

    /* which is passed to the users print function */

    if (p != NULL) {
	(*k)++;			/* increment the level we're on */
	inorderprint(p->tn_rlink, k, th_usiz);	/* take right branch to leaf  */


	if ((pcopy = (t_node *) tallocm(T_NODE, ph)) == NULL) {
	    bst_errno = BST_ERR_MALLOC;
	    return;
	}
	memcpy(pcopy, p, sizeof(t_node) + th_usiz);
#ifdef DEBUG_MALLAC_USAGE
	printf(">>> memcpy FROM LOCATION 0x%-5x TO LOCATION 0x%-5x; %i BYTES <<<\n", p, pcopy, sizeof(t_node) + th_usiz);
#endif

	/* make call to user node print function */

	ph->th_upf(p + 1, *k);	/* pass node to user print function for printing */

	inorderprint(p->tn_llink, k, th_usiz);	/* take left branch now */
	(*k)--;			/* decrement the level counter now that we're popping & going up */
    }				/* if */
}
