#include "Interface.h"
#include "ResourceManager.h"
Interface::Interface()    
{
    setupColorPalette();
    setupTextLabels();
    setupBoardBackground();
    setupResetButton();
}

void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(stopwatch_);
    target.draw(mines_to_find_left_);
    target.draw(board_background_);
    target.draw(reset_button_);
}

void Interface::setupColorPalette()
{
    background_color_ = sf::Color(20, 20, 20);
    interface_color_ = sf::Color(235, 94, 40);
    board_background_board_color_ = sf::Color(134, 130, 121);
}

void Interface::setupTextLabels()
{
    text_font_ = *ResourceManager<sf::Font>::Instance().GetResource("resources/inv.ttf");

    // every symbol in this font is empty for a 45 percent
    float topEmptyness = 0.45; 
    //calculate required size of font in order to be the same as text_font_size_
    unsigned adapted_text_font_size = text_font_size_  / (1-topEmptyness); 

    stopwatch_.setOrigin({ 0 * 1.f, adapted_text_font_size * topEmptyness });
    stopwatch_.setFont(text_font_);
    setStopwatch(0);
    stopwatch_.setPosition(B_MARGIN * 1.f, TILE_SIDE_SIZE * B_HEIGHT + 2.f * B_MARGIN);
    stopwatch_.setCharacterSize(adapted_text_font_size); 
    stopwatch_.setFillColor(interface_color_);

    mines_to_find_left_.setFont(text_font_);
    setMinesToFindAmount(MINE_AMOUNT);
    mines_to_find_left_.setOrigin({ 0 * 1.f, adapted_text_font_size * topEmptyness });
    mines_to_find_left_.setPosition(W_WIDTH - B_MARGIN - 250, TILE_SIDE_SIZE * B_HEIGHT + 2.f * B_MARGIN);
    mines_to_find_left_.setCharacterSize(adapted_text_font_size); 
    mines_to_find_left_.setFillColor(interface_color_);
}

void Interface::setupBoardBackground()
{
    float outlineThicknessCoefficient = 0.1f; // higher coefficient means lower thickness
    board_background_.setSize({ B_WIDTH * TILE_SIDE_SIZE + B_MARGIN *  outlineThicknessCoefficient,
                            B_HEIGHT * TILE_SIDE_SIZE + B_MARGIN *  outlineThicknessCoefficient });
    board_background_.setPosition({ B_MARGIN * (1 - outlineThicknessCoefficient/2), B_MARGIN * (1 - outlineThicknessCoefficient/2) });
    board_background_.setFillColor(board_background_board_color_);
}

void Interface::setupResetButton()
{
    reset_button_.setSize({ button_size_ * 1.f, button_size_ * 1.f }); // reset button has the same size as all text in interface
    reset_button_.setPosition({W_WIDTH/2.f - button_size_/2,
                                1.5f * B_MARGIN + TILE_SIDE_SIZE * B_HEIGHT });
    reset_button_.setTexture(&(*ResourceManager<sf::Texture>::Instance().GetResource("resources/icon.png")));
}

//Shows stopwatch with the appropriate number of zeros at the beginning
void Interface::setStopwatch(const int& gameTime)
{
    std::string stopwatch{};
    if (gameTime < 10)
        stopwatch = "00" + std::to_string(gameTime);
    else if (gameTime >= 10 && gameTime <= 99)
        stopwatch = "0" + std::to_string(gameTime);
    else if (gameTime >= 100 && gameTime <= 999)
        stopwatch = std::to_string(gameTime);
    else
        stopwatch = "999";

    stopwatch_.setString(stopwatch);
}

//Shows amount of mines left to find on field with the appropriate number of zeros at the beginning
void Interface::setMinesToFindAmount(const int& minesToFindAmount)
{
    std::string minesToFindLeftCounter{};
    if (minesToFindAmount >= 100)
        minesToFindLeftCounter = "" + std::to_string(minesToFindAmount);

    else if (minesToFindAmount >= 10 && minesToFindAmount <= 99)
        minesToFindLeftCounter = "0" + std::to_string(minesToFindAmount);

    else if (minesToFindAmount >= 0 && minesToFindAmount <= 9)
        minesToFindLeftCounter = "00" + std::to_string(minesToFindAmount);

    else if (minesToFindAmount <= -1 && minesToFindAmount >= -9)
        minesToFindLeftCounter = "-0" + std::to_string(minesToFindAmount - 2 * minesToFindAmount);

    else if (minesToFindAmount <= -10 && minesToFindAmount >= -99)
        minesToFindLeftCounter = std::to_string(minesToFindAmount);

    else if (minesToFindAmount < -99)
        minesToFindLeftCounter = "-99";

    mines_to_find_left_.setString(minesToFindLeftCounter);
}

bool Interface::isResetButton(const sf::Vector2i& eventCoord)
{
    return eventCoord.x >= reset_button_.getPosition().x &&
            eventCoord.x <= reset_button_.getPosition().x + reset_button_.getSize().x &&
            eventCoord.y >= reset_button_.getPosition().y &&
            eventCoord.y <= reset_button_.getPosition().y + reset_button_.getSize().y;
}

sf::Color Interface::backgroundColor()
{
    return background_color_;
}

void Interface::resetCounters()
{
    //back to default values
    setStopwatch(0);
    setMinesToFindAmount(MINE_AMOUNT);
}
