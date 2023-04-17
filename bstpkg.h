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

/* userland programs include this file for use of libbst.a */

extern int bst_errno;

typedef enum { FALSE, TRUE } Boolean;
typedef enum { AVL, BST } BstType;
typedef enum { TREE_VERIFY_NO, TREE_VERIFY_YES } TreeVerifyType;


extern void *bst_alloc(char *);
extern Boolean bst_copy(char *, char *);
extern int bst_count(char *);
extern Boolean bst_create(char *, int, int, int, int (*)(Leaf *, Leaf *), void (*prntf) (Leaf *, int), int);
extern Boolean bst_defined(char *);
extern Boolean bst_delete(char *);
extern Boolean bst_empty(char *);
extern char *bst_errmsg(int);
extern Boolean bst_equal(char *, char *);
extern void *bst_get(char *, void *);
extern Boolean bst_ident(char *, char *);
extern void *bst_node(char *);
extern void bst_print(char *);
extern Boolean bst_put(char *, void *);
extern void bst_rprint(char *);
extern Boolean bst_remove(char *, void *);
extern Boolean bst_release(char *, void *);
extern void bst_stat(char *);	/* debugging purposes only; remove when done */

/* TODO extern void     bst_trees  (void); *//* return array of defined trees */
/* TODO extern char[] bst_treewalk(tn, treeorder,userfunction); */
/* perform inorder/preorder/postorder on each node then calling user defined function */
