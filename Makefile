# CXX = [c++], clang++ g++-10
# OPT = [0], 1, 2, 3, f
# SAN = [1], 0

SRC_DIR = src
BUILD_DIR = build
CACHE_DIR = .cache
BIN = asteroid-arena-3d
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

CPPFLAGS += -MMD

CXXFLAGS += -std=c++14

ifeq ($(CXX), g++-10)
	CXXFLAGS += -Wall \
		-Wextra \
		-pedantic
else
	CXXFLAGS += -Weverything \
		-Wno-deprecated-declarations \
		-Wno-c++98-compat \
		-Wno-c++98-compat-pedantic \
		-Wno-poison-system-directories \
		-Wno-padded \
		-Wno-exit-time-destructors \
		-Wno-global-constructors \
		-Wno-shadow \
		-Wno-undef
endif

ifeq ($(OPT), 1)
	CXXFLAGS += -O1
else ifeq ($(OPT), 2)
	CXXFLAGS += -O2
else ifeq ($(OPT), 3)
	CXXFLAGS += -O3
else ifeq ($(OPT), f)
	CXXFLAGS += -Ofast
else
	CXXFLAGS += -g -O0
endif

ifneq ($(SAN), 0)
ifeq ($(CXX), c++)
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif
endif

ifeq ($(shell uname), Darwin)
	LDFLAGS += -framework GLUT -framework OpenGL -framework Carbon
else
	LDFLAGS += -lglut -lGLU -lGL
endif

$(shell mkdir -p $(BUILD_DIR))

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN) $(BUILD_DIR) $(CACHE_DIR)

-include $(DEP)
