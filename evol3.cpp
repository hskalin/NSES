#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <unistd.h>
#include <thread>
#include <ncurses.h>
#include <atomic>


#define WIDTH           168
#define HEIGHT          35
#define PLANT_ENERGY    80
#define REP_ENERGY      200         //energy required for reproduction

//using namespace std;

using std::cout;
using std::cin;
using std::vector;
using std::endl;
using std::thread;

int plants[HEIGHT][WIDTH];          //this contains the positions of the plants
int jungle[4] = {45, 10, 10, 10};    //this defines the jungle area
int sizeg;

int end = 0;
char command;

std::atomic<bool> terminate(false);

//PLANT RELATED FUNCTIONS

void randomPlant (int left, int top, int width, int height){
    int x,y;
    x = left + (rand()%width);
    y = top + (rand()%height);
    
    //this thing marks the pos. of plant as 1 in the plants array
    plants[y][x] = 1;
}

void growPlant () {
    randomPlant(jungle[0], jungle[1], jungle[2], jungle[3]);    //grows plant in jungle
    randomPlant(0, 0, WIDTH, HEIGHT);                           //grows plant elsewhere

    //randomPlant(0, 0, WIDTH, HEIGHT);
    //randomPlant(0, 0, WIDTH, HEIGHT);
}







// ANIMAL RELTED FUNCTIONS

class animal{
public:
    int x, y, dir, energy, age, speed;      //dir is the direction currently facing
    int gene[8];
    void moveAnimal ();
    void eatAnimal ();
    void turnAnimal ();
    void repAnimal (animal &a);
    void geneDisplay ();
    animal();                       //default constructor
    //animal(animal &a);              //copy constructor
};

animal::animal () {
    x = WIDTH/2;
    y = HEIGHT/2;
    age = 0;
    
    dir = 0;
    energy = 1000;
    
    speed = rand()%4 + 1; 
    
    for(int i=0; i<8; i++){         //randomly allocates the genes      //may cause difficulty in deciding reference (initial) traits.  
        gene[i]=rand()%10;
    }
}

void animal::repAnimal (animal &a) {
    x = a.x;                        //copies stuff
    y = a.y;
    dir = a.dir;
    
    energy = a.energy / 2;          //half energy to parent and child
    a.energy = a.energy / 2;
    
    for(int i=0; i<8; i++){         
        gene[i]=a.gene[i];
    }
    
    int geneChng, genePos;
    
                      //randomly chooses gene and in(de)crements by 1
    genePos = rand() % 8;
    geneChng = (rand() % 3) - 1;
    
    gene[genePos] = abs(gene[genePos] + geneChng);     //cant be negative
    speed = abs(a.speed + geneChng);
    
    if(speed == 0) speed = 1;                          //speed cant be zero 
}



void animal::turnAnimal () {
     
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

int mod(int a){
    if(a>=0) return a;
    else if(a<0) return (-a);
}

void animal::moveAnimal () {
    if (dir > 1 && dir < 5)                 x = mod((x + speed)%WIDTH);     //this thing caused a lot of trouble
    else if (dir==0 || dir==7 || dir==6)    x = mod((x - speed)%WIDTH);
    
    if (dir > 3 && dir < 7)                 y = mod((y + speed)%HEIGHT);
    else if (dir==0 || dir==1 || dir==2)    y = mod((y - speed)%HEIGHT);
    
    energy = energy - speed*speed;
}

void animal::eatAnimal () {
    if(plants[y][x] == 1){
        energy = energy + PLANT_ENERGY;
        plants[y][x] = 0;
    }
}



//WORLD RELATED FUNCTIONS
vector<animal> animals;                 //make a vector animal

void updateWorld(){
    int size;                                   //i do not know why i did this
    
    
    for(auto i=animals.begin(); i != animals.end(); ++i) {
        if ((i->energy) <= 0) {
            animals.erase(i);
            i--;
        }
    }
    
    size = animals.size();
    
    for(int i=0; i<size; i++){
        animals[i].turnAnimal();
        animals[i].moveAnimal();
        animals[i].eatAnimal();
        animals[i].age++;
        if(animals[i].energy >= REP_ENERGY){
            animals.emplace_back(animal());
            animals[animals.size()-1].repAnimal(animals[i]);
        }
    }
            
    growPlant();
    sizeg=size;
}
    


//UI related functions
void animal::geneDisplay() {
    cout<<"GENE : ";
    for (int i=0; i<8; i++) cout<<gene[i]<<" ";
    cout<<endl;
}


//Layout Related Functions
void posDisplay()
{
    int i,j,k,flag=1;
    for(i=0; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++){
            
            flag=1;                                //resetting the flag
            if(plants[i][j]==1) cout<<"\u2633";
            
            else {
                for(k=0;k<animals.size();k++){
                    if((animals[k].x==j)&&(animals[k].y==i)){
                        cout<<"\u2688";
                        flag = 0;
                        break;
                    }
                }
                
            if(flag) cout<<" ";
            }         
        }
        cout<<endl;
    }
    for(i=0; i<WIDTH; i++) cout<<"_";
    cout<<"\nEnter Command\n";
}

void fun(){
    
    while(!terminate.load()){
        
        cin>>command;
        if(command=='x'){
            end = 1;
            break;
        }
    }
}



void run(){
    
    long delay=100000;
    thread th1(fun);             //Multithreading !!!!!!!!!!!!
    
    while(1){
        //cout<<animals.size()<<endl;
        updateWorld();
        posDisplay();
        
        if (command == 'p'){                        //p - pause
            while(command != 's'){                  //s - start
                if(command == 'x') break;           //x - end program
                usleep(100000);
            }
        }
        
        if (command == 'g'){                        //if you press g, it will increment by 1000 generatons
            command = 0;
            for(int i=0; i<1000; i++){ updateWorld(); }
            posDisplay();
        }
            
            
        if(command == ','){
            command =0;
            delay += 10000;
        }
        
        if(command == '.'){
            command =0;
            delay -= 10000;
            if(delay<0) delay=10000;
        }
        
        usleep(delay);
        system("clear");
        
        if (animals.size()==0){
            cout<<"All animals died. Press x to end the program"<<endl;
            terminate = true;
            break;
        }
        
        
        if (end == 1) break;
    }
    
    th1.join();                           //waits for the thread function to end
}


int main(){
    srand(time(0));
    
    animals.reserve(500);
    animals.emplace_back(animal());         //the initial animal
    
    /* animals[0].geneDisplay();
    
    for(int i=0; i<5; i++){
         animals.emplace_back(animal());
         animals[i+1].repAnimal(animals[i]);
         animals[i+1].geneDisplay();
    }
    
    cout<<animals.size()<<endl;
    
    animals.erase(animals.begin() + 2);
    
    
    for(int i=0; i<animals.size(); i++){
         animals[i].geneDisplay();
    }*/
    
    run();
    
    posDisplay();
    
    cout<<"test";
    
    return 0;
    
}  
