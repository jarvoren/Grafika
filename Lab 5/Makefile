CC = gcc
LIBS = -lX11 -lglut -lGL -lGLU -lGLEW -lm
SOURCES = $(wildcard  *.c)
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE = blender

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LIBS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

