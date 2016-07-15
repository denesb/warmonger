#include <QSGSimpleTextureNode>

#include "ui/CampaignMapEditor.h"
#include "ui/MapUtil.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    QQuickItem(parent),
    campaignMap(nullptr)
{
    QObject::connect(this, &CampaignMapEditor::widthChanged, this, &CampaignMapEditor::updateGeometry);
    QObject::connect(this, &CampaignMapEditor::heightChanged, this, &CampaignMapEditor::updateGeometry);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &CampaignMapEditor::windowRectChanged);
}

core::CampaignMap* CampaignMapEditor::getCampaignMap() const
{
    return this->campaignMap;
}

void CampaignMapEditor::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        this->campaignMap = campaignMap;
        this->updateContent();

        emit campaignMapChanged();
    }
}

WorldSurface* CampaignMapEditor::getWorldSurface() const
{
    return this->worldSurface;
}

void CampaignMapEditor::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

QRect CampaignMapEditor::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

QSGNode* CampaignMapEditor::updatePaintNode(QSGNode *, UpdatePaintNodeData *)
{
    return nullptr;
    //QSGSimpleTextureNode *node = new QSGSimpleTextureNode();
}

void CampaignMapEditor::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty()
            || this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setFlags(0);
        return;
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);
    }

    this->nodesPos = positionNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());
}

void CampaignMapEditor::updateGeometry()
{
    const QPoint topLeft = this->mapWindow.getWindowRect().topLeft();
    this->mapWindow.setWindowRect(QRect(topLeft.x(), topLeft.y(), this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
