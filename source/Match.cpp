/*
** EPITECH PROJECT, 2018
** IndieStudio
** File description:
** Match.cpp
*/

#include <iostream>
#include "Match.hpp"
#include "Game.hpp"
#include "MapGenerator.hpp"
#include "Player.hpp"

bbm::Match::Match(Game &game) :
	IMyEventReceiver(),
	_game(game),
	_graphic(game.getGraphic()),
	_evManager(new EventManager()),
	_floor(game.getGraphic()),
	_map(*this),
	_players(),
	_camera()
{
	_evManager->addEventReceiver(this);
	_camera = _graphic.getScene()->addCameraSceneNode(0, 
			irr::core::vector3df(7.5f, 20.f, -2.f), 
			irr::core::vector3df(7.5f, 0, 6.5f));

}

void bbm::Match::init()
{
	_map.loadMap(MapGenerator::generate("./assets/maps/map1"));
	_players.push_back(new Player(*this, 1, 1, PLAYER_1));
	_players.push_back(new Player(*this, 11, 13, PLAYER_2));
	_players.push_back(new Player(*this, 1, 13, PLAYER_3));
	_players.push_back(new Player(*this, 11, 1, PLAYER_4));
	auto lala = static_cast<Player *>(_players[0]);
	_evManager->addEventReceiver(lala);
//	_evManager->addEventReceiver(_players[1]);
//	_evManager->addEventReceiver(_players[2]);
//	_evManager->addEventReceiver(_players[3]);
	_map.addEntity(_players[0]);
	_map.addEntity(_players[1]);
	_map.addEntity(_players[2]);
	_map.addEntity(_players[3]);
	std::cout << _map << std::endl;
	std::cout << "height: " << _map.getHeight() << std::endl;
	std::cout << "width: " << _map.getWidth() << std::endl;

}

bool bbm::Match::OnEvent(const irr::SEvent &event)
{
	IMyEventReceiver::OnEvent(event);
	
	std::cout << "[OnEvent - Match]" << std::endl;
	if (isKeyPressed(irr::KEY_KEY_Q, NONE)) {
		deactivate();
		resetKey(irr::KEY_KEY_Q, NONE);
		return true;
	}
	return false;
}

void bbm::Match::draw()
{

}

bool bbm::Match::run()
{
	activate();
	
	while(_graphic.getDevice()->run() && isActive()) {
		_graphic.setWindowCaption(_camera->getPosition(), L"Match loop");
		_graphic.getDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));

		_graphic.getScene()->drawAll();		
		_graphic.getDriver()->endScene();
		_players[0]->move();
		update();
	}
	_map.clear();
	deactivate();
	return true;
}

void bbm::Match::update()
{
	_players[0]->update();
}

bbm::EventManager *bbm::Match::getEventManager()
{
	return _evManager;
}

bbm::Graphic &bbm::Match::getGraphic()
{
	return _graphic;
}

bbm::Map &bbm::Match::getMap()
{
	return _map;
}
