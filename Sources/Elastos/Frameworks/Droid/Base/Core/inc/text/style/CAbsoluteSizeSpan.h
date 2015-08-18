
#ifndef __ELASTOS_DROID_TEXT_STYLE_CABSOLUTESIZESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CABSOLUTESIZESPAN_H__

#include "_Elastos_Droid_Text_Style_CAbsoluteSizeSpan.h"
#include "text/style/AbsoluteSizeSpan.h"




using Elastos::Droid::Text::Style::AbsoluteSizeSpan;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CAbsoluteSizeSpan)
    , public AbsoluteSizeSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSize(
        /* [out] */ Int32* ret);

    CARAPI GetDip(
        /* [out] */ Boolean* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI constructor(
        /* [in] */ Int32 size,
        /* [in] */ Boolean dip);

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CABSOLUTESIZESPAN_H__
