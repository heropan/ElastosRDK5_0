
#include "hardware/input/CKeyboardLayout.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Input {

ECode CKeyboardLayout::constructor()
{
    return NOERROR;
}

ECode CKeyboardLayout::constructor(
    /* [in] */ const String& descriptor,
    /* [in] */ const String& label,
    /* [in] */ const String& collection)
{
    mDescriptor = descriptor;
    mLabel = label;
    mCollection = collection;

    return NOERROR;
}

ECode CKeyboardLayout::constructor(
   /* [in] */ IParcel* source)
{
    source->ReadString(&mDescriptor);
    source->ReadString(&mLabel);
    source->ReadString(&mCollection);

    return NOERROR;
}

ECode CKeyboardLayout::GetDescriptor(
    /* [out] */ String* descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    *descriptor = mDescriptor;
    return NOERROR;
}

ECode CKeyboardLayout::GetLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    return NOERROR;
}

ECode CKeyboardLayout::GetCollection(
    /* [out] */ String* collection)
{
    VALIDATE_NOT_NULL(collection);
    *collection = mCollection;
    return NOERROR;
}

ECode CKeyboardLayout::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mDescriptor));
    FAIL_RETURN(source->ReadString(&mLabel));
    FAIL_RETURN(source->ReadString(&mCollection));

    return NOERROR;
}

ECode CKeyboardLayout::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mDescriptor));
    FAIL_RETURN(dest->WriteString(mLabel));
    FAIL_RETURN(dest->WriteString(mCollection));

    return NOERROR;
}

ECode CKeyboardLayout::CompareTo(
    /* [in] */ IInterface* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(another);
    CKeyboardLayout* temp = (CKeyboardLayout*)IKeyboardLayout::Probe(another);
    if (!temp) {
        *result = -1;
        return NOERROR;
    }
    *result = mLabel.CompareIgnoreCase(temp->mLabel);
    if (*result == 0) {
        *result = mCollection.CompareIgnoreCase(temp->mCollection);
    }

    return NOERROR;
}

// //@Override
// public String toString() {
//     if (mCollection.isEmpty()) {
//         return mLabel;
//     }
//     return mLabel + " - " + mCollection;
// }

} // namespace Input
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
