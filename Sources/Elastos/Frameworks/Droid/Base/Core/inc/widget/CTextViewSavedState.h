#ifndef __CTEXTVIEWSAVEDSTATE_H_
#define __CTEXTVIEWSAVEDSTATE_H_

#include "_CTextViewSavedState.h"
#include "widget/TextViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CTextViewSavedState), public TextViewSavedState
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor();

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CTEXTVIEWSAVEDSTATE_H_
