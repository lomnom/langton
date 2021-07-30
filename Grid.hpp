#ifndef Grid
#define Grid

#include <unordered_map>
using std::unordered_map;

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

#undef unordered_map

#endif