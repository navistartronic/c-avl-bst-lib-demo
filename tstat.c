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

#define DEB_FULL   1
#define DBG_VERIFY 1

static int ncount;		/* local global to this module only */
static int maxdepth;		/* local global to this module only */

static char *RCSid[] = { "$Id$" };

extern int bst_errno;


/* bst_stat: display tree header characteristics */
/* bst_stat: DO NOT USE; OLD CODE; INTERNAL KNOWLEDGE EXPOSED; USE FOR R&D ONLY */
/*           USERS SHOULD USE bst_print() INSTEAD */

void bst_stat(char *tname)
{
 /*******************************************************************************
  *  A user acccessible function that verifies that the tree is still in a balanced 
  *  state. If an error *should* occur, the global error variable *bst_errno* is 
  *  set to the proper number. Nothing else is printed or sent back to the caller.
  *
  *  bst_stat can also be called by AVLPUT and bst_remove if the  flag in the 
  *  header record is set when bst_create was invoked to th_stat the condition 
  *  after *every* call to bst_put and bst_remove.   
  *
  *  Input Parameters
  *  =================
  *  tname       : Name of the tree to check.
  *
  *  Output Parameters
  *  =================
  *  bst_errno  : Global error variable is either the RESET value (all is ok)
  *               else it contains the error number of what's wrong.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;

    void checkbalance(t_node * p);

    extern t_header *find_header(char *);
    extern void bst_print(char *);

    bst_errno = BST_ERR_RESET;

    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return;
    }

    if (ph->th_bsttype == BST) {
	printf("checking status of tree: nothing to check, tree is type BST, not AVL: OK\n");
	return;

    }
    printf("********** VERIFYING **********");

    ncount = 0;
    checkbalance(ph->th_root);

    if (bst_errno == 0)
	printf("...........................................OK\n");
    else
	printf("\n\n\007\007...................................*** ERROR %3i! ***\n\n", bst_errno);

    if (ph->th_ncnt != ncount)
	printf("\007.................. node miscount: ph->th_ncnt %i  run time count %i\n", ph->th_ncnt, ncount);
}				/* bst_stat */


/* checkbalance: does the actual work */
void checkbalance(t_node * p)
{
 /*******************************************************************************
  *  A user acccessible function that creates a new AVL or BST search tree.
  *  Creates a new tree header node, initializes it and adds it to the global
  *  link list pointed to by t_head.
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the new tree to define.
  *  ttype      : Type of bst to use: AVL or BST.
  *  leafsize   : sizeof(Leaf) of the _user's_ data record.
  *  fixedrec   : TRUE if users Leaf data record contains no pointers.
  *               FALSE if users Leaf data record contains pointers.
  *               (2018 rpj: unknown what the idea of this was)
  *  compf      : Pointer to the user written compare function needed to
  *               compare any two keys for this particular tree.
  *  prntf      : Pointer to user written print function that will print
  *               the users key (and any other info)  for that node.
  *               No big deal if user passed NULL to it.
  *  th_stat    : After each bst_put() or bst_remove() should we check (i.e.
  *               verify) the tree balanced state? Meaninful for only AVL trees.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : A new tree was created.
  *  FALSE      : Tree is already defined or malloc error.
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    int depth(t_node *);

    /* checkbalance  -- compute ACTUAL balance factors of the tree. At each   */
    /* node in the tree decend in it's left an right subtree */
    /* and find the actual height of each subtree.           */
    int bf;

    if ((p == NULL) || (bst_errno == BST_ERR_OUT_OF_BALANCE))
	return;

    ncount++;
    if (p->tn_llink != NULL) {
	if (p->tn_llink->tn_tag != LEFT_SON) {
	    printf("p->left->tn_tag is incorrect! node (0x%-5x)\n", p->tn_llink);
	    bst_errno = BST_ERR_TAG;
	}
	if (p->tn_llink->tn_ulink != p) {
	    printf("p->left->tn_ulink is astray! node (0x%-5x)\n", p->tn_llink);
	    bst_errno = BST_ERR_TAG;
	}
    }
    checkbalance(p->tn_llink);

    bf = depth(p->tn_llink) - depth(p->tn_rlink);

    if (p->tn_bf != bf) {
	printf("tree is out of balance! node (0x%-5x)\n", p);
	bst_errno = BST_ERR_OUT_OF_BALANCE;
	return;
    }
    if (p->tn_rlink != NULL) {
	if (p->tn_rlink->tn_tag != RIGHT_SON) {
	    printf("p->right->tn_tag is incorrect! node (0x%-5x)\n", p->tn_rlink);
	    bst_errno = BST_ERR_TAG;
	}
	if (p->tn_rlink->tn_ulink != p) {
	    printf("p->right->tn_ulink is astray! node (0x%-5x)\n", p->tn_rlink);
	    bst_errno = BST_ERR_TAG;
	}
    }
    checkbalance(p->tn_rlink);

}				/* checkbalance */


/* depth: compute the depth or height current tree where we are currently at. */
int depth(t_node * p1)
{
 /*******************************************************************************
  *  A private local function that compute the depth or height current tree now.
  *  Now we're in either the node above left or right subtree. Find actual
  *  depth with searchall.
  *
  *  From our current position in the tree, we call searchall() 
  *
  *  Input Parameters
  *  =================
  *  p1         : Current tree node we are at in the tree.
  *
  *  Output Parameters
  *  =================
  *  Function name returns int result: the longest path encountered
  *
  *  Global Variables
  *  =================
  *  maxdepth   :  maxdepth was the longest path encountered
  *******************************************************************************/

    int d;

    void searchall(t_node *, int *);

    /* initialize */

    d = 0;
    maxdepth = d;		/* maxdepth is global */

    searchall(p1, &d);		/* search all possible paths to the leafs */

    return maxdepth;		/* maxdepth was the longest path encountered */
}				/* depth */

/* searchall():  take all possible paths in this tree to the leaves and keep track of the maximum height encountered in this tree */
void searchall(t_node * p2, int *d1)
{
 /*******************************************************************************
  *  A private local function that take all possible paths in this tree to the leaves and
  *  keep track of the maximum height encountered in this tree.
  *
  *  Input Parameters
  *  =================
  *  p2         : Current tree node we are at.
  *  d1         : current depth of tree.
  *
  *  Output Parameters
  *  =================
  *  d1         : depth of tree encountered in this call
  *
  *  Global Variables
  *  =================
  *  maxdepth   : Current max depth encountered in tree from the node being visited
  *******************************************************************************/

    /* searchall -- take all possible paths in this tree to the leaves and    */
    /* keep track of the maximum height encountered in this tree */

    if (p2 == NULL)		/* end of the trail for this path. */
	return;

    (*d1)++;			/* increment depth var one more level that we're at    */

    if (*d1 > maxdepth)		/* retain the depth level if this is the longest path  */
	maxdepth = *d1;		/* encountered thus far.                            */

    searchall(p2->tn_llink, d1);	/* take the left branch... */
    searchall(p2->tn_rlink, d1);	/* now take the right branch... */

    (*d1)--;			/* decrease the level we're on as popping back upwards */
}				/* searchall */
