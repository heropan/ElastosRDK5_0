#include "CNotificationAction.h"
#include "text/TextUtils.h"
#include "app/CPendingIntent.h"

using Elastos::Droid::App::CPendingIntent;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace App {

CNotificationAction::CNotificationAction()
    : mIcon(-1)
{
}

CNotificationAction::~CNotificationAction()
{
}

ECode CNotificationAction::constructor()
{
    return NOERROR;
}

ECode CNotificationAction::constructor(
    /* [in] */ Int32 icon_,
    /* [in] */ ICharSequence* title_,
    /* [in] */ IPendingIntent* intent_)
{
    mIcon = icon_;
    mTitle = title_;
    mActionIntent = intent_;
    return NOERROR;
}

ECode CNotificationAction::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(mIcon);
    TextUtils::WriteToParcel(mTitle, out);
    if (mActionIntent != NULL) {
        out->WriteInt32(1);
        IParcelable* parcleable = IParcelable::Probe(mActionIntent.Get());
        assert(parcleable);
        parcleable->WriteToParcel(out);
    } else {
        out->WriteInt32(0);
    }
    return NOERROR;
}

ECode CNotificationAction::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    in->ReadInt32(&mIcon);
    mTitle = NULL;
    FAIL_RETURN(TextUtils::CHAR_SEQUENCE_CREATOR::CreateFromParcel(in, (ICharSequence**)&mTitle));

    mActionIntent = NULL;
    Int32 value;
    in->ReadInt32(&value);
    if (value == 1) {
        CPendingIntent::New((IPendingIntent**)&mActionIntent);
        IParcelable* parcleable = IParcelable::Probe(mActionIntent.Get());
        assert(parcleable);
        parcleable->ReadFromParcel(in);
    }
    return NOERROR;
}

ECode CNotificationAction::Clone(
    /* [out] */ INotificationAction** action)
{
    assert(0 && "TODO");
    // VALIDATE_NOT_NULL(action);
    // *action->mIcon = mIcon;
    // mTitle->ToString(&(*action->mTitle));
    // *action->mActionIntent = mActionIntent;
    // return NOERROR;
    return E_NOT_IMPLEMENTED;
}

ECode CNotificationAction::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CNotificationAction::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    REFCOUNT_ADD(*title);
    return NOERROR;
}

ECode CNotificationAction::GetActionIntent(
    /* [out] */ IPendingIntent** actionIntent)
{
    VALIDATE_NOT_NULL(actionIntent);
    *actionIntent = mActionIntent;
    REFCOUNT_ADD(*actionIntent);
    return NOERROR;
}

}
}
}
