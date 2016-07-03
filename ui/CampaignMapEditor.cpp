#include <QSGSimpleTextureNode>

#include "ui/CampaignMapEditor.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    QQuickItem(parent),
    map(new core::CampaignMap())
{
}

QSGNode * CampaignMapEditor::updatePaintNode(QSGNode *, UpdatePaintNodeData *)
{
    return nullptr;
    //QSGSimpleTextureNode *node = new QSGSimpleTextureNode();
}

} // namespace ui
} // namespace warmonger
