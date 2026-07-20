CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

SRC = src/lista_compras.cpp src/similaridade.cpp src/recomendacao.cpp

all: teste_atividade1 teste_atividade2 teste_atividade3

teste_atividade1: testadores/teste_atividade1.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

teste_atividade2: testadores/teste_atividade2.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

teste_atividade3: testadores/teste_atividade3.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f teste_atividade1 teste_atividade2 teste_atividade3

.PHONY: all clean
