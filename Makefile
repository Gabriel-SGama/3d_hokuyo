# URG_CPP_LIB = /usr/local/lib/liburg_cpp.a
# CXXFLAGS = -O2 $(INCLUDES) -I /usr/local/include/urg_cpp 

# Command:  g++ -O2 -I /usr/local/include/urg_cpp opencv_test.cpp Connection_information.o /usr/local/lib/liburg_cpp.a -o opencv_test `pkg-config --cflags --libs opencv4`

PROJ_NAME = hokuyo
# .cpp files
# CPP_SOURCE=$(wildcard ./src/*.cpp)
CPP_SOURCE= src/main.cpp src/Hokuyo.cpp src/Viewer.cpp

# .h files
H_SOURCE=$(wildcard ./src/*.h)
all:
	g++ -O2 -I /usr/local/include/urg_cpp $(CPP_SOURCE) Connection_information.o /usr/local/lib/liburg_cpp.a -o $(PROJ_NAME) `pkg-config --cflags --libs opencv4`

clean:
	@ $(RM) $(PROJ_NAME)


# $(PROJ_NAME): $(OBJ)
# 	@ echo 'Building binary using GCC linker: $@'
# 	$(CC) $^ $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo 'Finished building binary: $@'
# 	@ echo ' '

# ./objects/%.o: ./src/%.cpp ./src/headers/%.h
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo ' '

# ./objects/main.o: ./src/main.cpp $(H_SOURCE)
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) $< Connection_information.o $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo ' '


# # Name of the project
# PROJ_NAME= 
# # $(PROJ_NAME) : Connection_information.o  $(URG_CPP_LIB)

# # Compiler and linker
# CC=g++

# # Flags for compiler
# CC_FLAGS= -c	\
# 		  -O2 \
# 		  -I /usr/local/include/urg_cpp

# # Command used at clean target
# RM = rm


# OPENCV = `pkg-config --cflags --libs opencv4`
# LIBS = $(OPENCV)

# urg_flags = Connection_information.o /usr/local/lib/liburg_cpp.a
# # urg_flags = Connection_information.o URG_CPP_LIB 
# #
# # Compilation and linking
# #
# all: $(PROJ_NAME)

# $(PROJ_NAME): $(PROJ_NAME).cpp
# 	@ echo 'Building binary using GCC linker: $@'
# 	$(CC) $(CC_FLAGS) $(urg_flags) $@.cpp -o $@.o $(LIBS)
# 	@ echo 'Finished building binary: $@'
# 	@ echo ' '

# clean:
# 	@ $(RM) $(PROJ_NAME).o

# .PHONY: all clean

# Name of the project
# PROJ_NAME=hokuyo.o

# # .cpp files
# CPP_SOURCE=$(wildcard ./src/*.cpp)

# # .h files
# H_SOURCE=$(wildcard ./src/*.h)

# # Object files
# OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))

# # Compiler and linker
# CC=g++

# # Flags for compiler
# CC_FLAGS= -c	\
# 		  -O2 \
# 		  -I /usr/local/include/urg_cpp

# # Command used at clean target
# RM = rm -rf

# OPENCV = `pkg-config --cflags --libs opencv4`
# LIBS = $(OPENCV)

# #
# # Compilation and linking
# #
# all: objFolder $(PROJ_NAME)

# $(PROJ_NAME): $(OBJ)
# 	@ echo 'Building binary using GCC linker: $@'
# 	$(CC) $^ $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo 'Finished building binary: $@'
# 	@ echo ' '

# ./objects/%.o: ./src/%.cpp ./src/headers/%.h
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo ' '

# ./objects/main.o: ./src/main.cpp $(H_SOURCE)
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) $< Connection_information.o $(CC_FLAGS) -o $@ $(LIBS)
# 	@ echo ' '

# objFolder:
# 	@ mkdir -p objects


# .PHONY: all clean


