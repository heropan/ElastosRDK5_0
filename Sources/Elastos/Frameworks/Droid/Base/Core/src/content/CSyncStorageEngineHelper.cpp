
#include "content/CIntent.h"
#include "content/CSyncStorageEngine.h"
#include "content/CSyncStorageEngineHelper.h"
//***#include "os/Environment.h"
#include <ext/frameworkext.h>

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CSyncStorageEngineHelper::GetEVENTS(
    /* [out, callee] */ ArrayOf<String>** events)
{
    VALIDATE_NOT_NULL(events)
    AutoPtr<ArrayOf<String> > tmpArray = ArrayOf<String>::Alloc(2);
    (*tmpArray)[0] = String("START");
    (*tmpArray)[1] = String("STOP");
    *events = tmpArray;
    INTERFACE_ADDREF(*events);
    return NOERROR;
}

ECode CSyncStorageEngineHelper::GetSYNC_CONNECTION_SETTING_CHANGED_INTENT(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    return CIntent::New(String("com.android.sync.SYNC_CONN_STATUS_CHANGED"), intent);
}

ECode CSyncStorageEngineHelper::GetSOURCES(
    /* [out, callee] */ ArrayOf<String>** sources)
{
    VALIDATE_NOT_NULL(sources)
    AutoPtr<ArrayOf<String> > tmpArray = ArrayOf<String>::Alloc(5);
    (*tmpArray)[0] = String("SERVER");
    (*tmpArray)[1] = String("LOCAL");
    (*tmpArray)[2] = String("POLL");
    (*tmpArray)[3] = String("USER");
    (*tmpArray)[4] = String("PERIODIC");
    *sources = tmpArray;
    INTERFACE_ADDREF(*sources);
    return NOERROR;
}

ECode CSyncStorageEngineHelper::NewTestInstance(
    /* [in] */ IContext* context,
    /* [out] */ ISyncStorageEngine** newEngine)
{
    VALIDATE_NOT_NULL(context)
    VALIDATE_NOT_NULL(newEngine)
    AutoPtr<IFile> file;
    FAIL_RETURN(context->GetFilesDir((IFile**)&file))
    return CSyncStorageEngine::New(context, file, newEngine);
}

ECode CSyncStorageEngineHelper::Init(
    /* [in] */ IContext* context)
{
    if (NULL != CSyncStorageEngine::sSyncStorageEngine) {
        return NOERROR;
    }
    // This call will return the correct directory whether Encrypted File Systems is
    // enabled or not.
    AutoPtr<IFile> dataDir;
//***    dataDir = Environment::GetSystemSecureDirectory();
    return CSyncStorageEngine::New(context, dataDir, (ISyncStorageEngine**)&CSyncStorageEngine::sSyncStorageEngine);
}

ECode CSyncStorageEngineHelper::GetSingleton(
    /* [out] */ ISyncStorageEngine** syncStoregeEngine)
{
    VALIDATE_NOT_NULL(syncStoregeEngine)

    if (NULL == CSyncStorageEngine::sSyncStorageEngine) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *syncStoregeEngine = (ISyncStorageEngine*) CSyncStorageEngine::sSyncStorageEngine;
    INTERFACE_ADDREF(*syncStoregeEngine);
    return NOERROR;
}

ECode CSyncStorageEngineHelper::Equals(
    /* [in] */ IBundle* bundleOne,
    /* [in] */ IBundle* bundleTwo,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(bundleOne)
    VALIDATE_NOT_NULL(bundleTwo)
    VALIDATE_NOT_NULL(isEqual)
    Int32 size1 = 0;
    Int32 size2 = 0;
    Boolean isEmpty = FALSE;
    FAIL_RETURN(bundleOne->GetSize(&size1))
    FAIL_RETURN(bundleTwo->GetSize(&size2))

    if (size1 != size2) {
        *isEqual = FALSE;
        return NOERROR;
    }

    FAIL_RETURN(bundleOne->IsEmpty(&isEmpty))
    if (isEmpty) {
        *isEqual = TRUE;
        return NOERROR;
    }

    AutoPtr<IObjectContainer> stringSet;
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    FAIL_RETURN(bundleOne->KeySet((IObjectContainer**)&stringSet))

    if (NULL != stringSet) {
        AutoPtr<ICharSequence> stringKey;
        AutoPtr<IInterface> compare1;
        AutoPtr<IInterface> conpare2;
        String key;
        Boolean hasNext = FALSE;
        Boolean isContain = FALSE;
        FAIL_RETURN(stringSet->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator))

        while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
            stringKey = NULL;
            FAIL_RETURN(ObjEnumerator->Current((IInterface**)&stringKey))
            FAIL_RETURN(stringKey->ToString(&key))
            FAIL_RETURN(bundleTwo->ContainsKey(key, &isContain))
            if(!isContain){
                *isEqual = FALSE;
                return NOERROR;
            }

            AutoPtr<IInterface> compare1;
            AutoPtr<IInterface> compare2;
            FAIL_RETURN(bundleOne->Get(key, (IInterface**)&compare1))
            FAIL_RETURN(bundleTwo->Get(key, (IInterface**)&conpare2))

            if(!_CObject_Compare(compare1, conpare2)){
                *isEqual = FALSE;
                return NOERROR;
            }
        }

    }

    *isEqual = TRUE;
    return NOERROR;
}

}
}
}

