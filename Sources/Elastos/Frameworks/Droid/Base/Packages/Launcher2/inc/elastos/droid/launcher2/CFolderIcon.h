#ifndef  __ELASTOS_DROID_LAUNCHER2_CFOLDERICON_H__
#define  __ELASTOS_DROID_LAUNCHER2_CFOLDERICON_H__

#include "_Elastos_Droid_Launcher2_CFolderIcon.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/launcher2/FolderIcon.h"

namespace Elastos {
namespace Droid {
namespace Launcher2 {

/**
 * We use a custom tab view to process our own focus traversals.
 */
CarClass(CFolderIcon)
    , public FolderIcon
{
public:
    CAR_OBJECT_DECL();
};

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_LAUNCHER2_CFOLDERICON_H__