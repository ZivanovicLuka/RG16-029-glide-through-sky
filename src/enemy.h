#ifndef ENEMY_H
  #define ENEMY_H

  #define BULLETS_COUNT 4
  #define DYING 2

  void draw_enemy(int index);//float y, float x, float colorR, float colorG, float colorB, int alive, float angle);
  void summon_enemy();
  void aim();

  typedef struct {
    float x_curr;
    float y_curr;
    int alive;
    int pass;
    float angle;
    float dying_time;
    float x_aim;
    float y_aim;
    int rotation;
  } Enemy;

  typedef struct{
    float x_curr;
    float y_curr;
    float angle;
    int alive;
  } Bullet;

  float bullet_dmg;
  int enemy_summon_index;

#endif
