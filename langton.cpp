#include "../nncurses/nncurses.hpp"
#include <unordered_map>
#include <thread>
#include <vector>

using nc::Texture, nc::Effect, nc::Col256, nc::Style, nc::Terminal, nc::Screen, nc::TextLine, nc::TimeTracker, nc::Renderable, nc::TimeLimiter,nc::cinchr;
using std::unordered_map, std::map, std::cout, std::pair, std::string, std::thread, std::vector, std::stoi, std::to_string;

class InfGrid{
public:
	unordered_map<int64_t, int32_t> grid;
	int32_t defvalue;
	InfGrid(int32_t defvalue):defvalue(defvalue){};

	void alloc(int32_t x, int32_t y){
		int64_t hash=x;
		hash= (hash<<32)+y;
		grid[hash]=defvalue;
	}

	void dealloc(int32_t x, int32_t y){
		int64_t hash=x;
		hash= (hash<<32)+y;
		grid.erase(hash);
	}

	int32_t at(int32_t x, int32_t y){ // s p e e d
		int64_t hash=x;
		hash= (hash<<32)+y;
		return grid.insert( {hash,defvalue} ).first->second;
	}

	void assign(int32_t x, int32_t y, int32_t val){ // s p e e d
		int64_t hash=x;
		hash= (hash<<32)+y;
		grid[hash]=val;
	}
};

uint8_t* noeffectint=new uint8_t(0);
Effect* noeffect=new Effect(noeffectint);

template<class T>
T toroid(T a, T b) {
	int result = a % b;
	return result >= 0 ? result : result + b;
}

class LangBoard{
public:
	int32_t antX,antY;
	InfGrid grid;
	int8_t direction; //0-up 1-right 2-down 3-left
	string* character=&nc::AscBlok::block[0b1100];
	uint64_t steps=0;

	vector< int8_t > rules;
	vector< Texture* > chars;

	short ant;
	LangBoard(int8_t startDir,vector< int8_t > rules,short ant): antX(0), antY(0), direction(startDir), rules(rules), grid(0), ant(ant){}

	void step(){
		steps++;
		int ruleId=grid.at(antX,antY);
		uint8_t rule=rules[ruleId];
		grid.assign(antX,antY, (ruleId+1)%rules.size() );
		direction=toroid(direction+rule,4);
		switch (direction){
			case 0:
				antY--;
				// cout << "y--\n";
				break;
			case 1:
				antX++;
				// cout << "x++\n";
				break;
			case 2:
				antY++;
				// cout << "y++\n";
				break;
			case 3:
				antX--;
				// cout << "x--\n";
				break;
		};
		// cout << "X: " << antX << " Y: " << antY << " assigned: "<< (ruleId+1)%rules.size() << " direction: " << (int)direction << "\n";
	}

	void render(Screen* scr, int32_t partStartX, int32_t partStartY, int32_t renderStartX, int32_t renderStartY, int32_t height, int32_t width){
		for (int n=0;n<chars.size();n++){
			Texture* texture=chars[n];
			delete texture->style->color->fg;
			delete texture->style->color->bg;
			delete texture->style->color;
			delete texture->style;
			delete texture;
		}
		chars.erase(chars.begin(),chars.end());

		Texture* texture;
		for (int row=0;row<height;row++){
			for (int col=0;col<width;col++){
				texture=new Texture(
					character,
					new Style(
						new Col256( //fg top bg bottom
							new short(
								grid.at(partStartX+col,partStartY+(row*2))
							),
							new short(
								grid.at(partStartX+col,partStartY+(row*2)+1)
							)
						),
						noeffect
					)
				);
				scr->screen[renderStartY+row][renderStartX+col]=texture;
				chars.push_back(texture);
			}
		}
	}

};

short* black=new short(256);
short* white=new short(252);

Col256* backgroundCol=new Col256(white,black);
Style* backgroundStl=new Style(backgroundCol,noeffect);
Texture* background=new Texture(new string(" "),backgroundStl);

string statsStr="";
int statsX=0;
int statsY=0;
TextLine stats(&statsStr, white, noeffect, &statsX, &statsY);
Terminal terminal(background);

bool ended=false;

int main(int argc, char *argv[]){
	std::vector<std::string> arguments(argv + 1, argv + argc);
	vector< int8_t > rules;

	for (int i=0;i<arguments.size();i++){
		rules.push_back(stoi(arguments[i]));
	}

	LangBoard board(0,rules,89);
	
	//render(Screen* scr, int32_t partStartX, int32_t partStartY, int32_t renderStartX, int32_t renderStartY, int32_t height, int32_t width)
	int topX=0;
	int topY=0;
	unsigned long step=1;
	unsigned long move=1;

	while (true) {
		board.render(&terminal.screen,topX,topY,0,0,terminal.screen.rows,terminal.screen.cols);
		statsStr="step count: "+to_string(board.steps)+
				 " step size: "+to_string(step)+
				 " x: "+to_string(board.antX)+
				 " y: "+to_string(board.antY)+
				 " vX: " +to_string(topX)+
				 " vY: " +to_string(topY)+
				 " move: " +to_string(move);

		stats.render(&terminal.screen);
		terminal.project();

		char currChr=cinchr();
		if (currChr=='q'){
			break;
		}else if (currChr=='w'){
			topY-=move;
		}else if (currChr=='a'){
			topX-=move;
		}else if (currChr=='s'){
			topY+=move;
		}else if (currChr=='d'){
			topX+=move;

		}else if (currChr=='c'){
			for (int n=0;n<step;n++)
				board.step();

		}else if (currChr=='e'){
			step= (step==1 ? 1 : step/2);
		}else if (currChr=='r'){
			step= (step*2==0 ? step : step*2);

		}else if (currChr=='f'){
			move= (move==1 ? 1 : move/2);
		}else if (currChr=='g'){
			move= (move*2==0 ? move : move*2);
		}
	}
}