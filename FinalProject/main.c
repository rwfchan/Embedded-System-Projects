//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
// Application Name     - SPI Demo
// Application Overview - The demo application focuses on showing the required 
//                        initialization sequence to enable the CC3200 SPI 
//                        module in full duplex 4-wire master and slave mode(s).
//
//*****************************************************************************


//*****************************************************************************
//
//! \addtogroup SPI_Demo
//! @{
//
//*****************************************************************************

// Standard includes

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "spi.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"

// Common interface includes
#include "uart_if.h"
#include "pin_mux_config.h"
#include "i2c_if.h"

#include "Adafruit_SSD1351.h"
#include "glcdfont.h"

// Costume classes
#include "cars.h"
#include "untility.h"

#define APPLICATION_VERSION     "1.4.0"
//*****************************************************************************
//
// Application Master/Slave mode selector macro
//
//*****************************************************************************

#define SPI_IF_BIT_RATE  100000
#define TR_BUFF_SIZE     100

#define MASTER_MSG       "This is CC3200 SPI Master Application\n\r"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
static unsigned char g_ucTxBuff[TR_BUFF_SIZE];
static unsigned char g_ucRxBuff[TR_BUFF_SIZE];
static unsigned char ucTxBuffNdx;
static unsigned char ucRxBuffNdx;

static struct Car cars[10];

static unsigned int timer = 0;
static int speed = 50;  // speed => 0 -> 100
static int score = 0;   // Decrement when player is at same lane; Increment when player switches lane

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
  //
  // Set vector table base
  //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

// Indicate if car slot is empty
unsigned short isEmpty(unsigned short index) {
    if (cars[index].color == 0) return 1;
    else return 0;
}

// Searching through the array to find empty slot according to indicated LANE
unsigned short searchLane(unsigned short lane) {
    // If lane == 1, search through 1 -> 3
    if (lane == 1) {
        unsigned short i;
        for (i = 1; i <= 3; i++) {
            if (isEmpty(i)) {
                return i;
            }
        }
        return 99;
    }

    // If lane == 2, search through 4 -> 6
    if (lane == 2) {
        unsigned short i;
        for (i = 4; i <= 6; i++) {
            if (isEmpty(i)) {
                return i;
            }
        }
        return 99;
    }

    // If lane == 3, search through 7 -> 9
    if (lane == 3) {
        unsigned short i;
        for (i = 7; i <= 9; i++) {
            if (isEmpty(i)) {
                return i;
            }
        }
        return 99;
    }

    printf("No valid lane. Error\n");
    return 99;
}

void LaneSpawn(unsigned short lane) {
    int random = rand();
    if (random % 30 == 0) {
        unsigned short tag = searchLane(lane);
        if (tag == 99) return;
        if (lane == 1) {
            cars[tag] = spawnCar(tag, LANE1, spawnDir1, GREEN);
            return;
        }
        if (lane == 2) {
            cars[tag] = spawnCar(tag, LANE2, spawnDir2, GREEN);
            return;
        }
        if (lane == 3) {
            cars[tag] = spawnCar(tag, LANE3, spawnDir3, GREEN);
            return;
        } // spawnDir
    }
}

static void updateSpeed(void) {
    unsigned char board_string[5];

    if (speed % 10 == 0) {
        drawChar(BORDERLINE + 5, SPEED_POSITION, ' ', WHITE, ROAD_COLOR, 1);
        drawChar(BORDERLINE + 10, SPEED_POSITION, ' ', WHITE, ROAD_COLOR, 1);
        setCursor(BORDERLINE + 5, SPEED_POSITION);
        sprintf(board_string, "%d", speed);
        Outstr(board_string);
    }
}

static void updateScore(void) {
    unsigned char board_string[5];

    if (score % 100 == 0) {
        short i;
        for (i = 1; i < 5; i++) {
            drawChar(BORDERLINE + i * 5, SCORE_POSITION, ' ', WHITE, ROAD_COLOR, 1);
        }
        setCursor(BORDERLINE + 5, SCORE_POSITION);
        sprintf(board_string, "%d", score);
        Outstr(board_string);
    }
}

// Draw the start up screen
static void drawInit(void) {
    // Printing game screen
    fillScreen(ROAD_COLOR);
    drawDottedLine(LANE1 * 2, 0, LANE1 * 2, SCREEN_HEIGHT, WHITE);
    drawDottedLine(LANE1 * 4, 0, LANE1 * 4, SCREEN_HEIGHT, WHITE);
    drawLine(BORDERLINE, 0, BORDERLINE, SCREEN_HEIGHT, BLACK);

    //
    // Printing status screen
    //
    unsigned char board_string[5];

    // Printing player speed
    setCursor(BORDERLINE, SCREEN_WIDTH/4);
    Outstr("Speed\0");
    updateSpeed();

    printf("Finished with board init\n");

    // Printing player Score
    setCursor(BORDERLINE, SCREEN_WIDTH/4 + NEWLINE * 5);
    Outstr("Score\0");
    updateScore();
}

static void printGameOver(void) {
    setTextSize(3);
    setTextColor(YELLOW);
    setCursor(0, SCREEN_WIDTH/5);
    Outstr("GAME\0");
    setCursor(SCREEN_WIDTH/3 + 15, SCREEN_WIDTH/3 + 5);
    Outstr("OVER\0");
}

//*****************************************************************************
//
//! Main function for spi demo application
//!
//! \param none
//!
//! \return None.
//
//*****************************************************************************
void main()
{
    srand(1);
    BoardInit();
    PinMuxConfig();
    I2C_IF_Open(I2C_MASTER_MODE_FST);
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
    MAP_SPIReset(GSPI_BASE);
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVEHIGH |
                     SPI_WL_8));
    MAP_SPIEnable(GSPI_BASE);
    Adafruit_Init();

    drawInit();
    int inity = SCREEN_HEIGHT - 5 - CAR_HEIGHT/2;
    cars[0] = spawnCar(0, LANE2, inity, RED);

    int accelerating = 0;
    int decelerating = 0;

    spawnDir1 = FRONT;
    spawnDir2 = FRONT;
    spawnDir3 = FRONT;

    game_over = 0;

    while(!game_over) {
        //
        // Reading data
        //

        // Storage for output
        unsigned char aucRdDataBufx[256];

        // Target register to read from
        unsigned char ucRegOffsetx = 0x3;

        // Read data from target, and store informations
        I2C_IF_Write(24, &ucRegOffsetx,1,0);
        I2C_IF_Read(24, &aucRdDataBufx[0], 1);

        //
        // Move car based on detected input
        //
        if (timer % PLAYER_FRAME == 0) {
            // Reset Spawn point occupation
            SP1 = 1;
            SP2 = 1;
            SP3 = 1;

            // Accelerometer
            if (200 > aucRdDataBufx[0] && aucRdDataBufx[0] > 190) {
                if (cars[0].x == LANE2) moveCar(LANE1, -1, cars + 0);
                if (cars[0].x == LANE3) moveCar(LANE2, -1, cars + 0);
                score += 30;
            }
            if (55 < aucRdDataBufx[0] && aucRdDataBufx[0] < 62) {
                if (cars[0].x == LANE2) moveCar(LANE3, -1, cars + 0);
                if (cars[0].x == LANE1) moveCar(LANE2, -1, cars + 0);
                score += 30;
            }

            // SW3 and 2
            // SW3 --> Gas
            if (GPIOPinRead(GPIOA1_BASE, 0x20)) accelerating = 1;
            // SW2 --> Brake
            if (GPIOPinRead(GPIOA2_BASE, 0x40)) decelerating = 1;

            // If button input detected
            if (accelerating || decelerating) {
                // moveCar based on input
                if (accelerating) {
                    moveCar(-1, cars[0].y - 2, cars + 0);
                    speed++;
                }
                if (decelerating) {
                    moveCar(-1, cars[0].y + 2, cars + 0);
                    speed--;
                }
                updateSpeed();
                accelerating = 0;
                decelerating = 0;
            }
            score--;
            updateScore();
            moveCar(-1, -1, cars + 0);
        }



        // Updating other cars
        if (timer % OBJECT_FRAME == 0) {
            if (speed < LANE1_SPEED) {
                spawnDir1 = BACK;
            } else {
                spawnDir1 = FRONT;
            }

            if (speed < LANE2_SPEED) {
                spawnDir2 = BACK;
            } else {
                spawnDir2 = FRONT;
            }

            if (speed < LANE3_SPEED) {
                spawnDir3 = BACK;
            } else {
                spawnDir3 = FRONT;
            }

            int i;
            for (i = 1; i < 10; i++) {
                if (!isEmpty(i)) {
                    // Changing LANE1
                    if (i < 4) {
                        moveCar(-1, cars[i].y + speed - LANE1_SPEED, cars + i);
                        continue;
                    }
                    // Changing LANE2
                    if (i < 7) {
                        moveCar(-1, cars[i].y + speed - LANE2_SPEED, cars + i);
                        continue;
                    }
                    // Changing LANE3
                    moveCar(-1, cars[i].y + speed - LANE3_SPEED, cars + i);
                }
            }
            //printf("Get Game_Over = %d\n", game_over);

            // Spawning new cars
            // LANE1
            if (SP1) LaneSpawn(1);

            // LANE2
            if (SP2) LaneSpawn(2);

            // LANE3
            if (SP3) LaneSpawn(3);
        }

        timer++;

        // Want to read data in interval
        MAP_UtilsDelay(50000);
    }

    printGameOver();
    printf("Game Over!!!\n");

    MAP_UtilsDelay(5000000);
}

