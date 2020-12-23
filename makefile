make: main.o Game.o Object_2D.o
	g++ -o project1.exe main.o Game.o Object_2D.o -L"soil" -L"freeglut\lib\x64" -lsoil -lopengl32 -lfreeglut -Wl,--subsystem,windows

main.o: main.cpp
	g++ -c -o main.o main.cpp

Game.o: Game.cpp Game.h Object_2D.h Colors.h Texture.h Presets.h
	g++ -c -o Game.o Game.cpp -I"freeglut\include"

Object_2D.o: Object_2D.cpp Object_2D.h Colors.h Texture.h
	g++ -c -o Object_2D.o Object_2D.cpp -I"freeglut\include"