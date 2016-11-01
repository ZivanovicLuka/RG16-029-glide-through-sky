#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//==============================================================================

#ifndef GLUT_FUCNTIONS_H
  #define GLUT_FUCNTIONS_H
  #include "glut_functions.h"
#endif

//==============================================================================

#ifndef PLAYER_H
  #define PLAYER_H
  #include "player.h"
#endif

#include "player_externs.h"

//==============================================================================

#ifndef WALL_H
  #define WALL_H
  #include "wall.h"
#endif

#include "wall_externs.h"

//==============================================================================

#ifndef WORLD_H
  #define WORLD_H
  #include "world.h"
#endif

#include "world_externs.h"

//==============================================================================

int main(int argc, char* argv[]){

  glutInit(&argc,argv);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Glide Through Sky");

  glClearColor(0.0, 0.0, 0.0, 0);
  glEnable(GL_DEPTH_TEST);

  glutKeyboardFunc(on_keyboard);
  glutDisplayFunc(on_display);

  srand(time(NULL));

  strcpy(gui.score_text,"Score: 0");

  trails[0].pos_x = player.x_curr;
  trails[0].pos_y = 0;
  trails[0].pos_z = -.9;
  trails[0].colors = trail_color_alpha;

  int i;
  for(i=1; i<TRAIL_MAX; i++){
    trails[i].pos_x = trails[i-1].pos_x - trail_x_move;
    trails[i].pos_y = 0;
    trails[i].pos_z = trails[i-1].pos_z + .001; // FIXME magicna konstanta
    trails[i].colors = trails[i-1].colors - trail_color_alpha/TRAIL_MAX;
  }

  for(i=0; i<WALL_COUNT; i++){
    walls[i].pass = 1;
    walls[i].alive = 0;
  }

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
            glutTimerFunc(WALL_SUMMON_INTERVAL, on_timer, WALL_SUMMON_TIMER_ID);
            glutTimerFunc(TRAIL_TIMER_INTERVAL, on_timer, TRAIL_TIMER_ID);
            world.animation_ongoing = 1;
        }
        break;

      case 's':
      case 'S':
        world.animation_ongoing = 0;
        break;

      case 'q':
      case 'Q':
        player.v_y = world.jump;
        break;

      case 'w':
      case 'W':
        if(player.dashing == 0){
          glutTimerFunc(DASH_TIMER_INTERVAL, on_timer, DASH_TIMER_ID);
          // mana -= 5; // TODO
          player.dashing = 1;
          dash();
        }
        break;
    }
}

static void on_timer(int value){
    if (value == WALL_SUMMON_TIMER_ID){
      summon_wall(wall_summon_index);
      wall_summon_index = (++wall_summon_index == WALL_COUNT) ? 0 : wall_summon_index;

      if(wall_gap>wall_gap_min){
        wall_gap-=.03;
      }

      if (world.animation_ongoing) {
        glutTimerFunc(WALL_SUMMON_INTERVAL, on_timer, WALL_SUMMON_TIMER_ID);
      }
    }

    if(value == TRAIL_TIMER_ID){
      summon_trail();
      if (world.animation_ongoing) {
        glutTimerFunc(TRAIL_TIMER_INTERVAL, on_timer, TRAIL_TIMER_ID);
      }
    }

    if(value == PLAYER_REFRESH_TIMER_ID){
      player.y_curr += player.v_y;
      if(player.v_y > -.05){ // FIXME konstanta
        player.v_y -= world.gravity;
      }

      int i;
      for(i=0;i<WALL_COUNT;i++){
        walls[i].x_curr -= wall_speed;
      }

      glutPostRedisplay();

      if (world.animation_ongoing) {
          glutTimerFunc(PLAYER_REFRESH_TIMER_INTERVAL, on_timer, PLAYER_REFRESH_TIMER_ID);
      }
    }

    if(value == DASH_TIMER_ID){
      if(player.dashing){
        player.dashing = 0;
      }
      dash();
    }
}

void on_display(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  int i;
  if(!player.invulnerable){
    for(i=0;i<WALL_COUNT;i++){
      wall_collision(i);
    }
  }

  for(i=0;i<WALL_COUNT;i++){
    draw_wall(walls[i].x_curr,i);
    check_score(i);
  }

  teleport();
  if(player.dashing){
    dash();
  }

  draw_player(player.y_curr, player.x_curr, -1., 1, 1, 1); // FIXME jos konstanti
  // -1. je z

  for(i=0;i<trail_count;i++){
    draw_player(trails[i].pos_y, trails[i].pos_x, trails[i].pos_z,
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors, // FIXME sve ovo dole
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors,
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors);
  }

// TODO Svasta sa ispisom teksta, pocevsi od plate-a pa na dalje

  RenderString(-1.0, -1.0f, GLUT_BITMAP_HELVETICA_18, gui.score_text, 1.0f, 1.0f, 1.0f);

  glFlush();
  glutSwapBuffers();
}

void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{
  glColor3f(r, g, b);
  glRasterPos3f(x, y, gui.z);
  glutBitmapString(font, string);
}
