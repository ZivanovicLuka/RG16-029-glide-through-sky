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
/* Array with players colors */
/* Random ones can look too dim */
Color3f global_colors[] = {{1,0,1},{0,1,0},{1,1,0},{1,.2,.2},{.5,0,1},{0,1,.6},{.3,.2,1},{0,1,1}};
int global_colors_number = 8;

Trail trails[TRAIL_MAX];
float trail_x_move = .04;
float trail_color_alpha = 1;
int trail_count;

int max_hp = 100;
int heal_hp = 30;

void player_init()
{
  player = (Player){
    -.5, /*player.curr_x*/
    0,   /*player.curr_y*/
    0,   /*player.v_y*/
    .1,  /*player.size*/
    70,  /*player.hp*/
    global_colors[(int)(rand()/(float)RAND_MAX * global_colors_number)], /*player.colors*/
    0,   /*player.mana*/
    0,   /*player.invulnerable*/
    0,   /*player.dashing*/
    0    /*player.dash_distance*/
  };

  /* Trail init */
  trail_count = -5; /* Delay for drawing trails at start */
  trails[0].curr_x = player.curr_x;
  trails[0].curr_y = 0;
  trails[0].pos_z = -.4;
  trails[0].size = player.size * .3;
  trails[0].colors = trail_color_alpha;

  /* Every trail takes position of the last one + trail_x_move */
  /* Every trail takes darker color then the last one */
  int i;
  for(i=1; i<TRAIL_MAX; i++){
    trails[i].curr_x = trails[i-1].curr_x - trail_x_move;
    trails[i].curr_y = 0;
    trails[i].pos_z = trails[i-1].pos_z;
    trails[i].colors = trails[i-1].colors - trail_color_alpha/TRAIL_MAX;
  }
}

void draw_player()
{
  float x = player.curr_x;
  float y = player.curr_y;
  float colorR = player.colors.R;
  float colorG = player.colors.G;
  float colorB = player.colors.B;

  GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);


  glPushMatrix();
      glTranslatef(x,y,0);
      glutSolidCube(player.size);
  glPopMatrix();
  glutPostRedisplay();
}

void wall_collision(int index)
{
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

  /* If palyer is over bottom of upper wall, and in its width, collision is true */
  /* Same for other wall part */
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

int alive()
{
  if(player.hp <= 0)
    return 0;
  return 1;
}

void mana_collision()
{
  if(!crystal.alive)
    return;

  float mana_height = .085; /* .06 * sqrt(2), because cube is rotated */

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

void enemy_collision(int index)
{
  if(enemies[index].alive != 1)
    return;

  float enemy_height = .14; 
  float enemy_width = .16;

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
     /* If player hits enemy while dashing, it drops 1 or 2 mana */
     if(player.dashing){
        player.mana += (int)rand()/(float)RAND_MAX * 2 + 1;
     } else {
       /* If player hits enemy while not dashing, takes 30dmg */
       player.hp -= 30;
     }
  }
}

void mana_enemies_collision()
{
  int i;
  for(i=0;i<WALL_COUNT;i++){
      wall_collision(i);
      enemy_collision(i);
  }
}

/* Teleport throgh floor to ceil and other way */
void teleport()
{
  if(player.curr_y < -1){
    player.curr_y = 1;
  }

  if(player.curr_y > 1){
    player.curr_y = -1;
  }
}

/* If E is pressed, player loses 1 mana and ganis 30hp (not more than 100 total) */
void heal()
{
  if(player.mana >0 && player.hp < max_hp){
    player.mana--;
    if(player.hp + heal_hp > max_hp)
      player.hp = 100;
    else
      player.hp += heal_hp; 
  }
}

void player_move()
{
  player.curr_y += player.v_y * speed_correction;
  /* Fix for not falling too fast */
  if(player.v_y > -.015*speed_correction){
    player.v_y -= world.gravity * speed_correction;
  }
}

void dash_start()
{
  player.mana -= 1;
  player.dashing = 1;
  int pom = (int)(rand()/(float)RAND_MAX * global_colors_number); /* Generates one color from color array */
  Color3f new = global_colors[pom];
  /* If color is the same like last one, get next one */
  if(player.colors.R == new.R && player.colors.G == new.G && player.colors.B == new.B){
    player.colors = global_colors[(pom+1)%global_colors_number];
  } else
    player.colors = new;
  dash();
}

void dash()
{
  if(player.dashing){
    player.v_y = 0;
    wall_speed = .03;
  }
  else{
    wall_speed = .013;
  }
}

void dashing(float ms)
{
  dash();
  if(player.dashing)
    player.dash_distance += ms; /* Counts how long dash lasts */
  /* If dash distance is big enough, stop dashing and reset dash distance */
  if(player.dash_distance >= .95){ 
    if(player.dashing){
      player.dashing = 0;
    }
    player.dash_distance = 0;
  }
}

void summon_trail()
{
  int i;
  /* Trail represents players hp */
  /* +2 is default */
  int n = (TRAIL_MAX-2)*(float)player.hp/100 + 2;

  /* When new spawns, older one goes 1 step behind */
  for(i=n-1; i>=1; i--){
    trails[i].curr_y = trails[i-1].curr_y;
    trails[i].size = trails[i-1].size;
  }

  /* Random generate trail particles up and down */
  float dy = rand()/(float)RAND_MAX * player.size - player.size/2;

  trails[0].curr_y = player.curr_y + dy;

  /* Size is also random */
  trails[0].size = (rand()/(float)RAND_MAX * .3 + .4)* player.size;

  if(trail_count < n)
    trail_count++;
  else
    trail_count = n;
}

void draw_trail_particle(int index)
{

  float x = trails[index].curr_x;
  float y = trails[index].curr_y;
  float colorR = player.colors.R * trails[index].colors;
  float colorG = player.colors.G * trails[index].colors;
  float colorB = player.colors.B * trails[index].colors;
  float size = trails[index].size;

  GLfloat diffuse_coeffs[] = { colorR, colorG, colorB, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

  glPushMatrix();
      glTranslatef(x,y,0);
      glScalef(size,size,size);
      glutSolidCube(1);
  glPopMatrix();
  glutPostRedisplay();
}

void draw_trail()
{
  int i;
  for(i=0;i<trail_count;i++){
    draw_trail_particle(i);
  }
}