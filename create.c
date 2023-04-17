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

static char *RCSid[] = { "$Id: create.c,v 2.2 1999/01/18 03:49:23 roger Exp $" };

extern t_header *t_head;
extern int bst_errno;


/* bst_create: create a new tree header record returning true or false */
Boolean bst_create(char *tname, BstType ttype, int leafsize, int fixedrec, int (*compf) (void *, void *),
		   void (*prntf) (void *, int), TreeVerifyType th_stat)
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

    t_header *p;

    extern t_header *find_header(char *tname);
    extern void *tallocm(MallocTypes mkind, ...);
    extern double tid(void);

    bst_errno = BST_ERR_RESET;

    /* Check if len of tree name is ok */
    if (strlen(tname) < MIN_TREE_NAME_LEN) {
	bst_errno = BST_ERR_NAME_LEN;
	return (FALSE);
    }
    /* Check if tree already defined */
    if ((find_header(tname) != TREE_NOT_DEFINED)) {
	bst_errno = BST_ERR_TREE_ALREADY_DEFINED;
	return (FALSE);		/* tree already defined */
    }

    /* Check for valid bst class: AVL or bst
       if (ttype != AVL || ttype != BST) {  
       bst_errno = BST_ERR_UKNOWN_BST_TYPE;
       return (FALSE);
       }

       /* check for valid leaf size being passed */
    if (leafsize <= 0) {
	bst_errno = BST_ERR_LEAFNODE_SIZE_ZERO;
	return (FALSE);
    }

    /* check if a user written compare function is passed */
    if (compf == NULL) {
	bst_errno = BST_ERR_NO_UCF_GIVEN;
	return (FALSE);
    }

    /* check if user written compare function is same as the user written */
    /* print function (user written print function is optional)           */
    if ((long) compf == (long) prntf) {
	bst_errno = BST_ERR_UCF_EQUALS_UPF;
	return (FALSE);
    }

    /* ok,  so create a new tree and check for malloc error */
    if ((p = (t_header *) tallocm(T_HEADER, sizeof(t_header))) == NULL) {
	bst_errno = BST_ERR_MALLOC;
	return (FALSE);
    }

    /* Initialize the new tree node header */
    strcpy(p->th_name, tname);
    p->th_bsttype = ttype;
    p->th_root = EMPTY_TREE;
    p->th_flist = EMPTY_LIST;
    p->th_id = tid();		/*  get unique id for this tree */
    p->th_flcnt = 0;
    p->th_stat = (ttype == AVL && th_stat == TREE_VERIFY_YES) ? TRUE : FALSE;
    p->th_usiz = leafsize;
    p->th_ucf = (int (*)(void *, void *)) compf;	/* user compare two nodes function(Leaf1,Leaf2) */
    p->th_upf = (void (*)(void *, int)) prntf;	/* user print function given a node Leaf */
    p->th_ncnt = 0;
    p->th_np = fixedrec;
    strcpy(p->th_version_id, VERSION_ID);
    p->th_reserved1 = 0;
    p->th_reserved2 = 0;

#ifdef DEBUG_TRACE
    if (p->th_stat == TRUE)
	printf("********** AVL TREE BALANCE VERIFICATION IS IN EFFECT **********\n");
#endif

    /* Insert new tree header record into linked list of defined AVL trees: */
    p->th_link = t_head;
    t_head = p;

    return (TRUE);
}
