CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRCDIR = src
OBJDIR = build
TESTDIR = tests
LIBDIR = libs

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.obj)
TESTSOURCES = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJECTS = $(TESTSOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/%.obj)

TARGET = foip_direct_demo
TESTLIBS = $(OBJDIR)/logger.obj $(OBJDIR)/pjsip_stub.obj $(OBJDIR)/spandsp_stub.obj

.PHONY: all clean test directories

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(OBJDIR)/%.obj: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.obj: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: directories $(TESTLIBS) $(TESTOBJECTS)
	$(CXX) $(OBJDIR)/test_logger.obj $(TESTLIBS) -o test_logger
	$(CXX) $(OBJDIR)/test_integration.obj $(TESTLIBS) -o test_integration
	./test_logger
	./test_integration

clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	rm -f test_logger test_integration

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/