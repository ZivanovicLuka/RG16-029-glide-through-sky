#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"
#include "wall_externs.h"
#include "enemy.h"

Enemy enemies[WALL_COUNT]; // moze biti na zidu samo
// Bullet bullets[WALL_COUNT][BULLETS_COUNT];

float bullet_dmg = 10;
int enemy_summon_index = 0;

void enemies_init(){
  int i;
  for(i=0; i<WALL_COUNT; i++){
      enemies[i].pass = 1;
      enemies[i].alive = 0;
  }
}

void summon_enemy(int level){
  enemy_summon_index = (++enemy_summon_index == WALL_COUNT) ? 0 : enemy_summon_index;
  int index = enemy_summon_index;

  enemies[index].x_curr = walls[index].x_curr;
  enemies[index].y_curr = walls[index].y_top + .05; // fix
  enemies[index].alive = 1;
  enemies[index].pass = 0;
}


void draw_enemy(int index){//float x, float y, float colorR, float colorG, float colorB, int alive, float angle){
  if(!enemies[index].alive)
    return;

  float body_height=0.08;
  float body_width=0.16;

  float top_height=0.06;
  float top_width=0.1;

  float gun_width=0.025;
  float gun_length=0.12;

  float top_part_width = (top_width - gun_width) / 2;

  float x = enemies[index].x_curr;
  float y = enemies[index].y_curr;
  float colorR = walls[index].colorR * 0.9;
  float colorG = walls[index].colorG * 0.9;
  float colorB = walls[index].colorB * 0.9;
  // float topColorR = enemies[index].colorR;
  // float topColorG = enemies[index].colorG;
  // float topColorB = enemies[index].colorB;
  float angle = enemies[index].angle;

  glPushMatrix();

      // glRotatef(90,1,0,0);

      GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(x,y,0);
      glScalef(1,body_height/body_width,1);
      glutSolidCube(body_width);
      glScalef(1,body_width/body_height,1);
/////

       diffuse_coeffs[0] = .4 * colorR;
       diffuse_coeffs[1] = .4 * colorG;
       diffuse_coeffs[2] = .4 * colorB;
       glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(0,(body_height+top_height)/2, top_part_width/2 + gun_width/2);
      glScalef(top_width,top_height,top_part_width);
      glutSolidCube(1);
      glScalef(1/top_width,1/top_height,1/top_part_width);


      glTranslatef(0,0,-top_part_width - gun_width);
      glScalef(top_width,top_height,top_part_width);
      glutSolidCube(1);
      glScalef(1/top_width,1/top_height,1/top_part_width);

      // float global[] = {x,y,0,1};
      // glMultMatrixf(global);

      // dx = xf - global[0];
      // dy = yf - global[1];

      glTranslatef(0,0,top_part_width/2 + gun_width/2);

      // glPushMatrix();
      //   glLoadMatrixf(global_transform_matrix);
      //   float aim[] = {x,y,0,0};
      //   glMultMatrixf(aim);
      //   float aim_ret[16];
      //   glGetFloatv(GL_MODELVIEW_MATRIX, aim_ret);

      //   enemies[index].x_aim = aim_ret[0];
      //   enemies[index].y_aim = aim_ret[1];
      //   // printf("%f %f\n",aim[1],aim_ret[1]);
      // glPopMatrix();

        enemies[index].x_aim = enemies[index].x_curr;
        enemies[index].y_aim = enemies[index].y_curr;

//////

       diffuse_coeffs[0] = .3;
       diffuse_coeffs[1] = .3;
       diffuse_coeffs[2] = .3;
       glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glRotatef(angle,0,0,1);
      glScalef(gun_length,gun_width,gun_width);
      glTranslatef(.5,0,0);
      glutSolidCube(1);


      diffuse_coeffs[0] = colorR;
      diffuse_coeffs[1] = colorG;
      diffuse_coeffs[2] = colorB;
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(.5+gun_width/gun_length/2,0,0);
      glScalef(1/gun_length,1/gun_width,1/gun_width);
      glutSolidCube(gun_width);


  glPopMatrix();
  glutPostRedisplay();
}
