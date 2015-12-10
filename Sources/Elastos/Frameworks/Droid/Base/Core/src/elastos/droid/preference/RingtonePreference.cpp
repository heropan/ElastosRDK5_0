#include "elastos/droid/content/CIntent.h"
// #include "elastos/droid/media/CRingtoneManagerHelper.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/preference/RingtonePreference.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IFragment;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Media::IRingtoneManager;
using Elastos::Droid::Media::IRingtoneManagerHelper;
// using Elastos::Droid::Media::CRingtoneManagerHelper;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Preference {

const String RingtonePreference::TAG("RingtonePreference");

CAR_INTERFACE_IMPL_2(RingtonePreference, Preference, IRingtonePreference, IPreferenceManagerOnActivityResultListener)

RingtonePreference::RingtonePreference()
    : mRingtoneType(0)
    , mShowDefault(FALSE)
    , mShowSilent(FALSE)
    , mRequestCode(0)
{}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    FAIL_RETURN(Preference::constructor(context, attrs, defStyleAttr, defStyleRes));

    AutoPtr<ArrayOf<Int32> > arrayAttrs = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RingtonePreference),
            ARRAY_SIZE(R::styleable::RingtonePreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, arrayAttrs, defStyleAttr, defStyleRes, (ITypedArray**)&a);
    a->GetInt32(R::styleable::RingtonePreference_ringtoneType, IRingtoneManager::TYPE_RINGTONE, &mRingtoneType);
    a->GetBoolean(R::styleable::RingtonePreference_showDefault, TRUE, &mShowDefault);
    a->GetBoolean(R::styleable::RingtonePreference_showSilent, TRUE, &mShowSilent);
    a->Recycle();
    return NOERROR;
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::ringtonePreferenceStyle);
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode RingtonePreference::GetRingtoneType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mRingtoneType;
    return NOERROR;
}

ECode RingtonePreference::SetRingtoneType(
    /* [in] */ Int32 type)
{
    mRingtoneType = type;
    return NOERROR;
}

ECode RingtonePreference::GetShowDefault(
    /* [out] */ Boolean* showDefault)
{
    VALIDATE_NOT_NULL(showDefault)
    *showDefault = mShowDefault;
    return NOERROR;
}

ECode RingtonePreference::SetShowDefault(
    /* [in] */ Boolean showDefault)
{
    mShowDefault = showDefault;
    return NOERROR;
}

ECode RingtonePreference::GetShowSilent(
    /* [out] */ Boolean* showSilent)
{
    VALIDATE_NOT_NULL(showSilent)
    *showSilent = mShowSilent;
    return NOERROR;
}

ECode RingtonePreference::SetShowSilent(
    /* [in] */ Boolean showSilent)
{
    mShowSilent = showSilent;
    return NOERROR;
}

ECode RingtonePreference::OnClick()
{
    AutoPtr<IIntent> intent;
    CIntent::New(IRingtoneManager::ACTION_RINGTONE_PICKER, (IIntent**)&intent);
    OnPrepareRingtonePickerIntent(intent);
    AutoPtr<IPreferenceManager> preferencemanager;
    GetPreferenceManager((IPreferenceManager**)&preferencemanager);
    AutoPtr<IPreferenceFragment> owningFragment;
    preferencemanager->GetFragment((IPreferenceFragment**)&owningFragment);
    if (owningFragment != NULL) {
        AutoPtr<IFragment> fragment = IFragment::Probe(owningFragment);
        fragment->StartActivityForResult(intent, mRequestCode);
    }
    else {
        AutoPtr<IActivity> activity;
        preferencemanager->GetActivity((IActivity**)&activity);
        activity->StartActivityForResult(intent, mRequestCode);
    }

    return NOERROR;
}

ECode RingtonePreference::OnPrepareRingtonePickerIntent(
    /* [in] */ IIntent* ringtonePickerIntent)
{
    assert(0);
#if 0 //struct IParcelable' has no member named 'PutParcelableExtra
    AutoPtr<IUri> uri;
    OnRestoreRingtone((IUri**)&uri);
    IParcelable::Probe(ringtonePickerIntent)->PutParcelableExtra(IRingtoneManager::EXTRA_RINGTONE_EXISTING_URI,
            IParcelable::Probe(uri));
    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_DEFAULT, mShowDefault);
    if (mShowDefault) {
        Int32 type;
        GetRingtoneType(&type);
        AutoPtr<IUri> defaultUri;
        AutoPtr<IRingtoneManagerHelper> helper;
        CRingtoneManagerHelper::AcquireSingleton((IRingtoneManagerHelper**)&helper);
        helper->GetDefaultUri(type, (IUri**)&defaultUri);
        ringtonePickerIntent->PutParcelableExtra(IRingtoneManager::EXTRA_RINGTONE_DEFAULT_URI,
                IParcelable::Probe(defaultUri));
    }

    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_SILENT, mShowSilent);
    ringtonePickerIntent->PutInt32Extra(IRingtoneManager::EXTRA_RINGTONE_TYPE, mRingtoneType);
    AutoPtr<ICharSequence> cs;
    GetTitle((ICharSequence**)&cs);
    ringtonePickerIntent->PutCharSequenceExtra(IRingtoneManager::EXTRA_RINGTONE_TITLE, cs);
#endif
    return NOERROR;
}

ECode RingtonePreference::OnSaveRingtone(
    /* [in] */ IUri* ringtoneUri)
{
    String uri("");
    if (ringtoneUri != NULL) IObject::Probe(ringtoneUri)->ToString((String*)&uri);
    Boolean result;
    return PersistString(uri, &result);
}

ECode RingtonePreference::OnRestoreRingtone(
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

ECode RingtonePreference::OnGetDefaultValue(
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

ECode RingtonePreference::OnSetInitialValue(
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

ECode RingtonePreference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    FAIL_RETURN(Preference::OnAttachedToHierarchy(preferenceManager))

    preferenceManager->RegisterOnActivityResultListener(this);
    preferenceManager->GetNextRequestCode(&mRequestCode);
    return NOERROR;
}

ECode RingtonePreference::OnActivityResult(
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
            if (uri != NULL) IObject::Probe(uri)->ToString((String*)&str);
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

}
}
}

