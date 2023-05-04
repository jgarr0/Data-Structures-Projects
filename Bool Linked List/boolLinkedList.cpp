#include "boolLinkedList.h"
#include <iostream>
#include <assert.h>

boolLinkedList::boolLinkedList()
{
        headPtr = nullptr;                                                                                                                      // initialize headPtr to nullptr
        nodeCount = 0;                                                                                                                          // nodeCount == 0 -> empty boolLinkedList
}

boolLinkedList::boolLinkedList(std::initializer_list<int> boolList)
{
        nodeCount = 0;
        headPtr = nullptr;
        auto boolListItr = boolList.begin();
        for(unsigned long long int i = 0; i < boolList.size(); ++i)                                                                             // add boolListNode with initialization list data == size of initialization list
        {
            addBoolListNode(*boolListItr);                                                                                                      // add boolListNode containing copy of data of corresponding element in initialization list
            boolListItr++;
        }
}

boolLinkedList::~boolLinkedList()
{
    boolListNode *nodePtr;
    boolListNode *nextPtr;

    nodePtr = headPtr;                                                                                                                          // assign nodePtr to headPtr
    while(nodePtr != nullptr)                                                                                                                   // assign next to nodePtr->next
    {                                                                                                                                           // until last element reached, delete nodePtr and reassign it to next
        nextPtr = nodePtr->nextPtr;
        delete nodePtr;
        nodePtr = nextPtr;
    }
    headPtr = nullptr;                                                                                                                          // nobody likes dangling pointers
}

void boolLinkedList::addBoolListNode(unsigned long long int boolRep)
{

    boolListNode *newNode;
    boolListNode *nodePtr;

    newNode = new boolListNode;                                                                                                                 // create new node
    newNode->compactedBools = boolRep;                                                                                                          // assign bool representation to node
    newNode->nextPtr = nullptr;                                                                                                                 // point to nothing

    // check for empty linked list
    if(headPtr == nullptr)                                                                                                                      // if empty list, newNode becomes head ndoe
               headPtr = newNode;
    else                                                                                                                                        // if not empty, iterate through list + add newNode to end
    {
        nodePtr = headPtr;
        while(nodePtr->nextPtr)
            nodePtr = nodePtr->nextPtr;
        nodePtr->nextPtr = newNode;
    }
    nodeCount++;                                                                                                                                // increment nodeCount
}

void boolLinkedList::insertBoolListNode(unsigned long long int nodePlace, unsigned long long int boolRep)
{
    // create new node
    boolListNode *newNode;

    newNode = new boolListNode;                                                                                                                 // create new node
    newNode->compactedBools = boolRep;                                                                                                          // assign bool representation to node
    newNode->nextPtr = nullptr;                                                                                                                 // point to nothing

    // insert new node
    if(headPtr == nullptr)                                                                                                                      // if empty linked list, make inserted node the head
        headPtr = newNode;
    // if not empty and place = 0, make new node the head
    else if((nodePlace == 0) || (nodePlace == 1))
    {
        boolListNode* tempPtr = headPtr;                                                                                                        // store old headPtr
        headPtr = newNode;                                                                                                                      // update headPtr to new node
        newNode->nextPtr = tempPtr;                                                                                                             // new node points to old headPtr
    }
    // if place > #nodes, append to end
    else if(nodePlace > nodeCount)
    {
        boolListNode* tempPtr = headPtr;                                                                                                        // store old headPtr
        while(tempPtr->nextPtr)                                                                                                                 // find end of linked list
            tempPtr = tempPtr->nextPtr;
        tempPtr->nextPtr = newNode;
    }
    // else, insert accordingly
    else
    {
        boolListNode* tempPtr = headPtr;
        // find node before desired insert position
        for(unsigned long long int i = 1; i < (nodePlace-1); ++i)                                                                               // find pos before location to insert new element
            {
                tempPtr = tempPtr->nextPtr;
            }
            newNode->nextPtr = tempPtr->nextPtr;                                                                                                // assign newNode-> next to point to node after pos to insert
            tempPtr->nextPtr = newNode;                                                                                                         // position before insert points to newNode
    }
    nodeCount++;                                                                                                                                // increment nodeCount
}

void boolLinkedList::deleteBoolListNode(unsigned long long int nodePos)
{
    assert((nodePos <= nodeCount) && (nodePos > 0));
    boolListNode* nodePtr;                                                                                                                      // temp ptr
    boolListNode* prvPtr;                                                                                                                       // temp ptr for previous node

    // can't delete an empty linkedlist
    if(headPtr == nullptr)
        return;
    else
    {
        // can't delete a node that does not exist
        if(nodePos > nodeCount)
            return;
        // if deleting head node
        else if(nodePos == 1)
        {
            nodePtr = headPtr->nextPtr;                                                                                                         // temp ptr = ptr after 1st node
            delete headPtr;                                                                                                                     // delete headptr and reassign to temp ptr
            headPtr = nodePtr;
        }
        // if deleting last node
        else if(nodePos == nodeCount)
        {
            nodePtr = headPtr;
            while(nodePtr->nextPtr)                                                                                                             //set nodePtr to last element
            {
                prvPtr = nodePtr;                                                                                                               //prvPtr = 2nd to last element
                nodePtr = nodePtr->nextPtr;                                                                                                     //nodePtr = last element
            }
            delete nodePtr;                                                                                                                     // delete last node
            prvPtr->nextPtr = nullptr;                                                                                                          // nobody likes dangling pointers

        }

        // if deleting any node in between
        else
        {
            nodePtr = headPtr;                                                                                                                  // start at headPtr
            for(unsigned long long int i = 1; i < nodePos; ++i)                                                                                 // find node before node to delete
            {
                prvPtr = nodePtr;                                                                                                               // prvPtr = old nodePtr
                nodePtr = nodePtr->nextPtr;                                                                                                     // increment nodePtr to next node
            }
            if(nodePtr == nullptr)
                delete (prvPtr->nextPtr);
            else
            {
                prvPtr->nextPtr = nodePtr->nextPtr;                                                                                             // prvPtr points to node after nodePtr
                delete nodePtr;                                                                                                                 // delete nodePtr
            }
        }
        nodeCount--;                                                                                                                            // decrement nodeCount to reflect deleted node
    }
}

unsigned long long int boolLinkedList::getNodeCount()
{
    return nodeCount;
}

void boolLinkedList::printBools()
{
    boolListNode* nodePtr = headPtr;
    unsigned long long int i = 1;
    while(nodePtr)                                                                                                                              // starting at headPtr, print all ints stored in boolLinkedList
    {
        std::cout << nodePtr->compactedBools << std::endl;
        ++i;
        nodePtr = nodePtr->nextPtr;
    }
}

unsigned long long int boolLinkedList::getBool(unsigned long long int nodeIndex)
{
    // assert on valid index
    assert((nodeIndex <= nodeCount) && (nodeIndex > 0));
    boolListNode* nodePtr = headPtr;
    // read from headPtr if 1st index
    if(nodeIndex == 1)
        return nodePtr->compactedBools;
    // otherwise, iterate through boolLinkedList to correct node + return int
    else
    {
        unsigned long long int i = 1;
        while(i < nodeIndex)
        {
            ++i;
            nodePtr = nodePtr->nextPtr;
        }
        return nodePtr->compactedBools;
    }
}

int boolLinkedList::bitShiftCompactedBool(unsigned long long int compactedBool, int boolPosition)
{
    assert((boolPosition <= boolListNode::boolCount) && (boolPosition > 0));                                                                    // assert on valid range
    int tmpBool = compactedBool;                                                                                                                // backup of int in node
    return((tmpBool >> (boolListNode::boolCount - boolPosition)) & boolListNode::boolMask);                                                     // left shift pool # times = (boolCount - pos of bool) and & to mask of 1 to return if 1 or 0
}

int boolLinkedList::isolateBool(unsigned long long int nodeIndex, int boolPosition)
{
    return bitShiftCompactedBool(getBool(nodeIndex), boolPosition);                                                                             // use bitShiftCompactedBool w/ args of getBool(index) and desired pos in bit
}

void boolLinkedList::setBoolAtPos(unsigned long long int nodeIndex, unsigned long long int newBool)
{
    assert((nodeIndex > 0) && (nodeIndex <= nodeCount));                                                                                        // aassert if invalid pos
    boolListNode* nodePtr = headPtr;
    if(nodeIndex == 1)                                                                                                                          // if pos == 1; reassign headPtr
        nodePtr->compactedBools = newBool;
    else                                                                                                                                        // otherwise, iterate through to proper node + reassign it
    {
        unsigned long long int i = 1;
        while(i < nodeIndex)
        {
            ++i;
            nodePtr = nodePtr->nextPtr;
        }
        nodePtr->compactedBools = newBool;
    }
}

void boolLinkedList::modifyBoolAtPos(unsigned long long int nodeIndex, int boolPosition, bool newBool)
{
    assert(((nodeIndex > 0) && (nodeIndex <= nodeCount)) && ((boolPosition <= boolListNode::boolCount) && (boolPosition > 0)));                                     // IMPORTANT: bool 1 = left most bool, bool 8 = right most bool
    unsigned long long int boolBackup = getBool(nodeIndex);                                                                                                         // backup bools currently in node
    if(newBool == 1)                                                                                                                                                // if setting bit to a 1
    {                                                                                                                                                               // right shift a constant 1 to the position corresponding to the bit in the binary representation of the int
        setBoolAtPos(nodeIndex, (1 << (boolListNode::boolCount - boolPosition))|boolBackup);                                                                        // then 'or' the right shifted 1 with the boolBackup int to produce a new int with the 1 included in it
    }
    else                                                                                                                                                            // if setting a bit to a 0
    {
        unsigned long long int lhsBool = ((boolBackup >> (boolListNode::boolCount - boolPosition + 1))<<(boolListNode::boolCount - boolPosition + 1));              // isolate bits to left of bit changed to 0 by performing a right shift, and then shift back an equal amount to the right to preserve the bit width of the number
        unsigned long long int copyMask, rhsBool;                                                                                                                   // new ints to copy values to the right of the bit changed to 0
        if(boolPosition == boolListNode::boolCount)                                                                                                                 // if changing left most bit to a 0, equivalent to subtracting 1, so set rhsBool to -1 to add to lhsBool later
            rhsBool = -1;
        else                                                                                                                                                        // if changing a different bit, need to create a mask to copy bits to the right of value to change
        {
            copyMask = boolListNode::boolMask;                                                                                                                      // copymask == constant '1'
            for(unsigned long long int i = 1; i < (boolListNode::boolCount - boolPosition); ++i)                                                                    // creats a mask of constant '1's used to copy values to the right of the bool that was changed
                copyMask = ((copyMask << 1) + 1);
        }
        rhsBool = (boolBackup & copyMask);                                                                                                                          // and the backup bool with the mask of '1's to create the bool for bit's to the right of the bit that was changed
        setBoolAtPos(nodeIndex, (lhsBool + rhsBool));                                                                                                               // use setBoolAtPos w/ passed args, bool to set = addition of RHS + LHS masks
    }
}

void boolLinkedList::recursiveBitShift(unsigned long long int bitPosMask, unsigned long long int compactedBool)
{
    if(bitPosMask == (boolListNode::boolMask << (boolListNode::boolCount - 1)))                                                                 // once at RHS
    {                                                                                                                                           // output RHS with 1 bit shifted to be in line with RHS
        std::cout << (compactedBool & bitPosMask)/bitPosMask;                                                                                   // cout unwinds values from here, displaying binary w/ highest power of 2 at the left and lowest power of 2 at the right
    }
    else
    {
        recursiveBitShift((bitPosMask<<1), compactedBool);                                                                                      // while not at RHS, call recursiveBitShift again w/ mask shifted left 1 bit
        std::cout << (compactedBool & bitPosMask)/bitPosMask;                                                                                   // cout stacks values from RHS-1 to LHS to output binary equivalent of number stored in node
    }
}

void boolLinkedList::displayNodeBinary(unsigned long long int nodeIndex)
{
    recursiveBitShift(boolListNode::boolMask, getBool(nodeIndex));
}

void boolLinkedList::displayNodeBinaryFormatted(unsigned long long int nodeIndex)
{
    std::cout << "----------------------\n"
                 "|   Node " << nodeIndex << " Binary:   |\n"
                 "|      ";
                 displayNodeBinary(nodeIndex);
    std::cout << "      |\n"
                 "----------------------\n";
}

void boolLinkedList::displayListBinary()
{
    boolListNode* nodePtr = headPtr;
    while(nodePtr)
    {
        recursiveBitShift(boolListNode::boolMask, nodePtr->compactedBools);
        nodePtr = nodePtr->nextPtr;
    }
}

void boolLinkedList::displayListBinaryFormatted()
{
    for(unsigned long long int i = 1; i <= nodeCount; ++i)
    {
    std::cout <<
                 "----------------------\n"
                 "|   Node " << i << " Binary:   |\n"
                 "|      ";
                 displayNodeBinary(i);
    std::cout << "      |\n"
                "----------------------\n";
    }
}

void boolLinkedList::flipBits()
{
    boolListNode* nodePtr = headPtr;                                                                                                            // ptr to headptr
    unsigned long long reverseMe = 0;                                                                                                           // unsigned long long is used as a mask to provide support up to 64 bits
    long long int internalNodeCount = 1;                                                                                                                  // internal node counter used as index for setBoolAtPos function
    while(nodePtr)
    {
        reverseMe = ~(nodePtr->compactedBools);                                                                                                 // assign inverse of node's int to reverseMe = 64 bit number
        unsigned long long mask = 1;                                                                                                            // mask initialized to 1 so that powers of 2 equal to length of node's int in bits may be added to it to create a mask
        for(unsigned long long i = 0; i < (sizeof(reverseMe) * 8); ++i)                                                                         // whose size is the same as reverseMe (64 bits), with unmasked bits being leading '0's
        {
           if(i < (boolListNode::boolCount-1))                                                                                                  // add powers of 2 to mask relative to max value of int storing bools
                mask += (2 << i);
        }
        //setBoolAtPos(internalNodeCount, (mask & reverseMe));                                                                                  // at the node determined by internalNodeCounter, set the int obtained by anding reverseMe and mask
        nodePtr->compactedBools = (mask & reverseMe);                                                                                           // optimized setting of new value
        nodePtr = nodePtr->nextPtr;                                                                                                             // this creates an integer whose value is that of the inverse of the original value
        //internalNodeCount++;                                                                                                                    // increment internalNodeCounter + nodePtr to repeat operation
    }
}
