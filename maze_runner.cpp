#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

using Maze = std::vector<std::vector<char>>;

struct Position {
    int row;
    int col;
};

Maze maze;
int num_rows;
int num_cols;
Position start_pos;
std::stack<Position> path_stack;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return {-1, -1};
    }
    
    file >> num_rows >> num_cols;
    maze.resize(num_rows, std::vector<char>(num_cols));
    Position start{-1, -1};
    
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            if (maze[i][j] == 'e') {
                start = {i, j};
                maze[i][j] = 'o';
                path_stack.push(start);
            }
        }
    }
    
    file.close();
    return start;
}

// Função para imprimir o labirinto
void print_maze() {
    system("cls"); // Para Windows
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Pequeno delay
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's'));
}

// Função principal para a busca automática da saída
bool walk(Position pos) {
    while (!path_stack.empty()) {
        Position current = path_stack.top();
        path_stack.pop();

        // Se encontrou a saída, termina a busca
        if (maze[current.row][current.col] == 's') {
            print_maze();
            std::cout << "Saida encontrada!" << std::endl;
            return true;
        }
        
        // Marca a posição atual como explorada
        if (maze[current.row][current.col] != 'o') {
            maze[current.row][current.col] = '.';
        }
        
        print_maze();

        // Movimentos: baixo, direita, cima, esquerda
        std::vector<Position> moves = {
            {current.row + 1, current.col},
            {current.row, current.col + 1},
            {current.row - 1, current.col},
            {current.row, current.col - 1}
        };
        
        for (const auto& move : moves) {
            if (is_valid_position(move.row, move.col)) {
                path_stack.push(move);
                maze[move.row][move.col] = 'o'; // Marca a posição explorada
            }
        }
    }
    return false;
}

int main() {
    start_pos = load_maze("labirinto.txt");
    if (start_pos.row == -1 || start_pos.col == -1) {
        std::cerr << "Posicao inicial nao encontrada no labirinto." << std::endl;
        return 1;
    }
    
    bool found_exit = walk(start_pos);
    if (!found_exit) {
        std::cout << "Nao foi possivel encontrar a saida." << std::endl;
    }
    
    return 0;
}
