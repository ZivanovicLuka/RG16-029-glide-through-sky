#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"
#include "wall_externs.h"
#include "enemy.h"

Enemy enemies[WALL_COUNT]; // moze biti na zidu samo

float bullet_dmg = 10;
int enemy_summon_index = 0;

void enemies_init(){
  int i;
  for(i=0; i<WALL_COUNT; i++){
      enemies[i].pass = 1;
      enemies[i].alive = 0;
  }
}

void summon_enemy(){
  enemy_summon_index = (++enemy_summon_index == WALL_COUNT) ? 0 : enemy_summon_index;
  int index = enemy_summon_index;

  //jump = enemy_gap * nesto;

  enemies[index].x_curr = walls[index].x_curr;
  enemies[index].y_curr = walls[index].y_top + .05; // fix
  enemies[index].colorR = rand() / (float)RAND_MAX * 0.5 + 0.5;
  enemies[index].colorG = rand() / (float)RAND_MAX * 0.5 + 0.5;
  enemies[index].colorB = rand() / (float)RAND_MAX * 0.5 + 0.5;
  enemies[index].alive = 1;
  enemies[index].pass = 0;
}


void draw_enemy(float x, float y, float colorR, float colorG, float colorB, int alive){
  if(!alive)
    return;

  GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  float body_height=0.08;
  float body_width=0.16;

  float top_height=0.06;
  float top_width=0.1;

  float gun_width=0.025;
  float gun_length=0.13;

  float top_part_width = (top_width - gun_width) / 2;

  glPushMatrix();

      // glRotatef(90,1,0,0);

      glTranslatef(x,y,0);
      glScalef(1,body_height/body_width,1);
      glutSolidCube(body_width);
      glScalef(1,body_width/body_height,1);
/////

      // diffuse_coeffs[2] = 1;
      // glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(0,(body_height+top_height)/2, top_part_width/2 + gun_width/2);
      glScalef(top_width,top_height,top_part_width);
      glutSolidCube(1);
      glScalef(1/top_width,1/top_height,1/top_part_width);

      // diffuse_coeffs[0] = 0;
      // glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(0,0,-top_part_width - gun_width);
      glScalef(top_width,top_height,top_part_width);
      glutSolidCube(1);
      glScalef(1/top_width,1/top_height,1/top_part_width);

      glTranslatef(0,0,top_part_width/2 + gun_width/2);

//////

      // diffuse_coeffs[1] = 1;
      // glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glRotatef(45,0,0,1);
      glScalef(gun_length,gun_width,gun_width);
      glTranslatef(.5,0,0);
      glutSolidCube(1);
      //
      // glTranslatef(.5,0,0);
      // glScalef(1/gun_length,1/gun_width,1/gun_width);
      // glutSolidCube(gun_width*1.5);


  glPopMatrix();
  glutPostRedisplay();
}
