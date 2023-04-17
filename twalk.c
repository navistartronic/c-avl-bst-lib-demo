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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifndef BST_HDR
#include "bst.h"
#endif

#define BIT_FALSE (unsigned int) 0
#define BIT_TRUE  (unsigned int) 1

typedef enum { OK, ERROR, NOT_IDENT, NOT_EQUAL } operation_status;

static struct traversals {
    unsigned int inorder:1;
    unsigned int preorder:1;
    unsigned int postorder:1;
};

static char *RCSid[] = { "$Id:twalk.c,v 2.2 1999/01/02 22:27:55 roger Exp $" };

extern int bst_errno;
extern t_header *t_head;
extern double tid(void);


/* twalk: walk the tree in the specified order calling the user defined function at each node */
Boolean twalk(TWalkOps op, Traversals order, ...)
{
 /*******************************************************************************
  *  A private library function that walks the tree in the specfied order calling
  *  the user defined function for action on the current node.
  *  
  *   USAGE:
  *          twalk(VISIT, INORDER,   ph->th_root, Print_Node);
  *          twalk(VISIT, PREORDER,  ph->th_root, Print_Node);
  *          twalk(VISIT, POSTORDER, ph->th_root, Print_Node)
  *
  *          twalk(DELETE, POSTORDER, ph->th_root);
  *          twalk(COPY, COPYORDER, ph, ntn);
  *          twalk(IDENT, PREORDER, ph1, ph2);
  *          twalk(EQUAL, PREORDER, ph1, ph2);
  *
  *  Input Parameters
  *  =================
  *  ops        : What action we are going to perform on this node.
  *  order      : What type of tree walk we are going to perform.
  *
  *  Output Parameters
  *  =================
  *  Function name returns Boolean result:
  *  TRUE       : Tree walk performed. 
  *  FALSE      : Error, other, malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    struct {
	unsigned int done:1;
	unsigned int try_going_left:1;
	unsigned int time_to_go_right:1;
	unsigned int end_of_left_branch:1;
    } bitflags;

    va_list ap;
    int ttype, depth, cnt;
    char *ntname;
    t_header *ph, *ph_dup;
    t_node *p, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p_dup, *pp_dup, *dp;
    struct traversals traversal;

    t_header *cp_header(t_header *, char *);

    void setflags(Traversals, struct traversals *);
    operation_status action(TWalkOps op, void (*compf) (void *, int), int depth, t_node * p, t_header * ph_dup, t_node * p_dup,
			    t_node ** pp_dup, t_node ** dp, int *count);

    extern void tdispose(t_header *);
    extern void tfreem(MallocTypes mkind, ...);

    /* initialization: */
    ttype = 0;
    depth = 0;
    cnt = 0;
    ntname = NULL;
    ph = NULL;
    ph_dup = NULL;
    p = NULL;
    p1 = NULL;
    p2 = NULL;
    p3 = NULL;
    p4 = NULL;
    p5 = NULL;
    p6 = NULL;
    p7 = NULL;
    p_dup = NULL;
    pp_dup = NULL;
    dp = NULL;
    bitflags.done = BIT_FALSE;
    setflags(order, &traversal);


    /* special handling: we are passing in a point to a function in the va_arg list, we need to typedef it for ease of parsing: */
    /* technically it is "Leaf" but we don't have access to what that is in here: typedef void (*PFUNC)(Leaf *pl, int level); */
    /* reference: https://cboard.cprogramming.com/c-programming/151134-using-function-pointers-when-argument-list-functions-arent-known-beforehand.html */
    typedef void (*PFUNC) (void *pl, int level);
    PFUNC p_uvf;
    p_uvf = NULL;

    /* arguments processing and initialization: */
    /* NOTE: had to fix the next line for FreeBSD */
    va_start(ap, order);	/* ap points to 3rd arg now */

    switch (op) {
    case VISIT:
    case DELETE:
	/* root tree node */
	p = (t_node *) va_arg(ap, t_node *);

	if (op == VISIT) {
	    /* printf("op = VISIT\n\n"); */
	    /* next arg is the user definded function to do something with this leaf node */
	    p_uvf = va_arg(ap, PFUNC);

	    /* if there is no function to perform on the leaf, what's the point then? */
	    if (p_uvf == NULL) {
		bst_errno = BST_ERR_NO_UPF_GIVEN;
		va_end(ap);
		return (FALSE);
	    }
	}
	break;
    case COPY:
	/* printf("op = COPY\n\n"); */
	ph = (t_header *) va_arg(ap, t_header *);
	ntname = (char *) va_arg(ap, char *);
	p = ph->th_root;
	pp_dup = NULL;
	if ((ph_dup = (t_header *) cp_header(ph, ntname)) == NULL) {
	    va_end(ap);
	    return (FALSE);
	}
	/* printf(">>> tree header duplicated <<<\n"); */
	break;
    case IDENT:
    case EQUAL:
	if (op == IDENT) {
	    /* printf("op = IDENT\n\n"); */
	} else {
	    /* printf("op = EQUAL\n\n"); */
	}
	ph = (t_header *) va_arg(ap, t_header *);
	p = ph->th_root;
	ph_dup = (t_header *) va_arg(ap, t_header *);
	p_dup = ph_dup->th_root;
	if (p == NULL && p_dup != NULL || p != NULL && p_dup == NULL) {
	    va_end(ap);
	    return (FALSE);
	}
	if (p == NULL && p_dup == NULL) {
	    va_end(ap);
	    /* printf("twalk: EXIT @ p or p_dup NULL\n"); */
	    return (TRUE);
	}
	/* printf("each root is non-null\n"); */
	break;
    }
    va_end(ap);

    if (p == NULL) {
	/* printf("Empty tree; depth = %i\n", depth); */
	/* printf("twalk: EXIT @ p == ROOT == NULL\n"); */
	return (TRUE);
    }

    /* traverse the tree in specified order */
    while (!bitflags.done) {
	bitflags.end_of_left_branch = BIT_FALSE;
	while (!bitflags.end_of_left_branch) {
	    if (traversal.preorder) {
		switch (action(op, *p_uvf, depth, p, ph_dup, p_dup, &pp_dup, &dp, &cnt)) {
		case NOT_EQUAL:
		    /* printf("twalk: EXIT @ loop: not equal\n"); */
		    return (FALSE);
		    break;
		case NOT_IDENT:
		    /* printf("twalk: EXIT @ loop: not identical\n"); */
		    return (FALSE);
		    break;
		case ERROR:
		    tdispose(ph_dup);
		    /* printf("twalk: EXIT @ loop: ERROR\n"); */
		    return (FALSE);
		    break;
		case OK:
		    break;
		}
	    }
	    if (p->tn_llink != NULL) {
		p = p->tn_llink;
		if (op == IDENT)
		    if (p_dup->tn_llink != NULL)
			p_dup = p_dup->tn_llink;
		    else {
			/* printf("twalk: EXIT @ IDENT: LEFT !NULL & LEFT NULL\n"); */
			return (FALSE);
		    }
		depth++;
	    } else {
		bitflags.end_of_left_branch = BIT_TRUE;
		if (op == IDENT && p_dup->tn_llink != NULL) {
		    /* printf("twalk: EXIT @ IDENT: LEFT NULL & RIGHT !NULL\n"); */
		    return (FALSE);
		}
	    }
	}
	bitflags.try_going_left = BIT_FALSE;
	while (!bitflags.try_going_left && !bitflags.done) {
	    if (traversal.inorder)
		action(op, *p_uvf, depth, p, ph_dup, p_dup, &pp_dup, &dp, &cnt);
	    if (op == IDENT)
		if (p->tn_rlink == NULL && p_dup->tn_rlink != NULL || p->tn_rlink != NULL && p_dup->tn_rlink == NULL) {
		    /* printf("twalk: EXIT @ IDENT: 4-way *if* test\n"); */
		    return (FALSE);
		}
	    if (p->tn_rlink != NULL) {
		p = p->tn_rlink;
		if (op == IDENT)
		    p_dup = p_dup->tn_rlink;
		depth++;
		bitflags.try_going_left = BIT_TRUE;
	    }
	    if (!bitflags.try_going_left) {
		bitflags.time_to_go_right = BIT_FALSE;
		while (!bitflags.done && !bitflags.time_to_go_right) {
		    if (traversal.postorder)
			action(op, *p_uvf, depth, p, ph_dup, p_dup, &pp_dup, &dp, &cnt);
		    if (!bitflags.done) {
			depth--;
			if (p->tn_tag == LEFT_SON) {
			    bitflags.time_to_go_right = BIT_TRUE;
			    bitflags.try_going_left = BIT_FALSE;
			} else if (p->tn_ulink == NULL)
			    bitflags.done = BIT_TRUE;
/* need to clean this up. if done, then break, and clean up at end */
			p = p->tn_ulink;
			if (op == IDENT)
			    if (p_dup->tn_ulink != NULL)
				p_dup = p_dup->tn_ulink;
			    else if (!bitflags.done)
				return (FALSE);
			if (op == COPY && pp_dup != NULL)
			    pp_dup = pp_dup->tn_ulink;

			if (op == DELETE && dp != NULL) {
			    tfreem(T_NODE, FREE, dp);
			    dp = NULL;
			}
		    }
		}
	    }
	}
    }

    if ((op == COPY) && (cnt != ph->th_ncnt)) {
	bst_errno = BST_ERR_COPY_CNT;
	tdispose(ph_dup);
	/* printf("twalk EXIT: (COPY) @ end\n"); */
	return (FALSE);
    }
    if (op == IDENT || op == EQUAL) {
	/* printf("twalk EXIT: (IDENT/EQUAL) @ end\n"); */
	return (TRUE);
    }
    /* printf("twalk EXIT: @ end of function\n"); */
}

/* setflags: initialize the traversal structure */
void setflags(Traversals order, struct traversals *traversal)
{
 /*******************************************************************************
  *  A private local function that initializes the traversal structure
  *
  *  Input Parameters
  *  =================
  *  order      : Treewalk type
  *  traversals : structure holding traversal types
  *
  *  Output Parameters
  *  =================
  *  traversals : structure holding traversal types
  *
  *  Global Variables
  *  =================
  *  None.
  *******************************************************************************/

    switch (order) {
    case INORDER:
	(*traversal).inorder = 1;
	(*traversal).preorder = 0;
	(*traversal).postorder = 0;
	/* printf(">>> INORDER <<<\n\n"); */
	break;
    case PREORDER:
	(*traversal).inorder = 0;
	(*traversal).preorder = 1;
	(*traversal).postorder = 0;
	/* printf(">>> PREOREDER <<<\n\n"); */
	break;
    case POSTORDER:
	(*traversal).inorder = 0;
	(*traversal).preorder = 0;
	(*traversal).postorder = 1;
	/* printf(">>> POSTORDER <<<\n\n"); */
	break;
    }
}

/* cp_header: make a copy of the tree header */
t_header *cp_header(t_header * ph, char *ntn)
{
 /*******************************************************************************
  *  A private local function that copies the current tree header.
  *
  *  Input Parameters
  *  =================
  *  ph         : Pointer to source tree header record.
  *  ntn        : New Tree Name - name of the new tree.
  *
  *  Output Parameters
  *  =================
  *  Function name returns new tree header record:
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *******************************************************************************/

    t_header *ph_dup;

    extern void *tallocm(MallocTypes mkind, ...);
    extern char *strcpy(char *, const char *);

    if ((ph_dup = (t_header *) tallocm(T_HEADER, sizeof(t_header))) == NULL)
	return (NULL);

    strcpy(ph_dup->th_name, ntn);

    ph_dup->th_root = NULL;
    ph_dup->th_flist = NULL;
    ph_dup->th_id = tid();
    ph_dup->th_ncnt = ph->th_ncnt;
    ph_dup->th_usiz = ph->th_usiz;
    ph_dup->th_ucf = ph->th_ucf;
    ph_dup->th_upf = ph->th_upf;
    ph_dup->th_flcnt = 0;
    ph_dup->th_np = ph->th_np;
    ph_dup->th_stat = ph->th_stat;
    strcpy(ph_dup->th_version_id, ph->th_version_id);
    ph_dup->th_reserved1 = ph->th_reserved1;
    ph_dup->th_reserved2 = ph->th_reserved2;
    ph_dup->th_link = t_head;

    t_head = ph_dup;
    return (ph_dup);
}

/* operation_status_action: at each node we visit, perform this operation */
operation_status action(TWalkOps op, void (*compf) (), int depth, t_node * p, t_header * ph_dup, t_node * p_dup, t_node ** pp_dup,
			t_node ** dp, int *count)
{
 /*******************************************************************************
  *  A private library function that performs the action on the node visited.
  *
  *  Input Parameters
  *  =================
  *  op         : Operation to peform on this node.
  *  compf      : User function to call on this node.
  *  depth      : Depth of this node in the tree.
  *  p          : Pointer to the node being visited.
  *  ph_dup     : Pointer to the user written compare function needed to
  *               compare any two keys for this particular tree.
  *
  *  Output Parameters
  *  =================
  *  pp_dup     : Pointer to 
  *  dp         : Pointer to 
  *  count      : Pointer to 
  *
  *  Global Variables
  *  =================
  *  t_head     : A linked list of defined AVL trees.
  *  bst_errno  : Global error variable; reset on entry.
  *******************************************************************************/

    t_node *pcopy;

    Boolean qfind(t_header *, t_node *);
    extern void *tallocm(MallocTypes mkind, ...);
    extern void tfreem(MallocTypes mkind, ...);

    /* VISIT can occur at any node in the tree
     * DELETE will execute ONLY on a postorder traversal
     * COPY will execute on both inorder and preorder
     */

    switch (op) {
    case VISIT:
	/* Data Integrity: how or what to do here?
	 * For this tree node [node header|user data], we are going to call the users specified function with
	 * a reference to this tree node [|user data]. But, how will this node be used to safe guard integrity?
	 * If the user updates their data, copy out/in, or not is a problem. If a copy out/in, we have to copy 
	 *    back their potentially changed copy into the tree and if theirs is corrupted, ours is then corrupted.
	 * If direct access, then instead of a copy out/in, it would be corrupted directly with the same outcome.
	 *
	 * We cannot make a copy of the current node because we do not have access to the tree header that contains
	 * the freelist and the size of the users data structure.
	 *
	 * So, we let them have direct access to the tree node users data area
	 */

	/* check if function pointer exists supplied by user */
	if (compf == NULL) {
	    bst_errno = BST_ERR_NO_UCF_GIVEN;
	    return (ERROR);
	}

	/* call the users supplied function with this node as an argument, pointing to their data area */
	/* technically it is "Leaf" but the library does not have access to it: compf( (Leaf *) (p + 1), depth); */
	compf((void *) (p + 1), depth);
	return (OK);
	break;
    case DELETE:
	*dp = p;
	return (OK);
	break;
    case COPY:
	(*count)++;
	if ((p_dup = (t_node *) tallocm(T_NODE, ph_dup)) == NULL)
	    return (ERROR);
	memcpy(p_dup, p, sizeof(t_node) + ph_dup->th_usiz);
#ifdef DEBUG_MALLAC_USAGE
	printf(">>> memcpy FROM LOCATION 0x%-5x TO LOCATION 0x%-5x; %i BYTES <<<\n", p, p_dup,
	       sizeof(t_header) + ph_dup->th_usiz);
#endif
	p_dup->tn_id = ph_dup->th_id;
	if (*pp_dup != NULL)
	    switch (p_dup->tn_tag) {
	    case LEFT_SON:
		(*pp_dup)->tn_llink = p_dup;
		break;
	    case RIGHT_SON:
		(*pp_dup)->tn_rlink = p_dup;
		break;
	    }
	p_dup->tn_ulink = *pp_dup;
	p_dup->tn_llink = NULL;
	p_dup->tn_rlink = NULL;
	*pp_dup = p_dup;
	if (*count == 1) {
	    ph_dup->th_root = p_dup;
	    /* printf("     >>> this s the root node <<<\n"); */
	} else
	    /* printf("\n"); */
	    return (OK);
	break;
    case IDENT:
	if (ph_dup->th_ucf(p + 1, p_dup + 1) == 0)
	    return (OK);
	else
	    return (NOT_IDENT);
	break;
    case EQUAL:
	if (qfind(ph_dup, p)) {
	    /* printf("FOUND\n"); */
	    return (OK);
	} else {
	    /* printf("NOT FOUND\n"); */
	    return (NOT_EQUAL);
	}
	break;
    }
}
