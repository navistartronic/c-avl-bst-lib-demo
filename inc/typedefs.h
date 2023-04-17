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

/* private library modules include this file for use of libbst.a */

typedef struct header t_header;
typedef struct node t_node;

typedef
    enum {
    FALSE,
    TRUE
} Boolean;

typedef
    enum {
    T_HEADER,
    T_NODE
} MallocTypes;

typedef
    enum {
    CHAIN,
    FREE
} FreeOpts;

typedef
    enum {
    AVL,
    BST
} BstType;

typedef
    enum {
    TREE_VERIFY_NO,
    TREE_VERIFY_YES
} TreeVerifyType;

typedef
    enum {
    DELETE,
    COPY,
    VISIT,
    IDENT,
    EQUAL
} TWalkOps;

typedef
    enum {
    INORDER,
    PREORDER,
    POSTORDER
} Traversals;

typedef
    enum {
    LEFT_SON,
    ROOT,
    RIGHT_SON
} Tags;

typedef
    enum {
    LEFT_SIDE,
    RIGHT_SIDE
} Sides;

typedef
    enum {
    LEFT_ROTATION,
    RIGHT_ROTATION
} Rotations;
