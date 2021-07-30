# langton, a *blazingly fast* CLI langton's ant stimulator written in c++
## installation (unix-like systems)
```bash
git clone https://github.com/lomnom/langton
cd langton
git clone https://github.com/lomnom/nncurses
g++ -std=c++2a -O3 langton.cpp -o langton
cp langton /usr/local/bin/langton
cd ..
rm -rf langton
```
## uninstallation (unix-like systems)
```bash
rm /usr/local/bin/langton
```
## usage
run langton's ant by providing rules as arguments!  
eg: 
  - `langton RRLLLRLLLRRR`
  - `langton LRRRRRLLR`  

controls:
  - q: quit
  - w,a,s,d: move
  - f: move twice as slowly
  - g: move twice as fast
  - c: step the ant
  - e: step half as much
  - r: step twice as much
## screenshots