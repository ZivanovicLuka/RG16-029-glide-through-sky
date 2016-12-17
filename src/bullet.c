#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"
#include "wall.h"
#include "bullet.h"

Bullet bullets[WALL_COUNT][BULLET_COUNT];

float bullet_dmg = 5;

void bullets_init(){
    int i,j;
    for(i=0;i<WALL_COUNT;i++){
        for(j=0;j<BULLET_COUNT;j++){
            bullets[i][j].alive=0;
        }
    }
}

void draw_bullet(int enemy_index, int bullet_index){
    if(!bullets[enemy_index][bullet_index].alive)
        return;
    GLfloat diffuse_coeffs[] = { 1, 1, 1, .1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    glPushMatrix();
        glTranslatef(bullets[enemy_index][bullet_index].x_curr,
                        bullets[enemy_index][bullet_index].y_curr,
                        0);
        
        glRotatef(bullets[enemy_index][bullet_index].angle,
                    0,
                    0,
                    1);
        
        glutSolidCube(.03);
    glPopMatrix();
}