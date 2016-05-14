CC := clang++
STANDARDFLAG := --std=c++14
CFLAGS := `pkg-config --cflags sdl2` `pkg-config --cflags SDL2_mixer`
LDFLAGS :=`pkg-config --libs sdl2` `pkg-config --libs SDL2_mixer`
SOURCES := typeefect.cpp
EXECUTABLE := typeefect

all: $(SOURCES)
	$(CC) $(STANDARDFLAG) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

g++: $(SOURCES)
	g++ $(STANDARDFLAG) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)
