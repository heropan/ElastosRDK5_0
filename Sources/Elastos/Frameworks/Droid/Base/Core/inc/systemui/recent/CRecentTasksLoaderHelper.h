#ifndef __CRECENTTASKSLOADERHELPER_H_
#define __CRECENTTASKSLOADERHELPER_H_

#include "_CRecentTasksLoaderHelper.h"

using Elastos::Droid::SystemUI::Recent::IRecentTasksLoader;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

CarClass(CRecentTasksLoaderHelper)
{
public:
    GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IRecentTasksLoader** loader);
};

}// namespace Recent
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CRECENTTASKSLOADERHELPER_H_
