#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

//==============================================================================
// FUNCTION DECLARATIONS
//==============================================================================

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


//==============================================================================
// TIMERS
//==============================================================================

#define BOX_REFRESH_TIMER_ID 0
#define BOX_REFRESH_TIMER_INTERVAL 10

#define WALL_SUMMON_TIMER_ID 1
#define WALL_SUMMON_INTERVAL 1000

#define TRAIL_TIMER_ID 2
#define TRAIL_TIMER_INTERVAL 60

#define DASH_TIMER_ID 3
#define DASH_TIMER_INTERVAL 350


//==============================================================================
// GLOBAL VARIABLES
//==============================================================================

//------------------------------------------------------------------------------
/* WORLD */
typedef struct {
  float gravity;
  float jump;
  int score;
  int animation_ongoing;
} World;

static World world = {
  .0015, // world.gravity
  .035,  // world.jump
  0,     // world.score
  0      // world.animation_ongoing
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/* PLAYER */
typedef struct {
  float x_curr;
  float y_curr;
  float v_y; // velocity
  float size;
  int invulnerable;
  int dashing;
} Player;

static Player player = {
  -.5, //player.x_curr
  0,   //player.y_curr
  0,   //player.v_y
  .1,  //player.size
  0,   //player.invulnerable
  0    //player.dashing
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/* TRAIL */
#define TRAIL_MAX 8
static float trail_x_move = .05;
static float trail_color_alpha = 1;
static int trail_count = -5;

typedef struct {
  float pos_x;
  float pos_y;
  float pos_z;
  float colors;
} Trail;

static Trail trails[TRAIL_MAX];
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/* WALLS */
#define WALL_COUNT 3
const static float wall_width = .3;
static float wall_speed = 0.02;
static float wall_gap = 1.;
static float wall_gap_min = .6;
static int wall_summon_index = 0;

typedef struct {
  float x_curr;
  float y_bot;
  float y_top;
  int alive;
  int pass;
  float colorR;
  float colorG;
  float colorB;
} Wall;

static Wall walls[WALL_COUNT];
//------------------------------------------------------------------------------


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


  trails[0].pos_x = player.x_curr;
  trails[0].pos_y = 0;
  trails[0].pos_z = .1;
  trails[0].colors = trail_color_alpha;

  int i;
  for(i=1; i<TRAIL_MAX; i++){
    trails[i].pos_x = trails[i-1].pos_x - trail_x_move;
    trails[i].pos_y = 0;
    trails[i].pos_z = trails[i-1].pos_z + .1; // FIXME magicna konstanta
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
            glutTimerFunc(BOX_REFRESH_TIMER_INTERVAL, on_timer, BOX_REFRESH_TIMER_ID);
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
        dash_start();
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

    if(value == BOX_REFRESH_TIMER_ID){
      player.y_curr += player.v_y;
      if(player.v_y > -.05){
        player.v_y -= world.gravity;
      }

      int i;
      for(i=0;i<WALL_COUNT;i++){
        walls[i].x_curr -= wall_speed;
      }

      glutPostRedisplay();

      if (world.animation_ongoing) {
          glutTimerFunc(BOX_REFRESH_TIMER_INTERVAL, on_timer, BOX_REFRESH_TIMER_ID);
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
      collision(i);
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

  draw_box(player.y_curr,player.x_curr,0 ,1,1,1); // FIXME jos konstanti

  for(i=0;i<trail_count;i++){
    draw_box(trails[i].pos_y, trails[i].pos_x, trails[i].pos_z,
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors, // FIXME sve ovo dole
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors,
       rand() / ((float)RAND_MAX*0.5+0.5) * trails[i].colors);
  }

  glFlush();
  glutSwapBuffers();
}

void check_score(int index){
  if(!walls[index].pass){
    if(walls[index].x_curr + wall_width/2 < player.x_curr - player.size/2){
      walls[index].pass = 1;
      world.score++;
      printf("SCORE! (%d)\n", world.score);
    }
  }
}

void collision(int index){
  if(!walls[index].alive)
    return;

  float wall_bot = walls[index].y_bot;
  float wall_top = walls[index].y_top;
  float wall_left = walls[index].x_curr - wall_width/2;
  float wall_right = walls[index].x_curr + wall_width/2;

  float box_bot = player.y_curr - player.size/2;
  float box_top = player.y_curr + player.size/2;
  float box_left = player.x_curr - player.size/2;
  float box_right = player.x_curr + player.size/2;

  if((box_top >= wall_bot && box_left <= wall_right && box_right >= wall_left) ||
  (box_bot <= wall_top && box_left <= wall_right && box_right >= wall_left)){
     exit(0);
     return;
  }
}

void draw_box(float y, float x, float z, float colorR, float colorG, float colorB){
  glBegin(GL_POLYGON);
    glColor3f( colorR, colorG, colorB );
    glVertex3f(  player.size/2 + x, -player.size/2 + y, z );
    glVertex3f(  player.size/2 + x,  player.size/2 + y, z );
    glVertex3f( -player.size/2 + x,  player.size/2 + y, z );
    glVertex3f( -player.size/2 + x, -player.size/2 + y, z );
  glEnd();
  glutPostRedisplay();
}

void summon_wall(int index){
  float r = rand() / (float)RAND_MAX * (2 - wall_gap) - 1 + wall_gap;
  walls[index].x_curr = 1 + wall_width/2;
  walls[index].y_bot = r;
  walls[index].y_top = r - wall_gap;
  walls[index].colorR = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorG = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].colorB = rand() / (float)RAND_MAX * 0.5 + 0.5;
  walls[index].alive = 1;
  walls[index].pass = 0;
}

void draw_wall(float x, int index){
  if(walls[index].alive){
    glBegin(GL_POLYGON);
      glColor3f( walls[index].colorR, walls[index].colorG, walls[index].colorB );
      glVertex3f(  wall_width/2 + x, -1, -1 );
      glVertex3f(  wall_width/2 + x,  walls[index].y_top, 0.01 );
      glVertex3f( -wall_width/2 + x,  walls[index].y_top, 0.01 );
      glVertex3f( -wall_width/2 + x, -1, -1 );
    glEnd();

    glBegin(GL_POLYGON);
      glColor3f( walls[index].colorR, walls[index].colorG, walls[index].colorB );
      glVertex3f(  wall_width/2 + x, 1, -1 );
      glVertex3f(  wall_width/2 + x,  walls[index].y_bot, -1 );
      glVertex3f( -wall_width/2 + x,  walls[index].y_bot, -1 );
      glVertex3f( -wall_width/2 + x, 1, -1 );
    glEnd();

    glutPostRedisplay();
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

void teleport(){
  if(player.y_curr < -1){
    player.y_curr = 1;
  }

  if(player.y_curr > 1){
    player.y_curr = -1;
  }
}

void dash_start(){
  if(player.dashing == 0){
    glutTimerFunc(DASH_TIMER_INTERVAL, on_timer, DASH_TIMER_ID);
    // score -= 5; // TODO mana ne skor ili nesto slicno
    player.dashing = 1;
    dash();
  }
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
