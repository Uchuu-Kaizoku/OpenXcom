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
#include "State.h"
#include <unordered_map>

namespace OpenXcom
{

/**
 * Game state with automatic resizing of lists.
 */
class ListState : public State
{
  private:
	std::unordered_map<Surface *, bool> _anchoredSurfaces;
	Window *_mainWindow = 0;
  public:
	/// Centers and adjusts height of all the surfaces on the screen depending on the oxceListsHeightPercentage option.
	virtual void centerAllSurfaces() override;
	/// Updates the scale with adjustments for arbitrary height lists.
	virtual void resize(int &dX, int &dY) override;
	/// Anchor to the bottom of the screen.
	void setAnchoredBottom(Surface *surface, bool anchor = true);
	/// Is Surface anchored to the bottom of the screen?
	bool isAnchoredBottom(Surface *surface);
};

}
