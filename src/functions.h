#include <Arduino.h>
#include <Font_Data.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

void button_delay(){

}

void display(char* top, char* bottom){
  P.setFont(0, nullptr);
  P.setFont(1, nullptr);
  static uint8_t  curZone = 0;
  P.displayZoneText (1,top, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT ,PA_NO_EFFECT);
  P.displayZoneText (0,bottom, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT ,PA_NO_EFFECT);
  while (!P.getZoneStatus (curZone))
    P.displayAnimate(); // Refresh display
}

void display_text(char *input_string){        
  P.displayClear(1);
  P.displayClear(0);
  P.setFont(0, BigFontLower);
  P.setFont(1, BigFontUpper);
  P.displayZoneText(1, input_string, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  P.displayZoneText(0, input_string, PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  static uint8_t  curZone = 1;
  while (!P.getZoneStatus (curZone))
    P.displayAnimate(); // Refresh display
}

void start_screen(int button){
    while(digitalRead(button) != 0){
		display("RDY","P1");
    } 
    return;
}

void spawn_fruit()
{
	bool validSpawn = false;
	do {
		fruitX = rand() % WIDTH;
		fruitY = rand() % HEIGHT;
		// Fruit will only be spawned in empty spaces (' ').
		if (board[fruitY][fruitX] == ' ') {
			validSpawn = true;
		}
	} while (validSpawn== false);
}

void get_input()
{	
	if(digitalRead(buttonPin1)!= 1)
		dir = UP;
	else if(digitalRead(buttonPin4)!=1)
		dir = LEFT;
	else if(digitalRead(buttonPin3)!=1)
		dir = DOWN;
	else if(digitalRead(buttonPin2)!=1)
		dir = RIGHT;
}

void update_tailPos()
{
	//Tail positions are stored in the tailX and tailY arrays. 
	for (int i = tail_length; i > 0; i--)
	{
		/*Set tail X and Y position equal to the tail element before it.
		In void updatePos(), tailX[0] and tailY[0] is set to the previous head position before updating it. 
		Therefore, the first tail element will move to previous head postion.*/
		tailX[i] = tailX[i - 1]; 
		tailY[i] = tailY[i - 1];
	}
}

void updatePos()
{
	tailX[0] = xPos; // Set previous X and y values for head as start point for 1st tail element.
	tailY[0] = yPos;
	if (dir == UP)
        yPos--;
	else if (dir == LEFT)
        xPos--; 
	else if (dir == DOWN) 
        yPos++; 
	else if (dir == RIGHT)
        xPos++; 
}

void collision()
{
	if (xPos <= 1 || xPos >= WIDTH- 2 || yPos <= 1 || yPos >= HEIGHT-2) // If head position = border of map.
	{
		game_over = true;
	}
	else if (xPos == fruitX && yPos == fruitY) // When colliding with fruit.
	{
		score += 20;
		spawn_fruit();
		tail_length++;
	}
	else {
		for (int i = tail_length; i > 0; i--)  // When colliding with tail
		{
			if (xPos == tailX[i] && yPos == tailY[i])
			{
				game_over = true;
			}
		}
	}
}

void set_walls()  // Set map borders to be walls ('#'). Fill in the rest with empty spaces (' ').
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH -1)
			{
				board[i][j] = '#';
			}
			// Fill inn board with empty space
			else {
				board[i][j] = ' ';
			}
		}
	}

}

void print_board()
{
	for (int i = 0; i < HEIGHT; i++) // RESET BOARD 
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (board[i][j] != '#') 
			{
				board[i][j] = ' '; 
			}
		}
	}

	board[yPos][xPos] = SnakeMark; // Set snake head
	for (int i = tail_length; i > 0; i--) // Set snake tail
	{
		board[tailY[i]][tailX[i]] = SnakeMark;	
	}
	board[fruitY][fruitX] = FruitMark; // Set fruit

	mx.clear(); // Clear display
	for (int i = 0; i < HEIGHT; i++) // Print to LED matrix
	{
		for (int j = 0; j < WIDTH; j++)
		{	
			if(board[i][j]==' ')
				mx.setPoint(i,j,0); // Set LED to off on whitespaces
			else
				mx.setPoint(i,j,1); // Set LED on for every other character (Snake, walls, fruits)
		}
	}
} 