#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"

#include "wall.h"
#include "wall_externs.h"

#include "world.h"


int window_width = 500;
int window_height = 500;

World world = {
  .0015, // world.gravity
  .035,  // world.jump
  0,     // world.score
  0      // world.animation_ongoing
};

GUI gui = {
  -1.,
  0,
  0,
  0,
  0,
  0,
  0,
};

void check_score(int index){
  if(!walls[index].pass){
    if(walls[index].x_curr + wall_width/2 < player.x_curr - player.size/2){
      walls[index].pass = 1;
      world.score++;
      sprintf(gui.score_text, "Score: %d", world.score); // FIXME SPORO!!!
    }
  }
}
