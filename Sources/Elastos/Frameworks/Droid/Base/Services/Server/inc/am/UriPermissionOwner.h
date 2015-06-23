
#ifndef __URIPERMISSIONOWNER_H__
#define __URIPERMISSIONOWNER_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashSet.h>
#include "am/UriPermission.h"

using Elastos::Utility::Etl::HashSet;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CActivityManagerService;

class UriPermissionOwner : public ElRefBase
{
public:
    UriPermissionOwner(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ Handle32 owner);

    ~UriPermissionOwner();

    CARAPI_(AutoPtr<IBinder>) GetExternalTokenLocked();

    static CARAPI_(AutoPtr<UriPermissionOwner>) FromExternalToken(
        /* [in] */ IBinder* token);

    CARAPI RemoveUriPermissionsLocked();

    CARAPI RemoveUriPermissionsLocked(
        /* [in] */ Int32 mode);

    CARAPI RemoveUriPermissionLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 mode);

    CARAPI AddReadPermission(
        /* [in] */ UriPermission* perm);

    CARAPI AddWritePermission(
        /* [in] */ UriPermission* perm);

    CARAPI RemoveReadPermission(
        /* [in] */ UriPermission* perm);

    CARAPI RemoveWritePermission(
        /* [in] */ UriPermission* perm);

    CARAPI_(String) ToString();

public:
    CActivityManagerService* mService;  // weak-ref
    Handle32 mOwner;

    AutoPtr<IWeakReference> mExternalToken;
    AutoPtr< HashSet< AutoPtr<UriPermission> > > mReadUriPermissions; // special access to reading uris.
    AutoPtr< HashSet< AutoPtr<UriPermission> > > mWriteUriPermissions; // special access to writing uris.
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos



#endif //__URIPERMISSIONOWNER_H__
