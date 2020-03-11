/*
 * File:   main.c
 * Author: j4-smith
 *RuleInduction example in C
 * Jim Smith 2020
 */


#include <stdio.h>
#include <stdlib.h>

#include "StructureDefinitions.h"
#include "SolutionListOperations.h"
#include "TrainAndTest.h"
#include "ExampleData.h"



int Xmain(void)
{
    
    int sample, trainingsamples=0,testsamples=0,feature=0;
    int ok=1;
    int correct=0, wrong=0;
    int prediction;
    int returnval=0;
    double   mySample[NUM_FEATURES];
    
    int confusionMatrix[3][4];
    
    //allocate space for data storage
    double **trainingSet = calloc(NUM_TRAINING_SAMPLES , sizeof(double *));
    for(sample=0;sample < NUM_TRAINING_SAMPLES; sample++)
        trainingSet[sample] = calloc(NUM_FEATURES, sizeof(double));
    
                                  
    int *trainingLabels = calloc(NUM_TRAINING_SAMPLES , sizeof(int));
    
    double **testSet = calloc(NUM_TEST_SAMPLES , sizeof(double *));
    for(sample=0;sample < NUM_TEST_SAMPLES; sample++)
        testSet[sample] = calloc(NUM_FEATURES, sizeof(double));

    int * testLabels = calloc(NUM_TEST_SAMPLES, sizeof(int));
    

    
    //simple 2/3: 1/3 split of iris data into training and test set matrices
    for(sample=0; sample< TEST_SET_SIZE;  sample++)
      {
        if(sample%3==0)
          {
            for (feature=0;feature < TESTFEATURES;  feature++)
              testSet[testsamples][feature] = test_data[sample][feature];

              testLabels[testsamples] =test_labels[sample];
            testsamples++;
          }
        else
          {
            for (feature=0;feature < TESTFEATURES;  feature++)
              trainingSet[trainingsamples][feature] = test_data[sample][feature];
            
            trainingLabels[trainingsamples] = test_labels[sample];
            trainingsamples++;
          }
      }
    
    
    
    //call the train function
    ok= train(  &trainingSet[0], trainingLabels ,trainingsamples, NUM_FEATURES);
    if( ok!= 1)
      {
        printf("there was a problem running the train() function\n");
        returnval=0;
      }
    
    else
      {
        printf("\n Testing learned classifier \n");
          
        //print the results from the training set for information
        printf("On the training set:\n");
       //clean the confusion matrix
          for(int actual=0;actual < 3;actual++)
              for(int predicted=0;predicted< 4;predicted++)
                  confusionMatrix[actual][predicted] = 0;
          
          
        for(sample=0; sample < trainingsamples; sample++)
          {
            
            // Make a copy of the sample ot be classified because it is faster to access a local copy
            //and pass a pointer to it, and also  I don't want to overwrite it by accident
            for(feature = 0 ; feature < NUM_FEATURES; feature++)
                mySample[feature] = trainingSet[sample][feature];
            
            prediction = predictLabel( mySample, NUM_FEATURES);
            if (prediction == trainingLabels[sample])
                correct++;
            else
              wrong++;
            //if(prediction==NO_PREDICTION)
            //    prediction = 3;
            confusionMatrix[trainingLabels[sample]][prediction]++;
          }
        
        printf("\t%d correct %d incorrect, accuracy = %f %%\n", correct,wrong, 100.0*(float)correct/(correct+wrong));
        printf("\tThe confusion matrix is: \n");
         printf("\t\tPrediction | 0   | 1   |  2  | other |   \n");
         printf("\t\t -------------------------------------\t\t\n");
          printf("\t\t Target 0  | %3d | %3d | %3d |  %3d  | \n", confusionMatrix[0][0], confusionMatrix[0][1],confusionMatrix[0][2],confusionMatrix[0][3]);
          printf("\t\t Target 1  | %3d | %3d | %3d |  %3d  | \n", confusionMatrix[1][0], confusionMatrix[1][1],confusionMatrix[1][2],confusionMatrix[2][3]);
          printf("\t\t Target 2  | %3d | %3d | %3d |  %3d  |\n", confusionMatrix[2][0], confusionMatrix[2][1],confusionMatrix[2][2],confusionMatrix[2][3]);
         printf("\t\t -------------------------------------\n\n\n");

        //now the results that matter - from the test set
        printf("On the test set:\n");        correct=wrong=0;
        //clean the confusion matrix
          for(int actual=0;actual < 3;actual++)
                for(int predicted=0;predicted< 4;predicted++)
                    confusionMatrix[actual][predicted] = 0;
          
        for(sample=0; sample < testsamples; sample++)
          {
            //copy into temp array ot pass into fiunction
            for(feature = 0 ; feature < NUM_FEATURES; feature++)
                mySample[feature] = testSet[sample][feature];
            
            prediction = predictLabel(mySample, NUM_FEATURES);
            if (prediction == testLabels[sample])
                correct++;
            else
              wrong++;
            //if(prediction==NO_PREDICTION)
            //   prediction = 3;
            confusionMatrix[testLabels[sample]][prediction]++;
          }
        
          printf("\t%d correct %d incorrect, accuracy = %f %%\n", correct,wrong, 100.0*(float)correct/(correct+wrong));
          printf("\tThe confusion matrix is: \n");
          printf("\t\tPrediction | 0   | 1   |  2  | other |   \n");
          printf("\t\t -------------------------------------\t\t\n");
          printf("\t\t Target 0  | %3d | %3d | %3d |  %3d  | \n", confusionMatrix[0][0], confusionMatrix[0][1],confusionMatrix[0][2],confusionMatrix[0][3]);
          printf("\t\t Target 1  | %3d | %3d | %3d |  %3d  | \n", confusionMatrix[1][0], confusionMatrix[1][1],confusionMatrix[1][2],confusionMatrix[2][3]);
          printf("\t\t Target 2  | %3d | %3d | %3d |  %3d  |\n", confusionMatrix[2][0], confusionMatrix[2][1],confusionMatrix[2][2],confusionMatrix[2][3]);
          printf("\t\t -------------------------------------\n\n\n");

        
      }
    
    
    return returnval;
    
}
