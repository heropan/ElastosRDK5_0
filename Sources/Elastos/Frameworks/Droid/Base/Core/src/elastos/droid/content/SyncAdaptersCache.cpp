
#include "elastos/droid/content/SyncAdaptersCache.h"
#include "elastos/droid/content/CSyncAdapterType.h"
#include "elastos/droid/content/CSyncAdapterTypeHelper.h"
#include "elastos/droid/R.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Content {

const String SyncAdaptersCache::TAG("Account");
const String SyncAdaptersCache::SERVICE_INTERFACE("android.content.SyncAdapter");
const String SyncAdaptersCache::SERVICE_META_DATA("android.content.SyncAdapter");
const String SyncAdaptersCache::ATTRIBUTES_NAME("sync-adapter");
const AutoPtr<SyncAdaptersCache::MySerializer> SyncAdaptersCache::sSerializer = new SyncAdaptersCache::MySerializer();

CAR_INTERFACE_IMPL(SyncAdaptersCache::MySerializer, Object, IXmlSerializerAndParser)

ECode SyncAdaptersCache::MySerializer::WriteAsXml(
    /* [in] */ IInterface* item,
    /* [in] */ IXmlSerializer* serializer)
{
    AutoPtr<ISyncAdapterType> adapterType = (ISyncAdapterType*) item;
    String authority;
    String accountType;
    FAIL_RETURN(adapterType->GetAuthority(&authority))
    FAIL_RETURN(adapterType->GetAccountType(&accountType))
    FAIL_RETURN(serializer->WriteAttribute(String(NULL), String("authority"), authority))
    FAIL_RETURN(serializer->WriteAttribute(String(NULL), String("accountType"), accountType))
    return NOERROR;
}

ECode SyncAdaptersCache::MySerializer::CreateFromXml(
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    String authority;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("authority"), &authority))
    String accountType;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("accountType"), &accountType))
    AutoPtr<ISyncAdapterTypeHelper> typeHelper;
    FAIL_RETURN(CSyncAdapterTypeHelper::AcquireSingleton((ISyncAdapterTypeHelper**)&typeHelper))
    AutoPtr<ISyncAdapterType> adapterType;
    FAIL_RETURN(typeHelper->NewKey(authority, accountType, (ISyncAdapterType**)&adapterType))
    *obj = adapterType;
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

SyncAdaptersCache::SyncAdaptersCache(
    /* [in] */ IContext *context)
    : RegisteredServicesCache(context, String(NULL), String(NULL), String(NULL), sSerializer.Get())
{}

SyncAdaptersCache::~SyncAdaptersCache()
{}

ECode SyncAdaptersCache::ParseServiceAttributes(
    /* [in] */ IResources* res,
    /* [in] */ const String& packageName,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IInterface** syncAdapterType)
{
    VALIDATE_NOT_NULL(syncAdapterType)
    *syncAdapterType = NULL;

    Int32 size = ArraySize(R::styleable::SyncAdapter);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::SyncAdapter, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa))

    String authority;
    ECode ec = NOERROR;
    String accountType;
    Boolean userVisible = FALSE;
    Boolean supportsUploading = FALSE;
    Boolean isAlwaysSyncable = FALSE;
    Boolean allowParallelSyncs = FALSE;
    String settingsActivity;
    AutoPtr<ISyncAdapterType> adapterType;
    ec = sa->GetString(R::styleable::SyncAdapter_contentAuthority, &authority);
    if (FAILED(ec)) goto EXIT;
    ec = sa->GetString(R::styleable::SyncAdapter_accountType, &accountType);
    if (FAILED(ec)) goto EXIT;

    if (authority.IsNull() || accountType.IsNull()) {
        *syncAdapterType = NULL;
        return NOERROR;
    }

    ec = sa->GetBoolean(R::styleable::SyncAdapter_userVisible, TRUE, &userVisible);
    if (FAILED(ec)) goto EXIT;
    ec = sa->GetBoolean(R::styleable::SyncAdapter_supportsUploading, TRUE, &supportsUploading);
    if (FAILED(ec)) goto EXIT;
    ec = sa->GetBoolean(R::styleable::SyncAdapter_isAlwaysSyncable, FALSE, &isAlwaysSyncable);
    if (FAILED(ec)) goto EXIT;
    ec = sa->GetBoolean(R::styleable::SyncAdapter_allowParallelSyncs, FALSE, &allowParallelSyncs);
    if (FAILED(ec)) goto EXIT;
    ec = sa->GetString(R::styleable::SyncAdapter_settingsActivity, &settingsActivity);
    if (FAILED(ec)) goto EXIT;
    ec = CSyncAdapterType::New((ISyncAdapterType**)&adapterType);
    ((CSyncAdapterType*)adapterType.Get())->constructor(authority, accountType, userVisible, supportsUploading,
            isAlwaysSyncable, allowParallelSyncs, settingsActivity);
    if (FAILED(ec)) goto EXIT;

    *syncAdapterType = adapterType.Get();
    REFCOUNT_ADD(*syncAdapterType);

EXIT:
    FAIL_RETURN(sa->Recycle())
    return ec;
}

}
}
}

