
#include "view/inputmethod/CCorrectionInfo.h"
#include "text/TextUtils.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CCorrectionInfo::CCorrectionInfo()
    : mOffset(0)
{
}

ECode CCorrectionInfo::constructor()
{
    return NOERROR;
}

ECode CCorrectionInfo::constructor(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* oldText,
    /* [in] */ ICharSequence* newText)
{
    mOffset = offset;
    mOldText = oldText;
    mNewText = newText;
    return NOERROR;
}

ECode CCorrectionInfo::GetOffset(
    /* [out] */ Int32* offset)
{
    assert(offset != NULL);
    *offset = mOffset;
    return NOERROR;
}

ECode CCorrectionInfo::GetOldText(
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    *text = mOldText;
    INTERFACE_ADDREF(*text);

    return NOERROR;
}

ECode CCorrectionInfo::GetNewText(
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    *text = mNewText;
    INTERFACE_ADDREF(*text);

    return NOERROR;
}

ECode CCorrectionInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    dest->WriteInt32(mOffset);
    assert(0);
    //TODO
    // TextUtils::WriteToParcel(mOldText, dest, flags);
    // TextUtils::WriteToParcel(mNewText, dest, flags);
    return NOERROR;
}

ECode CCorrectionInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(0);
    return NOERROR;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
