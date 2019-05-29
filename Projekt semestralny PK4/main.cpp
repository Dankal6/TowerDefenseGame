#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <thread>
#include <iostream>
#include "Enemy.h"
#include "Tower.h"
#include "EnemyBase.h"
#include "PlayerBase.h"
#include "Map.h"
#include "TowerManager.h"
#include "Scale.h"

using namespace std;


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920*scale, 1080*scale), "TD");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	float deltaTime;

	int frame = 0;
	bool start = false;
	bool placing_tower = false;
	bool game_paused = false;

	vector<Tower*> Towers;
	vector<Enemy*> Enemies;
	Map map;
	EnemyBase enemybase(sf::Vector2f(-20.0f*scale, 145.0f*scale));
	PlayerBase playerbase(sf::Vector2f(1940.0f*scale, 920.0f*scale));
	TowerManager towermanager(&map, &Towers,&window);



	while (window.isOpen())
	{
		//std::cout << sf::Mouse::getPosition(window).x << ";" << sf::Mouse::getPosition(window).y << endl;
		deltaTime = clock.restart().asSeconds();
		window.clear();
		frame++;
		map.drawMap(window);

		//czesc odpowiedzialna za czestotliwosc pojawiania sie przeciwnika, oraz pociskow
		for (auto t : Towers)
		{
			t->incrementFrame();
		}
		enemybase.incrementFrame();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				if (!towermanager.isTowerClicked(mousePos) && !towermanager.isPlaceClicked(mousePos))
				{
					towermanager.nothingClicked();
				}
			}
			//Tworze nowego przeciwnika
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
			{
				game_paused = false;
				window.setFramerateLimit(60);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
			{
				game_paused = false;
				window.setFramerateLimit(120);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
			{
				game_paused = false;
				window.setFramerateLimit(240);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
			{
				game_paused = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				start = true;
				enemybase.setToSpawn(5);
				//prowizoryczne poki co wyswietlanie Start
				sf::Font ttf;
				ttf.loadFromFile("fonts/font.ttf");
				std::string s("Start");
				sf::Text txt(s, ttf);
				txt.setCharacterSize(150);
				txt.setFillColor(sf::Color::Red);
				txt.setPosition(1000 * scale, 40 * scale);
				window.draw(txt);
			}
		}
		if (game_paused == false)
		{

			playerbase.check_if_enemy_in(&Enemies);
			//rysowanie wiez
			for (auto t : Towers)
			{
				t->shoot();
				//sprawdzanie, czy jakis przecinik jest w zasiegu
				Enemy *enemy_to_shoot = t->check_if_in_range(&Enemies);
				if (enemy_to_shoot != NULL)
				{
					if (t->returnFrames() >= 10)
					{
						//wycelowanie
						t->aim(enemy_to_shoot);
						t->resetFrames();
					}
				}
				//sprawdzanie, czy przeciwnik zostal trafiony
				for (auto e : Enemies)
				{
					e->gotHitted(&Enemies, t->returnBullets(), &playerbase);
				}
			}
			//start gry, przeciwnik rusza
			if (start)
			{
				if (enemybase.returnFrames() >= 30)
				{
					enemybase.spawnEnemy(&Enemies);
					enemybase.resetFrames();
				}

				for (auto e : Enemies)
				{
					e->move(deltaTime);
				}
			}
		}
		//rysowanie bazy gracza
		playerbase.drawBase(&window);
		enemybase.drawBase(window);
		//rysowanie pociskow i wiez
		for (auto t : Towers)
		{
			t->drawTower();
		}
		//rysowanie przeciwnikow
		for (auto e : Enemies)
		{
			e->drawEnemy(window);
		}

		window.display();
	}

	return 0;
}