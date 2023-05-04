#ifndef BOOLLINKEDLIST_H
#define BOOLLINKEDLIST_H
#include <initializer_list>

class boolLinkedList
{
    public:
        boolLinkedList();                                                                               // default constructor
        boolLinkedList(std::initializer_list<int>);                                                     // initialize with an initialization list
        ~boolLinkedList();                                                                              // deconstructor

        void addBoolListNode(unsigned long long int);                                                   // add a node whose int value corresponds to binary bools
        // pre: none
        // post: qdds a new binaryListNode with the passed integer value to the end of the binaryLinkedList

        void insertBoolListNode(unsigned long long int, unsigned long long int);                        // insert a new node at a desired place in the linked list. Ex an arg of 2 means this new node is the 2nd node.
        // pre: none
        // post: adds a new binaryListNode at the specified position. If the boolLinkedList is empty,
        //       the inserted boolListNode becomes the head

        void deleteBoolListNode(unsigned long long int);                                                // delete node from linked list
        // pre: existing boolLinkedList + valid node index
        // post: deletes specified boolListNode

        unsigned long long int getNodeCount();                                                          // return nodeCount
        // pre: existing boolLinkedList
        // post: returns number of nodes in boolLinkedList

        void printBools();                                                                              // print all condensed bool
        // pre: existing boolLinkedList
        // post: prints to console all compactedBools in boolLinkedList

        unsigned long long int getBool(unsigned long long int);                                         // return bool stored in specific node
        // pre: existing boolLinkedList + valid node position
        // post: returns condensedBools at specified node

        int bitShiftCompactedBool(unsigned long long int, int);                                         // shift provided number to isolate bit of bool in provided position
        // pre: boolCount and number of ints to shift it
        // post: returns shifted version of boolCount anded with 1 to determine value of that bit

        int isolateBool(unsigned long long int, int);                                                   // isolate single bool in desired place from a node
        // pre: valid boolCount + position in boolCount
        // post: calls bitShiftCompactedBool w/ passed arguments and returns value of desired bit

        void setBoolAtPos(unsigned long long int, unsigned long long int);                              // set new collection of bools in specified node
        // pre: valid node index + int
        // post: reassigns compactedBool at specified node to new int argument

        void modifyBoolAtPos(unsigned long long int, int, bool);                                        // modify individual bool in a node
        // pre: valid node index, position in node, and bool value
        // post: changes indicated bool at specifed node to new value

        void recursiveBitShift(unsigned long long int, unsigned long long int);                         // recursively determine binary content of node
        // pre: mask and integer of equal length
        // post: recursively calls itself by shifting the mask over 1 towatds the right each time
        //       once the end is reach, it unwinds and outputs the binary representation of compactedBools

        void displayNodeBinary(unsigned long long int);                                                 // print binary stored in a node
        // pre: valid node index
        // post: displays binary representation of compactedBools in specified node

        void displayNodeBinaryFormatted(unsigned long long int);                                        // print binary stored in a node + node index and formatting
        // pre: valid node index
        // post: displays binary representation of compactedBools in specified node w/ some formatting + indicate node index

        void displayListBinary();                                                                       // print binary for each node in boolLinkedList
        // pre: boolLinkedList object
        // post: displays binary equivalent of compactedBools in each boolListNode in the boolLinkedList

        void displayListBinaryFormatted();                                                              // print binary for each node in boolLinkedList, indicate node index, + format it
        // pre: boolLinkedList object
        // post: displays binary equivalent of compactedBools in each boolListNode in
        //       the boolLinkedList + indicates node index

        void flipBits();                                                                                // invert all bool values in binaryLinkedList
        // pre: boolLinkedList object
        // post: the value of ocmpactedBools in each node is inverted, changing all 1's to 0's and 0's to 1's

    private:
        struct boolListNode                                                         // structure for nodes of linked list
        {
                // configuration settings for linked list nodes
                // boolCount stores the number of ints that number stored in the node will represent
                // boolMask is a simple mask whose value is 1 (The zeros are to visualize how many bools are storted in the int).
                // boolMask is used to simplify internal operations performed on node values.
                // Possible boolCount values are powers of 2, which controls the bit field width of the compactedBools variable.
                //   /--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\
                //   |           |      |      |          |              |                        |                                    |                                                                    |
                //   | boolCount |  1   |  2   |    4     |     8        |         16             |                 32                 |                                64                                  |
                //   |           |      |      |          |              |                        |                                    |                                                                    |
                //   |-----------|------|------|----------|--------------|------------------------|------------------------------------|--------------------------------------------------------------------|
                //   |           |      |      |          |              |                        |                                    |                                                                    |
                //   | boolMask: |  1   |  01  |   0001   |   00000001   |    0000000000000001    |  00000000000000000000000000000001  |  0000000000000000000000000000000000000000000000000000000000000001  |
                //   |           |      |      |          |              |                        |                                    |                                                                    |
                //   \--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------/
                //

                constexpr static int boolCount = 64;                                                    // can be any power of 2 up to 64; Tested are 8, 16, 32, and 64
                constexpr static unsigned long long int boolMask = 1;                                   // always a constant 1

                unsigned long long int compactedBools : boolCount;                                      // 64 bit unsigned long long int to store # bools == boolCount; bit field taken from https://en.cppreference.com/w/cpp/language/bit_field
                boolListNode *nextPtr;                                                                  // pointer to next boolNode in linked list
        };
        unsigned long long int nodeCount;                                                               // int to store number of nodes in linked list
        boolListNode* headPtr;                                                                          // pointer to head of linked list
};

#endif // BOOLLINKEDLIST_H
