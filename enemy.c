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
	
	enemy->name    = "The Bad Guy";
	enemy->health  = (rand() % 100) * 3;
	enemy->attack  = rand() % 5;
	enemy->defense = rand() % 2;
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
	int new_damage;

	switch(die_roll){
		default:
			new_damage = damage - enemy->defense;
			printf("You attack for %d damage!\n", new_damage);
			break;

		case 1:
			new_damage = (damage - enemy->defense)*2;
			printf("Critical hit!\nYou attack for %d damage!\n", new_damage);
			break;
 
		case 2:
			new_damage = (int)(damage / 2) - enemy->defense;
			if (new_damage < 0) {
				new_damage = 0;
			}
			printf("Your blow glances off the enemy and only causes %d damage!\n", new_damage);
			break;
	}
	if(new_damage > 0) {
		enemy->health -= new_damage;
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