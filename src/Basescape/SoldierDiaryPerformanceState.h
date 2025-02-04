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
#include <string>

namespace OpenXcom
{

class TextButton;
class Window;
class Text;
class TextList;
class Base;
class SoldierDiaryOverviewState;
class Surface;
class Soldier;
class SoldierCommendations;

enum SoldierDiaryDisplay { DIARY_KILLS, DIARY_MISSIONS, DIARY_COMMENDATIONS };

/**
 * Diary screen that lists soldier totals.
 */
class SoldierDiaryPerformanceState : public ListState
{
private:
	Base *_base;
	size_t _soldierId;
	SoldierDiaryOverviewState *_soldierDiaryOverviewState;
	Soldier *_soldier;
	std::vector<Soldier*> *_list;

	TextButton *_btnOk, *_btnPrev, *_btnNext, *_btnKills, *_btnMissions, *_btnCommendations;
	Window *_window;
	Text *_txtTitle, *_txtMedalName, *_txtMedalLevel, *_txtMedalInfo;
	TextList *_lstPerformance, *_lstKillTotals, *_lstMissionTotals, *_lstCommendations;
	std::vector<std::string> _commendationsListEntry;
	std::vector<std::string> _commendationsNames;
	std::vector<std::pair<std::string, SoldierCommendations*> > _sortedCommendations;
	std::vector<Surface*> _commendations, _commendationDecorations;

	SoldierDiaryDisplay _display;
	int _lastScrollPos;
	TextButton *_group;

public:
	/// Creates the Soldier Diary Totals state.
	SoldierDiaryPerformanceState(Base *base, size_t soldierId, SoldierDiaryOverviewState *soldierDiaryState, SoldierDiaryDisplay display);
	/// Cleans up the Soldier Diary Totals state.
	~SoldierDiaryPerformanceState();
	/// Updates the soldier info.
	void init() override;
	/// Draw sprites.
	void drawSprites();
	/// Handler for clicking the OK button.
	void btnOkClick(Action *action);
	/// Handler for clicking the Previous button.
	void btnPrevClick(Action *action);
	/// Handler for clicking the Next button.
	void btnNextClick(Action *action);
	/// Handler for clicking the Kills button.
	void btnKillsToggle(Action *action);
	/// Handler for clicking the Missions button.
	void btnMissionsToggle(Action *action);
	/// Handler for clicking the Missions button.
	void btnCommendationsToggle(Action *action);
	/// Handler for moving the mouse over a medal.
	void lstInfoMouseOver(Action *action);
	/// Handler for moving the mouse outside the medals list.
	void lstInfoMouseOut(Action *action);
	/// Handler for clicking on a medal.
	void lstInfoMouseClick(Action *action);
	/// Runs state functionality every cycle.
	void think() override;
	/// Updates the scale with adjustments for arbitrary height lists.
	void resize(int &dX, int &dY) override;
};

}
