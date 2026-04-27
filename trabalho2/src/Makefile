# Nome do executável
TARGET = programa

# Compilador
CXX = g++

# Flags
CXXFLAGS = -Wall -std=c++11 -Ihash -Ileitura -Istructs

# Arquivos fonte
SRCS = main.cpp $(wildcard hash/*.cpp) $(wildcard leitura/*.cpp)

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

# Apaga os buckets de texto gerados no teste 
clean:
	rm -f $(OBJS) $(TARGET) bucket_*.txt diretorio.txt
