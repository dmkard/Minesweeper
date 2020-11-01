#include "Interface.h"

Interface::Interface(): base_color_(sf::Color(20, 20, 20))    
{
    setupTextLabels();
    setupBackground();
    setupResetButton();
    setupTitleText();
}

void Interface::setStopwatch(const int gameTime)
{
    stopwatch_.setString(std::to_string(gameTime));
}

void Interface::setMineAmount(const int mineAmount)
{
    mine_left_.setString(std::to_string(mineAmount));
}

sf::Color Interface::baseColor()
{
    return base_color_;
}

void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(stopwatch_);
    target.draw(mine_left_);
    target.draw(background_);
    target.draw(reset_button_);
    target.draw(title_);
}

void Interface::setupTextLabels()
{
    font_.loadFromFile("resources/font/resin.ttf");

    stopwatch_.setFont(font_);
    stopwatch_.setString(L"0");
    stopwatch_.setPosition(margin * 1.f, 2.f * margin + TILE_SIDE_SIZE * B_HEIGHT);
    //stopwatch_.setPosition(margin * 1.f, 2.f * margin + TILE_SIDE_SIZE * B_WIDTH);
    stopwatch_.setCharacterSize(50); // in pixels, not points!
    stopwatch_.setFillColor(sf::Color(235, 94, 40));

    mine_left_.setFont(font_);
    mine_left_.setString(std::to_string(MINE_AMOUNT));
    mine_left_.setPosition(margin * 10.f, 2.f * margin + TILE_SIDE_SIZE * B_HEIGHT);
    mine_left_.setCharacterSize(50); // in pixels, not points!
    mine_left_.setFillColor(sf::Color(235, 94, 40));
}

void Interface::setupBackground()
{
    float outlineThicknessCoefficient = 0.96f; // lower coefficient means lower thickness
    background_.setSize({ B_WIDTH * TILE_SIDE_SIZE + margin * (1 - outlineThicknessCoefficient),
                            B_HEIGHT * TILE_SIDE_SIZE + margin * (1 - outlineThicknessCoefficient) });
    background_.setPosition({ margin * outlineThicknessCoefficient,
                                margin * outlineThicknessCoefficient });
    background_.setFillColor(sf::Color(134, 130, 121));
}

void Interface::setupResetButton()
{
    button_texture_.loadFromFile("resources/pictures/has_mine_tile.png");
    reset_button_.setSize({ TILE_SIDE_SIZE * 1.f, TILE_SIDE_SIZE * 1.f });
    reset_button_.setPosition({ margin + TILE_SIDE_SIZE * (B_WIDTH - 1), 2.f * margin + TILE_SIDE_SIZE * B_HEIGHT });
    reset_button_.setTexture(&button_texture_);
}

void Interface::setupTitleText()
{
    title_.setFont(font_);
    title_.setString("");
    title_.setPosition(margin * 20.f, 2.f * margin + TILE_SIDE_SIZE * B_HEIGHT);
    title_.setCharacterSize(50); // in pixels, not points!
    title_.setFillColor(sf::Color(235, 94, 40));
}

void Interface::showCongatulations()
{
    title_.setString(titles_[0]);
}

void Interface::showRegrets()
{
    title_.setString(titles_[1]);
}

void Interface::resetTitle()
{
    title_.setString("");
}
