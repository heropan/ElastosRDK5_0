
#ifndef __CCHARARRAYBUFFERSHELPER_H__
#define __CCHARARRAYBUFFERSHELPER_H__

#include "_CCharArrayBuffersHelper.h"

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
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __CCHARARRAYBUFFERSHELPER_H__
