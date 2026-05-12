#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int ROWS = 13;
const int COLS = 25;
const int CELL_SIZE = 30;

int main() {
    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Map Editor 13x21");
    window.setFramerateLimit(60);

    // Inicializa matriz com zeros (caminho)
    std::vector<std::vector<int>> map(ROWS, std::vector<int>(COLS, 0));

    // Retângulos para desenhar células
    std::vector<sf::RectangleShape> cells;
    cells.reserve(ROWS * COLS);

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            rect.setPosition(c * CELL_SIZE, r * CELL_SIZE);
            rect.setFillColor(sf::Color::White);
            cells.push_back(rect);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Fechar janela
            if (event.type == sf::Event::Closed)
                window.close();

            // Clique do mouse
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / CELL_SIZE;
                int y = event.mouseButton.y / CELL_SIZE;
                if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
                    // Alterna entre 0 e 1
                    map[y][x] = 1 - map[y][x];
                    cells[y * COLS + x].setFillColor(map[y][x] ? sf::Color::Black : sf::Color::White);
                }
            }

            // Tecla pressionada
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    // Imprime a matriz no console
                    std::cout << "\nMapa gerado:\n";
                    for (int r = 0; r < ROWS; ++r) {
                        std::cout << " \" ";
                        for (int c = 0; c < COLS; ++c) {
                            std::cout << map[r][c];
                        }
                        std::cout << "\",\n";
                    }
                    std::cout << "\nCopie o mapa acima.\n";
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::White);

        for (auto& cell : cells)
            window.draw(cell);

        window.display();
    }

    return 0;
}
