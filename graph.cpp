#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

//here i have included the drawille-cpp file in the code for portability
//https://github.com/ix/drawille-plusplus

#ifndef DRAWILLE_HPP
#define DRAWILLE_HPP

#include <vector>
#include <ostream>

namespace Drawille {
  using std::vector;
  using std::wostream;

  constexpr size_t pixmap[4][2] = {
    {0x01, 0x08},
    {0x02, 0x10},
    {0x04, 0x20},
    {0x40, 0x80},
  };

  constexpr wchar_t braille = 0x2800;

  class Canvas {
  public:
    Canvas(size_t width, size_t height) {
      this->canvas.resize(height);
      for(auto& v: this->canvas)
        v.resize(width);
    }

    void set(size_t x, size_t y, int clr) {
      if(x > (this->canvas[0].size() * 2) or x < 1) x = 0;
      if(y > (this->canvas.size() * 4) or y < 1)    y = 0;
      this->canvas[y / 4][x / 2] |= pixmap[y % 4][x % 2];
    }

    void unset(size_t x, size_t y) {
      if(x > (this->canvas[0].size() * 2) or x < 1) x = 0;
      if(y > (this->canvas.size() * 4) or y < 1)    y = 0;
      this->canvas[y / 4][x / 2] &= ~pixmap[y % 4][x % 2];
    }

    void draw(wostream& strm) {
      for(auto& v: this->canvas) {
        for(auto& c: v) {
          if(c == 0) strm << " ";
//	  else if(1 == 0) strm << std::wstring{braille+c};
          else strm<<std::wstring{braille+c};
        }
        strm << std::endl;
      }
    }

  protected:
    vector<vector<wchar_t>> canvas;
  };
}

#endif
//drawille.hpp end


using namespace std;
using namespace Drawille;
using std::wcout;
using std::locale;

//INDEX
class counter{
	public:
	int plantCount;
	int genCount;		//this is not used in this program
	int animalCount;
};

vector<counter> cntrTemp, cntr;


void inputPopulation(){
	int pos=0;

	/*The population.dat file was made using
	
		std::ofstream fout("./population/population.dat", std::ios::binary|std::ios::app);
		fout.write((char*)&index, sizeof(index));
	
	where index is a data memeber of the counter class*/
	
	ifstream fin("population.dat", ios::binary);
	cntrTemp.emplace_back(counter());

	while(fin.read((char*)&cntrTemp[pos], sizeof(cntrTemp[pos]))){
		cntrTemp.emplace_back(counter());
		pos = cntrTemp.size()-1;
	}

	fin.close();

	//initializing pos such that only the last 3000 points will be 
	//entered into cntr
	if(cntrTemp.size()<3000) pos=0;
	else pos = cntrTemp.size() - 3000;
	
	//initialising cntr with last 3000 values. i am using 3000 as a limit
	//because that is how many will fit on my monitor
	for(int i=pos; i<cntrTemp.size(); i++){
		cntr.emplace_back(counter());
		cntr[cntr.size()-1] = cntrTemp[i];
	}
}

int main(){

	inputPopulation();

	locale::global(locale(""));
  	Canvas canvas(165, 42);

  	for(int i = 0; i <= cntr.size(); i++) {
		
		/*to fill in the points, the syntax is canvas.set(x,y) where 
		x and y are the coordinates of the points.
		however as it measures the y axis distance from upper left corner
		instead of lower left corner, we instead write canvas.set(x,y_max-y)
		where y_max is the max limit of the y axis (in characters)
		
		for scaling the output you can multiply or divide by any number
		division will compress the graph and multiplication will stretch it
		*/
		
    		canvas.set(i/9,160-cntr[i].animalCount/3,0); 	
  	}	
        
  	for(int i = 0; i <= cntr.size(); i++) {
    		canvas.set(i/9,160-cntr[i].plantCount/3,1);
  	}	

  	canvas.draw(wcout);


	return 0;
}
