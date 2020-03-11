/**
* file TrainAndTest.h
* declares the  RuleInduction challenge - specific functions
*
* Author Jim Smith 17-1-20
*/


#include <stdio.h>
#include <stdlib.h>
#include "StructureDefinitions.h"
#include "SolutionListOperations.h"


#ifndef _TrainAndTest



/**
 @file TrainandTest.h
 @brief one useful type of structure and various specific functions for RuleInduction  Problem
 */


//=======================================
/*here are some constant values that we will refer to
 * the beauty of using defines here is that now if I ever want to change them, I only need do so in one place
 */
// it would be cleaner to make these dynamic but for safety resons we can;t have student code dynamically allocating memory
#define NUM_FEATURES 6
#define NUM_SAMPLES 150
#define NUM_TRAINING_SAMPLES 100
#define NUM_TEST_SAMPLES 50



#define NO_PREDICTION -1
#define VALUES_PER_RULE 4
#define MAX_NUM_RULES  5

#define N (VALUES_PER_RULE * MAX_NUM_RULES)

#define THRESHOLD_PRECISION 100



// some rule specific declarations
#define NUM_OPERATORS 3
enum operator {lessThan =0, equals = 1, greaterThan = 2};
    
///  struct to hold a simple (axis-perpendicular) rule  of form IF feature[X] > threshold THEN class = Y
typedef struct {
    int variableAffected; ///< the index of the feature that this rule refers to
    enum operator comparison; ///< the comparison - one of < (0) == (1)  or > (2) using the enum operator type
    int threshold; ///<  value in range [0,THRESHOLD_PRECISION].  Given a feature with minVal and maxVal, the actual rule threshold is calculated as minval + (maxval-minval) * (threshold / THRESHOLD_PRECISION)
    int prediction; ///< int representing the class that this rule predicts
}rule;
    
    

#define NO_PREDICTION -1



    
    /// function that calls a machibe learning algorithm and learns a classifier
    /// returns 1 if successful,  0 if some error condtion occurs
    /// @param trainingSamples : 2array -one row per sample, one column per feature
    /// @param trainingLabels : 1D array - one row per sample, of class labels
    /// @param numSamples : int - the number of sanp,es (rows)
    /// @param numFeatures :int the number of features (columns)
int  train( double **trainingSamples, int *trainingLabels, int numSamples, int numFeatures);
    
    /// function that calls the trained classifier to make a prediction for a provided sample
    /// returns the prediction if successful else NO_PREDICTION
    ///  will fail if the number of features in the sample does not match the training set
    /// @param sample :1D array holding the feature f=values for the sample to be classified
    /// @param numFeatures : int: the number of features in the sample
int  predictLabel(double *sample, int numFeatures);


void StoreData(double **trainingSamples, int *trainingLabels, int numSamples, int numFeatures);

    
    /// @brief this is  the part that you have to write
    ///     Your code should make use of the structures and operators provided to create a ruleset
    ///         Starting with an empty working cndidate, at each iteration it shoul the single rule that most increases the score
    ///         to test a candidage soltion copy it into th     you should call the function ScoreWorkingCandidateOnTrainingSet()    to test a candidate soltion

    void GreedyConstructiveSearch(void);

    
    /// Simple Boolean teast of whether the working candidate
    ///  (i) classifies all of the trainig set example correctly  OR has no room left to add furthe rules
bool GoalFound(void);
    
    /// @brief Function that calculates amnd s tortes the scopre for the working candidate
///     Function that interprets the working candidate as a ruleset, and calculates its score,
///     No return values as it is stored in workingcandidate.score
///             Note that score is zero if any of the treaiing set are classified incorrectly,  otherwise it is the number of correctly classified samples in the traiing sset
///   Pre-conditions:  (i) training set has been processed and stored; (ii) workingcandidate contains some rules i.e .size >0
/// Post-conditions: workingCandidate.score is updated.
void ScoreWorkingCandidateOnTrainingSet(void);
    
    /// @Brief: function to predict the class of an item by applying a simple rule
    /// @param thisRule rule to be applied
    /// @param sample sample ot apply rule to
    /// @param numFeatures number of featues in sample
/// Return is  class predicted or NO_PREDICTION if sample does not match rule condition
int PredictClassFromRule(rule thisRule, double * sample, int numFeatures );


    
    /// @Brief Function to print out the ruleset represented within a canidate solution
    /// preconditino is the existience of a variable called workiongCandidate that holds the learned rule set
void printWorkingCandidate (void);


    
    /// @brief  Extends a partial solution by adding a new rule
    ///  Preconditions:
    /// (i) existence of global variable workingCandidate of type candidateSolution
    ///(ii) workingCandidate has space for new rule i.e. workingCandidate.size <N.
    
    ///Postconditions: (i) workingCandidate.size increased by VARIABLES_IN_RULE (4);
    ///           (ii) values added to array of variables. i.e. {variable_affected, comparison, threshold,prediction}.

    /// @param newRule : struct rule to be added to the ruleset represented in the working candidate
void ExtendWorkingCandidateByAddingRule(rule newRule);


    
    
    /// function that initialsies the local storage ready to hold the trainig data
    /// @param numSamples number of examples (rows)
    /// @param numFeatures number of features (columns)
void prepareTrainingDataArrays(int numSamples, int numFeatures);
#define _TrainAndTest 1
#endif
