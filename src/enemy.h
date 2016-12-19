#ifndef ENEMY_H
  #define ENEMY_H

  #define BULLETS_COUNT 4
  #define DYING 2

  void draw_enemy(int index);
  void draw_enemies();
  void summon_enemy();
  void enemies_move(float ms, float speed_correction);
  void enemies_init();
  void enemies_aim();
  void enemy_fire(int index);
  void enemies_fire();
  int in_range(int index);

  typedef struct {
    float curr_x;
    float curr_y;
    int alive;
    float angle;
    float dying_time;
    int rotation;
    int bullet_index;
  } Enemy;

  int enemy_summon_index;

#endif
