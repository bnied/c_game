struct Enemy {
  char *name;
  int health;
  int attack;
  int defense;
  int dead;
};

struct Enemy *Enemy_create();
void Enemy_destroy(struct Enemy *enemy);
void Enemy_print(struct Enemy *enemy);
void enemy_damage(struct Enemy *enemy, int damage, int die_roll);
int check_enemy_death(struct Enemy *enemy);