/**
 * Copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "core/Utils.h"

#include <algorithm>
#include <iterator>
#include <random>

#include "core/Faction.h"
#include "core/World.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

std::tuple<Banner*, Color*, Color*> nextAvailableCombination(
    const std::vector<Faction*>& factions, const std::vector<Banner*>& banners, const std::vector<Color*>& colors)
{
    typedef std::tuple<Banner*, Color*, Color*> Combination;

    std::vector<Combination> usedCombinations;

    for (const auto& faction : factions)
    {
        usedCombinations.emplace_back(faction->getBanner(), faction->getPrimaryColor(), faction->getSecondaryColor());
    }

    std::vector<Banner*> shuffledBanners(banners);
    std::vector<Color*> shuffledColors(colors);
    std::random_device rd;
    std::mt19937 mtd(rd());

    std::shuffle(shuffledBanners.begin(), shuffledBanners.end(), mtd);
    std::shuffle(shuffledColors.begin(), shuffledColors.end(), mtd);

    std::uniform_int_distribution<std::size_t> bannersDist(0, banners.size() - 1);
    std::uniform_int_distribution<std::size_t> colorsDist(0, colors.size() - 1);

    Combination nextCombination;
    do
    {
        std::size_t primaryColorIndex = colorsDist(mtd);
        std::size_t secondaryColorIndex = colorsDist(mtd);

        if (secondaryColorIndex == primaryColorIndex)
            secondaryColorIndex = (secondaryColorIndex + 1) % colors.size();

        nextCombination =
            std::make_tuple(banners.at(bannersDist(mtd)), colors.at(primaryColorIndex), colors.at(secondaryColorIndex));
    } while (std::find(usedCombinations.begin(), usedCombinations.end(), nextCombination) != usedCombinations.end());

    return nextCombination;
}

} // namespace core
} // namespace warmonger
