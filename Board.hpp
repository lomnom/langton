#ifndef Board
#define Board

#include <string>
#include <vector>
#include "Grid.hpp"
#include "nncurses/Screens.hpp"

using std::string,std::vector,nc::Screen,nc::blocks;
using nc::Texture,nc::Style;

unordered_map<char,int8_t> ruleMap={
	{'R',1}, //right
	{'L',-1}, //left
	{'C',0}, //continue straight
	{'U',2} //make a u turn
};

class LangBoard{
public:
	int32_t antX,antY;
	InfGrid grid;
	uint8_t direction; //0-up 1-right 2-down 3-left
	string character=blocks[0b1100];
	uint64_t steps=0;

	vector< int8_t > rules;

	LangBoard(int8_t startDir,vector< int8_t > rules): antX(0), antY(0), direction(startDir), rules(rules), grid(0){}

	void step(){
		steps++;
		int* ruleIdPtr=grid.atptr(antX,antY);
		uint8_t rule=rules[ *ruleIdPtr ];
		*ruleIdPtr = ( *ruleIdPtr+1 )%rules.size();

		direction=(direction+rule)%4; //its a uint

		if (direction==0){
			antY--;
		}else if (direction==1){
			antX++;
		}else if (direction==2){
			antY++;
		}else{
			antX--;
		}
	}

	void render(Screen* scr, int32_t partStartX, int32_t partStartY, int32_t renderStartX, int32_t renderStartY, int32_t height, int32_t width){
		for (int row=0;row<height;row++){
			for (int col=0;col<width;col++){
				scr->screen[renderStartY+row][renderStartX+col]=Texture(
					character,
					Style(
						grid.at(partStartX+col,partStartY+(row*2)),
						grid.at(partStartX+col,partStartY+(row*2)+1),
						0
					)
				);
			}
		}
	}
};

#undef string
#undef vector
#undef Screen
#undef Texture
#undef Style
#undef blocks

#endif