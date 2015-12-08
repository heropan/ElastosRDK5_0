
#ifndef __ELASTOS_DROID_SERVER_AM_CPENDINGINTENTRECORDKEY_H__
#define __ELASTOS_DROID_SERVER_AM_CPENDINGINTENTRECORDKEY_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ActivityRecord;

class CPendingIntentRecordKey : public ElRefBase
{
public:
    CPendingIntentRecordKey(
        /* [in] */ Int32 t,
        /* [in] */ const String& p,
        /* [in] */ ActivityRecord * a,
        /* [in] */ const String& w,
        /* [in] */ Int32 r,
        /* [in] */ ArrayOf<IIntent*>* i,
        /* [in] */ ArrayOf<String>* it,
        /* [in] */ Int32 f,
        /* [in] */ IBundle* o,
        /* [in] */ Int32 userId);

    ~CPendingIntentRecordKey();

    CARAPI Equals(
        /* [in] */ const CPendingIntentRecordKey* otherObj,
        /* [out] */ Boolean* equal) const;

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI_(String) ToString();

    CARAPI_(String) TypeName();

    inline Boolean operator == (const CPendingIntentRecordKey* other) const
    {
        if (other == NULL)
            return FALSE;

        Boolean equal;
        this->Equals(other, &equal);
        return equal;
    }

public:
    static const Int32 ODD_PRIME_NUMBER = 37;

    Int32 mType;
    String mPackageName;
    ActivityRecord* mActivity; // weak-ref
    String mWho;
    Int32 mRequestCode;
    AutoPtr<IIntent> mRequestIntent;
    String mRequestResolvedType;
    AutoPtr<IBundle> mOptions;
    AutoPtr< ArrayOf<IIntent*> > mAllIntents;
    AutoPtr< ArrayOf<String> > mAllResolvedTypes;
    Int32 mFlags;
    Int32 mHashCode;
    Int32 mUserId;
};


} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_CPENDINGINTENTRECORDKEY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Server::Am::CPendingIntentRecordKey)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Server::Am::CPendingIntentRecordKey)
#endif

#endif //__ELASTOS_DROID_SERVER_AM_CPENDINGINTENTRECORDKEY_H__
