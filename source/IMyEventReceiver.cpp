//
// EPITECH PROJECT, 2018
// IMyEventReceiver.hpp
// File description:
// IMyEventReceiver implementation
//

#include "IMyEventReceiver.hpp"

bbm::IMyEventReceiver::IMyEventReceiver() :
	_isActive(false),
	_keys(),
	_keysCtrl(),
	_keysShift()
{
	resetKeys();
}

bool bbm::IMyEventReceiver::OnEvent(const irr::SEvent &event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
			event.KeyInput.PressedDown) {
		if (event.KeyInput.Control)
			_keysCtrl[event.KeyInput.Key] = true;
		else if (event.KeyInput.Shift)
			_keysShift[event.KeyInput.Key] = true;
		else
			_keys[event.KeyInput.Key] = true;
	} else if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Control)
			_keysCtrl[event.KeyInput.Key] = false;
		else if (event.KeyInput.Shift)
			_keysShift[event.KeyInput.Key] = false;
		else
			_keys[event.KeyInput.Key] = false;
	}
	return false;
}

bool bbm::IMyEventReceiver::isActive()
{
	return _isActive;
}

void bbm::IMyEventReceiver::activate()
{
	_isActive = true;
}

void bbm::IMyEventReceiver::deactivate()
{
	_isActive = false;
}

bool bbm::IMyEventReceiver::isKeyPressed(irr::EKEY_CODE key,
	SPECIALKEY specialKey)
{
	switch (specialKey) {
	case NONE:
		return _keys[key];
	case CONTROL:
		return _keysCtrl[key];
	case SHIFT:
		return _keysShift[key];
	}
	return false;
}

void bbm::IMyEventReceiver::resetKey(irr::EKEY_CODE key,
	SPECIALKEY specialKey)
{
	switch (specialKey) {
	case NONE:
		_keys[key] = false;
		break;
	case CONTROL:
		_keysCtrl[key] = false;
		break;
	case SHIFT:
		_keysShift[key] = false;
		break;
	}
}

void bbm::IMyEventReceiver::resetKeys()
{
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; ++i) {
		_keys[i] = false;
		_keysCtrl[i] = false;
		_keysShift[i] = false;
	}
}
