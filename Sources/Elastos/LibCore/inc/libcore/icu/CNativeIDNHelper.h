
#ifndef __ICU_CNATIVEIDNHELPER_H__
#define __ICU_CNATIVEIDNHELPER_H__

#include "_CNativeIDNHelper.h"

using Elastos::Core::ICharSequence;

namespace Libcore {
namespace ICU {

CarClass(CNativeIDNHelper)
{
public:
    CARAPI ToASCII(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [out] */ String * str);

    CARAPI ToUnicode(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [out] */ String * str);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVEIDNHELPER_H__
