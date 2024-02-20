/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "questions.h"

question questions[NUM_QUESTIONS];

    char* GREEN_EC =  "\033[1;32m";
    char* PINK_EC =  "\033[1;35m";
    char* YELLOW_EC = "\033[1;33m";
    char* RESET_EC =  "\033[0m";


// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    for(int i = 0; i < NUM_QUESTIONS; i++){
        strcpy(questions[i].category, categories[i % 3]);
    	questions[i].answered = false;
    }

// Programming Category Questions
strcpy(questions[0].category, "Programming");
questions[0].value = 200;
strcpy(questions[0].question, "This programming paradigm focuses on the use of functions without changing state or data");
strcpy(questions[0].answer, "Functional Programming");

questions[1].value = 400;
strcpy(questions[1].category, "Programming");
strcpy(questions[1].question, "Named after a British mathematician, this form of notation expresses the logic of computer programs without specifying a particular programming language");
strcpy(questions[1].answer, "BNF (Backus-Naur Form)");

questions[2].value = 600;
strcpy(questions[2].category, "Programming");
strcpy(questions[2].question, "This term describes the concept of hiding the internal implementation details of a class and only showing the necessary features of an object to the outside world");
strcpy(questions[2].answer, "Encapsulation");

questions[3].value = 800;
strcpy(questions[3].category, "Programming");
strcpy(questions[3].question, "This programming model allows for the execution of multiple threads or processes to optimize the use of CPU and processing time");
strcpy(questions[3].answer, "Concurrency");

// Algorithms Category Questions
questions[4].value = 200;
strcpy(questions[4].category, "Algorithms");
strcpy(questions[4].question, "This algorithm class is characterized by dividing a problem into subproblems of the same type and recursively solving them");
strcpy(questions[4].answer, "Divide and Conquer");

questions[5].value = 400;
strcpy(questions[5].category, "Algorithms");
strcpy(questions[5].question, "This famous algorithm is used to find the shortest path from a starting node to all other nodes in a weighted graph ____'s algorithm");
strcpy(questions[5].answer, "Dijkstra");

questions[6].value = 600;
strcpy(questions[6].category, "Algorithms");
strcpy(questions[6].question, "This sort algorithm improves on bubble sort by only passing through the list until no swaps are needed");
strcpy(questions[6].answer, "Cocktail Shaker Sort");

questions[7].value = 800;
strcpy(questions[7].category, "Algorithms");
strcpy(questions[7].question, "This advanced algorithm is used for text searching, allowing for mismatches, and is known for its efficiency in 'fuzzy' matching");
strcpy(questions[7].answer, "KMP Algorithm"); //(Knuth-Morris-Pratt)

// Databases Category Questions
questions[8].value = 200;
strcpy(questions[8].category, "Databases");
strcpy(questions[8].question, "This type of database model organizes data into one or more tables where data types may be related to each other; these relationships help structure the data");
strcpy(questions[8].answer, "Relational Database");

questions[9].value = 400;
strcpy(questions[9].category, "Databases");
strcpy(questions[9].question, "This SQL clause is used to filter rows returned by the SELECT statement based on specified conditions");
strcpy(questions[9].answer, "WHERE");

questions[10].value = 600;
strcpy(questions[10].category, "Databases");
strcpy(questions[10].question, "In database management, this property ensures that transactions are processed reliably and that the database will keep its integrity even in the event of a failure");
strcpy(questions[10].answer, "Atomicity");

questions[11].value = 800;
strcpy(questions[11].category, "Databases");
strcpy(questions[11].question, "This NoSQL database type is optimized for retrieval and appending operations, making it ideal for analytics and real-time web applications");
strcpy(questions[11].answer, "Document Store Database");

}

// Displays each of the remaining categories and question dollar values that have not been answered
int display_categories(void) {
    
    
    // Print category headers
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s%15s%s ", YELLOW_EC, categories[i], RESET_EC);
    }
    printf("\n");

    // Assuming an equal number of questions per category for simplicity
    int questionsPerCategory = NUM_QUESTIONS / NUM_CATEGORIES;
    

    uint8_t anyRemaining = 0;

    for (int q = 0; q < questionsPerCategory; q++) {
        for (int c = 0; c < NUM_CATEGORIES; c++) {
            int questionIndex = c * questionsPerCategory + q;
            
            if (!questions[questionIndex].answered) { //Unanswered
                anyRemaining = 1;
                printf("%s%15d%s ", GREEN_EC, questions[questionIndex].value, RESET_EC);
            } else {
                printf("%s%15s%s ", PINK_EC, "[XX]", RESET_EC); //Answered a.k.a used up
            }

        }
    printf("\n");

    }


    //Return wether the game is terminated or not:
    return anyRemaining;
}                                                                                                   

// Implement display_question
void display_question(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            printf("%sQuestion: %s\n,",RESET_EC, questions[i].question);
            return;
        }
    }
}

// Implement valid_answer
bool valid_answer(char *category, int value, char *answer) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return strcmp(questions[i].answer, answer) == 0;
        }
    }
    return false;
}

// Implement already_answered
bool already_answered(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return questions[i].answered;
        }
    }
    return false;
}