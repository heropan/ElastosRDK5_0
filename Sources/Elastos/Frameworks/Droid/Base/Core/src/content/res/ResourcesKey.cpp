#include "content/res/ResourcesKey.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/Math.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CAR_INTERFACE_IMPL(ResourcesKey, Object, IResourcesKey)

ResourcesKey::ResourcesKey(
    /* [in] */ const String& resDir,
    /* [in] */ Int32 displayId,
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [in] */ Float scale,
    /* [in] */ IBinder* token)
{
    CConfiguration::New((IConfiguration**)&mOverrideConfiguration)
    mResDir = resDir;
    mDisplayId = displayId;
    if (overrideConfiguration != NULL) {
        mOverrideConfiguration->SetTo(overrideConfiguration);
    }
    mScale = scale;
    mToken = token;

    Int32 hash = 17;
    hash = 31 * hash + (!mResDir.IsNull() ? 0 : mResDir.GetHashCode());
    hash = 31 * hash + mDisplayId;
    hash = 31 * hash + (mOverrideConfiguration != NULL
        ? Object::GetHashCode(mOverrideConfiguration) : 0);
    hash = 31 * hash + Elastos::Core::Math::FloatToIntBits(mScale);
    mHash = hash;
}

Boolean ResourcesKey::HasOverrideConfiguration()
{
    return !Object::Equals(CConfiguration::EMPTY, mOverrideConfiguration);
}

ECode ResourcesKey::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = mHash;
    return NOERROR;
}

ECode ResourcesKey::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!(obj instanceof ResourcesKey)) {
        return false;
    }
    ResourcesKey peer = (ResourcesKey) obj;
    if (mResDir != peer.mResDir) {
        if (mResDir == null || peer.mResDir == null) {
            return false;
        } else if (!mResDir.equals(peer.mResDir)) {
            return false;
        }
    }
    if (mDisplayId != peer.mDisplayId) {
        return false;
    }
    if (mOverrideConfiguration != peer.mOverrideConfiguration) {
        if (mOverrideConfiguration == null || peer.mOverrideConfiguration == null) {
            return false;
        }
        if (!mOverrideConfiguration.equals(peer.mOverrideConfiguration)) {
            return false;
        }
    }
    if (mScale != peer.mScale) {
        return false;
    }
    return true;
    return NOERROR;
}

ECode ResourcesKey::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = StringUtils::ToString(mHash);
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos