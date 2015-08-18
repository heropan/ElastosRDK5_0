#ifndef __ELASTOS_DROID_TEXT_CALLCAPS_H__
#define __ELASTOS_DROID_TEXT_CALLCAPS_H__

#include "_Elastos_Droid_Text_CAllCaps.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAllCaps)
{
public:
    CARAPI Filter(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest,
        /* [in] */ Int32 dstart,
        /* [in] */ Int32 dend,
        /* [out] */ ICharSequence** cs);
};

}//namespace Text
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CALLCAPS_H__
