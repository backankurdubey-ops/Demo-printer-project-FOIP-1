# Makefile for FOIP Direct Demo Project v1.1

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRCDIR = src
INCDIR = include
BUILDDIR = build
LIBSDIR = libs
TESTSDIR = tests

# Source files
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/pjsip_stub.cpp $(SRCDIR)/spandsp_stub.cpp $(SRCDIR)/logger.cpp
OBJECTS = $(BUILDDIR)/main.obj $(BUILDDIR)/pjsip_stub.obj $(BUILDDIR)/spandsp_stub.obj $(BUILDDIR)/logger.obj

# Test sources
TEST_SOURCES = $(TESTSDIR)/test_logger.cpp $(SRCDIR)/logger.cpp
TEST_OBJECTS = $(BUILDDIR)/test_logger.obj $(BUILDDIR)/logger_test.obj

# Target executable
TARGET = foip_direct_demo
TEST_TARGET = test_logger

.PHONY: all clean test directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(LIBSDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(BUILDDIR)/%.obj: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: directories $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(BUILDDIR)/test_logger.obj $(BUILDDIR)/logger_test.obj
	$(CXX) $^ -o $(TEST_TARGET)

$(BUILDDIR)/test_logger.obj: $(TESTSDIR)/test_logger.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/logger_test.obj: $(SRCDIR)/logger.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(LIBSDIR) $(TARGET) $(TEST_TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.SUFFIXES: .cpp .obj
