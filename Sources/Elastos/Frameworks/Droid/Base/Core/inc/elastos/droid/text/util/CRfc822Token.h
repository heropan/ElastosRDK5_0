
#ifndef __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKEN_H__
#define __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKEN_H__

#include "_Elastos_Droid_Text_Utility_CRfc822Token.h"
#include "elastos/droid/text/utility/Rfc822Token.h"

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

#endif // __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKEN_H__
