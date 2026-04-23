# Makefile pour le jeu de plateforme
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -lsfml-graphics -lsfml-audio -lsfml-system -lsfml-window

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = JeuPlateforme

all: $(TARGET)

$(TARGET): $(OBJECTS)
$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
rm -f $(SRCDIR)/*.o $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
