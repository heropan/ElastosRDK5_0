
#include "CPreferenceActivityHeader.h"
#include "content/CIntent.h"
#include "PreferenceActivity.h"

using Elastos::Core::EIID_ICharSequence;
using Elastos::Droid::Os::EIID_IBundle;
using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace Preference {

CPreferenceActivityHeader::CPreferenceActivityHeader()
    : mId(PreferenceActivity::HEADER_ID_UNDEFINED)
    , mTitleRes(0)
    , mSummaryRes(0)
    , mBreadCrumbTitleRes(0)
    , mBreadCrumbShortTitleRes(0)
    , mIconRes(0)
{}

ECode CPreferenceActivityHeader::constructor()
{
    // Empty
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetTitleEx(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mTitleRes != 0) {
        return res->GetText(mTitleRes, title);
    }
    *title = mTitle;
    INTERFACE_ADDREF(*title)
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetSummaryEx(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    if (mSummaryRes != 0) {
        return res->GetText(mSummaryRes, summary);
    }
    *summary = mSummary;
    INTERFACE_ADDREF(*summary)
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbTitleEx(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mBreadCrumbTitleRes != 0) {
        return res->GetText(mBreadCrumbTitleRes, title);
    }
    *title = mBreadCrumbTitle;
    INTERFACE_ADDREF(*title)
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbShortTitleEx(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    if (mBreadCrumbShortTitleRes != 0) {
        return res->GetText(mBreadCrumbShortTitleRes, title);
    }
    *title = mBreadCrumbShortTitle;
    INTERFACE_ADDREF(*title);
    return NOERROR;
}

ECode CPreferenceActivityHeader::WriteToParcel(
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

ECode CPreferenceActivityHeader::ReadFromParcel(
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

ECode CPreferenceActivityHeader::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mId;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetId(
    /* [in] */ Int64 id)
{
    mId = id;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetTitleRes(
    /* [out] */ Int32* titleRes)
{
    VALIDATE_NOT_NULL(titleRes)
    *titleRes = mTitleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetTitleRes(
    /* [in] */ Int32 titleRes)
{
    mTitleRes = titleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    *title = mTitle;
    INTERFACE_ADDREF(*title)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetSummaryRes(
    /* [out] */ Int32* summaryRes)
{
    VALIDATE_NOT_NULL(summaryRes)
    *summaryRes = mSummaryRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetSummaryRes(
    /* [in] */ Int32 summaryRes)
{
    mSummaryRes = summaryRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = mSummary;
    INTERFACE_ADDREF(*summary)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    mSummary = summary;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbTitleRes(
    /* [out] */ Int32* breadCrumbTitleRes)
{
    VALIDATE_NOT_NULL(breadCrumbTitleRes)
    *breadCrumbTitleRes = mBreadCrumbTitleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetBreadCrumbTitleRes(
    /* [in] */ Int32 breadCrumbTitleRes)
{
    mBreadCrumbTitleRes = breadCrumbTitleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbTitle(
    /* [out] */ ICharSequence** breadCrumbTitle)
{
    VALIDATE_NOT_NULL(breadCrumbTitle)
    *breadCrumbTitle = mBreadCrumbTitle;
    INTERFACE_ADDREF(*breadCrumbTitle)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetBreadCrumbTitle(
    /* [in] */ ICharSequence* breadCrumbTitle)
{
    mBreadCrumbTitle = breadCrumbTitle;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbShortTitleRes(
    /* [out] */ Int32* breadCrumbShortTitleRes)
{
    VALIDATE_NOT_NULL(breadCrumbShortTitleRes);
    *breadCrumbShortTitleRes = mBreadCrumbShortTitleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetBreadCrumbShortTitleRes(
    /* [in] */ Int32 breadCrumbShortTitleRes)
{
    mBreadCrumbShortTitleRes = breadCrumbShortTitleRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetBreadCrumbShortTitle(
    /* [out] */ ICharSequence** breadCrumbShortTitle)
{
    VALIDATE_NOT_NULL(breadCrumbShortTitle)
    *breadCrumbShortTitle = mBreadCrumbShortTitle;
    INTERFACE_ADDREF(*breadCrumbShortTitle)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetBreadCrumbShortTitle(
    /* [in] */ ICharSequence* breadCrumbShortTitle)
{
    mBreadCrumbShortTitle = breadCrumbShortTitle;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetIconRes(
    /* [out] */ Int32* iconRes)
{
    VALIDATE_NOT_NULL(iconRes)
    *iconRes = mIconRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetIconRes(
    /* [in] */ Int32 iconRes)
{
    mIconRes = iconRes;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment)
    *fragment = mFragment;
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetFragment(
    /* [in] */ const String& fragment)
{
    mFragment = fragment;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetFragmentArguments(
    /* [out] */ IBundle** fragmentArguments)
{
    VALIDATE_NOT_NULL(fragmentArguments)
    *fragmentArguments = mFragmentArguments;
    INTERFACE_ADDREF(*fragmentArguments)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetFragmentArguments(
    /* [in] */ IBundle* fragmentArguments)
{
    mFragmentArguments = fragmentArguments;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mIntent;
    INTERFACE_ADDREF(*intent)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetIntent(
    /* [in] */ IIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

ECode CPreferenceActivityHeader::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    INTERFACE_ADDREF(*extras)
    return NOERROR;
}

ECode CPreferenceActivityHeader::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

}
}
}

