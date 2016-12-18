#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"
#include "wall.h"

Wall walls[WALL_COUNT];

const float wall_width = .3;
float wall_speed = 0.013;
float wall_gap = 1.;
float wall_gap_min = .7;
float wall_gap_dec = .03;
int wall_summon_index = 0;

void wall_init(){
  int i;
  for(i=0; i<WALL_COUNT; i++){
      walls[i].pass = 1;
      walls[i].alive = 0;
      walls[i].hollow = 0;
  }
}

void summon_wall(){
  wall_summon_index = (++wall_summon_index == WALL_COUNT) ? 0 : wall_summon_index;
  int index = wall_summon_index;

  walls[index].hollow = 0;

  if(wall_gap > wall_gap_min){
    wall_gap -= wall_gap_dec;
  }

  float r = rand() / (float)RAND_MAX * (1.7 - wall_gap) - 1 + wall_gap + .15; //1.6 i +.2 fiksuju da zid ne bude previse kratak (ni gornji ni donji)
  walls[index].curr_x = 1 + wall_width/2 + 1; // od kad je 3d ide i +1
  walls[index].y_bot = r;
  walls[index].y_top = r - wall_gap;
  walls[index].colorR = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorG = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorB = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].alive = 1;
  walls[index].pass = 0;
}

void draw_wall(float x, int index){
  if(walls[index].alive){

    float distance = 0;

    float hole_size = player.size * 1.3;
    float hole_top = walls[index].hole_y + hole_size/2;
    float hole_bot = walls[index].hole_y - hole_size/2;
    float top_height, mid_depth, mid_height, bot_height;

    GLfloat diffuse_coeffs[] = { walls[index].colorR, walls[index].colorG, walls[index].colorB, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    // TOP WALL
    float height = 1 - walls[index].y_bot;
    glPushMatrix();
      if(walls[index].hollow && hole_top > walls[index].y_bot){
        top_height = 1 - hole_top;
        glTranslatef(x, 1 - top_height/2 , 0);
        glScalef(wall_width, top_height, wall_width);
        glutSolidCube(1);
        glScalef(1/wall_width, 1/top_height, 1/wall_width);

        mid_depth = (wall_width - hole_size)/2;
        mid_height = hole_top - walls[index].y_bot < hole_size? hole_top - walls[index].y_bot : hole_size;
        glTranslatef(0,-top_height/2 - mid_height/2,hole_size/2 + mid_depth/2);
        glScalef(wall_width,mid_height,mid_depth);
        glutSolidCube(1);

        glTranslatef(0,0,-1 - hole_size/mid_depth);
        glutSolidCube(1);
        glScalef(1/wall_width,1/hole_size,1/mid_depth);
        glTranslatef(0,0,mid_depth/2 + hole_size/2);

        bot_height = height - top_height - hole_size;
        if(bot_height>0){
          glTranslatef(0,-bot_height/2 - hole_size/2, 0);
          glScalef(wall_width,bot_height,wall_width);
          glutSolidCube(1);
        }
      } else {
        glTranslatef(x, 1 - height/2 , 0);
        glScalef(wall_width, height, wall_width);
        glutSolidCube(1);
      }
    glPopMatrix();
    // glutPostRedisplay();

    height = 1 + walls[index].y_top;
    glPushMatrix();
      if(walls[index].hollow && hole_bot < walls[index].y_top){
        top_height = walls[index].y_top - hole_top;
        glTranslatef(x, walls[index].y_top - top_height/2 , 0);
        if(top_height>0){
          glScalef(wall_width, top_height, wall_width);
          glutSolidCube(1);
          glScalef(1/wall_width, 1/top_height, 1/wall_width);
        }

        mid_depth = (wall_width - hole_size)/2;
        mid_height = walls[index].y_top - hole_bot < hole_size? walls[index].y_top - hole_bot : hole_size;
        float mid_height_fix = top_height<0? top_height : 0;
        glTranslatef(0, -top_height/2 - mid_height/2 + mid_height_fix, hole_size/2 + mid_depth/2);
        glScalef(wall_width,mid_height,mid_depth);
        glutSolidCube(1);

        glTranslatef(0,0,-1 - hole_size/mid_depth);
        glutSolidCube(1);
        glScalef(1/wall_width,1/mid_height,1/mid_depth);
        glTranslatef(0,0,mid_depth/2 + hole_size/2);

        bot_height = height - top_height - mid_height;
        if(bot_height>0){
          glTranslatef(0, -bot_height/2 - mid_height/2, 0);
          glScalef(wall_width,bot_height,wall_width);
          glutSolidCube(1);
        }
      } else {
        glTranslatef(x, -1 + height/2 , 0);
        glScalef(wall_width, height, wall_width);
        glutSolidCube(1);
      }
    glPopMatrix();
    glutPostRedisplay();
  }
}
