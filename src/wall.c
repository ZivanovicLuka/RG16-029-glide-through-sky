#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"
#include "wall.h"

Wall walls[WALL_COUNT];

const float wall_width = .3;
float wall_speed = .013;
float wall_gap = 1.;
float wall_gap_min = .7;
float wall_gap_dec = .03;
int wall_summon_index;

void wall_init()
{
  wall_gap = 1.;
  int i;
  for(i=0; i<WALL_COUNT; i++){
      /* Tracks if player passed wall, for score */
      walls[i].pass = 1;
      walls[i].alive = 0;
      /* If player dashes through wall */
      walls[i].hollow = 0;
  }
}

void summon_wall()
{
  wall_summon_index = (++wall_summon_index == WALL_COUNT) ? 0 : wall_summon_index;
  int index = wall_summon_index;

  /* Gap between walls decreases on each summon */
  if(wall_gap > wall_gap_min){
    wall_gap -= wall_gap_dec;
  }

  /* y_bot is bottom part of the upper wall  */
  /* it is generated to have minimum size for top and bottom wall, and to fit gap between */
  walls[index].y_bot = rand() / (float)RAND_MAX * (1.7 - wall_gap) - 1 + wall_gap + .15;
  walls[index].curr_x = 1 + wall_width/2 + 1; 
  walls[index].y_top =  walls[index].y_bot - wall_gap;
  /* Colors are generated to be bright, because background is dark, and because it looks good */
  walls[index].colorR = rand() / (float)RAND_MAX * .5 + .5;
  walls[index].colorG = rand() / (float)RAND_MAX * .5 + .5;
  walls[index].colorB = rand() / (float)RAND_MAX * .5 + .5;
  
  walls[index].hollow = 0;
  walls[index].alive = 1;
  walls[index].pass = 0;
}

void walls_move(float ms)
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
    walls[i].curr_x -= ms;
  }
}

void draw_wall(int index)
{
  if(walls[index].alive){

    float distance = 0;
    float x = walls[index].curr_x;

    /* Hole size is a bit bigger then player */
    float hole_size = player.size * 1.3;
    float hole_top = walls[index].hole_y + hole_size/2;
    float hole_bot = walls[index].hole_y - hole_size/2;
    float top_height, mid_depth, mid_height, bot_height;

    GLfloat diffuse_coeffs[] = { walls[index].colorR, walls[index].colorG, walls[index].colorB, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    /*
      If wall has hole it is made with 4 pieces:
        1 above
        2 on hole level
        1 below

      Hole can also be on tip of the wall (up and down),
      and it is fixed too
    */

    /* Top wall */
    float height = 1 - walls[index].y_bot;
    glPushMatrix();
    /* If wall has hole, and it is located on upper wall */
      if(walls[index].hollow && hole_top > walls[index].y_bot){
        /* Part above hole */
        top_height = 1 - hole_top;
        glTranslatef(x, 1 - top_height/2 , 0);
        glScalef(wall_width, top_height, wall_width);
        glutSolidCube(1);
        glScalef(1/wall_width, 1/top_height, 1/wall_width);

        /* 2 parts around hole */
        mid_depth = (wall_width - hole_size)/2;
        mid_height = hole_top - walls[index].y_bot < hole_size? hole_top - walls[index].y_bot : hole_size;
        glTranslatef(0,-top_height/2 - mid_height/2,hole_size/2 + mid_depth/2);
        glScalef(wall_width,mid_height,mid_depth);
        glutSolidCube(1);

        glTranslatef(0,0,-1 - hole_size/mid_depth);
        glutSolidCube(1);
        glScalef(1/wall_width,1/hole_size,1/mid_depth);
        glTranslatef(0,0,mid_depth/2 + hole_size/2);

        /* Bottom part */
        bot_height = height - top_height - hole_size;
        if(bot_height>0){
          glTranslatef(0,-bot_height/2 - hole_size/2, 0);
          glScalef(wall_width,bot_height,wall_width);
          glutSolidCube(1);
        }
      } else {
        /* Normal wall, without hole */
        glTranslatef(x, 1 - height/2 , 0);
        glScalef(wall_width, height, wall_width);
        glutSolidCube(1);
      }
    glPopMatrix();

    /* Bottom wall */
    height = 1 + walls[index].y_top;
    glPushMatrix();
      /* Part above hole */
      if(walls[index].hollow && hole_bot < walls[index].y_top){
        top_height = walls[index].y_top - hole_top;
        glTranslatef(x, walls[index].y_top - top_height/2 , 0);
        if(top_height>0){
          glScalef(wall_width, top_height, wall_width);
          glutSolidCube(1);
          glScalef(1/wall_width, 1/top_height, 1/wall_width);
        }

        /* 2 parts around hole */
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

        /* Bottom part */
        bot_height = height - top_height - mid_height;
        if(bot_height>0){
          glTranslatef(0, -bot_height/2 - mid_height/2, 0);
          glScalef(wall_width,bot_height,wall_width);
          glutSolidCube(1);
        }
      } else {
        /* Normal wall, without hole */
        glTranslatef(x, -1 + height/2 , 0);
        glScalef(wall_width, height, wall_width);
        glutSolidCube(1);
      }
    glPopMatrix();
    glutPostRedisplay();
  }
}

void draw_walls()
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
        draw_wall(i);
  }
}
