
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

	Screen *screen = _game->getScreen();
	double heightMultiplier = ((double)Options::oxceListsScreenHeightPercentage / 100);
	int screenHeightDelta = (Options::baseYResolution * heightMultiplier) - screen->ORIGINAL_HEIGHT;
	for (std::vector<Surface *>::iterator i = _surfaces.begin(); i != _surfaces.end(); ++i)
	{
		(*i)->setX((*i)->getX() + screen->getDX());

		// Assume TextButtons and ComboBoxes on the lower half of the screen should stay on the bottom, make the rest static.
		if (dynamic_cast<TextButton *>(*i) || dynamic_cast<ComboBox *>(*i))
		{
			int y = (*i)->getY();
			if (y > screen->ORIGINAL_HEIGHT / 2)
			{
				(*i)->setY(y + screenHeightDelta);
			}
			else
			{
				setStatic(*i);
			}
		}
		// Expand main Window height to fill the screen.
		else if (dynamic_cast<Window *>(*i))
		{
			// TODO: Consider that there might be child windows that shouldn't be resized?
			(*i)->setHeight(std::max(0, (*i)->getHeight() + screenHeightDelta));
		}
		// Expand TextList, keep height pegged to the bottom TextButtons/ComboBoxes later.
		else if (dynamic_cast<TextList *>(*i))
		{
			// Diary performance screens using TextLists as single line text displays makes this harder than it should be so we're back to making more assumptions.
			if ((*i)->getY() > screen->ORIGINAL_HEIGHT / 2)
			{
				(*i)->setY((*i)->getY() + screenHeightDelta);
				setAnchoredBottom(*i);
			}
			else
			{
				int h = (*i)->getHeight() + screenHeightDelta;
				(*i)->setHeight(std::max(0, h - (h % 8))); // TextLists only render properly with multiples of 8
			}
		}
		// More special diary screen handling
		else if (Text* t = dynamic_cast<Text *>(*i))
		{
			if (t->getY() == 135 && t->getText() == "") // _txtMedalInfo
			{
				(*i)->setY((*i)->getY() + screenHeightDelta);
				setAnchoredBottom(t);
			}
		}

		(*i)->setY((*i)->getY() + (Options::baseYResolution * (1 - heightMultiplier)) / 2);
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

	int textlistAnchorY = Options::baseYResolution - 24;
	std::vector<TextList *> textlists;
	for (std::vector<Surface *>::const_iterator i = _surfaces.begin(); i != _surfaces.end(); ++i)
	{
		(*i)->setX((*i)->getX() + dX / 2);

		if (isStatic(*i))
			continue;

		if (isAnchoredBottom(*i))
		{
			(*i)->setY((*i)->getY() + dY);
		}
		// Bottom TextButtons and ComboBoxes, keep track of Y position for the TextList.
		else if (dynamic_cast<TextButton *>(*i) || dynamic_cast<ComboBox *>(*i))
		{
			int y = (*i)->getY() + dY;
			textlistAnchorY = std::min(textlistAnchorY, y);
			(*i)->setY(y);
		}
		// Window handling.
		else if (dynamic_cast<Window *>(*i))
		{
			(*i)->setHeight(std::max(0, (*i)->getHeight() + dY));
		}
		// TextLists are usually added after buttons but don't rely on it, process them later when we are sure we have lowerButtonsY.
		else if (TextList *tl = dynamic_cast<TextList *>(*i))
		{
			textlists.push_back(tl);
		}
	}
	while (!textlists.empty())
	{
		TextList *tl = textlists.back();
		int h = textlistAnchorY - 2 - tl->getY();  // Use a point 2 pixels above the lower buttons as anchor
		tl->setHeight(std::max(8, h - (h % 8))); // Set height a multiple of 8 for proper rendering

		textlists.pop_back();
	}
}

/**
 * Prevent a Surface from being moved and resized.
 * @param surface Pointer to Surface;
 * @param bStatic Set static or not;
 */
void ListState::setStatic(Surface* surface, bool bStatic)
{
	_staticSurfaces[surface] = bStatic;
}

/**
 * Is Surface static?
 * @param surface Pointer to Surface;
 */
bool ListState::isStatic(Surface *surface)
{
	std::map<Surface *, bool>::const_iterator it = _staticSurfaces.find(surface);
	if (it != _staticSurfaces.end())
	{
		return (*it).second;
	}
	return false;
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
	std::map<Surface *, bool>::const_iterator it = _anchoredSurfaces.find(surface);
	if (it != _anchoredSurfaces.end())
	{
		return (*it).second;
	}
	return false;
}

}
