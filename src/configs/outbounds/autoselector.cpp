#include "configs/outbounds/autoselector.h"

#include "database/DatabaseManager.h"
#include "database/GroupsRepo.h"
#include "database/entities/Group.h"

namespace Configs
{
    QString autoSelector::DisplayAddress()
    {
        if (gid < 0) return QObject::tr("no group");
        auto group = dataManager->groupsRepo->GetGroup(gid);
        if (group == nullptr) return QObject::tr("missing group");
        return group->name;
    }
}
