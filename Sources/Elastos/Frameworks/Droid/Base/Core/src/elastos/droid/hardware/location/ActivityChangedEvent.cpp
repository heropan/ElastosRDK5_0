
#include "elastos/droid/hardware/location/ActivityChangedEvent.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/Arrays.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Arrays;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Location {

CAR_INTERFACE_IMPL_2(ActivityChangedEvent, Object, IActivityChangedEvent, IParcelable)

ActivityChangedEvent::ActivityChangedEvent()
    : mActivityRecognitionEvents(NULL)
{
}

ActivityChangedEvent::~ActivityChangedEvent()
{
}

ECode ActivityChangedEvent::constructor()
{
    return NOERROR;
}

ECode ActivityChangedEvent::constructor(
    /* [in] */ ArrayOf<IActivityRecognitionEvent*>* activityRecognitionEvents)
{
    if (activityRecognitionEvents == NULL) {
        // throw new InvalidParameterException(
        //         "Parameter 'activityRecognitionEvents' must not be null.");
        return E_INVALID_PARAMETER_EXCEPTION;
    }

    return Arrays::AsList(activityRecognitionEvents, (IList**)&mActivityRecognitionEvents);
}

ECode ActivityChangedEvent::GetActivityRecognitionEvents(
    /* [out] */ IIterable** result)
{
    VALIDATE_NOT_NULL(result);

    *result = IIterable::Probe(mActivityRecognitionEvents);
    REFCOUNT_ADD(*result);

    return NOERROR;
}

ECode ActivityChangedEvent::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);

    *val = 0;
    return NOERROR;
}

ECode ActivityChangedEvent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    AutoPtr<ArrayOf<IActivityRecognitionEvent*> > activityRecognitionEventArray;
    FAIL_RETURN(mActivityRecognitionEvents->ToArray((ArrayOf<IInterface*>**)&activityRecognitionEventArray))
    FAIL_RETURN(dest->WriteInt32(activityRecognitionEventArray->GetLength()))
    FAIL_RETURN(dest->WriteArrayOf((Handle32)activityRecognitionEventArray.Get()))
    return NOERROR;
}

ECode ActivityChangedEvent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 length;
    FAIL_RETURN(source->ReadInt32(&length))
    AutoPtr<ArrayOf<IActivityRecognitionEvent*> > data;
    data = ArrayOf<IActivityRecognitionEvent*>::Alloc(length);
    FAIL_RETURN(source->ReadArrayOf((Handle32*)&data))
    FAIL_RETURN(Arrays::AsList(data, (IList**)&mActivityRecognitionEvents))
    return NOERROR;
}

ECode ActivityChangedEvent::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder builder;
    builder += "[ ActivityChangedEvent:";

    AutoPtr<IInterface> obj;
    Int32 size;
    FAIL_RETURN(mActivityRecognitionEvents->GetSize(&size))
    for (Int32 i = 0; i < size; i++) {
        mActivityRecognitionEvents->Get(i, (IInterface**)&obj);
        builder += "\n    ";
        String tmp = Object::ToString(obj);
        builder += tmp;
    }

    builder += ("\n]");

    return builder.ToString(str);
}

} //Location
} //Hardware
} //Droid
} //Elastos