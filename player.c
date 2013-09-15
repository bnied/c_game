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
	
	player->name = name;
	player->health = 100;
	player->attack = 5;
	player->defense = 2;
	player->dead = 0;
	
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
	int new_damage;
	
	switch(die_roll){
		default:
			new_damage = damage - player->defense;
			printf("The enemy attacks for %d damage!\n", new_damage);
			break;

		case 1:
			new_damage = (damage - player->defense)*2;
			printf("Critical hit!\nYour enemy attacks for %d damage!\n", new_damage);
			break;
 
		case 2:
			new_damage = (int)(damage / 2) - player->defense;
			printf("Your enemy's blow glances off you and only causes %d damage!\n", new_damage);
			break;
	}
	if(new_damage > 0){
		player->health -= new_damage;
	}
}

// Check if our player is dead
int check_player_death(struct Player *player){
	if(player->health <= 0) {
		player->dead = 1;
		return 1;
	} else {
		return 0;
	}
}