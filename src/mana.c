#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "wall.h"
#include "mana.h"

#include "player.h"
#include "player_externs.h"

Crystal crystal; //nece biti vise zivih kristala od zidova sigurno
float mana_crystal_rotation = 30;
int walls_passed_counter = 0;

void summon_mana(int index){
  crystal.alive = 1;
  crystal.curr_x = 1 + .55; // FIXME .55 treba da predstavlja polovinu razmaka izmedju 2 zida
}

void draw_mana_crystal(){
  if(crystal.alive){
    GLfloat diffuse_coeffs[] = { 0.0, .2, 1, .1 };
    GLfloat emission_coeffs[] = { .03, .03, .35, 1 };

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
        glScalef(1, 1.2, 1);
        glRotatef(45,1,0,1);

        glutWireCube(.065);
    glPopMatrix();

    emission_coeffs[0] = 0;
    emission_coeffs[1] = 0;
    emission_coeffs[2] = 0;
    emission_coeffs[3] = 1;

    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);


  }
}

void draw_mana_bar(int mana){
  float height = .02;

  float small_width = .08;

  float start_x = -.9;
  float y = .92;
  float z = .3;

  // GLfloat emission_coeffs[] = { 0, 0, 1, 1 };
  GLfloat diffuse_coeffs[] = { 0, 0, 1, 1 };
  GLfloat specular_coeffs[] = { 0, 0.05, 1, 1 };


  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  // glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);


  int i;
  for(i=0;i<mana;i++){

    glPushMatrix();
        glTranslatef(start_x + i*small_width, y, z);
        glScalef(small_width*0.8 ,height,.001);
        glutSolidCube(1);
    glPopMatrix();

  }
  // emission_coeffs[0] = 0;
  // emission_coeffs[1] = 0;
  // emission_coeffs[2] = 0;
  // emission_coeffs[3] = 1;
  // glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
}
