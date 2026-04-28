CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
TARGET := lab_app

SOURCES := \
	ui/append_ui.cpp \
	ui/concat_ui.cpp \
	ui/create_sequence_ui.cpp \
	ui/insert_at_ui.cpp \
	main.cpp \
	ui/map_ui.cpp \
	ui/prepend_ui.cpp \
	ui/print_sequences_ui.cpp \
	ui/reduce_ui.cpp \
	ui/run_tests_ui.cpp \
	ui/sequence_ui_utils.cpp \
	ui/slice_ui.cpp \
	ui/subsequence_ui.cpp \
	tests.cpp \
	ui/ui.cpp \
	ui/where_ui.cpp \
	ui/zip_ui.cpp \
	ui/unzip_ui.cpp

OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
