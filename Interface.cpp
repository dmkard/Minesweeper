#include "Interface.h"
#include "ResourceManager.h"
Interface::Interface()    
{
    setupColorPalette();
    setupTextLabels();
    setupBackground();
    setupResetButton();
    setupTitleText();
}


void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(stopwatch_);
    target.draw(mine_left_);
    target.draw(background_);
    target.draw(reset_button_);
    target.draw(text_box_);
}

void Interface::setupColorPalette()
{
    base_color_ = sf::Color(20, 20, 20);
    interface_color_ = sf::Color(235, 94, 40);
    background_board_color_ = sf::Color(134, 130, 121);
}

void Interface::setupTextLabels()
{

    font_ = *ResourceManager<sf::Font>::Instance().GetResource("resources/inv.ttf");

    stopwatch_.setFont(font_);
    stopwatch_.setString(L"0");
    std::wcout << font_size_;
    //stopwatch_.setOrigin({ 0 * 1.f, font_size_*0.30666f});
    stopwatch_.setPosition(B_MARGIN * 1.f,  TILE_SIDE_SIZE * B_HEIGHT);
   // stopwatch_.setPosition(B_MARGIN * 1.f, W_HEIGHT-font_size_);

    stopwatch_.setCharacterSize(100); // in pixels, not points!
    stopwatch_.setFillColor(interface_color_);

    mine_left_.setFont(font_);
    mine_left_.setString(std::to_string(MINE_AMOUNT));
    mine_left_.setPosition(B_MARGIN * 10.f, 1.f * B_MARGIN + TILE_SIDE_SIZE * B_HEIGHT);
    mine_left_.setCharacterSize((B_MARGIN * 3)/0.694); // in pixels, not points!
    mine_left_.setFillColor(interface_color_);
}

void Interface::setupBackground()
{
    float outlineThicknessCoefficient = 0.96f; // lower coefficient means lower thickness
    background_.setSize({ B_WIDTH * TILE_SIDE_SIZE + B_MARGIN * (1 - outlineThicknessCoefficient),
                            B_HEIGHT * TILE_SIDE_SIZE + B_MARGIN * (1 - outlineThicknessCoefficient) });
    background_.setPosition({ B_MARGIN * outlineThicknessCoefficient,
                                B_MARGIN * outlineThicknessCoefficient });
    background_.setFillColor(background_board_color_);
}

void Interface::setupResetButton()
{
    button_texture_ = *ResourceManager<sf::Texture>::Instance().GetResource("resources/icon.png");
    reset_button_.setSize({ font_size_ * 1.f, font_size_ * 1.f }); // reset button has the same size as all text in interface
    reset_button_.setPosition({W_WIDTH - B_MARGIN - font_size_*1.f,
                                2.f * B_MARGIN + TILE_SIDE_SIZE * B_HEIGHT });
    reset_button_.setTexture(&button_texture_);
}

void Interface::setupTitleText()
{
    text_box_.setFont(font_);
    text_box_.setString("");
    text_box_.setPosition(B_MARGIN * 20.f, 2.f * B_MARGIN + TILE_SIDE_SIZE * B_HEIGHT);
    text_box_.setCharacterSize(font_size_); 
    text_box_.setFillColor(interface_color_);
}

void Interface::showCongatulations()
{
    text_box_.setString(titles_[0]);
}

void Interface::showRegrets()
{
    text_box_.setString(titles_[1]);
}

void Interface::resetTitle()
{
    text_box_.setString("");
}
void Interface::setStopwatch(const int& gameTime)
{
    stopwatch_.setString(std::to_string(gameTime));
}

void Interface::setMineAmount(const int& mineAmount)
{
    mine_left_.setString(std::to_string(mineAmount));
}

sf::Color Interface::baseColor()
{
    return base_color_;
}