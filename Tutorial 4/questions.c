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
#include "questions.h"

// Initializes the array of questions for the game
void initialize_game(void)
{
    //Initilize programming questions
    struct question P1 = {category[0], "In object-oriented programming, what term describes the process of creating a new instance of a class?", "instantiation", 100, false};
    struct question P2 = {category[0], "In web development, what does CSS stand for?", "cascading style sheet", 200, false};
    struct question P3 = {category[0], "Which programming language, developed by Apple Inc., is commonly used for building applications for IOS and macOS platforms?", "swift", 300, false};
    struct question P4 = {category[0], "What programming paradigm emphasizes the use of mathematical function s and immutable data, avoiding side effects and mutable state?", "functional programming", 400, false};

    questions[0] = P1;
    questions[1] = P2;
    questions[2] = P3;
    questions[3] = P4;

    //Initilize Algorithms questions
    struct question A1 = {category[1], "What sorting algorithm works by repeatedly stepping through the list, comparing adjacent elements and swapping them if they are in the wrong roder?", "bubble sort", 100, false};
    struct question A2 = {category[1], "In computer science, what is the worst-case time complexity of the quicksort algorithm?", "O(n^2)", 200, false};
    struct question A3 = {category[1], "What type of algorithm solves a problem by dividing it inot smaller subproblems, solving each subproblem recursively, and then combining the solutions?", "divide and conquer", 300, false};
    struct questoin A4 = {category[1], "What algorithm, named after a British mathematician, is used to find the shortest path in a weighted graph?", "dijkstra's algorithm", 400, false};

    questions[4] = A1;
    questions[5] = A2;
    questions[6] = A3;
    questions[7] = A4;

    //Initilize Databases questions

    struct question D1 = {category[2], "In SQL, what keyword is used to retrieve data from a database table?", "select", 100, false};
    struct question D2 = {category[2], "What type of database model organizes data into tables, where each table consists of rows and columns, with each row representing a record and each column representing a data attribute?", "relational database model", 200, false};
    struct question D3 = {category[2], "What database system can store all data within one file?", "sqlite", 300, false};
    struct question D4 = {category[2], "What term describes the process of breaking down a large databse into smaller, more manageable parts to improve performance and scalability?", "database sharding", 400, false};

    questions[8] = D1;
    questions[9] = D2;
    questions[10] = D3;
    questions[11] = D4;

}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    // print categories and dollar values for each unanswered question in questions array
    
    //print categories
    for(int i=0; i<3; i++){
        printf("%s\t", category[i]);
    }

    printf("\n");

    // //print questions in categories
    // for(int i=0; i<4; i++){
    //     for(int j=0; j<3; j++){
    //         if(questions[i+(j*4)].answered == false)
    //             printf("%d\t", questions[i+(j*4)].value);
    //         else{
    //             printf("closed\t");
    //         }
    //     }
    //     printf("\n");
    // }
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
    for(int i=0; i<NUM_QUESTIONS; i++){
        if(strcmp_s(questions[i].category, category) == 0 && questions[i].value == value){
            printf("%s\n", questions[i].question);
            break;
        }
    }
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
    for(int i=0; i<NUM_QUESTIONS; i++){
        if(strcmp_s(questions[i].category, category) && questions[i].value == value){
            if(strcmp_s(questions[i].answer, answer) == 0){
                return true;
            }
        }
    }
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
    for(int i=0; i<NUM_QUESTIONS; i++){
        if(strcmp_s(questions[i].category, category) == 0 && questions[i].value == value){
            if(questions[i].answered == true){
                return true;
            }
        }
    }
    return false;
}
