#include "elastos/droid/media/session/CPlaybackStateCustomAction.h"
#include <elastos/utility/Objects.h>

using Elastos::Utility::Objects;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

CAR_INTERFACE_IMPL_2(CPlaybackStateCustomAction, Object, IPlaybackStateCustomAction, IParcelable)

CAR_OBJECT_IMPL(CPlaybackStateCustomAction)

CPlaybackStateCustomAction::CPlaybackStateCustomAction()
    : mAction(String(NULL))
    , mIcon(0)
{
}

CPlaybackStateCustomAction::~CPlaybackStateCustomAction()
{
}

ECode CPlaybackStateCustomAction::constructor(
    /* [in] */ const String& action,
    /* [in] */ ICharSequence * name,
    /* [in] */ Int32 icon,
    /* [in] */ IBundle * extras)
{
    mAction = action;
    mName = name;
    mIcon = icon;
    mExtras = extras;
    return NOERROR;
}

ECode CPlaybackStateCustomAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    source->ReadString(&mAction);
    source->ReadInterfacePtr((Handle32*)(IInterface**)&obj);
    mName = ICharSequence::Probe(obj);
    source->ReadInt32(&mIcon);
    source->ReadInterfacePtr((Handle32*)(IInterface**)&obj);
    mExtras = IBundle::Probe(obj);
    return NOERROR;
}

ECode CPlaybackStateCustomAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mAction);
    dest->WriteInterfacePtr(mName);
    dest->WriteInt32(mIcon);
    dest->WriteInterfacePtr(mExtras);
    return NOERROR;
}

ECode CPlaybackStateCustomAction::GetAction(
    /* [out] */ String * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mAction;
    return NOERROR;
}

ECode CPlaybackStateCustomAction::GetName(
    /* [out] */ ICharSequence ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mName;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateCustomAction::GetIcon(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mIcon;
    return NOERROR;
}

ECode CPlaybackStateCustomAction::GetExtras(
    /* [out] */ IBundle ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mExtras;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateCustomAction::ToString(
    /* [out] */ String * result)
{
    VALIDATE_NOT_NULL(result)
    *result = String("Action:") + mAction;
    String str;
    mName->ToString(&str);
    *result = *result + "mName='" + str + ", mIcon=" + mIcon;
    // mExtras->ToString(&str);
    // *result = *result + ", mExtras=" + str;
    return NOERROR;
}

} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos
