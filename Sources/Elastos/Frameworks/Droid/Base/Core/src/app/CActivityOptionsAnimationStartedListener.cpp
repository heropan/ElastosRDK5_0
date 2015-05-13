
#include "app/CActivityOptionsAnimationStartedListener.h"
#include "app/CActivityOptions.h"
#include "os/Handler.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CActivityOptionsAnimationStartedListener::constructor(
    /* [in] */ IHandler* handler,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener)
{
    mHandler = handler;
    mListener = listener;
    return NOERROR;
}

ECode CActivityOptionsAnimationStartedListener::SendResult(
    /* [in] */ IBundle* data)
{
    if (mHandler != NULL) {
        Boolean result;
        AutoPtr<IRunnable> r = new AnimationStartedListenerRunnable(mListener);
        mHandler->Post(r, &result);
    }
    return NOERROR;
}

ECode CActivityOptionsAnimationStartedListener::ToString(
    /* [out]*/ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CActivityOptionsAnimationStartedListener");
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
