CXX := g++
CXX_FLAGS := -std=c++17

BIN := bin
SRC := src
INCLUDE := include

LIBRARIES := 
EXECUTABLE := app

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX)  $(CXX_FLAGS) -I$(INCLUDE)   $^  -o $@ `wx-config --cxxflags --libs` $(LIBRARIES)
	
clean:
	-rm $(BIN)/*