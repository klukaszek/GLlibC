# this makefile creates engine executable (CAN BE EASILY CONVERTED TO C++ BY CHANGING CC)
CC = gcc
CFLAGS = -Wall -std=c17 -Wpedantic -g -s
ifeq ($(OS),Windows_NT)
	LDFLAGS = -lglfw3 -lglad -lgdi32 -lopengl32 -lcglm
else
	LDFLAGS = -lglfw3 -lglad -lX11 -lopengl32 -lcglm
endif

INC = include/
SRC = src/
BIN = bin/

LIB_SRC_FILES = $(wildcard src/GL*.c)
LIB_OBJ_FILES = $(patsubst src/GL%.c,bin/GL%.o,$(LIB_SRC_FILES))

all: lib exec clean

# ----------------------- LIBRARY STARTS HERE ------------------------------------

# override the implicit compilation rules for executables so that we can
# override them with new ones
%: %.o
%: %.c

#GET DYNAMIC LINKING WORKING ON WINDOWS!!!!!!!!!!
#FOR UNIX THIS SHOULD BE CONFIGURED TO SPIT OUT A .SO FILE INSTEAD OF .a/.DLL
lib: slib
slib: $(BIN)libglengine.a clean
dlib: $(BIN)libglengine.dll clean
llib: $(BIN)libglengine.so clean

#.so
$(BIN)libglengine.so: $(LIB_OBJ_FILES)
	$(CC) -shared -o $(BIN)libglengine.so $(LIB_OBJ_FILES) $(LDFLAGS)

#.dll
$(BIN)libglengine.dll: $(LIB_OBJ_FILES)
	$(CC) -shared -o $(BIN)libglengine.dll $(LIB_OBJ_FILES) $(LDFLAGS)

#.a
$(BIN)libglengine.a: $(LIB_OBJ_FILES)
	ar rc $(BIN)libglengine.a $(LIB_OBJ_FILES)

#Compiles all files named GL*.c in src/ into object files, places all corresponding GL*.o files in bin/
$(BIN)GL%.o: $(SRC)GL%.c $(INC)GL*.h $(INC)GLheader.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INC) -c -fpic $< -o $@ -DLOG_USE_COLOR

#LIBRARIES (.dll/.so/.a) SHOULD BE LOCATED IN "/bin" FOR EASIER ACCESS (BY DEFAULT WRITTEN TO "/bin")
#CAN PUT IN LINKER LIBRARY PATH IF YOU WANT TO

# ------------------------- EXECUTABLE STARTS HERE ---------------------------------

#CHANGE PROGRAM NAME HERE
#FOR UNIX LINK WITH .SO FILE INSTEAD OF DLL FILE
# create executables by linking %.o files
exec: compile clean

compile: $(BIN)main.o
	$(CC) $(CFLAGS) -L$(BIN) -o main $(BIN)main.o $(LDFLAGS) -lglengine

# create .o files by compiling c files with header files in $(INC)
$(BIN)main.o : $(SRC)main.c
	$(CC) -I$(INC) -c -fpic $< $(CFLAGS) $(LDFLAGS) -o $@

# ------------------------- CLEAN FUNCTION -----------------------------------------

#adjust clean to work for both windows and unix based operating systems
clean:
ifeq ($(OS),Windows_NT)
	del bin\\*.o
else
	rm -rf $(BIN)*.o
endif

emptybin:
ifeq ($(OS),Windows_NT)
	del bin\\*.o
	del bin\\*.a
	del bin\\*.dll
	del bin\\*.so
else
	rm -rf $(BIN)*.o
	rm -rf $(BIN)*.a
	rm -rf $(BIN)*.so
endif