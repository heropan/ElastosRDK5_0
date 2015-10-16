#include "elastos/droid/text/style/EasyEditSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

EasyEditSpan::EasyEditSpan()
    : mDeleteEnabled(TRUE)
{
}

CARAPI constructor()
{
}

CARAPI constructor(
    /* [in] */ IPendingIntent* intent)
{

}

ECode EasyEditSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
        mPendingIntent = source.readParcelable(null);
        mDeleteEnabled = (source.readByte() == 1);
    return NOERROR;
}

ECode EasyEditSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
        dest.writeParcelable(mPendingIntent, 0);
        dest.writeByte((byte) (mDeleteEnabled ? 1 : 0));
    return NOERROR;
}

Int32 EasyEditSpan::GetSpanTypeId()
{
    return ITextUtils::EASY_EDIT_SPAN;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos