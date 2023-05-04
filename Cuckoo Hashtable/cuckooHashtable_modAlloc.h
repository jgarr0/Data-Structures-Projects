#ifndef CUCKOOHASHTABLE_MODALLOC_H
#define CUCKOOHASHTABLE_MODALLOC_H
#include <math.h>
#include <iostream>

/*
 *   cuckoo hashing ~ Data Structures 3460:316
 *   FILE:   cuckooHashtable_modAlloc.h
 *   Author: Joseph Garro (4211105)
 *   EMAIL:  jmg289@uakron.edu
 */

class cuckooHashtable_modAlloc
{
    public:

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        //                              CONSTRUCTOR AND DESTRUCTOR
        //
        ///////////////////////////////////////////////////////////////////////////////////////

        /* default constructor */
        cuckooHashtable_modAlloc();
        cuckooHashtable_modAlloc(int);
        /* default destructor */
        ~cuckooHashtable_modAlloc();

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        //                                 EXTERNAL FUNCTIONS
        //
        ///////////////////////////////////////////////////////////////////////////////////////

        void insert(int);
        //pre:  existing cuckooHashtable object
        //post: inserts the passed value into the cuckooHashtable object. Does not insert
        //      duplicate or negative values

        void print();
        //pre:  existing cuckooHashtable object
        //post: prints the values of each hashtable in the cuckooHashtable object, as well as
        //      the tableMap values corresponding to both hashtables

        int find(int);
        //pre:  existing cuckooHashtable object
        //post:

        void remove(int);
        //pre:  existing cuckooHashtable object
        //post: removes desired element from cuckooHashtable object if it exists, and updates
        //      the appropriate hashtable, index in tableMap, and insertion counts

        void debug(){
            std::cout << "size: " << size
                      << "     total insertions: " << insertions
                      << "     t1 inserts: " << t1
                      << "     nt2 inserts: " << t2 << std::endl;
        }

    private:

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        //                                 INTERNAL VARIABLES
        //
        ///////////////////////////////////////////////////////////////////////////////////////

        /* size of arrays */
        int size;
        /* number of insertions that have been made */
        int insertions;
        /* store the number of insertions in hashtable1 and hashtable2 respectively */
        int t1;
        int t2;
        /* determine allocation algorithim */
        int allocation;
        /* pointers to 1st and 2nd hashtables */
        int* hashtable1;
        int* hashtable2;
        /* tableMap is an array that maps the usage of both hashtables
         * for the elements in either hashtabl at a given index i:
         * tableMap[i] == 0 if neither hashtable1[i] nor hashtable2[i] exist
         * tableMap[i] == 1 if only hashtable2[i] exists
         * tableMap[i] == 2 if only hashtable1[i] exists
         * tableMap[i] == 3 if both hashtable1[i] and hashtable2[i] exist */
        int* tableMap;

        /* bool to indicate the set of hash functions being used to hash both hash tables
         * 0 = hash1 -> hashtable1 and hash2 -> hashtable2
         * 1 = hash3 -> hashtable1 and hash4 -> hashtable2 */
        bool hashSet;

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        //                                 INTERNAL FUNCTIONS
        //
        ///////////////////////////////////////////////////////////////////////////////////////

        /* hashfunction declarations */
        int hash1(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: return the index in the hashtable where the supplied value is to be placed
        //      IN USE IF hashSet == 0

        int hash2(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: return the index in the hashtable where the supplied value is to be placed
        //      IN USE IF hashSet == 0

        int hash3(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: return the index in the hashtable where the supplied value is to be placed
        //      IN USE IF hashSet == 1

        int hash4(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: return the index in the hashtable where the supplied value is to be placed
        //      IN USE IF hashSet == 1

        /* insert into individual hashtables */
        int insertHashtable1(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: insert into hashtable1 + return inserted value if no relocation needed. return
        //      the victim value if relocation needed, which will be inserted in hashtable2

        /* insert into hashtable2 + return inserted value if no relocation, victim value if relocation */
        int insertHashtable2(int);
        //pre:  existing cuckooHashtable object and valid integer value
        //post: insert into hashtable2 + return inserted value if no relocation needed. return
        //      the victim value if relocation needed, which will be inserted in hashtable1

        int jumpAround(int, int, int);
        //pre:  existing cuckooHashtable object, maximum collisions, current collisions, int value
        //post: recursively relocates values based on displacements, inserting displaced values
        //      into appropriate hashtable and keeping track of displacements. if more displacements
        //      occur than maxmimum displacements, rehash the tables. otherwise, the tables are
        //      updates to include the passed value in a valid arrangement

        void rehash();
        //pre:  existing cuckooHashtable object
        //post: triggers rehashing of both hashtables

        void rehash(int);
        //pre:  existing cuckooHashtable object
        //post: triggers rehashing of both hashtables + includes displaced value

        /* function to swap pair of hash functions and increase hashtable size */
        void performRehash(int*);
        //pre:  cuckooHashtable object with a cycle
        //post: breaks cycles by rehashing both hashtables. rehashing achieved through recursively
        //      changing the hashing functions in use and the size of the hashtables with each
        //      iteration until a valid arrangement of values in the hashtables is obtained

        /* rehash helper function to get all values in both hashtables */
        int* getValues();
        //pre:  existing cuckooHashtable object
        //post: returns a pointer to an array of all elements inserted into the cuckooHashtable

        /* helper function to ensure tables are below 50% full + can rehash if not */
        void balanceCheck();
        //pre:  existing cuckooHashtable object
        //post: triggers rehashing if a hashtable is over 50% full
};

#endif // CUCKOOHASHTABLE_MODALLOC_H
