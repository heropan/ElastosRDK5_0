#include "elastos/droid/text/style/EasyEditSpan.h"
// #include "elastos/droid/app/CPendingIntent.h"

// using Elastos::Droid::App::CPendingIntent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

EasyEditSpan::EasyEditSpan()
    : mDeleteEnabled(TRUE)
{
}

ECode EasyEditSpan::constructor()
{
    return NOERROR;
}

ECode EasyEditSpan::constructor(
    /* [in] */ IPendingIntent* intent)
{
    mPendingIntent = intent;
    return NOERROR;
}

ECode EasyEditSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mPendingIntent = NULL;
    assert(0 && "TODO");
    // CPendingIntent::New((IPendingIntent**)&mPendingIntent);
    IParcelable::Probe(mPendingIntent)->ReadFromParcel(source);
    Byte b;
    source->ReadByte(&b);
    mDeleteEnabled = (b == 1);
    return NOERROR;
}

ECode EasyEditSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    IParcelable::Probe(mPendingIntent)->WriteToParcel(dest);
    dest->WriteByte(mDeleteEnabled ? (Byte)1 : (Byte) 0);
    return NOERROR;
}

ECode EasyEditSpan::GetSpanTypeId(
    /* [in] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = ITextUtils::EASY_EDIT_SPAN;
    return NOERROR;
}

ECode EasyEditSpan::IsDeleteEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled)
    *enabled = mDeleteEnabled;
    return NOERROR;
}

ECode EasyEditSpan::SetDeleteEnabled(
    /* [in] */ Boolean value)
{
    mDeleteEnabled = value;
    return NOERROR;
}

ECode EasyEditSpan::GetPendingIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mPendingIntent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos