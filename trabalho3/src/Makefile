# Nome do executável
TARGET = programa

# Compilador
CXX = g++

# Flags
CXXFLAGS = -Wall -std=c++11 \
           -Ibuffer \
           -Ileitura \
           -Istructs \
           -Isort \
           -Idisco

# Arquivos fonte
SRCS = main.cpp \
       $(wildcard buffer/*.cpp) \
       $(wildcard leitura/*.cpp) \
       $(wildcard structs/*.cpp) \
       $(wildcard sort/*.cpp) \
       $(wildcard disco/*.cpp)

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