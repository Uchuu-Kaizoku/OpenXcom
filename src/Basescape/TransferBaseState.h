#pragma once
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
#include "../Engine/ListState.h"
#include <vector>

namespace OpenXcom
{

class TextButton;
class Window;
class Text;
class TextList;
class Base;
class DebriefingState;

/**
 * Window that lets the player pick the base
 * to transfer items to.
 */
class TransferBaseState : public ListState
{
private:
	Base *_base;
	DebriefingState *_debriefingState;
	TextButton *_btnCancel;
	Window *_window;
	Text *_txtTitle, *_txtFunds, *_txtName, *_txtArea;
	TextList *_lstBases;
	std::vector<Base*> _bases;
public:
	/// Creates the Transfer Base state.
	TransferBaseState(Base *base, DebriefingState *debriefingState);
	/// Cleans up the Transfer Base state.
	~TransferBaseState();
	/// Handler for clicking the Cancel button.
	void btnCancelClick(Action *action);
	/// Handler for clicking the Bases list.
	void lstBasesClick(Action *action);
};

}
