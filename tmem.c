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

#include <stdarg.h>

#ifndef BST_HDR
#include "bst.h"
#endif

#define  OUT_OF_MEM  NULL

static char *RCSid[] = { "$Id: tmem.c,v 2.1 1999/01/02 17:08:43 roger Exp $" };

extern int bst_errno;


/* tallocm: central memory allocator for the library */
void *tallocm(MallocTypes mkind, ...)
{
 /*******************************************************************************
  *  A private library function that  is the central memory allocator. All modules
  *  that need to make a dynamic memory allocataion is perfomed with tallocm. 
  *  Exceptions to this are the library calls to strdup. All tree nodes returned
  *  whether new or used are  zeroed out via memset to guarentee a blank node.
  *
  *  Input Parameters
  *  =================
  *  tallocm uses a variable argument list
  *  USAGE: ph = (t_header *) tallocm(T_HEADER, sizeof(t_header);
  *         pn = (t_node *) tallocm(T_NODE, ph);
  *
  *  If mkind is T_HEADER, then allocate a new tree header record:
  *        mkind : Is T_Header
  *        size  : Number of bytes to allocate
  *
  *  If mkind is T_NODE, then allocate a new tree node:
  *        mkind : Is T_NODE
  *        ph    : Is pointer to the header record for this tree
  *
  *  Output Parameters
  *  =================
  *  p : If mkind is T_HEADER, p is pointing to a newly allocated header record
  *      If mkind is T_NODE, p pointing to to a newly allocated tree node
  *
  *  p is NULL, pointer to a header record, or pointer to a tree node on exit
  *
  *  Function name returns Boolean result:
  *  TRUE       : A new tree was created.
  *  FALSE      : Tree is already defined or malloc error.
  *
  *  Global Variables
  *  =================
  *  bst_errno  : Global error varible. Set only if an error occurs
  *******************************************************************************/

    int size;			/* bytes to allocate */
    void *p;			/* generic pointer to a t_header or a t_node */
    va_list ap;			/* formal function argument pointer */
    Boolean error;		/* routine error flag */
    t_header *ph;		/* pointer to a defined tree header record */

#ifdef DEBUG_SHOWGRAPHS
    extern void gheader(t_header * ph);
    extern void gnode(t_header * ph, t_node * pn);
#endif

    error = FALSE;		/* initalize */
    va_start(ap, mkind);	/* ap points to 1st argument now */

    switch (mkind) {
    case T_HEADER:		/* return a NEW tree header record */
	size = (int) va_arg(ap, int);
	if ((p = (void *) malloc(size)) == OUT_OF_MEM)
	    error = TRUE;
#ifdef DEBUG_MALLAC_USAGE
	printf(">>> ALLOCATING MEMORY FOR T_HEADER AT 0x%-5x; %i BYTES <<<\n", p, size);
#endif
#ifdef DEBUG_SHOWGRAPHS
	gheader(p);
#endif

	break;
    case T_NODE:		/* return a NEW or USED node */
	ph = (t_header *) va_arg(ap, t_header *);	/* next arg */

	/* first check and see if there are any used nodes in the free list;  if there is, */
	/* get one from the list; else  malloc a new one. In either case zero out the node */
	/* before returning it:                                                            */

	if (ph->th_flist == NULL) {
	    size = sizeof(t_node) + ph->th_usiz;
	    if ((p = (t_node *) malloc(size)) == OUT_OF_MEM)
		error = TRUE;
#ifdef DEBUG_MALLAC_USAGE
	    printf(">>> ALLOCATING MEMORY FOR T_NODE AT 0x%-5x; %i BYTES <<<\n", p, size);
#endif
#ifdef DEBUG_SHOWGRAPHS
	    gnode(ph, p);
#endif
	} else {
	    p = (t_node *) ph->th_flist;
	    ph->th_flist = ((t_node *) p)->tn_ulink;
	    ph->th_flcnt--;
#ifdef DEBUG_MALLAC_USAGE
	    printf(">>> RE-USING MEMORY FOR T_NODE AT 0x%-5x; %i BYTES <<<\n", p, ph->th_usiz);
#endif
#ifdef DEBUG_SHOWGRAPHS
	    gnode(ph, p);
#endif
	}
	if (!error) {
	    memset(((t_node *) p) + 1, 0, ph->th_usiz);
	    p = (void *) p;
#ifdef DEBUG_MALLAC_USAGE
	    printf(">>> memset AT LOCATION 0x%-5x to 0; %i BYTES <<<\n", ((t_node *) p) + 1, ph->th_usiz);
#endif
	}
	break;
    }

    va_end(ap);			/* this call is required before leaving the function */
    if (error) {
	bst_errno = BST_ERR_MALLOC;
	fprintf(stderr, "malloc error: cannot allocate memory %i\n", size);
    }

    return (p);
}

/* tfreem: central memory deallocator for the library */
void tfreem(MallocTypes mkind, ...)
{
 /*******************************************************************************
  *  A private library function that is the central memory deallocator. All
  *  modules that need to free up memory is done via tfreem.
  *
  *  Input Parameters
  *  =================
  *  tfreem uses a variable argument list
  *  USAGE: tfreem(T_HEADER, ph);
  *         tfreem(T_NODE, CHAIN, ph, p);
  *         tfreem(T_NODE, FREE, p); *                                                                              |
  *  if mkind is T_HEADER, then deallocate a tree header record:
  *       mkind : Is T_HEADER
  *       ph    : Is a pointer to the header record to deallocate
  *
  *  if mkind is T_NODE:
  *       mkind : Is T_NODE
  *       op    : Is the operation to perform: CHAIN or FREE the node
  *               If op is CHAIN, then the next 2 arg's are:
  *                     ph : Pointer to the tree header record to return the node
  *                     p  : Pointer to the tree ode to return
  *               If op is FREE, then the next arg is
  *                     p : Pointer to the node to free
  *
  *  Output Parameters
  *  =================
  *  If mkind is T_HEADER:  ph is set to NULL
  *  If mkind is T_NODE:    p is set to NULL
  *
  *  Global Variables
  *  =================
  *  None.
  *******************************************************************************/

    va_list ap;			/* points to each argument in turn */
    t_header *ph;		/* pointer to tree header record */
    t_node *pn;			/* pointer to tree node */
    FreeOpts op;		/* operation to perform on node: FREE it or CHAIN it */

    va_start(ap, mkind);	/* initialize arg pointer */

    switch (mkind) {
    case T_HEADER:		/* free a tree header record */
	ph = (t_header *) va_arg(ap, t_header *);
	free(ph);
#ifdef DEBUG_MALLAC_USAGE
	printf(">>> FREEING MEMORY FOR T_HEADER AT 0x%-5x <<<\n", ph);
#endif
	break;
    case T_NODE:		/* free or chain a tree node */
	op = va_arg(ap, FreeOpts);
	switch (op) {
	case CHAIN:		/* chain it into the list of free nodes in the tree header */
	    ph = (t_header *) va_arg(ap, t_header *);
	    pn = (t_node *) va_arg(ap, t_node *);

	    /* There is a limit to the number of nodes that can be in a tree header's free   */
	    /* list defined by MAX_FLIST. If the free list is max'd out, then free the node; */
	    /* else chain it into the the free list maintained in the header record:         */

	    if (ph->th_flcnt < MAX_FLIST - 1) {
		pn->tn_ulink = ph->th_flist;
		ph->th_flist = pn;
		ph->th_flcnt++;
#ifdef DEBUG_MALLAC_USAGE
		printf(">>> CHAINING T_NODE AT LOCATION 0x%-5x TO HEADER <<<\n", pn);
#endif
	    } else {
#ifdef DEBUG_MALLAC_USAGE
		printf(">>> (case T_NODE/CHAIN (th_flist too many) FREEING T_NODE AT LOCATION 0x%-5x <<<\n", pn);
#endif
		free(pn);
	    }
	    break;
	case FREE:		/* free it up */
	    pn = (t_node *) va_arg(ap, t_node *);
#ifdef DEBUG_MALLAC_USAGE
	    printf(">>> (case T_NODE/FREE) FREEING T_NODE AT LOCATION 0x%-5x <<<\n", pn);
#endif
	    free(pn);
	    break;
	}
	break;
    }
    va_end(ap);			/* required call before exiting */
}
