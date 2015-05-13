
#ifndef __CSTRIKETHROUGHSPAN_H__
#define __CSTRIKETHROUGHSPAN_H__

#include "_CStrikethroughSpan.h"
#include "text/style/StrikethroughSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/ParcelableSpanMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CStrikethroughSpan), public StrikethroughSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSTRIKETHROUGHSPAN_H__
