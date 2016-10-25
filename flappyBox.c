#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

#define BOX_REFRESH_TIMER_ID 0
#define BOX_REFRESH_TIMER_INTERVAL 10

#define WALL_SUMMON_TIMER_ID 1
#define WALL_SUMMON_INTERVAL 1000

#define TRAIL_TIMER_ID 2
#define TRAIL_TIMER_INTERVAL 60

#define DASH_TIMER_ID 3
#define DASH_TIMER_INTERVAL 500


static float gravity = .0015;
static float jump = .035;
static int score = 0;

/* PLAYER */
static float box_x_curr = -.5;
static float box_size = 0.1;
static float box_y_curr = 0;
static float box_v_y = 0;
static int invulnerable = 0;
static int dashing = 0;

/* TRAIL */
#define TRAIL_MAX 8
static float trail_x_move = .05;
static float trail_pos_x[TRAIL_MAX];
static float trail_pos_y[TRAIL_MAX];
static float trail_pos_z[TRAIL_MAX];
static float trail_colors[TRAIL_MAX];
static float trail_color_alpha = 1;
static int trail_count = -5;

const static float wall_width = .3;
static float wall_speed = 0.02;
static float wall_gap = 1.;
static float wall_gap_min = .6;
static int summon_index = 0;

static float wall_x_curr[5];
static float wall_y_bot[5];
static float wall_y_top[5];
static int alive[5];
static int pass[5];
static float color1[5];
static float color2[5];
static float color3[5];

static int animation_ongoing = 0;

void draw_box(float y, float x, float z, float R, float G, float B);
void draw_wall(float x, int index);
void summon_wall(int index);
void summon_trail();
void collision(int index);
void check_score(int index);
void teleport();
void dash();
void dash_start();

static void on_timer(int value);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);

int main(int argc, char* argv[]){

  glutInit(&argc,argv);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Flappy Box");

  glClearColor(0.0, 0.0, 0.0, 0);
  glEnable(GL_DEPTH_TEST);

  glutKeyboardFunc(on_keyboard);
  glutDisplayFunc(on_display);

  srand(time(NULL));

  int i;

  trail_pos_x[0] = box_x_curr;
  trail_pos_y[0] = 0;
  trail_pos_z[0] = .1;
  trail_colors[0] = trail_color_alpha;
  for(i=1; i<TRAIL_MAX; i++){
    trail_pos_x[i] = trail_pos_x[i-1] - trail_x_move;
    trail_pos_y[i] = 0;
    trail_pos_z[i] = trail_pos_z[i-1] + .1;
    trail_colors[i] = trail_colors[i-1] - trail_color_alpha/TRAIL_MAX;
  }

  for(i=0; i<5; i++){
    pass[i] = 1;
    alive[i] = 0;
  }

  glutMainLoop();

  return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
      case 27:
        exit(0);
        break;

      case 'g':
      case 'G':
        if (!animation_ongoing) {
            glutTimerFunc(BOX_REFRESH_TIMER_INTERVAL, on_timer, BOX_REFRESH_TIMER_ID);
            glutTimerFunc(WALL_SUMMON_INTERVAL, on_timer, WALL_SUMMON_TIMER_ID);
            glutTimerFunc(TRAIL_TIMER_INTERVAL, on_timer, TRAIL_TIMER_ID);
            animation_ongoing = 1;
        }
        break;

      case 's':
      case 'S':
        animation_ongoing = 0;
        break;

      case 'q':
      case 'Q':
        box_v_y = jump;
        break;

      case 'w':
      case 'W':
        dash_start();
        break;
    }
}

static void on_timer(int value)
{
    if (value == WALL_SUMMON_TIMER_ID){
      summon_wall(summon_index);
      summon_index = (++summon_index == 5) ? 0 : summon_index;
      if(wall_gap>wall_gap_min)
        wall_gap-=.03;

      if (animation_ongoing) {
        glutTimerFunc(WALL_SUMMON_INTERVAL, on_timer, WALL_SUMMON_TIMER_ID);
      }
    }

    if(value == TRAIL_TIMER_ID){
      summon_trail();
      if (animation_ongoing) {
        glutTimerFunc(TRAIL_TIMER_INTERVAL, on_timer, TRAIL_TIMER_ID);
      }
    }

    if(value == BOX_REFRESH_TIMER_ID){
      box_y_curr += box_v_y;
      if(box_v_y > -.05)
        box_v_y -= gravity;

      int i;
      for(i=0;i<5;i++){
        wall_x_curr[i] -= wall_speed;
      }

      glutPostRedisplay();

      if (animation_ongoing) {
          glutTimerFunc(BOX_REFRESH_TIMER_INTERVAL, on_timer, BOX_REFRESH_TIMER_ID);
      }
    }

    if(value == DASH_TIMER_ID){
      if(dashing)
        dashing = 0;
        dash();
    }
}

void on_display(){

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  int i;
  if(!invulnerable){
    for(i=0;i<5;i++){
      collision(i);
    }
  }

  for(i=0;i<5;i++){
    draw_wall(wall_x_curr[i],i);
    check_score(i);
  }

  teleport();
  if(dashing){
    dash();
  }
  draw_box(box_y_curr,box_x_curr,0 ,1,1,1);

  for(i=0;i<trail_count;i++){
    draw_box(trail_pos_y[i], trail_pos_x[i], trail_pos_z[i],
       rand() / ((float)RAND_MAX*0.5+0.5) * trail_colors[i],
       rand() / ((float)RAND_MAX*0.5+0.5) * trail_colors[i],
       rand() / ((float)RAND_MAX*0.5+0.5) * trail_colors[i]);
  }



  glFlush();
  glutSwapBuffers();
}

void check_score(int index){
  if(!pass[index]){
    if(wall_x_curr[index] + wall_width/2 < box_x_curr - box_size/2){
      pass[index] = 1;
      score++;
      printf("SCORE! (%d)\n", score);
    }
  }
}

void collision(int index){
  if(!alive[index])
    return;

  float wall_bot = wall_y_bot[index];
  float wall_top = wall_y_top[index];
  float wall_left = wall_x_curr[index] - wall_width/2;
  float wall_right = wall_x_curr[index] + wall_width/2;

  float box_bot = box_y_curr - box_size/2;
  float box_top = box_y_curr + box_size/2;
  float box_left = box_x_curr - box_size/2;
  float box_right = box_x_curr + box_size/2;

  if((box_top >= wall_bot && box_left <= wall_right && box_right >= wall_left) ||
  (box_bot <= wall_top && box_left <= wall_right && box_right >= wall_left)){
     exit(0);
     return;
  }
}

void draw_box(float y, float x, float z, float colorR, float colorG, float colorB){
  glBegin(GL_POLYGON);
    glColor3f( colorR, colorG, colorB );
    glVertex3f(  box_size/2 + x, -box_size/2 + y, z );
    glVertex3f(  box_size/2 + x,  box_size/2 + y, z );
    glVertex3f( -box_size/2 + x,  box_size/2 + y, z );
    glVertex3f( -box_size/2 + x, -box_size/2 + y, z );
  glEnd();
  glutPostRedisplay();
}

void summon_wall(int index){
  float r = rand() / (float)RAND_MAX * (2 - wall_gap) - 1 + wall_gap;
  wall_x_curr[index] = 1 + wall_width/2;
  wall_y_bot[index] = r;
  wall_y_top[index] = r - wall_gap;
  color1[index] = rand() / (float)RAND_MAX * 0.5 + 0.5;
  color2[index] = rand() / (float)RAND_MAX * 0.5 + 0.5;
  color3[index] = rand() / (float)RAND_MAX * 0.5 + 0.5;
  alive[index] = 1;
  pass[index] = 0;
}

void draw_wall(float x, int index){
  if(alive[index]){
    glBegin(GL_POLYGON);
      glColor3f( color1[index], color2[index], color3[index] );
      glVertex3f(  wall_width/2 + x, -1, -1 );
      glVertex3f(  wall_width/2 + x,  wall_y_top[index], 0.01 );
      glVertex3f( -wall_width/2 + x,  wall_y_top[index], 0.01 );
      glVertex3f( -wall_width/2 + x, -1, -1 );
    glEnd();

    glBegin(GL_POLYGON);
      glColor3f( color1[index], color2[index], color3[index] );
      glVertex3f(  wall_width/2 + x, 1, -1 );
      glVertex3f(  wall_width/2 + x,  wall_y_bot[index], -1 );
      glVertex3f( -wall_width/2 + x,  wall_y_bot[index], -1 );
      glVertex3f( -wall_width/2 + x, 1, -1 );
    glEnd();

    glutPostRedisplay();
  }
}

void summon_trail(){
  int i;
  int n = TRAIL_MAX;

  for(i=n-1; i>=1; i--){
    trail_pos_y[i] = trail_pos_y[i-1];
  }

  trail_pos_y[0] = box_y_curr;

  if(trail_count < TRAIL_MAX)
    trail_count++;
}

void teleport(){
  if(box_y_curr < -1){
    box_y_curr = 1;
  }
  if(box_y_curr > 1){
    box_y_curr = -1;
  }
}

void dash_start(){
  if(dashing == 0){
    glutTimerFunc(DASH_TIMER_INTERVAL, on_timer, DASH_TIMER_ID);
    // score -= 5; // TODO mana ne skor ili nesto slicno
    dashing = 1;
    dash();
  }
}

void dash(){
  if(dashing){
    invulnerable = 1;
    box_v_y = 0;
    wall_speed = 0.05;
  }

  else{
    invulnerable = 0;
    wall_speed = 0.02;
  }
}
