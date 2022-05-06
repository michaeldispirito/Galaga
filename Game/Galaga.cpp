#include "Precompiled.h"
#include "Galaga.h"


Galaga::Galaga() :
	gameScreen(),
	textureManager(),
	pathManager(),
	enemyManager(textureManager.GetMainTexture(), textureManager.GetTextureRef(TextureManager::TextureType::Bee), textureManager.GetTextureRef(TextureManager::TextureType::Moth), textureManager.GetTextureRef(TextureManager::TextureType::Boss), pathManager.GetEntrancePaths().size()),
	player(gameScreen.GetGameWidth(), gameScreen.GetHeight(), textureManager.GetMainTexture(), textureManager.GetTextureRef(TextureManager::TextureType::PlayerShip), textureManager.GetTextureRef(TextureManager::TextureType::Bullet))
{

	if (!font.loadFromFile("../Emulogic-zrEw.ttf")) {
		std::cout << "Error Loading Font" << std::endl;
	}
	stageText.setCharacterSize(15);
	stageText.setString("stage " + std::to_string(stageNumber));
	stageText.setFont(font);
	stageText.setFillColor(sf::Color::Red);
	stageText.setOrigin(sf::Vector2f(stageText.getLocalBounds().width / 2, stageText.getLocalBounds().height / 2));
	stageText.setPosition(sf::Vector2f(gameScreen.GetGameWidth() * 0.5, gameScreen.GetHeight() * 0.33f));

	scoreText.setCharacterSize(12);
	scoreText.setString("score XXXXXX");
	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setOrigin(sf::Vector2f(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2));
	scoreText.setPosition(sf::Vector2f(gameScreen.GetGameWidth() + (gameScreen.GetStatsWidth() / 2), gameScreen.GetHeight() * 0.1f));

	playerLivesText.setCharacterSize(12);
	playerLivesText.setString("Lives: 3");
	playerLivesText.setFont(font);
	playerLivesText.setFillColor(sf::Color::White);
	playerLivesText.setOrigin(sf::Vector2f(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2));
	playerLivesText.setPosition(sf::Vector2f(gameScreen.GetGameWidth() + (gameScreen.GetStatsWidth() / 2), gameScreen.GetHeight() * 0.7f));

	roundBuffer.loadFromFile("../Music/StartOfRound.wav");
	roundSound.setBuffer(roundBuffer);
}

void Galaga::Play()
{


	while (gameScreen.window.isOpen()) {

		sf::Event event;
		while (gameScreen.window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					gameScreen.window.close();
					break;

				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::X) {
						player.StopShooting();
					}
			}


		}

		switch (state) {
		case GameState::StageComplete:
			stageNumber++;
			enemyManager.SetupRound(pathManager.GetEntrancePaths());
			SetStageName();
			stageClock.restart();
			state = GameState::InbetweenStages;
			roundSoundPlayed = false;
			break;
		case GameState::InbetweenStages:
			if (stageClock.getElapsedTime().asSeconds() >= 3) {
				state = GameState::SendingInEnemies;
			}
			break;
		case GameState::SendingInEnemies:
			if (enemyManager.SendNextEnemy()) {
				state = GameState::Playing;
			}
			enemyManager.UpdateEnemies(player, score, pathManager.GetAttackPaths());
			if (enemyManager.AllEnemiesDefeated()) {
				state = GameState::StageComplete;
			}
			break;
		case GameState::Playing:
			enemyManager.UpdateEnemies(player, score, pathManager.GetAttackPaths());
			if (enemyManager.AllEnemiesDefeated()) {
				state = GameState::StageComplete;
			}
			break;
		}

		if (player.GetLives() <= 0) {
			state = GameState::GameOver;
		}

		if (state == GameState::GameOver) {
			DrawGameOverScreen();
		} else {
			SetText();
			player.CheckKeyboardInput();
			player.UpdateBullets();
			Render();
		}

	}
}

void Galaga::Render()
{
	gameScreen.Render();
	player.Render(gameScreen.window);

	if (state == GameState::InbetweenStages || state == GameState::GameOver) {
		gameScreen.window.draw(stageText);
		if (!roundSoundPlayed) {
			roundSound.play();
			roundSoundPlayed = true;
		}
	}
	else if (state == GameState::Playing || state == GameState::SendingInEnemies) {
		enemyManager.Render(gameScreen.window);
	}
	gameScreen.window.draw(scoreText);
	gameScreen.window.draw(playerLivesText);
	gameScreen.window.display();
}

void Galaga::SetStageName()
{
	if (state != GameState::GameOver) {
		stageText.setString("stage " + std::to_string(stageNumber));
	}
	else {
		stageText.setString("game over");
	}
}

void Galaga::SetText()
{
	scoreText.setString("score: " + std::to_string(score));
	playerLivesText.setString("Lives: " + std::to_string(player.GetLives()));
}

void Galaga::DrawGameOverScreen()
{
	SetText();
	SetStageName();
	Render();
}
