#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"

Wall walls[WALL_COUNT];

const float wall_width = .3;
float wall_speed = 0.02;
float wall_gap = 1.;
float wall_gap_min = .7;
int wall_summon_index = 0;

void summon_wall(int index){
  float r = rand() / (float)RAND_MAX * (2 - wall_gap) - 1 + wall_gap;
  walls[index].x_curr = 1 + wall_width/2;
  walls[index].y_bot = r;
  walls[index].y_top = r - wall_gap;
  walls[index].colorR = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorG = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorB = rand() / (float)RAND_MAX * 0.5 + 0.5;
  // walls[index].colorR = 1;
  // walls[index].colorG = 1;
  // walls[index].colorB = 1;

  walls[index].alive = 1;
  walls[index].pass = 0;
}

void draw_wall(float x, int index){
  if(walls[index].alive){

    GLfloat diffuse_coeffs[] = { walls[index].colorR, walls[index].colorG, walls[index].colorB, 1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    float distance = 0;

    glBegin(GL_POLYGON);
      glColor3f( walls[index].colorR, walls[index].colorG, walls[index].colorB );
      glVertex3f(  wall_width/2 + x, -1, distance );
      glVertex3f(  wall_width/2 + x,  walls[index].y_top, distance );
      glVertex3f( -wall_width/2 + x,  walls[index].y_top, distance );
      glVertex3f( -wall_width/2 + x, -1, distance );
    glEnd();

    glBegin(GL_POLYGON);
      glColor3f( walls[index].colorR, walls[index].colorG, walls[index].colorB );
      glVertex3f(  wall_width/2 + x, 1, distance );
      glVertex3f(  wall_width/2 + x,  walls[index].y_bot, distance );
      glVertex3f( -wall_width/2 + x,  walls[index].y_bot, distance );
      glVertex3f( -wall_width/2 + x, 1, distance );
    glEnd();

    glutPostRedisplay();
  }
}
