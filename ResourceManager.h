#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
	ResourceManager();
	std::shared_ptr<sf::Texture> getTexture(const std::string& name);
	std::shared_ptr<sf::Texture> getIcon();
	std::shared_ptr<sf::Font> getFont();
	void Update();

private:
	std::shared_ptr<sf::Texture> LoadTexture(const std::string& name);
	void LoadIcon();
	void LoadFont();
	

	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> tile_textures_;
	std::shared_ptr<sf::Texture> icon_;
	std::shared_ptr<sf::Font> font_;
};

