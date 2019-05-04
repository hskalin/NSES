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


#define WIDTH           100
#define HEIGHT          30
#define PLANT_ENERGY    80
#define REP_ENERGY      200         //energy required for reproduction

using namespace std;

int plants[HEIGHT][WIDTH];          //this contains the positions of the plants
int jungle[4] = {45, 10, 10, 10};   //this defines the jungle area
//int cntr = 0;                       //counter to keep track of plants





//PLANT RELATED FUNCTIONS

void randomPlant (int left, int top, int width, int height){
    srand(time(0));
    
    //this thing marks the pos. of plant as 1 in the plants array
    plants[top + (rand() % height)][left + (rand() % width)] = 1;
}

void growPlant () {
    randomPlant(jungle[0], jungle[1], jungle[2], jungle[3]);    //grows plant in jungle
    randomPlant(0, 0, WIDTH, HEIGHT);                           //grows plant elsewhere
}






// ANIMAL RELTED FUNCTIONS

class animal{
public:
    int x, y, dir, energy;          //dir is the direction currently facing
    int gene[8];
    void moveAnimal ();
    void eatAnimal ();
    void turnAnimal ();
    animal();                       //default constructor
    animal(animal &a);              //copy constructor
};

animal::animal () {
    x = WIDTH/2;
    y = HEIGHT/2;
    dir = 0;
    energy = 1000;
    
    srand(time(0));
    
    for(int i=0; i<8; i++){         //randomly allocates the genes
        gene[i]=rand()%10;
    }
}

animal::animal (animal &a) {
    x = a.x;                        //copies stuff
    y = a.y;
    dir = a.dir;
    
    energy = a.energy / 2;          //half energy to parent and child
    a.energy = a.energy / 2;
    
    for(int i=0; i<8; i++){         
        gene[i]=a.gene[i];
    }
    
    int geneChng, genePos;
    
    srand(time(0));                 //randomly chooses gene and in(de)crements by 1
    genePos = rand() % 8;
    geneChng = rand()%3 - 1;
    
    gene[genePos] += (geneChng > 1) ? (geneChng) : (1);     //cant be negative
}

void animal::turnAnimal () {
    srand(time(0));
    int sum=0, num;
    
    for (int i=0; i<8; i++){
        sum += gene[i];
    }
    
    num = rand()%sum + 1;
    for (int i=0; i<8; i++){
        num = num - gene[i];
        if (num <= 0){
            dir = i;
            break;
        }
    }
}

void animal::moveAnimal () {
    if (dir > 1 && dir < 5) x = (x + 1)%WIDTH;
    if (dir==0 || dir==7 || dir==6) x = (x - 1)%WIDTH;
    if (dir > 3 && dir < 7) y = (y + 1)%HEIGHT;
    if (dir==0 || dir==1 || dir==2) y = (y - 1)%HEIGHT;
    
    energy--;
}

void animal::eatAnimal () {
    if(plants[y][x] == 1){
        energy = energy + PLANT_ENERGY;
        plants[y][x] = 0;
    }
}



//WORLD RELATED FUNCTIONS


int main(){

    animal p1;
    
    cout<<p1.energy;
    
    //cout<<a.gene[1];
    return 0;
}
