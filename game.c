/* © 2013 Benjamin Nied
 * This is a game program designed to simulate a battle between two entities
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "player.h"
#include "enemy.h"

#define NAME_LENGTH 40
#define SCREEN_ROWS 100

// Clear the screen
void clear_screen() {
    // Clear the screen
    int i;
    for (i = 0; i < SCREEN_ROWS; i++) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    // Setup; get the player's name and generate our characters
    char name_input[NAME_LENGTH];
    printf("Hello and welcome! What is your name?\n");
    char *name = fgets(name_input, NAME_LENGTH, stdin);
    struct Player *player = Player_create(name);
    struct Enemy *enemy = Enemy_create();
    
    // DEBUG
    //printf("Player's Memory Location: %p.\n", player);
    //printf("Enemy's Memory Location: %p.\n", enemy);

    // Seed our RNG
    srand(time(NULL));

    // Clear the screen and show the player/enemy stats
    clear_screen();
    
    // Start the game. The introduction...
    printf("An enemy approaches...\n");
    printf("Your enemy's statistics:\n");
    Enemy_print(enemy);
    printf("Your statistics:\n");
    Player_print(player);
    sleep(5);

    // Now clear the screen and begin the game proper
    clear_screen();
    
    // Endless loop; keep going until one of the characters is dead.
    while (1) {
        // Player's turn
        int player_critical_roll = rand() % 3;
        int player_attack_roll = rand() % 3;

        switch(player_attack_roll) {
            default:
                player->attack = 3;
                break;

            case 2:
                player->attack = 5;
                break;

            case 3:
                player->attack = 10;
                break;
        }

        enemy_damage(enemy, player->attack, player_critical_roll);
        
        // If the enemy is dead, break the loop and end the game.
        int dead_enemy = check_enemy_death(enemy);
        if (dead_enemy) {
            printf("You killed the enemy! You win, %s.", player->name);
            break;
        }
        printf("The enemy's health is now %d.\n\n", enemy->health);
        
        // Give the player a chance to read up.
        sleep(1);
        
        // Enemy's turn. Two rolls: one for crit and one for attack damage.
        int enemy_critical_roll = rand() % 3;
        int enemy_attack_roll = rand() % 3;

        switch(enemy_attack_roll) {
            default:
                enemy->attack = 3;
                break;

            case 1:
                enemy->attack = 5;
                break;

            case 2:
                enemy->attack = 10;
                break;
        }
        
        player_damage(player, enemy->attack, enemy_critical_roll);
        
        // If the enemy killed the player, break the loop and quit the game.
        int dead_player = check_player_death(player);
        if (dead_player) {
            printf("The enemy has killed you! You lose, %s.", player->name);
            sleep(2);
            break;
        }
        printf("Your health is now %d.\n\n", player->health);
        
        // Give the player a chance to read up.
        sleep(1);
        
        //clear_screen();
    }

    Player_destroy(player);
    Enemy_destroy(enemy);
    return 0;
}