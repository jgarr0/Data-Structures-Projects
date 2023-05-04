#ifndef AVLTREE_H
#define AVLTREE_H
#include <algorithm>
#include <iostream>

/*
 *   AVL Tree ~ Data Structures 3460:316
 *   FILE:   avlTree.h
 *   AUTHOR: Joseph Garro (4211105)
 *   EMAIL:  jmg289@uakron.edu
 */

class avlTree
{
    public:

        avlTree(){                                                              // constructor
            root = nullptr;                                                     // create new AVL tree + initialize root to nullptr
        }

        ~avlTree(){                                                             // deconstructor
            // destroy values in tree
            clear();
            // delete root
            delete root;
        }

        void insert(int);
        // pre:  need an existing AVL tree
        // post: inserts a new node containing the passed value to the appropriate location in the AVL tree

        void remove(int);
        // pre:  need an existing AVL tree
        // post: removes node with matching key from AVL tree and rebalances if necessary

        void clear();
        // pre:  need an existing AVL tree
        // post: removes all nodes from AVL tree. Resulting height is -1

        bool find(int);
        // pre:  need an existing AVL tree
        // post: returns true if an element is in the AVL tree. Otherwise returns false

        int height();
        // pre:  need an existing AVL tree
        // post: returns the total height of the tree. The height of an empty tree is -1 and the height
        //       of a tree with only a root node is 0. Otherwise, the height is the maximum height of the
        //       roots two possible subtrees

        int count();
        // pre:  need an existing AVL tree
        // post: returns the number of nodes in the AVL tree

        int getMax();
        // pre:  need an existing AVL tree
        // post: returns the maximum value in the AVL tree WITHOUT deleting it

        int getMin();
        // pre:  need an existing AVL tree
        // post: returns the minimum value in the AVL tree WITHOUT deleting it

        int removeMax();
        // pre:  need an existing AVL tree
        // post: returns the maximum value in the AVL tree and deletes it

        int removeMin();
        // pre:  need an existing AVL tree
        // post: returns the minimum value in the AVL tree and deletes it

        void printPreOrder();
        // pre:  need an existing AVL tree
        // post: prints the contents of the AVL tree using a preorder traversal from the root of the entire AVL tree.
        //       The contents of the AVL tree are printed to the console in sequential format, with null pointers in
        //       the AVL tree indicated by '/'


    private:

        // nodes that compose the AVL tree
        // each node has a data value, an integer that corresponds to its height in the tree
        // and two pointers- 1 to its left child and 1 to its right child
        struct avlNode{
            int element;                                                        // data value
            int nHeight;                                                        // stores the height of the node
            avlNode* lc;                                                        // ptr to left child
            avlNode* rc;                                                        // ptr to right child

            // constructor to create avlNodes
            // Used for root node creation; Default parameters are values for node inserted into root
            avlNode(int element_ = 0, int nHeight_ = 0, avlNode* rc_ = nullptr, avlNode* lc_ = nullptr){
                element = element_;
                nHeight = nHeight_;
                lc = lc_;
                rc = rc_;
            }

            // destructor to destroy avlNodes
            ~avlNode(){
                // delete left and right pointers
                delete lc;
                delete rc;
            }
        };

        avlNode* root;                                                          // ptr to root of AVL tree

        void insertAVLNode(avlNode *&, avlNode *&);
        // pre:  existing AVL tree
        // post: inserts the first avlNode (child) parameter at the avlNode indicated by the second parameter (parent). The new
        //       avlNode is placed into the proper position relative to the parent and the AVL tree is rebalanced if necessary.

        avlNode* deleteAVLNode(int, avlNode *&);
        // pre:  existing AVL tree
        // post: removed the avlNode (child) with the indicated integer value from the tree with whose root is indicated by the avlNode
        //       parameter. Once removed, the AVL tree is rebalanced if necessary

        int getBalanceFactor(avlNode *&);
        // pre:  existing AVL tree
        // post: returns the difference in height (balance factor) between the passed avlNode's left child and right child
        //       possible values:
        //           2   =   left child > right child; unbalanced
        //           1   =   left child > right child; balanced
        //           0   =   left child and right child have even heights
        //          -1   =   left child < right child; balanced
        //          -2   =   left child < right child; unbalanced

        void rebalance(int , avlNode *&);
        // pre:  existing AVL tree
        // post: rebalances the indicated node according to the passed integer value = balance factor.
        //       if the balance factor is 2 or -2, the node is rebalanced.


        void singleRightRotation(avlNode *&);
        // pre:  existing AVL tree
        // post: performs a single right rotation around the passed avlNode == root of rotation
        //
        //            START                             RESULT
        //
        //              5  (ROOT)
        //             / \                                3
        //   (PIVOT)  3   A              |\              / \
        //           / \             ----| \            2   5
        //          2   B            ----| /           / \ / \
        //         / \                   |/           D  C B  A
        //        D   C

        void singleLeftRotation(avlNode *&);
        // pre:  existing AVL tree
        // post: performs a single left rotation around the passed avlNode == root of rotation
        //
        //            START                             RESULT
        //
        //              3  (ROOT)
        //             / \                                5
        //            A   5  (PIVOT)     |\              / \
        //               / \         ----| \            3   7
        //              B   7        ----| /           / \ / \
        //                 / \           |/           A  B C  D
        //                C   D

        void leftRightRotation(avlNode *&);
        // pre:  existing AVL tree
        // post: performs a single left rotation around the left child of the parent and then a single right rotation
        //       around the parent to rebalance the AVL tree
        //
        //            START                                                       RESULT
        //
        //              5              LEFT               5  (ROOT)   RIGHT
        //             / \                               / \                        4
        //     (ROOT) 3   A              |\    (PIVOT)  4   A          |\          / \
        //           / \             ----| \           / \         ----| \        3   5
        //          B   4 (PIVOT)    ----| /          3   D        ----| /       / \ / \
        //             / \               |/          / \               |/       B  C D  A
        //            C   D                         B   C

        void rightLeftRotation(avlNode *&);
        // pre:  existing AVL tree
        // post: performs a single right rotation around the right child of the parent and then a single left rotation
        //       around the parent to rebalance the AVL tree
        //
        //            START                                                       RESULT
        //
        //              3              RIGHT              3  (ROOT)    LEFT
        //             / \                               / \                         4
        //            A   5  (ROOT)      |\    (PIVOT)  A   4           |\          / \
        //               / \         ----| \               / \      ----| \        3   5
        //     (PIVOT)  4   B        ----| /              D   5     ----| /       / \ / \
        //             / \               |/                  / \        |/       A  D C  B
        //            D   C                                 C   B

        void updateHeight(avlNode *&);
        // pre:  existing AVL tree
        // post: updates the height of the passed avlNode

        void preOrderAVLNode(avlNode *&);
        // pre:  existing AVL tree
        // post: prints the pre order traversal in sequential format from the passed AVL node

        int getHeight(avlNode *&);
        // pre:  existing AVL tree
        // post: returns the height of the passed AVL node

        int getCount(avlNode *&);
        // pre:  existing AVL tree
        // post: returns the number of nodes the the passed AVL node is a parent of

        avlNode* getMaxAVLNode(avlNode *&);
        // pre:  existing AVL tree
        // post: returns a pointer to the avlNode with the maximum value relative to the passed avlNode

        avlNode* getMinAVLNode(avlNode *&);
        // pre:  existing AVL tree
        // post: returns a pointer to the avlNode with the minimum value relative to the passed avlNode
};

#endif // AVLTREE_H
