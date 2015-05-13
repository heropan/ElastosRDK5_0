
#ifndef __CPENDINGINTENTRECORD_H__
#define __CPENDINGINTENTRECORD_H__

#include "_CPendingIntentRecord.h"
#include <Elastos.Droid.Core.h>
#include "am/CPendingIntentRecordKey.h"

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntentReceiver;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern "C" const InterfaceID EIID_PendingIntentRecord;

class CActivityManagerService;

CarClass(CPendingIntentRecord)
{
public:
    CPendingIntentRecord();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI Init(
        /* [in] */ Handle32 owner,
        /* [in] */ Handle32 k,
        /* [in] */ Int32 u);

    CARAPI constructor(
        /* [in] */ Handle32 owner,
        /* [in] */ Handle32 k,
        /* [in] */ Int32 u);

    CARAPI Send(
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* finishedReceiver,
        /* [in] */ const String& requiredPermission,
        /* [out] */ Int32* result);

    CARAPI CompleteFinalize();

    // void dump(PrintWriter pw, String prefix);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    CARAPI_(Int32) SendInner(
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* finishedReceiver,
        /* [in] */ const String& requiredPermission,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ IBundle* options);

public:
    CActivityManagerService* mOwner;
    AutoPtr<CPendingIntentRecordKey> mKey;
    Int32 mUid;
    AutoPtr<IWeakReference> mRef;
    Boolean mSent;
    Boolean mCanceled;

    String mStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#ifndef HASH_FUNC_FOR_AUTOPTR_USING_ADDR_CPENDINGINTENTRECORD
#define HASH_FUNC_FOR_AUTOPTR_USING_ADDR_CPENDINGINTENTRECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::CPendingIntentRecord)
#endif

#endif //__CPENDINGINTENTRECORD_H__
