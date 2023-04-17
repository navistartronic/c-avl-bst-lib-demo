/*
  +------------------------------------------------------------------------+
  | test is a terminal program utilizing the C library libbst of AVL & BST |
  | routines that adds, finds, checks tree balance, and deletes all for a  |
  | predetermined number of keys.                                          |
  |                                                                        |
  | Copyright (C) 2023 Roger P. Johnson, roger0080@netscape.net            |
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
 * test will insert ARRSIZ keys into the the tree. Then it will perform
 * a find for each key in the tree noting any errors.
 * test will stress test the tree upto system limits.
 * It will:
 *  1. create the AVL tree.
 *  2. insert ARRSIZ random keys into the tree.
 *  3. peform a check tree to see that the tree is still in balance.
 *  4. read/search for each key in the tree noting any not founds.
 *  5. print upto MAX_DISPLAY records in doing a tree print.
 *  6. delete each key in the tree
 *  7. delete the tree.
 *  8. print a report summary
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __GNUC__
#include <time.h>
#else
#include <sys/time.h>
#endif

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
 **               FOR DEBUGGING AND TESTING ONLY                        **
 ************************************************************************/
#ifdef DEBUG_EXPLOIT_TREE_HDR
#define  MAX_ID_LEN          32	/* defined in bst.h */
/* included only for debugging or temp TODO on treewalk */
#include "struct.h"
typedef struct header t_header;
typedef struct node t_node;
#endif

/* ARRSIZ: number of nodes to insert into the tree to test */
/* Should be a runtime argv parameter -n 1000 */
/* SUN Ultra 60 2GB RAM: after 262144-something  arrkey[ARRSIZ][LEAF_KEYLEN+1] (Sun Studo & GCC) becomes to large for the stack. Need to allocate it on the heap with malloc(). */
/* See example using setrlimit(2) to increase the heap size during execution: https://www.linuxquestions.org/questions/programming-9/declare-large-arrays-in-c-947092/ */
			    /* #define ARRSIZ 250000 *//* init: sizeof(arrkey[250000][31]) is [7750000] */
/*
 * anything greater than 10 or to your liking disable the ASCII art graph boxes in the Makefile:
 *   DEBUG_LIB_WITH_LIBCALL_TRACING =
 *   DEBUG_LIB_SHOW_LIBCALL_MALLOC_GRAPHS =
 *   DEBUG_LIB_SHOW_LIBCALL_MALLOC_INFO =
 *   DEBUG_LIB_SHOW_LIBCALL_TREE_REBALANCE =
 */
#define ARRSIZ 10
/*#define ARRSIZ 262144*/
/*#define ARRSIZ (256 * 1024)*/
/*#define ARRSIZ (384 * 1024) segmentation fault */

/* output in here will display one less than this, ie MAX_DISPLAY+1 */
#define MAX_DISPLAY 26

static char *RCSid[] = { "$Id$" };

int f(Leaf *, Leaf *);

void reverse(char s[]);
void itoa(int, char s[]);
void Print_Node(Leaf * pl, int level);

extern int bst_errno;
extern void bst_stat(char *);

int main(int argc, char *argv[])
{
    int randnum, i, j, missing, lost;
    int rand1, rand2;
    char cmd[100], tn[] = "t", kn[100], chari[LEAF_KEYLEN + 1];
    Leaf *l, *pk, *pnl;
    char *pc, arrkey[ARRSIZ][LEAF_KEYLEN + 1];
    unsigned int seed;


    printf("init: sizeof(arrkey[%d][%d]) is [%ld]\n\n", ARRSIZ, LEAF_KEYLEN + 1, sizeof(arrkey));

    /*if (bst_create(tn, AVL, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_NO) == FALSE) { */
    /*if (bst_create(tn, AVL, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_YES) == FALSE) { */
    if (bst_create(tn, AVL, sizeof(Leaf), FALSE, f, Print_Node, TREE_VERIFY_NO) == FALSE) {
	printf("   ### unable to create bst tree ###\n\n");
	return 0;
    }
    printf("--- tree defined: '%s' ---\n\n", tn);

    pk = (Leaf *) bst_alloc(tn);


/*
    printf("Enter a integer seed: ");
    scanf("%i", &seed);
    srand(seed);
*/

    srand(time(0));

    /* current test key size is LEAF_KEYLEN char */
    pnl = (Leaf *) bst_alloc(tn);

    printf("----------------------begin insertion of [%d] records ---------------------------\n\n", ARRSIZ);
    for (i = 0; i < ARRSIZ; i++) {
	do {
	    /*randnum = rand(); */
	    rand1 = rand();
	    rand2 = rand();
	    memset(chari, '\0', LEAF_KEYLEN + 1);
	    /*itoa(randnum, chari); */
	    /*sprintf(chari, "%d", randnum); */
	    /* key content formats, two random numbers, period or "decimal" is meaningless, just a seperator */
	    sprintf(chari, "%d%d", rand1, rand2);
	    /* sprintf(chari, "%d.%d", rand1,rand2); */
	    /*sprintf(chari, "%X.%X", rand1,rand2); */
	    /*printf("random number (int) [%d]  as (string) [%s]\n", randnum, chari); */
	    memset(pnl->key, '\0', LEAF_KEYLEN + 1);
	    strncpy(pnl->key, chari, LEAF_KEYLEN);
	} while (bst_put(tn, pnl) == FALSE);

	if (i < MAX_DISPLAY || ARRSIZ - i < MAX_DISPLAY)
	    printf("new node added %10d of %10d, rand1 [%d] rand2 [%d],  (string) key = [%s]\n", i, ARRSIZ, rand1, rand2,
		   pnl->key);
	/*printf("new node added %10d of %10d, randnum (int) [%d],  (string) key = [%s]\n", i, ARRSIZ, randnum, pnl->key); */

	/* save the generated key into our array */
	memset(arrkey[i], '\0', LEAF_KEYLEN + 1);
	strncpy(arrkey[i], chari, LEAF_KEYLEN);
    }
    printf("------------------ end of insertion ---------------------\n\n\n");

    /* check if tree is still in balance after an insertion/rotation */
    /* check it once here since we have it turned off in bst_create(...,FALSE,...) above */
    if (1) {
	printf("--------------------- begin check tree balance of [%d] records ------------------------\n", ARRSIZ);
	bst_stat(tn);
	if (bst_errno != 0)
	    printf("bst_errno = %d\n");
	else
	    printf("success: tree still in balance\n");
	printf("------------------ end of check tree balance ---------------------\n\n\n");
    }

    printf("--------------------- begin find of [%d] records ------------------------\n", ARRSIZ);

    for (i = 0; i < ARRSIZ; i++) {
	memset(pk->key, '\0', LEAF_KEYLEN + 1);
	strcpy(pk->key, arrkey[i]);
	if (i < MAX_DISPLAY || ARRSIZ - i < MAX_DISPLAY)
	    printf("FIND KEY: pk->key = arrkey[%10d] = [%s]\n", i, arrkey[i]);
	if ((l = (Leaf *) bst_get(tn, pk)) == NULL)
	    printf("\007  ### KEY NOT FOUND: '%s' ###\n\n", pk->key);
    }
    printf("------------------- end of find -------------------------\n\n\n");

    printf("------------------ begin tree print of [%d] records -----------------------\n", ARRSIZ);
    if (ARRSIZ < MAX_DISPLAY) {
	bst_print(tn);
	if (bst_errno != 0) {
	    printf("  caused BST error %4d: %s\n", bst_errno, bst_errmsg(bst_errno));
	    printf("\n  ### Can't print tree %s ###\n", tn);
	}
    }
    printf("------------------- end of tree print -------------------------\n\n\n");


    printf("------------------ begin deletion of [%d] records -----------------------\n", ARRSIZ);

    lost = 0;
    for (i = 0; i < ARRSIZ; i++) {
	memset(pk->key, '\0', LEAF_KEYLEN + 1);
	strcpy(pk->key, arrkey[i]);
	if (i < MAX_DISPLAY || ARRSIZ - i < MAX_DISPLAY)
	    printf("DELETE KEY record [%10d] key [%s]\n", i, pk->key);
	if (bst_remove(tn, pk) == FALSE) {
	    printf("\n\n\007### CANNOT DELETE %s ###\n\n", pk->key);
	    lost++;
	}
    }
    printf("----------------------end deletetion----------------------------\n\n\n");

    printf("Number of records added: %d\n", ARRSIZ);
    printf("Number of lost keys: %d\n", lost);
    bst_delete(tn);
    printf("### done ###\n");

}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)
	n = -n;

    i = 0;
    do {
	s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
	s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
	c = s[i];
	s[i] = s[j];
	s[j] = c;
    }
}

int f(Leaf * r1, Leaf * r2)
{
    if (strcmp(r1->key, r2->key) < 0)
	return -1;
    else if (strcmp(r1->key, r2->key) == 0)
	return 0;
    else
	return 1;
}


/*
#define TAB_COL   30


void Print_Node2(Leaf * pl, int level)
{

    t_node *pn;
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
	pn = (t_node *) pl;
	pn--;
	for (j = 1; j < 4 * level + 3; j++)
	    putchar(' ');
	printf("[%s]", pl->key);
	for (j = 4 * level + 5; j < TAB_COL; j++)
	    putchar(' ');
	printf ("(0x%-4x) LVL: %3i, BAL: %2i, TAG: %s (0x%-4x) (0x%-4x) (0x%-4x) \n\n", pn, level, pn->tn_bf, pn->tn_tag == LEFT_SON ? "lson" : "rson", pn->tn_ulink, pn->tn_llink, pn->tn_rlink);
    }
}
*/

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
    typedef
	enum {
	LEFT_SON, ROOT, RIGHT_SON
    } Tags;


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

	printf("[%s]", pl->key);
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
