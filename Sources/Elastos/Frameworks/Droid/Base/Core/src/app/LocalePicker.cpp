
#include "app/LocalePicker.h"
#include "app/ActivityManagerNative.h"
//#include "app/backup/CBackupManagerHelper.h"
#include "content/res/CResources.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Character.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::Character;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IComparable;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::Backup::IBackupManagerHelper;
//using Elastos::Droid::App::Backup::CBackupManagerHelper;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::EIID_IArrayAdapter;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Widget::EIID_IListAdapter;
using Elastos::Droid::Widget::EIID_IAdapter;
using Elastos::Droid::Widget::EIID_ISpinnerAdapter;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IAssetManager;


namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {


AutoPtr<IView> _LocalArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    AutoPtr<ITextView> text;
    if (convertView == NULL) {
        mInflater->Inflate(mLayoutId, parent, FALSE, (IView**)&view);
        AutoPtr<IView> tempView;
        view->FindViewById(mFieldId, (IView**)&tempView);
        text = ITextView::Probe(tempView);
        view->SetTag(text);
    }
    else {
        view = convertView;
        AutoPtr<IView> tempView;
        view->GetTag((IInterface**)&tempView);
        text = ITextView::Probe(tempView);
    }

    AutoPtr<LocalePicker::LocaleInfo> item =
            (LocalePicker::LocaleInfo*)GetItem(position)->Probe(EIID_IComparable);
    String string;
    item->ToString(&string);
    AutoPtr<ICharSequence> cText;
    CStringWrapper::New(string, (ICharSequence**)&cText);
    text->SetText(cText);
    AutoPtr<ILocale> locale;
    item->GetLocale((ILocale**)&locale);
    text->SetTextLocale(locale);

    return view;
}

IARRAYADAPTER_METHODS_IMPL(LocalArrayAdapter, _LocalArrayAdapter);
IBASEADAPTER_METHODS_IMPL(LocalArrayAdapter, _LocalArrayAdapter);
ILISTADAPTER_METHODS_IMPL(LocalArrayAdapter, _LocalArrayAdapter);
IADAPTER_METHODS_IMPL(LocalArrayAdapter, _LocalArrayAdapter);
ISPINNERADAPTER_METHODS_IMPL(LocalArrayAdapter, _LocalArrayAdapter);

UInt32 LocalArrayAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalArrayAdapter::Release()
{
    return ElRefBase::Release();
}

PInterface LocalArrayAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IArrayAdapter*)this;
    }
    else if (riid == EIID_IArrayAdapter) {
        return (PInterface)(IArrayAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (PInterface)(IBaseAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (PInterface)(IListAdapter*)this;
    }
    else if (riid == EIID_IAdapter) {
        return (PInterface)(IAdapter*)this;
    }
    else if (riid == EIID_ISpinnerAdapter) {
        return (PInterface)(ISpinnerAdapter*)this;
    }
    return NULL;
}

ECode LocalArrayAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IArrayAdapter *)this) {
        *pIID = EIID_IArrayAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IBaseAdapter *)this) {
        *pIID = EIID_IBaseAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IListAdapter *)this) {
        *pIID = EIID_IListAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IAdapter *)this) {
        *pIID = EIID_IAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ISpinnerAdapter *)this) {
        *pIID = EIID_ISpinnerAdapter;
        return NOERROR;
    }


    return E_INVALID_ARGUMENT;
}

const String LocalePicker::TAG = String("LocalePicker");

CAR_INTERFACE_IMPL(LocalePicker::LocaleInfo, IComparable);

LocalePicker::LocaleInfo::LocaleInfo(
    /* [in] */ const String& label,
    /* [in] */ ILocale* locale)
    : mLabel(label)
    , mLocale(locale)
{
    Init();
}

ECode LocalePicker::LocaleInfo::GetLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);

    *label = mLabel;
    return NOERROR;
}

ECode LocalePicker::LocaleInfo::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    *locale = mLocale;
    REFCOUNT_ADD(*locale);
    return NOERROR;
}

ECode LocalePicker::LocaleInfo::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    *string = mLabel;
    return NOERROR;
}

ECode LocalePicker::LocaleInfo::CompareTo(
    /* [in] */ IInterface* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

//    return sCollator->Compare(mLabel, (LocaleInfo*)another->mLabel);
    return E_NOT_IMPLEMENTED;
}

ECode LocalePicker::Init()
{
//    return Collator::GetInstance((ICollator**)&sCollator);
    return E_NOT_IMPLEMENTED;
}

ECode LocalePicker::ConstructAdapter(
    /* [in] */ IContext* context,
    /* [out] */ IArrayAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);

    return ConstructAdapter(context, 0/*R.layout.locale_picker_item*/, 0/*R.id.locale*/,
            adapter);
}

ECode LocalePicker::ConstructAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutId,
    /* [in] */ Int32 fieldId,
    /* [out] */ IArrayAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<IAssetManager> assetMgr;
    CResources::GetSystem()->GetAssets((IAssetManager**)&assetMgr);
    AutoPtr<ArrayOf<String> > locales;
    assetMgr->GetLocales((ArrayOf<String>**)&locales);
    AutoPtr<ArrayOf<String> > specialLocaleCodes;
    resources->GetStringArray(0/*R.array.special_locale_codes*/, (ArrayOf<String>**)&specialLocaleCodes);
    AutoPtr<ArrayOf<String> > specialLocaleNames;
    resources->GetStringArray(0/*R.array.special_locale_names*/, (ArrayOf<String>**)&specialLocaleNames);
//     Arrays.sort(locales);
    Int32 origSize = locales->GetLength();
    AutoPtr<ArrayOf<LocaleInfo*> > preprocess = ArrayOf<LocaleInfo*>::Alloc(origSize);
    Int32 finalSize = 0;
    for (Int32 i = 0 ; i < origSize; i++ ) {
        const String s = (*locales)[i];
        const Int32 len = s.GetLength();
        if (len == 5) {
            String language = s.Substring(0, 2);
            String country = s.Substring(3, 5);
            AutoPtr<ILocale> l;
//            CLocale::New(language, country, (ILocale**)&l);

            if (finalSize == 0) {
                if (DEBUG) {
                    String lang;
                    l->GetDisplayLanguage(l, &lang);
                    Slogger::V(TAG, "adding initial %s", ToTitleCase(lang).string());
                }
                String language;
                l->GetDisplayLanguage(l, &language);
                preprocess->Set(finalSize++, new LocaleInfo(ToTitleCase(language), l));
            } else {
                // check previous entry:
                //  same lang and a country -> upgrade to full name and
                //    insert ours with full name
                //  diff lang -> insert ours with lang-only name
                String lang;
                (*preprocess)[finalSize-1]->mLocale->GetLanguage(&lang);
                if (lang == language) {
                    if (DEBUG) {
                        Slogger::V(TAG, "backing up and fixing %s to %s",
                                (*preprocess)[finalSize-1]->mLabel.string(),
                                GetDisplayName((*preprocess)[finalSize-1]->mLocale,
                                        specialLocaleCodes, specialLocaleNames).string());
                    }
                    (*preprocess)[finalSize-1]->mLabel = ToTitleCase(
                            GetDisplayName((*preprocess)[finalSize-1]->mLocale,
                                    specialLocaleCodes, specialLocaleNames));
                    if (DEBUG) {
                        Slogger::V(TAG, "  and adding %s", ToTitleCase(
                                GetDisplayName(l, specialLocaleCodes, specialLocaleNames)).string());
                    }
                    preprocess->Set(finalSize++,
                            new LocaleInfo(ToTitleCase(
                                    GetDisplayName(
                                            l, specialLocaleCodes, specialLocaleNames)), l));
                } else {
                    String displayName;
                    if (s == String("zz_ZZ")) {
                        displayName = String("Pseudo...");
                    } else {
                        String lang;
                        l->GetDisplayLanguage(l, &lang);
                        displayName = ToTitleCase(lang);
                    }
                    if (DEBUG) {
                        Slogger::V(TAG, "adding %s", displayName.string());
                    }
                    preprocess->Set(finalSize++, new LocaleInfo(displayName, l));
                }
            }
        }
    }

    AutoPtr<IObjectContainer> localeInfos;
    for (Int32 i = 0; i < finalSize; i++) {
        localeInfos->Add((IInterface*)(*preprocess)[i]);
    }
//     Arrays.sort(localeInfos);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE,
            (IInterface**)(ILayoutInflater**)&inflater);
    *adapter = new LocalArrayAdapter(context, layoutId, fieldId, localeInfos, inflater);
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

String LocalePicker::ToTitleCase(
    /* [in] */ const String& s)
{
    if (s.IsNullOrEmpty()) {
        return s;
    }

    return String((char*)Character::ToUpperCase(s.GetChar(0))) + s.Substring(1);
}

String LocalePicker::GetDisplayName(
    /* [in] */ ILocale* l,
    /* [in] */ ArrayOf<String>* specialLocaleCodes,
    /* [in] */ ArrayOf<String>* specialLocaleNames)
{
    String code;
    l->ToString(&code);

    for (Int32 i = 0; i < specialLocaleCodes->GetLength(); i++) {
        if ((*specialLocaleCodes)[i] == code) {
            return (*specialLocaleNames)[i];
        }
    }

    String name;
    l->GetDisplayName(l, &name);
    return name;
}

ECode LocalePicker::OnActivityCreated(
    /* [in] */ IBundle* savedInstanceState)
{
    ListFragment::OnActivityCreated(savedInstanceState);
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    AutoPtr<IArrayAdapter> adapter;
    ConstructAdapter((IContext*)activity, (IArrayAdapter**)&adapter);
    SetListAdapter((IListAdapter*)adapter->Probe(EIID_IListAdapter));
    return NOERROR;
}

ECode LocalePicker::SetLocaleSelectionListener(
    /* [in] */ ILocaleSelectionListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode LocalePicker::OnResume()
{
    ListFragment::OnResume();
    AutoPtr<IListView> lview;
    GetListView((IListView**)&lview);
    Boolean result;
    lview->RequestFocus(&result);
    return NOERROR;
}

ECode LocalePicker::OnListItemClick(
    /* [in] */ IListView* l,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mListener != NULL) {
        AutoPtr<IListAdapter> ladapter;
        GetListAdapter((IListAdapter**)&ladapter);
        AutoPtr<IInterface> item;
        ladapter->GetItem(position, (IInterface**)&item);
        AutoPtr<ILocale> locale = ((LocaleInfo*)item->Probe(EIID_IComparable))->mLocale;
        mListener->OnLocaleSelected(locale);
    }
    return NOERROR;
}

ECode LocalePicker::UpdateLocale(
    /* [in] */ ILocale* locale)
{
//     try {
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IConfiguration> config;
    am->GetConfiguration((IConfiguration**)&config);

    // Will set userSetLocale to indicate this isn't some passing default - the user
    // wants this remembered
    config->SetLocale(locale);

    am->UpdateConfiguration(config);
    // Trigger the dirty bit for the Settings Provider.
    AutoPtr<IBackupManagerHelper> backupMgrHelper;
//    CBackupManagerHelper::AcquireSingleton((IBackupManagerHelper**)&backupMgrHelper);
    backupMgrHelper->DataChanged(String("com.android.providers.settings"));
//     } catch (RemoteException e) {
//         // Intentionally left blank
//     }
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
