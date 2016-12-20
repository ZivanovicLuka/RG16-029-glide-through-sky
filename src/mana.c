#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "wall.h"
#include "mana.h"

#include "player.h"
#include "player_externs.h"

#include "world.h"
#include "world_externs.h"

Crystal crystal;
float mana_crystal_rotation;
float mana_init_time;
float mana_init;
int walls_passed_counter;

void mana_crystal_init()
{
  mana_crystal_rotation = 30;
  mana_init_time = 0;
  mana_init = 0;
  walls_passed_counter = 0;
  crystal.alive = 0;
  circle_rotation_time = 0;
}

void summon_mana(int index)
{
  if((walls_passed_counter = (walls_passed_counter+1)%4) == 0){
    crystal.alive = 1;
    crystal.curr_x = 1 + .55; /* Near second wall, for dash + pass through wall */
  }
}

void draw_mana_crystal()
{
  if(crystal.alive){
    /* Emission, because of low visibility with normal light */
    GLfloat diffuse_coeffs[] = { .0, .2, 1, .1 };
    GLfloat emission_coeffs[] = { .03, .03, .43, 1 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);


    glPushMatrix();
        glTranslatef(crystal.curr_x, crystal.curr_y , 0);

        glRotatef(mana_crystal_rotation, 0, 1, 0);
        glScalef(1, 1.2, 1);
        glRotatef(45,1,0,1);

        glutSolidCube(.06);
    glPopMatrix();

    /* Reseting emission coeffs */
    emission_coeffs[0] = 0;
    emission_coeffs[1] = 0;
    emission_coeffs[2] = 0;
    emission_coeffs[3] = 1;
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
  }
}

void mana_crystal_move(float ms)
{
  /* Circle rotation speed is faster when dashing, so binding to ms is fine */
  circle_rotation_time += ms;
  mana_crystal_rotation += speed_correction * 1.8;
  /* Moving with the rest of world */
  if (world.animation_ongoing) {
    crystal.curr_x -= ms;
  }
  /* Crystal is jumping up and down a bit */
  crystal.curr_y = sin(mana_crystal_rotation*.02)*.08;
}

void draw_mana_circle(int mana)
{
  if(mana == 0)
    return;

  float height = .02;
  float small_width = .1;
  if(mana>5)
    small_width = .05;

  float x_center = player.curr_x;
  float y_center = player.curr_y;
  float z = 0;

  GLfloat diffuse_coeffs[4];

  /* Same distance between mana bars around circle */
  float rotate_angle = 360 / mana;
  /* Parameter for rotation */
  float t = -circle_rotation_time*350;
  /* radius */
  float r = .13; 

  float x,y;
  int i;
  for(i=0;i<mana;i++){

    /* First one is the darkest, the last one is players color */
    diffuse_coeffs[0] = player.colors.R*(i+1)/mana;
    diffuse_coeffs[1] = player.colors.G*(i+1)/mana;
    diffuse_coeffs[2] = player.colors.B*(i+1)/mana;
    diffuse_coeffs[3] = 1;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    /* Drawing circle with parameter t and radius r */
    x = r * cos(t/180*M_PI);
    y = r * sin(t/180*M_PI);

    /* Drawing each mana bar */
    glPushMatrix();        
        glTranslatef(x_center + x, y_center + y, z);
        glRotatef(t + 90,0,0,1);
        glScalef(small_width*.8 ,height,.01);
        glutSolidCube(1);
    glPopMatrix();
    
    /* Updating parameter */
    t += rotate_angle;
  }
}

/* Player starts with 3 mana, and this is animation for slow spawning it */
void mana_circle_init_animation(float ms)
{
  if(mana_init<3){
    mana_init_time+=ms;
    if(mana_init_time > 30*ms){
      mana_init++;
      player.mana++;
      mana_init_time-= 30*ms;
    }
  }
}
