
#ifndef __ELASTOS_DROID_SERVER_AM_URIPERMISSION_H__
#define __ELASTOS_DROID_SERVER_AM_URIPERMISSION_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Net::IUri;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {
class UriPermissionOwner;
}}}}

#define HASH_FUNC_FOR_PTR_URIPERMISSIONOWNER
DEFINE_HASH_FUNC_FOR_PTR_USING_ADDR(Elastos::Droid::Server::Am::UriPermissionOwner)
#endif


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class UriPermission : public ElRefBase
{
public:
    UriPermission(
        /* [in] */ Int32 uid,
        /* [in] */ IUri* uri);

    virtual ~UriPermission();

    CARAPI ClearModes(
        /* [in] */ Int32 modeFlagsToClear);

    CARAPI_(String) ToString();

    CARAPI Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix);

public:
    Int32 mUid;
    AutoPtr<IUri> mUri;
    Int32 mModeFlags;
    Int32 mGlobalModeFlags;
    HashSet<UriPermissionOwner*> mReadOwners; // weak ref
    HashSet<UriPermissionOwner*> mWriteOwners; // weak ref

    String mStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#define HASH_FUNC_FOR_AUTOPTR_URIPERMISSION
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::UriPermission)
#endif

#endif //__ELASTOS_DROID_SERVER_AM_URIPERMISSION_H__
