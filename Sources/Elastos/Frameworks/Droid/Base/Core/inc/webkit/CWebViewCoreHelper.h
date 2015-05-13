
#ifndef _CWEBVIEWCOREHELPER_H_
#define _CWEBVIEWCOREHELPER_H_

#include "_CWebViewCoreHelper.h"

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

#endif //_CWEBVIEWCOREHELPER_H_