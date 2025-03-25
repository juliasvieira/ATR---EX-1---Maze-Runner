#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <conio.h> // Para capturar teclas pressionadas
#include <thread>
#include <chrono>

using Maze = std::vector<std::vector<char>>;

struct Position {
    int row;
    int col;
};

Maze maze;
Maze original_maze; // Para armazenar o labirinto original
int num_rows;
int num_cols;
Position p_pos;
std::stack<Position> valid_positions; 

// 1. Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return {-1, -1};
    }
    
    file >> num_rows >> num_cols;
    maze.resize(num_rows, std::vector<char>(num_cols));
    original_maze.resize(num_rows, std::vector<char>(num_cols)); // Redimensionando o labirinto original
    Position start{-1, -1};
    
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            original_maze[i][j] = maze[i][j]; // Copiando o labirinto original
            if (maze[i][j] == 'e') {
                start = {i, j};
                maze[i][j] = 'o'; // Marca posição inicial
                valid_positions.push(start); // Adiciona a posição inicial à pilha
            }
        }
    }
    
    file.close();
    return start;
}

// 2. Função para imprimir o labirinto
void print_maze() {
    system("cls"); // Limpa o console (no Windows)
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\nUse as setas para mover. Pressione 'q' para sair." << std::endl;
}

// 3. Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols && maze[row][col] != '#');
}

// 4. Função principal para movimentação do jogador
void walk() {
    print_maze();  // Exibe o labirinto original na primeira vez

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Pequeno delay para melhorar a exibição
        char key = _getch(); // Captura tecla pressionada
        int new_row = p_pos.row;
        int new_col = p_pos.col;
        
        switch (key) {
            case 72: new_row--; break; // Seta para cima
            case 80: new_row++; break; // Seta para baixo
            case 75: new_col--; break; // Seta para esquerda
            case 77: new_col++; break; // Seta para direita
            case 'q': return; // Sair do jogo
        }
        
        if (is_valid_position(new_row, new_col)) {
            if (maze[new_row][new_col] == 's') {
                print_maze();
                std::cout << "Saida encontrada!" << std::endl;
                return;
            }
            
            // Marca a posição anterior como explorada
            maze[p_pos.row][p_pos.col] = '.';
            
            // Atualiza a nova posição do jogador
            p_pos = {new_row, new_col};
            maze[p_pos.row][p_pos.col] = 'o'; // Atualiza nova posição como 'o'
            valid_positions.push(p_pos); // Adiciona a nova posição à pilha

            print_maze(); // Exibe o labirinto atualizado
        }
    }
}

int main() {
    p_pos = load_maze("labirinto.txt");
    if (p_pos.row == -1 || p_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }
    
    walk();
    return 0;
}
