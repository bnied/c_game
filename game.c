/* © 2013 Benjamin Nied
 * This is a same program designed to simulate a battle between two entities
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NAME_LENGTH 40

struct Player {
	char *name;
	int health;
	int attack;
	int defense;
    int dead;
};

struct Enemy {
	char *name;
	int health;
	int attack;
	int defense;
    int dead;
};

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
    
    free(player->name);
    free(player);
}

struct Enemy *Enemy_create() {
    struct Enemy *enemy = malloc(sizeof(struct Enemy));
    assert(enemy != NULL);
    
    enemy->name = "Bad Guy";
    enemy->health = 50;
    enemy->attack = 7;
    enemy->defense = 4;
    enemy->dead = 0;
    
    return enemy;
}

void Enemy_destroy(struct Enemy *enemy){
    assert(enemy != NULL);
    
    free(enemy->name);
    free(enemy);
}

// Take the damage dealt and subtract it from the Player's DEF
void player_damage(struct Player *player, int damage, int critical_hit, int glancing_blow) {
    int new_damage = 0;
    
    if(glancing_blow) {
        new_damage = (int)(damage / 2) - player->defense;
    } else {
        new_damage = damage - player->defense;
    }

    if (critical_hit) {
        new_damage = (damage - player->defense)*2;
    } else {
        new_damage = damage - player->defense;
    }
    
    player->health -= new_damage;
}

// Take the damage dealt and subtract it from the Enemy's DEF
void enemy_damage(struct Enemy *enemy, int damage, int critical_hit, int glancing_blow) {
    int new_damage = 0;

    if(glancing_blow) {
        new_damage = (int)(damage / 2) - enemy->defense;
    } else {
        new_damage = damage - enemy->defense;
    }
    
    if (critical_hit) {
        new_damage = (damage - enemy->defense)*2;
    } else {
        new_damage = damage - enemy->defense;
    }
    
    enemy->health -= new_damage;
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

int main(int argv, char *argc[]) {
    // Setup; get the player's name and generate our characters
    char input[NAME_LENGTH];
    printf("Hello and welcome! What is your name?\n");
    char *name = fgets(input, NAME_LENGTH, stdin);
    struct Player *player = Player_create(name);
    struct Enemy *enemy = Enemy_create();
    
    // Start the game. The introduction...
    printf("Oh no, an enemy!\n");
    printf("Your current health is %d.\nYour enemy's health is %d.\n\n", player->health, enemy->health);
    
    // Endless loop; keep going until one of the characters is dead.
    while (1 == 1) {
        // Player's turn
        int player_critical_hit = rand() % 2;
        int player_glancing_blow = rand() % 2;

        char player_message[80];
        strcpy(player_message, "You attack!\n");
        
        if(player_critical_hit) {
            strcat(player_message, "Critical hit!\n");
        }

        if(player_glancing_blow) {
            strcat(player_message, "You only land a glancing blow!!\n");   
        }

        printf("%s", player_message);
        
        enemy_damage(enemy, player->attack, player_critical_hit, player_glancing_blow);
        
        // Check if we killed it
        int dead_enemy = check_enemy_death(enemy);
        if (dead_enemy) {
            printf("You killed him! You win, %s!", player->name);
            break;
        }
        printf("Your enemy's health is %d.\n\n", enemy->health);
        
        // Enemy's turn.
        int enemy_critical_hit = rand() % 2;
        int enemy_glancing_blow = rand() % 2;

        char enemy_message[80];

        strcpy(enemy_message, "The enemy attacks!\n");
        
         if(enemy_critical_hit) {
            strcat(enemy_message, "Critical hit!\n");
        }

        if(enemy_glancing_blow) {
            strcat(enemy_message, "The enemy only lands a glancing blow!!\n");   
        }

        printf("%s", enemy_message);
        
        player_damage(player, enemy->attack, enemy_critical_hit, enemy_glancing_blow);
        
        // Check if the enemy killed the player
        int dead_player = check_player_death(player);
        if (dead_player) {
            printf("The enemy has killed you! You lose, %s!", player->name);
            break;
        }
        printf("Your current health is %d.\n\n", player->health);
    }
    
    return 0;
}