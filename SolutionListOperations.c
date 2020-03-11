//
//  SolutionListOperations.c
//  Password-Example
//
//  Created by Jim Smith on 25/09/2014.
//  Copyright (c) 2014 James Smith. All rights reserved.
//




#include <stdio.h>
#include <stdlib.h>


// #include "TrainAndTest.h"

#include "StructureDefinitions.h"
#include "SolutionListOperations.h"


#ifdef ONDEWIS
extern int numberTimesOpenListEmpty;
extern int numberTimesOpenListOccupied, numberTimesNotBestPicked;
extern bool usesAddWorkingCandidate,usesCopySolutionFromOpenListIntoWorkingCandidate;
extern bool constructiveSearch;
#endif

/* this is just a really handy function to use for debugging or for dealing with unexpected situations in your code*/
    void PrintThisMessageAndExit(char *message)
  {
      
#ifdef ONDEWIS
      printf("<DEWIS_FEEDBACK>Your code terminated unexpectedly with this message:<br>%s<br> You should get the same message in your IDE.</DEWIS_FEEDBACK>\n",message);
#else
    printf("%s\n",message);

      //arbitrary wait for the benefit of visual studio users
    for(int i=0;i < 5200000; i++)
          ;
#endif
    exit(0);

  }

//==========================

int GetIndexOfWorkingCandidateInThisList(candidateList thislist)
{
    extern candidateSolution workingCandidate;
   static  char wcvalues[MAXVARS];
    int variableCounter=0, which=0, mismatches=MAXVARS;
    
    //copy variable values from working candidate into conveniently named array becuase it is faster this way
    for(variableCounter=0;variableCounter<MAXVARS;variableCounter++)
        wcvalues[variableCounter] = workingCandidate.variableValues[variableCounter];
    
    //start at the end of the list
    which = thislist.size -1;
    //and loop through until we have found a match or reached the beginning
    while (which >=0)
      {
        // start looking variable by variable, stopping as soon as there is a mismatch
        for (variableCounter=0,mismatches = 0; (variableCounter < MAXVARS)&& (mismatches ==0);variableCounter++)
            if (thislist.listEntries[which].variableValues[variableCounter] != wcvalues[variableCounter])
                mismatches++;
        if (mismatches>0) //no mismatches found so move to next one in the list
            which --;
        else
            break;
      }
    //end of which loop,  now just need to look and see if we found it
    if (which < 0)
        which = NOTFOUND;
    return  which;
}

//===============================
int GetIndexOfWorkingCandidateInOpenList(void)
{
    extern candidateList openList;
    int position = GetIndexOfWorkingCandidateInThisList(openList);
    return position;
}

//===============================
int GetIndexOfWorkingCandidateInClosedList(void)
{
    extern candidateList closedList;
    int position = GetIndexOfWorkingCandidateInThisList(closedList);
    return position;
}

//==============================================================================
void CleanCandidate( candidateSolution *toBeCleaned)
{
	int currentVariable;
    ///can't clean something that doesn;t exist!
	if( toBeCleaned== (candidateSolution *)NULL)
		PrintThisMessageAndExit("error - parameter passed to CleanCandidate() was not valid address of candidate");

    ///to start with set the number of defined values to be zero in this solution
        toBeCleaned->size =0;
    ///just to be on the safe size,  set each actual value to "UNUSED"
		for(currentVariable = 0; currentVariable < MAXVARS; currentVariable++)
            toBeCleaned->variableValues[currentVariable] = UNUSED;
    ///and obvously there can't be any conflicts in an empty solution
        toBeCleaned->score=0;

}


/* void CleanListsOfSolutionsToStart(void)
 *this function runs through the two structures  we declared to hold our lists of candidate  and examinedsolutions and
 *sets each one to be empty
 * takes no parameters (because the list stuctures are declared as global and does not return any values
 */
void CleanListsOfSolutionsToStart(void)

{
    int currentSolution; //loop variables used to loop through the solutions
  
    // by referring to these as externs our compiler will check they exist in some other file when it builds the programme
    extern candidateList openList, closedList;
    
	// loop through each solution in each list emptying the contents
    for (currentSolution=0;currentSolution<SIZEOFSOLUTIONLIST;currentSolution++)
      {
		  CleanCandidate ( &openList.listEntries[currentSolution]);
		  CleanCandidate ( & closedList.listEntries[currentSolution]);
      }
    openList.size = closedList.size = 0;
    
 
}//end of CleanListsOfSolutionsToStart(void)


/*  void CleanWorkingCandidate (void)
*  make a call to the generic function to do what it says on the can
*/
void CleanWorkingCandidate (void)
{
	extern candidateSolution workingCandidate;
	CleanCandidate(& workingCandidate);
}
	
	
	
	
	/*CopySolutionParam1_IntoSolutionParam2( candidateSolution *from, candidateSolution *to)
 * copy contensts of candidate soltion whoe address in memory is given by param 1
 * into the candidate soltion whose address in memory is geniven by param 2
 * does all error checking internally so no return
 */
void CopySolution( candidateSolution *from, candidateSolution *to)
{
    
    int variableCounter;//loop variable
    
    //error checking - addreses in memory must have been designated as holding structures of the right type
    if(from == (candidateSolution *)NULL || to == (candidateSolution *)NULL )
        PrintThisMessageAndExit("invalid address provided in CopySolution() function");
    
    //now do a simple copy of the contents
    to->size = from->size;
    to->score = from->score;
    for ( variableCounter=0; variableCounter<from->size;variableCounter++)
        to->variableValues[variableCounter] = from->variableValues[variableCounter];
    
    //and blank out the unused spaces
    for( variableCounter=from->size; variableCounter < MAXVARS;variableCounter++)
        to->variableValues[variableCounter] = UNUSED;
}



/* void CopySolutionFromCurrentListIntoWorkingCandidate(int solutionIndex);
 * makes a call to the generic copy function
 * presupoosed the existence of the global variables
 * to make a nice simple to call function
 */
void CopySolutionFromOpenListIntoWorkingCandidate(int solutionIndex)
{
    
   //define the global variables as having beeen declared in a another file
	extern candidateList openList;
    extern candidateSolution workingCandidate;
    
	//check that the index is not off the end of the list
	if (solutionIndex >= SIZEOFSOLUTIONLIST || solutionIndex <0)
		PrintThisMessageAndExit("solution index passed to function CopySolutionFromCurrentListIntoWorkingCandidate() is beyond bounds of array");
	
    // if we got this far we can do the copy using the generic function
    CopySolution( &openList.listEntries[solutionIndex], &workingCandidate );
    
    #ifdef ONDEWIS
        usesCopySolutionFromOpenListIntoWorkingCandidate = true;
    
    //now check  what member was called
    int highestScore=0;
    for(int entry=0; entry < openList.size; entry++){
        if(  openList.listEntries[entry].score> highestScore ) {
            highestScore = openList.listEntries[entry].score;
        }
    }
    
    if ( openList.listEntries[solutionIndex].score< highestScore)
        numberTimesNotBestPicked ++;
    #endif

    
}



/* generic function to add a candidate to a list
* firstr param is address of candidate ot add
* second is address of list ot receive
* does all the error handling in the function so no return value
*/
void AddSolutionPram1_ToListParam2(candidateSolution *toAdd, candidateList *receiver)
{
	int currentsize;

	//check parameters are valid
	if  ( toAdd == (candidateSolution *)NULL  || receiver ==(candidateList *)NULL)
		PrintThisMessageAndExit ("parameters to AddSolutionPram1_ToListParam2 are not valid pointers");

	//check reciever is not full
	else if ( receiver->size == SIZEOFSOLUTIONLIST)
		PrintThisMessageAndExit ("can't add candidate  because receiving list is full. Is your algorithm stopping as soon as it finds the solution?");

	else
		{
			currentsize = receiver->size; //remember because arrays start from zero
			CopySolution(toAdd, & receiver->listEntries[currentsize ]);
			receiver->size = currentsize+1;
	}



}





void RemoveFromListParam1_CandidateSolutionAtIndexParam2( candidateList *listToEdit, int solutionIndex)
{
    
    int numberOfEntriesInThisList,lastEntry; //declare int to hold number of solutions currently in the list
	 int copyingfrom, copyingto;// and two more to use when we shuffle things along
	 int variableCounter=0;

    //next we'll start off with a series of error checks before we do anything
    //each of these is serous enough to cause us to quit the programme
    
    //first of all, have we been given the address of a valid candidate list
    if( listToEdit == (candidateList *)(NULL))
        PrintThisMessageAndExit("first parameter passed was not a pointer to a valid list");

    // then of all check that the index we are given is valid
    else if ( (solutionIndex >= SIZEOFSOLUTIONLIST) || (solutionIndex <0))
        PrintThisMessageAndExit("oops - tried to remove a solution that lies beyond the bounds of our list");
 
	// if we've got here we know it is safe to try and access the lists
	else 
      {
        // last couple of tests
		  //find out how many entries are in the list
        numberOfEntriesInThisList = listToEdit->size;
        lastEntry = numberOfEntriesInThisList -1;
          
        // check that the list is not already empty
        if (numberOfEntriesInThisList<1)
            PrintThisMessageAndExit("oops - tried to remove an entry from an empty list");
        //check the index isn't too high
        if ( solutionIndex > lastEntry)
            PrintThisMessageAndExit("oops - tried to remove a solution that does not exist in our list");



        //  ok, then we can proceed
        //printf("removing solution at index %d \n", solutionIndex);
        //if the one to remove is the last on the list,  its all very simple
          //otherwisde we have to shuffle all the rest along and then delete what is now a duplicate lasdt entry
        if(solutionIndex < lastEntry)
        {
            //start with the one to be deleted, and overwrite the contents with  the solution to the right
            //then move right and repeat
            for(copyingto = solutionIndex; copyingto < lastEntry;copyingto++)
            {
                copyingfrom = copyingto + 1;//overwrite with the contents of the solution to the right in the list
                listToEdit->listEntries[copyingto].size =  listToEdit->listEntries[copyingfrom].size;
                listToEdit->listEntries[copyingto].score = listToEdit->listEntries[copyingfrom].score;
                for (variableCounter=0;variableCounter < MAXVARS;variableCounter++)
                    listToEdit->listEntries[copyingto].variableValues[variableCounter] = listToEdit->listEntries[copyingfrom].variableValues[variableCounter];
                
            }//end of copying loop
        }//end if code that deals with shuffling situation
          
        //now delete the last entry - code should skip to here if that's the one we're asked to remove
        numberOfEntriesInThisList --;
        
        listToEdit->listEntries[numberOfEntriesInThisList ].size = 0;
        listToEdit->listEntries[numberOfEntriesInThisList ].score = 0;
        for ( variableCounter=0;variableCounter < MAXVARS;variableCounter++)
            listToEdit->listEntries[numberOfEntriesInThisList].variableValues[variableCounter] = UNUSED;
        //and reduce the count of number of things in the list
        listToEdit->size  -- ;
 
        
      }//end of code for valid call
}//end of generic removefunction


//now we make a simple call to the generic function that hides all the complexity of calling with pointers etc.

void RemoveSolutionFromOpenList(int solutionIndex)
{
    extern candidateList openList;

    RemoveFromListParam1_CandidateSolutionAtIndexParam2(&openList,solutionIndex);
}






/* void AddCandidateParam1_ToListParam2(candidateSolution * candidate, candidateList *listToChange)
 * generic version of a function to add a candidate solution to a list
 * first param is address of thing ot be added
 * second is addres of list ot add it to
 */

void AddCandidateParam1_ToListParam2(candidateSolution * candidate, candidateList *listToChange)
{
    int newpos; //get number of entries in list, loop variable for rows
    newpos = listToChange->size;
    
    //error checking
    if(newpos ==SIZEOFSOLUTIONLIST)
        PrintThisMessageAndExit("can't add member - list is already full");
    
    //add the contents by using copy to overwrite the empty list entry
     CopySolution(candidate,  &listToChange->listEntries[newpos]);
    //all done,  just now need to increment the counter in the list
    listToChange->size = newpos+1;
    
}

//these two functions are just wrapped up to be easier to use
void AddWorkingCandidateToOpenList(void)
{
    extern candidateList openList;
    extern candidateSolution workingCandidate;
    
#ifdef ONDEWIS
    usesAddWorkingCandidate = true;
    if(openList.size ==0)
        numberTimesOpenListEmpty ++;
    if(openList.size >100)
        numberTimesOpenListOccupied ++;
        
#endif
    
    AddCandidateParam1_ToListParam2(&workingCandidate,&openList);
}


void AddWorkingCandidateToClosedList(void)
{
    extern candidateList closedList;
    extern candidateSolution workingCandidate;
    AddCandidateParam1_ToListParam2(&workingCandidate,&closedList);
    
}


/* 
*void ExtendWorkingCandidateByAddingValue(int value)
* function to add a new value on to the end if possible
* no return as all the error checking is done in house
*/
void ExtendWorkingCandidateByAddingValue(int newValue)
{
extern candidateSolution workingCandidate;
int newLastPos=0;
//check it isn't already full
if (workingCandidate.size==MAXVARS)
	{ 
		PrintThisMessageAndExit (" can't add value to already full working candidate");
    }
else
 {
	 newLastPos = workingCandidate.size;
	 workingCandidate.variableValues[newLastPos] = newValue;
	 workingCandidate.size++;
 }

#ifdef ONDEWIS
    constructiveSearch = true;
#endif
}



/* ChangeWorkingCandidateByReplaceValueinPlaceParam1_WithValueParam2(int positionToChange, int newValue)
* function TO Change a specific value in a solution ot a new one
* first param is index of value to change
* second is new value
* * no return as all the error checking is done in house
*/
void ChangeWorkingCandidateByReplaceValueinPlaceParam1_WithValueParam2(int positionToChange, int newValue)
{
extern candidateSolution workingCandidate;

    char errmessage[128] ;
//check we are trying to change  valid value that ha already been set
if (   (positionToChange >= workingCandidate.size) || (positionToChange <0)  || (positionToChange >MAXVARS)  )
  {
    sprintf(errmessage,"invalid value %d for positionToChange  passed to ChangeWorkingCandidate which has %d defined values...\n",positionToChange
            ,workingCandidate.size);
	PrintThisMessageAndExit(errmessage);
  }
else
	workingCandidate.variableValues[positionToChange] = newValue;
}