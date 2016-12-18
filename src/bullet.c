#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "player_externs.h"
#include "wall.h"
#include "wall_externs.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_externs.h"

Bullet bullets[WALL_COUNT][BULLET_COUNT];

float bullet_dmg = 10;
float bullet_size = .03;

void bullets_init(){
    int i,j;
    for(i=0;i<WALL_COUNT;i++){
        for(j=0;j<BULLET_COUNT;j++){
            bullets[i][j].alive=0;
        }
    }
}

void bullets_player_collision(){
    int i,j;
    for(i=0;i<WALL_COUNT;i++){
        for(j=0;j<BULLET_COUNT;j++){
            if(!bullets[i][j].alive)
                continue;

            float bullet_bot = bullets[i][j].curr_y - bullet_size/2;
            float bullet_top = bullets[i][j].curr_y + bullet_size/2;
            float bullet_left = bullets[i][j].curr_x - bullet_size/2;
            float bullet_right = bullets[i][j].curr_x + bullet_size/2;

            float player_bot = player.curr_y - player.size/2;
            float player_top = player.curr_y + player.size/2;
            float player_left = player.curr_x - player.size/2;
            float player_right = player.curr_x + player.size/2;

            if(player_top >= bullet_bot && player_left <= bullet_right &&
               player_right >= bullet_left && player_bot <= bullet_top){
                bullets[i][j].alive = 0;
                if(player.dashing)
                    player.hp -= bullet_dmg/2;
                else
                    player.hp -= bullet_dmg;
            }
        }
    }
}

void bullets_walls_turrets_collision(){
    int i,j,k;
    for(i=0;i<WALL_COUNT;i++){
        for(j=0;j<BULLET_COUNT;j++){
            for(k=0;k<WALL_COUNT;k++){
                if(!bullets[i][j].alive)
                    continue;

                float bullet_bot = bullets[i][j].curr_y - bullet_size/2;
                float bullet_top = bullets[i][j].curr_y + bullet_size/2;
                float bullet_left = bullets[i][j].curr_x - bullet_size/2;
                float bullet_right = bullets[i][j].curr_x + bullet_size/2;

                float wall_bot = walls[k].y_bot;
                float wall_top = walls[k].y_top;
                float wall_left = walls[k].curr_x - wall_width/2;
                float wall_right = walls[k].curr_x + wall_width/2;

               if((bullet_top >= wall_bot && bullet_left <= wall_right && bullet_right >= wall_left) ||
  (bullet_bot <= wall_top && bullet_left <= wall_right && bullet_right >= wall_left)){
                    bullets[i][j].alive = 0;
                }
                if(enemies[k].alive==1 && bullets[i][j].alive){
                    float enemy_height = 0.14; // TODO iz enemy.c
                    float enemy_width = 0.16;

                    float enemy_bot = enemies[k].curr_y - enemy_height/2;
                    float enemy_top = enemies[k].curr_y + enemy_height/2;
                    float enemy_left = enemies[k].curr_x - enemy_height/2;
                    float enemy_right = enemies[k].curr_x + enemy_height/2;

                    if(enemy_top >= bullet_bot && enemy_left <= bullet_right &&
                        enemy_right >= bullet_left && enemy_bot <= bullet_top){
                        bullets[i][j].alive = 0;
                        enemies[k].alive = DYING;
                    }
                }
            }
        }
    }
}

void bullets_world_collision(){

}

void draw_bullet(int enemy_index, int bullet_index){
    if(!bullets[enemy_index][bullet_index].alive)
        return;
    GLfloat diffuse_coeffs[] = { 1, 1, 1, .1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

    glPushMatrix();
        glTranslatef(bullets[enemy_index][bullet_index].curr_x,
                        bullets[enemy_index][bullet_index].curr_y,
                        0);
        
        glRotatef(bullets[enemy_index][bullet_index].angle,
                    0,
                    0,
                    1);
        
        glutSolidCube(bullet_size);
    glPopMatrix();
}