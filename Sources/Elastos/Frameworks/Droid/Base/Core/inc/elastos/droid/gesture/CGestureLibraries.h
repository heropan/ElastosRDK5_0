#ifndef __ELASTOS_DROID_GESTURE_CGESTURELIBRARIES_H__
#define __ELASTOS_DROID_GESTURE_CGESTURELIBRARIES_H__

#include "_Elastos_Droid_Gesture_CGestureLibraries.h"
#include "elastos/droid/gesture/GestureLibrary.h"
#include "elastos/droid/gesture/GestureLibraries.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGestureLibraries)
    , public GestureLibraries
{
public:
    CAR_OBJECT_DECL();

    CGestureLibraries();
    virtual ~CGestureLibraries();
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTURELIBRARIES_H__
