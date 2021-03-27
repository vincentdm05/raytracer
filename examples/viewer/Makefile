
##---------------------------------------------------------------------
## DEFINITIONS
##---------------------------------------------------------------------

PROJECT_NAME = viewer
PROJECT_ROOT = ../../
EXE = $(PROJECT_ROOT)bin/$(PROJECT_NAME)
BUILD_DIR = $(PROJECT_ROOT)build/$(PROJECT_NAME)/
SOURCES = $(addsuffix .cpp, $(PROJECT_NAME))
HEADERS = $(shell find $(PROJECT_ROOT) -type f -name '*.hpp')
OBJS = $(addprefix $(BUILD_DIR), $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXX = g++
CXXFLAGS = -I$(PROJECT_ROOT)src
CXXFLAGS += -O3 -g -Wall -Wformat -std=c++11
LIBS =

# OpenGL loader
LIBS += -lGLEW
# OpenGL
LIBS += -framework OpenGL
# Window and input lib
LIBS += -lglfw

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(BUILD_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# Depend on build dir existence, and rebuild if any header is out of date
$(OBJS): $(HEADERS) | $(BUILD_DIR)

# Create build dir
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	rm -f $(EXE) $(OBJS)
