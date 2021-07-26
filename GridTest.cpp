#include <unordered_map>
#include <iostream>
#include "../nncurses/Utils.hpp"
using std::unordered_map, std::cout;
using nc::TimeTracker;

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

int main(){
	int32_t defVal=69;
	InfGrid grid(defVal);

	cout << "starting ats on unallocated\n";

	TimeTracker time;
	time.start();

	for (int32_t y=-256;y<256;y++)
		for (int32_t x=-256;x<256;x++)
			grid.at(x,y);

	time.end();
	cout << time.time() << "ms\n";

	cout << "0,0: " << grid.at(0,0) << "\n";

	cout << "starting assigns\n";
	time.start();
	int32_t val=420;
	for (int32_t y=-256;y<256;y++)
		for (int32_t x=-256;x<256;x++)
			grid.assign(x,y,val);
	time.end();
	cout << time.time() << "ms\n";

	cout << "0,0: " << grid.at(0,0) << "\n";

	cout << "starting ats on allocated\n";

	time.start();

	for (int32_t y=-256;y<256;y++)
		for (int32_t x=-256;x<256;x++)
			grid.at(x,y);

	time.end();
	cout << time.time() << "ms\n";

	cout << "0,0: " << grid.at(0,0) << "\n";
}