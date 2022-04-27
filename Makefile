# URG_CPP_LIB = /usr/local/lib/liburg_cpp.a
# CXXFLAGS = -O2 $(INCLUDES) -I /usr/local/include/urg_cpp 

# Command:  g++ -O2 -I /usr/local/include/urg_cpp opencv_test.cpp Connection_information.o /usr/local/lib/liburg_cpp.a -o opencv_test `pkg-config --cflags --libs opencv4`

# Name of the project
PROJ_NAME=opencv_test main
# $(PROJ_NAME) : Connection_information.o  $(URG_CPP_LIB)

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS= -c	\
		  -O2 \
		  -I /usr/local/include/urg_cpp

# Command used at clean target
RM = rm

OPENCV = `pkg-config --cflags --libs opencv4`
LIBS = $(OPENCV)

urg_flags = Connection_information.o /usr/local/lib/liburg_cpp.a
# urg_flags = Connection_information.o URG_CPP_LIB 
#
# Compilation and linking
#
all: $(PROJ_NAME)

$(PROJ_NAME): $(PROJ_NAME).cpp
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $(CC_FLAGS) $(urg_flags) $@.cpp -o $@.o $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

clean:
	@ $(RM) $(PROJ_NAME).o

.PHONY: all clean
