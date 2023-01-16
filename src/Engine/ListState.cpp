
#include "ListState.h"
#include "../Engine/Screen.h"
#include "../Engine/Game.h"
#include "../Engine/Options.h"
#include "../Interface/ComboBox.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextEdit.h"
#include "../Interface/TextList.h"

namespace OpenXcom
{

/**
 * Centers and adjusts height of all the surfaces on the screen depending on the oxceListsHeightPercentage option.
 */
void ListState::centerAllSurfaces()
{
	if (Options::oxceListsScreenHeightPercentage == 0)
		return State::centerAllSurfaces();

	// Find main window. At the time of writing there aren't any relevant game states with more than one window.
	for (std::vector<Surface *>::iterator i = _surfaces.begin(); i != _surfaces.end(); ++i)
	{
		if (Window *w = dynamic_cast<Window *>(*i))
		{
			_mainWindow = w;
			break;
		}
	}

	Screen *screen = _game->getScreen();
	int oldMainWindowHeight = _mainWindow->getHeight();
	// int newMainWindowHeight = ((double)oldMainWindowHeight / screen->ORIGINAL_HEIGHT) * (Options::baseYResolution * (Options::oxceListsScreenHeightPercentage / 100.0));
	// The below is not really correct but prefer short windows to be slightly more expanded. The above line would more accurately keep to their original screen ratio.
	int newMainWindowHeight = std::max(0.0, oldMainWindowHeight + (Options::baseYResolution * (Options::oxceListsScreenHeightPercentage / 100.0)) - screen->ORIGINAL_HEIGHT);
	_mainWindow->setHeight(newMainWindowHeight);

	int oldMainWindowY = _mainWindow->getY();
	_mainWindow->setY((Options::baseYResolution - newMainWindowHeight) / 2);

	int mainWindowDeltaHeight = newMainWindowHeight - oldMainWindowHeight;
	int mainWindowDeltaY = _mainWindow->getY() - oldMainWindowY;

	for (std::vector<Surface *>::iterator i = _surfaces.begin(); i != _surfaces.end(); ++i)
	{
		(*i)->setX((*i)->getX() + screen->getDX());

		if ((*i) == _mainWindow)
			continue;

		// Assume most things on the lower half of the screen should stay on the bottom.
		if (dynamic_cast<TextButton *>(*i) || dynamic_cast<ComboBox *>(*i) || dynamic_cast<Text *>(*i))
		{
			int y = (*i)->getY();
			if (y > screen->ORIGINAL_HEIGHT / 2)
			{
				(*i)->setY(y + mainWindowDeltaHeight);
				setAnchoredBottom(*i);
			}
		}
		// Expand TextList, keep height pegged to the anchored Surfaces later.
		else if (dynamic_cast<TextList *>(*i))
		{
			// Diary performance screens using TextLists as single line text displays makes this harder than it should be so we're back to making more assumptions.
			if ((*i)->getY() > screen->ORIGINAL_HEIGHT / 2)
			{
				(*i)->setY((*i)->getY() +  mainWindowDeltaHeight);
				setAnchoredBottom(*i);
			}
			else
			{
				int h = (*i)->getHeight() + mainWindowDeltaHeight;
				(*i)->setHeight(std::max(0, h - (h % 8))); // TextLists only render properly with multiples of 8
			}
		}

		(*i)->setY((*i)->getY() + mainWindowDeltaY);
	}
}

/**
 * Updates the scale with adjustments for arbitrary height lists.
 * @param dX delta of X;
 * @param dY delta of Y;
 */
void ListState::resize(int &dX, int &dY)
{
	if (Options::oxceListsScreenHeightPercentage == 0)
		return State::resize(dX, dY);

	int oldMainWindowHeight = _mainWindow->getHeight();
	int newMainWindowHeight = ((double)oldMainWindowHeight / (Options::baseYResolution - dY)) * Options::baseYResolution;
	_mainWindow->setHeight(newMainWindowHeight);

	int oldMainWindowY = _mainWindow->getY();
	_mainWindow->setY((Options::baseYResolution - newMainWindowHeight) / 2);

	int mainWindowDeltaHeight = newMainWindowHeight - oldMainWindowHeight;
	int mainWindowDeltaY = _mainWindow->getY() - oldMainWindowY;

	int textlistAnchorY = _mainWindow->getY() + newMainWindowHeight - 24; // Just in case
	std::vector<TextList *> textlists;
	for (std::vector<Surface *>::const_iterator i = _surfaces.begin(); i != _surfaces.end(); ++i)
	{
		(*i)->setX((*i)->getX() + dX / 2);

		if ((*i) == _mainWindow)
			continue;

		if (isAnchoredBottom(*i))
		{
			int y = (*i)->getY() + mainWindowDeltaY + mainWindowDeltaHeight;
			textlistAnchorY = std::min(textlistAnchorY, y);
			(*i)->setY(y);
		}
		else
		{
			(*i)->setY((*i)->getY() + mainWindowDeltaY);
		}

		// Do TextList height later when we are sure we have lowerButtonsY.
		if (TextList *tl = dynamic_cast<TextList *>(*i))
		{
			textlists.push_back(tl);
		}
	}
	while (!textlists.empty())
	{
		TextList *tl = textlists.back();
		int h = textlistAnchorY - 2 - tl->getY();// Use a point 2 pixels above the lower buttons as anchor
		tl->setHeight(std::max(8, h - (h % 8))); // Set height a multiple of 8 for proper rendering

		textlists.pop_back();
	}
}

/**
 * Anchor to the bottom of the screen.
 * @param surface Pointer to Surface;
 * @param anchor Boolean whether to anchor or not;
 */
void ListState::setAnchoredBottom(Surface *surface, bool anchor)
{
	_anchoredSurfaces[surface] = anchor;
}

/**
 * Is Surface anchored to the bottom of the screen?
 * @param surface Pointer to Surface;
 */
bool ListState::isAnchoredBottom(Surface *surface)
{
	std::unordered_map<Surface *, bool>::const_iterator it = _anchoredSurfaces.find(surface);
	if (it != _anchoredSurfaces.end())
	{
		return (*it).second;
	}
	return false;
}

}
