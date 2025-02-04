/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ConfirmNewBaseState.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextButton.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Region.h"
#include "../Savegame/Country.h"
#include "../Mod/RuleRegion.h"
#include "../Mod/RuleCountry.h"
#include "../Savegame/Base.h"
#include "BaseNameState.h"
#include "../Menu/ErrorMessageState.h"
#include "../Engine/Options.h"
#include "../Engine/Unicode.h"
#include "../Mod/RuleInterface.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Confirm New Base window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to place.
 * @param globe Pointer to the Geoscape globe.
 */
ConfirmNewBaseState::ConfirmNewBaseState(Base *base, Globe *globe) : _base(base), _globe(globe), _cost(0)
{
	_screen = false;

	// Create objects
	_window = new Window(this, 224, 72, 16, 64);
	_btnOk = new TextButton(54, 12, 68, 104);
	_btnCancel = new TextButton(54, 12, 138, 104);
	_txtCost = new Text(120, 9, 68, 80);
	_txtArea = new Text(140, 9, 68, 90);

	// Set palette
	setInterface("geoscape");

	add(_window, "genericWindow", "geoscape");
	add(_btnOk, "genericButton2", "geoscape");
	add(_btnCancel, "genericButton2", "geoscape");
	add(_txtCost, "genericText", "geoscape");
	add(_txtArea, "genericText", "geoscape");

	centerAllSurfaces();

	// Set up objects
	setWindowBackground(_window, "geoscape");

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&ConfirmNewBaseState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&ConfirmNewBaseState::btnOkClick, Options::keyOk);

	_btnCancel->setText(tr("STR_CANCEL_UC"));
	_btnCancel->onMouseClick((ActionHandler)&ConfirmNewBaseState::btnCancelClick);
	_btnCancel->onKeyboardPress((ActionHandler)&ConfirmNewBaseState::btnCancelClick, Options::keyCancel);

	Region *r = Region::getRegion(_game->getSavedGame()->getRegions(), _base->getLongitude(), _base->getLatitude());

	_cost = r->getRules()->getBaseCost();
	_txtCost->setText(tr("STR_COST_").arg(Unicode::formatFunding(_cost)));

	std::string region = tr(r->getRules()->getType());
	std::string country = tr(Country::getCountryName(_game->getSavedGame()->getCountries(), _base->getLongitude(), _base->getLatitude()));
	if (country.empty())
	{
		_txtArea->setText(tr("STR_AREA_").arg(region));
	}
	else
	{
		_txtArea->setText(tr("STR_AREA_").arg(tr("STR_COUNTRIES_COMMA").arg(country).arg(region)));
	}
	if (_txtArea->getTextWidth() > _txtArea->getWidth())
	{
		_txtArea->setX(_window->getX() + 6);
		_txtArea->setWidth(_window->getWidth() - 12);
		_txtArea->setAlign(ALIGN_CENTER);
	}
}

/**
 *
 */
ConfirmNewBaseState::~ConfirmNewBaseState()
{

}

/**
 * Go to the Place Access Lift screen.
 * @param action Pointer to an action.
 */
void ConfirmNewBaseState::btnOkClick(Action *)
{
	if (_game->getSavedGame()->getFunds() >= _cost)
	{
		_game->getSavedGame()->setFunds(_game->getSavedGame()->getFunds() - _cost);
		_game->getSavedGame()->getBases()->push_back(_base);
		_game->pushState(new BaseNameState(_base, _globe, false, false));
	}
	else
	{
		_game->pushState(new ErrorMessageState(tr("STR_NOT_ENOUGH_MONEY"), _palette, _game->getMod()->getInterface("geoscape")->getElement("genericWindow")->color, "BACK01.SCR", _game->getMod()->getInterface("geoscape")->getElement("palette")->color));
	}
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void ConfirmNewBaseState::btnCancelClick(Action *)
{
	_globe->onMouseOver(0);
	_game->popState();
}

}
