
#ifndef __ELASTOS_DROID_SERVER_NOTIFICATION_GLOBALSORTKEYCOMPARATOR_H__
#define __ELASTOS_DROID_SERVER_NOTIFICATION_GLOBALSORTKEYCOMPARATOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::IComparator;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Notification {

/**
 * Sorts notifications by their global sort key.
 */
class GlobalSortKeyComparator
    : public Object
    , public IComparator
{
public:
    CAR_INTERFACE_DECL();

    GlobalSortKeyComparator();

    ~GlobalSortKeyComparator();

    // @Override
    CARAPI Compare(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs,
        /* [out] */ Int32* result);
};

} // Notification
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_NOTIFICATION_GLOBALSORTKEYCOMPARATOR_H__
