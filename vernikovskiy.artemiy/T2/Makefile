CC=g++
CFLAGS=-Wall -g
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=app

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
$(base_name.o): task.h

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
