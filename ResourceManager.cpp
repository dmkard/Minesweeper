#include "ResourceManager.h"
#include <iostream>
ResourceManager::ResourceManager()
{
    LoadIcon();
    LoadFont();
}

std::shared_ptr<sf::Texture> ResourceManager::LoadTexture(const std::string& name)
{
    sf::Texture texture;
    texture.loadFromFile("resources/pictures/" + name + ".png");
    tile_textures_.emplace(name, std::make_shared<sf::Texture>(texture));
    return tile_textures_[name];
}

void ResourceManager::LoadIcon()
{
    /*sf::Texture icon;
    icon.loadFromFile("resources/pictures/icon.png");
    tile_textures_.emplace(std::make_shared<sf::Texture>(icon));*/
}

void ResourceManager::LoadFont()
{

}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& name)
{
    auto it = tile_textures_.find(name);
    if (it != tile_textures_.end())
    {
        return it->second;
    }
        
    else
    {
        std::cout << "We have to load " << name << std::endl;
        return LoadTexture(name);
    }
        
}
void ResourceManager::Update()
{
    for (auto it = tile_textures_.begin(); it != tile_textures_.end();)
    {
        std::cout << it->first << " has " << it->second.use_count() << " examples\n";
        if (it->second.use_count() == 1)
        {
            std::cout << "we dont need " << it->first << " anymore\n";
            it = tile_textures_.erase(it);
        }
        else
            ++it;
            
    }
}
/*
std::shared_ptr<sf::Texture> ResourceManager::getIcon(const TextureType& type)
{
	return std::shared_ptr<sf::Texture>();
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const TextureType& type)
{
	return std::shared_ptr<sf::Font>();
}*/
