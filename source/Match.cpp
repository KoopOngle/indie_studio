//
// EPITECH PROJECT, 2018
// IndieStudio
// File description:
// Match.cpp
//

#include <iostream>
#include "Match.hpp"
#include "Game.hpp"
#include "Definer.hpp"
#include "MapGenerator.hpp"
#include "Player.hpp"
#include "Ia.hpp"
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
	_camera(),
	_counting(false)
{
	_evManager->addEventReceiver(this);
	_camera = _graphic.getScene()->addCameraSceneNode(0,
			irr::core::vector3df(6.92f, 13.05f, 2.16f),
			irr::core::vector3df(6.94f, -1.82f, 5.70f));
}

void bbm::Match::init(std::vector<bbm::AttrEntity> attrs,
	std::vector<bbm::TeamColor> teams)
{
	_map.loadMap(MapGenerator::generate("./assets/maps/map1"));
	if (attrs[0] == bbm::ATTR_PLAYER)
		addPlayer(new Player(*this, 1.5, 1.5, PLAYER_1, teams[0]));
	else if (attrs[0] == bbm::ATTR_AI)
		addPlayer(new Ia(*this, 1.5, 1.5, PLAYER_1, teams[0]));
	if (attrs[1] == bbm::ATTR_PLAYER)
		addPlayer(new Player(*this, 11.5, 13.5, PLAYER_2, teams[1]));
	else if (attrs[1] == bbm::ATTR_AI)
		addPlayer(new Ia(*this, 11.5, 13.5, PLAYER_2, teams[1]));
	if (attrs[2] == bbm::ATTR_PLAYER)
		addPlayer(new Player(*this, 1.5, 13.5, PLAYER_3, teams[2]));
	else if (attrs[2] == bbm::ATTR_AI)
		addPlayer(new Ia(*this, 1.5, 13.5, PLAYER_3, teams[2]));
	if (attrs[3] == bbm::ATTR_PLAYER)
		addPlayer(new Player(*this, 11.5, 1.5, PLAYER_4, teams[3]));
	else if (attrs[3] == bbm::ATTR_AI)
		addPlayer(new Ia(*this, 11.5, 1.5, PLAYER_4, teams[3]));
	std::cout << _map << std::endl;
}

bool bbm::Match::OnEvent(const irr::SEvent &event)
{
	IMyEventReceiver::OnEvent(event);

	if (isKeyPressed(irr::KEY_ESCAPE, NONE) && !_counting) {
		deactivate();
		resetKey(irr::KEY_ESCAPE, NONE);
		_menuReturn = _game.getMenuInGame()->run();
		resetKey(irr::KEY_ESCAPE, NONE);
		if (_menuReturn) {
			drawStarter();
			activate();
		}
		return true;
	}
	return false;
}

void bbm::Match::draw()
{

}

irr::video::ITexture *bbm::Match::getWinnerColor(TeamColor color)
{
	irr::video::ITexture	*ret;
	auto driver = _game.getGraphic().getDriver();

	switch (color) {
	case TEAM_NONE :
		return driver->getTexture("./assets/gameplay/draw.png");
	case TEAM_RED :
		return driver->getTexture("./assets/gameplay/red_win.png");
	case TEAM_BLUE :
		return driver->getTexture("./assets/gameplay/blue_win.png");
	case TEAM_GREEN :
		return driver->getTexture("./assets/gameplay/green_win.png");
	case TEAM_PURPLE :
		return driver->getTexture("./assets/gameplay/purple_win.png");
	}
	return NULL;
}

void bbm::Match::drawWinnerRec(const irr::core::dimension2du& screenSize,
	irr::video::ITexture *img)
{
	_graphic.getDriver()->beginScene(true, true,
	irr::video::SColor(255, 100, 101, 140));
	_graphic.getScene()->drawAll();
	_graphic.getDriver()->draw2DRectangle(
	irr::video::SColor(110, 150, 150, 150),
	irr::core::rect<irr::s32>(0, 0, screenSize.Width,
	screenSize.Height));
	_graphic.getDriver()->draw2DImage(img,
	irr::core::rect<irr::s32>(screenSize.Width / 5 * 2,
	screenSize.Height / 5 * 2, screenSize.Width / 5 * 3,
	screenSize.Height / 5 * 3),
	irr::core::rect<irr::s32>(0, 0, img->
	getSize().Width, img->getSize().Height), 0, 0, true);
	_graphic.getGuienv()->drawAll();
	_graphic.getDriver()->endScene();
}

void bbm::Match::drawWinner()
{
	const irr::core::dimension2du& screenSize = _graphic.
		getDriver()->getScreenSize();
	auto winner = getWinner();
	auto color = getWinnerColor(winner);
	auto cTime = std::chrono::steady_clock::now();
	auto nTime = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::seconds>
	(nTime - cTime);

	_graphic.getDriver()->enableMaterial2D();
	while (diff.count() < 3) {
		nTime = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::seconds>
		(nTime - cTime);
		drawWinnerRec(screenSize, color);
	}
	_graphic.getDriver()->enableMaterial2D(false);
}

void bbm::Match::runAudio()
{
	_game.getAudio().stopMenuMusic();
	_game.getAudio().playInGameMusic();
}

void bbm::Match::stopAudio()
{
	_game.getAudio().stopInGameMusic();
	_game.getAudio().playMenuMusic();
	deactivate();
}

bool bbm::Match::run()
{
	runAudio();
	print_skybase();
	drawStarter();
	activate();
	while(_graphic.getDevice()->run() && isActive() && !isFinished()) {
		_graphic.setWindowCaption(_camera->getPosition(),
			L"Match loop");
		_graphic.getDriver()->beginScene(true, true,
			irr::video::SColor(255, 100, 101, 140));
		_graphic.getScene()->drawAll();
		_graphic.getDriver()->endScene();
		update();
	}
	if (isFinished())
		drawWinner();
	_map.clear();
	_players.clear();
	_bombs.clear();
	stopAudio();
	return true;
}

void bbm::Match::update()
{
	int lastSize;

	for (auto it = _players.begin(); it != _players.end(); ++it) {
		if ((*it))
			(*it)->update();
	}
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

bbm::Game &bbm::Match::getGame()
{
	return _game;
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
	if (!player) {
		return;
	}
	_players.push_back(player);
}

void bbm::Match::removePlayer(IPlayer *player)
{
	for (auto it = _players.begin(); it != _players.end(); ++it)
		if (*it == player)
			_players.erase(it--);
}

void bbm::Match::save()
{
	std::ofstream myfile;

	_map.save();
	myfile.open("PlayerMatch.cfg");
	if (myfile.is_open() == false)
		return;
	for(unsigned int i = 0; i < _players.size(); i++) {
		myfile << _players[i] << std::endl;
	}
	myfile.close();
}

bbm::IPlayer *bbm::Match::createPlayer(std::string line)
{
	std::string::size_type sz;

	size_t pos = line.find(":");
	std::string number = line.substr(0, pos);
	line.erase(0, pos + 1);
	pos = line.find(":");
	std::string type = line.substr(0, pos);
	line.erase(0, pos + 1);
	pos = line.find(":");
	std::string x = line.substr(0, pos);
	line.erase(0, pos + 1);
	pos = line.find(":");
	std::string z = line.substr(0, pos);
	line.erase(0, pos + 1);
	std::string color = line;
	if (type.compare("IA") == 0)
		return (new Ia(*this, stoi(z, &sz), stoi(x, &sz),
			entities[stoi(number, &sz)], strToTeamColor(color)));
	return (new Player(*this, stoi(z, &sz), stoi(x, &sz),
		entities[stoi(number, &sz)], strToTeamColor(color)));
}

bbm::TeamColor bbm::Match::strToTeamColor(std::string str)
{
	if (str.compare("TEAM_RED") == 0)
		return TEAM_RED;
	if (str.compare("TEAM_GREEN") == 0)
		return TEAM_GREEN;
	if (str.compare("TEAM_PURPLE") == 0)
		return TEAM_PURPLE;
	if (str.compare("TEAM_BLUE") == 0)
		return TEAM_BLUE;
	return TEAM_RED;
}

bool bbm::Match::handleLine(std::string line, int i, bbm::IPlayer **player)
{
	size_t pos = 0;
	std::string firstTok;
	std::string delimiter = ":";
	std::string token;
	std::string::size_type sz;

	pos = line.find(delimiter);
	token = line.substr(0, pos);
	firstTok = token;
	if (isValuable(firstTok) && firstTok.compare(valuable[i]) == 0)
		*player = this->createPlayer(line);
	else if (this->isValuable(firstTok) && *player)
		return false;
	line.erase(0, pos + delimiter.length());
	doWithTokens(firstTok, line, player);
	return true;
}

bool bbm::Match::isValuable(std::string str)
{
	for (int i = 0; i < 4; i++) {
		if (valuable[i].compare(str) == 0)
			return true;
	}
	return false;
}

void bbm::Match::doWithTokens(std::string t1, std::string t2,
	bbm::IPlayer **p)
{
	std::string::size_type sz;

	if (t1.compare("WALLPASS") == 0 && *p)
		(*p)->setWallPass(t2.compare("false"));
	if (t1.compare("SPEED") == 0 && (*p))
		(*p)->setSpeed(std::stoi(t2, &sz));
	if (t1.compare("BOMB_COUNT") == 0 && (*p)) {
		(*p)->setBombCount(std::stoi(t2, &sz));
		if (std::stoi(t2, &sz) <= 0)
			(*p)->setBombCount(1);
	}
	if (t1.compare("POWER") == 0 && (*p))
		(*p)->setPower(std::stoi(t2, &sz));
}

bool bbm::Match::hasSave()
{
	std::ifstream file("PlayerMatch.cfg");

	if (!file)
		return false;
	file.close();
	return true;
}


bbm::IPlayer *bbm::Match::loadIPlayer(int nbPlayer)
{
	std::ifstream file("PlayerMatch.cfg");
	std::string line;
	bbm::IPlayer *player = nullptr;
	bool check = true;

	if (!file) {
		return player;
	}
	while (std::getline(file, line) && check == true) {
		check = this->handleLine(line, nbPlayer, &player);
	}
	return player;
}

void bbm::Match::load()
{
	_map.load();
	for(int i = 0; i < 4; i++)
		this->addPlayer(this->loadIPlayer(i));
}

void bbm::Match::print_skybase()
{
	std::string path = "assets/model3D/Sky/brouillard.jpg";
	auto texture = _graphic.getDriver()->getTexture(path.c_str());
	auto skybase = _graphic.getScene()->addSkyBoxSceneNode(texture,
			texture, texture, texture, texture, texture);
}

std::vector<irr::video::ITexture *>	bbm::Match::createCounter()
{
	std::vector<irr::video::ITexture *>	ret;

	ret.push_back(_game.getGraphic().getDriver()->
	getTexture("./assets/gameplay/3.png"));
	ret.push_back(_game.getGraphic().getDriver()->
	getTexture("./assets/gameplay/2.png"));
	ret.push_back(_game.getGraphic().getDriver()->
	getTexture("./assets/gameplay/1.png"));
	ret.push_back(_game.getGraphic().getDriver()->
	getTexture("./assets/gameplay/fight.png"));
	return ret;
}

irr::video::ITexture	*bbm::Match::getCurrentCounter(
	std::vector<irr::video::ITexture *> list, float count)
{
	irr::video::ITexture	*ret = list[0];

	if (count > 0)
		ret = list[0];
	if (count >= 750)
		ret = list[1];
	if (count >= 1500)
		ret = list[2];
	if (count >= 2250)
		ret = list[3];
	return ret;
}

void bbm::Match::drawImageBack(const irr::core::dimension2du& screenSize,
	irr::video::ITexture *img)
{
	_graphic.getDriver()->beginScene(true, true,
	irr::video::SColor(255, 100, 101, 140));
	_graphic.getScene()->drawAll();
	_graphic.getDriver()->draw2DImage(img,
	irr::core::rect<irr::s32>(screenSize.Width / 5 * 2,
	screenSize.Height / 5 * 2, screenSize.Width / 5 * 3,
	screenSize.Height / 5 * 3),
	irr::core::rect<irr::s32>(0, 0, img->
	getSize().Width, img->getSize().Height), 0, 0, true);
	_graphic.getDriver()->draw2DRectangle(
	irr::video::SColor(110, 150, 150, 150),
	irr::core::rect<irr::s32>(0, 0, screenSize.Width,
	screenSize.Height));
	_graphic.getGuienv()->drawAll();
	_graphic.getDriver()->endScene();
}

void bbm::Match::drawStarter()
{
	const irr::core::dimension2du& screenSize = _graphic.
		getDriver()->getScreenSize();
	auto list = createCounter();
	auto cTime = std::chrono::steady_clock::now();
	auto nTime = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>
	(nTime - cTime);
	irr::video::ITexture *background = list[0];

	print_skybase();
	_graphic.getDriver()->enableMaterial2D();
	while (_graphic.getDevice()->run() && diff.count() < 3000) {
		nTime = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::milliseconds>
		(nTime - cTime);
		background = getCurrentCounter(list, diff.count());
		drawImageBack(screenSize, background);
	}
	_graphic.getDriver()->enableMaterial2D(false);
}

bool bbm::Match::isFinished()
{
	TeamColor team;

	if (!_players.size())
		return true;
	team = _players[0]->getTeam();
	for (int i = 1; i < _players.size(); ++i)
		if (team != _players[i]->getTeam())
			return false;
	return true;
}

bbm::TeamColor bbm::Match::getWinner()
{
	return (!_players.size()) ? TeamColor::TEAM_NONE :
		_players[0]->getTeam();
}
