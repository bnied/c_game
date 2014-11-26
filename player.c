#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "player.h"

// Struct methods; create/destroy our player and the enemy
struct Player *Player_create(char *name) {
  struct Player *player = malloc(sizeof(struct Player));
  assert(player != NULL);

  player->name    = name;
  player->health  = 100;
  player->attack  = rand() % 5;
  player->defense = rand() % 4;
  player->dead    = 0;

  return player;
}

void Player_destroy(struct Player *player){
  assert(player != NULL);
  free(player);
}

void Player_print(struct Player *player) {
  printf("Name: %s", player->name);
  printf("Health: %d\n", player->health);
  printf("ATK: %d\n", player->attack);
  printf("DEF: %d\n\n", player->defense);
}


/* The die roll determines what kind of damage we take:
 * - Normal (0): Health - (ATK - DEF)
 * - Critical (1): (Health - (ATK - DEF)) * 2
 * - Glancing (2): (Health - (ATK - DEF)) / 2
 */
void player_damage(struct Player *player, int damage, int die_roll) {
  float multiplier = 1.0;

  switch(die_roll) {
    default:
      multiplier = 1.0;
      break;

    case 1:
      multiplier = 2.0;
      printf("CRITICAL HIT: ");
      break;

    case 2:
      multiplier = 0.5;
      printf("GLANCING BLOW: ");
      break;
  }

  int final_damage = (int)(damage * multiplier) - player->defense;

  if (final_damage > 0) {
    printf("The enemy attacks for %d damage!\n", final_damage);
    player->health -= final_damage;
  } else {
    printf("The enemy's attack causes no damage.\n");
  }
}

// Check if our player is dead
int check_player_death(struct Player *player) {
  if(player->health <= 0) {
    player->dead = 1;
    return 1;
  } else {
    return 0;
  }
}