
#include "ext/frameworkdef.h"
#include "content/pm/CVerifierDeviceIdentityHelper.h"
#include "content/pm/CVerifierDeviceIdentity.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CVerifierDeviceIdentityHelper::Generate(
    /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity)
{
    AutoPtr<Elastos::Security::ISecureRandom> sr;
    // TODO:
    // CSecureRandom::New((Elastos::Security::ISecureRandom**)&sr);
    return Generate(sr, identity);
}

ECode CVerifierDeviceIdentityHelper::Generate(
        /* [in] */ Elastos::Security::ISecureRandom* rng,
        /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity)
{
    Int64 lIdentity;
    rng->NextLong(&lIdentity);
    return CVerifierDeviceIdentity::New(lIdentity, identity);
}

ECode CVerifierDeviceIdentityHelper::Parse(
    /* [in] */ const String& deviceIdentity,
    /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity)
{
    AutoPtr<ArrayOf<Byte> > input;
    // try {
        // TODO:
        // FAIL_RETURN(deviceIdentity.GetBytes("US-ASCII", &input));
    // } catch (UnsupportedEncodingException e) {
    //     throw new IllegalArgumentException("bad base-32 characters in input");
    // }

    Int64 output;
    FAIL_RETURN(CVerifierDeviceIdentity::DecodeBase32(input, &output));
    return CVerifierDeviceIdentity::New(output, identity);
}

}
}
}
}

