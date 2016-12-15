#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"
#include "wall_externs.h"
#include "world.h"
#include "world_externs.h"
#include "mana.h"
#include "mana_externs.h"
#include "enemy.h"
#include "enemy_externs.h"


Player player = {
  -.5, //player.x_curr
  0,   //player.y_curr
  0,   //player.v_y
  .1,  //player.size
  {1,   //player.colorR
  0,   //player.colorG
  1},   //player.colorB
  0,   //player.mana
  0,   //player.invulnerable
  0    //player.dashing
};

Color3f global_colors[] = {{1,0,1},{0,1,0},{1,1,0},{1,.2,.2},{.5,0,1},{0,1,.6},{0,0,1},{0,1,1}};
int global_colors_number = 8;

Trail trails[TRAIL_MAX];
float trail_x_move = .04;
float trail_color_alpha = 1;
int trail_count = -5;

void draw_player(float x, float y, float colorR, float colorG, float colorB){

  GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);


  glPushMatrix();
      glTranslatef(x,y,0);
      glutSolidCube(player.size);
  glPopMatrix();
  glutPostRedisplay();

}

void wall_collision(int index){
  if(!walls[index].alive)
    return;

  float wall_bot = walls[index].y_bot;
  float wall_top = walls[index].y_top;
  float wall_left = walls[index].x_curr - wall_width/2;
  float wall_right = walls[index].x_curr + wall_width/2;

  float PLAYER_bot = player.y_curr - player.size/2;
  float PLAYER_top = player.y_curr + player.size/2;
  float PLAYER_left = player.x_curr - player.size/2;
  float PLAYER_right = player.x_curr + player.size/2;

  if((PLAYER_top >= wall_bot && PLAYER_left <= wall_right && PLAYER_right >= wall_left) ||
  (PLAYER_bot <= wall_top && PLAYER_left <= wall_right && PLAYER_right >= wall_left)){
    if(!player.invulnerable){
      printf("Score: %d\n", world.score);
     exit(0);
    } else {
      walls[index].hollow = 1;
      walls[index].hole_y = player.y_curr;
    }
   return;
  }
}

void mana_collision(){
  if(!crystal.alive)
    return;

  float mana_height = 0.085; // 0.06 * sqrt(2), posto je dijagonala

  float mana_bot = crystal.curr_y - mana_height/2;
  float mana_top = crystal.curr_y + mana_height/2;
  float mana_left = crystal.curr_x - mana_height/2;
  float mana_right = crystal.curr_x + mana_height/2;

  float PLAYER_bot = player.y_curr - player.size/2;
  float PLAYER_top = player.y_curr + player.size/2;
  float PLAYER_left = player.x_curr - player.size/2;
  float PLAYER_right = player.x_curr + player.size/2;

  if(PLAYER_top >= mana_bot && PLAYER_left <= mana_right && PLAYER_right >= mana_left &&
  PLAYER_bot <= mana_top){
     player.mana++;
     if(player.dashing)
        player.mana++;

      crystal.alive = 0;
      return;
  }
}

void enemy_collision(int index){
  if(enemies[index].alive != 1)
    return;

  float enemy_height = 0.14; // TODO iz enemy.c
  float enemy_width = 0.16;

  float enemy_bot = enemies[index].y_curr - enemy_height/2;
  float enemy_top = enemies[index].y_curr + enemy_height/2;
  float enemy_left = enemies[index].x_curr - enemy_height/2;
  float enemy_right = enemies[index].x_curr + enemy_height/2;

  float PLAYER_bot = player.y_curr - player.size/2;
  float PLAYER_top = player.y_curr + player.size/2;
  float PLAYER_left = player.x_curr - player.size/2;
  float PLAYER_right = player.x_curr + player.size/2;

  if(PLAYER_top >= enemy_bot && PLAYER_left <= enemy_right && PLAYER_right >= enemy_left &&
  PLAYER_bot <= enemy_top){
     if(player.dashing){
        enemies[index].alive = DYING;
        enemies[index].dying_time = 0;
        player.mana += (int)rand()/(float)RAND_MAX * 2.8;
     }
      return;
  }
}

void teleport(){
  if(player.y_curr < -1){
    player.y_curr = 1;
  }

  if(player.y_curr > 1){
    player.y_curr = -1;
  }
}

void player_move(){
  player.y_curr += player.v_y * speed_correction;
  if(player.v_y > -.029){ // FIXME konstanta
    player.v_y -= world.gravity * speed_correction;
  }
}

void dash_start(){
  player.mana -= 1;
  player.dashing = 1;
  int pom = (int)(rand()/(float)RAND_MAX * global_colors_number);
  Color3f new = global_colors[pom];
  if(player.colors.R == new.R && player.colors.G == new.G && player.colors.B == new.B){ // ako na rand da istu boju, uzmi sledecu

    player.colors = global_colors[(pom+1)%global_colors_number];
  } else
    player.colors = new;
  dash();
}

void dash(){
  if(player.dashing){
    player.invulnerable = 1;
    player.v_y = 0;
    wall_speed = 0.03;
  }
  else{
    player.invulnerable = 0;
    wall_speed = 0.013;
  }
}

void summon_trail(){
  int i;
  int n = TRAIL_MAX;

  for(i=n-1; i>=1; i--){
    trails[i].pos_y = trails[i-1].pos_y;
    trails[i].size = trails[i-1].size;
  }

  float dy = rand()/(float)RAND_MAX * player.size - player.size/2;

  trails[0].pos_y = player.y_curr + dy;
  trails[0].size = (rand()/(float)RAND_MAX * .3 + .4)* player.size;

  if(trail_count < TRAIL_MAX)
    trail_count++;
}

void draw_trail(float x, float y, float colorR, float colorG, float colorB, float size){

  rand()/(float)RAND_MAX * player.size - player.size/2;
  GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  glPushMatrix();
      glTranslatef(x,y,0);
      glScalef(size,size,size);
      glutSolidCube(1);
  glPopMatrix();
  glutPostRedisplay();
}
