#include <QtQml>

#include "ui/Banner.h"
#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMiniMap.h"
#include "ui/UI.h"
#include "utils/Constants.h"

namespace warmonger {
namespace ui {

void initUI()
{
    const char* const applicationName = utils::applicationName.toStdString().c_str();

    qmlRegisterType<Banner>(applicationName, 1, 0, "Banner");
    qmlRegisterType<CampaignMapEditor>(applicationName, 1, 0, "CampaignMapEditor");
    qmlRegisterType<CampaignMiniMap>(applicationName, 1, 0, "CampaignMiniMap");
}

} // namespace ui
} // namespace warmonger
