#include <iostream>
#include <string>
#include "DotsBoard.h"
//#include "DotsController.h"
//#include "DotsPlayer.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <unistd.h> 
using namespace std;
// Physical base address of GPIO
const unsigned gpio_address = 0x400d0000;

// Length of memory-mapped IO window
const unsigned gpio_size = 0xff;
const int gpio_led1_offset = 0x12C; // Offset for LED1
const int gpio_led2_offset = 0x130; // Offset for LED2
const int gpio_led3_offset = 0x134; // Offset for LED3
const int gpio_led4_offset = 0x138; // Offset for LED4
const int gpio_led5_offset = 0x13C; // Offset for LED5
const int gpio_led6_offset = 0x140; // Offset for LED6
const int gpio_led7_offset = 0x144; // Offset for LED7
const int gpio_led8_offset = 0x148; // Offset for LED8

const int gpio_sw1_offset = 0x14C; // Offset for Switch 1
const int gpio_sw2_offset = 0x150; // Offset for Switch 2
const int gpio_sw3_offset = 0x154; // Offset for Switch 3
const int gpio_sw4_offset = 0x158; // Offset for Switch 4
const int gpio_sw5_offset = 0x15C; // Offset for Switch 5
const int gpio_sw6_offset = 0x160; // Offset for Switch 6
const int gpio_sw7_offset = 0x164; // Offset for Switch 7
const int gpio_sw8_offset = 0x168; // Offset for Switch 8

const int gpio_pbtnl_offset = 0x16C; // Offset for left push button
const int gpio_pbtnr_offset = 0x170; // Offset for right push button
const int gpio_pbtnu_offset = 0x174; // Offset for up push button
const int gpio_pbtnd_offset = 0x178; // Offset for down push button
const int gpio_pbtnc_offset = 0x17C; // Offset for center push button

class ZedBoard
{
	// Declare class variables
	char *pBase;
	int fd;

public:

	/*
	 * Constructor for ZedBoard class
	 */
	ZedBoard()
	{
		fd = open( "/dev/mem", O_RDWR);
		pBase = (char *) mmap(
				NULL,
				gpio_size,
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				fd,	
				gpio_address);

		//Check error
		if (pBase == MAP_FAILED)
		{
			cerr << "Mapping I/O memory failed - Did you run with 'sudo'?\n";
			exit(1); // Returns 1 to the operating system; 
		}

	}

	/*
	 * Destructor for ZedBoard class 
	 */
	~ZedBoard()
	{
		munmap(pBase, gpio_size);
		close(fd);
	}

	/**
	 * Write a 4-byte value at the specified general-purpose I/O location.
	 *
	 * @parem offset Offset where device is mapped.
	 * @param value Value to be written.
	 */
	void RegisterWrite(int offset, int value)
	{
		* (int *) (pBase + offset) = value;
	}

	/**
	 * Read a 4-byte value from the specified general-purpose I/O location.
	 *
	 * @param offset Offset where device is mapped.
	 * @return Value read.
	 */
	int RegisterRead(int offset)
	{
		return * (int *) (pBase + offset);
	}

	/**
	 * Show lower 8 bits of integer value on LEDs
	 *
	 * @param value Value to show on LEDs
	 */
	void SetLedNumber(int value)
	{

		RegisterWrite(gpio_led1_offset, value % 2);
		RegisterWrite(gpio_led2_offset, (value / 2) % 2);
		RegisterWrite(gpio_led3_offset, (value / 4) % 2);
		RegisterWrite(gpio_led4_offset, (value / 8) % 2);
		RegisterWrite(gpio_led5_offset, (value / 16) % 2);
		RegisterWrite(gpio_led6_offset, (value / 32) % 2);
		RegisterWrite(gpio_led7_offset, (value / 64) % 2);
		RegisterWrite(gpio_led8_offset, (value / 128) % 2);
	}
	
	/* 
	 * Find a numerical representation of the switches
	 * the switch corresponding to a specific LED would add the amount necessary to 
	 * turn its LED on if in the on position or 0 if it is off (as determined in SetLedNumber)
	 */
	int GetSwitchNumber()
	{
		int total = 0;
        	total += RegisterRead(gpio_sw1_offset);
        	total += (RegisterRead(gpio_sw2_offset) * 2);
        	total += (RegisterRead(gpio_sw3_offset) * 4);
        	total += (RegisterRead(gpio_sw4_offset) * 8);
        	total += (RegisterRead(gpio_sw5_offset) * 16);
        	total += (RegisterRead(gpio_sw6_offset) * 32);
        	total += (RegisterRead(gpio_sw7_offset) * 64);
        	total += (RegisterRead(gpio_sw8_offset) * 128);

		return total;

	}

	/**
	 * Checks the push buttons and returns an integer representing which one is pushed 
	 * (1 = left, 2 = up, 3 = right, 4 = down, 5 = center, 0 = none)
	 *
	 * @param last_value Last value returned from this function to main
	 */
	int PushButtonGet(int last_value)
	{
		if (RegisterRead(gpio_pbtnu_offset)){ //UP
			return 1;
		}
		if (RegisterRead(gpio_pbtnd_offset)) //DOWN 
		{
			return 2;
		}
        if (RegisterRead(gpio_pbtnl_offset)) //LEFT
        {
        	return 3;
        }
		if (RegisterRead(gpio_pbtnr_offset)) //RIGHT
		{
			return 4;
		}
		if (RegisterRead(gpio_pbtnc_offset)) //CENTER
		{	
			return 5;
		}

		return 0;
	}

};

int TakeTurn(string name, DotsBoard *gameBoard, ZedBoard *z, int rows, int columns)
{
	//create a string indicating whose turn it is
	string turn_string = "It's ";
	turn_string.append(name);
	turn_string.append("'s turn!");
	
	// tell the player how to make a selection
	string box_directions = "Please use the push buttons to select a box\n";
	box_directions.append("and hit the center button to confirm");
	
	// print out turn indicator
	cout << endl << turn_string << endl;
	
	int selected_row = 0;
	int selected_column = 0;
	
	// indicates whether or not a final selection has been made
	bool confirmed = false;
	
	// Place to hold the value of the pushbutton we pressed - need two for comparison purposes
	int push_value = 0;
	int new_push = 0;
	
	//print directions
	cout << box_directions << endl;

	//print the board so they have some idea what's going on
	gameBoard->PrintSelect(selected_row, selected_column);

	//Infinite loop for selecting a box
	while(!confirmed)
	{
		new_push = z->PushButtonGet(push_value);
		
		if (new_push != push_value)
		{
			switch(new_push)
			{
				case 1: // Up - go up a row
					if ((selected_row - 1) >= 0)
					{
						selected_row -= 1;
					}
					break;
				case 2: // Down - go down a row
					if((selected_row + 1) < rows) {
						selected_row += 1;
					}
					break;
				case 3: // Left - go one column to the left
					if ((selected_column - 1) >= 0)
					{
						selected_column -= 1;
					}
					break;
				case 4: // Right - go one column to the right
					if((selected_column + 1) < columns) {
						selected_column += 1;
					}
					break;
				case 5: // Center - user has confirmed selection
					confirmed = true;
			}
			
			//only print the board on a button press
			gameBoard->PrintSelect(selected_row, selected_column);
		}
		//Sleep for half a second then repeat
		usleep(.5 * 1000000);
	}
	
	// reset the confirmed flag to enter the next loop
	confirmed = false;
	
	//reset the button values for the new loop
	push_value = 0;
	new_push = 0;
	
	// indicate which edge has been selected
	// 0 is nonsense, 1 = left, 2 = up, 3 = right, 4 = down
	int edge = 0;
	
	// print directions
	cout << "Please choose an edge using the push buttons" << endl;
	cout << "hit the center button to confirm" << endl;

	//Infinite loop for selecting an edge based on the box
	while(!confirmed)
	{
		
		new_push = z->PushButtonGet(push_value);
		
		if (new_push != push_value)
		{
			switch(new_push)
			{
				case 1: // Up - edge = 2
					edge = 2;
					break;
				case 2: // Down - edge = 4
					edge = 4;
					break;
				case 3: // Left - edge = 1
					edge = 1;
					break;
				case 4: // Right - edge = 3
					edge = 3;
					break;
				case 5: // Center - user has confirmed selection
					//double check that an edge has been selected
					confirmed = (edge != 0);
			}
			
			// only print on button press	
			gameBoard->PrintSelectEdge(selected_row, selected_column, edge);

		}
		//Sleep for half a second then repeat
		usleep(.5 * 1000000);
	}
	
	
	//now that we actually have all of the data, tell the board the edge is claimed
	int claimed_boxes = gameBoard->ClaimEdge(selected_row, selected_column, edge, name);
	
	if (claimed_boxes < 0)
	{
		// edge was taken, try again
		cout << "That edge has already been taken, please select another one" << endl;
		claimed_boxes = TakeTurn(name, gameBoard, z, rows, columns);
	}
	if (claimed_boxes > 0 && (!gameBoard->GameOver())) {
		cout << name << " gets another turn because they claimed a box!" << endl; 
	}
	
	return claimed_boxes;	
}

int main ()
{
	// Number of players
	int num_players = 2;
	
	// declare player data
	string player_names[num_players];
	player_names[0] = "P1";
	player_names[1] = "P2";

	int player_scores[num_players];
	
	//initialize player data
	for (int i = 0; i < num_players; i++)
	{
		player_scores[i] = 0;
	}
	
	// Board size
	int board_columns = 3;
	int board_rows = 3;
	
	// Make game Board
	DotsBoard *board = new DotsBoard(board_rows, board_columns);
	
	//print initial state of the game board
	board->PrintBoard();
	
	
	// Zedboard instance to control the game
	ZedBoard *zed = new ZedBoard();
 	
 	// run the game	
 	int current = 0;
 	
 	while (!board->GameOver())
 	{
		int claimed = TakeTurn(player_names[current], board, zed, board_rows, board_columns);
 		if (claimed > 0)
 		{
 			player_scores[current] += claimed;
 		}
 		else {
 			// update whose turn it is
 			current = (current + 1) % num_players;	
		}	
 	}
	
	//print final state of board
	board->PrintBoard();

	//tell user the game is over
	cout << "The game is now over, all the boxes are taken!" << endl;
	
	// Find winner & print
	if (player_scores[0] == player_scores[1])
	{
		cout << player_names[0] << " and " << player_names[1] << " Tied!" << endl;
	}
	else if ( player_scores[0] > player_scores[1])
	{
		cout << player_names[0] << " won this game!" << endl;
	}
	else
	{
		cout << player_names[1] << " won this game!" << endl;
	}
	
	
	// Delete game board, controller, players, zedboard
	delete zed;
	delete board;
	
}
