#ifndef CARS_H_
#define CARS_H_

#define CAR_WIDTH 5
#define CAR_HEIGHT 10

// Player  => tag = 0
// Lane1 Objects => tag = 1 -> 3
// Lane2 Objects => tag = 4 -> 6
// Lane3 Objects => tag = 7 -> 9
struct Car {
    unsigned short tag;
    int x;
    int y;
    unsigned int color;
};

struct Car spawnCar(unsigned short tag, int x, int y, unsigned int color);
void destroyCar(struct Car* c);
void drawCar(struct Car c, int erase);

// moveCar should take in the car it is attempting to move
void moveCar(int newx, int newy, struct Car* c);

#endif /* CARS_H_ */
