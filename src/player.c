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


Player player;

Color3f global_colors[] = {{1,0,1},{0,1,0},{1,1,0},{1,.2,.2},{.5,0,1},{0,1,.6},{.3,.2,1},{0,1,1}};
int global_colors_number = 8;

Trail trails[TRAIL_MAX];
float trail_x_move = .04;
float trail_color_alpha = 1;
int trail_count;

int max_hp = 100;
int heal_hp = 30;

void player_init(){
  player = (Player){
    -.5, //player.curr_x
    0,   //player.curr_y
    0,   //player.v_y
    .1,  //player.size
    70,  //player.hp
    global_colors[(int)(rand()/(float)RAND_MAX * global_colors_number)], // player.colors
    0,   //player.mana
    0,   //player.invulnerable
    0,    //player.dashing
    0    //player.dash_distance
  };

  trail_count = -5;
  trails[0].pos_x = player.curr_x;
  trails[0].pos_y = 0;
  trails[0].pos_z = -.4;
  trails[0].size = player.size * .3;
  trails[0].colors = trail_color_alpha;

  int i;
  for(i=1; i<TRAIL_MAX; i++){
    trails[i].pos_x = trails[i-1].pos_x - trail_x_move;
    trails[i].pos_y = 0;
    trails[i].pos_z = trails[i-1].pos_z + .001;
    trails[i].colors = trails[i-1].colors - trail_color_alpha/TRAIL_MAX;
  }
}

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
  float wall_left = walls[index].curr_x - wall_width/2;
  float wall_right = walls[index].curr_x + wall_width/2;

  float player_bot = player.curr_y - player.size/2;
  float player_top = player.curr_y + player.size/2;
  float player_left = player.curr_x - player.size/2;
  float player_right = player.curr_x + player.size/2;

  if((player_top >= wall_bot && player_left <= wall_right && player_right >= wall_left) ||
  (player_bot <= wall_top && player_left <= wall_right && player_right >= wall_left)){
    if(!player.dashing){
      restart();
    } else {
      walls[index].hollow = 1;
      walls[index].hole_y = player.curr_y;
    }
   return;
  }
}

int alive(){
  if(player.hp <= 0)
    return 0;
  return 1;
}

void mana_collision(){
  if(!crystal.alive)
    return;

  float mana_height = 0.085; // 0.06 * sqrt(2), posto je dijagonala

  float mana_bot = crystal.curr_y - mana_height/2;
  float mana_top = crystal.curr_y + mana_height/2;
  float mana_left = crystal.curr_x - mana_height/2;
  float mana_right = crystal.curr_x + mana_height/2;

  float player_bot = player.curr_y - player.size/2;
  float player_top = player.curr_y + player.size/2;
  float player_left = player.curr_x - player.size/2;
  float player_right = player.curr_x + player.size/2;

  if(player_top >= mana_bot && player_left <= mana_right && player_right >= mana_left &&
  player_bot <= mana_top){
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

  float enemy_bot = enemies[index].curr_y - enemy_height/2;
  float enemy_top = enemies[index].curr_y + enemy_height/2;
  float enemy_left = enemies[index].curr_x - enemy_height/2;
  float enemy_right = enemies[index].curr_x + enemy_height/2;

  float player_bot = player.curr_y - player.size/2;
  float player_top = player.curr_y + player.size/2;
  float player_left = player.curr_x - player.size/2;
  float player_right = player.curr_x + player.size/2;

  if(player_top >= enemy_bot && player_left <= enemy_right && player_right >= enemy_left &&
  player_bot <= enemy_top){
     enemies[index].alive = DYING;
     enemies[index].dying_time = 0;
     if(player.dashing){
        player.mana += (int)rand()/(float)RAND_MAX * 2 + 1;
     } else {
       player.hp -= 50;
     }
  }
}

void mana_enemies_collision(){
  int i;
  for(i=0;i<WALL_COUNT;i++){
        wall_collision(i);
        enemy_collision(i);
  }
}

void teleport(){
  if(player.curr_y < -1){
    player.curr_y = 1;
  }

  if(player.curr_y > 1){
    player.curr_y = -1;
  }
}

void heal(){
  if(player.mana >0 && player.hp < max_hp){
    player.mana--;
    if(player.hp + heal_hp > max_hp)
      player.hp = 100;
    else
      player.hp += heal_hp; 
  }
}

void player_move(){
  player.curr_y += player.v_y * speed_correction;
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
  int n = (TRAIL_MAX-2)*(float)player.hp/100 + 2;

  for(i=n-1; i>=1; i--){
    trails[i].pos_y = trails[i-1].pos_y;
    trails[i].size = trails[i-1].size;
  }

  float dy = rand()/(float)RAND_MAX * player.size - player.size/2;

  trails[0].pos_y = player.curr_y + dy;
  trails[0].size = (rand()/(float)RAND_MAX * .3 + .4)* player.size;

  if(trail_count < n)
    trail_count++;
  else
    trail_count = n;
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
