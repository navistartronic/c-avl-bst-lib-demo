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

/* Error numbers for global variable bst_errno set in the routines */

#define  BST_ERR_RESET                    0	/* no error occured            */
#define  BST_ERR_TREE_NOT_DEFINED       101	/* no such tree                */
#define  BST_ERR_TREE_NODE_MISMATCH     102	/* wrong node passed for tree  */
#define  BST_ERR_MALLOC                 103	/* no memory available         */
#define  BST_ERR_LEAFNODE_SIZE_ZERO     104	/* doesn't make sense          */
#define  BST_ERR_CALLOC                 105	/* no memory available         */
#define  BST_ERR_STACK_FULL             106	/* I doubt it                  */
#define  BST_ERR_STACK_EMPTY            107	/* again I doubt it            */
#define  BST_ERR_OUT_OF_BALANCE         108	/* tree not balanced anymore   */
#define  BST_ERR_NO_UPF_GIVEN           109	/* no upf for tree printing    */
#define  BST_ERR_NO_UCF_GIVEN           110	/* no ucf for node compare     */
#define  BST_ERR_TREE_ALREADY_DEFINED   111	/* duplicate tree creation     */
#define  BST_ERR_UCF_EQUALS_UPF         112	/* compare f. same as print f. */
#define  BST_ERR_KEY_NOT_FOUND          113	/* key not in tree             */
#define  BST_ERR_DUPLICATE_KEY          114	/* key already in tree         */
#define  BST_ERR_TAG                    115	/* node has wrong tag          */
#define  BST_ERR_NAME_LEN               116	/* tree name length problems   */
#define  BST_ERR_COPY_TO_DEFINED        117	/* copying to a defined tree   */
#define  BST_ERR_COPY_FROM_NON_EXISTANT 118	/* copy from not defined       */
#define  BST_ERR_COPY_CNT               119	/* copy counts wrong           */
#define  BST_ERR_FIRST_TREE_UNDEF       120	/* which tree is undefined     */
#define  BST_ERR_SECOND_TREE_UNDEF      121	/* which tree is undefined     */
#define  BST_ERR_TREES_NOT_SAME_FAMILY  122	/* which tree is undefined     */
#define  BST_ERR_NAME_LEN_T1            123	/* tree name length problems   */
#define  BST_ERR_NAME_LEN_T2            124	/* tree name length problems   */
#define  BST_ERR_UKNOWN_BST_TYPE        125	/* tree type is not AVL or BST */
