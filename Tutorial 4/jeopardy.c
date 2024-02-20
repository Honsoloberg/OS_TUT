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
#include <stdbool.h>
#include <ctype.h>

#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Implement tokenize function
void tokenize(char *input, char **tokens) {
    const char *delim = " ";
    char *ptr = strtok(input, delim);
    int i = 0;
    while(ptr != NULL) {
        tokens[i++] = ptr;
        ptr = strtok(NULL, delim);
    }
}

// Implement show_results function
void show_results(player *players, int num_players) {
    // Simple bubble sort for demonstration; consider using a more efficient sorting algorithm for larger datasets
    for (int i = 0; i < num_players - 1; i++) {
        for (int j = 0; j < num_players - i - 1; j++) {
            if (players[j].score < players[j + 1].score) {
                player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
    // Display sorted results
    printf("\nGame Results:\n");
    for (int i = 0; i < num_players; i++) {
        printf("Player: %s, Score: %d\n", players[i].name, players[i].score);
    }
}


int main(int argc, char *argv[]) {
    (void)argc; // Optionally suppress unused parameter warning
    (void)argv; // Optionally suppress unused parameter warning

    player players[NUM_PLAYERS];
    char buffer[BUFFER_LEN] = {0};
    char category[BUFFER_LEN] = {0};
    int value, i;
    bool gameEnded = false;

    // Display the game introduction and initialize the questions
    initialize_game();
    printf("Let's Play Jeopardy!\n");

    // Prompt for players names
    for(int i = 0; i < NUM_PLAYERS; i++){
        players[i].score = 0;
        printf("\nPlease Enter Player %d's Name: ", i+1);
        scanf("%s", players[i].name);
    }

    // Clear input buffer
    while (getchar() != '\n');

    display_categories();
    display_players(&players,4);

    while (!gameEnded) {
        int currentPlayerIndex = rand() % NUM_PLAYERS; // Pick a random player index
        printf("\nPlayer %s, choose a category and value (e.g., History 200): ", players[currentPlayerIndex].name);
        fgets(buffer, BUFFER_LEN, stdin);
        sscanf(buffer, "%s %d", category, &value);

        for(i = 0; i < NUM_QUESTIONS; i++) {
            if(strcmp(questions[i].category, category) == 0 && questions[i].value == value && !questions[i].answered) {
                display_question(category, value);

                printf("Player %s, your answer (without 'what is' or 'who is'): ", players[currentPlayerIndex].name);
                fgets(buffer, BUFFER_LEN, stdin);
                buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

                if (valid_answer(category, value, buffer)) {
                    printf("Correct!\n");
                    players[currentPlayerIndex].score += questions[i].value; // Update player's score
                } else {
                    printf("Incorrect. The correct answer was: %s\n", questions[i].answer);
                }

                questions[i].answered = true;
                break; // Exit the loop once the question is found and processed
            }
        }

        if (i == NUM_QUESTIONS) {
            printf("Question not found or already answered. Please try again.\n");
        }

        // Optional: Implement criteria to check if game should end
        // gameEnded = true; // Update this based on your game's ending conditions

        int game_status = display_categories(); 

        if(game_status == 0){
            rank_players(&players, NUM_PLAYERS);
            break;
        } else {
            display_players(&players,NUM_PLAYERS);
        }
    }

    // Show final results
    //show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
