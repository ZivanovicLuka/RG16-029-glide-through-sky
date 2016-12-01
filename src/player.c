#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"
#include "wall_externs.h"
#include "mana.h"
#include "mana_externs.h"


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

// 1,0,1 FF00FF = Neon Pink
// 0,1,0  00FF00 = Neon Green
// 1,1,0  FFFF00 = Neon Yellow
// 1,.2,0 FF3300 = Neon Orange
// 1,0,0  FF0009 = Neon Red
// .5,0,1 6600FF = Neon Purple
// 0,1,.6 00FF99 = Neon Mint Green
// 0,0,.8 0000BF = Neon Dark Blue
// 0,0,1  0000FF = Neon Blue
// 0,1,1  00FFFF = Light Blue

Color3f global_colors[] = {{1,0,1},{0,1,0},{1,1,0},{1,.2,0},{1,0,0},{.5,0,1},{0,1,.6},{0,0,.5},{0,0,1},{0,1,1}};
int global_colors_number = 10;

Trail trails[TRAIL_MAX];
float trail_x_move = .05;
float trail_color_alpha = 1;
int trail_count = -5;

void draw_player(float y, float x, float colorR, float colorG, float colorB){

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
     exit(0);
     return;
  }
}

void mana_collision(int index){
  if(!crystals[index].alive)
    return;

  float mana_width = 0.085; // 0.06 * sqrt(2), posto je dijagonala

  float mana_bot = crystals[index].curr_y - mana_width/2;
  float mana_top = crystals[index].curr_y + mana_width/2;
  float mana_left = crystals[index].curr_x - mana_width/2;
  float mana_right = crystals[index].curr_x + mana_width/2;

  float PLAYER_bot = player.y_curr - player.size/2;
  float PLAYER_top = player.y_curr + player.size/2;
  float PLAYER_left = player.x_curr - player.size/2;
  float PLAYER_right = player.x_curr + player.size/2;

  if(PLAYER_top >= mana_bot && PLAYER_left <= mana_right && PLAYER_right >= mana_left &&
  PLAYER_bot <= mana_top){
     player.mana++;
     if(player.dashing)
        player.mana++;

      crystals[index].alive = 0;
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

void dash_start(){
  // FIXME
    // if(player.dashing == 0){
    //   glutTimerFunc(DASH_TIMER_INTERVAL, on_timer, DASH_TIMER_ID);
    //   // mana -= 5; // TODO
    //   player.dashing = 1;
    //   dash();
    // }
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
  }

  trails[0].pos_y = player.y_curr;

  if(trail_count < TRAIL_MAX)
    trail_count++;
}
