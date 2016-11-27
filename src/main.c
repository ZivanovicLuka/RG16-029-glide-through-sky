#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

//==============================================================================

#include "glut_functions.h"

#include "player.h"
#include "player_externs.h"

#include "wall.h"
#include "wall_externs.h"

#include "world.h"
#include "world_externs.h"

#include "mana.h"
#include "mana_externs.h"

//==============================================================================

int main(int argc, char* argv[]){

  glutInit(&argc,argv);

  glutInitWindowSize(window_width,window_height);
  glutInitWindowPosition(100,100);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Glide Through Sky");

  // TODO PRELAZAK NA 3D
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1, 0.1, 100.0);

  glClearColor(0.0, 0.0, 0.0, 0);
  glEnable(GL_DEPTH_TEST);

  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);

  srand(time(NULL));

  strcpy(gui.score_text,"Score: 0");

  player.mana = 3;

  trails[0].pos_x = player.x_curr;
  trails[0].pos_y = 0;
  trails[0].pos_z = -.4;
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

  glutSolidCube(0); // HACK kad uvedes 3d playera, radice
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
        if(player.dashing == 0 && player.mana > 0){
          glutTimerFunc(DASH_TIMER_INTERVAL, on_timer, DASH_TIMER_ID);
          player.mana -= 1; // TODO
          player.dashing = 1;
          dash();
        }
        break;
    }
}

// FIXME ne radi uopste
static void on_reshape(int width, int height){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float)width/height, 0.1, 100.0);
}

int mana_counter = 0;
void wall_mana(){
  summon_wall(wall_summon_index);
  wall_summon_index = (++wall_summon_index == WALL_COUNT) ? 0 : wall_summon_index;

  if(wall_gap>wall_gap_min){
    wall_gap-=.03; // TODO prebaci konstantu u wall.c
  }

  mana_counter++;
  if( mana_counter == 4 ){
    summon_mana(mana_summon_index);
    mana_summon_index = (++mana_summon_index == WALL_COUNT) ? 0 : mana_summon_index;
    mana_counter = 0;
  }

  if (world.animation_ongoing) {
    glutTimerFunc(WALL_SUMMON_INTERVAL, on_timer, WALL_SUMMON_TIMER_ID);
  }
}

float distance = 0;

static void on_timer(int value){

    if(distance>=1.5){
      distance-=1.3;
      wall_mana();
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

      distance += wall_speed;

      int i;
      for(i=0;i<WALL_COUNT;i++){
        walls[i].x_curr -= wall_speed;

        //TODO SREDI OVO (MANA)
        rotation += 1.5;
        if (world.animation_ongoing) {
          crystals[i].curr_x -= wall_speed;
        }
        crystals[i].curr_y = sin(crystals[i].curr_x*5)*.08;
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

  /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
  // GLfloat light_position[] = { player.x_curr + .5, player.y_curr, 2, .8 };
  GLfloat light_position[] = { player.x_curr + .2, 0, 2, .8 };

  GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
  GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 1 };
  GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

  GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3, 1 };
  GLfloat diffuse_coeffs[] = { 0.0, 0.5, 0.8, 1 };
  GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

  /* Koeficijent glatkosti materijala. */
  GLfloat shininess = 20;

  /* Podesava se vidna tacka. */
  gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);

  /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  /* Podesavaju se parametri materijala. */
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_world();

  int i;
  for(i=0;i<WALL_COUNT;i++){
    if(!player.invulnerable){
      wall_collision(i);
    }
    mana_collision(i);
  }

  for(i=0;i<WALL_COUNT;i++){ // sigurno je manje kristala od zidova (ili jednako)
    draw_mana_crystal(i);

    draw_wall(walls[i].x_curr,i);
    check_score(i);
  }

  teleport();
  if(player.dashing){
    dash();
  }

  draw_player(player.y_curr, player.x_curr, -.5, 1, 0, 1); // FIXME jos konstanti
  draw_mana_bar(player.mana);

  for(i=0;i<trail_count;i++){
    draw_player(trails[i].pos_y, trails[i].pos_x, trails[i].pos_z, // TODO crtaj trouglove, opacity, itd
      //  rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors, // FIXME sve ovo dole
      //  rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors,
      //  rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors);
      trails[i].colors,
      0,
      trails[i].colors);
  }

// TODO Svasta sa ispisom teksta, pocevsi od plate-a pa na dalje
// FIXME ovaj tekst ubija fps
  // RenderString(-1.0, -1.0f, GLUT_BITMAP_HELVETICA_18, gui.score_text, 1.0f, 1.0f, 1.0f);

  glFlush();
  glutSwapBuffers();
}

void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{
  glColor3f(r, g, b);
  glRasterPos3f(x, y, gui.z);
  glutBitmapString(font, string);
}
