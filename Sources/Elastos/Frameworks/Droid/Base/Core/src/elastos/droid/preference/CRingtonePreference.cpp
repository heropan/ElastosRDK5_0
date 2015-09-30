
#include "CRingtonePreference.h"
#include "content/CIntent.h"
#include "media/CRingtoneManagerHelper.h"
#include "net/Uri.h"
#include "text/TextUtils.h"
#include "R.h"

using Elastos::Core::CString;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Media::IRingtoneManager;
using Elastos::Droid::Media::IRingtoneManagerHelper;
using Elastos::Droid::Media::CRingtoneManagerHelper;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Preference {

const String CRingtonePreference::TAG("RingtonePreference");

CRingtonePreference::CRingtonePreference()
    : mRingtoneType(0)
    , mShowDefault(FALSE)
    , mShowSilent(FALSE)
    , mRequestCode(0)
{}

PInterface CRingtonePreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IRingtonePreference) {
        return (IRingtonePreference*)this;
    }
    else if (riid == EIID_IPreferenceManagerOnActivityResultListener) {
        return (IPreferenceManagerOnActivityResultListener*)this;
    }

    return Preference::Probe(riid);
}

UInt32 CRingtonePreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CRingtonePreference::Release()
{
    return ElRefBase::Release();
}

ECode CRingtonePreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IRingtonePreference*)this) {
        *pIID = EIID_IRingtonePreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IPreferenceManagerOnActivityResultListener*)this) {
        *pIID = EIID_IPreferenceManagerOnActivityResultListener;
        return NOERROR;
    }
    return Preference::GetInterfaceID(pObject, pIID);
}

ECode CRingtonePreference::GetRingtoneType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mRingtoneType;
    return NOERROR;
}

ECode CRingtonePreference::SetRingtoneType(
    /* [in] */ Int32 type)
{
    mRingtoneType = type;
    return NOERROR;
}

ECode CRingtonePreference::GetShowDefault(
    /* [out] */ Boolean* showDefault)
{
    VALIDATE_NOT_NULL(showDefault)
    *showDefault = mShowDefault;
    return NOERROR;
}

ECode CRingtonePreference::SetShowDefault(
    /* [in] */ Boolean showDefault)
{
    mShowDefault = showDefault;
    return NOERROR;
}

ECode CRingtonePreference::GetShowSilent(
    /* [out] */ Boolean* showSilent)
{
    VALIDATE_NOT_NULL(showSilent)
    *showSilent = mShowSilent;
    return NOERROR;
}

ECode CRingtonePreference::SetShowSilent(
    /* [in] */ Boolean showSilent)
{
    mShowSilent = showSilent;
    return NOERROR;
}

ECode CRingtonePreference::OnClick()
{
    AutoPtr<IIntent> intent;
    CIntent::New(IRingtoneManager::ACTION_RINGTONE_PICKER, (IIntent**)&intent);
    OnPrepareRingtonePickerIntent(intent);
    AutoPtr<IPreferenceManager> preferencemanager;
    GetPreferenceManager((IPreferenceManager**)&preferencemanager);
    AutoPtr<IPreferenceFragment> owningFragment;
    preferencemanager->GetFragment((IPreferenceFragment**)&owningFragment);
    if (owningFragment != NULL) {
        owningFragment->StartActivityForResult(intent, mRequestCode);
    }
    else {
        AutoPtr<IActivity> activity;
        preferencemanager->GetActivity((IActivity**)&activity);
        activity->StartActivityForResult(intent, mRequestCode);
    }

    return NOERROR;
}

ECode CRingtonePreference::OnPrepareRingtonePickerIntent(
    /* [in] */ IIntent* ringtonePickerIntent)
{
    AutoPtr<IUri> uri;
    OnRestoreRingtone((IUri**)&uri);
    ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_EXISTING_URI,
            IParcelable::Probe(uri));
    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_DEFAULT, mShowDefault);
    if (mShowDefault) {
        Int32 type;
        GetRingtoneType(&type);
        AutoPtr<IUri> defaultUri;
        AutoPtr<IRingtoneManagerHelper> helper;
        CRingtoneManagerHelper::AcquireSingleton((IRingtoneManagerHelper**)&helper);
        helper->GetDefaultUri(type, (IUri**)&defaultUri);
        ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_DEFAULT_URI,
                IParcelable::Probe(defaultUri));
    }

    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_SILENT, mShowSilent);
    ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_TYPE, mRingtoneType);
    AutoPtr<ICharSequence> cs;
    GetTitle((ICharSequence**)&cs);
    ringtonePickerIntent->PutCharSequenceExtra(IRingtoneManager::EXTRA_RINGTONE_TITLE, cs);
    return NOERROR;
}

ECode CRingtonePreference::OnSaveRingtone(
    /* [in] */ IUri* ringtoneUri)
{
    String uri("");
    if (ringtoneUri != NULL) {
        ringtoneUri->ToString((String*)&uri);
    }
    Boolean result;
    return PersistString(uri, &result);
}

ECode CRingtonePreference::OnRestoreRingtone(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    String uriString;
    GetPersistedString(String(NULL), &uriString);
    if(!TextUtils::IsEmpty(uriString)){
        AutoPtr<IUri> uritmp;
        Uri::Parse(uriString, (IUri**)&uritmp);
        *uri = uritmp;
        REFCOUNT_ADD(*uri)
    }
    else {
        *uri = NULL;
    }

    return NOERROR;
}

ECode CRingtonePreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    String out;
    a->GetString(index, (String*)&out);
    AutoPtr<ICharSequence> charsequ;
    CString::New(out, (ICharSequence**)&charsequ);
    *value = charsequ;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CRingtonePreference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultObj)
{
    String defaultValue;
    ICharSequence::Probe(defaultObj)->ToString(&defaultValue);

    /*
     * This method is normally to make sure the internal state and UI
     * matches either the persisted value or the default value. Since we
     * don't show the current value in the UI (until the dialog is opened)
     * and we don't keep local state, if we are restoring the persisted
     * value we don't need to do anything.
     */
    if (restorePersistedValue) {
        return NOERROR;
    }

    // If we are setting to the default value, we should persist it.
    if (!TextUtils::IsEmpty(defaultValue)) {
        AutoPtr<IUri> uri;
        Uri::Parse(defaultValue, (IUri**)&uri);
        OnSaveRingtone(uri);
    }
    return NOERROR;
}

ECode CRingtonePreference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    FAIL_RETURN(Preference::OnAttachedToHierarchy(preferenceManager))

    preferenceManager->RegisterOnActivityResultListener(this);
    preferenceManager->GetNextRequestCode(&mRequestCode);
    return NOERROR;
}

ECode CRingtonePreference::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (requestCode == mRequestCode) {
        if (data != NULL) {
            AutoPtr<IParcelable> p;
            data->GetParcelableExtra(IRingtoneManager::EXTRA_RINGTONE_PICKED_URI, (IParcelable**)&p);
            AutoPtr<IUri> uri = IUri::Probe(p);
            String str("");
            if (uri != NULL) {
                uri->ToString(&str);
            }
            AutoPtr<ICharSequence> cs;
            CString::New(str, (ICharSequence**)&cs);
            Boolean value;
            if (CallChangeListener(cs, &value), value) {
                OnSaveRingtone(uri);
            }
        }
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CRingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Preference::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > arrayAttrs = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RingtonePreference),
            ARRAY_SIZE(R::styleable::RingtonePreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, arrayAttrs, defStyle, 0, (ITypedArray**)&a);
    a->GetInt32(R::styleable::RingtonePreference_ringtoneType, IRingtoneManager::TYPE_RINGTONE, &mRingtoneType);
    a->GetBoolean(R::styleable::RingtonePreference_showDefault, TRUE, &mShowDefault);
    a->GetBoolean(R::styleable::RingtonePreference_showSilent, TRUE, &mShowSilent);
    a->Recycle();
    return NOERROR;
}

ECode CRingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::ringtonePreferenceStyle);
}

ECode CRingtonePreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

}
}
}

