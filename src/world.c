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
  .0007, // world.gravity
  .027,  // world.jump
  0,     // world.score
  0,     // world.distance
  0,     // world.animation_ongoing
  0      // world.time
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


Star stars[100][10]; // x_number * y_number

void score_init(){
  strcpy(gui.score_text,"Score: 0");
  return;
}

void stars_init(){
  int i,j;
  for(i=0;i<STAR_X_NUMBER;i++){
    for(j=0;j<STAR_Y_NUMBER;j++){
      float dx = rand() / (float)RAND_MAX *1.6 - 0.8;
      float dy = rand() / (float)RAND_MAX *1.6 - 0.8;

      stars[i][j].curr_x = 4.0/STAR_X_NUMBER * (i + dx) - 2;
      stars[i][j].curr_y = 2.4/STAR_Y_NUMBER * (j + dy) - 1.2;
      stars[i][j].speed =  rand() / (float)RAND_MAX * .0007 + 0.0002;
    }
  }
}

void draw_world(){
  GLfloat diffuse_coeffs[] = { 0, 0, 0.1, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse_coeffs);

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

  int i,j;
  diffuse_coeffs[0] = 0.01;
  diffuse_coeffs[1] = 0.01;
  diffuse_coeffs[2] = 0.01;
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  GLfloat ambient_coeffs[] = { .8, .8, .8, 1 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);

  for(i=0;i<STAR_X_NUMBER;i++){
    for(j=0;j<STAR_Y_NUMBER;j++){
      glTranslatef(stars[i][j].curr_x,stars[i][j].curr_y,-.8);
      glutSolidCube(.01);
      glTranslatef(-stars[i][j].curr_x,-stars[i][j].curr_y,.8);
    }
  }

  ambient_coeffs[0] = 0.3;
  ambient_coeffs[1] = 0.3;
  ambient_coeffs[2] = 0.3;
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);

  glutPostRedisplay();
}

void check_score(int index){
  if(!walls[index].pass){
    if(walls[index].curr_x + wall_width/2 < player.curr_x - player.size/2){
      walls[index].pass = 1;
      world.score++;
      sprintf(gui.score_text, "Score: %d", world.score); // FIXME SPORO!!!
    }
  }
}