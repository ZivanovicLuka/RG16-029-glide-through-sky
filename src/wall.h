#ifndef WALL_H
  #define WALL_H

  void draw_wall(float x, int index);
  void summon_wall();
  void wall_init();
  void walls_move(float ms);

  #define WALL_COUNT 4

  typedef struct {
    float curr_x;
    float y_bot;
    float y_top;
    int alive;
    int pass;
    float colorR;
    float colorG;
    float colorB;
    int hollow;
    float hole_y;
  } Wall;

#endif
