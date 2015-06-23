
#ifndef __CIMAGESPAN_H__
#define __CIMAGESPAN_H__

#include "_CImageSpan.h"
#include "text/style/ImageSpan.h"


//
//

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CImageSpan),public ImageSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    //IREPLACEMENTSPAN_METHODS_DECL()
    //IDynamicDrawableSpan_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSize(
        /* [in] */ IPaint* paint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaintFontMetricsInt* fm,
        /* [out] */ Int32* size);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ IPaint* paint);

    CARAPI GetVerticalAlignment(
        /* [out] */ Int32* ret);

    CARAPI GetDrawable(
        /* [out] */ IDrawable** ret);

    CARAPI GetSource(
        /* [out] */ String* ret);

    CARAPI constructor(
        /* [in] */ IBitmap* b);

    CARAPI constructor(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    CARAPI constructor(
        /* [in] */ IDrawable* d);

    CARAPI constructor(
        /* [in] */ IDrawable* d,
        /* [in] */ Int32 verticalAlignment);

    CARAPI constructor(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source);

    CARAPI constructor(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source,
        /* [in] */ Int32 verticalAlignment);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IUri* pUri);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 verticalAlignment);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId,
        /* [in] */ Int32 verticalAlignment);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CIMAGESPAN_H__
