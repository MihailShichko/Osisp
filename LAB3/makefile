CC=g++
CFLAGS=-c -Wall
LDFLAGS=
CHILD_SOURCES=Child.cpp
PARENT_SOURCES=Parent.cpp
CHILD_OBJECTS=$(CHILD_SOURCES:.cpp=.o)
PARENT_OBJECTS=$(PARENT_SOURCES:.cpp=.o)
CHILD_EXECUTABLE=Child
PARENT_EXECUTABLE=Parent

all: $(CHILD_EXECUTABLE) $(PARENT_EXECUTABLE)

child.o: Child.cpp ChildProcess.h Signals.h
parent.o: parent.cpp ChildProcess.h Signals.h

$(CHILD_EXECUTABLE): $(CHILD_OBJECTS) ChildProcess.h
	$(CC) $(CHILD_OBJECTS) -o $@

$(PARENT_EXECUTABLE): $(PARENT_OBJECTS) ChildProcess.h
	$(CC) $(PARENT_OBJECTS) -o $@

$(CHILD_EXECUTABLE): $(CHILD_OBJECTS) Signals.h
	$(CC) $(CHILD_OBJECTS)	-o $@

$(PARENT_EXECUTABLE): $(PARENT_OBJECTS) Signals.h
	$(CC) $(PARENT_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(CHILD_EXECUTABLE) $(PARENT_EXECUTABLE)