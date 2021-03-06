SHELL := /bin/bash
LIB_FLAG = -lncursesw -lpthread -lopencv_core -lopencv_imgproc -lopencv_videoio
# following https://stackoverflow.com/a/43835225/6511525
OPENCV_INCLUDE_FLAG = `pkg-config --cflags opencv`
NCURSES_INCLUDE_FLAG = `ncursesw5-config --cflags`

.phony: all clean install

all: ascii-videoplayer

ascii-videoplayer: main.o render.o color.o
	g++ $^ ${LIB_FLAG} -o $@

main.o: main.cpp render.h color.h
	g++ main.cpp ${OPENCV_INCLUDE_FLAG} ${NCURSES_INCLUDE_FLAG} -c -o $@

render.o: render.cpp render.h color.h
	g++ render.cpp ${OPENCV_INCLUDE_FLAG} ${NCURSES_INCLUDE_FLAG} -c -o $@

color.o: color.cpp color.h
	g++ color.cpp ${NCURSES_INCLUDE_FLAG} -c -o $@

install: ascii-videoplayer
	cp $^ ${DESTDIR}
clean:
	rm ascii-videoplayer *.o
