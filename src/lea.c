/*
* Lea Script Reader 1.0
* Author: David Kviloria
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

#define LINE_BUFFER_SIZE 128

#define LEA "[Lea Script Reader]\r\n"
#define TILDE "~\r\n"

struct LeaConfig {
  int sr, sc;
  char buffer[LINE_BUFFER_SIZE];
  int key;
};

struct LeaConfig Editor;
struct winsize ws;

void drawEdges(int initCall) {
  for (unsigned i = 1; i < Editor.sr; ++i) {
    write(STDOUT_FILENO, TILDE, strlen(TILDE));
    if (initCall) {
      if (i == (Editor.sr - 1)) {
        write(STDOUT_FILENO, LEA, strlen(LEA) -1);
      }
    }
  }
}

void appendEdge() {
  write(STDOUT_FILENO, TILDE, strlen(TILDE));
}

void clearScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);                                     
}

void setCursorPositionHome() {
  write(STDOUT_FILENO, "\x1b[H", 3); // CURSOR HOME POSITION (0, 0)
}

void moveCursorPositionDown() {
  write(STDOUT_FILENO, "\x1b[B", 4); // MOVE CURSOR DOWN
}

void moveCursorPositionUp() {
  write(STDOUT_FILENO, "\x1b[A", 4); // MOVE CURSOR UP
}

void setCursorPositionForward() {
  write(STDOUT_FILENO, "\x1b[C", 3); // MOVE CURSOR DOWN
}

void setCursorPositionBackward() {
  write(STDOUT_FILENO, "\x1b[D", 3); // MOVE CURSOR DOWN
}

int Lea_on_exit() {
  clearScreen();
  setCursorPositionHome();
  setCursorPositionBackward();
  setCursorPositionBackward();
  return 1;
}

int Lea_sleep (int millisec) {
  return (usleep ((useconds_t) millisec *1000));
}

void Lea_read (int delay, const char *string) {
  
  int position = 0;

  do{
    Lea_sleep (delay);
    putchar (string[position]);
    fflush (stdout);
  } while (string[position++] != '\0');

}

void Lea_init() {
  
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  
  Editor.sr = ws.ws_row;
  Editor.sc = ws.ws_col;
  
  clearScreen();
  drawEdges(1);

  setCursorPositionHome();
  setCursorPositionForward();
  setCursorPositionForward();
}

void Lea_read_from_file(const char *path, unsigned short speed) {

  FILE *fp = fopen(path, "r");

  char *linbuf = NULL;
  size_t siz = 0;
  ssize_t linlen = 0;


  if (!fp) {
    perror(" - Error While Processing File");
    exit(-1);
  }
  
  Lea_init();

  printf("Source: %s\n", path);
  
  while ((linlen = getline(&linbuf, &siz, fp)) > 0) {
    Lea_read(speed, linbuf);  
  };

  fclose(fp);
  
  free(linbuf), linbuf = NULL;

  linlen = 0, siz = 0;
}

void Lea_trigger_core(unsigned short speed, const char *path) {
  
  if (!path) {
    perror(" - No Input File");
    exit(-1);
  }

  if (!speed) speed = 300;
  
  Lea_read_from_file(path, speed);
}

int main(int argc, char *argv[]){

  unsigned speed = 100;

  for (unsigned short i = 1; i <= argc; ++i) {

    if (strcmp(argv[1], "--file") == 0 
    || strcmp(argv[1], "-f") == 0) {
      Lea_trigger_core(speed, argv[2]);
      break;
    }

  }

  (void) Lea_on_exit();

	return 0;
}


