#ifndef PLAYER_H
  #define PLAYER_H

  void draw_player(float y, float x, float z, float R, float G, float B);
  void wall_collision(int index);
  void mana_collision(int index);
  void dash();
  void dash_start();
  void teleport();
  void summon_trail();

  #define PLAYER_REFRESH_TIMER_ID 0
  #define PLAYER_REFRESH_TIMER_INTERVAL 10

  #define DASH_TIMER_ID 3
  #define DASH_TIMER_INTERVAL 350

  #define TRAIL_TIMER_ID 2
  #define TRAIL_TIMER_INTERVAL 60
  #define TRAIL_MAX 5

  typedef struct {
    float R;
    float G;
    float B;
  } Color3f;

  int global_colors_number;

  typedef struct {
    float x_curr;
    float y_curr;
    float v_y; // velocity
    float size;
    Color3f colors;
    int mana;
    int invulnerable;
    int dashing;
  } Player;

  typedef struct {
    float pos_x;
    float pos_y;
    float pos_z;
    float colors;
  } Trail;

  float trail_x_move;
  float trail_color_alpha;
  int trail_count;

#endif
