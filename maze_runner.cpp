#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <algorithm>

using Maze = std::vector<std::vector<char>>;

struct Position {
    int row;
    int col;
};

Maze maze;
int num_rows;
int num_cols;
Position start_pos;             // posição de início
Position exit_pos;              // posição de saída
std::stack<Position> path_stack;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);                     // abrir o arquivo
    if (!file) {
        std::cerr << "\nErro ao abrir o arquivo!\n" << std::endl; 
        return {-1, -1};
    }

    file >> num_rows >> num_cols;              // leitura do número de linhas
    maze.resize(num_rows, std::vector<char>(num_cols)); // redimensionamento matriz
    Position start{-1, -1};

    for (int i = 0; i < num_rows; ++i) {           // leitura do labirinto (conteúdo)
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            if (maze[i][j] == 'e') {               // encontrando a posição inicial
                start = {i, j};
                maze[i][j] = 'o';
                path_stack.push(start);
            }
            if (maze[i][j] == 's') {
                exit_pos = {i, j}; // salva a posição da saída
            }
        }
    }

    file.close();          // fecha o arquivo
    return start;
}

// Função para imprimir o labirinto
void print_maze() {
    system("cls");
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // delay
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's'));
}

// Função principal para navegar pelo labirinto
bool walk() {
    Position previous = start_pos; 

    auto manhattan_distance = [](Position p) {
        return std::abs(p.row - exit_pos.row) + std::abs(p.col - exit_pos.col);
    };

   
    auto choose_best_move = [manhattan_distance](Position current) {   // escolher o melhor movimento em direção à saída
            std::vector<Position> moves = {
            {current.row + 1, current.col},  // baixo
            {current.row, current.col + 1},  // direita
            {current.row - 1, current.col},  // cima
            {current.row, current.col - 1}   // esquerda
        };

        // ordena os movimentos pela distância Manhattan em relacao a saida
        std::sort(moves.begin(), moves.end(), [manhattan_distance](Position a, Position b) {
            return manhattan_distance(a) < manhattan_distance(b);
        });

        // retorna o primeiro movimento valido que nos leva mais perto da saída
        for (const auto& move : moves) {
            if (is_valid_position(move.row, move.col)) {
                return move; 
            }
        }

        return current; 
    };

    while (!path_stack.empty()) {
        Position current = path_stack.top();
        path_stack.pop();

        if (maze[current.row][current.col] == 's') {
            print_maze();
            std::cout << "\nSaida encontrada!\n" << std::endl;
            return true; 
        }

        if (maze[previous.row][previous.col] != 's' && maze[previous.row][previous.col] != 'e') {
            maze[previous.row][previous.col] = '.'; // marcando posicoes ja exploradas
        }

        maze[current.row][current.col] = 'o';  //marcando posicao atual

        print_maze(); 
        
        previous = current;

        Position next_move = choose_best_move(current); // escolhe o melhor movimento em direcao à saída

        // adiciona o próximo movimento à pilha
        if (next_move.row != current.row || next_move.col != current.col) {
            path_stack.push(next_move);
        }
    }

    return false; 
}

int main() {
    start_pos = load_maze("maze.txt");
    if (start_pos.row == -1 || start_pos.col == -1) {
        std::cerr << "\nPosicao inicial nao encontrada no labirinto.\n" << std::endl;
        return 1;
    }

    bool found_exit = walk(); 
    if (!found_exit) {
        std::cout << "\nNao foi possível encontrar a saida.\n" << std::endl;
    }

    return 0;
}
