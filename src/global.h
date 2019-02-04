#include <Arduino.h>
#include <stdio.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <Font_Data.h>
#include <SPI.h>

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

#define buttonPin1 2
#define buttonPin2 3
#define buttonPin3 4
#define buttonPin4 5

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define MAX_ZONES   2
#define ZONE_SIZE (MAX_DEVICES/MAX_ZONES)
#define SCROLL_SPEED 30
#define SPEED_TIME  20
#define PAUSE_TIME  0 

#define WIDTH 16
#define HEIGHT 8

#define GAME_SPEED 200
#define SnakeMark 254 // ASCII square
#define FruitMark 'O'
enum directions {STOP=0, UP, LEFT , DOWN, RIGHT};
directions dir;

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
char board[HEIGHT][WIDTH];
bool game_over;
bool exit_game;
int score;
int xPos;
int yPos;
int tailX[(HEIGHT - 1)*(WIDTH - 1)]; // Tail need not be longer than the area of the map.
int tailY[(HEIGHT - 1)*(WIDTH - 1)];
int tail_length;
int fruitX;
int fruitY;
