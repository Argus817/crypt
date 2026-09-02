# Sample `make` config

```
󰣇 Documents/GitHub/crypt   main  !? 
❯ make
mkdir -p obj
g++ -Wall -Wextra -std=c++17 -O2 -c src/main.cpp -o obj/main.o
g++ -Wall -Wextra -std=c++17 -O2 -o target/executable obj/main.o

󰣇 Documents/GitHub/crypt   main  !? 
❯ make run
./target/executable
abc

󰣇 Documents/GitHub/crypt   main  !? 
❯ make clean
rm -rf obj target/executable
```