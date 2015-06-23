
#include "content/pm/PackageUserState.h"
#include <Elastos.CoreLibrary.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

PackageUserState::PackageUserState()
    : mStopped(FALSE)
    , mNotLaunched(FALSE)
    , mInstalled(TRUE)
    , mEnabled(IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT)
{}

PackageUserState::PackageUserState(
    /* [in] */ PackageUserState* orig)
{
    assert(orig != NULL);
    mStopped = orig->mStopped;
    mNotLaunched = orig->mNotLaunched;
    mInstalled = orig->mInstalled;
    mEnabled = orig->mEnabled;

    if (orig->mDisabledComponents != NULL) {
        mDisabledComponents = new HashSet<String>(orig->mDisabledComponents->Begin(),
            orig->mDisabledComponents->End());
    }

    if (orig->mEnabledComponents != NULL) {
        mEnabledComponents = new HashSet<String>(orig->mEnabledComponents->Begin(),
            orig->mEnabledComponents->End());
    }
}

PackageUserState::~PackageUserState()
{
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
