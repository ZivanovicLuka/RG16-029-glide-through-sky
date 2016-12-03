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
float speed_correction = 1;

World world = {
  .0008, // world.gravity
  .027,  // world.jump
  0,     // world.score
  0,     // world.distance
  0      // world.animation_ongoing
};

float distance = 0;

GUI gui = {
  -1,    // z, Global z-index of gui
  0,     // plate_width 
  0,     // plate_height
  0,     // hp_x
  0,     // hp_y
  0,     // score_x
  0,     // score_y
  0,     // score_text[15]
};

void score_init(){
  strcpy(gui.score_text,"Score: 0");
  return;
}


void draw_world(){
  GLfloat diffuse_coeffs[] = { 0.2, 0.2, 0.2, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  glPushMatrix();
      glTranslatef(0,3.5,0);
      glScalef(1, 1, 0.3);
      glutSolidCube(5);
  glPopMatrix();

  glPushMatrix();
      glTranslatef(0,-3.5,0);
      glScalef(1, 1, 0.3);
      glutSolidCube(5);
  glPopMatrix();

  glutPostRedisplay();
}

void check_score(int index){
  if(!walls[index].pass){
    if(walls[index].x_curr + wall_width/2 < player.x_curr - player.size/2){
      walls[index].pass = 1;
      world.score++;
      sprintf(gui.score_text, "Score: %d", world.score); // FIXME SPORO!!!
    }
  }
}