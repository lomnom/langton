#include "nncurses/nncurses.hpp"
#include <unordered_map>
#include <vector>

using nc::Texture, nc::Effect, nc::Col256, nc::Style, nc::Terminal, nc::Screen, nc::TranspText, nc::TimeTracker, nc::Renderable, nc::TimeLimiter,nc::cinchr,nc::gettermsize,nc::blocks;
using std::unordered_map, std::map, std::cout, std::pair, std::string, std::vector, std::stoi, std::to_string;

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

	int32_t* atptr(int32_t x, int32_t y){
		int64_t hash=x;
		hash= (hash<<32)+y;

		return &grid[hash];
	}
};

TranspText stats("",Effect(0), 252);

long topX=0;
long topY=0;

unsigned long step=1;
unsigned long move=1;

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

int main(int argc, char *argv[]){
	vector<string> arguments(argv, argv + argc);
	if (arguments.size()==1){
		cout << "You need to provide rules as arguments! \n"
				"examples: langton RRLLLRLLLRRR\n"
				"          langton LRRRRRLLR\n"
				"          langton -h\n";
		return 0;
	}

	if ( arguments[1]=="-h" ){
		cout << "run langton's ant by providing rules as arguments!\n"
				"eg: langton RRLLLRLLLRRR\n"
				"    langton LRRRRRLLR\n"
				"controls:\n"
				"    q: quit\n"
				"    w,a,s,d: move\n"
				"    f: move twice as slowly\n"
				"    g: move twice as fast\n"
				"    c: step the ant\n"
				"    e: step half as much\n"
				"    r: step twice as much\n";
		return 0;
	}

	vector< int8_t > rules={};

	for (int i=0;i<arguments[1].size();i++){
		rules.push_back( ruleMap.at(arguments[1].at(i)) );
	}

	Terminal terminal(
		Texture(" ",Style(252,256,0))
	);

	topX= -(terminal.screen.cols/2);
	topY= -(terminal.screen.rows);

	LangBoard board(0,rules);

	while (true) {
		board.render(&terminal.screen,topX,topY,0,0,terminal.screen.rows,terminal.screen.cols);
		stats.text="step count: "+to_string(board.steps)+
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