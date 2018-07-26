objects = shortest_path_main.o shortest_path.o
CXX = g++
CXXFLAGS = -std=c++0x

sp: $(objects)
	$(CXX) $(CXXFLAGS) -o sp $(objects)

shortest_path_main.o: shortest_path.hpp
shortest_path.o: shortest_path.hpp

clean:
	rm *.o 
	rm -f sp