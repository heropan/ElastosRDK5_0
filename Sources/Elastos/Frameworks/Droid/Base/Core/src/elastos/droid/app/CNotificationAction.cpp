#include "CNotificationAction.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/app/CPendingIntent.h"

using Elastos::Droid::App::CPendingIntent;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL_2(CNotificationAction, Object, INotificationAction, IParcelable)

CAR_OBJECT_IMPL(CNotificationAction)

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
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence* title,
    /* [in] */ IPendingIntent* intent)
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    return constructor(icon, title, intent, bundle, NULL);
}

ECode CNotificationAction::constructor(
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence* title,
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IBundle* extras,
    /* [in] */ ArrayOf<IRemoteInput*>* remoteInputs)
{
    mIcon = icon;
    mTitle = title;
    mActionIntent = intent;
    mExtras = extras;
    if (mExtras == NULL) {
        CBundle::New((IBundle**)&mExtras);
    }
    mRemoteInputs = remoteInputs;
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

    out.writeBundle(mExtras);
    out.writeTypedArray(mRemoteInputs, flags);
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

    mExtras = in.readBundle();
    mRemoteInputs = in.createTypedArray(RemoteInput.CREATOR);
    return NOERROR;
}

CARAPI CNotificationAction::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

CARAPI CNotificationAction::GetRemoteInputs(
    /* [out, callee] */ ArrayOf<IRemoteInput*>** inputs)
{
    VALIDATE_NOT_NULL(inputs)
    *inputs = mRemoteInputs;
    REFCOUNT_ADD(*inputs)
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

    return new Action(
            icon,
            title,
            actionIntent, // safe to alias
            new Bundle(mExtras),
            getRemoteInputs());
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
