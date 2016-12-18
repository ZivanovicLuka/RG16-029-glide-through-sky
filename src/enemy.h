#ifndef ENEMY_H
  #define ENEMY_H

  #define BULLETS_COUNT 4
  #define DYING 2

  void draw_enemy(int index);//float y, float x, float colorR, float colorG, float colorB, int alive, float angle);
  void summon_enemy();
  void aim();
  void fire(int index);
  int in_range(int index);

  typedef struct {
    float curr_x;
    float curr_y;
    int alive;
    int pass;
    float angle;
    float dying_time;
    float x_aim;
    float y_aim;
    int rotation;
    int bullet_index;
  } Enemy;

  int enemy_summon_index;

#endif
