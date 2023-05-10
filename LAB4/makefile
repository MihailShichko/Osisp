CC=g++
CFLAGS=-c -Wall
LDFLAGS=

SOURCES=Main.cpp consumer.cpp producer.cpp init.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=program

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

main.o: consumer.h producer.h
consumer.o: consumer.h init.h
producer.o: producer.h init.h

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)