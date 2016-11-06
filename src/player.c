#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "player.h"
#include "wall.h"
#include "wall_externs.h"


Player player = {
  -.5, //player.x_curr
  0,   //player.y_curr
  0,   //player.v_y
  .1,  //player.size
  0,   //player.mana
  0,   //player.invulnerable
  0    //player.dashing
};

Trail trails[TRAIL_MAX];
float trail_x_move = .05;
float trail_color_alpha = 1;
int trail_count = -5;

void draw_player(float y, float x, float z, float colorR, float colorG, float colorB){
  glBegin(GL_POLYGON);
    glColor3f( colorR, colorG, colorB );
    glVertex3f(  player.size/2 + x, -player.size/2 + y, z );
    glVertex3f(  player.size/2 + x,  player.size/2 + y, z );
    glVertex3f( -player.size/2 + x,  player.size/2 + y, z );
    glVertex3f( -player.size/2 + x, -player.size/2 + y, z );
  glEnd();
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
    wall_speed = 0.05;
  }

  else{
    player.invulnerable = 0;
    wall_speed = 0.02;
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
