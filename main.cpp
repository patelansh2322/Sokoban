// Copyright [2025] Ansh Patel

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    // Command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <level_file>\n";
        return 1;
    }

    // Load file
    std::ifstream levelFile(argv[1]);
    if (!levelFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    // Read the map from file
    SB::Sokoban game(argv[1]);
    levelFile >> game;
    levelFile.close();

    game.setInitialState();

    // Create window dimensions
    unsigned int windowWidth = game.width() * SB::Sokoban::TILE_SIZE;
    unsigned int windowHeight = (game.height() * SB::Sokoban::TILE_SIZE)+50;

    // Font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Unable to load font\n";
    }

    // Victory audio
    sf::Music victory;
    if (!victory.openFromFile("victory.wav")) {
        std::cerr << "Error: Unable to load victory audio\n";
    }

    // Create text for moveCount
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, windowHeight-40);

    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(36);
    winText.setFillColor(sf::Color::Green);
    winText.setString("You Win!");
    winText.setPosition(windowWidth / 2 - winText.getLocalBounds().width / 2,
    windowHeight / 2 - winText.getLocalBounds().height / 2);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sokoban Game");

    bool gameWon = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    game.reset();
                    gameWon = false;
                }
                if (!gameWon) {
                    switch (event.key.code) {
                        case sf::Keyboard::W: game.movePlayer(SB::Direction::Up); break;
                        case sf::Keyboard::A: game.movePlayer(SB::Direction::Left); break;
                        case sf::Keyboard::S: game.movePlayer(SB::Direction::Down); break;
                        case sf::Keyboard::D: game.movePlayer(SB::Direction::Right); break;
                        case sf::Keyboard::Z: game.undo(); break;
                        case sf::Keyboard::X: game.redo(); break;
                        default: break;
                    }
                }
                text.setString("Moves: " + std::to_string(game.count()));
            }
        }

        if (!gameWon && game.isWon()) {
            gameWon = true;
            victory.play();
        }

        window.clear();
        if (gameWon) {
            window.draw(winText);
        } else {
            window.draw(text);
            window.draw(game);
        }
        window.display();
    }
    // Print the map on terminal
    std::cout << game << std::endl;
    return 0;
}
