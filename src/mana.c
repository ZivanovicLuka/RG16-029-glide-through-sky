#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "wall.h"
#include "mana.h"

Crystal crystals[WALL_COUNT]; //nece biti vise zivih kristala od zidova sigurno
float rotation = 30;
int mana_summon_index = 0;

void summon_mana(int index){
  crystals[index].alive = 1;
  crystals[index].curr_x = 1 + .75; // FIXME .75 treba da predstavlja polovinu razmaka izmedju 2 zida
}

void draw_mana_crystal(int index){
  if(crystals[index].alive){
    GLfloat diffuse_coeffs[] = { 0.0, 0.1, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    glPushMatrix();
        // glColor3f(0, 0, 1);

        glTranslatef(crystals[index].curr_x, crystals[index].curr_y , 0);

        glRotatef(rotation, 0, 1, 0);
        glScalef(1, 1.2, 1);
        glRotatef(45,1,0,1);

        glutSolidCube(.06);
    glPopMatrix();
    glutPostRedisplay();
  }
}
