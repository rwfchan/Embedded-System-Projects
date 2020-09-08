/*
 * untility.h
 *
 *  Created on: Jun 1, 2020
 *      Author: wacha
 */

#ifndef UNTILITY_H_
#define UNTILITY_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define GREEN           0x07E0
#define CYAN            0x07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define ROAD_COLOR 0x0005

#define PLAYER_FRAME     5
#define OBJECT_FRAME     30

#define FRONT -CAR_HEIGHT + 1
#define BACK  SCREEN_HEIGHT - 1

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

#define LANE1            SCREEN_WIDTH/8         // Speed is 80
#define LANE2            3 * SCREEN_WIDTH/8     // Speed is 60
#define LANE3            SCREEN_WIDTH * 5/8     // Speed is 70

#define LANE1_SPEED      90
#define LANE2_SPEED      40
#define LANE3_SPEED      70

#define BORDERLINE       LANE1 * 6
#define NEWLINE          10
#define SPEED_POSITION   SCREEN_WIDTH/4 + NEWLINE
#define SCORE_POSITION   SCREEN_WIDTH/4 + NEWLINE * 6

short SP1;
short SP2;
short SP3;

int spawnDir1;
int spawnDir2;
int spawnDir3;

short game_over;   // Collision is detected via player perspective (player crash onto others only)

#endif /* UNTILITY_H_ */
