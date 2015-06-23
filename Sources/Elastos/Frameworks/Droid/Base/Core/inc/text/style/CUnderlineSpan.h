
#ifndef __CUNDERLINESPAN_H__
#define __CUNDERLINESPAN_H__

#include "_CUnderlineSpan.h"
#include "text/style/UnderlineSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CUnderlineSpan),public UnderlineSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CUNDERLINESPAN_H__
