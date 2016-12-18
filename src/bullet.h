#ifndef BULLET_H
  #define BULLET_H

  void bullets_init();
  void draw_bullet();

  #define BULLET_COUNT 4

  typedef struct {
    float curr_x;
    float curr_y;
    float v_x;
    float v_y;
    float angle;
    int alive;
    float colorR;
    float colorG;
    float colorB;
  } Bullet;

#endif
