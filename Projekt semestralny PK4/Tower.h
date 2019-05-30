#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"
#include <math.h>
#include "Scale.h"
#include "PlaceForTower.h"

#include <iostream>

class Enemy;	//potrzebny aby  nie bylo bledu wzajemnego include, do poprawy

class Tower
{
private:
	sf::RectangleShape tower;
	sf::CircleShape range;
	sf::Vector2f position;
	sf::RenderWindow *window;
	std::vector<Bullet*> Bullets;
	sf::Texture towerTexture;
	PlaceForTower* place;
	int size = 140 * scale;
	int range_rad = 200 * scale;
	bool is_builded = false;
	int showRange = false;
	int damage;
	int upgradeLevel;
	int frame;	//prowizoryczna czestotliwosc strzelania dla kazdej z wiezyczek
public:
	Tower(sf::RenderWindow*,sf::Vector2f);
	~Tower();
	void drawTower();
	void drawRange(bool);
	void shoot();
	void aim(Enemy*);
	Enemy* check_if_in_range(std::vector<Enemy*>*);
	bool isClicked(sf::Vector2f);
	sf::Vector2f returnPosition();
	void setPlace(PlaceForTower*);
	PlaceForTower* getPlace();

	int returnDamage();
	void setDamage(int);
	int returnLevel();
	void setLevel(int);

	std::vector<Bullet*> *returnBullets();
	void incrementFrame();
	int returnFrames();
	void resetFrames();
	sf::RectangleShape* returnTower();

	void actionWithTower();
};


