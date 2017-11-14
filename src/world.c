#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"

#include "wall.h"
#include "wall_externs.h"

#include "enemy.h"

#include "bullet.h"

#include "mana.h"

#include "world.h"

#include "misc_functions.h"


int window_width = 500;
int window_height = 500;
float speed_correction = 1;

World world = {
  .0007, /*world.gravity*/
  .027,  /*world.jump*/
  0,     /*world.score*/
  0,     /*world.wall_summon_distance*/
  0,     /*world.trail_summon_distance*/
  0,     /*world.animation_ongoing*/
  0      /*world.time*/
};

float distance = 0;

Star stars[STAR_X_NUMBER][STAR_Y_NUMBER];

void score_init()
{
  strcpy(score_text,"Score: 0");
  return;
}

void stars_init()
{
  int i,j;
  for(i=0;i<STAR_X_NUMBER;i++){
    for(j=0;j<STAR_Y_NUMBER;j++){
      /* Positioning stars nearly grid line, but randomly moved for dx and dy */
      float dx = rand() / (float)RAND_MAX *1.6 - .8;
      float dy = rand() / (float)RAND_MAX *1.6 - .8;

      stars[i][j].curr_x = 4.0/STAR_X_NUMBER * (i + dx) - 2;
      stars[i][j].curr_y = 2.4/STAR_Y_NUMBER * (j + dy) - 1.2;
      stars[i][j].speed =  rand() / (float)RAND_MAX * .0007 + .0002;
    }
  }
}

void stars_move()
{
  int i,j;
  for(i=0;i<STAR_X_NUMBER;i++){
    for(j=0;j<STAR_Y_NUMBER;j++){
      stars[i][j].curr_x -= stars[i][j].speed;
      if(stars[i][j].curr_x<-2)
        stars[i][j].curr_x=2;
    }
  }
}

/* Draws stars, floor and ceil  */
void draw_world()
{
  GLfloat diffuse_coeffs[4];
  if(alive()){
    diffuse_coeffs[0] = 0;
    diffuse_coeffs[1] = 0;
    diffuse_coeffs[2] = .1;
    diffuse_coeffs[3] = 1;
  } else {
    diffuse_coeffs[0] = .5;
    diffuse_coeffs[1] = 0;
    diffuse_coeffs[2] = .1;
    diffuse_coeffs[3] = 1;
  }
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse_coeffs);

  glPushMatrix();
      glTranslatef(0,3.5,0);
      glScalef(1, 1, .3);
      glutSolidCube(5);
  glPopMatrix();

  glPushMatrix();
      glTranslatef(0,-3.5,0);
      glScalef(1, 1, .3);
      glutSolidCube(5);
  glPopMatrix();

  int i,j;
  diffuse_coeffs[0] = .01;
  diffuse_coeffs[1] = .01;
  diffuse_coeffs[2] = .01;
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  /* Stars are only light by ambinet light */
  GLfloat ambient_coeffs[] = { .8, .8, .8, 1 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);

  for(i=0;i<STAR_X_NUMBER;i++){
    for(j=0;j<STAR_Y_NUMBER;j++){
      glTranslatef(stars[i][j].curr_x,stars[i][j].curr_y,-.8);
      glutSolidCube(.01);
      glTranslatef(-stars[i][j].curr_x,-stars[i][j].curr_y,.8);
    }
  }

  ambient_coeffs[0] = .3;
  ambient_coeffs[1] = .3;
  ambient_coeffs[2] = .3;
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);

  glutPostRedisplay();
}

void check_score()
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
    if(!walls[i].pass){
      if(walls[i].curr_x + wall_width/2 < player.curr_x - player.size/2){
        walls[i].pass = 1;
        world.score++;
        sprintf(score_text, "Score: %d", world.score);
      }
    }
  }
}

void draw_score()
{
  float text_x = -.93;
  float text_y = .89;
  float text_z = .3;
  RenderString(text_x, text_y, GLUT_BITMAP_HELVETICA_18, score_text, .6, .6, .6);
}

void restart()
{
  printf("Score: %d\n", world.score);
  player_init();
  bullets_init();
  stars_init();
  score_init();
  wall_init();
  enemies_init();
  mana_crystal_init();
  world.score = 0;
  wall_speed = .013;
}

void game_over()
{
    player.colors.R = .5;
    player.colors.G = 0;
    player.colors.B = 0;

    int i;
    for(i=0;i<WALL_COUNT;i++){
      walls[i].colorR = .5;
      walls[i].colorG = 0;
      walls[i].colorB = 0;
    }

    world.animation_ongoing = 0;
}
