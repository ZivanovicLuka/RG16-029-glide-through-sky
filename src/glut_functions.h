#ifndef GLUT_FUCNTIONS_H
  #define GLUT_FUCNTIONS_H

  void RenderString(float x, float y, void *font, const char* string, float r, float g, float b);
  static void on_timer(int value);
  static void on_display(void);
  static void on_keyboard(unsigned char key, int x, int y);
  static void on_reshape(int width, int height);
  static void onDisplay(void);
  static void updateDeltaTime(void);
  int dt;
  static void fps(int print);

#endif
