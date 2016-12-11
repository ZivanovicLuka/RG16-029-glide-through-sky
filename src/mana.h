#ifndef MANA_H
  #define MANA_H

  void draw_mana_crystal();
  void summon_mana();

  typedef struct {
    float curr_y;
    float curr_x;
    int alive;
    int pass; // ne znam sta je sad
  } Crystal;

  float mana_crystal_rotation;
  int walls_passed_counter;

#endif
