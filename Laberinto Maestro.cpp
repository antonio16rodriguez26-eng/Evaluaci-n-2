# Evaluaci-n-2
Laberinto Maestro 
#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <chrono>

// Definiciones para la representación del laberinto
const char WALL = '#';
const char PATH = ' ';
const char ENTRY = 'E';
const char EXIT = 'S';
const char SOLUTION = '*';

// Clase para representar un punto en el laberinto (coordenadas x, y)
struct Point {
    int x, y;
};

// Función para generar un laberinto utilizando el algoritmo de backtracking recursivo.
// Este método asegura que el laberinto siempre tendrá un camino válido.
void generateMaze(std::vector<std::vector<char>>& maze, int x, int y, std::mt19937& gen) {
    // Definir las 4 direcciones posibles
    std::vector<Point> directions = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};
    
    // Mezclar las direcciones para un resultado aleatorio
    std::shuffle(directions.begin(), directions.end(), gen);

    for (const auto& dir : directions) {
        int newX = x + dir.x;
        int newY = y + dir.y;

        // Verificar si la nueva posición está dentro de los límites del laberinto
        if (newX >= 0 && newY >= 0 && newX < maze.size() && newY < maze[0].size() && maze[newX][newY] == WALL) {
            // Carvar un camino entre el punto actual y el nuevo
            maze[x + dir.x / 2][y + dir.y / 2] = PATH;
            maze[newX][newY] = PATH;
            // Llamada recursiva para continuar el camino desde el nuevo punto
            generateMaze(maze, newX, newY, gen);
        }
    }
}

// Función para imprimir el laberinto en la consola
void printMaze(const std::vector<std::vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
}

// Función para resolver el laberinto usando el algoritmo de backtracking recursivo (DFS).
// Devuelve true si encuentra una solución, de lo contrario false.
bool solveMaze(std::vector<std::vector<char>>& maze, int x, int y, int endX, int endY) {
    // Condición de salida: si la posición actual es la salida
    if (x == endX && y == endY) {
        return true;
    }

    // Condición de parada: si la posición está fuera de límites o es un muro o una celda ya visitada
    if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size() ||
        maze[x][y] == WALL || maze[x][y] == SOLUTION) {
        return false;
    }

    // Marcar la celda actual como parte de la ruta de la solución
    maze[x][y] = SOLUTION;

    // Probar las 4 direcciones (arriba, abajo, izquierda, derecha)
    if (solveMaze(maze, x + 1, y, endX, endY)) return true;
    if (solveMaze(maze, x - 1, y, endX, endY)) return true;
    if (solveMaze(maze, x, y + 1, endX, endY)) return true;
    if (solveMaze(maze, x, y - 1, endX, endY)) return true;

    // Si no se encuentra un camino desde esta posición, se hace un "backtracking"
    // y se desmarca la celda.
    maze[x][y] = PATH;
    
    return false;
}

int main(int argc, char* argv[]) {
    int rows = 11; // Por defecto 10x10 (matriz de 11x11 con paredes)
    int cols = 11;

    // Manejar argumentos para tamaño configurable
    if (argc >= 3) {
        rows = std::stoi(argv[1]) * 2 + 1;
        cols = std::stoi(argv[2]) * 2 + 1;
    }

    // Asegurar que las dimensiones sean impares para el algoritmo
    if (rows % 2 == 0) rows++;
    if (cols % 2 == 0) cols++;

    // Sembrar el generador de números aleatorios
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    // Crear la matriz del laberinto, inicializada con muros
    std::vector<std::vector<char>> maze(rows, std::vector<char>(cols, WALL));

    // Generar el laberinto desde la entrada (1, 1)
    maze[1][1] = PATH;
    generateMaze(maze, 1, 1, gen);

    // Definir la entrada y la salida
    maze[0][1] = ENTRY;
    maze[rows - 1][cols - 2] = EXIT;

    std::cout << "Laberinto generado:" << std::endl;
    printMaze(maze);
    std::cout << "\n";

    // Medir el tiempo de ejecución para la resolución
    auto start = std::chrono::high_resolution_clock::now();
    bool solved = solveMaze(maze, 1, 1, rows - 2, cols - 2);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (solved) {
        std::cout << "Laberinto resuelto:" << std::endl;
        maze[0][1] = ENTRY; // Asegurar que la entrada se vea bien
        maze[rows-1][cols-2] = EXIT; // Asegurar que la salida se vea bien
        printMaze(maze);
    } else {
        std::cout << "No se encontró una solución." << std::endl;
    }
    
    std::cout << "\nTiempo de ejecución de la resolución: " << duration.count() << " segundos" << std::endl;

    return 0;
}
