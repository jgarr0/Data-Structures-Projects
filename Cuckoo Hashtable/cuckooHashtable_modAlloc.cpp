#include "cuckooHashtable_modAlloc.h"
#include <string.h>
/*
 *   cuckoo hashing ~ Data Structures 3460:316
 *   FILE:   cuckooHashtable.cpp
 *   Author: Joseph Garro (4211105)
 *   EMAIL:  jmg289@uakron.edu
 */

//---------------------------------------
//
//     EXTERNAL   PUBLIC   FUNCTIONS
//
//---------------------------------------

cuckooHashtable_modAlloc::cuckooHashtable_modAlloc(){
    /* default size of arrays is 2 */
    size = 2;
    /* default allocation is 0 */
    allocation = 0;
    /* no insertions yet */
    insertions = t1 = t2 = 0;
    /* start with default pair of hashfunctions */
    hashSet = 0;
    /* create appropriately sized hashtables and table map + initialize all elements to '0' to avoid garbage */
    hashtable1 = new int[size]();
    hashtable2 = new int[size]();
    tableMap = new int[size]();
}

cuckooHashtable_modAlloc::cuckooHashtable_modAlloc(int allocation_){
    /* default size of arrays is 2 */
    size = 2;
    /* set allocation */
    allocation = allocation_;
    /* no insertions yet */
    insertions = t1 = t2 = 0;
    /* start with default pair of hashfunctions */
    hashSet = 0;
    /* create appropriately sized hashtables and table map + initialize all elements to '0' to avoid garbage */
    hashtable1 = new int[size]();
    hashtable2 = new int[size]();
    tableMap = new int[size]();
}
cuckooHashtable_modAlloc::~cuckooHashtable_modAlloc(){
    /* free space occupied by arrays */
    delete[] hashtable1;
    delete[] hashtable2;
    delete[] tableMap;
    hashtable1 = hashtable2 = tableMap = nullptr;

}

void cuckooHashtable_modAlloc::insert(int value){
    /* no duplicate or negative values */
    if(value < 0 || find(value) != -1)
        return;
    /* if through, try to insert into hashtable1 first */
    /* if return from insertHashtable1 == value, no movement*/
    int ht1rt, ht2rt;
    /* update insertion count */
    insertions++;
    /* insert value into hashtable1 */
    ht1rt = insertHashtable1(value);
    /* if not equal, require relocate return value to hashtable2 */
    if(ht1rt != value){
        ht2rt = insertHashtable2(ht1rt);
        /* if ht2rt == ht1rt, single relocation succeeded. crisis averted... */
        if(ht1rt != ht2rt){
            /* determine maximum allowable number of collisions */
            int maxCollisions = log2(size);
            /* now know max number of allowable collisions, have had 2 collisions at this point,
             * and we know the value to attempt to rehash into hashtable1 (ht2rt). save the output
             * of jumparound as an integer so that in the event of a rehash, the last relocated value is not lost */
            int test = jumpAround(maxCollisions, 2, ht2rt);
            /* rehash if jumpAround returns -1 */
            if(test != -1)
                rehash(test);
        }
    }
    /* check that arrays are under 50% load after insertion */
    balanceCheck();
}

void cuckooHashtable_modAlloc::print(){
    for(int i = 0; i < size; ++i)
        std::cout << hashtable1[i] << "     " << hashtable2[i] << "     " << tableMap[i] << std::endl;
}

int cuckooHashtable_modAlloc::find(int value){
    /* load proper hash functions based on hashset
     * thank you @https://stackoverflow.com/questions/2402579/function-pointer-to-member-function */
    int (cuckooHashtable_modAlloc::*func1)(int);
    int (cuckooHashtable_modAlloc::*func2)(int);
    /* hashSet == 0 is using hash1 and hash2 */
    if(hashSet == 0){
        func1 = cuckooHashtable_modAlloc::hash1;
        func2 = cuckooHashtable_modAlloc::hash2;
    }
    /* otherwise, hash3 and hash4 are being used */
    else{
        func1 = cuckooHashtable_modAlloc::hash3;
        func2 = cuckooHashtable_modAlloc::hash4;
    }
    /* save index where value will reside. Saving the indicies requires them to only be calculated once */
    int i1 = (this->*func1)(value);
    int i2 = (this->*func2)(value);
    /* try hashtable1 first: hash must match and valid table entry must exist */
    if((hashtable1[i1] == value) && (tableMap[i1] > 1))
        return i1;
    /* try hashtable2: hash must match and valid table entry must exist */
    else if((hashtable2[i2] == value) && (tableMap[i2]%2 == 1))
        return i2;
    /* otherwise, not in either hashtable */
    else
        return -1;
}

void cuckooHashtable_modAlloc::remove(int value){
    /* find returns index into hashtable1 or hashtable2, or -1 */
    int index = find(value);
    if(value == -1)
        return;

    /* if here, the value is in 1 of the hash tables */
    /* check hash table 1 */
    if(hashtable1[index] == value){
        /* delete value by setting it to '0' */
        hashtable1[index] = 0;
        /* update tableMap to indicate value was removed */
        tableMap[index]-=2;
        /* update hashtable1 and insertion counters */
        insertions--;
        t1--;
        return;
    }
    /* otherwise, value must be in hashtable2*/
    else{
        /* delete value by setting it to '0' */
        hashtable2[index] = 0;
        /* update tableMap to indicate value was removed */
        tableMap[index]--;
        /* update hashtable1 and insertion counters */
        insertions--;
        t2--;
        return;
    }
}

//---------------------------------------
//
//     INTERNAL   PRIVATE   FUNCTIONS
//
//---------------------------------------

int cuckooHashtable_modAlloc::hash1(int key){
    /* places values in index corresponding to remainder when divided by table size */
    return key % (size);
}

int cuckooHashtable_modAlloc::hash2(int key){
    return (key*2) % (size);
}

int cuckooHashtable_modAlloc::hash3(int key){
    return (key/2)%(size);
}

int cuckooHashtable_modAlloc::hash4(int key){
    return (key*3/size)%(size);
}

int cuckooHashtable_modAlloc::insertHashtable1(int value){
    /* ptr to hash member function. only need to consider hash1 and hash3 which manipulate hashtable1 */
    int(cuckooHashtable_modAlloc::*func)(int);
    if(hashSet == 0)
        func = &cuckooHashtable_modAlloc::hash1;
    else
        func = &cuckooHashtable_modAlloc::hash3;

    /* get index into hashtable 1*/
    int ht1Index = (this->*func)(value);
    /* check tableMap. if 0 or 1, no relocation */
    if(tableMap[ht1Index] < 2){
        /* update hashtable1 with new value */
        hashtable1[ht1Index] = value;
        /* update tableMap. since we do not care about hashtable2 in this instance,
         * just need to add 2 to tableMap[ht1Index] to show a value being stored in hashtable1 */
        tableMap[ht1Index]+=2;
        /* update hashtable1 element count */
        t1++;
        /* return supplied value to indicate success */
        return value;
    }
    /* otherwise must relocate */
    /* preserve value to relocate */
    int temp = hashtable1[ht1Index];
    /* update hashtable1 with new value and return evicted value */
    hashtable1[ht1Index] = value;
    return temp;
}

int cuckooHashtable_modAlloc::insertHashtable2(int value){
    /* ptr to hash member function. only need to consider hash2 and hash4 which manipulate hashtable2 */
    int(cuckooHashtable_modAlloc::*func)(int);
    if(hashSet == 0)
        func = &cuckooHashtable_modAlloc::hash2;
    else
        func = &cuckooHashtable_modAlloc::hash4;
    /* get index into hashtable 2*/
    int ht2Index = (this->*func)(value);

    /* check tableMap. if 0 or 2, no relocation */
    if(tableMap[ht2Index]%2 == 0){
        /* update hashtable1 with new value */
        hashtable2[ht2Index] = value;
        /* update tableMap. since we do not care about hashtable1 in this instance,
         * just need to add 1 to tableMap[ht2Index] to show a value being stored in hashtable2
         */
        tableMap[ht2Index]+=1;
        /* update hashtable2 element count */
        t2++;
        /* return supplied value to indicate success */
        return value;
    }
    /* otherwise must relocate */
    /* preserve value to relocate */
    int temp = hashtable2[ht2Index];
    /* update hashtable1 with new value and return evicted value */
    hashtable2[ht2Index] = value;
    return temp;
}

int cuckooHashtable_modAlloc::jumpAround(int maxCollisions, int currentCollisions, int value){
    /* rehash once maximum number of collisions have been reached.
     * <= is used to catch all situations as it does not matter when the rehash occurs after an invalid arrangement is detected */
    if(maxCollisions <= currentCollisions){
        /* if resulting in a rehash, need to save recursed value to an open spot in either hashtable */
        return value;
    }
    int hfrtn;
    /* the return value needs to be rehashed into hashtable1 if currentCollisions is even */
    if(currentCollisions%2 == 0){
        hfrtn = insertHashtable1(value);
        /* if hfrtn == value, done */
        if(hfrtn != value)
            jumpAround(maxCollisions, currentCollisions+1, hfrtn);
        else
            return -1;
    }
    /* otherwise, try and load into hashtable2 */
    else{
        hfrtn = insertHashtable2(value);
        /* if hfrtn == value, done */
        if(hfrtn != value)
            jumpAround(maxCollisions, currentCollisions+1, hfrtn);
        else
            return -1;
    }
}

void cuckooHashtable_modAlloc::rehash(){
    /* get all values from hashtables */
    int* values = getValues();
    /* rehash */
    performRehash(values);

}

void cuckooHashtable_modAlloc::rehash(int displacedValue){
    /* get all values from hashtables */
    int* values = getValues();
    /* add displaced value to list of all values */
    values[insertions-1] = displacedValue;
    /* rehash */
    performRehash(values);
}

void cuckooHashtable_modAlloc::performRehash(int* values){
    /* values has all elements that have been inserted into both hashtables
     * solution to rehashing: switch hashing functions and increase size until
     * a valid set of hashtables exist that can hold all the data */

    /* switch hashset every iteration */
    hashSet = !hashSet;
    /* clear t1 and t2 every iteration */
    t1 = t2 = 0;

    /* setup rehashing based on hashSet
     * if hashSet == 1, want to clear values in hashtable1, hashtable2, tableMap */
    if(hashSet == 1){
        /* clear all tables */
        for(int i = 0; i < size; ++i){
            hashtable1[i] = hashtable2[i] = tableMap[i] = 0;
        }
    }
    /* otherwise, allocate new hashtable1, hashtable2, tableMap */
    else if(hashSet == 0){
        /* free space occupied by arrays */
        delete[] hashtable1;
        delete[] hashtable2;
        delete[] tableMap;
        /* allocate new larger space for each array */
        //size +=2;
        //size *=2;
        switch(allocation){
        case 0:
            size *=2;
        break;
        case 1:
            size *=4;
        break;
        case 2:
            size *=8;
        break;
        case 3:
            size +=4096;
        break;
        case 4:
            size +=2048;
        break;
        case 5:
            size +=1024;
        break;
        case 6:
            size +=512;
        break;
        default:
            size *=2;
        }
        hashtable1 = new int[size]();
        hashtable2 = new int[size]();
        tableMap = new int[size]();
    }

    /* update maxCollisions to reflect new size, and set collisions to 0 */
    int maxCollisions = log2(size);
    int collisions = 0;
    /* once setup complete, can try inserting values
     * once 1 value cycles, scrap attempt and try again */
    int i;
    /* for each value in values, insert it into hashtables using jumparound function. break if cycle */
    for(i = 0; i < insertions; ++i){
        if(jumpAround(maxCollisions, collisions, values[i]) != -1)
            break;
    }
    /* if value of counter != number of insertions, a cycle occurred. call perform rehash again*/
    if(i != insertions)
        performRehash(values);
    /* on success, check that each hashtable has less than 50% load */
    balanceCheck();
}

int* cuckooHashtable_modAlloc::getValues(){
    /* only insertions elements exist */
    int* values = new int[insertions]();
    /* fill values
     * index for values array */
    int j = 0;
    /* tableMap has numberElements*2 entries*/
    for(int i = 0; i < size; ++i){
        if(tableMap[i] == 1){
            values[j] = hashtable2[i];
            j++;
        }
        else if(tableMap[i] == 2){
            values[j] = hashtable1[i];
            j++;
        }
        else if(tableMap[i] == 3){
            values[j] = hashtable1[i];
            j++;
            values[j] = hashtable2[i];
            j++;
        }
    }
    return values;
}

void cuckooHashtable_modAlloc::balanceCheck(){
    /* rehash if 1 hashtable more than half full */
    int maxTable = size/2;
    if(t1 > maxTable || t2 > maxTable)
        rehash();
}
