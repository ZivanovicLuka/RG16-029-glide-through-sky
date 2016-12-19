#ifndef BULLET_H
  #define BULLET_H

  void bullets_init();
  void draw_bullet();
  void bullets_init();
  void bullets_move(float ms);
  void bullets_player_collision();
  void bullets_walls_turrets_collision();
  void bullets_world_collision();

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
