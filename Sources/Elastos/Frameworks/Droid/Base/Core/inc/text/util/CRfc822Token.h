
#ifndef __CRFC822TOKEN_H__
#define __CRFC822TOKEN_H__

#include "_CRfc822Token.h"
#include "text/util/Rfc822Token.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

CarClass(CRfc822Token)
    , public Rfc822Token
{
public:
    CARAPI GetName(
        /* [out] */ String* ret);

    CARAPI GetAddress(
        /* [out] */ String* ret);

    CARAPI GetComment(
        /* [out] */ String* ret);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI SetAddress(
        /* [in] */ const String& address);

    CARAPI SetComment(
        /* [in] */ const String& comment);

    CARAPI ToString(
        /* [out] */ String* ret);

    CARAPI GetHashCode(
        /* [out] */ Int32* ret);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* ret);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& address,
        /* [in] */ const String& comment);

};

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __CRFC822TOKEN_H__
