#ifndef PLAYER_H
  #define PLAYER_H

  void player_init();
  void draw_player(float x, float y, float R, float G, float B);
  void draw_trail(float x, float y, float R, float G, float B, float size);
  void wall_collision(int index);
  void mana_collision();
  void enemy_collision(int index);
  void mana_enemies_collision();
  void dash();
  void dash_start();
  void teleport();
  void heal();
  void player_move();
  void summon_trail();
  int alive();
  

  #define PLAYER_REFRESH_TIMER_ID 0
  #define PLAYER_REFRESH_TIMER_INTERVAL 10

  #define DASH_TIMER_ID 3
  #define DASH_TIMER_INTERVAL 350

  #define TRAIL_TIMER_ID 2
  #define TRAIL_TIMER_INTERVAL 40
  #define TRAIL_MAX 15

  typedef struct {
    float R;
    float G;
    float B;
  } Color3f;

  Color3f global_colors[8]; // TODO konstanta
  int global_colors_number;

  typedef struct {
    float curr_x;
    float curr_y;
    float v_y;
    float size;
    int hp;
    Color3f colors;
    int mana;
    int invulnerable;
    int dashing;
    float dash_distance;
  } Player;

  typedef struct {
    float pos_x;
    float pos_y;
    float pos_z;
    float colors;
    float size;
  } Trail;

  float trail_x_move;
  float trail_color_alpha;
  int trail_count;

#endif
