#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/CRestrictionsManager.h"

#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::XmlPull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Content {

const String CRestrictionsManager::TAG("RestrictionsManager");

const String CRestrictionsManager::TAG_RESTRICTION("restriction");

CAR_INTERFACE_IMPL(CRestrictionsManager, Object, IRestrictionsManager)

CAR_OBJECT_IMPL(CRestrictionsManager)

CRestrictionsManager::CRestrictionsManager()
{}

CRestrictionsManager::~CRestrictionsManager()
{}

ECode CRestrictionsManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIRestrictionsManager* service)
{
    mContext = context;
    mService = service;
    return NOERROR;
}

ECode CRestrictionsManager::GetApplicationRestrictions(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    *bundle = NULL;

    // try {
    String packageName;
    mContext->GetPackageName(&packageName);
    if (mService != NULL) {
        return mService->GetApplicationRestrictions(packageName, bundle);
    }
    // } catch (RemoteException re) {
    //     Logger::W(TAG, "Couldn't reach service");
    // }
    // return NULL;
    return NOERROR;
}

ECode CRestrictionsManager::HasRestrictionsProvider(
    /* [out] */ Boolean** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = FALSE;
    // try {
    if (mService != NULL) {
        return mService->HasRestrictionsProvider();
    }
    // } catch (RemoteException re) {
    //     Logger::W(TAG, "Couldn't reach service");
    // }
    // return false;
    return NOERROR;
}

ECode CRestrictionsManager::RequestPermission(
    /* [in] */ const String& requestType,
    /* [in] */ const String& requestId,
    /* [in] */ IPersistableBundle* request)
{
    if (requestType.IsNull()) {
        throw new NullPointerException("requestType cannot be NULL");
    }
    if (requestId.IsNull()) {
        throw new NullPointerException("requestId cannot be NULL");
    }
    if (request == NULL) {
        throw new NullPointerException("request cannot be NULL");
    }
    // try {
    if (mService != NULL) {
        String packageName;
        mContext->GetPackageName(&packageName);
        return mService->RequestPermission(packageName, requestType, requestId, request);
    }
    // } catch (RemoteException re) {
    //     Logger::W(TAG, "Couldn't reach service");
    // }
    return NOERROR;
}

ECode CRestrictionsManager::CreateLocalApprovalIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = NULL;
    // try {
    if (mService != NULL) {
        return mService->CreateLocalApprovalIntent(intent);
    }
    // } catch (RemoteException re) {
    //     Logger::W(TAG, "Couldn't reach service");
    // }
    // return NULL;
    return NOERROR;
}

ECode CRestrictionsManager::NotifyPermissionResponse(
    /* [in] */ const String& packageName,
    /* [in] */ IPersistableBundle* response)
{
    if (packageName.IsNull()) {
        // throw new NullPointerException("packageName cannot be NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (response == NULL) {
        // throw new NullPointerException("request cannot be NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (!response.containsKey(REQUEST_KEY_ID)) {
        // throw new IllegalArgumentException("REQUEST_KEY_ID must be specified");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!response.containsKey(RESPONSE_KEY_RESULT)) {
        // throw new IllegalArgumentException("RESPONSE_KEY_RESULT must be specified");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
        if (mService != NULL) {
            return mService->NotifyPermissionResponse(packageName, response);
        }
    // } catch (RemoteException re) {
    //     Logger::W(TAG, "Couldn't reach service");
    // }
    return NOERROR;
}

ECode CRestrictionsManager::GetManifestRestrictions(
    /* [in] */ const String& packageName,
    /* [out] */ IList** restrictions)
{
    VALIDATE_NOT_NULL(restrictions)
    *restrictions = NULL;

    AutoPtr<IApplicationInfo> appInfo;
    // try {
    AutoPtr<IPackageManager> pgkMgr;
    mContext->GetPackageManager((IPackageManager**)&pgkMgr);

    ECode ec = pgkMgr->GetApplicationInfo(packageName, IPackageManager::GET_META_DATA, (IApplicationInfo**)&appInfo);
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // } catch (NameNotFoundException pnfe) {
    //     throw new IllegalArgumentException("No such package " + packageName);
    // }
    if (appInfo == NULL) {
        return NOERROR;
    }

    CApplicationInfo* cai = (CApplicationInfo*)appInfo.Get();
    Boolean contains;
    cai->mMetaData->ContainsKey(IRestrictionsManager::META_DATA_APP_RESTRICTIONS, &contains);
    if (!contains) {
        return NOERROR;
    }

    AutoPtr<IXmlResourceParser> xml;
    appInfo->LoadXmlMetaData(pgkMgr, IRestrictionsManager::META_DATA_APP_RESTRICTIONS, (IXmlResourceParser**)&xml);
    AutoPtr<IList> r;
    FAIL_RETURN(LoadManifestRestrictions(packageName, xml, (IList**)&r))

    *restrictions = r;
    REFCOUNT_ADD(*restrictions)
    return NOERROR;
}

ECode CRestrictionsManager::LoadManifestRestrictions(
    /* [in] */ const String& packageName,
    /* [in] */ IXmlResourceParser* xml,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IContext> appContext;
    // try {
    FAIL_RETURN(mContext->CreatePackageContext(packageName, 0 /* flags */, (IContext**)&appContext))
    // } catch (NameNotFoundException nnfe) {
    //     return NULL;
    // }

    AutoPtr<IArrayList> restrictions;
    CArrayList::New((IArrayList**)&restrictions);
    AutoPtr<IRestrictionEntry> restriction;

    IXmlPullParser* parser = IXmlPullParser::Probe(xml);
    // try {
    String name;
    Int32 tagType;
    FAIL_RETURN(parser->Next(&tagType))
    while (tagType != IXmlPullParser::END_DOCUMENT) {
        if (tagType == IXmlPullParser::START_TAG) {
            parser->GetName(&name);
            if (name.Equals(TAG_RESTRICTION)) {
                AutoPtr<IAttributeSet> attrSet = Xml::AsAttributeSet(xml);
                if (attrSet != NULL) {
                    Int32 size = ARRAY_SIZE(R::styleable::RestrictionEntry);
                    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
                    layout->Copy(R::styleable::RestrictionEntry, size);

                    AutoPtr<ITypedArray> a;
                    FAIL_RETURN(appContext->ObtainStyledAttributes(attrSet, layout, (ITypedArray**)&a))
                    restriction = NULL;
                    FAIL_RETURN(LoadRestriction(appContext, a, (IRestrictionEntry**)&restriction))
                    if (restriction != NULL) {
                        restrictions->Add(restriction);
                    }
                }
            }
        }
        FAIL_RETURN(restriction->Next(&tagType))
    }
    // } catch (XmlPullParserException e) {
    //     Logger::W(TAG, "Reading restriction metadata for " + packageName, e);
    //     return NULL;
    // } catch (IOException e) {
    //     Logger::W(TAG, "Reading restriction metadata for " + packageName, e);
    //     return NULL;
    // }

    *result = restrictions;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CRestrictionsManager::LoadRestriction(
    /* [in] */ IContext* appContext,
    /* [in] */ ITypedArray* a,
    /* [out] */ IRestrictionEntry** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    String key;
    a->GetString(R::styleable::RestrictionEntry_key, &key);
    Int32 restrictionType;
    a->GetInt32(R::styleable::RestrictionEntry_restrictionType, -1, &restrictionType);
    String title;
    a->GetString(R::styleable::RestrictionEntry_title, &title);
    String description;
    a->GetString(R::styleable::RestrictionEntry_description, &description);
    Int32 entries;
    a->GetResourceId(R::styleable::RestrictionEntry_entries, 0, &entries);
    Int32 entryValues;
    a->GetResourceId(R::styleable::RestrictionEntry_entryValues, 0, &entryValues);

    if (restrictionType == -1) {
        Logger::W(TAG, "restrictionType cannot be omitted");
        return NOERROR;
    }

    if (key == NULL) {
        Logger::W(TAG, "key cannot be omitted");
        return NOERROR;
    }

    AutoPtr<IRestrictionEntry> restriction;
    CRestrictionEntry::New(restrictionType, key, (IRestrictionEntry**)&restriction);
    restriction->SetTitle(title);
    restriction->SetDescription(description);
    if (entries != 0) {
        restriction->SetChoiceEntries(appContext, entries);
    }
    if (entryValues != 0) {
        restriction->SetChoiceValues(appContext, entryValues);
    }

    String strval;
    Int32 ival;
    Boolean bval;

    // Extract the default value based on the type
    switch (restrictionType) {
        case IRestrictionEntry::TYPE_NULL: // hidden
        case IRestrictionEntry::TYPE_STRING:
        case IRestrictionEntry::TYPE_CHOICE:
            a->GetString(R::styleable::RestrictionEntry_defaultValue, &strval);
            restriction->SetSelectedString(strval);
            break;
        case IRestrictionEntry::TYPE_INTEGER:
            a->GetInt32(R::styleable::RestrictionEntry_defaultValue, 0, &ival);
            restriction->GetInt32Value(ival);
            break;
        case IRestrictionEntry::TYPE_MULTI_SELECT:
            Int32 resId;
            a->GetResourceId(R::styleable::RestrictionEntry_defaultValue, 0, &resId);
            if (resId != 0) {
                AutoPtr<IResources> res;
                appContext->GetResources((IResources**)&res);
                AutoPtr<ArrayOf<String> > strArray;
                res->GetStringArray((ArrayOf<String>**)&strArray);
                restriction->SetAllSelectedStrings(strArray);
            }
            break;
        case IRestrictionEntry::TYPE_BOOLEAN:
            a->GetBoolean(R::styleable::RestrictionEntry_defaultValue, FALSE, &bval);
            restriction->SetSelectedState(bval);
            break;
        default:
            Logger::W(TAG, "Unknown restriction type %d", restrictionType);
            break;
    }

    *result = restriction;
    REFCOUNT_ADD(*result)
    return NOERROR;
}


}
}
}