/** \file
 * CampaignMapWatcher class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef W_UI_CAMPAIGN_MAP_WATCHER_H
#define W_UI_CAMPAIGN_MAP_WATCHER_H

#include <QObject>

namespace warmonger {

namespace core {

class CampaignMap;
}

namespace ui {

/**
 * Dummy QObject used as an aggregator of signals from another object.
 *
 * Makes it easy to unsubscribe from a batch of signals with a single
 * disconnect call.
 */
struct Watcher : public QObject
{
    Q_OBJECT

    using QObject::QObject;

signals:
    /**
     * Should be emitted when any of the aggregated signals are emitted.
     */
    void changed();
};

/**
 * Watch a campaign-map and emit changed() whenever it changes.
 *
 * Only those changes are considered that cause a redraw of the campaign-map.
 */
class CampaignMapWatcher : public QObject
{
    Q_OBJECT

public:
    /**
     * Construct a CampaignMapWatcher object.
     *
     * \param campaignMap the campaign-map to watch
     * \parent parent the parent QObject
     */
    CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent = nullptr);

signals:
    /**
     * Emitted when the campaign-map is changed.
     */
    void changed();

private:
    void connectMapNodeSignals();
    void connectSettlementSignals();
    void connectArmySignals();
    void onMapNodesChanged();
    void onSettlementsChanged();
    void onArmiesChanged();

private:
    const core::CampaignMap* const campaignMap;
    Watcher* mapNodeWatcher;
    Watcher* settlementWatcher;
    Watcher* armyWatcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_WATCHER_H
