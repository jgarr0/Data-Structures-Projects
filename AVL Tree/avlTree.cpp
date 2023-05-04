#include "avlTree.h"

/*
 *   AVL Tree ~ Data Structures 3460:316
 *   FILE:   avlTree.cpp
 *   Author: Joseph Garro (4211105)
 *   EMAIL:  jmg289@uakron.edu
 */

// own max function to remove dependence on STL's std::max
namespace
{
    int maxof(int a, int b){
        if(a >= b)
            return a;
        else
            return b;
    }
}

//---------------------------------------
//
//     EXTERNAL   PUBLIC   FUNCTIONS
//
//---------------------------------------

void avlTree::insert(int element_){
    // create new AVL node w/ passed element and utilize default parameters
    avlNode* newNode = new avlNode(element_);
    // insert AVL node into AVL tree
    insertAVLNode(newNode, root);
}

void avlTree::remove(int element_){
    // delete element starting search from root of AVL tree
    deleteAVLNode(element_, root);
}

void avlTree::clear(){
    // remove root over and over again
    while(root)
        deleteAVLNode(root->element, root);
}

bool avlTree::find(int element_){
    // tmpPtr to traverse AVLtree
    avlNode* tempPtr = root;
    while(tempPtr){
        // if value found return true
        if(tempPtr->element == element_){
                     return true;
        }
        // if value < root, know it must be in left subtree by BST property
        else if(element_ < tempPtr->element){
            tempPtr = tempPtr->lc;
        }
        else if(element_ > tempPtr->element){
        // if greater, must be in right subtree by BST property
            tempPtr = tempPtr->rc;
        }
    }
    // return false if not found
    return false;
}

int avlTree::height(){
    return getHeight(root);
}

int avlTree::count(){
    return getCount(root);
}

int avlTree::getMax(){
    // return max value in AVL tree
    return getMaxAVLNode(root)->element;
}

int avlTree::getMin(){
    //return min value in AVL tree
    return getMinAVLNode(root)->element;
}

int avlTree::removeMax(){
    // temp value to hold max
    int temp = getMax();
    // delete max value from AVL tree
    deleteAVLNode(temp, root);
    // return max
    return temp;
}

int avlTree::removeMin(){
    // temp value to hold min
    int temp = getMin();
    // delete min value from AVL tree
    deleteAVLNode(temp, root);
    // return min
    return temp;
}

void avlTree::printPreOrder(){
    // print AVL tree in pre order from root
    preOrderAVLNode(root);
}

//---------------------------------------
//
//     INTERNAL   PRIVATE   FUNCTIONS
//
//---------------------------------------

void avlTree::insertAVLNode(avlNode *&newNode, avlNode *&parentPtr){
    // place at first nullptr found
    if(parentPtr == nullptr)
        parentPtr = newNode;

    // no duplicate values; return if value already exists
    // duplicates can be enabled by commenting this out, but multiple duplicate values do not delete well. The AVL tree balancing still works
    // but the leftmost maximum value is deleted and not the right most value on the LHS.
    else if(newNode->element == parentPtr->element)
       return;

    // otherwise find appropriate location to insert newNode
    // if newNode value less than parentPtr value, insert on LHS of parentPtr
    else if(newNode->element < parentPtr->element)
        insertAVLNode(newNode, parentPtr->lc);
    // otherwise, insert on RHS of parentPtr
    else if(newNode->element >= parentPtr->element)
            insertAVLNode(newNode, parentPtr->rc);

    // update height of parentPtr
    updateHeight(parentPtr);
    // check for unbalance after inserting new node. Subtrees are unbalanced if the difference between their heights is 2
    rebalance(getBalanceFactor(parentPtr), parentPtr);
}

// recursive deletion of AVL nodes adapted from geeksforgeeks.org (https://www.geeksforgeeks.org/avl-tree-set-2-deletion/)
// and from Harish R (https://gist.github.com/Harish-R/097688ac7f48bcbadfa5)
avlTree::avlNode* avlTree::deleteAVLNode(int element_, avlNode *&parentPtr){
    // used for getting largest value
    avlNode* tempPtr = nullptr;

    // stop deletion + unwind stack if parentPtr == nullptr
    if(parentPtr == nullptr)
        return nullptr;

    // find element recursively so path to reach it can be retraced back to root to rebalance AVL tree
    // set lc of parentPtr to deletion of its left side if value of lc is less than element. Otherwise,
    // set rc of parenPtr to deletion of its right side
    else if(element_ < parentPtr->element)
        parentPtr->lc = deleteAVLNode(element_, parentPtr->lc);
    else if(element_ > parentPtr->element)
        parentPtr->rc = deleteAVLNode(element_, parentPtr->rc);

    // if 2 children, largest child on LHS becomes new parent
    else if(parentPtr->rc && parentPtr->lc){
        // find largest value on LHS
        tempPtr = getMaxAVLNode(parentPtr->lc);
        // set parentPtr to largest value on its LHS
        parentPtr->element = tempPtr->element;
        // delete max value from LHS by setting LHS of parentPtr equal top the deletion of itself and the max value
        parentPtr->lc = deleteAVLNode(parentPtr->element, parentPtr->lc);
    }
    // if right child does not exist, parent is set to left child
    else if(!parentPtr->rc)
            parentPtr = parentPtr->lc;

    // if left child does not exist, parentPtr set to right child
    else if(!parentPtr->lc)
        parentPtr = parentPtr->rc;

    // if nullptr, return because nullptr's do not have a height
    if(parentPtr == nullptr)
        return parentPtr;

    // update parentPtr height
    updateHeight(parentPtr);
    // now check for imbalance of subtrees
    rebalance(getBalanceFactor(parentPtr), parentPtr);

    // return parentPtr to repeat again
    return parentPtr;
    }

int avlTree::getBalanceFactor(avlNode *&parentPtr){
    return (getHeight(parentPtr->lc) - getHeight(parentPtr->rc));
}

void avlTree::rebalance(int balanceFactor, avlNode *&parentPtr){
    // if balance factor is 2: unbalanced and LHS is larger
    if(balanceFactor == 2)
    {
        //2 possible rotations from here: single right or left right
        // single right if balance factor of parentPtr->lc >= 0, otherwise leftRight
        if(getBalanceFactor(parentPtr->lc) >= 0)
            singleRightRotation(parentPtr);
        else
            leftRightRotation(parentPtr);
    }
    // if balance factor is -2: unbalanced and RHS is larger
    if(balanceFactor == -2){
        //2 possible rotations from here: single left or right left
        // single left if balance factor of parentPtr->lc <= 0, otherwise rightLeft
        if(getBalanceFactor(parentPtr->rc) <= 0)
            singleLeftRotation(parentPtr);
        else
            rightLeftRotation(parentPtr);
    }
}

void avlTree::singleRightRotation(avlNode *&parentPtr){
    // get left child of parentPtr
    avlNode* tempLC = parentPtr->lc;
    // get right child of left child of parentPtr
    avlNode* tempLCRC = tempLC->rc;
    // set right child of left child of parentPtr to parentPtr
    tempLC->rc = parentPtr;
    // copy rhs of tempLc to lhs of new rightchild
    tempLC->rc->lc = tempLCRC;
    // update heights
    updateHeight(parentPtr);
    updateHeight(tempLC);
    // new parentPtr = tempLC == root of balanced subtree
    parentPtr = tempLC;
}

void avlTree::singleLeftRotation(avlNode *&parentPtr){
    // get right child of parentPtr
    avlNode* tempRC = parentPtr->rc;
    // get left child of right child of parentPtr
    avlNode* tempRCLC = tempRC->lc;
    // set left child of right child of parentPtr to parentPtr
    tempRC->lc = parentPtr;
    // copy lhs of tempRC to rhs of new leftchild
    tempRC->lc->rc = tempRCLC;
    // update heights
    updateHeight(parentPtr);
    updateHeight(tempRC);
    // new parentPtr = tempRC == root of balanced subtree
    parentPtr = tempRC;
}

void avlTree::leftRightRotation(avlNode *&parentPtr){
    // single left rotation around left child of parentPtr
    singleLeftRotation(parentPtr->lc);
    // single right rotation around parentPtr
    singleRightRotation(parentPtr);
}

void avlTree::rightLeftRotation(avlNode *&parentPtr){
    // single right rotation around right child of parentPtr
    singleRightRotation(parentPtr->rc);
    // single left rotation around parentPtr
    singleLeftRotation(parentPtr);
}

void avlTree::updateHeight(avlNode *&parentPtr){
    // set new height to the max of the subtrees + 1
    parentPtr->nHeight = maxof(getHeight(parentPtr->lc), getHeight(parentPtr->rc)) + 1;
}

void avlTree::preOrderAVLNode(avlNode *&rootPtr){
    // starting at the rootPtr, while it exists
    if(rootPtr){
        // output node's value
        std::cout << rootPtr->element;
        // if left child is nullptr, output '/'
        if(rootPtr->lc == nullptr)
            std::cout << '/';
        // if not a nullptr, call again on LHS of parent
        else
            preOrderAVLNode(rootPtr->lc);
        // if RHS of parent is nullptr, output '/'
        if(rootPtr->rc == nullptr)
            std::cout << '/';
        // if not a nullptr, call again on RHS of parent
        else
            preOrderAVLNode(rootPtr->rc);
    }
}

int avlTree::getHeight(avlNode *&parentPtr){
    // if nullptr, nHeight = -1
    if(parentPtr == nullptr)
        return -1;

    else
        // otherwise return height stored in specified node
        return parentPtr->nHeight;
}

int avlTree::getCount(avlNode *&parentPtr){
    // do not count nullptrs; nullptrs = stop condition for recursion
    if(parentPtr == nullptr)
        return 0;
    else
        // sum up number of nodes recursively
        return 1 + getCount(parentPtr->rc) + getCount(parentPtr->lc);
}

avlTree::avlNode* avlTree::getMaxAVLNode(avlNode *&parentPtr){
    // modify tempPtr and not parent
    avlNode* tempPtr = parentPtr;
    // while tempPtr has a right child, set it equal to it
    while(tempPtr->rc)
        tempPtr = tempPtr->rc;
    // return a pointer to the maximum node
    return tempPtr;
}

avlTree::avlNode* avlTree::getMinAVLNode(avlNode *&parentPtr){
    // modify tempPtr and not parent
    avlNode* tempPtr = parentPtr;
    // while tempPtr has a left child, set it equal to it
    while(tempPtr->lc)
        tempPtr = tempPtr->lc;
    // return a pointer to the minimum node
    return tempPtr;
}
