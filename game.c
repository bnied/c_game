/* © 2013 Benjamin Nied
 * This is a same program designed to simulate a battle between two entities
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NAME_LENGTH 40

// Declarations
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

struct Enemy *Enemy_create() {
    struct Enemy *enemy = malloc(sizeof(struct Enemy));
    assert(enemy != NULL);
    
    enemy->name = "Bad Guy";
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

/* The die roll determines what kind of damage we take:
 * - Normal (0): Health - (ATK - DEF)
 * - Critical (1): (Health - (ATK - DEF)) * 2
 * - Glancing (2): (Health - (ATK - DEF)) / 2
 */
void player_damage(struct Player *player, int damage, int die_roll) {
    int new_damage = 0;
    
   switch(die_roll){
        default:
            new_damage = damage - player->defense;
            break;

        case 1:
            new_damage = (damage - player->defense)*2;
            break;
 
        case 2:
            new_damage = (int)(damage / 2) - player->defense;
            break;
    }
    
    player->health -= new_damage;
}

/* The die roll determines what kind of damage we do:
 * - Normal (0): Health - (ATK - DEF)
 * - Critical (1): (Health - (ATK - DEF)) * 2
 * - Glancing (2): (Health - (ATK - DEF)) / 2
 */
void enemy_damage(struct Enemy *enemy, int damage, int die_roll) {
    int new_damage = 0;

    switch(die_roll){
        default:
            new_damage = damage - enemy->defense;
            break;

        case 1:
            new_damage = (damage - enemy->defense)*2;
            break;
 
        case 2:
            new_damage = (int)(damage / 2) - enemy->defense;
            break;
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

int main(int argc, char *argv[]) {
    // Setup; get the player's name and generate our characters
    char input[NAME_LENGTH];
    printf("Hello and welcome! What is your name?\n");
    char *name = fgets(input, NAME_LENGTH, stdin);
    struct Player *player = Player_create(name);
    struct Enemy *enemy = Enemy_create();
    
    // DEBUG
    printf("Player's Memory Location: %p.\n", player);
    printf("Enemy's Memory Location: %p.\n", enemy);

    // Seed our RNG
    srand(time(NULL));
    
    // Start the game. The introduction...
    printf("An enemy approaches...\n");
    printf("You and your enemy both start with 100 health.\n\n");
    
    // Endless loop; keep going until one of the characters is dead.
    while (1 == 1) {
        // Player's turn
        int player_die_roll = rand() % 3;

        switch(player_die_roll){
            default:
                printf("You attack!\n");
                break;

            case 1:
                printf("You attack!\nCritical hit!\n");
                break;

            case 2:
                printf("You attack!\nYou only land a glancing blow!!\n");
                break;
        }

        enemy_damage(enemy, player->attack, player_die_roll);
        
        // Check if we killed it
        int dead_enemy = check_enemy_death(enemy);
        if (dead_enemy) {
            printf("You killed the enemy! You win, %s.", player->name);
            break;
        }
        printf("The enemy's health is now %d.\n\n", enemy->health);
        
        // Give the player a chance to read up
        sleep(1);
        
        // Enemy's turn.
        int enemy_die_roll = rand() % 3;

        switch(enemy_die_roll){
            default:
                printf("The enemy attacks!\n");
                break;

            case 1:
                printf("The enemy attacks!\nCritical hit!\n");
                break;

            case 2:
                printf("The enemy attacks!\nThey only land a glancing blow!!\n");
                break;
        }
        
        player_damage(player, enemy->attack, enemy_die_roll);
        
        // Check if the enemy killed the player
        int dead_player = check_player_death(player);
        if (dead_player) {
            printf("The enemy has killed you! You lose, %s.", player->name);
            sleep(2);
            break;
        }
        printf("Your health is now %d.\n\n", player->health);
        
        // Give the player a chance to read up.
        sleep(1);
        
        // Clear the screen
        int height = 80;
        int i;
        
        for (i = 0; i < height; i++) {
            printf("\n");
        }
    }

    Player_destroy(player);
    Enemy_destroy(enemy);
    return 0;
}
