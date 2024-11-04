CC=g++
CPLUSPLUSVERSION=-std=c++11
SRCDIR=src
BUILDDIR=build
TARGET=todo
SRCEXT=cpp
INC=-I include
CFLAGS=-g
#SIGFILE="creds.txt"
#SIG=$(shell cat ${SIGFILE})
MANFILE="todo.1"
MANLOCATION="/usr/local/share/man/man1/"

.PHONY: read

SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): env checkBuildDir $(OBJECTS) man-in
	$(CC) $(CPLUSPLUSVERSION) $(OBJECTS) -o $(TARGET)

# Useful site below
#https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

# OLD METHOD BELOW
# (dont delete)
# VVVVVVVVVVVVVVVV
#
#$(TARGET): env checkBuildDir $(BUILDDIR)/todo.o $(BUILDDIR)/configReader.o $(BUILDDIR)/stringUtils.o $(BUILDDIR)/todoItem.o $(BUILDDIR)/todoFileHandler.o $(BUILDDIR)/listItem.o
#	$(CC) $(CPLUSPLUSVERSION) $(BUILDDIR)/todo.o $(BUILDDIR)/configReader.o $(BUILDDIR)/stringUtils.o $(BUILDDIR)/todoItem.o $(BUILDDIR)/todoFileHandler.o $(BUILDDIR)/listItem.o -o $(TARGET)

env:
	@echo $(SIG)
	@echo "CC         = ${CC}"
	@echo "BUILDDIR   = ${BUILDDIR}"
	@echo "SRCDIR     = ${SRCDIR}"
	@echo "INC        = ${INC}"
	@echo "MAKE       = ${MAKE}"

#the - infront of the mkdir allows it to continue if it fails
#in this case if the folder already exists
checkBuildDir:
	-mkdir build

#Note:
# $@ - Variable : File name of the target of the rule
# $< - Variable : Name of the first prereq
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CPLUSPLUSVERSION) -c $< -o $@

# OLD METHOD BELOW
# (dont delete)
# VVVVVVVVVVVVVVVV

#$(BUILDDIR)/todo.o: $(SRCDIR)/todo.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todo.cpp -o $(BUILDDIR)/todo.o
#
#$(BUILDDIR)/configReader.o: $(SRCDIR)/configReader.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/configReader.cpp -o $(BUILDDIR)/configReader.o
#
#$(BUILDDIR)/stringUtils.o: $(SRCDIR)/stringUtils.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/stringUtils.cpp -o $(BUILDDIR)/stringUtils.o
#
#$(BUILDDIR)/todoItem.o: $(SRCDIR)/todoItem.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todoItem.cpp -o $(BUILDDIR)/todoItem.o
#
#$(BUILDDIR)/todoFileHandler.o: $(SRCDIR)/todoFileHandler.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/todoFileHandler.cpp -o $(BUILDDIR)/todoFileHandler.o
#
#$(BUILDDIR)/listItem.o: $(SRCDIR)/listItem.cpp
#	$(CC) $(CPLUSPLUSVERSION) -c $(SRCDIR)/listItem.cpp -o $(BUILDDIR)/listItem.o

read: env
	@echo "Sources:"
	@echo $(SOURCES)
	@echo "Objects:"
	@echo $(OBJECTS)

clean: man-un
	rm $(BUILDDIR)/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
	
man-in: 
	@echo "Installing the man page" $(mandir)
	@echo "At: /usr/local/share/man/man1"
	#cp $(MANFILE) $(MANLOCATION)

man-un: 
	@echo "Uninstalling the man page" $(mandir)
	@echo "At: /usr/local/share/man/man1/todo.1"
	rm $(MANLOCATION)$(MANFILE)
