struct Player {
  char *name;
  int health;
  int attack;
  int defense;
  int dead;
};

struct Player *Player_create(char *name);
void Player_destroy(struct Player *player);
void Player_print(struct Player *player);
void player_damage(struct Player *player, int damage, int die_roll);
int check_player_death(struct Player *player);