/*
 * cars.c
 *
 *  Created on: Jun 1, 2020
 *      Author: wacha
 */

#include "cars.h"
#include "untility.h"
#include "Adafruit_SSD1351.h"

static int player_positionX;
static int player_positionY;

// When spawning player car, expect x = SCREEN_WIDTH/2, y = SCREEN_HEIGHT - 5 - 25
struct Car spawnCar(unsigned short tag, int x, int y, unsigned int color) {
    if (tag == 0) {
        player_positionX = x;
        player_positionY = y;
    }

    struct Car c = {tag, x, y, color};
    drawCar(c, 0);

    return c;
}

void drawCar(struct Car c, int erase) {
    int bodyx = c.x - CAR_WIDTH/2;
    int bodyy = c.y - CAR_HEIGHT/2;

    int tire_width  = 1;
    int tire_height = 2;

    int tire1x = bodyx - tire_width;
    int tire1y = bodyy + 2;

    int tire2x = bodyx + CAR_WIDTH;
    int tire2y = tire1y;

    int tire3x = tire1x;
    int tire3y = bodyy + CAR_HEIGHT - tire_height - 2;

    int tire4x = tire2x;
    int tire4y = tire3y;

    unsigned int car_color = c.color;
    unsigned int tire_color = BLACK;

    if (erase) {
        car_color  = ROAD_COLOR;
        tire_color = ROAD_COLOR;
    }

    fillRect(tire1x, tire1y, tire_width, tire_height, tire_color);
    fillRect(tire2x, tire2y, tire_width, tire_height, tire_color);
    fillRect(tire3x, tire3y, tire_width, tire_height, tire_color);
    fillRect(tire4x, tire4y, tire_width, tire_height, tire_color);

    fillRect(bodyx, bodyy, CAR_WIDTH, CAR_HEIGHT, car_color);
}

// Destroy car object by replacing it with an empty struct
void destroyCar(struct Car* c) {
    *c = (const struct Car) {0};
}

static unsigned short collisionCheck(int oldy, int newy, int spawnDir) {
    if (spawnDir == FRONT) {
        if (oldy - CAR_HEIGHT/2 <= player_positionY - CAR_HEIGHT/2) {
            if (oldy + CAR_HEIGHT/2 >= player_positionY - CAR_HEIGHT/2
             || newy + CAR_HEIGHT/2 >= player_positionY - CAR_HEIGHT/2) {
                game_over = 1;
                printf("FRONT COLLISION, GAME OVER MAN!!!\n");
            }
        } else if (oldy - CAR_HEIGHT/2 <= player_positionY + CAR_HEIGHT/2) {
            game_over = 1;
            printf("FRONT TRAPPED, GAME OVER MAN!!!\n");
        }
    } else if (spawnDir == BACK) {
        if (oldy + CAR_HEIGHT/2 >= player_positionY + CAR_HEIGHT/2) {
            if (oldy - CAR_HEIGHT/2 <= player_positionY + CAR_HEIGHT/2
             || newy - CAR_HEIGHT/2 <= player_positionY + CAR_HEIGHT/2) {
                game_over = 1;
                printf("BACK COLLISION, GAME OVER MAN!!!\n");
            }
        } else if (oldy + CAR_HEIGHT/2 >= player_positionY - CAR_HEIGHT/2) {
            game_over = 1;
            printf("BACK TRAPPED, GAME OVER MAN!!!\n");
        }
    } else {
        printf ("Error: Wrong spawning direction!!!\n");
    }

    return game_over;
}

// Given struct Car array cars,
// Can call function moveCar(cars + i), where i is the position of the target car
void moveCar(int newx, int newy, struct Car* c) {
    // If receive double -1, this is a dull update
    if (newx != -1 || newy != -1) drawCar(*c, 1);     // Erase car at current position

    // Player cannot go off screen
    if (c->tag == 0) {
        // Update global variable as player car moves
        if (newx != -1) player_positionX = newx;
        if (newy != -1) player_positionY = newy;

        // Update car's position
        // If newx or y == -1, that indicates we don't update car in that axis
        if (newx > 5 && newx < SCREEN_WIDTH - 5) {
            c->x = newx;
        }
        if (newy > 5 && newy < SCREEN_HEIGHT - 5) {
            c->y = newy;
        }
    // But other cars can
    } else {
        // If car is at the same lane as player:
        //      If player box is between car's old head and car's new head, prompt game_over
        if (c->x == player_positionX) {
            if (player_positionX == LANE1) collisionCheck(c->y, newy, spawnDir1);
            else if (player_positionX == LANE2) collisionCheck(c->y, newy, spawnDir2);
            else if (player_positionX == LANE3) collisionCheck(c->y, newy, spawnDir3);
            else printf("Error: Player at wrong lane!!!\n");
        }

        if (game_over) {
            return;
        }

        if (newy >= FRONT && newy <= BACK) {
            c->y = newy;
        // If other cars goes out of bound, trash it
        } else {
            destroyCar(c);
            return;
        }
    }

    // Check if current car is occupying any spawn point
    // LANE 1
    if (c->x == LANE1) {
        if (spawnDir1 == FRONT && c->y < CAR_HEIGHT * 2) {
            SP1 = 0;
        } else if (spawnDir1 == BACK && c->y > SCREEN_HEIGHT - CAR_HEIGHT * 2) {
            SP1 = 0;
        }
    //LANE 2
    } else if (c->x == LANE2) {
        if (spawnDir2 == FRONT && c->y < CAR_HEIGHT * 2) {
            SP2 = 0;
        } else if (spawnDir2 == BACK && c->y > SCREEN_HEIGHT - CAR_HEIGHT * 2) {
            SP2 = 0;
        }
    // LANE 3
    } else if (c->x == LANE3) {
        if (spawnDir3 == FRONT && c->y < CAR_HEIGHT * 2) {
            SP3 = 0;
        } else if (spawnDir3 == BACK && c->y > SCREEN_HEIGHT - CAR_HEIGHT * 2) {
            SP3 = 0;
        }
    // Error
    } else {
        printf("Error: Wrong Lane\n");
    }

    if (newx != -1 || newy != -1) drawCar(*c, 0);
}
