# Paths
INC_DIR = include
SRC_DIR = src
GUI_DIR = gui
OBJ_DIR = obj
LIB_DIR = lib
PUGIXML_DIR = vendor/pugixml
INCLUDE_DIRS = -I $(INC_DIR) -I $(INC_DIR)/entities -I $(INC_DIR)/readers -I $(INC_DIR)/exporters -I $(PUGIXML_DIR)

# Compiler and linker
CXX = g++
CXXFLAGS = -std=c++11 -g -O3 -Wall -fPIC $(INCLUDE_DIRS)
LDFLAGS = -pthread -L$(LIB_DIR)

# Qt flags
QT_CXXFLAGS = $(shell pkg-config --cflags Qt5Widgets)
QT_LDFLAGS = $(shell pkg-config --libs Qt5Widgets)

# Source files
SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS)) $(OBJ_DIR)/vendor/pugixml/pugixml.o

# GUI files
GUI_SOURCES = $(wildcard $(GUI_DIR)/*.cpp)
GUI_OBJECTS = $(patsubst $(GUI_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(GUI_SOURCES))

# UI files
UI_FILES = $(wildcard $(GUI_DIR)/*.ui)
UI_HEADERS = $(patsubst $(GUI_DIR)/%.ui, $(OBJ_DIR)/ui_%.h, $(UI_FILES))

# MOC files
MOC_HEADERS = $(wildcard $(GUI_DIR)/*.h)
MOC_SOURCES = $(patsubst $(GUI_DIR)/%.h, $(OBJ_DIR)/moc_%.cpp, $(MOC_HEADERS))
MOC_OBJECTS = $(patsubst $(OBJ_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MOC_SOURCES))

# Targets
TARGET_REDATAM = redatam
TARGET_REDATAM_LIB = libredatam.a
TARGET_GUI = redatamgui

# Rules
all: $(TARGET_REDATAM) $(TARGET_GUI)
nogui: $(TARGET_REDATAM)

# Build redatam
$(OBJ_DIR)/vendor/pugixml/pugixml.o: $(PUGIXML_DIR)/pugixml.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_REDATAM): $(OBJS) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build redatam as a static library
$(TARGET_REDATAM_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIB_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

# Build redatamgui
$(TARGET_GUI): $(filter-out $(OBJ_DIR)/main.o, $(GUI_OBJECTS)) $(TARGET_REDATAM_LIB) $(OBJ_DIR)/gui_main.o $(UI_HEADERS) $(MOC_OBJECTS)
	$(CXX) $(filter-out $(OBJ_DIR)/main.o, $(GUI_OBJECTS)) $(OBJ_DIR)/gui_main.o $(MOC_OBJECTS) -L$(LIB_DIR) -lredatam $(LDFLAGS) $(QT_LDFLAGS) -o $@

$(OBJ_DIR)/gui_main.o: $(GUI_DIR)/main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(GUI_DIR)/%.cpp $(UI_HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/ui_%.h: $(GUI_DIR)/%.ui
	@mkdir -p $(dir $@)
	uic $< -o $@

$(OBJ_DIR)/moc_%.cpp: $(GUI_DIR)/%.h
	@mkdir -p $(dir $@)
	moc $(QT_CXXFLAGS) $< -o $@

$(OBJ_DIR)/moc_%.o: $(OBJ_DIR)/moc_%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET_REDATAM) $(TARGET_GUI)

.PHONY: all clean nogui