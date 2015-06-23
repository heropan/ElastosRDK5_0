
#ifndef __CSPELLCHECKSPAN_H__
#define __CSPELLCHECKSPAN_H__

#include "_CSpellCheckSpan.h"
#include "text/style/SpellCheckSpan.h"


namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CSpellCheckSpan),public SpellCheckSpan
{
public:
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI SetSpellCheckInProgress(
        /* [in] */ Boolean inProgress);

    CARAPI IsSpellCheckInProgress(
        /* [out] */ Boolean* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSPELLCHECKSPAN_H__
