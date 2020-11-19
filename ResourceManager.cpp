#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	LoadTextures();
	LoadIcon();
	LoadFont();
}

void ResourceManager::LoadTextures()
{
    sf::Texture texture;
    texture.loadFromFile("resources/pictures/empty_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(new sf::Texture(texture)));
    for (int i = 1; i < 9; ++i)
    {
        texture.loadFromFile("resources/pictures/tile" + std::to_string(i) + ".png");
        tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));
    }
    texture.loadFromFile("resources/pictures/flagged_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));

    texture.loadFromFile("resources/pictures/primary_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));

    texture.loadFromFile("resources/pictures/has_mine_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));

    texture.loadFromFile("resources/pictures/wrong_mine_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));

    texture.loadFromFile("resources/pictures/mine_exploded_tile.png");
    tile_textures_.emplace_back(std::make_shared<sf::Texture>(texture));
}

void ResourceManager::LoadIcon()
{
    sf::Image icon;
    icon.loadFromFile("resources/pictures/logo.png"); // File/Image/Pixel
    icon_ = std::make_shared<sf::Image>(new sf::Image(icon));
}

void ResourceManager::LoadFont()
{
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const TextureType& type)
{
	return std::shared_ptr<sf::Texture>();
}

std::shared_ptr<sf::Texture> ResourceManager::getIcon(const TextureType& type)
{
	return std::shared_ptr<sf::Texture>();
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const TextureType& type)
{
	return std::shared_ptr<sf::Font>();
}
