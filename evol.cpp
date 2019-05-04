/*                          this is the layout of the world
 *      _______________________________________________________________________
 *      |                                                                      |
 *      |                                                          #           |
 *      |                       O                                              |
 *      |                                           #                          |
 *      |                             #                    O                   |
 *      |                                                                      |
 *      |   #         #                                                        |
 *      |                                                              #       |
 *      |                      _______________________                         | 
 *      |                      |                      |                        |
 *      |                      |   #          O     # |                        |
 *      |                      |      #  O            |                        |
 *      |                      |               #   #  |                        |
 *      |                      |   #      #     #     |                        |
 *      |      #               |       O      #    O  |                        |
 *      |                      |______________________|                        |
 *      |                                                          O           |
 *      |                                                                      |
 *      |                                                                      |
 *      |                                                                      |
 *      |                O                      #                              |
 *      |                                                                      |
 *      |                                                     #                |
 *      |                                                                      |
 *      |______________________________________________________________________|
         100 X 30 UNITS
         
        -the middle jungle has more dense vegetation than the rest of the world
        -currently the animals are represented by 'O' and plants by '#'
        -the world is contnuous. if animal crosses boundry, it willl appear on the orther side
        
        the direction coding of an animal :
                    
                    0   1   2
                    
                    7   A   3
                    
                    6   5   4
                    
        each number corresponds to the direction animal is facing
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 100
#define HEIGHT 30
#define PLANT_ENERGY 80

using namespace std;

int plants[HEIGHT][WIDTH];          //this contains the positions of the plants
int jungle[4] = {45, 10, 10, 10};   //this defines the jungle area
//int cntr = 0;                       //counter to keep track of plants

void randomPlant (int left, int top, int width, int height){
    srand(time(0));
    
    //this thing marks the pos. of plant as 1 in the plants array
    plants[top + (rand() % height)][left + (rand() % width)] = 1;
}

void growPlant () {
    randomPlant(jungle[0], jungle[1], jungle[2], jungle[3]);    //grows plant in jungle
    randomPlant(0, 0, WIDTH, HEIGHT);                           //grows plant elsewhere
}

class animal{
public:
    int x, y, dir, energy;          //dir is the direction currently facing
    int gene[8];
};

void animal::moveAnimal () {
    if (dir > 1 && dir < 5) x = (x + 1)%WIDTH;
    if (dir==0 || dir==7 || dir==6) x = (x - 1)%WIDTH;
    if (dir > 3 && dir < 7) y = (y + 1)%HEIGHT;
    if (dir==0 || dir==1 || dir==2) y = (y - 1)%HEIGHT;
    
    energy--;
}


int main(){
    srand(time(0));
    
    /*animal a1={WIDTH/2, HEIGHT/2, 0, 1000};
    
    for(int i=0; i<8; i++){
        a1.gene[i]=rand()%10;
    }*/
    
    return 0;
}
