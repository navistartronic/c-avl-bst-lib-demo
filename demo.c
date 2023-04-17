/*
  +------------------------------------------------------------------------+
  | demo is an interactive terminal program utilizing the C library libbst |
  | of AVL & BST routines.                                                 |
  |                                                                        |
  | Copyright (C) 2023  Roger P. Johnson, roger0080@netscape.net           |
  |                                                                        |
  | This program is free software: you can redistribute it and/or modify   |
  | it under the terms of the GNU General Public License as published by   |
  | the Free Software Foundation, either version 3 of the License, or      |
  | (at your option) any later version.                                    |
  |                                                                        |
  | This program is distributed in the hope that it will be useful,        |
  | but WITHOUT ANY WARRANTY; without even the implied warranty of         |
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          |
  | GNU General Public License for more details.                           |
  |                                                                        |
  | You should have received a copy of the GNU General Public License      |
  | along with this program.  If not, see <https://www.gnu.org/licenses/>. |
  +------------------------------------------------------------------------+
*/

/*
 * What it is:
 * -----------
 * demo is an interactive terminal program utilizing the C library libbst.a
 * containing AVL & BST routines. 
 * 
 * This program demonstrates how you would use these library API calls.
 * 
 * You can create both AVL and Binary Search Trees when you create a new
 * tree via 
 *    bst_create(tn, AVL, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_YES))
 *    bst_create(tn, BST, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_YES)
 * see create.c for explanation of the parameters. The demo program defaults to
 * AVL tree and the 'newt' new tree command does not prompt for which type of
 * tree to create.
 * 
 * Program compile options:
 *   DEBUG_EXPLOIT_TREE_HDR : 
 *     Do not enable for production use.
 *     User space should not know about this detail, how to get to the tree header record 
 *     and print internal information about the internal workings.
 *
 * Makefile Library compile options/library DEBUG/TRACE build flags that can be enabled:
 *   Enable/disable informative messages as routines in the library execute (for production and test usage)
 *   DEBUG_LIB_WITH_LIBCALL_TRACING = -DDEBUG_TRACE
 * 
 *   Enable/disable the showing of ASCII art graph boxes on allocation of nodes w/addresses from the library routines:
 *   DEBUG_LIB_SHOW_LIBCALL_MALLOC_GRAPHS = -DDEBUG_SHOWGRAPHS
 *
 *   Enable/disable the showing of allocation/deallocation malloc'd memory w/addresses from the library routines:
 *   DEBUG_LIB_SHOW_LIBCALL_MALLOC_INFO = -DDEBUG_MALLAC_USAGE
 *
 *   Enable/disable AVL tree rebalance info from the library routines:
 *   DEBUG_LIB_SHOW_LIBCALL_TREE_REBALANCE = -DDEBUG_SHOWREBALANCE
 *
 * 
 *  To run:
 *     Edit the Makefile if you want to enable any of the DEBUG_LIB_SHOW_LIBCALL_* features
 *     $ vi Makefile
 *
 *     Edit the include file "leaf.h" and add any fields you want.
 *     Currently it contains just the key, no data fields:
 *     $ vi leaf.h
 *
 *     Write the function to compare your keys, see below in this module:
 *        int f(Leaf * r1, Leaf * r2)
 *
 *     Write the function to print out your records, see below in this module:
 *        void Print_Node(Leaf * pl, int level)
 *
 *     Build it
 *     $ gmake all
 *
 *     Run it
 *     $ ./demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/**************************************************************************
 ** PART 1 OF 2:
 ** YOU MUST DEFINE SOMETHING THAT YOU WANT TO PUT INTO THE TREE. IN THIS **
 ** CASE IT IS A VERY SIMPLE STRUCTURE. WHAT THAT OBJECT IS IS IRREVELANT **
 ** AS LONG AS YOU CAN COMPARE IT AND PRINT IT.                           **
 **************************************************************************/
/* include of leaf.h needs to come before include of bstpkg.h */
#include "leaf.h"


/************************************************************************ 
 ** PART 2 OF 2:
 ** THE FOLLOWING INCLUDE DIRECTIVE IS ALL THAT IS NEEDED TO ACCESS AND **
 ** USE THE BST LIBRARY ROUTINES:                                       **
 ************************************************************************/
#include "bstpkg.h"


/************************************************************************
 **               FOR DEBUGGING AND TESTING ONLY                       **
 ************************************************************************/
#ifdef DEBUG_EXPLOIT_TREE_HDR
#define  MAX_ID_LEN          32	/* program version number */
/* included only for debugging or temp TODO on treewalk */
#include "struct.h"
typedef struct header t_header;
typedef struct node t_node;
#endif

#define  MAX_TREENAME_LENGTH   8	/* max length for a tree name;  */
#define  MAX_CMD_LEN           64	/* buffer size for command */

typedef enum { DELETE, COPY, VISIT, IDENT, EQUAL } TWalkOps;

typedef enum { INORDER, PREORDER, POSTORDER } Traversals;

typedef enum { LEFT_SON, ROOT, RIGHT_SON } Tags;

/* All user input commands are mapped to this enumeration */
typedef enum { NEWT, DELT, SHOWT, ISEMPTY, COUNT, DEF, CKT, ADDK, FINDK,
    DELK, PRINT, RPRINT, QUIT, STOP, UNKNOWN, STATS, TWALK, TCOPY,
    TIDENT, TEQUAL
} ValidCmd;

const int rev_major = 1;
const int rev_minor = 0;
const int rev_patch = 0;
static char *RCSid[] = { "$Id$" };

void display_err_msg(void);


int main(int argc, char *argv[])
{
    void display_intro(void);	/* display greeting to user */
    void display_screen(void);	/* display standard screen */
    ValidCmd get_cmd(void);	/* get and validate user command */
    int process_cmd(ValidCmd);	/* process user command */
    void display_ending(void);	/* display end of program message */

    display_intro();
    do
	display_screen();
    while (process_cmd(get_cmd()));
    display_ending();

    return (0);
}

/* display_intro: display's introductory screen to user */
void display_intro(void)
{
    int n;
    char buffer[26];
    struct tm *tm;
    struct timeval tp;
    struct timezone tz;

/*
    time_t timer;

    time(&timer);
    tm = localtime(&timer);
*/
    gettimeofday(&tp, &tz);
    tm = localtime(&tp.tv_sec);

    strftime(buffer, 26, "%c", tm);

    /* URL https://www.gnu.org/licenses/gpl-3.0.html#howto */
    printf("demo  Copyright (C) 2023  Roger P. Johnson\n");
    printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
    printf("Program licensed under GNU General Public License v3.0 (GNU GPLv3),\n");
    printf("Program library licensed under GNU Lesser General Public License v3.0 (GNU LGPLv3).\n");
    printf("This is free software, and you are welcome to redistribute it under\n");
    printf("certain conditions; see appropriate GNU license.\n");
    printf("\n");
    printf("Version %d.%d.%d %s\n", rev_major, rev_minor, rev_patch, buffer);

    if (0) {
	printf("Here is a list of possible error numbers and messages:\n\n");
	for (n = 100; n < 130; n++) {
	    bst_errno = n;
	    display_err_msg();
	}
    }
}

/* display_screen: what you want the user to see before each prompting */
void display_screen(void)
{
    printf("\n\n\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("\n");
    printf("Commands available:\n");
    printf("     To exit program:\n");
    printf("         quit (releases allocated tree/node memory)\n");
    printf("         stop (abort, no cleanup of allocated memory)\n");
    printf("\n");
    printf("     tree operations       key operations");
#ifdef DEBUG_EXPLOIT_TREE_HDR
    printf("       non-std utilities\n");
#else
    printf("\n");
#endif

    printf("     ===============       --------------");
#ifdef DEBUG_EXPLOIT_TREE_HDR
    printf("       =================\n");
#else
    printf("\n");
#endif
    printf("     newt     delt         addk     delk");
#ifdef DEBUG_EXPLOIT_TREE_HDR
    printf("        stat    twalk\n");
#else
    printf("\n");
#endif
    printf("     copy\n");
    printf("\n");
    printf("     tree inquiries        key inquiries\n");
    printf("     ===============       --------------\n");
    printf("     showt    def          findk\n");
    printf("     isempty  count\n");
    printf("     equal    ident\n");
    printf("     rprint   print\n");
    printf("     ckt\n");
    printf("\n");
}

/* get_cmd: prompt for command and validate it to a know state */
ValidCmd get_cmd(void)
{
    char cmd[MAX_CMD_LEN + 1];	/* buffer to hold user's input in */

    do {
	printf("Command: ");
	fflush(stdin);
	fgets(cmd, MAX_CMD_LEN + 1, stdin);
	cmd[strcspn(cmd, "\r\n")] = '\0';
    } while (strlen(cmd) == 0);

    if (strcmp("newt", cmd) == 0)
	return (NEWT);
    else if (strcmp("addk", cmd) == 0)
	return (ADDK);
    else if (strcmp("showt", cmd) == 0)
	return (SHOWT);
    else if (strcmp("delk", cmd) == 0)
	return (DELK);
    else if (strcmp("print", cmd) == 0)
	return (PRINT);
    else if (strcmp("delt", cmd) == 0)
	return (DELT);
    else if (strcmp("count", cmd) == 0)
	return (COUNT);
    else if (strcmp("isempty", cmd) == 0)
	return (ISEMPTY);
    else if (strcmp("ckt", cmd) == 0)
	return (CKT);
    else if (strcmp("stop", cmd) == 0)
	return (STOP);
    else if (strcmp("findk", cmd) == 0)
	return (FINDK);
    else if (strcmp("rprint", cmd) == 0)
	return (RPRINT);
    else if (strcmp("def", cmd) == 0)
	return (DEF);
    else if (strcmp("stop", cmd) == 0)
	return (STOP);
    else if (strcmp("quit", cmd) == 0)
	return (QUIT);
#ifdef DEBUG_EXPLOIT_TREE_HDR
    else if (strcmp("stat", cmd) == 0)
	return (STATS);
    else if (strcmp("twalk", cmd) == 0)
	return (TWALK);
#endif
    else if (strcmp("copy", cmd) == 0)
	return (TCOPY);
    else if (strcmp("ident", cmd) == 0)
	return (TIDENT);
    else if (strcmp("equal", cmd) == 0)
	return (TEQUAL);
    else
	return (UNKNOWN);
}

/* process_cmd: execute desired command */
int process_cmd(ValidCmd usrcmd)
{


    char tn[MAX_TREENAME_LENGTH + 1],	/* buffer to hold user entered tree name */
     ttype[MAX_CMD_LEN + 1],		/* buffer to hold user tree type: avl or bst */
     kn[LEAF_KEYLEN + 2];		/* buffer to hold user entered key name include/leaf.h */

    void display_bad_cmd(void);		/* comment to make when an invalid selection is made */
    void new_tree(char *, char *);	/* create a new tree */
    void rprint_tree(char *);		/* recursively print the tree (DEBUGGING ONLY) */
    void print_tree(char *);		/* use a non-recursive tree print routine */
    void node_count(char *);		/* get current number of nodes in the tree */
    void is_empty(char *);		/* check if tree is empty: has zero nodes */
    void check_tree(char *);		/* perform tree verification (DEBUGGING ONLY) */
    void delete_tree(char *);		/* delete entire tree; all nodes and tree definintion */
    void show_trees(void);		/* show list of defined trees */
    void delete_trees(void);		/* delete list of defined trees */
    void add_key(char *, char *);	/* add a new key to the tree */
    void find_key(char *, char *);	/* find a key in the tree and return it */
    void delete_key(char *, char *);	/* remove a key from the tree */
    void is_defined(char *);		/* see if tree is defined or not */
#ifdef DEBUG_EXPLOIT_TREE_HDR
    void stat_tree(char *);		/* print tree header record */
    void treewalk(char *);		/* traverse tree in 3 ways */
#endif
    void tcopy(char *);			/* duplicate an existing tree */
    void tident(char *);		/* are two trees identical in structure */
    void tequal(char *);		/* are two trees equal */

    if (usrcmd == QUIT) {
	/* traverse through all the trees freeing each node */
	delete_trees();
	return (0);
    }

    if (usrcmd == STOP)
	/* abort, no clean up, no freed memory */
	return (0);

    if (usrcmd == UNKNOWN) {
	display_bad_cmd();
	return (1);
    }

    if (usrcmd != SHOWT) {
	do {
	    printf("Tree name (%d char max): ", MAX_TREENAME_LENGTH);
	    fflush(stdin);
	    fgets(tn, MAX_TREENAME_LENGTH + 1, stdin);
	    tn[strcspn(tn, "\r\n")] = '\0';
	} while (strlen(tn) == 0);
    }

    switch (usrcmd) {
    case NEWT:
	do {
	    printf("create new tree as an AVL tree or a BST Binary Search Tree [avl | bst] ? ");
	    fgets(ttype, MAX_CMD_LEN, stdin);
	    ttype[strcspn(ttype, "\r\n")] = '\0';
	    if (strlen(ttype) && (strncmp(ttype, "avl", 3) != 0 && strncmp(ttype, "bst", 3) != 0)) {
		printf(" invalid tree type, try again ..\n");
		strcpy(ttype, "\0");
	    }
	} while (strlen(ttype) == 0);
	/*printf("tree type [%s]\n", ttype);*/
	new_tree(tn, ttype);
	break;
    case RPRINT:
	rprint_tree(tn);
	break;
    case PRINT:
	print_tree(tn);
	break;
    case COUNT:
	node_count(tn);
	break;
    case ISEMPTY:
	is_empty(tn);
	break;
    case CKT:
	check_tree(tn);
	break;
#ifdef DEBUG_EXPLOIT_TREE_HDR
    case STATS:
	stat_tree(tn);
	break;
#endif
    case DELT:
	delete_tree(tn);
	break;
    case SHOWT:
	show_trees();
	break;
    case ADDK:
    case FINDK:
    case DELK:
	do {
	    printf("Enter key (%d char max): ", LEAF_KEYLEN);
	    fflush(stdin);
	    fgets(kn, LEAF_KEYLEN + 2, stdin);
	    kn[strcspn(kn, "\r\n")] = '\0';
	} while (strlen(kn) == 0);
	printf("key [%s]\n", kn);

	switch (usrcmd) {
	case ADDK:
	    add_key(tn, kn);
	    break;
	case FINDK:
	    find_key(tn, kn);
	    break;
	case DELK:
	    delete_key(tn, kn);
	    break;
	}
	break;
    case DEF:
	is_defined(tn);
	break;
#ifdef DEBUG_EXPLOIT_TREE_HDR
    case TWALK:
	treewalk(tn);
	break;
#endif
    case TCOPY:
	tcopy(tn);
	break;
    case TIDENT:
	tident(tn);
	break;
    case TEQUAL:
	tequal(tn);
	break;
    case UNKNOWN:
	display_bad_cmd();
	break;
    }
    return (1);
}

/* display_ending: messge to display on leaving */
void display_ending(void)
{
    printf("\nThank you, come again.\n");
}

/* new_tree: create a new bst tree */
void new_tree(char *tn, char *tt)
{
    int f(Leaf *, Leaf *);		/* user written compare function for this tree */
    void Print_Node(Leaf *, int);	/* user written tree printing routine (optional) */

    if (strncmp(tt, "avl", 3) && strncmp(tt, "bst", 3)) {
	printf("\n  ### Can't create tree %s -- invalid tree type [%s]###\n", tn, tt);
	return;
    }

    if (bst_create(tn, strncmp(tt, "avl", 3) == 0 ? AVL : BST, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_YES)) {
	printf("\n  --- tree [%s] created as [%s] ---\n", tn, tt);
    } else {
	display_err_msg();
	printf("\n  ### Can't create tree [%s] ###\n", tn);
    }
}

/* rprint_tree: recursively print the tree (DEBUGGING ONLY) */
void rprint_tree(char *tn)
{
    bst_rprint(tn);
    if (bst_errno != 0) {
	display_err_msg();
	printf("\n  ### Can't print tree %s ###\n", tn);
    }
}

/* print_tree: non-recursive tree print routine */
void print_tree(char *tn)
{
    bst_print(tn);
    if (bst_errno != 0) {
	display_err_msg();
	printf("\n  ### Can't print tree %s ###\n", tn);
    }
}

/* node_count: get current number of nodes in the tree */
void node_count(char *tn)
{
    int n;

    if ((n = bst_count(tn)) < 0) {
	display_err_msg();
	printf("\n  ### Tree not defined %s ###\n", tn);
    } else
	printf("\n  --- Tree %s has %i node(s) ---\n", tn, n);
}


/* is_empty: check if tree is empty; tree defined and no nodes */
void is_empty(char *tn)
{
    if (bst_empty(tn)) {
	if (bst_errno != 0)
	    display_err_msg();
	printf("\n  --- TRUE ---\n");
    } else
	printf("\n  --- FALSE ---\n");
}

/* check_tree: perform tree verification; tn_tags, bf's & links (DEBUGGING ONLY) */
void check_tree(char *tn)
{
    bst_stat(tn);
    if (bst_errno != 0)
	display_err_msg();
    else
	printf("\n  --- verified  ---\n");
}

/* deltete_tree: delete all nodes and the tree definition itself */
void delete_tree(char *tn)
{
    Leaf *pk;			/* structure to pass containing key to find */

    if (bst_delete(tn))
	printf(" --- Tree Deleted ---\n");
    else {
	display_err_msg();
	printf("\n ### Can't delete tree %s ###\n", tn);
    }
}

/* show_trees: show list of all defined trees */
/* to-do/complete */
void show_trees(void)
{
    /* strings is dynamically created malloc'd array of pointer to char, pointing to a 
     * malloc'd null terminated string which is the tree name and both must be free'd
     * after the call and usage of it: the malloc'd tree name string array value, and 
     * the malloc'd array of pointers string[] 
     *   storage[0]  => "treeFoo", storage[1] => "treeBar", ..., storage[size-1] => "treeBar"
     */ 
    char **strings = NULL;
    int size, i;
    extern void find_header_list(char ***stringArr, int *arrSize);

    find_header_list(&strings, &size);
    printf("Defined Trees (%d):\n", size);
    if (size == 0) {
	printf("\tNo trees defined.\n");
    } else {
	for (i = 0; i < size; i++) {
	    printf("\t%5d\t%s\n", i + 1, strings[i]);
            free(strings[i]);
	}
    }

    free(strings);

}

void delete_trees(void)
{
    /* strings is dynamically created malloc'd array of pointer to char, pointing to a 
     * malloc'd null terminated string which is the tree name and both must be free'd
     * after the call and usage of it: the malloc'd tree name string array value, and 
     * the malloc'd array of pointers string[] 
     *   storage[0]  => "treeFoo", storage[1] => "treeBar", ..., storage[size-1] => "treeBar"
     */ 
    char **strings = NULL;
    int size, i;
    extern void find_header_list(char ***stringArr, int *arrSize);

    find_header_list(&strings, &size);
    if (size == 0) {
	//printf("cleanup: no trees defined to delete.\n");
    } else {
	printf("cleanup: %d defined tree(s) to delete\n", size);
	for (i = 0; i < size; i++) {
	    printf("cleanup: deleting tree %5d\t%s\n", i + 1, strings[i]);
	    delete_tree(strings[i]);
            free(strings[i]);
	}
    }

    free(strings);

}


/* add_key: add a new key into the tree */
void add_key(char *tn, char *kn)
{
    Leaf *pk;			/* structure to pass containing key to find */

    if ((pk = (Leaf *) bst_alloc(tn)) != NULL) {
	strcpy(pk->key, kn);
	if (bst_put(tn, pk))
	    printf("\n --- key added %s ---\n", kn);
	else {
	    display_err_msg();
	    printf("\n ### duplicate key ###\n");
	}
	bst_release(tn, pk);
    } else {
	display_err_msg();
	printf("\n  ### Cannot add key %s to tree %s ###\n", kn, tn);
    }
}

/* find_key: find a key in the tree */
void find_key(char *tn, char *kn)
{
    Leaf *pnl, *l;

    if (bst_defined(tn)) {
	pnl = (Leaf *) bst_alloc(tn);
	strcpy(pnl->key, kn);
	if ((l = (Leaf *) bst_get(tn, pnl)) == NULL) {
	    display_err_msg();
	    printf("\n  ### key not found %s ###\n", kn);
	} else {
	    printf("\n  --- key found %s ---\n", kn);
	    bst_release(tn, l);
	}
	bst_release(tn, pnl);
    } else
	printf("\n  ### Tree not defined %s ###\n", tn);
}

/* delete_key: remove a key from the tree */
void delete_key(char *tn, char *kn)
{
    Leaf *pnl;

    if (bst_defined(tn)) {
	pnl = (Leaf *) bst_alloc(tn);
	strcpy(pnl->key, kn);
	if (bst_remove(tn, pnl)) {
	    printf("\n  --- key deleted %s ---\n", kn);
	} else {
	    display_err_msg();
	    printf("\n  ### key not found %s ###\n", kn);
	}
	bst_release(tn, pnl);
    } else
	printf("\n  ### Tree not defined %s ###\n", tn);

}

/* is_defined: check if the tree is defined */
void is_defined(char *tn)
{
    if (bst_defined(tn))
	printf("\n  --- TRUE ---\n");
    else {
	display_err_msg();
	printf("\n  --- FALSE ---\n");
    }
}

/* tequal: compare two tree for equal keys */
void tequal(char *tn)
{
    char ntn[MAX_TREENAME_LENGTH + 1];

    do {
	printf("Compare Tree: ");
	fflush(stdin);
	fgets(ntn, MAX_TREENAME_LENGTH + 1, stdin);
	ntn[strcspn(ntn, "\r\n")] = '\0';
    } while (strlen(ntn) == 0);

    if (bst_equal(tn, ntn))
	printf("\n  --- Trees %s & %s are equal ---\n", tn, ntn);
    else {
	if (bst_errno != 0)
	    display_err_msg();
	printf("\n  ### Trees %s, %s are not equal ###\n", tn, ntn);
    }
}

/* tident: compare two tree for identical structure and keys */
void tident(char *tn)
{
    char ntn[MAX_TREENAME_LENGTH + 1];

    do {
	printf("Compare Tree: ");
	fflush(stdin);
	fgets(ntn, MAX_TREENAME_LENGTH + 1, stdin);
	ntn[strcspn(ntn, "\r\n")] = '\0';
    } while (strlen(ntn) == 0);

    if (bst_ident(tn, ntn))
	printf("\n  --- Trees %s & %s are identical ---\n", tn, ntn);
    else {
	if (bst_errno != 0)
	    display_err_msg();
	printf("\n  ### Trees %s, %s are not identical ###\n", tn, ntn);
    }
}

/* tcopy: create a copy of an existing tree */
void tcopy(char *tn)
{
    char ntn[MAX_TREENAME_LENGTH + 1];

    do {
	printf("Copy to name: ");
	fflush(stdin);
	fgets(ntn, MAX_TREENAME_LENGTH + 1, stdin);
	ntn[strcspn(ntn, "\r\n")] = '\0';
    } while (strlen(ntn) == 0);

    if (bst_copy(tn, ntn))
	printf("\n  tree copied\n");
    else {
	if (bst_errno != 0)
	    display_err_msg();
	printf("\n  can't copy %s\n", tn);
    }
}

#ifdef DEBUG_EXPLOIT_TREE_HDR
/* twalk: traverse the tree in 3 orders */
void treewalk(char *tn)
{
    t_header *ph;
    char buf[MAX_CMD_LEN + 1];
    int ttraversal;
    void Print_Node(Leaf *, int);		/* user written tree printing routine (optional) */
    void Treewalk_Print_Node(Leaf *, int);	/* user written tree printing routine (optional) */
    Boolean twalk(TWalkOps, Traversals, ...);
    extern t_header *find_header(char *tname);

    /* TODO bst_treewalk(tn, treeorder,userfunction); */

    if ((ph = (t_header *) find_header(tn)) == NULL) {;
	display_err_msg();
	printf("\n ### tree not defined %s ###\n", tn);
    } else {
	do {
	    printf("1 = inorder, 2 = preorder, 3 = postorder; which ? ");
	    fflush(stdin);
	    fgets(buf, MAX_CMD_LEN + 1, stdin);
	    buf[strcspn(buf, "\r\n")] = '\0';
	} while (strlen(buf) == 0 || atoi(buf) < 1 || atoi(buf) > 3);
	ttraversal = atoi(buf);
	printf("tree walk = [%d]\n", ttraversal);

	switch (ttraversal) {
	    /* pass in a user defined function for the routine to call using the current leaf node data: */
	case 1:
	    twalk(VISIT, INORDER, ph->th_root, Treewalk_Print_Node);
	    break;
	case 2:
	    twalk(VISIT, PREORDER, ph->th_root, Treewalk_Print_Node);
	    break;
	case 3:
	    twalk(VISIT, POSTORDER, ph->th_root, Treewalk_Print_Node);
	    break;
	default:
	    twalk(VISIT, INORDER, ph->th_root, Treewalk_Print_Node);
	}
    }
}
#endif

#ifdef DEBUG_EXPLOIT_TREE_HDR
/* stat_tree: output contents of tree header record -- debug only */
void stat_tree(char *tn)
{
    t_header *ph;
    t_node *pn;
    extern t_header *find_header(char *tname);

    if ((ph = (t_header *) find_header(tn)) == NULL) {
	printf("\n### tree not defined ###\n\n");
	return;
    }
    printf("\n");
    printf("size of this structure = %i\n", sizeof(t_header));
    printf("address for this header = (0x%-5x)\n\n", ph);
    printf("ph->th_link       = (0x%-5x)\n", ph->th_link);
    printf("ph->th_name       = '%s'\n", ph->th_name);
    printf("ph->th_bsttype    = '%s'\n", ph->th_bsttype == AVL ? "AVL" : (ph->th_bsttype == BST ? "BST" : "UNKNOWN"));
    printf("ph->th_root       = (0x%-5x)\n", ph->th_root);
    printf("ph->th_flist      = (0x%-5x)\n", ph->th_flist);
    printf("ph->th_id         = %f\n", ph->th_id);
    printf("ph->th_flcnt      = %i\n", ph->th_flcnt);
    printf("ph->th_stat       = %s\n", ph->th_stat == TRUE ? "TRUE" : "FALSE");
    printf("ph->th_np         = %s\n", ph->th_np == TRUE ? "TRUE" : "FALSE");
    printf("ph->th_usiz       = %i\n", ph->th_usiz);
    printf("ph->th_ucf        = (0x%-5x)\n", ph->th_ucf);
    printf("ph->th_upf        = (0x%-5x)\n", ph->th_upf);
    printf("ph->th_ncnt       = %i\n", ph->th_ncnt);
    printf("ph->th_version_id = '%s'\n", ph->th_version_id);
    return;
}
#endif

/* display_bad_cmd: displays a randomm message or a wrong/invalid/unknown command by the user */
void display_bad_cmd(void)
{
    int r;
    struct timeval tp;
    struct timezone tz;

    gettimeofday(&tp, &tz);

    srand(tp.tv_usec);

    r = rand();
    r /= 1000;

    if (0) {
	if (r < 1)
	    printf(" uh?\n");
	else if (r < 2)
	    printf(" what?\n");
	else if (r < 3)
	    printf(" say what?\n");
	else if (r < 4)
	    printf(" what'da?\n");
	else if (r < 5)
	    printf(" eh?\n");
	else if (r < 6)
	    printf(" who? me?\n");
	else if (r < 7)
	    printf(" what'da mean ?\n");
	else if (r < 8)
	    printf(" oh sure...\n");
	else if (r < 9)
	    printf(" ouch!\n");
	else
	    printf(" yeah right!\n");
    }

    printf(" bad command\n");
}


/* display_err_msg: display the error message corresponding to the current bst_errno number */
void display_err_msg(void)
{				/* prints out the error msg. corresponding to bst_errno */
    printf("  caused BST error %4i: %s\n", bst_errno, bst_errmsg(bst_errno));
}

/* f: user written compare function to compare any two nodes in the tree */
int f(Leaf * r1, Leaf * r2)
{
    /*********************************************************************************
    ** YOU MUST WRITE A COMPARE FUNCTION FOR YOUR STRUCTURE/OBJECT THAT WILL RETURN **
    ** NEGATIVE IF a < b; ZERO IF a = b; POSITIVE IF a > b                          **
    *********************************************************************************/
    if (strcmp(r1->key, r2->key) < 0)
	return (-1);
    else if (strcmp(r1->key, r2->key) == 0)
	return (0);
    else
	return (1);
}

/* Print_Node: user written tree print routing (optional); (this one has debugging info in it */
void Print_Node(Leaf * pl, int level)
{
    /***************************************************************************************
    ** THIS USER WRITTEN FUNCTION IS OPTIONAL. IF YOU USE IT, YOU CAN FORMAT THE PRINTING **
    ** OF THE TREE AS PLAIN/FANCY AS YOU WISH                                             **
    ***************************************************************************************/

#define TAB_COL   30		/* which column to print the debugging info in */

#ifdef DEBUG_EXPLOIT_TREE_HDR
    /* user space should not know about this detail, how to get to the tree header record */
    /* the following typedef is put in *only* for development purposes only */

    t_node *pn;
#endif

    int j;

    if (level < 0)
	printf("/\n");
    else {
	if (level == 0) {
	    printf("/ ");
	    if (pl == NULL) {
		printf("<null>\n");
		return;
	    }
	} else
	    printf("  ");

	for (j = 1; j < 4 * level + 3; j++)
	    putchar(' ');

	printf("[%2s]", pl->key);
	for (j = 4 * level + 5; j < TAB_COL; j++)
	    putchar(' ');

#ifdef DEBUG_EXPLOIT_TREE_HDR
	/* user space should not know about this detail, how to get to the tree header record */
	pn = ((t_node *) pl) - 1;

	printf("(0x%x) LVL: %3i, BAL: %2i, TAG: %s ", pn, level, pn->tn_bf,
	       pn->tn_tag == LEFT_SON ? "lson" : (pn->tn_tag == ROOT ? "ROOT" : "rson"));

	printf("ulink(0x%-4x) llink(0x%-4x) rlink(0x%-4x) \n\n", pn->tn_ulink, pn->tn_llink, pn->tn_rlink);
#endif
    }
}

void Treewalk_Print_Node(Leaf * pl, int level)
{
    if (pl == NULL) {
	printf("(Treewalk_Print_Node():: level [%09d] key [%s]\n", level, "NULL");
    } else {
	printf("(Treewalk_Print_Node():: level [%09d] key [%s]\n", level, pl->key);
    }
}
