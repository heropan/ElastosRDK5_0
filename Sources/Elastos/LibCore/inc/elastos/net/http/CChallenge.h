
#ifndef __CCHALLENGE_H__
#define __CCHALLENGE_H__

#include "_CChallenge.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CChallenge)
{
public:
    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& realm);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* value);

    CARAPI GetHashCode(
        /* [out] */ Int32* value);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    String mScheme;
    String mRealm;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CCHALLENGE_H__
