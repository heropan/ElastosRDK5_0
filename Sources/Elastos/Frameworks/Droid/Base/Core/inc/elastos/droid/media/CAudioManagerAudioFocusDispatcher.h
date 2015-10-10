
#ifndef __ELASTOS_DROID_MEDIA_CAUDIOMANAGERAUDIOFOCUSDISPATCHER_H__
#define __ELASTOS_DROID_MEDIA_CAUDIOMANAGERAUDIOFOCUSDISPATCHER_H__

#include "_Elastos_Droid_Media_CAudioManagerAudioFocusDispatcher.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioManagerAudioFocusDispatcher)
{
public:
    CARAPI constructor(
        /* [in] */ IAudioManager* owner);

    CARAPI DispatchAudioFocusChange(
        /* [in] */ Int32 focusChange,
        /* [in] */ const String& id);

private:
    AutoPtr<IWeakReference> mOwnerWeak;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CAUDIOMANAGERAUDIOFOCUSDISPATCHER_H__
