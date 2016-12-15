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

Crystal crystal; //nece biti vise zivih kristala od zidova sigurno
float mana_crystal_rotation = 30;
float mana_init_time=0;
float mana_init=0;
int walls_passed_counter = 0;

void summon_mana(int index){
  crystal.alive = 1;
  crystal.curr_x = 1 + .55; // FIXME .55 treba da predstavlja polovinu razmaka izmedju 2 zida
}

void draw_mana_crystal(){
  if(crystal.alive){
    GLfloat diffuse_coeffs[] = { 0.0, .2, 1, .1 };
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

    emission_coeffs[0] = .3;
    emission_coeffs[1] = .3;
    emission_coeffs[2] = 1;
    emission_coeffs[3] = 1;
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);

    glPushMatrix();
        glTranslatef(crystal.curr_x, crystal.curr_y , 0);

        glRotatef(mana_crystal_rotation, 0, 1, 0);
        glScalef(1, 1.5, 1);
        glRotatef(45,1,0,1);

        // glutWireCube(.0601);
    glPopMatrix();

    emission_coeffs[0] = 0;
    emission_coeffs[1] = 0;
    emission_coeffs[2] = 0;
    emission_coeffs[3] = 1;

    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);


  }
}

void draw_mana_circle(int mana){
  if(mana == 0)
    return;

  float height = .02;

  float small_width = .1;
  if(mana>5)
    small_width = .05;

  float x_center = player.x_curr;
  float y_center = player.y_curr;
  float z = 0;//-player.size;

  
  GLfloat emission_coeffs[] = { .1, .1, .3, 1 };
  GLfloat diffuse_coeffs[4];// = { player.colors.R*.4, player.colors.G*.4, player.colors.B*.4, 1 };
  GLfloat specular_coeffs[] = { 0, 0, 1, 1 };


  // glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);

  float rotate_angle = 360 / mana;
  float t = -world.time*350;
  float r = .13; 

  float x,y;
  int i;
  for(i=0;i<mana;i++){

    diffuse_coeffs[0] = player.colors.R*(i+1)/mana;
    diffuse_coeffs[1] = player.colors.G*(i+1)/mana;
    diffuse_coeffs[2] = player.colors.B*(i+1)/mana;
    diffuse_coeffs[3] = 1;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    x = r * cos(t/180*M_PI);
    y = r * sin(t/180*M_PI);

    glPushMatrix();
        
        glTranslatef(x_center + x, y_center + y, z);
        glRotatef(t + 90,0,0,1);
        glScalef(small_width*0.8 ,height,.01);
        glutSolidCube(1);
    glPopMatrix();
    
    t += rotate_angle;
  }

  // emission_coeffs[0] = 0;
  // emission_coeffs[1] = 0;
  // emission_coeffs[2] = 0;
  // emission_coeffs[3] = 1;
  // glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
}
