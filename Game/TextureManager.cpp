#include "Precompiled.h"
#include "TextureManager.h"

TextureManager::TextureManager()
{
	mainTexture->loadFromFile("../Textures/SpriteSheet.png");
	InitializeMap();
}

sf::Texture* TextureManager::GetMainTexture() const
{
	return mainTexture.get();
}

const sf::IntRect& TextureManager::GetTextureRef(TextureType type) const
{
	return textureLocationMap.at(type);
}

void TextureManager::InitializeMap()
{
	textureLocationMap.emplace(TextureType::PlayerShip, sf::IntRect(109, 1, 16, 16));
	textureLocationMap.emplace(TextureType::Bullet, sf::IntRect(311, 140, 8, 8));
	textureLocationMap.emplace(TextureType::Bee, sf::IntRect(109, 91, 16, 16));
	textureLocationMap.emplace(TextureType::Moth, sf::IntRect(109, 73, 16, 16));
	textureLocationMap.emplace(TextureType::Boss, sf::IntRect(109, 37, 16, 16));
}
