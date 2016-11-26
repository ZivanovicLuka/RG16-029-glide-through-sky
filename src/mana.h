#ifndef MANA_H
  #define MANA_H

  void draw_mana_crystal(int index);
  void summon_mana(int index);

  typedef struct {
    float curr_y;
    float curr_x;
    int alive;
    int pass; // ne znam sta je sad
  } Crystal;

  float rotation;
  int mana_summon_index;

#endif
