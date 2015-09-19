
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWCOREHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWCOREHELPER_H__

#include "_Elastos_Droid_Webkit_CWebViewCoreHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewCoreHelper)
{
public:
	/**
     * Causes all timers to pause. This applies to all WebViews in the current
     * app process.
     */
    CARAPI PauseTimers();

    /**
     * Resume all timers. This applies to all WebViews in the current process.
     */
    CARAPI ResumeTimers();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CWEBVIEWCOREHELPER_H__
