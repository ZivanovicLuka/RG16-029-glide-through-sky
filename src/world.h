#ifndef WORLD_H
  #define WORLD_H

  void check_score(int index);
  void draw_world();

  int window_width;
  int window_height;

  typedef struct {
    // Global z-index of gui
    float z;
    // Plate
    float plate_width;
    float plate_height;
    // HP
    float hp_x;
    float hp_y;
    // Mana
    float mana_x;
    float mana_y;
    // Score
    float score_x;
    float score_y;
    char score_text[15];
  } GUI;

  typedef struct {
    float gravity;
    float jump;
    int score;
    int animation_ongoing;
  } World;

#endif
