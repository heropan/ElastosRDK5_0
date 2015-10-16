#ifndef __ELASTOS_DROID_TEXT_STYLE_SpellCheckSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_SpellCheckSpan_H__

#include "Elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * A SpellCheckSpan is an internal data structure created by the TextView's SpellChecker to
 * annotate portions of the text that are about to or currently being spell checked. They are
 * automatically removed once the spell check is completed.
 *
 * @hide
 */
class SpellCheckSpan
    : public Object
    , public IsSpellCheckSpan
    , public IParcelableSpan
    , public IParcelable
{
public:
    SpellCheckSpan();

    SpellCheckSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init();

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(void) SetSpellCheckInProgress(
        /* [in] */ Boolean inProgress);

    CARAPI_(Boolean) IsSpellCheckInProgress();

    //@Override
    CARAPI_(Int32) DescribeContents();

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI_(Int32) GetSpanTypeId();

private:
    Boolean mSpellCheckInProgress;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_SpellCheckSpan_H__
