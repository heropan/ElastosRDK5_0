
#include "am/UriPermission.h"
#include "am/UriPermissionOwner.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

UriPermission::UriPermission(
    /* [in] */ Int32 uid,
    /* [in] */ IUri* uri) :
    mUid(uid),
    mUri(uri),
    mModeFlags(0),
    mGlobalModeFlags(0)
{
}

UriPermission::~UriPermission()
{
    mReadOwners.Clear();
    mWriteOwners.Clear();
}

ECode UriPermission::ClearModes(
    /* [in] */ Int32 modeFlagsToClear)
{
    if ((modeFlagsToClear & IIntent::FLAG_GRANT_READ_URI_PERMISSION) != 0) {
        mGlobalModeFlags &= ~IIntent::FLAG_GRANT_READ_URI_PERMISSION;
        mModeFlags &= ~IIntent::FLAG_GRANT_READ_URI_PERMISSION;
        if (mReadOwners.IsEmpty() == FALSE) {
            HashSet<UriPermissionOwner*>::Iterator it = mReadOwners.Begin();
            for (; it != mReadOwners.End(); it++) {
                (*it)->RemoveReadPermission(this);
            }
            mReadOwners.Clear();
        }
    }
    if ((modeFlagsToClear & IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
        mGlobalModeFlags &= ~IIntent::FLAG_GRANT_WRITE_URI_PERMISSION;
        mModeFlags &= ~IIntent::FLAG_GRANT_WRITE_URI_PERMISSION;
        if (mWriteOwners.IsEmpty() == FALSE) {
            HashSet<UriPermissionOwner*>::Iterator it = mWriteOwners.Begin();
            for (; it != mWriteOwners.End(); it++) {
                (*it)->RemoveWritePermission(this);
            }
            mWriteOwners.Clear();
        }
    }

    return NOERROR;
}

String UriPermission::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }

    StringBuilder sb;
    sb += "UriPermission{";
    sb += (Int32)this;//(Integer.toHexString(System.identityHashCode(this)));
    sb += ' ';
    sb += mUri;
    sb += '}';
    mStringName = sb.ToString();
    return mStringName;
}

ECode UriPermission::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    StringBuilder sb(prefix);
    sb += ("modeFlags=0x");
    sb += StringUtils::Int32ToHexString(mModeFlags);
    sb += (" uid=");
    sb += (StringUtils::Int32ToString(mUid));
    sb += (" mGlobalModeFlags=0x");
    sb += StringUtils::Int32ToHexString(mGlobalModeFlags);
    sb += ("\n");
    if (mReadOwners.IsEmpty() == FALSE) {
        sb += (prefix);
        sb += ("\n");
        HashSet<UriPermissionOwner*>::Iterator it = mReadOwners.Begin();
        for (; it != mReadOwners.End(); it++) {
            sb += (prefix);
            sb += ("  * ");
            sb += (*it)->ToString();
            sb += ("\n");
        }
    }
    if (mWriteOwners.IsEmpty() == FALSE) {
        sb += (prefix);
        sb += ("\n");
        HashSet<UriPermissionOwner*>::Iterator it = mWriteOwners.Begin();
        for (; it != mWriteOwners.End(); it++) {
            sb += (prefix);
            sb += ("  * ");
            sb += (*it)->ToString();
            sb += ("\n");
        }
    }

    pw->PrintString(sb.ToString());
    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
