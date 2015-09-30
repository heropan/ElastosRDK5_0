#include "elastos/droid/text/style/SpellCheckSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

SpellCheckSpan::SpellCheckSpan()
{
    Init();
}

SpellCheckSpan::SpellCheckSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void SpellCheckSpan::Init()
{
    mSpellCheckInProgress = FALSE;
}

void SpellCheckSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

void SpellCheckSpan::SetSpellCheckInProgress(
    /* [in] */ Boolean inProgress)
{
    mSpellCheckInProgress = inProgress;
}

Boolean SpellCheckSpan::IsSpellCheckInProgress()
{
    return mSpellCheckInProgress;
}

Int32 SpellCheckSpan::DescribeContents()
{
    return 0;
}

ECode SpellCheckSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadBoolean(&mSpellCheckInProgress);
}

ECode SpellCheckSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteBoolean(mSpellCheckInProgress);
}

Int32 SpellCheckSpan::GetSpanTypeId()
{
    return ITextUtils::SPELL_CHECK_SPAN;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos