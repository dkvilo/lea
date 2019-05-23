/*
* Lea Script Reader 1.0
* Author: David Kviloria
*/

#include "../includes/lea.h"

void drawEdges(int initCall) {
  for (unsigned i = 1; i < Editor.sr; ++i) {
    write(STDOUT_FILENO, TILDE, strlen(TILDE));
    if (initCall && Editor.sr / 2 < ws.ws_row) {
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
  write(STDOUT_FILENO, "\x1b[C", 3); // MOVE CURSOR FORWARD
}

void setCursorPositionBackward() {
  write(STDOUT_FILENO, "\x1b[D", 3); // MOVE CURSOR BACKWARD
}

int Lea_on_exit() {
  setCursorPositionHome();
  setCursorPositionBackward();
  setCursorPositionBackward();
  clearScreen();
  return 1;
}

int Lea_sleep (int millisec) {
  return (usleep ((useconds_t) millisec *1000));
}


void Lea_set_terminal_read_only() {

  struct termios t;

  tcgetattr(fileno(stdin), &t);
  t.c_lflag |= ~ECHO;

  Editor.state = t;
  
  if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
    perror("Unable to set terminal to read-only");
    exit(-1);
  }
  
}


void Lea_line_rules() {
  printf("\n\n");
  setCursorPositionForward();
  setCursorPositionForward();
  Editor.sr++;
}

void Lea_insert_char_rules(int ch) {
  putchar(ch);
}

void Lea_read (int delay, const char *string) {

  Lea_set_terminal_read_only();
  
  int position = 0;

  do {

    if (string[position] != ' ') Lea_sleep (delay);

    (string[position] == '\n') 
      ? Lea_line_rules()
      : Lea_insert_char_rules(string[position]);

    fflush (stdout);

  } while (string[position++] != '\0');

}

void Lea_init() {
  
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  
  Editor.sr = ws.ws_row;
  Editor.sc = ws.ws_col;
  
  clearScreen();
  // drawEdges(0);

  setCursorPositionHome();
  setCursorPositionForward();
  setCursorPositionForward();
}

void Lea_read_from_file(const char *path, unsigned short speed) {

  FILE *fp = fopen(path, "r");

  Editor.buffer = NULL;
  Editor.size = 0;
  Editor.length = 0;

  if (!fp) {
    perror(" - Error While Processing File");
    exit(-1);
  }
  
  Lea_init();

  printf("\n  File: %s%s%s\n", Color.Green, path, Color.Default);
  printf("\n  Speed: %s%d%s ms\n", Color.Green, Editor.speed, Color.Default);

  while ((Editor.length = getline(&Editor.buffer, &Editor.size, fp)) > 0) {
    Lea_read(speed, Editor.buffer);
  };

  fclose(fp);
  
  free(Editor.buffer), Editor.buffer = NULL;

  Editor.length = 0, Editor.size = 0;
}

void Lea_trigger_core(unsigned int speed, const char *path) {
  
  if (!path) {
    perror(" - No Input File");
    exit(-1);
  }
  
  Lea_read_from_file(path, speed);
}

void Lea_help() {
  perror(" - Usage: lea --file file.lea");
  exit(-1);
}

int main(int argc, char *argv[]) {

  unsigned int speed = 100;
  char *p;

  if (argc <= 1) Lea_help();

  if (argv[3] != NULL) {
    if (strcmp(argv[3], "--speed") == 0 
    || strcmp(argv[3], "-s") == 0) {
      if (!argv[4]) {
        perror(" - Error: Missing Speed Value");
        exit(-1);
      }
      speed = strtol(argv[4], &p, 10);
      Editor.speed = speed;
    }
  }

  if (strcmp(argv[1], "--file") == 0 
  || strcmp(argv[1], "-f") == 0) {
    Lea_trigger_core(Editor.speed, argv[2]);
  }

  Lea_on_exit();
	return 0;
}


