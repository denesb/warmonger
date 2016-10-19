#include <QtQml>

#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMiniMap.h"
#include "ui/UI.h"
#include "utils/Constants.h"

namespace warmonger {
namespace ui {

void initUI()
{
    const char* const applicationName = utils::applicationName.toStdString().c_str();

    qmlRegisterType<CampaignMiniMap>(applicationName, 1, 0, "CampaignMiniMap");
    qmlRegisterType<CampaignMapEditor>(applicationName, 1, 0, "CampaignMapEditor");
}

} // namespace ui
} // namespace warmonger
