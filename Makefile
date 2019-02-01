CC=g++
CPLUSPLUSVERSION=-std=c++11
SRCDIR=src
BUILDDIR=build
TARGET=todo
SRCEXT=cpp
INC=-I include
CFLAGS=-g

SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): env $(BUILDDIR)/todo.o $(BUILDDIR)/configReader.o $(BUILDDIR)/stringUtils.o $(BUILDDIR)/todoItem.o $(BUILDDIR)/todoFileHandler.o $(BUILDDIR)/identifier.o $(BUILDDIR)/listItem.o
	$(CC) $(CPLUSPLUSVERSION) $(BUILDDIR)/todo.o $(BUILDDIR)/configReader.o $(BUILDDIR)/stringUtils.o $(BUILDDIR)/todoItem.o $(BUILDDIR)/todoFileHandler.o $(BUILDDIR)/identifier.o $(BUILDDIR)/listItem.o -o $(TARGET)

env:
	@echo "CC         = ${CC}"
	@echo "BUILDDIR   = ${BUILDDIR}"
	@echo "SRCDIR     = ${SRCDIR}"
	@echo "INC        = ${INC}"
	@echo "MAKE       = ${MAKE}"

$(BUILDDIR)/todo.o: $(SRCDIR)/todo.cpp
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todo.cpp -o $(BUILDDIR)/todo.o

$(BUILDDIR)/configReader.o: $(SRCDIR)/configReader.cpp $(SRCDIR)/configReader.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/configReader.cpp -o $(BUILDDIR)/configReader.o

$(BUILDDIR)/stringUtils.o: $(SRCDIR)/stringUtils.cpp $(SRCDIR)/stringUtils.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/stringUtils.cpp -o $(BUILDDIR)/stringUtils.o

$(BUILDDIR)/todoItem.o: $(SRCDIR)/todoItem.cpp $(SRCDIR)/todoItem.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todoItem.cpp -o $(BUILDDIR)/todoItem.o

$(BUILDDIR)/todoFileHandler.o: $(SRCDIR)/todoFileHandler.cpp $(SRCDIR)/todoFileHandler.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todoFileHandler.cpp -o $(BUILDDIR)/todoFileHandler.o

$(BUILDDIR)/identifier.o: $(SRCDIR)/identifier.cpp $(SRCDIR)/identifier.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/identifier.cpp -o $(BUILDDIR)/identifier.o

$(BUILDDIR)/listItem.o: $(SRCDIR)/listItem.cpp $(SRCDIR)/listItem.h
	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/listItem.cpp -o $(BUILDDIR)/listItem.o

read: env
	echo $(SOURCES)
	echo $(OBJECTS)

clean:
	rm $(BUILDDIR)/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
	
