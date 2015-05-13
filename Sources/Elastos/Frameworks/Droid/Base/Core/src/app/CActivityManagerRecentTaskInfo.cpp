
#include "app/CActivityManagerRecentTaskInfo.h"
#include <ext/frameworkext.h>
#include "content/CComponentName.h"
#include "content/CIntent.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace App {

CActivityManagerRecentTaskInfo::CActivityManagerRecentTaskInfo()
    : mId(0)
    , mPersistentId(0)
{
}

CActivityManagerRecentTaskInfo::~CActivityManagerRecentTaskInfo()
{
}

ECode CActivityManagerRecentTaskInfo::constructor()
{
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mId);
    dest->WriteInt32(mPersistentId);
    dest->WriteInterfacePtr(mOrigActivity);
    dest->WriteInterfacePtr(mBaseIntent);
    dest->WriteInterfacePtr(mDescription);
    // TODO
//    TextUtils.writeToParcel(description, dest,
//            Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    source->ReadInt32(&mId);
    source->ReadInt32(&mPersistentId);
    source->ReadInterfacePtr((Handle32*)&obj);
    mOrigActivity = IComponentName::Probe(obj);
    obj = NULL;
    source->ReadInterfacePtr((Handle32*)&obj);
    mBaseIntent = IIntent::Probe(obj);
    obj = NULL;
    source->ReadInterfacePtr((Handle32*)&obj);
    mDescription = ICharSequence::Probe(obj);
    // TODO
//    description = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::GetPersistentId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mPersistentId;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::SetPersistentId(
    /* [in] */ Int32 id)
{
    mPersistentId = id;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::GetBaseIntent(
    /* [out] */ IIntent** baseIntent)
{
    VALIDATE_NOT_NULL(baseIntent);
    *baseIntent = mBaseIntent;
    INTERFACE_ADDREF(*baseIntent);
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::SetBaseIntent(
    /* [in] */ IIntent* baseIntent)
{
    mBaseIntent = baseIntent;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::GetOrigActivity(
    /* [out] */ IComponentName** origActivity)
{
    VALIDATE_NOT_NULL(origActivity);
    *origActivity = mOrigActivity;
    INTERFACE_ADDREF(*origActivity);
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::SetOrigActivity(
    /* [in] */ IComponentName* origActivity)
{
    mOrigActivity = origActivity;
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::GetDescription(
    /* [out] */ ICharSequence** description)
{
    VALIDATE_NOT_NULL(description);
    *description = mDescription;
    INTERFACE_ADDREF(*description);
    return NOERROR;
}

ECode CActivityManagerRecentTaskInfo::SetDescription(
    /* [in] */ ICharSequence* description)
{
    mDescription = description;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
