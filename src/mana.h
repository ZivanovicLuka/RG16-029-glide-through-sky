#ifndef MANA_H
  #define MANA_H

  void draw_mana_crystal();
  void mana_crystal_init();
  void draw_mana_circle(int mana);
  void summon_mana();
  void mana_crystal_move(float ms);
  void mana_circle_init_animation(float ms);

  typedef struct {
    float curr_y;
    float curr_x;
    int alive;
    int pass; // ne znam sta je sad
  } Crystal;

  float mana_crystal_rotation;
  int walls_passed_counter;
  float mana_init_time;
  float mana_init;

#endif
