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
#define SCREEN_ROWS 80

// Clear the screen
void clear_screen() {
  // Clear the screen
  for (int i = 0; i < SCREEN_ROWS; i++) {
    printf("\n");
  }
}

// Emulate a 3-sided die
int roll_dice() {
  return rand() % 3;
}

int main(int argc, char *argv[]) {

  // Setup; get the player's name and generate our characters
  char name_input[NAME_LENGTH];
  printf("Hello and welcome! What is your name?\n");
  char *name = fgets(name_input, NAME_LENGTH, stdin);
  struct Player *player = Player_create(name);
  struct Enemy *enemy = Enemy_create();

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
    // The roll determines crit
    enemy_damage(enemy, player->attack, roll_dice());

    // If our enemy is dead, break the loop and end the game
    if (enemy->dead) {
      printf("You killed the enemy! You win, %s.", player->name);
      break;
    }
    printf("The enemy's health is now %d.\n\n", enemy->health);

    // Give the player a chance to read up.
    sleep(2);

    // Enemy's turn

    // The roll determines crit
    player_damage(player, enemy->attack, roll_dice());

    // If the enemy killed the player, break the loop and quit the game.
    if (player->dead) {
      printf("The enemy has killed you! You lose, %s.", player->name);
      break;
    }
    printf("Your health is now %d.\n\n", player->health);

    // Give the player a chance to read up.
    sleep(2);
  }

  Player_destroy(player);
  Enemy_destroy(enemy);
  return 0;
}
