PROJ_NAME = hokuyo
# .cpp files
CPP_SOURCE= src/main.cpp src/Hokuyo.cpp src/Viewer.cpp src/Logic.cpp src/History.cpp

all:
	g++ -O2 -I /usr/local/include/urg_cpp $(CPP_SOURCE) Connection_information.o /usr/local/lib/liburg_cpp.a -o $(PROJ_NAME) `pkg-config --cflags --libs opencv4`

clean:
	@ $(RM) $(PROJ_NAME)


