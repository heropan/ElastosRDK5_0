
#include "ext/frameworkext.h"
#include "view/inputmethod/CExtractedText.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

const Int32 CExtractedText::FLAG_SINGLE_LINE = 0x0001;
const Int32 CExtractedText::FLAG_SELECTING = 0x0002;

CExtractedText::CExtractedText()
    : mStartOffset(0)
    , mPartialStartOffset(0)
    , mPartialEndOffset(0)
    , mSelectionStart(0)
    , mSelectionEnd(0)
    , mFlags(0)
{}

ECode CExtractedText::constructor()
{
    return NOERROR;
}

ECode CExtractedText::SetText(
    /* [in] */ ICharSequence* text)
{
    mText = text;
    return NOERROR;
}

ECode CExtractedText::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CExtractedText::SetPartialStartOffset(
    /* [in] */ Int32 offset)
{
    mPartialStartOffset = offset;
    return NOERROR;
}

ECode CExtractedText::GetPartialStartOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mPartialStartOffset;
    return NOERROR;
}

ECode CExtractedText::SetPartialEndOffset(
    /* [in] */ Int32 offset)
{
    mPartialEndOffset = offset;
    return NOERROR;
}

ECode CExtractedText::GetPartialEndOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mPartialEndOffset;
    return NOERROR;
}

ECode CExtractedText::SetSelectionStart(
    /* [in] */ Int32 value)
{
    mSelectionStart = value;
    return NOERROR;
}

ECode CExtractedText::GetSelectionStart(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSelectionStart;
    return NOERROR;
}

ECode CExtractedText::SetSelectionEnd(
    /* [in] */ Int32 value)
{
    mSelectionEnd = value;
    return NOERROR;
}

ECode CExtractedText::GetSelectionEnd(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSelectionEnd;
    return NOERROR;
}

ECode CExtractedText::SetFlags(
    /* [in] */ Int32 value)
{
    mFlags = value;
    return NOERROR;
}

ECode CExtractedText::GetFlags(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mFlags;
    return NOERROR;
}

ECode CExtractedText::GetStartOffset(
    /* [out] */ Int32* startOffset)
{
    VALIDATE_NOT_NULL(startOffset);
    *startOffset = mStartOffset;
    return NOERROR;
}

ECode CExtractedText::SetStartOffset(
    /* [in] */ Int32 startOffset)
{
    mStartOffset = startOffset;
    return NOERROR;
}

ECode CExtractedText::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CExtractedText::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
