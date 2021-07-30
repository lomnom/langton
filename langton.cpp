#include "nncurses/nncurses.hpp"
#include "Board.hpp"
#include <unordered_map>
#include <vector>

using nc::Texture, nc::Effect, nc::Col256, nc::Style, nc::Terminal, nc::TranspText, nc::cinchr,nc::gettermsize;
using std::cout, std::string, std::vector, std::to_string;

TranspText stats("",Effect(0), 252);

long topX=0;
long topY=0;

unsigned long step=1;
unsigned long move=1;

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