
#include "cmdef.h"
#include "CChallenge.h"

namespace Elastos {
namespace Net {
namespace Http {

ECode CChallenge::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& realm)
{
    mScheme = scheme;
    mRealm = realm;
    return NOERROR;
}

ECode CChallenge::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IChallenge> res = (IChallenge*)o->Probe(EIID_IChallenge);
    if (res) {
        *value = ((CChallenge*)res.Get())->mScheme.Equals(mScheme)
              && ((CChallenge*)res.Get())->mRealm.Equals(mRealm);
    }
    else {
        *value = FALSE;
    }

    return NOERROR;
}

ECode CChallenge::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)


    *value = mScheme.GetHashCode() + 31 * mRealm.GetHashCode();
    return NOERROR;
}

ECode CChallenge::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = String("Challenge[") + mScheme + String(" ") + mRealm + String("]");
    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
