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

typedef enum { OFF, ON } BalancingSwitch;

static char *RCSid[] = { "$Id: remove.c,v 2.2 1999/01/19 03:04:48 roger Exp $" };

extern int bst_errno;


/* bst_remove: non-recursive delete a node from the tree */
Boolean bst_remove(char *tname, void *pl)
{
 /*******************************************************************************
  *  A private library function that removes a tree_node from the tree.
  *  Algorithim is adopted from N. Wirth's "Algorithims & Data Structures", 
  *  Englewood Cliffs, p. 218-227 with the recursion removed from the algorithm.  
  *
  *  Input Parameters
  *  =================
  *  tname      : Name of the tree to delete node from.
  *  pl         : Pointer to the users structure that contains the key to remove
  *               from the tree.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Node was deleted from tree.
  *  FALSE      : Tree not defined, tre/node mismatch, malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_header *ph;		/* pointer to the tree header record */
    t_node *dp;			/* move through the tree with */
    t_node *p;			/* move through the tree with */
    t_node *pn;			/* pointer the header part of users leaf node */
    t_node **q;			/* hold the previous node */
    t_node *r;			/* pointer to the node to be deleted when found so that the leaf node in its left subtree, rightmost node can be copied to here at r. */
    int tside;			/* value of the node LEFT_SON or RIGHT_SON */
    int cmpresult;		/* Integer result from the user written compare funtion to determe the key ordering. (neg. <.,0 =, pos. >) */
    Boolean found;
    BalancingSwitch rbalsw;

    t_header *find_header(char *);
    void balancer(t_node **, t_node **, BalancingSwitch *);
    void balancel(t_node **, t_node **, BalancingSwitch *);
    extern void bst_stat(char *);
    extern void tfreem(MallocTypes mkind, ...);

    bst_errno = BST_ERR_RESET;

    /* check if tree is even defined */
    if ((ph = find_header(tname)) == TREE_NOT_DEFINED) {
	bst_errno = BST_ERR_TREE_NOT_DEFINED;
	return (FALSE);
    }

    /* check if node passed belongs to this tree */
    pn = ((t_node *) pl - 1);	/* pn is cast from user type to type t_node */

    if (ph->th_id != pn->tn_id) {
	bst_errno = BST_ERR_TREE_NODE_MISMATCH;
	return (FALSE);
    }

    /* ok, on with initialization */
    found = FALSE;

    p = ph->th_root;		/* p and q both initially point to the address of the  */
    q = &ph->th_root;		/* location that contains the pointer to the tree root */

    while (p != NULL && !found) {	/* trace down through tree searching */
	cmpresult = ph->th_ucf(pl, p + 1);	/* make the key comparision call     */

	if (cmpresult < 0) {	/* take left branch */
	    q = &p->tn_llink;	/* q is the address of the structure   */
	    p = p->tn_llink;	/* member 'tn_llink' of the current */
	} else /* node before moving to next node   */ if (cmpresult > 0) {	/* greater than, take right branch */
	    q = &p->tn_rlink;
	    p = p->tn_rlink;
	} else {		/* found the desired node */
	    found = TRUE;
	    rbalsw = ON;

	    if (p->tn_llink == NULL && p->tn_rlink == NULL)
		*q = NULL;
	    else if (p->tn_llink == NULL) {
		p->tn_rlink->tn_ulink = p->tn_ulink;
		p->tn_rlink->tn_tag = p->tn_tag;
		*q = p->tn_rlink;	/* link & bypass node p        */
	    } else if (p->tn_rlink == NULL) {
		p->tn_llink->tn_ulink = p->tn_ulink;
		p->tn_llink->tn_tag = p->tn_tag;
		*q = p->tn_llink;	/* link & bypass node p    */
	    } else {
		r = p;		/* r points to node to delete before moving on */
		q = &p->tn_llink;	/* move to the left subtree now      */
		p = p->tn_llink;

		/* now take the rightmost branch every time untill the */
		/* end of the tree is encountered. p will point to the */
		/* last leaf on this path, and it may or may not have  */
		/* a left subtree itself.                              */

		while (p->tn_rlink != NULL) {
		    q = &p->tn_rlink;
		    p = p->tn_rlink;
		}

		/* now move the data contents of the leaf to the node */
		/* position pointed to by r and then unlink the leaf  */
		/* node pointed to by p:                              */
		/* note: ONLY the data is copied, not the header part */
		/* of the leaf involving the pointers, bf's etc */

		memcpy(r + 1, p + 1, ph->th_usiz);
#ifdef DEBUG_MALLAC_USAGE
		printf(">>> memcpy FROM LOCATION 0x%-5x TO LOCATION 0x%-5x; %i BYTES <<<\n", p + 1, r + 1, ph->th_usiz);
#endif
		if (p->tn_llink != NULL) {
		    p->tn_llink->tn_ulink = p->tn_ulink;
		    if (r->tn_llink != p)
			p->tn_llink->tn_tag = RIGHT_SON;
		}
		*q = p->tn_llink;
	    }

	}
    }				/* while not found */

    if (!found) {
	bst_errno = BST_ERR_KEY_NOT_FOUND;
	return (FALSE);
    }

    tside = p->tn_tag;
    dp = p;
    for (p = dp->tn_ulink; p != NULL; p = p->tn_ulink) {
	if (ph->th_bsttype == AVL) {
	    switch (rbalsw) {
	    case ON:
		switch (tside) {
		case LEFT_SON:
		    balancel(&ph->th_root, &p, &rbalsw);
		    break;
		case RIGHT_SON:
		    balancer(&ph->th_root, &p, &rbalsw);
		    break;
		}
		break;
	    case OFF:
		break;
	    }
	}
	tside = p->tn_tag;
    }

    /* add to the free list in the AVL header record for this tree */
    tfreem(T_NODE, CHAIN, ph, dp);
    ph->th_ncnt--;

    if (ph->th_stat)
	bst_stat(tname);
    return (TRUE);
}

/* balancel; perform a left rotation */
void balancel(t_node ** root, t_node ** p, BalancingSwitch * bsw)
{
    t_node *p1, *p2;

#ifdef DEBUG_SHOWREBALANCE
    printf(">>> balancel(): ROOT (0x%-5x) DELETE BL: case %2i", *p, (*p)->tn_bf);
#endif
    switch ((*p)->tn_bf) {
    case +1:
	(*p)->tn_bf = 0;
	break;
    case 0:
	(*p)->tn_bf = -1;
	*bsw = OFF;
	break;
    case -1:
	p1 = (*p)->tn_rlink;
#ifdef DEBUG_SHOWREBALANCE
	printf(":%2i ", p1->tn_bf);
#endif
	if (p1->tn_bf <= 0) {
	    /* single RR */
#ifdef DEBUG_SHOWREBALANCE
	    printf(" (RR)");
#endif

	    /* LINKS */
	    (*p)->tn_rlink = p1->tn_llink;
	    p1->tn_llink = *p;

	    /* UP LINKS */
	    p1->tn_ulink = (*p)->tn_ulink;
	    (*p)->tn_ulink = p1;
	    if ((*p)->tn_rlink != NULL)
		(*p)->tn_rlink->tn_ulink = *p;

	    /* TAGS */
	    p1->tn_tag = (*p)->tn_tag;
	    (*p)->tn_tag = LEFT_SON;
	    if ((*p)->tn_rlink != NULL)
		(*p)->tn_rlink->tn_tag = RIGHT_SON;

	    /* BALANCE FACTORS */
	    if (p1->tn_bf == 0) {
		(*p)->tn_bf = -1;
		p1->tn_bf = +1;
		*bsw = OFF;
	    } else {
		(*p)->tn_bf = 0;
		p1->tn_bf = 0;
	    }

	    /* RE-ASSIGN POINTER TO REFLECT ROTATION */
	    if (*p == *root)
		*root = p1;
	    else {
		switch (p1->tn_tag) {
		case LEFT_SON:
		    p1->tn_ulink->tn_llink = p1;
		    break;
		case RIGHT_SON:
		    p1->tn_ulink->tn_rlink = p1;
		    break;
		}
	    }
	    *p = p1;
	} else {
	    /* double RL */
#ifdef DEBUG_SHOWREBALANCE
	    printf(" (RL)");
#endif
	    /* LINKS */
	    p2 = p1->tn_llink;
	    p1->tn_llink = p2->tn_rlink;
	    p2->tn_rlink = p1;
	    (*p)->tn_rlink = p2->tn_llink;
	    p2->tn_llink = *p;

	    /* UP LINKS */
	    p2->tn_ulink = (*p)->tn_ulink;
	    (*p)->tn_ulink = p2;
	    p1->tn_ulink = p2;
	    if ((*p)->tn_rlink != NULL)
		(*p)->tn_rlink->tn_ulink = *p;
	    if (p1->tn_llink != NULL)
		p1->tn_llink->tn_ulink = p1;

	    /* TAGS */
	    p2->tn_tag = (*p)->tn_tag;
	    (*p)->tn_tag = LEFT_SON;
	    if ((*p)->tn_rlink != NULL)
		(*p)->tn_rlink->tn_tag = RIGHT_SON;
	    if (p1->tn_llink != NULL)
		p1->tn_llink->tn_tag = LEFT_SON;

	    /* BALANCE FACTORS */
	    if (p2->tn_bf == -1)
		(*p)->tn_bf = +1;
	    else
		(*p)->tn_bf = 0;
	    if (p2->tn_bf == +1)
		p1->tn_bf = -1;
	    else
		p1->tn_bf = 0;
	    p2->tn_bf = 0;

	    /* RE-ASSIGN POINTER TO REFLECT ROTATION */
	    if (*p == *root)
		*root = p2;
	    else
		switch (p2->tn_tag) {
		case LEFT_SON:
		    p2->tn_ulink->tn_llink = p2;
		    break;
		case RIGHT_SON:
		    p2->tn_ulink->tn_rlink = p2;
		    break;
		}
	    *p = p2;
	}
	break;
    }				/* switch */
#ifdef DEBUG_SHOWREBALANCE
    printf(" <<<\n");
#endif
}				/* balancel */

/* balancer: perform a right rotation */
void balancer(t_node ** root, t_node ** p, BalancingSwitch * bsw)
{				/* balancer */
    t_node *p1, *p2;

#ifdef DEBUG_SHOWREBALANCE
    printf("balancer(): >>> ROOT (0x%-5x) DELETE BR: case %2i", *p, (*p)->tn_bf);
#endif
    switch ((*p)->tn_bf) {
    case -1:
	(*p)->tn_bf = 0;
	break;
    case 0:
	(*p)->tn_bf = +1;
	*bsw = OFF;
	break;
    case +1:
	p1 = (*p)->tn_llink;
#ifdef DEBUG_SHOWREBALANCE
	printf(":%2i ", p1->tn_bf);
#endif
	if (p1->tn_bf >= 0) {
#ifdef DEBUG_SHOWREBALANCE
	    printf(" (LL)");
#endif
	    /* single LL */

	    /* LINKS */
	    (*p)->tn_llink = p1->tn_rlink;
	    p1->tn_rlink = *p;

	    /* UP LINKS */
	    p1->tn_ulink = (*p)->tn_ulink;
	    (*p)->tn_ulink = p1;
	    if ((*p)->tn_llink != NULL)
		(*p)->tn_llink->tn_ulink = *p;

	    /* TAGS */
	    p1->tn_tag = (*p)->tn_tag;
	    (*p)->tn_tag = RIGHT_SON;
	    if ((*p)->tn_llink != NULL)
		(*p)->tn_llink->tn_tag = LEFT_SON;

	    /* BALANCE FACTORS */
	    if (p1->tn_bf == 0) {
		(*p)->tn_bf = +1;
		p1->tn_bf = -1;
		*bsw = OFF;
	    } else {
		(*p)->tn_bf = 0;
		p1->tn_bf = 0;
	    }

	    /* RE-ASSIGN POINTER TO REFLECT ROTATION */
	    if (*p == *root)
		*root = p1;
	    else
		switch (p1->tn_tag) {
		case LEFT_SON:
		    p1->tn_ulink->tn_llink = p1;
		    break;
		case RIGHT_SON:
		    p1->tn_ulink->tn_rlink = p1;
		    break;
		}
	    *p = p1;
	} else {
	    /* double LR */
#ifdef DEBUG_SHOWREBALANCE
	    printf(" (LR)");
#endif
	    /* LINKS */
	    p2 = p1->tn_rlink;
	    p1->tn_rlink = p2->tn_llink;
	    p2->tn_llink = p1;
	    (*p)->tn_llink = p2->tn_rlink;
	    p2->tn_rlink = *p;

	    /* UP LINKS */
	    p2->tn_ulink = (*p)->tn_ulink;
	    (*p)->tn_ulink = p2;
	    p1->tn_ulink = p2;
	    if ((*p)->tn_llink != NULL)
		(*p)->tn_llink->tn_ulink = *p;
	    if (p1->tn_rlink != NULL)
		p1->tn_rlink->tn_ulink = p1;

	    /* TAGS */
	    p2->tn_tag = (*p)->tn_tag;
	    (*p)->tn_tag = RIGHT_SON;
	    if ((*p)->tn_llink != NULL)
		(*p)->tn_llink->tn_tag = LEFT_SON;
	    if (p1->tn_rlink != NULL)
		p1->tn_rlink->tn_tag = RIGHT_SON;

	    /* BALANCE FACTORS */
	    if (p2->tn_bf == +1)
		(*p)->tn_bf = -1;
	    else
		(*p)->tn_bf = 0;

	    if (p2->tn_bf == -1)
		p1->tn_bf = +1;
	    else
		p1->tn_bf = 0;
	    p2->tn_bf = 0;

	    /* RE-ASSIGN POINTER TO REFLECT ROTATION */
	    if (*p == *root)
		*root = p2;
	    else
		switch (p2->tn_tag) {
		case LEFT_SON:
		    p2->tn_ulink->tn_llink = p2;
		    break;
		case RIGHT_SON:
		    p2->tn_ulink->tn_rlink = p2;
		    break;
		}
	    *p = p2;
	}
	break;
    }				/* switch */
#ifdef DEBUG_SHOWREBALANCE
    printf(" <<<\n");
#endif
}
