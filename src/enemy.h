#ifndef ENEMY_H
  #define ENEMY_H

  #define BULLETS_COUNT

  void draw_enemy(int index);//float y, float x, float colorR, float colorG, float colorB, int alive, float angle);
  void summon_enemy();

  typedef struct {
    float x_curr;
    float y_curr;
    int alive;
    int pass;
    float angle;
    float x_aim;
    float y_aim;
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
