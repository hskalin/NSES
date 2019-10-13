#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <random>

#include <unistd.h>
#include <termios.h>
#include <fstream>

#include <sstream>
#include <string>




#define WIDTH           197                 //BOTH MUST BE GREATER THAN 27
#define HEIGHT          46
#define PLANT_ENERGY    80
#define REP_ENERGY      200         //energy required for reproduction


#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#endif  /* _COLORS_ */

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

using std::cout;
using std::cin;
using std::vector;
using std::endl;
using std::thread;


int plants[HEIGHT][WIDTH];          //this contains the positions of the plants
int jungle[4] = {45, 10, 10, 10};    //this defines the jungle area
int sizeg, species;
float initialDeviation=1.0;

int end = 0;
char command;

std::atomic<bool> terminate(false);

//PLANT RELATED FUNCTIONS

/*
struct runNo{
	int n;
} runs;

void getRuns(){
	runs.n=0;
	std::ifstream g("run.dat", std::ios::binary);
	g.read((char*)&runs, sizeof(runs));
	g.close();
}
*/

int random(int a, int b){
    std::random_device rdev{};
    static std::default_random_engine e{rdev()};
    static std::uniform_int_distribution<int> d{a,b};
    
    return d(e);
}


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
}


//INDEX
class counter{
	public:
	int plantCount=0;
	int genCount=0;
	int animalCount=0;
};

counter index;


// ANIMAL RELTED FUNCTIONS

class animal{
public:
    int x, y, dir, energy, age, speed, specie;      //dir is the direction currently facing
    float deviation, relDev;
    int gene[8];
    void moveAnimal ();
    void eatAnimal ();
    void turnAnimal ();
    void repAnimal (animal &a);
    void geneDisplay ();
    float stdDeviation();
    animal();                       //default constructor
};

float animal::stdDeviation(){
	float avg, sumSq=0, sum=0;
	for(int i=0; i<8; i++){
		sum = sum + gene[i];
	}

	avg = sum/8;

	for(int i=0; i<8; i++){
		sumSq = sumSq + (avg-gene[i])*(avg-gene[i]);
	}

	return (sqrt(sumSq/7));
}

animal::animal () {
    //x = WIDTH/2;
    //y = HEIGHT/2;
    
    specie=0;
    x=50; y=15;
    age = 0;
    
    dir = 0;
    energy = 1000;
    
    speed = rand()%4 + 1; 
    
    for(int i=0; i<8; i++){         //randomly allocates the genes      //may cause difficulty in deciding reference (initial) traits.  
        gene[i]=random(1,10);//abs(rand()%10 + 1);
    }

    deviation = stdDeviation();

    relDev = deviation/initialDeviation -1;
}

int max(int a, int b){
    if(a>b) return a;
    if(b>a) return b;
    return 0;
}

void animal::repAnimal (animal &a) {

    
    x = a.x + 1;                        //copies stuff
    y = a.y + 1;
    dir = a.dir;
    
    energy = a.energy / 2;          //half energy to parent and child
    a.energy = a.energy / 2;
    
    for(int i=0; i<8; i++){         
        gene[i]=a.gene[i];
    }
    
    
    int geneChng, genePos;
    
                      //randomly chooses gene and in(de)crements by 1
    genePos = abs(rand()%8);
    geneChng = abs((rand()%5)) - 2;
    
    gene[genePos] = abs(gene[genePos] + geneChng);     //cant be negative
    
    if(gene[genePos]==0) gene[genePos] = 1;

    deviation = stdDeviation();

    relDev = deviation/initialDeviation -1;
}



void animal::turnAnimal () {
     
    int sum=0, num;
    
    for (int i=0; i<8; i++){
        sum += gene[i];
    }
    
    num = abs(rand()%sum + 1);
    for (int i=0; i<8; i++){
        num = num - gene[i];
        if (num <= 0){
            dir = (dir + i)%8; 
            break;
        }
    }
}



void animal::moveAnimal () {
    if (dir > 1 && dir < 5)            x = abs((x + 1)%WIDTH);     //this thing caused a lot of trouble
    if (dir==0 || dir==7 || dir==6)    x = abs((x - 1)%WIDTH);
    
    if (dir > 3 && dir < 7)            y = abs((y + 1)%HEIGHT);
    if (dir==0 || dir==1 || dir==2)    y = abs((y - 1)%HEIGHT);
    
    energy = energy - 1; 
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
    int deviationCat[4]={0,0,0,0};
    
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
        
	for(int j=0; j<4; j++){
		if(animals[i].relDev < j){ 
			deviationCat[j] = 1;
			animals[i].specie = j;
			break;
		}
	}
        
        if(animals[i].energy >= REP_ENERGY){
            animals.emplace_back(animal());
            animals[animals.size()-1].repAnimal(animals[i]);
        }
    }
    
    species = 0;

    for(int i=0; i<4; i++) species = species+deviationCat[i];
    
    growPlant();
    sizeg=size;
    index.genCount++;
    index.animalCount = animals.size();
}
    
void clearAr()
{
    int xl = (rand() % (WIDTH*3/4))+(WIDTH*3/8), yl = (rand() % (HEIGHT*3/4))+(HEIGHT*3/8), j, k ;                            //= (rand() % (upper – lower + 1)) + lower
    /*for(auto i=animals.begin(); i != animals.end(); ++i) {
        if ((xl <= (i->x) <= (xl+10))&&(yl <= (i->y) <= (yl+10))) {
            animals.erase(i);
            i--;
        }
    }*/
    for(j=xl;j<xl+10;j++)                                                                  //sometimes causes segmentation faults
    {
        for(k=yl;k<yl+10;k++)
           plants[k][j]=0;    
    }        
}

void clearSp()
{
    int x = (rand() % 4) ;                            //= (rand() % (upper – lower + 1)) + lower
    for(auto i=animals.begin(); i != animals.end(); ++i) {
        if ((i->specie) == x) {
            animals.erase(i);
            i--;
        }
    }
}

//UI related functions
void animal::geneDisplay() {
    cout<<"X:"<<"\033[1;34m"<<x<<"\033[0m";
    cout<<"     Y:"<<"\033[1;34m"<<y<<"\033[0m";
    cout<<"     DIR:"<<"\033[1;31m"<<dir<<"\033[0m";
    cout<<"     ENERGY:"<<"\033[1;31m"<<energy<<"\033[0m";
  //  cout<<"     SPEED:"<<"\033[1;31m"<<x<<"\033[0m";
    cout<<"     RDEV:"<<"\033[1;31m"<<relDev<<"\033[0m";
    cout<<"     DEV:"<<"\033[1;31m"<<deviation<<"\033[0m";
    
    cout<<"     AGE:"<<"\033[1;31m"<<age<<"\033[0m";
    cout<<"     GENE : ";
    for (int i=0; i<8; i++) cout<<"\033[1;37m"<<gene[i]<<"\033[0m"<<" ";
    cout<<endl;
}

char dmode = 'e';

//Layout Related Functions
void posDisplay()
{
    index.plantCount=0;
    int i,j,k,flag=1;
    for(i=0; i<HEIGHT; i++) {
        for(j=0; j<WIDTH; j++){
            
            flag=1;                                //resetting the flag
            if(plants[i][j]==1){
		    cout<<BOLD(FGRN("\u2633"));
		    index.plantCount++;
	    }
            
            else {
                for(k=0;k<animals.size();k++){
                    if((animals[k].x==j)&&(animals[k].y==i)){
			    if(dmode == 'e'){
			    
                        if(animals[k].energy < (0.2*REP_ENERGY)){
                            cout<<FRED("\u2688");
                        }
                        else if(animals[k].energy > (0.75*REP_ENERGY)){
                            cout<<BOLD(FBLU("\u2688"));
                        }
                        else{
                            cout<<BOLD(FWHT("\u2688"));
                        }
			    }

			    else if(dmode == 's'){
			if(animals[k].specie == 0){
                            cout<<FRED("\u2688");
                        }
                        else if(animals[k].specie == 1){
                            cout<<BOLD(FBLU("\u2688"));
                        }
			else if(animals[k].specie == 2){
				cout<<BOLD(FWHT("\u2688"));
			}
                        else{
                            cout<<BOLD(FYEL("\u2688"));
                        }
			    }
			    else if(dmode == 'p')
                    cout<<" ";
                        flag = 0;
                        break;
                    }
                }
                
            if(flag) cout<<" ";
            }         
        }
        cout<<endl;
    }
    for(j=0; j<WIDTH; j++) cout<<"_";
    cout<<"\n"<<BOLD(FBLU("Generation:"))<<index.genCount<<BOLD(FBLU("   |   Animal population:"))<<index.animalCount<<BOLD(FBLU("  |  Plant population:"))<<index.plantCount<<BOLD(FBLU("  |  Species Detected:"))<<species<<endl;
}

void fun(){
    
    while(!terminate.load()){
        
        command = getch();
        if(command=='x'){
            end = 1;
            break;
        }
    }
}

//File handling
//storing data related functions
void outputPopulationCSV()

{  

  std::fstream fun;

  fun.open("Population.csv",std::ios::app);


       
      fun<<animals.size()<<","<<index.plantCount<<"\n";             //removed genecount


    fun.close();

  
}
/*
std::string dateTime(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d-%m-%Y--%X", &tstruct);
	return buf;
}

*/
void outputPopulation(){
	std::ofstream fout("./population/population.dat", std::ios::binary|std::ios::app);
	fout.write((char*)&index, sizeof(index));
	fout.close();
}


void outputGendata(){	//will store the attributes of the last generation of run (when x is pressed and run() ends)  
  
    std::ostringstream fileName;
    fileName<<"./gendata/gendata"<<index.genCount<<".dat";            
    std::string var = fileName.str();
    std::ofstream fout;

    fout.open(var, std::ios::binary|std::ios::app);

    for(int i=0; i<animals.size(); i++){
	    fout.write((char*)&animals[i], sizeof(animals[i]));
    }

    fout.close();
}
/*
void outputLog(){
	std::ofstream fout("log.txt", std::ios::app);

	fout<<"["<<runs.n<<"] "
	    <<"TIME:"<<dateTime()
	    <<"	   ANIMALS:"<<index.animalCount
	    <<"	   PLANTS:"<<index.plantCount
	    <<"    GENERATIONS:"<<index.genCount
	    <<"    SPECIES:"<<species
	    <<"\n";

	fout.close();

 	runs.n++;
	std::ofstream f("run.dat", std::ios::out|std::ios::binary);
	f.write((char*)&runs, sizeof(runs));
	f.close();
}

*/
void run(){
    
    long delay=100000;
    thread th1(fun);             //Multithreading !!!!!!!!!!!!
    
    while(1){
        //cout<<animals.size()<<endl;  animalPos[animals[i].y][animals[i].x] = 1;
        updateWorld();
        posDisplay();
//	outputPopulation();
//	outputPopulationCSV();
        
        if (command == 'p'){                        //p - pause
            while(command != 's'){                  //s - start
                if(command == 'd'){
                    command = 0;
                    for(int i=0; i<animals.size(); i++) animals[i].geneDisplay();
                }
		if(command == 'y'){
			command == 0;
			system("./graph");
		}
                if(command == 'x') break;           //x - end program
                usleep(100000);
                 if (command == 'm'){                        //if you press m, it will clear a random species or a random area
            command = 0;
            if(rand()%2==0)
                clearAr();      
            if(rand()%2==1)
                clearSp();                
        }
            }
        }
        
        if (command == 'g'){                        //if you press g, it will increment by 1000 generatons
            command = 0;
            for(int i=0; i<1000; i++){ updateWorld(); }
            posDisplay();
        }
        
       
        
        if (command == '.'){
            command = 0;
            delay -= 10000;
        }
        
         if (command == ','){
            command = 0;
            delay += 10000;
        }
         if (command == '1'){
            command = 0;
            dmode = 'e';
        }
         if (command == '2'){
            command = 0;
            dmode = 's';
        }
         if (command == '3'){
            command = 0;
            dmode = 'p';
        }
        


        if (delay<0) delay = 10000;
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

  //  outputGendata();
}



int main(){
    srand(time(0));
   // getRuns();

    animals.reserve(700);
    animals.emplace_back(animal());         //the initial animal

    initialDeviation = animals[0].deviation;
    animals[0].relDev = 0;
    
    
    
    run();
   // outputLog();
    
    posDisplay();
    
    return 0;
    
} 
