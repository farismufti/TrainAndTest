/*
*  StructureDefinitions.h
*  Rule Induction-Example
*  This is a header file where I define some problem-sepcific types such as structures
*  so that I can refer to things in my code using names that make sense in the context of the problem
*  Created by Jim Smith on 25/09/2014.
*  Copyright (c) 2014, 2018, 2020 James Smith. All rights reserved.
*/


// this line, with the ones at the bottom, stops the file and all its definitions being included in our source code twice
#ifndef Generic_StructureDefinitions_h
/**
 @file StructureDefinitions.h
 @brief Definitions of constant values and the data structures.
 */



typedef int bool;
#define true  1
#define false 0

#define NOTFOUND -1
#define UNUSED '[' ///< An arbitrary value used to make the code clearer

#define BIG_DBL 9999999.99
#define MAXVARS 50
#define SIZEOFSOLUTIONLIST 100000 ///< The size of the fixed arrays used to store open and closed lists

//============================
/**
 *  @brief Definition of struct datatype for candidate solution
 *
 * This is the definition of how we will represent each candidate solution
 *  and its associated information.
 *
 * Clearly this is problem-dependant
 */

typedef struct {
    int size;///< how many variables in thre representation of a solution have their vales set
    char variableValues [MAXVARS]; ///< and what  are they
    int score; ///< measure of how good the solution is
} candidateSolution;

/**
 @brief  Definition of a datatype for a list of candidate solutions
 
  Datatype to hold a collection of  candidate solutions in the form of an array
 along with a  variable that tells us the array index of the last entry in the list.
 
 NOTE  that it is intended that all add/remove actions are done via the provided functions so that the internal counter of the numer of entries is kept correct
 */
typedef struct {
    candidateSolution listEntries[SIZEOFSOLUTIONLIST]; ///< The array of candidate solutions
    int size; ///< Index in array of last member used: this is automatically incremented and decremented by my functions
} candidateList;




// this line stops any problems with double-inclusion of this header file
#define Generic_StructureDefinitions_h


#endif
