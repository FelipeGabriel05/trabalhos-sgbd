# Nome do executável
TARGET = programa

# Compilador
CXX = g++

# Flags (AGORA com include das pastas)
CXXFLAGS = -Wall -std=c++11 -Ibuffer -Ileitura -Istructs

# Arquivos fonte
SRCS = main.cpp $(wildcard buffer/*.cpp) $(wildcard leitura/*.cpp) $(wildcard structs/*.cpp)

# Objetos
OBJS = $(SRCS:.cpp=.o)

# Regra principal
all: $(TARGET)

# Linkagem
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilação
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)