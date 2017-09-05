#include "game.h"
#include <iostream>
//#include <ctime>

Game::Game():
    window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)
{
    font_.loadFromFile(PATH_TO_FONT);
    textureCards_.loadFromFile(PATH_TO_IMAGE);
    textureCards_.setSmooth(true);
    spriteCards_.setTexture(textureCards_);
    fillGameFieldRandomly();
}

void Game::run()
{
    while (window_.isOpen()) {
        inputPhase();
        drawingPhase();
    }
}

void Game::newGame()
{
    numSelectedCards_ = 0;
    indexSelectedCard1_ = 0;
    indexSelectedCard2_ = 0;
    fillGameFieldRandomly();
    isPlaying_ = true;
}

void Game::fillGameFieldRandomly()
{
    cards_.clear();
    std::uniform_int_distribution<int> distribution_{0,NUM_CARD_TYPES - 1};

    for(int y{0}; y < FIELD_HEIGHT; ++y)
    {
        for(int x{0}; x < FIELD_WIDTH; ++x)
        {
            bool isRandomIdOK = false;
            do
            {
                int rndNum = distribution_(mt);
                 if(countEntries(cards_, rndNum) < NUM_SELECTED_CARDS_MAX)
                {
                    Card newCard{rndNum, x, y, false};
                    isRandomIdOK = true;
                    cards_.push_back(newCard);
                }
            }
            while(!isRandomIdOK);
        }
    }
}

void Game::inputPhase()
{
    sf::Event event;
    while(window_.pollEvent(event))
    {
         if(event.type == sf::Event::Closed)
         {
             window_.close();
         }
         if(event.type == sf::Event::KeyReleased)
         {
             switch(event.key.code)
             {
                 case sf::Keyboard::N:
                     newGame();
                     break;
                 case sf::Keyboard::Escape:
                 case sf::Keyboard::Q:
                     window_.close();
                     break;
                 default:
                     break;
             }
         }
         if(event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Button::Left)
         {
             if(isPlaying_)
             {
                 int clickedCardX = event.mouseButton.x / FRAME_WIDTH;
                 int clickedCardY = event.mouseButton.y / FRAME_HEIGHT;
                 //std::cout << "clickedCardX = " << clickedCardX << std::endl;
                 //std::cout << "clickedCardY = " << clickedCardY << std::endl;
                 for(unsigned short int i{0}; i < cards_.size(); ++i)
                 {
                     if(cards_[i].x == clickedCardX &&
                        cards_[i].y == clickedCardY)
                     {
                         switch(numSelectedCards_)
                         {
                             case 0:
                                 ++numSelectedCards_;
                                 cards_[i].isShown = true;
                                 indexSelectedCard1_ = i;
                                 break;
                             case 1:
                                 if(i != indexSelectedCard1_)
                                 {
                                     ++numSelectedCards_;
                                     cards_[i].isShown = true;

                                     indexSelectedCard2_ = i;
                                     if(cards_[indexSelectedCard1_].id ==
                                        cards_[indexSelectedCard2_].id)
                                     {
                                         numSelectedCards_ = 0;
                                         std::cout << "Match!" << std::endl;
                                     }
                                 }
                                 else
                                 {
                                     std::cout << "Same card was can not be choosen1!" << std::endl;
                                 }
                                 break;
                             case NUM_SELECTED_CARDS_MAX :
                                 if(i != indexSelectedCard1_ && i != indexSelectedCard2_)
                                 {
                                     numSelectedCards_ = 0;
                                     cards_[indexSelectedCard1_].isShown = false;
                                     cards_[indexSelectedCard2_].isShown = false;
                                     std::cout << "Don't Match" << std::endl;
                                 }
                                 else
                                 {
                                     std::cout << "Same card was can not be choosen2!" << std::endl;
                                 }
                                 break;
                             default:
                                 break;
                         }
                         break;
                     }
                 }
                 std::cout << "numSelectedCards_ = " << numSelectedCards_ << std::endl;
             }
         }
    }
}

bool Game::checkWin()
{
    for(const auto &card: cards_)
    {
        if(card.isShown == false)
            return false;
    }
    isPlaying_ = false;
    return true;
}

void Game::drawingPhase()
{
    window_.clear();

    if(checkWin())
    {
        drawCongratulations();
    }
    else
    {
        for(const auto &card: cards_)
        {
            if(card.isShown)
            {
                spriteCards_.setTextureRect(sf::IntRect(card.id * FRAME_WIDTH, 0,
                                                       FRAME_WIDTH, FRAME_HEIGHT));
            }
            else
            {
                spriteCards_.setTextureRect(sf::IntRect(CLOSED_CARD_POSITION *
                                                        FRAME_WIDTH, 0, FRAME_WIDTH,
                                                        FRAME_HEIGHT));
            }
            spriteCards_.setPosition(FRAME_WIDTH * card.x, FRAME_WIDTH * card.y);
            window_.draw(spriteCards_);
        }
    }
    window_.display();
}

void Game::drawCongratulations()
{
    sf::Text text("Congratulations!", font_, 48);
    text.setColor(sf::Color::Yellow);
    text.setPosition((WINDOW_WIDTH - text.getGlobalBounds().width) / 2,
                     (WINDOW_HEIGHT - text.getGlobalBounds().height) / 2);
    window_.draw(text);
}

int Game::countEntries(const std::vector<Card>& cards, int card_id)
{
    int counter{0};
    for(const auto &c: cards)
    {
        if(c.id == card_id)
            ++counter;
    }
    return counter;
}
