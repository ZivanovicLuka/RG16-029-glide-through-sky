#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "player.h"
#include "player_externs.h"
#include "wall.h"
#include "wall_externs.h"
#include "enemy.h"
#include "bullet.h"
#include "bullet_externs.h"

Enemy enemies[WALL_COUNT]; // moze biti na zidu samo

float range = 2;
int enemy_summon_index = 0;

void enemies_init(){
  int i;
  for(i=0; i<WALL_COUNT; i++){
      enemies[i].pass = 1;
      enemies[i].alive = 0;
      enemies[i].rotation = 0;
      enemies[i].bullet_index = 0;
  }
}

void summon_enemy(){
  enemy_summon_index = (++enemy_summon_index == WALL_COUNT) ? 0 : enemy_summon_index;
  int index = enemy_summon_index;

  if(rand()/(float)RAND_MAX < .4)
    enemies[index].rotation = -1;
  else
    enemies[index].rotation = 1;

  enemies[index].x_curr = walls[index].x_curr;
  if(enemies[index].rotation == 1)
    enemies[index].y_curr = walls[index].y_top + .05; // fix
  else
    enemies[index].y_curr = walls[index].y_bot - .05; // fix
  enemies[index].alive = 1;
  enemies[index].dying_time = 0;
  enemies[index].pass = 0;
}

int in_range(int index){
  float x1 = enemies[index].x_curr;
  float y1 = enemies[index].y_curr;
  float x2 = player.x_curr;
  float y2 = player.y_curr;
  if(sqrt(pow(x2-x1,2)+pow(y2-y1,2))<range)
    return 1;
  return 0;
}

void fire(int index){
  // printf("%i\n",!in_range(index));
  if(in_range(index) && enemies[index].alive){
    // enemies[index].alive = 2;
    int bullet_index = enemies[index].bullet_index;
    bullets[index][bullet_index].angle = enemies[index].angle;
    float vx = cos(enemies[index].angle/180.0*M_PI);
    float vy = sin(enemies[index].angle/180.0*M_PI);
    float vector = sqrt(pow(vx ,2) + pow(vy,2));
    bullets[index][bullet_index].v_x = vx/vector;
    bullets[index][bullet_index].v_y = vy/vector;
    bullets[index][bullet_index].x_curr = enemies[index].x_curr + 
                                          bullets[index][bullet_index].v_x*0.15;
    bullets[index][bullet_index].y_curr = enemies[index].y_curr + 
                                          bullets[index][bullet_index].v_y*0.15 +
                                          enemies[index].rotation*(0.04 + 0.03);
    bullets[index][bullet_index].alive = 1;

    enemies[index].bullet_index = (++bullet_index == BULLET_COUNT) ?
                                  0 : bullet_index;
  }
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
  float angle = enemies[index].angle;
  float y_dying_speed = 0;

  if(enemies[index].alive == DYING){
    angle = enemies[index].dying_time*700;
  }

  glPushMatrix();

      // glRotatef(90,1,0,0);

      GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(x,y,0);
      if(enemies[index].rotation == -1)
        glRotatef(180,1,0,0);
      glScalef(1,body_height/body_width,1);
      if(enemies[index].alive!= DYING)
        glutSolidCube(body_width); // TELO TENKA
      glScalef(1,body_width/body_height,1);


       diffuse_coeffs[0] = .4 * colorR;
       diffuse_coeffs[1] = .4 * colorG;
       diffuse_coeffs[2] = .4 * colorB;
       glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(enemies[index].dying_time*.7,
                  (body_height+top_height)/2 + enemies[index].dying_time,
                   top_part_width/2 + gun_width/2 + enemies[index].dying_time);
      glScalef(top_width,top_height,top_part_width);
      glRotatef(enemies[index].alive == DYING ? angle : 0,1,0,0);
      glutSolidCube(1); // DEO OKO PUSKE
      glRotatef(enemies[index].alive == DYING ? -angle : 0,1,0,0);
      glScalef(1/top_width,1/top_height,1/top_part_width);


      glTranslatef(0,
                   0,
                   -top_part_width - gun_width - 2*enemies[index].dying_time);
      glScalef(top_width,top_height,top_part_width);
      glRotatef(enemies[index].alive == DYING ? angle : 0,1,0,0);
      glutSolidCube(1); // DEO OKO PUSKE
      glRotatef(enemies[index].alive == DYING ? -angle : 0,1,0,0);
      glScalef(1/top_width,1/top_height,1/top_part_width);

      // float global[] = {x,y,0,1};
      // glMultMatrixf(global);

      // dx = xf - global[0];
      // dy = yf - global[1];

      glTranslatef(0,0,top_part_width/2 + gun_width/2 + enemies[index].dying_time);

      enemies[index].x_aim = enemies[index].x_curr;
      enemies[index].y_aim = enemies[index].y_curr;

//////

       diffuse_coeffs[0] = .3;
       diffuse_coeffs[1] = .3;
       diffuse_coeffs[2] = .3;
       glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      // if(enemies[index].alive == DYING)
      //     glTranslatef(enemies[index].dying_time,y_dying_speed,0);
      if(enemies[index].rotation == -1)
        angle = -angle;
      glRotatef(angle,0,0,1);
      glScalef(gun_length,gun_width,gun_width);
      if(enemies[index].alive != DYING)
        glTranslatef(.5,0,0);
      glutSolidCube(1); // PUSKA


      diffuse_coeffs[0] = colorR;
      diffuse_coeffs[1] = colorG;
      diffuse_coeffs[2] = colorB;
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      glTranslatef(.5+gun_width/gun_length/2,0,0);
      glScalef(1/gun_length,1/gun_width,1/gun_width);
      glutSolidCube(gun_width); // VRH PUSKE


  glPopMatrix();
  glutPostRedisplay();
}

void aim(){
  float dx,dy;

  int i;
  for(i=0;i<WALL_COUNT;i++){
    if(!enemies[i].alive)
      continue;

    dx = player.x_curr + .3 - enemies[i].x_aim; // gadja ispred
    dy = player.y_curr - (enemies[i].y_aim + 0.04); // body height / 2

    float angle_goal = atan2(dy,dx)*180/M_PI;
    float angle = enemies[i].angle;

    if(enemies[i].rotation == 1){
      if(angle_goal>= 0){
        if(angle_goal > angle + 10)
          angle += 4;
        else if(angle_goal < angle - 10 )
          angle -= 4;
      } else if(angle>=4 && angle <= 176) {
        if(angle<90)
          angle -= 4;
        else
          angle += 4;
      }
    } else {
      if(angle_goal<= 0){
        if(angle_goal < angle - 10)
          angle -= 4;
        else if(angle_goal > angle + 10 )
          angle += 4;
      } else if(angle<=-4 && angle >= -176) {
        // if(angle<90)
        //   angle += 4;
        // else
        //   angle -= 4;
      }
    }
    enemies[i].angle = angle;
  }
}
