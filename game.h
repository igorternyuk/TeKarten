#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <random>

class Game
{
public:
    explicit Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
    void run();

private:
    const std::string WINDOW_TITLE{"Karten"};
    const std::string PATH_TO_IMAGE{"resources/gfx/set500.png"};
    const std::string PATH_TO_FONT{"resources/fonts/orbitron-light.ttf"};
    enum
    {
        FIELD_WIDTH = 6,
        FIELD_HEIGHT = 4,
        FRAME_WIDTH = 80,
        FRAME_HEIGHT = 80,
        WINDOW_WIDTH = FIELD_WIDTH * FRAME_WIDTH,
        WINDOW_HEIGHT = FIELD_HEIGHT * FRAME_HEIGHT,
        NUM_CARD_TYPES = 12,
        NUM_SELECTED_CARDS_MAX = 2,
        CLOSED_CARD_POSITION = 12
    };

    struct Card
    {
        int id;
        int x, y;
        bool isShown;
    };

    sf::RenderWindow window_;
    sf::Sprite spriteCards_;
    sf::Texture textureCards_;
    sf::Font font_;
    std::vector<Card> cards_;
    std::random_device rd;
    std::mt19937 mt{rd()};
    int numSelectedCards_{0};
    int indexSelectedCard1_{0},
        indexSelectedCard2_{0};
    bool isPlaying_{true};
    void newGame();
    void fillGameFieldRandomly();
    void inputPhase();
    bool checkWin();
    void drawingPhase();
    void drawCongratulations();
    int countEntries(const std::vector<Card>& cards_, int card_id);
};

