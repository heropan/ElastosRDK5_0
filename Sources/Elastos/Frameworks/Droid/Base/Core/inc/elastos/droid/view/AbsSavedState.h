
#ifndef __ELASTOS_DROID_VIEW_ABSSAVEDSTATE_H__
#define __ELASTOS_DROID_VIEW_ABSSAVEDSTATE_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace View {

class AbsSavedState
    : public Object
{
protected:
    AbsSavedState();

    AbsSavedState(
        /* [in] */ IParcelable* superState);

    AbsSavedState(
        /* [in] */ IParcel* source);

public:
    CARAPI_(AutoPtr<IParcelable>) GetSuperState();

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual ~AbsSavedState();

protected:
    virtual CARAPI Init(
        /* [in] */ IParcelable* superState);

    virtual CARAPI Init(
        /* [in] */ IParcel* source);

    virtual CARAPI Init();

public:
    static const AutoPtr<IAbsSavedState> EMPTY_STATE;

private:
    AutoPtr<IParcelable> mSuperState;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ABSSAVEDSTATE_H__
