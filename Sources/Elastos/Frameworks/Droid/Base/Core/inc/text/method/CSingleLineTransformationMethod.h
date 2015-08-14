
#ifndef __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHOD_H__
#define __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHOD_H__

#include "_CSingleLineTransformationMethod.h"
#include "text/method/ReplacementTransformationMethod.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CSingleLineTransformationMethod)
    ,public ReplacementTransformationMethod
{
public:
    CARAPI GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* view,
        /* [out] */ ICharSequence** csq);

    CARAPI OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    static CARAPI_(AutoPtr<ISingleLineTransformationMethod>) GetInstance();

protected:
    /**
     * The characters to be replaced are \n and \r.
     */
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetOriginal();

    /**
     * The character \n is replaced with is space;
     * the character \r is replaced with is FEFF (zero width space).
     */
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetReplacement();

private:
    static Char32 ORIGINAL[];// = { '\n', '\r' };
    static Char32 REPLACEMENT[];// = { ' ', '\uFEFF' };

    static AutoPtr<ISingleLineTransformationMethod> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHOD_H__
