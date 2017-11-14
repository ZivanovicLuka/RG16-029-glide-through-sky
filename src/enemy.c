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

/* Enemy is always on the wall, so WALL_COUNT is the same number for enemies as well */
Enemy enemies[WALL_COUNT];

float range = 2;
int enemy_summon_index = 0; /* enemies is static array, so this index tracks wich one should summon */

void enemies_init()
{
  int i;
  for(i=0; i<WALL_COUNT; i++){
      enemies[i].alive = 0;
      enemies[i].rotation = 0;
      enemies[i].bullet_index = 0;
  }
}

void summon_enemy()
{
  enemy_summon_index = (++enemy_summon_index == WALL_COUNT) ? 0 : enemy_summon_index; /* Increases index, or returns it to 0 if max*/
  int index = enemy_summon_index;

  if(rand()/(float)RAND_MAX < .3) /* 30% chance for turret to be upside down */
    enemies[index].rotation = -1;
  else
    enemies[index].rotation = 1;

  enemies[index].curr_x = walls[index].curr_x;
  if(enemies[index].rotation == 1)
    enemies[index].curr_y = walls[index].y_top + .05; /* 0.05 is half of turrets base */
  else
    enemies[index].curr_y = walls[index].y_bot - .05; /* 0.05 is half of turrets base */
  enemies[index].angle = 0;
  enemies[index].alive = 1;
  enemies[index].dying_time = 0; /* dying animation time */
}

int in_range(int index)
{
  float x1 = enemies[index].curr_x;
  float y1 = enemies[index].curr_y;
  float x2 = player.curr_x;
  float y2 = player.curr_y;
  if(sqrt(pow(x2-x1,2)+pow(y2-y1,2))<range) /* Distance between player and enemy */
    return 1;
  return 0;
}

void enemy_fire(int index)
{
  /* If player is in range and enemy alive */
  if(in_range(index) && enemies[index].alive){
    int bullet_index = enemies[index].bullet_index;

    float vx = cos(enemies[index].angle/180.0*M_PI);
    float vy = sin(enemies[index].angle/180.0*M_PI);
    float vector_intensity = sqrt(pow(vx ,2) + pow(vy,2));

    bullets[index][bullet_index].v_x = vx/vector_intensity; /* vector normalize */
    bullets[index][bullet_index].v_y = vy/vector_intensity; /* vector normalize */
    bullets[index][bullet_index].angle = enemies[index].angle;

    bullets[index][bullet_index].curr_x = enemies[index].curr_x +
                                          bullets[index][bullet_index].v_x*.15; /* Moving bullet to tip of gun */
    bullets[index][bullet_index].curr_y = enemies[index].curr_y +
                                          bullets[index][bullet_index].v_y*.15 + /* Moving bullet to tip of gun */
                                          enemies[index].rotation*(.04 + .03);   /* base/2 + top/2 */

    bullets[index][bullet_index].alive = 1;
    /* Increases index, or returns it to 0 if max*/
    enemies[index].bullet_index = (++bullet_index == BULLET_COUNT) ?
                                  0 : bullet_index;
  }
}

void enemies_fire()
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
    enemy_fire(i);
  }
}

void enemies_move(float ms, float speed_correction)
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
    enemies[i].curr_x -= ms;
    if(enemies[i].alive == DYING)
      /* If enemy is dying, increase dying parameter */
      /* Speed corection fixes differences between different performances */
      enemies[i].dying_time += .0075*speed_correction;
  }
}


void draw_enemy(int index)
{
  if(!enemies[index].alive)
    return;

  float body_height=.08;
  float body_width=.16;

  float top_height=.06;
  float top_width=.1;

  float gun_width=.025;
  float gun_length=.12;

  float top_part_width = (top_width - gun_width) / 2;

  float x = enemies[index].curr_x;
  float y = enemies[index].curr_y;
  float colorR = walls[index].colorR * .9;
  float colorG = walls[index].colorG * .9;
  float colorB = walls[index].colorB * .9;
  float angle = enemies[index].angle;
  float y_dying_speed = 0;

  if(enemies[index].alive == DYING){
    /* If enemy is dying, parts are spinning freely */
    angle = enemies[index].dying_time*700;
  }

  glPushMatrix();
      GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      /* Turret body */
      glTranslatef(x,y,0);
      if(enemies[index].rotation == -1)
        glRotatef(180,1,0,0);
      glScalef(1,body_height/body_width,1);
      if(enemies[index].alive!= DYING)
        glutSolidCube(body_width);
      glScalef(1,body_width/body_height,1);
      /***********************************/

      /* Darker color for upper part of turret */
      diffuse_coeffs[0] = .4 * colorR;
      diffuse_coeffs[1] = .4 * colorG;
      diffuse_coeffs[2] = .4 * colorB;
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      /* Upper body 1 */
      glTranslatef(enemies[index].dying_time*.7,
                  (body_height+top_height)/2 + enemies[index].dying_time,
                   top_part_width/2 + gun_width/2 + enemies[index].dying_time);
      glScalef(top_width,top_height,top_part_width);
      glRotatef(enemies[index].alive == DYING ? angle : 0,1,0,0); /* If dying rotate */
      glutSolidCube(1);
      glRotatef(enemies[index].alive == DYING ? -angle : 0,1,0,0);
      glScalef(1/top_width,1/top_height,1/top_part_width);
      /**************************************************/

      /* Upper body 2 */
      glTranslatef(0,
                   0,
                   -top_part_width - gun_width - 2*enemies[index].dying_time);
      glScalef(top_width,top_height,top_part_width);
      glRotatef(enemies[index].alive == DYING ? angle : 0,1,0,0); /* If dying rotate */
      glutSolidCube(1);
      glRotatef(enemies[index].alive == DYING ? -angle : 0,1,0,0);
      glScalef(1/top_width,1/top_height,1/top_part_width);
      glTranslatef(0,0,top_part_width/2 + gun_width/2 + enemies[index].dying_time);
      /**************************************************/

      /* Even darker color for upper part of turret */
       diffuse_coeffs[0] = .3 * colorR;
       diffuse_coeffs[1] = .3 * colorR;
       diffuse_coeffs[2] = .3 * colorR;
       glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      /* Gun */
      if(enemies[index].rotation == -1)
        angle = -angle;
      glRotatef(angle,0,0,1);
      glScalef(gun_length,gun_width,gun_width);
      if(enemies[index].alive != DYING)
        glTranslatef(.5,0,0);
      glutSolidCube(1);
      /**************/

      diffuse_coeffs[0] = colorR;
      diffuse_coeffs[1] = colorG;
      diffuse_coeffs[2] = colorB;
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

      /* Gun tip */
      glTranslatef(.5+gun_width/gun_length/2,0,0);
      glScalef(1/gun_length,1/gun_width,1/gun_width);
      glutSolidCube(gun_width);
      /***********************/


  glPopMatrix();
  glutPostRedisplay();
}

void draw_enemies()
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
    draw_enemy(i);
  }
}

void enemies_aim()
{
  float dx,dy;

  int i;
  for(i=0;i<WALL_COUNT;i++){
    if(!enemies[i].alive)
      continue;

    dx = player.curr_x + .3 - enemies[i].curr_x; /* Aiming a bit in front of palyer */
    dy = player.curr_y - (enemies[i].curr_y + .04); /* body height / 2 */

    float angle_goal = atan2(dy,dx)*180/M_PI; /* Angel that gun should have */
    float angle = enemies[i].angle;

    /* Gun tracks goal angle */
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
      }
    }
    enemies[i].angle = angle;
  }
}
