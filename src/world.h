#ifndef WORLD_H
  #define WORLD_H

  #define STAR_X_NUMBER 7
  #define STAR_Y_NUMBER 8

  void check_score();
  void draw_world();
  void score_init();
  void stars_init();
  void stars_move();
  void draw_score();
  void restart();

  int window_width;
  int window_height;
  float speed_correction;


  char score_text[15];


  typedef struct {
    float gravity;
    float jump;
    int score;
    float wall_summon_distance;
    float trail_summon_distance;
    int animation_ongoing;
    float time;
  } World;

  typedef struct {
    float curr_x;
    float curr_y;
    float speed;
  } Star;

#endif
