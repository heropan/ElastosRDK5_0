
#ifndef __CMESSAGEHELPER_H__
#define __CMESSAGEHELPER_H__

#include "_CMessageHelper.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CMessageHelper)
{
public:
    CARAPI Obtain(
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx(
        /* [in] */ IMessage* orig,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx2(
        /* [in] */ IHandler* target,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx3(
        /* [in] */ IHandler* target,
        /* [in] */ Int32 what,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx4(
        /* [in] */ IHandler* target,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx5(
        /* [in] */ IHandler* target,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx6(
        /* [in] */ IHandler* target,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [out] */ IMessage** msg);

    CARAPI ObtainEx7(
        /* [in] */ IHandler* target,
        /* [in] */ IRunnable* callback,
        /* [out] */ IMessage** msg);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CMESSAGEHELPER_H__
