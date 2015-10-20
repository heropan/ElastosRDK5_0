
#include "elastos/droid/PreferenceActivityHeader.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/preference/PreferenceActivity.h"

using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace Preference {
CAR_INTERFACE_IMPL(PreferenceActivityHeader,Object, IPreferenceActivityHeader, IParcelable)

PreferenceActivityHeader::PreferenceActivityHeader()
    : mId(PreferenceActivity::HEADER_ID_UNDEFINED)
    , mTitleRes(0)
    , mSummaryRes(0)
    , mBreadCrumbTitleRes(0)
    , mBreadCrumbShortTitleRes(0)
    , mIconRes(0)
{}

ECode PreferenceActivityHeader::constructor()
{
    // Empty
    return NOERROR;
}

ECode PreferenceActivityHeader::GetTitle(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mTitleRes != 0) {
        return res->GetText(mTitleRes, title);
    }
    *title = mTitle;
    REFCOUNT_ADD(*title)
    return NOERROR;
}

ECode PreferenceActivityHeader::GetSummary(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    if (mSummaryRes != 0) {
        return res->GetText(mSummaryRes, summary);
    }
    *summary = mSummary;
    REFCOUNT_ADD(*summary)
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbTitle(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mBreadCrumbTitleRes != 0) {
        return res->GetText(mBreadCrumbTitleRes, title);
    }
    *title = mBreadCrumbTitle;
    REFCOUNT_ADD(*title)
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbShortTitle(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mBreadCrumbShortTitleRes != 0) {
        return res->GetText(mBreadCrumbShortTitleRes, title);
    }
    *title = mBreadCrumbShortTitle;
    REFCOUNT_ADD(*title);
    return NOERROR;
}

ECode PreferenceActivityHeader::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest)

    dest->WriteInt64(mId);
    dest->WriteInt32(mTitleRes);
    dest->WriteInterfacePtr(mTitle);
    dest->WriteInt32(mSummaryRes);
    dest->WriteInterfacePtr(mSummary);
    dest->WriteInt32(mBreadCrumbTitleRes);
    dest->WriteInterfacePtr(mBreadCrumbTitle);
    dest->WriteInt32(mBreadCrumbShortTitleRes);
    dest->WriteInterfacePtr(mBreadCrumbShortTitle);
    dest->WriteInt32(mIconRes);
    dest->WriteString(mFragment);
    dest->WriteInterfacePtr(mFragmentArguments);
    if (mIntent != NULL) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> parcelable = IParcelable::Probe(mIntent);
        parcelable->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(0);
    }
    dest->WriteInterfacePtr(mExtras);

    return NOERROR;
}

ECode PreferenceActivityHeader::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt64(&mId);
    source->ReadInt32(&mTitleRes);
    source->ReadInterfacePtr((Handle32*)(ICharSequence**)&mTitle);
    source->ReadInt32(&mSummaryRes);
    source->ReadInterfacePtr((Handle32*)(ICharSequence**)&mSummary);
    source->ReadInt32(&mBreadCrumbTitleRes);
    source->ReadInterfacePtr((Handle32*)(ICharSequence**)&mBreadCrumbTitle);
    source->ReadInt32(&mBreadCrumbShortTitleRes);
    source->ReadInterfacePtr((Handle32*)(ICharSequence**)&mBreadCrumbShortTitle);
    source->ReadInt32(&mIconRes);
    source->ReadString(&mFragment);
    source->ReadInterfacePtr((Handle32*)(IBundle**)&mFragmentArguments);

    Int32 value;
    source->ReadInt32(&value);
    if (value != 0) {
        CIntent::New((IIntent**)&mIntent);
        AutoPtr<IParcelable> p = IParcelable::Probe(mIntent);
        p->ReadFromParcel(source);
    }

    source->ReadInterfacePtr((Handle32*)(IBundle**)&mExtras);

    return NOERROR;
}

ECode PreferenceActivityHeader::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mId;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetId(
    /* [in] */ Int64 id)
{
    mId = id;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetTitleRes(
    /* [out] */ Int32* titleRes)
{
    VALIDATE_NOT_NULL(titleRes)
    *titleRes = mTitleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetTitleRes(
    /* [in] */ Int32 titleRes)
{
    mTitleRes = titleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    *title = mTitle;
    REFCOUNT_ADD(*title)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetSummaryRes(
    /* [out] */ Int32* summaryRes)
{
    VALIDATE_NOT_NULL(summaryRes)
    *summaryRes = mSummaryRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetSummaryRes(
    /* [in] */ Int32 summaryRes)
{
    mSummaryRes = summaryRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = mSummary;
    REFCOUNT_ADD(*summary)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    mSummary = summary;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbTitleRes(
    /* [out] */ Int32* breadCrumbTitleRes)
{
    VALIDATE_NOT_NULL(breadCrumbTitleRes)
    *breadCrumbTitleRes = mBreadCrumbTitleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetBreadCrumbTitleRes(
    /* [in] */ Int32 breadCrumbTitleRes)
{
    mBreadCrumbTitleRes = breadCrumbTitleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbTitle(
    /* [out] */ ICharSequence** breadCrumbTitle)
{
    VALIDATE_NOT_NULL(breadCrumbTitle)
    *breadCrumbTitle = mBreadCrumbTitle;
    REFCOUNT_ADD(*breadCrumbTitle)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetBreadCrumbTitle(
    /* [in] */ ICharSequence* breadCrumbTitle)
{
    mBreadCrumbTitle = breadCrumbTitle;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbShortTitleRes(
    /* [out] */ Int32* breadCrumbShortTitleRes)
{
    VALIDATE_NOT_NULL(breadCrumbShortTitleRes);
    *breadCrumbShortTitleRes = mBreadCrumbShortTitleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetBreadCrumbShortTitleRes(
    /* [in] */ Int32 breadCrumbShortTitleRes)
{
    mBreadCrumbShortTitleRes = breadCrumbShortTitleRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetBreadCrumbShortTitle(
    /* [out] */ ICharSequence** breadCrumbShortTitle)
{
    VALIDATE_NOT_NULL(breadCrumbShortTitle)
    *breadCrumbShortTitle = mBreadCrumbShortTitle;
    REFCOUNT_ADD(*breadCrumbShortTitle)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetBreadCrumbShortTitle(
    /* [in] */ ICharSequence* breadCrumbShortTitle)
{
    mBreadCrumbShortTitle = breadCrumbShortTitle;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetIconRes(
    /* [out] */ Int32* iconRes)
{
    VALIDATE_NOT_NULL(iconRes)
    *iconRes = mIconRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetIconRes(
    /* [in] */ Int32 iconRes)
{
    mIconRes = iconRes;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment)
    *fragment = mFragment;
    return NOERROR;
}

ECode PreferenceActivityHeader::SetFragment(
    /* [in] */ const String& fragment)
{
    mFragment = fragment;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetFragmentArguments(
    /* [out] */ IBundle** fragmentArguments)
{
    VALIDATE_NOT_NULL(fragmentArguments)
    *fragmentArguments = mFragmentArguments;
    REFCOUNT_ADD(*fragmentArguments)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetFragmentArguments(
    /* [in] */ IBundle* fragmentArguments)
{
    mFragmentArguments = fragmentArguments;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mIntent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetIntent(
    /* [in] */ IIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

ECode PreferenceActivityHeader::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode PreferenceActivityHeader::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

}
}
}

