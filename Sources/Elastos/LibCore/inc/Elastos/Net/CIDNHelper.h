
#ifndef __CIDNHELPER_H__
#define __CIDNHELPER_H__

#include "_CIDNHelper.h"

namespace Elastos {
namespace Net {

CarClass(CIDNHelper)
{
public:
    CARAPI ToASCIIEx(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    CARAPI ToASCII(
        /* [in] */ const String& input,
        /* [out] */ String* result);

    CARAPI ToUnicodeEx(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    CARAPI ToUnicode(
        /* [in] */ const String& input,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif //__CIDNHELPER_H__
