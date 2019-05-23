#pragma once

#ifndef _LEA_H_
#define _LEA_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <termios.h>

#include "colors.h"

#define LEA "  [Lea Script Reader]\r\n"
#define TILDE "~\r\n"
#define VERSION 0.1

struct LeaConfig {
  int sr, sc;
  char *buffer;
  ssize_t length;
  size_t size;
  unsigned int speed;
  struct termios state;
};

typedef struct ColorsCollection {
  const char *Red     = KVILO_RED;
  const char *Blue    = KVILO_BLUE;
  const char *Yellow  = KVILO_YELLOW;
  const char *Green   = KVILO_GREEN;
  const char *Default = KVILO_RESET;
} Colors;

struct LeaConfig Editor;

Colors Color;

struct winsize ws;

void drawEdges(int initCall);
void appendEdge();
void clearScreen();
void Lea_set_terminal_read_only();

void setCursorPositionBackward();
void setCursorPositionForward();
void moveCursorPositionUp();
void moveCursorPositionDown();
void setCursorPositionHome();

void Lea_line_rules();

void Lea_insert_char_rules(int ch);

void Lea_help();

int Lea_on_exit();

int Lea_sleep();

void Lea_read (int delay, const char *string);

void Lea_init();

void Lea_read_from_file(const char *path, unsigned short speed);

void Lea_trigger_core(unsigned int speed, const char *path);

#endif // _LEA_H_
