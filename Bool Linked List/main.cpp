/*
   ACADEMIC INTEGRITY PLEDGE

   - I have not used source code obtained from another
     student nor any other unauthorized source, either
     modified or unmodified. Any source code used for
     testing that is another student's has been
     appropriately documented

   - All source code and documentation used in my program
     is either my original work or was derived by me from
     the source code published in the textbook for this
     course or presented in class. Other sources are acknowledged
     without transcribing code verbatim

   - I have not discussed coding details about this project
     with anyone other than my instructor, TA, and my immediate
     project team. I understand that I may discuss the concepts
     of this program with other students and that another student
     may help me debug my program so long as neither of us writes
     anything during the discussion or modifies any computer file
     during the discussion.

   - I have violated neither the spirit nor letter of these
     restrictions.

   Signed: Joseph Garro Date: 02/07/21


   COPYRIGHT (C) 2021 Joseph Garro. All Rights Reserved.
   Project 1: Project List
   Author. Joseph Garro, jmg289
   Version. 1.01 xxxx-xx-xx
   Files: Complexity_Runtime_DS_Example.cpp => basis of this file
          Complexity_Timer.hpp
          Complexity_Recorder.hpp
          boolLinkedList.cpp
          boolLinkedList.h


   COPYRIGHT (C) 2018 Will Crissey, Jr. All rights reserved.
   DS316 Sample Project example.
   Author.  Will Crissey, Jr.
   Version. 1.02 06.03.2018.
   Files: DS316_Project_Standard_Code_Example.cpp
          Complexity_Timer.hpp
          Complexity_Recorder.hpp
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <limits>
#include <fstream>
#include <iterator>
#include <algorithm>    // std::random_shuffle
#include "boolLinkedList.h"
#include <list>
#include <assert.h>
#include <bitset>
#include <string>
#include "Complexity_Timer.hpp"
#include "Complexity_Recorder.hpp"

//-----------------------------------------------------------------------------------------------------------
// GLOBAL VARIABLE DECLARATIONS

//Timer to record times
timer timer1;

// number_of_structures = number of structures tested at once
// number_of_trials = number of times each structure is tested
// repetitions controls number of times that a trial is repeated on a given structure
const int NUMBER_OF_STRUCTURES = 3;
const int NUMBER_OF_TRIALS = 5;
const int N1 = 1;    // smallest sequence
const int N2 = 2048;  // largest sequence
const int REPETITIONS = 4;

// FACTOR = size of problem = N
const int FACTOR = 20;

// TEST SIZE CONFIGURATION CORRESPONDING TO BOOLLINKEDLIST CLASS
// BOOLSIZE = number of bools stored in boolListNodes
// TWOPOWERSUBONE = 2^BOOLSIZE-1 = possible range of values for int in boolListNode
const int BOOLSIZE = 64;
const long long int TWOPOWERSUBONE = 18446744073709551615;
//const int BOOLSIZE = 32;
//const long long int TWOPOWERSUBONE = 4294967295;
//const int BOOLSIZE = 16;
//const long long int TWOPOWERSUBONE = 65535;
//const int BOOLSIZE = 8;
//const long long int TWOPOWERSUBONE = 255;
//-----------------------------------------------------------------------------------------------------------
//Formatting console output for various trial results
const char* menuOptions[8] =
{"---------------------------------------------------\n",
 "Available Tests:\n",
 "   1) Print all Elements Test\n",
 "   2) Access Last Element Test\n",
 "   3) Flip All Values Test\n",
 "   4) Delete Test (From Front)\n",
 "   5) Quit\n",
 "---------------------------------------------------\n"};

const char* headings_inverse_all[NUMBER_OF_STRUCTURES] =
{"| STL list inverse ",
 "| vector inverse  ",
 "| custom list inverse\n"};

const char* headings_delete[NUMBER_OF_STRUCTURES] =
{"| STL list del ",
 "| vector del  ",
 "| custom list erase \n"};
//-----------------------------------------------------------------------------------------------------------

int main()
{;
    /////////////////////////////
    //RECORDING + RESULTS SETUP//
    /////////////////////////////
    // for our outputting of the results
    std::ofstream ofs("results.txt");

     // random generation for generation of bool values to fill data structs
     std::srand(time(0));

    // this is going to hold the measurements
    std::vector<recorder<timer> > stats(NUMBER_OF_STRUCTURES);

    ////////////////////////////
    // menu for test selection//
    ////////////////////////////
    int testSelection = 1;
    while(true)
    {
        // output menu ui
        for(int i = 0; i < 8; ++i)
            std::cout << menuOptions[i];
        std::cout << "Selection: ";

        // selection input
        std::cin >> testSelection;
        assert(!std::cin.fail());                                                                                           // catch invalid input and terminate

        // test cases
        switch(testSelection)
        {
            // case 1 = test to print all elements in DS
            case 1:
                {
                    // output File Header
                    ofs << "\n///////////////////////////////////\n"
                    << "/                                 /\n"
                    << "/     Print all Elements Test     /\n"
                    << "/                                 /\n"
                    << "///////////////////////////////////\n\n"
                    << std::setw(11) << "Problem Size (n)" << std::endl;

                    // begin trials
                    for(int N0 = N1; N0 <= N2; N0 *= 2)
                    {
                        // test structures for comparisions
                        // linked list of bitsets that store a number of bits equal to BOOLSIZE
                        // source: https://www.cplusplus.com/reference/bitset/bitset/
                        boolLinkedList customLinkedList;
                        std::list<std::bitset<BOOLSIZE> > stdList;
                        std::vector<bool> stdVector;

                        unsigned long long N = N0 * FACTOR;                                                                // N = problem size for trial run
                        // loading data structures with random bools
                        // vector  loaded with BOOLSIZE as many elements to have equal elements stored in every structure
                        for(unsigned long long i = 0; i < (N*BOOLSIZE); ++i)
                        {
                            stdVector.push_back(rand() % 2);                                                                // load vector with random int 1 or 0
                        }
                        // loading boolLinkedList and vector of bitsets together, since they have the same number of nodes

                        unsigned long long randomBoolVal;
                        for(unsigned long long i = 0; i < N; ++i)
                        {
                            randomBoolVal = (rand() % TWOPOWERSUBONE + 1);
                            customLinkedList.addBoolListNode(randomBoolVal);
                            stdList.push_back(std::bitset<BOOLSIZE>(randomBoolVal));
                        }

                        // output Problem Size output file
                        ofs << std::setw(8) << N;

                        // reset stats vector to hold trial results
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                          stats[i].reset();

                        // begin tests == number_of_trials
                        for(int j = 0; j < NUMBER_OF_TRIALS; ++j)
                        {
                            // for each test, test each structure
                            for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                            {
                                // restart timer to record times
                                timer1.restart();

                                // test each structure equal to # of repetitions
                                for(int k = 0; k < REPETITIONS; ++k)
                                {
                                    if(i == 0)
                                    {
                                        // use auto itrs to display contents of linked list
                                        auto itr = stdList.begin();
                                        while(itr != stdList.end())
                                        {
                                            std::cout << *itr;
                                            itr++;
                                        }
                                    }
                                    else if(i == 1)
                                    {
                                        // use auto itr to display contents of vector
                                        auto itr = stdVector.begin();
                                        while(itr != stdVector.end())
                                        {
                                            std::cout << *itr;
                                            itr++;
                                        }
                                    }
                                    else
                                    {
                                        // use boolLinkedList displayListBinary() function to display node contents in binary
                                        customLinkedList.displayListBinary();
                                    }
                                }
                                // stop recording and save times to stats vector
                                timer1.stop();
                                stats[i].record(timer1);
                            }
                        }

                        // output results of trial to output file
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                        {
                            stats[i].report(ofs, REPETITIONS);                                              // output times to output file
                        }

                        // console and output file formatting
                        std::cout << std::endl;
                        ofs << std::endl;
                    }

                std::cout << "Print all Elements Test Complete" << std::endl;
                std::cout << "Check results.txt file for test results" << std::endl;
                }
            break;

            // case 2 = test to access last element in DS
            case 2:
                {
                    // output File Header
                    ofs << "\n////////////////////////////////////\n"
                    << "/                                  /\n"
                    << "/ Access + Print Last Element Test /\n"
                    << "/                                  /\n"
                    << "////////////////////////////////////\n\n"
                    << std::setw(11) << "Problem Size (n)" << std::endl;

                    // begin trials
                    for(int N0 = N1; N0 <= N2; N0 *= 2)
                    {
                        // test structures for comparisions
                        boolLinkedList customLinkedList;
                        std::list<std::bitset<BOOLSIZE> > stdList;
                        std::vector<bool> stdVector;

                        unsigned long long N = N0 * FACTOR;                                                                //N = problem size for trial run

                        // loading data structures with random bools
                        for(unsigned long long i = 0; i < (N*BOOLSIZE); ++i)
                        {
                            stdVector.push_back(rand() % 2);
                        }
                        // loading boolLinkedList
                        unsigned long long randomBoolVal;
                        for(int i = 0; i < N; ++i)
                        {
                            randomBoolVal = (rand() % TWOPOWERSUBONE + 1);
                            customLinkedList.addBoolListNode(randomBoolVal);
                            stdList.push_back(std::bitset<BOOLSIZE>(randomBoolVal));
                        }

                        // output Problem Size output file
                        ofs << std::setw(8) << N;

                        // reset stats vector to hold trial results
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                          stats[i].reset();

                        // begin tests == number_of_trials
                        for(int j = 0; j < NUMBER_OF_TRIALS; ++j)
                        {
                            // for each test, test each structure
                            for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                            {
                                // restart timer to record times
                                timer1.restart();

                                // test each structure equal to # of repetitions
                                for(int k = 0; k < REPETITIONS; ++k)
                                {
                                    if(i == 0)
                                    {
                                        std::cout << stdList.back() << std::endl;                           // output value of last list node
                                    }
                                    else if(i == 1)
                                    {
                                        for(unsigned long long i = (N*BOOLSIZE-BOOLSIZE); i < (N*BOOLSIZE); ++i)           // output last number of values in vector == BOOLSIZE
                                            std::cout << stdVector[i];
                                        std::cout << std::endl;
                                    }
                                    else
                                    {
                                        customLinkedList.displayNodeBinary(N);                              // output last node of boolLinkedList
                                        std::cout << "" << std::endl;
                                    }
                                }

                                // stop recording and save times to stats vector
                                timer1.stop();
                                stats[i].record(timer1);
                            }
                        }

                        // output results of trial to output file
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                        {
                            stats[i].report(ofs, REPETITIONS);                  // output times to output file
                        }

                        //Console and output file formatting
                        std::cout << "End of N = " << N << std::endl;
                        ofs << std::endl;
                    }

                std::cout << "Access + Print Last Element Test Complete" << std::endl;
                std::cout << "Check results.txt file for test results" << std::endl;

                }
            break;

            // case 3 = test to flip all values in DS
            case 3:
                {
                    // output File Header
                    ofs << "\n///////////////////////////////////\n"
                    << "/                                 /\n"
                    << "/      Flip all Elements Test     /\n"
                    << "/                                 /\n"
                    << "///////////////////////////////////\n\n"
                    << std::setw(11) << "Problem Size (n)" << std::endl;

                    // print heading to organize recorded times in console
                    for(int i = 0; i < 3; ++i)
                        std::cout << headings_inverse_all[i];

                    // begin trials
                    for(int N0 = N1; N0 <= N2; N0 *= 2)
                    {
                        // test structures for comparisions
                        boolLinkedList customLinkedList;
                        std::list<std::bitset<BOOLSIZE> > stdList;
                        std::vector<bool> stdVector;

                        unsigned long long N = N0 * FACTOR;                                 // N = problem size for trial run
                        std::cout << N*BOOLSIZE << " bools to flip" << std::endl;
                        // loading data structures with random bools
                        for(unsigned long long i = 0; i < (N*BOOLSIZE); ++i)
                        {
                            stdVector.push_back(rand() % 2);
                        }
                        // loading boolLinkedList and list of bitsets
                        unsigned long long randomBoolVal;
                        for(unsigned long long i = 0; i < N; ++i)
                        {
                            randomBoolVal = (rand() % TWOPOWERSUBONE + 1);
                            customLinkedList.addBoolListNode(randomBoolVal);
                            stdList.push_back(std::bitset<BOOLSIZE>(randomBoolVal));
                        }

                        // output Problem Size output file
                        ofs << std::setw(8) << N;

                        // reset stats vector to hold trial results
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                          stats[i].reset();

                        // begin tests == number_of_trials
                        for(int j = 0; j < NUMBER_OF_TRIALS; ++j)
                        {
                            // for each test, test each structure
                            for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                            {
                                // restart timer to record times
                                timer1.restart();

                                // test each structure equal to # of repetitions
                                for(int k = 0; k < REPETITIONS; ++k)
                                {
                                    if(i == 0)
                                    {
                                        auto itr = stdList.begin();
                                        while(itr != stdList.end())
                                        {
                                            (*itr).flip();
                                            itr++;
                                        }
                                    }
                                    else if(i == 1)
                                    {
                                        stdVector.flip();
                                    }
                                    else
                                    {
                                        customLinkedList.flipBits();
                                    }
                                }

                                // stop recording and save times to stats vector
                                timer1.stop();
                                stats[i].record(timer1);
                            }
                        }

                        // output results of trial to output file and console
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                        {
                            stats[i].report(cout, REPETITIONS);                 // output times to console
                            stats[i].report(ofs, REPETITIONS);                  // output times to output file
                        }

                        // console and output file formatting
                        std::cout << std::endl;
                        ofs << std::endl;
                    }

                std::cout << "Flip all Elements Test Complete" << std::endl;
                std::cout << "Check results.txt file for test results" << std::endl;

                }
            break;

            // case 4 = test to delete all values in DS
            case 4:
                {
                    // output File Header
                    ofs << "\n////////////////////////////////////////\n"
                    << "/                                      /\n"
                    << "/     Delete Entire Structure Test     /\n"
                    << "/                                      /\n"
                    << "////////////////////////////////////////\n\n"
                    << std::setw(11) << "Problem Size (n)" << std::endl;

                    // print heading to organize recorded times in console
                    for(int i = 0; i < 3; ++i)
                        std::cout << headings_delete[i];

                    // begin trials
                    for(unsigned long long N0 = N1; N0 <= N2; N0 *= 2)
                    {
                        // test structures for comparisions
                        boolLinkedList customLinkedList;
                        std::list<std::bitset<BOOLSIZE> > stdList;
                        std::vector<bool> stdVector;

                        unsigned long long N = N0 * FACTOR;

                        // output problem size to results.txt
                        ofs << std::setw(8) << N;

                        // reset stats vector
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                          stats[i].reset();

                        // begin tests == number_of_trials
                        for(int j = 0; j < NUMBER_OF_TRIALS; ++j)
                        {
                            // for each test, test each structure
                            for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                            {
                                    if(i == 0)
                                    {
                                        for(unsigned long long z = 0; z < N; ++z)
                                            stdList.push_back(std::bitset<BOOLSIZE>(std::to_string(rand() % 2)));
                                    }
                                    else if(i == 1)
                                    {
                                        for(unsigned long long z = 0; z < N*BOOLSIZE; ++z)
                                            stdVector.push_back(rand() % 2);
                                    }
                                    else
                                    {
                                        for(unsigned long long z = 0; z < N; ++z)
                                            customLinkedList.addBoolListNode(rand() % TWOPOWERSUBONE + 1);
                                    }
                                        //Restart timer to record times
                                    timer1.restart();
                                    {
                                        if(i == 0)
                                        {
                                            while(stdList.size() != 0)
                                                stdList.pop_front();
                                        }
                                        else if(i == 1)
                                        {
                                            while(stdVector.size() != 0)
                                                stdVector.pop_back();
                                        }
                                        else if(i == 2)
                                        {
                                        for(unsigned long long a = 0; a < N; ++a)
                                            customLinkedList.deleteBoolListNode(1);
                                        }
                                    }

                                    // stop recording and save times to stats vector
                                    timer1.stop();
                                    stats[i].record(timer1);
                            }
                        }

                        // output results of trial to output file and console
                        for(int i = 0; i < NUMBER_OF_STRUCTURES; ++i)
                        {
                            stats[i].report(cout, 1);                 // output times to console
                            stats[i].report(ofs, 1);                  // output times to output file
                        }

                        // console and output file formatting
                        std::cout << "Ending test N == " << N << std::endl;
                        ofs << std::endl;
                    }

                    std::cout << "Delete Entire Structure Test Complete" << std::endl;
                    std::cout << "Check results.txt file for test results" << std::endl;

                }
            break;

            // case 5 = quit testing
            case 5:
                {
                    return 0;
                }
            break;

            default:
                std::cout << "\nEnter a selection between 1 and 5" << std::endl;
        }
    }
}
