#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class ResourceManager
{
public:
	enum TextureType {
		empty_tile = 0,
		tile_1 = 1,
		tile_2 = 2,
		tile_3 = 3,
		tile_4 = 4,
		tile_5 = 5,
		tile_6 = 6,
		tile_7 = 7,
		tile_8 = 8,
		flagged_tile = 9,
		primary_tile = 10,
		has_mine_tile = 11,
		wrong_mine_tile = 12,
		mine_exploded_tile = 13,
	};
	ResourceManager();
private:
	void LoadTextures();
	void LoadIcon();
	void LoadFont();
	std::shared_ptr<sf::Texture> getTexture(const TextureType& type);
	std::shared_ptr<sf::Texture> getIcon(const TextureType& type);
	std::shared_ptr<sf::Font> getFont(const TextureType& type);

	std::vector<std::shared_ptr<sf::Texture>> tile_textures_;
	std::shared_ptr<sf::Texture> icon_;
	std::shared_ptr<sf::Font> font_;
};

