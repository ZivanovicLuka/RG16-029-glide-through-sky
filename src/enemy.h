#ifndef ENEMY_H
  #define ENEMY_H

  void draw_enemy(int index);//float y, float x, float colorR, float colorG, float colorB, int alive, float angle);
  void summon_enemy();

  typedef struct {
    float x_curr;
    float y_curr;
    float body_dmg;
    // float mana_drop; izracunaj ovo preko body dmg
    // float jump; izracunaj opet u zavisnosti od body_dmg
    int alive;
    int pass;
    // pravi crveni zuti i zeleni u zavisnosti od body_dmga i mana dropa
    // float colorR;
    // float colorG;
    // float colorB;
    float x_aim;
    float y_aim;

    float angle;
  } Enemy;

  float bullet_dmg;
  int enemy_summon_index;

#endif
