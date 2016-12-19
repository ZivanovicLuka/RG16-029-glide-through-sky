#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>


#include "misc_functions.h"

#include "player.h"
#include "player_externs.h"

#include "wall.h"
#include "wall_externs.h"

#include "world.h"
#include "world_externs.h"

#include "mana.h"
#include "mana_externs.h"

#include "enemy.h"
#include "enemy_externs.h"

#include "bullet.h"
#include "bullet_externs.h"

int main(int argc, char* argv[]){

  glutInit(&argc,argv);

  glutInitWindowSize(window_width,window_height);
  glutInitWindowPosition(100,100);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Glide Through Sky");

  glClearColor(.01, .01, .02, 0);
  glEnable(GL_DEPTH_TEST);

  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);

  dt = glutGet(GLUT_ELAPSED_TIME);

  srand(time(NULL));

  player_init();
  bullets_init();
  stars_init();
  score_init();
  wall_init();
  enemies_init();
  mana_crystal_init();

  glutMainLoop();

  return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
    switch (key) {
      case 27:
        exit(0);
        break;

      case 'g':
      case 'G':
        if (!world.animation_ongoing) {
            glutTimerFunc(PLAYER_REFRESH_TIMER_INTERVAL, on_timer, PLAYER_REFRESH_TIMER_ID);
            world.animation_ongoing = 1;
        }
        break;

      case 's':
      case 'S':
        world.animation_ongoing = 0;
        break;

      case 'q':
      case 'Q':
        if (world.animation_ongoing) {
          if(!player.dashing)
            player.v_y = world.jump;
        }
        break;

      case 'w':
      case 'W':
        if (world.animation_ongoing) {
          if(!player.dashing && player.mana > 0){
            dash_start();
          }
        }
        break;

      case 'e':
      case 'E':
        if (world.animation_ongoing) {
          heal();
        }
        break;
    }
}

static void on_reshape(int width, int height){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  window_height = height;
  window_width = width;

  int left = (width - height) / 2;
  glViewport(left, 0, height, height);
  gluPerspective(60.0, 1, .1, 100.0);
  glutPostRedisplay();
}

static void on_timer(int value){
    if(value == PLAYER_REFRESH_TIMER_ID){

      /* Correction for different performances */
      updateDeltaTime();
      speed_correction = dt / (float)PLAYER_REFRESH_TIMER_INTERVAL;
      float ms = wall_speed * speed_correction;

      if(!alive()){
        restart();
      }

      if(world.wall_summon_distance>=1.5){
        world.wall_summon_distance-=1.3;
        summon_wall();
        summon_enemy(); 
        enemies_fire();
        summon_mana();
      }
      world.wall_summon_distance += ms;

      if(world.trail_summon_distance>=.075){
        world.trail_summon_distance-=.075;
        summon_trail();
      }
      world.trail_summon_distance += ms;

      player_move();
      stars_move();
      walls_move(ms);
      enemies_move(ms,speed_correction);
      mana_crystal_move(ms);
      bullets_move(ms);
 
      mana_enemies_collision();
      bullets_player_collision();
      bullets_walls_turrets_collision();
      bullets_world_collision();
      mana_collision();
      
      teleport();
      dashing(ms);
      enemies_aim();
      mana_circle_init_animation(ms);
      check_score();

      glutPostRedisplay();

      if (world.animation_ongoing) {
          glutTimerFunc(PLAYER_REFRESH_TIMER_INTERVAL, on_timer, PLAYER_REFRESH_TIMER_ID);
      }
    }
}

void on_display(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  fps(1);

  /* Light positon */
  GLfloat light_position[] = { 0, 0, 2.45, 1 };

  GLfloat light_ambient[] = { .1, .1, .1, 1 };
  GLfloat light_diffuse[] = { .7, .7, .7, 1 };
  GLfloat light_specular[] = { .9, .9, .9, 1 };

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


  /* Koeficijent glatkosti materijala. */
  GLfloat ambient_coeffs[] = { .3, .3, .3, 1 };
  GLfloat specular_coeffs[] = { .5, .5, .5, 1 };
  GLfloat shininess = 100;

    /* Podesavaju se parametri materijala. */
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

  /* Podesava se vidna tacka. */

  gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);
  glEnable(GL_NORMALIZE);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_world();
  draw_walls();
  draw_enemies();
  draw_bullets();
  draw_mana_crystal();
  draw_mana_circle(player.mana);
  draw_trail();
  draw_player();
  draw_score();

  glFlush();
  glutSwapBuffers();
}

