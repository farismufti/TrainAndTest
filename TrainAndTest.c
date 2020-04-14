/*  file RuleInductionSupportFunctions.c
*
* Author Jim Smith 17-1-20
*/

#include "TrainAndTest.h"
#include "StructureDefinitions.h"


//variables for storing to training data and its characteristics
static double myModel[NUM_TRAINING_SAMPLES][NUM_FEATURES]; ///< local copy of the training data
static int myModelLabels[NUM_TRAINING_SAMPLES]; ///< local copy of the labels in the training data

static int numClasses = 0; // store the number of different class labels in the training set
static int validLabels[256]; // keep a list of what those valid labels are
static int trainingSetSize = 0; // keep track of how many examples (rows) are in the training set
static int trainingSetFeatures = 0; // record how many features (columns) were in the training set
static double minVal[NUM_FEATURES];
static double maxVal[NUM_FEATURES];
static bool modelTrained = false; // keep track of whether we have trained our model or not

//Variables workingCandidate, openList, closedList for use in greedy local search for best set of rules
candidateSolution workingCandidate;// this one will hold the solutions we are currently considering
candidateList openList; // this list will store all the solutions we've created but not examined yet
candidateList closedList; // this is where we will store all the ones we're done with


int train(double **trainingSamples, int *trainingLabels, int numSamples, int numFeatures)
{
    //make a simple copy of the data we are being passed but don't do anything with it
    //I'm just giving you this for the sake of people less familiar with pointers etc.
    StoreData(trainingSamples, trainingLabels, numSamples, numFeatures);
   
   
    // you can comment out the next print statement if you like - iut;s just there to help debugging
    printf("\nAbout to Learn rules:\n");
    
    GreedyConstructiveSearch();

    printWorkingCandidate();

    
    // you need to leave this line in
    // so that your predictLabel knows whether to return NO_PREDICTION or a valid default class label
    modelTrained = true;
    
    return (1);
}


int predictLabel(double *sample, int numFeatures)
{
    // sanity checking - make sure that data has been read first
    if(validLabels[0] == NO_PREDICTION)
        PrintThisMessageAndExit("predictLabel called before StoreData() so can't know what the valid labels are");
    

   
    /*  your code to complete this function goes here start by copy-pasting the pseudo code for
     * predict() from the lecture slide with the title "Implementing Rule Induction in my code
     * framework"and then code to that
     */

    /*SET class = NO_PREDICTION
    SET numRules = workingCandidate.size / 4
    FOR (i = 0; i < numRules AND class == NO_PREDICTION; i++)
        GetRuleFromWorkingCandidate // as above
        IF (sample[rule.variableAffected] rule.comparison actual_threshold)
            THEN class = prediction ---> thePrediction = PredictClassFromRule(Rule, sample, numFeatures);
    RETURN class */

    //workingCandidate holds the solution (ruleset) that is being constructed

    int prediction = NO_PREDICTION;
    int numRules = workingCandidate.size / VALUES_PER_RULE;

    rule Rule;

    for(int i = 0; i < numRules && prediction == NO_PREDICTION; i++)
    {
        //Get rules from workingCandidate
        int init = i * VALUES_PER_RULE;

        //Composition of the rule struct
        Rule.variableAffected = workingCandidate.variableValues[init];
        Rule.comparison = workingCandidate.variableValues[init + 1];
        Rule.threshold = workingCandidate.variableValues[init + 2];
        Rule.prediction = workingCandidate.variableValues[init + 3];

        //Test rule
        prediction = PredictClassFromRule(Rule, sample, NUM_FEATURES); //Instead of the IF statement
    }

    //NB possible for thePrediction still to be NO_PREDICTION if no rule covers the example
    // during training it's ok to return NO_PREDICTION
    // but in use (i.e., once you've finished training) you have to add code to return a valid class label

    /*IF(training finished AND the prediction is NO_PREDICTION)
        THEN set the prediction to a valid class */

    if(modelTrained == true && prediction == NO_PREDICTION)
        prediction = 1;

    return prediction;
}


void GreedyConstructiveSearch(void) {
    int bestSoln; //Best solution
    bool atGoal = false;
    rule newrule;
    int variable, operator, threshold, prediction;

    //this variable is used to store a copy of the working candidate at the start of every iteration
    //so that we can repeatedly add different rules to it
    candidateSolution tmp;


    // initialise the variables used for search e.g. workingCandidate, openList and closedList
    CleanListsOfSolutionsToStart();
    CleanWorkingCandidate();
    workingCandidate.score = 0;
    atGoal = false;
    CleanCandidate(&tmp);
    tmp.score = -1;

    // initial working Candidate has no rules in so must score zero for the number of training set items correctly classified
    AddWorkingCandidateToOpenList();

    //================= YOUR CODE GOES HERE  ===========//
    // ====== copy the train() pseudo code form the lecture slides the code it line by line
    // but use (atGoal==false) as the condition of  the while loop
    //  and set atGoal = goalFound() at the ned of each iteration
    /*
    WHILE(atGoal == false) DO
        SET tmp = workingCandidate; //make a copy so we can repeatedly edit it

        FOREACH  (possible rule)    ------> 4 nested for loops for each rule
             SET workingCandidate = tmp //reset to original
             CHANGE workingCandidate by Adding Rule(rule)
             Score(workingCandidate)
             IF (errors (workingCandidate) ==0)
                 AddWorkingCandidateToOpenList()

        ADD tmp to closed list
        SORT OpenList by decreasing number of correct predictions
        SET atGoal = goalFound()
        EMPTY OpenList
     RETURN workingCandidate
     */

    while (atGoal == false) {
        tmp = workingCandidate;
        rule Rule;

        for(variable = 0; variable < NUM_FEATURES; variable++)
        {
            for(operator = 0; operator < 3; operator++)
            {
                for(threshold = 0; threshold < THRESHOLD_PRECISION; threshold++)
                {
                    for(prediction = 0; prediction < numClasses; prediction++)
                    {
                        workingCandidate = tmp;

                        Rule.variableAffected = variable;
                        Rule.comparison = operator;
                        Rule.threshold = threshold;
                        Rule.prediction = validLabels[prediction];

                        ExtendWorkingCandidateByAddingRule(Rule);
                        ScoreWorkingCandidateOnTrainingSet();

                        if (workingCandidate.score >= tmp.score)
                            AddWorkingCandidateToOpenList();
                    }
                }
            }
        }

        AddWorkingCandidateToClosedList();

        //Sort openList by increasing distance to goal
        int solution = 0;

        for (int i = 0; i < openList.size; i++)
            if ((openList.listEntries[i].score >= openList.listEntries[solution].score))
                solution = i;

        //Copy the best solution into working candidate
        CopySolutionFromOpenListIntoWorkingCandidate(solution);

        atGoal = GoalFound();

        //Empty open list
        AddWorkingCandidateToOpenList();
        CleanListsOfSolutionsToStart();
    }

    printWorkingCandidate();

    //==================don't change anything below here
    
    if(!atGoal)
        printf("Left Search without classifying all examples\n");
 
    
   //make sure that workingCandidate holds a copy of the final rule set before leaving this function
    //DON'T CHANGE THESE NEXT FOUR LINES
    if(workingCandidate.score >= tmp.score   )
        CopySolution(&workingCandidate, &tmp);
    else
        CopySolution(&tmp, &workingCandidate);
}
       
void ScoreWorkingCandidateOnTrainingSet(void)
{
    int numWrong = 0, numRight = 0;

    int thePrediction = NO_PREDICTION;
    //safety checking
    if(trainingSetSize <= 0)
        PrintThisMessageAndExit("called ScoreWorkingCandidateOnTrainingSet() before training set has been loaded\n");
    
    //loop through the training set
    for(int nextSample = 0; nextSample < trainingSetSize; nextSample++)
    {
        //make prediction
        thePrediction =  predictLabel(myModel[nextSample], trainingSetFeatures);
        //increment score if correct
        if (thePrediction == myModelLabels[nextSample])
            numRight++;
        else if (thePrediction != NO_PREDICTION)
            numWrong++;
    }
 

    if(numWrong > 0)
        workingCandidate.score = -1; // worse to make wrong predictions than no predictions *for this algorithm*
    else
        workingCandidate.score = numRight;

}

int PredictClassFromRule(rule thisRule, double * sample, int numFeatures )
{
    int prediction = NO_PREDICTION;
    double percent, range, theThreshold;
    int feature = thisRule.variableAffected;
    //check the sample contains a value for the variable the rule covers
    if( feature < 0 || feature >= numFeatures  )
        printf("rule uses variable not present in sample");
    else
    {
        percent = (double) thisRule.threshold/100.0;
        range = maxVal[feature] - minVal[feature];
        theThreshold = minVal[feature] + percent*range;
    //apply the operator
        switch(thisRule.comparison)
        {
            case lessThan:
                if (sample[feature] < theThreshold)
                    prediction = thisRule.prediction;
                break;
            case equals:
                if (sample[feature] == theThreshold)
                    prediction = thisRule.prediction;
                break;
            case greaterThan:
                if (sample[feature] > theThreshold)
                    prediction = thisRule.prediction;
                break;
            default: PrintThisMessageAndExit("rule contains unknown comparator");
        }
    }
    return prediction;
}



//================ DON'T CHANGE ANYTHING BELOW THIS LINE ===================//


bool GoalFound(void)
{
    if(workingCandidate.score < trainingSetSize && workingCandidate.size < VALUES_PER_RULE*MAX_NUM_RULES)
        return(false);
    else
        return true;
}
/// function that prints out the working candidate as a rule set
void printWorkingCandidate (void)
{
    int index, rule, numrules;
    
    //check it exists and is not empty
    if (workingCandidate.size == 0)
        PrintThisMessageAndExit("workingCandidate does not seem to hold any rules\n ");
 
  else
  {
      numrules = workingCandidate.size/VALUES_PER_RULE;
      printf("Learned model  has %d rules and scores %d:\n",  numrules, workingCandidate.score);
      for( rule=0;rule<numrules; rule++)
        {
            index = rule*4;
            int feature = workingCandidate.variableValues[index];
            int operator = (workingCandidate.variableValues[index+1]);
            char op = (operator == lessThan)? '<': (operator== equals)? '=':'>';
            int thresh = (workingCandidate.variableValues[index+2]);
            double percent = (double) thresh/100.0;
            double range = maxVal[feature] - minVal[feature];
            double theThreshold = minVal[feature] + percent*range;
            int prediction = workingCandidate.variableValues[index+3];
            
            printf("\t%s feature %d %c %lf  CLASS = %d\n",(rule==0)?"IF": "ELSE IF",feature,op, theThreshold, prediction);
 
          }
  }
}



void ExtendWorkingCandidateByAddingRule(rule newRule)
{
 extern candidateSolution workingCandidate;
    //check it isn't already full
    if (workingCandidate.size==N)
        {
            PrintThisMessageAndExit (" can't add rule to already full working candidate");
        }
    else
     {
         ExtendWorkingCandidateByAddingValue(newRule.variableAffected);
         ExtendWorkingCandidateByAddingValue(newRule.comparison);
         ExtendWorkingCandidateByAddingValue(newRule.threshold);
         ExtendWorkingCandidateByAddingValue(newRule.prediction);

     }
}


void prepareTrainingDataArrays(int numSamples, int numFeatures)
{
    int feature, sample, label;
    //clean the arrays because C leaves whatever is in the memory
     for ( sample=0; sample < NUM_TRAINING_SAMPLES; sample++)
     {
         myModelLabels[sample]= UNUSED;
         for ( feature=0; feature<NUM_FEATURES; feature++)
             myModel[sample][feature] = 0.0;
     }
     for ( label=0; label<256; label++)
         validLabels[label] = NO_PREDICTION;
    
     
     for(feature=0;feature < NUM_FEATURES;feature++)
       {
       minVal[feature] = BIG_DBL; //no point guessing what the smallest possible value is
       maxVal[feature] = 0.0;
     }
     
     //sanity checking
     if ( numFeatures > NUM_FEATURES || numSamples > NUM_TRAINING_SAMPLES)
         PrintThisMessageAndExit ("error: called train with data set larger than spaced allocated to store it");
         
}


void StoreData(double **trainingSamples, int *trainingLabels, int numSamples, int numFeatures)
{

int label,sample, feature;
int thisLabel;
double thisVal;
int labelUsed[256];
    
    modelTrained = false;
    for(label=0;label<256;label++)
        labelUsed[label] = 0;
    prepareTrainingDataArrays(numSamples, numFeatures);
    
    //store the data
    trainingSetFeatures = numFeatures;
    trainingSetSize = numSamples;
    for (sample=0; sample < numSamples; sample++)
        {
            myModelLabels[sample] = trainingLabels[sample];
            for (feature=0; feature < numFeatures; feature++)
              {
                  thisVal = trainingSamples[sample][feature];
                  myModel[sample][feature] = thisVal;
                  if (thisVal < minVal[feature])
                      minVal[feature] = thisVal;
                  if (thisVal > maxVal[feature])
                      maxVal[feature] = thisVal;
              }
              //record which labels have been used
              thisLabel = trainingLabels[sample];
              labelUsed[thisLabel] ++ ;
          }
    // work out how many, and which,  labels were present in the training set
    numClasses = 0;
    for(label=0;label < 256;label++)
        {
        if( labelUsed[label]>0)
           {
            validLabels[numClasses] = label;
            numClasses++;
            }
        }
           
    fprintf(stdout,"data stored locally with %d examples and %d features\n",trainingSetSize,trainingSetFeatures);
    for(feature=0;feature<trainingSetFeatures;feature++)
        printf("\tfeature %d has minVal %f and maxVal %f \n", feature, minVal[feature],maxVal[feature]);
}


int main(int argc,char**argv){
    extern int Xmain(void);
    return Xmain( );
}