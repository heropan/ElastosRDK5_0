
#ifndef __PACKAGEVERIFICATIONSTATE_H__
#define __PACKAGEVERIFICATIONSTATE_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Server_server.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class InstallArgs;

class PackageVerificationState : public ElRefBase
{
public:
    /**
     * Create a new package verification state where {@code requiredVerifierUid}
     * is the user ID for the package that must reply affirmative before things
     * can continue.
     *
     * @param requiredVerifierUid user ID of required package verifier
     * @param args
     */
    PackageVerificationState(
        /* [in] */ Int32 requiredVerifierUid,
        /* [in] */ InstallArgs* args);

    CARAPI_(AutoPtr<InstallArgs>) GetInstallArgs();

    CARAPI_(void) AddSufficientVerifier(
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) SetVerifierResponse(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 code);

    CARAPI_(Boolean) IsVerificationComplete();

    CARAPI_(Boolean) IsInstallAllowed();

    CARAPI_(void) ExtendTimeout();

    CARAPI_(Boolean) TimeoutExtended();

private:
    AutoPtr<InstallArgs> mArgs;

    HashMap<Int32, Boolean> mSufficientVerifierUids;

    Int32 mRequiredVerifierUid;

    Boolean mSufficientVerificationComplete;

    Boolean mSufficientVerificationPassed;

    Boolean mRequiredVerificationComplete;

    Boolean mRequiredVerificationPassed;

    Boolean mExtendedTimeout;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__PACKAGEVERIFICATIONSTATE_H__
