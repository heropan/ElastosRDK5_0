#include "elastos/droid/text/SpannedString.h"
#include "elastos/droid/text/CSpannedString.h"

using Elastos::Core::EIID_ICharSequence;
using Elastos::Droid::Text::CSpannedString;

namespace Elastos {
namespace Droid {
namespace Text {

SpannedString::SpannedString()
{
}

SpannedString::SpannedString(
    /* [in] */ ICharSequence* source) :
    SpannableStringInternal(source, 0, GetLen(source))
{
}

SpannedString::SpannedString(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end) :
    SpannableStringInternal(source, start, end)
{
}

AutoPtr<ICharSequence> SpannedString::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ISpannedString> spannedString;
    CSpannedString::New(ICharSequence::Probe(THIS_PROBE(ISpannedString)), start, end, (ISpannedString**)&spannedString);
    AutoPtr<ICharSequence> csq = ICharSequence::Probe(spannedString);
    return csq;
}

ECode SpannedString::Init(
    /* [in] */ ICharSequence* source)
{
    SpannableStringInternal::Init(source, 0, GetLen(source));
    return NOERROR;
}

ECode SpannedString::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    SpannableStringInternal::Init(source, start, end);
    return NOERROR;
}

Int32 SpannedString::GetLen(
    /* [in] */ ICharSequence* source)
{
    Int32 len = 0;
    if (source != NULL) source->GetLength(&len);
    return len;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
