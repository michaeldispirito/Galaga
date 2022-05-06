#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include <unordered_map>

class TextureManager      
{
public:
	TextureManager();

	enum class TextureType {
		PlayerShip,
		Bullet,
		Bee,
		Moth,
		Boss
	};

	sf::Texture* GetMainTexture() const;
	const sf::IntRect& GetTextureRef(TextureType type) const;


private:
	std::unique_ptr<sf::Texture> mainTexture = std::make_unique<sf::Texture>();
	std::unordered_map<TextureType, sf::IntRect> textureLocationMap;


	void InitializeMap();

};

