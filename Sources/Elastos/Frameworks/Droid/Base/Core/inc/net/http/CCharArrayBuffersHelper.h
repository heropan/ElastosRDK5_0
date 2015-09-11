
#ifndef __ELASTOS_DROID_NET_HTTP_CCHARARRAYBUFFERSHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CCHARARRAYBUFFERSHELPER_H__

#include "_Elastos_Droid_Net_Http_CCharArrayBuffersHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CCharArrayBuffersHelper)
{
public:
    CARAPI ContainsIgnoreCaseTrimmed(
        /* [in] */ Org::Apache::Http::Util::ICharArrayBuffer* buffer,
        /* [in] */ Int32 beginIndex,
        /* [in] */ const String& str,
        /* [out] */ Boolean* result);

    CARAPI SetLowercaseIndexOf(
        /* [in] */ Org::Apache::Http::Util::ICharArrayBuffer* buffer,
        /* [in] */ Int32 ch,
        /* [out] */ Int32* index);

private:
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CCHARARRAYBUFFERSHELPER_H__
