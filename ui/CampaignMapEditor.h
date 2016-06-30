#ifndef UI_CAMPAIGN_MAP_EDITOR_H
#define UI_CAMPAIGN_MAP_EDITOR_H

#include <QtQuick/QQuickItem>

#include "core/CampaignMap.h"

namespace warmonger {
namespace ui {

class CampaignMapEditor :
    public QQuickItem
{
    Q_OBJECT

public:
    CampaignMapEditor(QQuickItem *parent = nullptr);

    QSGNode * updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *oldNodeData) override;

private:
    core::CampaignMap *map;
};

} // namespace ui
} // namespace warmonger

#endif // UI_CAMPAIGN_MAP_EDITOR_H
