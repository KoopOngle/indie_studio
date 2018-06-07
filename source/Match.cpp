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
#include "BombUp.hpp"
#include "SpeedUp.hpp"
#include "FireUp.hpp"
#include "WallPass.hpp"
#include "Bomb.hpp"

bbm::Match::Match(Game &game) :
	IMyEventReceiver(),
	_game(game),
	_graphic(game.getGraphic()),
	_evManager(new EventManager()),
	_floor(game.getGraphic()),
	_map(*this),
	_bombs(),
	_players(),
	_camera()
{
	_evManager->addEventReceiver(this);
/*	irr::SKeyMap keyMap[5];                    // re-assigne les commandes
	keyMap[0].Action = irr::EKA_MOVE_FORWARD;  // avancer
	keyMap[0].KeyCode = irr::KEY_KEY_I;        // w
	keyMap[1].Action = irr::EKA_MOVE_BACKWARD; // reculer
	keyMap[1].KeyCode = irr::KEY_KEY_K;        // s
	keyMap[2].Action = irr::EKA_STRAFE_LEFT;   // a gauche
	keyMap[2].KeyCode = irr::KEY_KEY_J;        // a
	keyMap[3].Action = irr::EKA_STRAFE_RIGHT;  // a droite
	keyMap[3].KeyCode = irr::KEY_KEY_L;        // d
	keyMap[4].Action = irr::EKA_JUMP_UP;       // saut
	keyMap[4].KeyCode = irr::KEY_SPACE;        // barre espace

	_camera = _graphic.getScene()->addCameraSceneNodeFPS(       // ajout de la camera FPS
		0,                                     // pas de noeud parent
		100.0f,                                // vitesse de rotation
		0.01f,                                  // vitesse de deplacement
		-1,                                    // pas de numero d'ID
		keyMap,                                // on change la keymap
		3);*/
	_camera = _graphic.getScene()->addCameraSceneNode(0,
			irr::core::vector3df(13.96f, 18.97f, 3.83f),
			irr::core::vector3df(14, -12, 14));

}

void bbm::Match::init()
{
	_map.loadMap(MapGenerator::generate("./assets/maps/map1"));
	addPlayer(new Player(*this, 1, 1, PLAYER_1));
	addPlayer(new Player(*this, 11, 13, PLAYER_2));
	addPlayer(new Player(*this, 1, 13, PLAYER_3));
	addPlayer(new Player(*this, 11, 1, PLAYER_4));
	auto lala = static_cast<Player *>(_players[0]);
	_evManager->addEventReceiver(lala);
//	_evManager->addEventReceiver(_players[1]);
//	_evManager->addEventReceiver(_players[2]);
//	_evManager->addEventReceiver(_players[3]);
	_map.addEntity(new BombUp(*this, 2, 6, true));
	_map.addEntity(new SpeedUp(*this, 4, 6, true));
	_map.addEntity(new FireUp(*this, 6, 6, true));
	_map.addEntity(new WallPass(*this, 8, 6, true));
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

void bbm::Match::print_skybase()
{
	auto *path = "assets/model3D/Sky/space.jpg";
	auto *skybase = _graphic.getScene()->addSkyBoxSceneNode(
		_graphic.getDriver()->getTexture(path),
		_graphic.getDriver()->getTexture(path),
		_graphic.getDriver()->getTexture(path),
		_graphic.getDriver()->getTexture(path),
		_graphic.getDriver()->getTexture(path),
		_graphic.getDriver()->getTexture(path));
}

bool bbm::Match::run()
{
	activate();

	print_skybase();
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
	int lastSize;

	for (auto it = _players.begin(); it < _players.end(); ++it)
		(*it)->update();
	lastSize = _bombs.size();
	for (int i = 0; i < _bombs.size(); ++i) {
		_bombs[i]->update();
		if (lastSize != _bombs.size()) {
			lastSize = _bombs.size();
			--i;
		}
	}
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

void bbm::Match::addBomb(Bomb *bomb)
{
	_bombs.push_back(bomb);
}

void bbm::Match::removeBomb(Bomb *bomb)
{
	for (auto it = _bombs.begin(); it != _bombs.end(); ++it)
		if (*it == bomb)
			_bombs.erase(it--);
}

void bbm::Match::addPlayer(IPlayer *player)
{
	_players.push_back(player);
}

void bbm::Match::removePlayer(IPlayer *player)
{
	for (auto it = _players.begin(); it != _players.end(); ++it) {
		if (*it == player) {
			_players.erase(it--);
		}
	}
}
