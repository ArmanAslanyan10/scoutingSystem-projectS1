#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100

struct Player {
    int id;
    char name[50];
    int goals;
    int matches;
    int injuries;
    int overallPoints; // Starting from 20 points
};

// Function to input player information
void inputPlayerInfo(struct Player *player, int playerNum) {
    printf("\nEnter player %d information:\n", playerNum);

    // Input player ID with validation
    while (1) {
        printf("ID: ");
        if (scanf("%d", &(player->id)) != 1) {
            printf("Please enter a valid numeric ID.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Input player name
    printf("Name: ");
    scanf("%s", player->name);

    // Input goals with validation
    while (1) {
        printf("Goals: ");
        if (scanf("%d", &(player->goals)) != 1) {
            printf("Please enter a valid numeric value for goals.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Input matches with validation
    while (1) {
        printf("Matches: ");
        if (scanf("%d", &(player->matches)) != 1) {
            printf("Please enter a valid numeric value for matches.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Input injuries with validation
    while (1) {
        printf("Injuries: ");
        if (scanf("%d", &(player->injuries)) != 1) {
            printf("Please enter a valid numeric value for injuries.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Calculate overall points based on rules
    player->overallPoints = 20;
    player->overallPoints += player->goals / 5;
    player->overallPoints -= player->injuries / 2;

    if (player->goals < 10) {
        player->overallPoints -= 10;
    }

    if (player->injuries > 15) {
        player->overallPoints -= 12;
    }

    if ((float)player->goals / player->matches > 1.2) {
        player->overallPoints += 6;
    }

    if ((float)player->injuries / player->matches > 0.5) {
        player->overallPoints -= 6;
    }
}

// Function to write player details to file
void writePlayerDetails(FILE *file, struct Player *player) {
    fprintf(file, "Player ID: %d\n", player->id);
    fprintf(file, "Name: %s\n", player->name);
    fprintf(file, "Goals: %d\n", player->goals);
    fprintf(file, "Matches: %d\n", player->matches);
    fprintf(file, "Injuries: %d\n", player->injuries);
    fprintf(file, "Overall Points: %d\n", player->overallPoints);

    // Provide feedback based on overall points
    fprintf(file, "Feedback: ");
    if (player->overallPoints >= 50) {
        fprintf(file, "Pro player!\n");
    } else if (player->overallPoints >= 40) {
        fprintf(file, "Good potential.\n");
    } else if (player->overallPoints >= 30) {
        fprintf(file, "Just good.\n");
    } else if (player->overallPoints >= 20) {
        fprintf(file, "Normal.\n");
    } else if (player->overallPoints >= 10) {
        fprintf(file, "Needs practice.\n");
    } else if (player->overallPoints >= 0) {
        fprintf(file, "Needs to work harder.\n");
    } else {
        fprintf(file, "No talent.\n");
    }

    // Separator for clarity
    fprintf(file, "----------------------------------\n");
}

// Function to add a player to the file
int addPlayerToFile(FILE *file, struct Player *players, int *numPlayers) {
    // Check if the maximum number of players has been reached
    if (*numPlayers >= MAX_PLAYERS) {
        printf("Maximum number of players reached.\n");
        return 0;
    }

    // Input player information and write details to file
    inputPlayerInfo(&players[*numPlayers], *numPlayers + 1);
    writePlayerDetails(file, &players[*numPlayers]);
    (*numPlayers)++; // Increment the number of players

    return 1;
}

// Function to remove a player
int removePlayer(struct Player *players, int *numPlayers, int playerId) {
    int found = 0;
    for (int i = 0; i < *numPlayers; ++i) {
        if (players[i].id == playerId) {
            found = 1;

            // Shift players to fill the gap
            for (int j = i; j < *numPlayers - 1; ++j) {
                players[j] = players[j + 1];
            }

            (*numPlayers)--; // Decrement the number of players
            printf("Player with ID %d removed successfully.\n", playerId);
            break;
        }
    }

    if (!found) {
        printf("Player with ID %d not found.\n", playerId);
    }

    return 1;
}

// Function to search for a player by ID
void searchPlayer(struct Player *players, int numPlayers, int playerId) {
    int found = 0;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].id == playerId) {
            found = 1;
            printf("\nPlayer found:\n");
            printf("ID: %d\n", players[i].id);
            printf("Name: %s\n", players[i].name);
            printf("Goals: %d\n", players[i].goals);
            printf("Matches: %d\n", players[i].matches);
            printf("Injuries: %d\n", players[i].injuries);
            printf("Overall Points: %d\n", players[i].overallPoints);
            break;
        }
    }

    if (!found) {
        printf("Player with ID %d not found.\n", playerId);
    }
}

// Function to display all players
void displayPlayers(struct Player *players, int numPlayers) {
    printf("\nList of Players:\n");
    for (int i = 0; i < numPlayers; ++i) {
        printf("ID: %d, Name: %s\n", players[i].id, players[i].name);
    }
}

int main() {
    FILE *file = fopen("file2.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int numPlayers = 0;
    struct Player players[MAX_PLAYERS];

    int choice;
    do {
        // Display menu
        printf("\nMenu:\n");
        printf("1. Add Player to File\n");
        printf("2. Remove Player\n");
        printf("3. Search Player\n");
        printf("4. Display Players\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        // Validate menu choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear the input buffer
            continue;
        }

        // Consume the newline character left in the buffer
        while (getchar() != '\n');

        // Perform the selected action
        switch (choice) {
            case 1:
                if (!addPlayerToFile(file, players, &numPlayers)) {
                    fclose(file);
                    return 1;
                }
                break;
            case 2:
                if (numPlayers > 0) {
                    int playerId;
                    printf("Enter the ID of the player to remove: ");
                    if (scanf("%d", &playerId) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        while (getchar() != '\n');  // Clear the input buffer
                        continue;
                    }
                    removePlayer(players, &numPlayers, playerId);
                } else {
                    printf("No players to remove.\n");
                }
                break;
            case 3:
                if (numPlayers > 0) {
                    int playerId;
                    printf("Enter the ID of the player to search: ");
                    if (scanf("%d", &playerId) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        while (getchar() != '\n');  // Clear the input buffer
                        continue;
                    }
                    searchPlayer(players, numPlayers, playerId);
                } else {
                    printf("No players to search.\n");
                }
                break;
            case 4:
                if (numPlayers > 0) {
                    displayPlayers(players, numPlayers);
                } else {
                    printf("No players to display.\n");
                }
                break;
            case 5:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    fclose(file);
    printf("\nResults written to file2.txt\n");

    return 0;
}
