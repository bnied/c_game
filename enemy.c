#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "enemy.h"

struct Enemy *Enemy_create() {

  struct Enemy *enemy = malloc(sizeof(struct Enemy));
  assert(enemy != NULL);

  // Seed the RNG
  srand((int)enemy);

  enemy->name    = "The Bad Guy";
  enemy->health  = ((rand() % 25) * 4) + 1;
  enemy->attack  = (rand() % 5) + 1;
  enemy->defense = (rand() % 4) + 1;
  enemy->dead    = 0;

  return enemy;
}

void Enemy_destroy(struct Enemy *enemy){
  assert(enemy != NULL);
  free(enemy);
}

void Enemy_print(struct Enemy *enemy) {
  printf("Name: %s\n", enemy->name);
  printf("Health: %d\n", enemy->health);
  printf("ATK: %d\n", enemy->attack);
  printf("DEF: %d\n\n", enemy->defense);
}

/* The die roll determines what kind of damage we do:
 * - Normal (0): Health - (ATK - DEF)
 * - Critical (1): (Health - (ATK - DEF)) * 2
 * - Glancing (2): (Health - (ATK - DEF)) / 2
 */
void enemy_damage(struct Enemy *enemy, int damage, int die_roll) {
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

  int final_damage = (int)(damage * multiplier) - enemy->defense;

  if (final_damage > 0) {
    printf("You attack for %d damage!\n", final_damage);
    enemy->health -= final_damage;
  } else {
    printf("Your attack causes no damage.\n");
  }
}

// Check if our enemy is dead
int check_enemy_death(struct Enemy *enemy){
  if(enemy->health <= 0) {
    enemy->dead = 1;
    return 1;
  } else {
    return 0;
  }
}