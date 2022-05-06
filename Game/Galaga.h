#pragma once
#include "GameScreen.h"
#include "Player.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "TextureManager.h"
#include "PathManager.h"
#include "EnemyManager.h"


class Galaga
{
public:
	enum class GameState {
		Playing,
		StageComplete,
		InbetweenStages,
		SendingInEnemies,
		GameOver
	};

	Galaga();

	void Play();
	void Render();

private:
	GameScreen gameScreen;
	TextureManager textureManager;
	PathManager pathManager;
	EnemyManager enemyManager;
	Player player;
	
	GameState state = GameState::StageComplete;

	int stageNumber = 0;
	int score = 0;
	sf::Text scoreText;
	sf::Text stageText;
	sf::Text playerLivesText;
	sf::Font font;
	sf::Clock stageClock;

	sf::SoundBuffer roundBuffer;
	sf::Sound roundSound;
	bool roundSoundPlayed = false;


	void SetStageName();
	void SetText();
	void DrawGameOverScreen();

};

