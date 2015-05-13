
#ifndef  __ANIMATORLISTENERADAPTER_H__
#define  __ANIMATORLISTENERADAPTER_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Animation {

class AnimatorListenerAdapter
    : public ElRefBase
    , public IAnimatorListener
{
public:
    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI OnAnimationStart(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationCancel(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationRepeat(
        /* [in] */ IAnimator* animation);
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif //__ANIMATORLISTENERADAPTER_H__
