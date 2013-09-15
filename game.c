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
#include "game.h"

#define NAME_LENGTH 40
#define SCREEN_ROWS 100

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

struct Enemy *Enemy_create() {
	struct Enemy *enemy = malloc(sizeof(struct Enemy));
	assert(enemy != NULL);
	
	enemy->name = "The Bad Guy";
	enemy->health = 100;
	enemy->attack = 5;
	enemy->defense = 2;
	enemy->dead = 0;
	
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
			printf("Your blow glances off the enemy and only causes %d damage!\n", new_damage);
			break;
	}
	if(new_damage > 0) {
		enemy->health -= new_damage;
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

// Check if our enemy is dead
int check_enemy_death(struct Enemy *enemy){
	if(enemy->health <= 0) {
		enemy->dead = 1;
		return 1;
	} else {
		return 0;
	}
}

// Clear the screen
void clear_screen() {
	// Clear the screen
	for (int i = 0; i < SCREEN_ROWS; i++) {
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	// Setup; get the player's name and generate our characters
	char name_input[NAME_LENGTH];
	int attack_choice;
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

		printf("Which attack would you like to use?\n\n");
		printf("1 - Punch (3 DMG - DEF)\n");
		printf("2 - Kick (5 DMG - DEF)\n");
		printf("3 - Knife (10 DMG - DEF)\n\n");

		scanf("%d", &attack_choice);

		switch(attack_choice) {
			default:
				printf("Punch selected.\n");
				player->attack = 3;
				break;

			case 2:
				printf("Kick selected.\n");
				player->attack = 5;
				break;

			case 3:
				printf("Knife selected.\n");
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
