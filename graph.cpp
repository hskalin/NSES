#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <sstream>
#include <locale>

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

using namespace std;
using namespace Drawille;
using std::wcout;
using std::locale;


/*int endp(char ch[20]){
	strrev(ch);
	if(ch[0]=='p') return 1;
	return 0;
}*/

//vector<string> ani, plant;

//INDEX
class counter{
	public:
	int plantCount;
	int genCount;
	int animalCount;
};

vector<counter> cntr1, cntr;

/*
void parseCSV(){
	fstream fin;
	fin.open("population.csv", ios::in);

	//int colnum = 2000;

	vector<string> row;
	string line, word, temp;

	while (fin >> temp) {
		row.clear();
		getline(fin, line);

		stringstream s(line);

		while ( getline(s, word, ',') ){
			row.push_back(word);
		}
		
		ani.emplace_back((row[0]));
		plant.emplace_back((row[1]));
	}
}
*/

void inputPopulation(){
	int pos=0;

	ifstream fin("population.dat", ios::binary);
	cntr1.emplace_back(counter());

	while(fin.read((char*)&cntr1[pos], sizeof(cntr1[pos]))){
		cntr1.emplace_back(counter());
		pos = cntr1.size()-1;
	}

	fin.close();

	if(cntr1.size()<3000) pos=0;
	else pos = cntr1.size() - 3000;

//	cntr.emplace_back(counter());
	for(int i=pos; i<cntr1.size(); i++){
		cntr.emplace_back(counter());
		cntr[cntr.size()-1] = cntr1[i];
	}
}

int main(){

	inputPopulation();

/*        for(int i=0; i<600; i++){
		parseCSV();
	}

	for(int i=0; i<100; i++){
		cout<<stoi(plant[i])<<" ";
	}
*/
//	cout<<5+stoi(ani[0]);
//	cout<<endl;                         //this was causing problem


	locale::global(locale(""));
  	Canvas canvas(165, 42);

  	for(int i = 0; i <= cntr.size(); i++) {
    		canvas.set(i/9,160-cntr[i].animalCount/3,0);
  	}	
        
  	for(int i = 0; i <= cntr.size(); i++) {
    		canvas.set(i/9,160-cntr[i].plantCount/3,1);
  	}	

  	canvas.draw(wcout);


	return 0;
}
