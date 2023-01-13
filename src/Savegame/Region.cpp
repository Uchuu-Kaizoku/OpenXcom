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
#include "Region.h"
#include "../Mod/RuleRegion.h"

namespace OpenXcom
{

/**
 * Initializes a region of the specified type.
 * @param rules Pointer to ruleset.
 */
Region::Region(RuleRegion *rules): _rules(rules)
{
	_activityAlien.push_back(0);
	_activityXcom.push_back(0);
}

/**
 *
 */
Region::~Region()
{
}

/**
 * Loads the region from a YAML file.
 * @param node YAML node.
 */
void Region::load(const YAML::Node &node)
{
	_activityXcom = node["activityXcom"].as< std::vector<int> >(_activityXcom);
	_activityAlien = node["activityAlien"].as< std::vector<int> >(_activityAlien);
}

/**
 * Saves the region to a YAML file.
 * @return YAML node.
 */
YAML::Node Region::save() const
{
	YAML::Node node;
	node["type"] = _rules->getType();
	node["activityXcom"] = _activityXcom;
	node["activityAlien"] = _activityAlien;
	return node;
}

/**
 * Returns the ruleset for the region's type.
 * @return Pointer to ruleset.
 */
RuleRegion *Region::getRules() const
{
	return _rules;
}

/**
 * Adds to the region's xcom activity level.
 * @param activity Amount to add.
 */
void Region::addActivityXcom(int activity)
{
	_activityXcom.back() += activity;
}

/**
 * Adds to the region's alien activity level.
 * @param activity Amount to add.
 */
void Region::addActivityAlien(int activity)
{
	_activityAlien.back() += activity;
}

/**
 * Gets the region's xcom activity level.
 * @return activity level.
 */
std::vector<int> &Region::getActivityXcom()
{
	return _activityXcom;
}

/**
 * Gets the region's alien activity level.
 * @return activity level.
 */
std::vector<int> &Region::getActivityAlien()
{
	return _activityAlien;
}

/**
 * Store last month's counters, start new counters.
 */
void Region::newMonth()
{
	_activityAlien.push_back(0);
	_activityXcom.push_back(0);
	if (_activityAlien.size() > 12)
		_activityAlien.erase(_activityAlien.begin());
	if (_activityXcom.size() > 12)
		_activityXcom.erase(_activityXcom.begin());
}

/**
 * get region at globe coordinates
 * @param regions Pointer to a vector of Region pointers like in SavedGame::getRegions()
 * @param lon Longitude in radians.
 * @param lat Latitude in radians.
 * @return Region pointer or 0
 */
Region* Region::getRegion(std::vector<Region *> *regions, double lon, double lat)
{
	for (std::vector<Region *>::iterator i = regions->begin(); i != regions->end(); ++i)
	{
		if ((*i)->getRules()->insideRegion(lon, lat))
		{
			return *i;
		}
	}
	return 0;
}

/**
 * get region name at globe coordinates
 * @param regions Pointer to a vector of Region pointers like in SavedGame::getRegions()
 * @param lon Longitude in radians.
 * @param lat Latitude in radians.
 * @return unique region language string or empty string
 */
std::string Region::getRegionName(std::vector<Region *> *regions, double lon, double lat)
{
	Region *r = getRegion(regions, lon, lat);
	if (r != 0)
		return r->getRules()->getType();
	return "";
}

}
