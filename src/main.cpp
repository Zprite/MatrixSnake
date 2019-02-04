#include <Arduino.h>
#include <stdio.h>
#include <global.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include "functions.h"
#include <Font_Data.h>
#include <SPI.h>


void setup(void){
	Serial.begin(9600);
	mx.begin();
	P.begin(MAX_ZONES);
	pinMode(buttonPin1,INPUT_PULLUP);
	pinMode(buttonPin2,INPUT_PULLUP);
	pinMode(buttonPin3,INPUT_PULLUP);
	pinMode(buttonPin4,INPUT_PULLUP);
	// Set up display
  P.setInvert(false);
  for (uint8_t i=0; i<MAX_ZONES; i++) // Set Zones for display
  {
    P.setZone(i, ZONE_SIZE*i, (ZONE_SIZE*(i+1))-1);
  }
	P.setIntensity(0,1);
	P.setIntensity(1,1);
}

void loop(void){
start_screen(buttonPin1);
	static bool started =0;
  if(started==0){
		start_screen(buttonPin1); // Proceed after button is pressed
		started=1;
	}
	score = 0;
	tail_length = 0;
	game_over = false;
	srand(millis());
	xPos = WIDTH / 2;
	yPos = HEIGHT / 2;
	dir = STOP;
	set_walls();
	spawn_fruit();
	print_board();
	do {
		get_input(); // Get keyboard input
    updatePos(); // Update x and y coordinates for head
		collision(); // Detect collision
		update_tailPos(); // Need to update tail position AFTER eating apple.
		print_board();
		delay(GAME_SPEED);  // Go to next loop/frame after delay
	} while (!game_over);
	//score_screen(); // Display score screen after a game over.

}
