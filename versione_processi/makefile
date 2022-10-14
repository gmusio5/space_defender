spacedefender.out: main.o setup.o menu.o graphics.o game.o
	cc main.o setup.o menu.o graphics.o game.o -o spacedefender.out -lncurses
main.o: global.h main.c
	cc -c main.c
setup.o: global.h setup.c
	cc -c setup.c
menu.o: global.h menu.h menu.c
	cc -c menu.c 
graphics.o: global.h graphics.c
	cc -c graphics.c
game.o: global.h game.h game.c 
	cc -c game.c

clean:
	rm *.o
	rm spacedefender.out 
	rm ./../fileAudio/*.txt

