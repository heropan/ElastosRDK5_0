
#ifndef __NATIVEIDN_H__
#define __NATIVEIDN_H__

#include <elastos.h>
#include <cmdef.h>
#include "Elastos.CoreLibrary_server.h"

namespace Libcore {
namespace ICU {

class NativeIDN
{
public:
    static CARAPI_(String) ToASCII(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags);

    static CARAPI_(String) ToUnicode(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags);

private:
    NativeIDN() {}

    static CARAPI Convert(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean toAscii,
        /* [out] */ String* result);

    static CARAPI ConvertImpl(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean toAscii,
        /* [out] */ String* result);
};

} // namespace ICU
} // namespace Libcore

#endif //__NATIVEIDN_H__
