# langton, a *blazingly fast* CLI langton's ant stimulator written in c++
## installation (unix-like systems)
```bash
git clone https://github.com/lomnom/langton
cd langton
g++ -std=c++2a -O3 langton.cpp -o langton
cp langton /usr/local/bin/langton
cd ..
rm -rf langton
```
## uninstallation (unix-like systems)
```bash
rm /usr/local/bin/langton
```