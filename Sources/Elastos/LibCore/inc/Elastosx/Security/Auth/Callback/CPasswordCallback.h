
#ifndef __CPASSWORDCALLBACK_H__
#define __CPASSWORDCALLBACK_H__

#include "_CPasswordCallback.h"

namespace Elastosx {
namespace Security {
namespace Auth {
namespace Callback {

CarClass(CPasswordCallback)
{
public:
    CPasswordCallback();

    CARAPI GetPrompt(
        /* [out] */ String *prompt);

    CARAPI IsEchoOn(
        /* [out] */ Boolean *echoOn);

    CARAPI SetPassword(
        /* [in] */ const ArrayOf<Char32>& password);

    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32>** password);

    CARAPI ClearPassword();

    CARAPI constructor(
        /* [in] */ const String& prompt,
        /* [in] */ Boolean echoOn);

private:
    CARAPI SetPrompt(
        /* [in] */ const String& prompt);

public:
    Boolean mEchoOn;

private:
    static const Int64 serialVersionUID;

    const String mPrompt;

    AutoPtr<ArrayOf<Char32> > mInputPassword;
};

}
}
}
}

#endif // __CPASSWORDCALLBACK_H__
