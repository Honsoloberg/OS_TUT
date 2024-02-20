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
#include "players.h"

bool player_exists(player *players, int num_players, char *name) {
    for (int i = 0; i < num_players; i++) {
        if (strcmp(players[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

void update_score(player *players, int num_players, char *name, int score) {
    for (int i = 0; i < num_players; i++) {
        if (strcmp(players[i].name, name) == 0) {
            players[i].score += score;
            return;
        }
    }
}

#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GRAY   "\x1b[90m"
#define ANSI_COLOR_RESET  "\x1b[0m"
char* AQUA_EC = "\033[1;36m";   // Aqua color

void display_players(player *players, int num_players) {
    printf("\n\033[0;37m******************************************\n");
        for (int i = 0; i < num_players; i++) {
            printf("%sPlayer: %s%s , : %d $ \n", ANSI_COLOR_RED, players[i].name, AQUA_EC, players[i].score);
        }
    printf("\033[0m");
}

// Function to compare players for sorting in descending order
int comparePlayers(const void *a, const void *b) {
    return ((player *)b)->score - ((player *)a)->score;
}

// ANSI escape codes for text color


// Function to display players with ranking and color formatting
void rank_players(player *players, int num_players) {
    
    printf("\n\033[0;37m******************************************\n");
    
    // Sort players in descending order based on score
    qsort(players, num_players, sizeof(player), comparePlayers);

    // Display players with ranking and color formatting
    printf("Rank\tName\t\tScore\n");
    for (int i = 0; i < num_players; i++) {
        const char *rankColor = (i == num_players - 1) ? ANSI_COLOR_GRAY : (i == num_players - 2) ? ANSI_COLOR_YELLOW : (i == num_players - 3) ? ANSI_COLOR_GREEN : ANSI_COLOR_RED;
        printf("%s%d%s\t%s\t\t%d%s\n", rankColor, i + 1, (i == num_players - 1) ? "th" : (i == num_players - 2) ? "rd" : (i == num_players - 3) ? "nd" : "st",
               players[i].name, players[i].score, ANSI_COLOR_RESET);
    }
}