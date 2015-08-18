
#ifndef __ELASTOS_DROID_VIEW_CINPUTQUEUE_H__
#define __ELASTOS_DROID_VIEW_CINPUTQUEUE_H__

#include "_Elastos_Droid_View_CInputQueue.h"

namespace Elastos {
namespace Droid {
namespace View {

/**
 * An input queue provides a mechanism for an application to receive incoming
 * input events.  Currently only usable from native code.
 */

CarClass(CInputQueue)
{
public:
    CARAPI constructor(
        /* [in] */ IInputChannel* channel);

    CARAPI GetInputChannel(
        /* [out] */ IInputChannel** inputChannel);

private:
    AutoPtr<IInputChannel> mChannel;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CINPUTQUEUE_H__
