#ifndef PLAYER_H
  #define PLAYER_H

  void player_init();
  void draw_player();
  void draw_trail_particle(int index);
  void draw_trail();
  void wall_collision(int index);
  void mana_collision();
  void enemy_collision(int index);
  void mana_enemies_collision();
  void dash();
  void dash_start();
  void dashing(float ms);
  void teleport();
  void heal();
  void player_move();
  void summon_trail();
  int alive();
  

  #define PLAYER_REFRESH_TIMER_ID 0
  #define PLAYER_REFRESH_TIMER_INTERVAL 10

  #define TRAIL_MAX 15

  typedef struct {
    float R;
    float G;
    float B;
  } Color3f;

  Color3f global_colors[8]; 
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
    float curr_x;
    float curr_y;
    float pos_z;
    float colors;
    float size;
  } Trail;

  float trail_x_move;
  float trail_color_alpha;
  int trail_count;

#endif
