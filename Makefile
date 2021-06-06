SRC_DIR = src
BUILD_DIR = build
CACHE_DIR = .cache
BIN = asteroid-arena-3d
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

CPPFLAGS += -MMD

CXXFLAGS += -std=c++17 -O2 -Wno-deprecated-declarations

LDFLAGS += -framework GLUT -framework OpenGL -framework Carbon 

$(shell mkdir -p $(BUILD_DIR))

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN) $(BUILD_DIR) $(CACHE_DIR)

-include $(DEP)
