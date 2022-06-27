CC          = g++
CFLAGS      = -std=c++20 -Wall -Werror -Werror -pedantic
INCLUDE_DIR = -IC:\mingw_dev_lib\sdl2\include\SDL2 -IC:\mingw_dev_lib\sdl2_image\include\SDL2 -IC:\mingw_dev_lib\sdl2_ttf\include\SDL2
LDFLAGS     = -LC:\mingw_dev_lib\sdl2\lib -LC:\mingw_dev_lib\sdl2_image\lib -LC:\mingw_dev_lib\sdl2_ttf\lib
LDLIBS      = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
RM          = del /f

game: tic_tac_toe.o text.o board.o
	$(CC) -o game tic_tac_toe.o text.o board.o $(CFLAGS) $(INCLUDE_DIR) $(LDFLAGS) $(LDLIBS)

tic_tac_toe.o: source/tic_tac_toe.cpp source/board.h source/text.h
	$(CC) -c source/tic_tac_toe.cpp $(CFLAGS) $(INCLUDE_DIR) $(LDFLAGS) $(LDLIBS)

board.o: source/board.cpp source/board.h
	$(CC) -c source/board.cpp  $(CFLAGS) $(INCLUDE_DIR) $(LDFLAGS) $(LDLIBS) 

text.o: source/text.cpp source/text.h
	$(CC) -c source/text.cpp $(CFLAGS) $(INCLUDE_DIR) $(LDFLAGS) $(LDLIBS) 

clean:	
	$(RM) *.o
	$(RM) *.exe