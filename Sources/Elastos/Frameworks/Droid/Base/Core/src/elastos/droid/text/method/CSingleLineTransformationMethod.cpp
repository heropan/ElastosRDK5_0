
#include "elastos/droid/text/method/CSingleLineTransformationMethod.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

Char32 CSingleLineTransformationMethod::ORIGINAL[] = { '\n', '\r' };
Char32 CSingleLineTransformationMethod::REPLACEMENT[] = { ' ', 0xFEFF/*'\uFEFF'*/ };

AutoPtr<ISingleLineTransformationMethod> CSingleLineTransformationMethod::sInstance;

ECode CSingleLineTransformationMethod::GetTransformation(
    /* [in] */ ICharSequence* source,
    /* [in] */ IView* view,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cs = ReplacementTransformationMethod::GetTransformation(source, view);
    *ret = cs;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CSingleLineTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    ReplacementTransformationMethod::OnFocusChanged(view, sourceText, focused, direction, previouslyFocusedRect);
    return NOERROR;
}

AutoPtr<ISingleLineTransformationMethod> CSingleLineTransformationMethod::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CSingleLineTransformationMethod> stm;
        CSingleLineTransformationMethod::NewByFriend((CSingleLineTransformationMethod**)&stm);
        sInstance = (ISingleLineTransformationMethod*)(stm.Get());
    }
    return sInstance;
}

AutoPtr< ArrayOf<Char32> > CSingleLineTransformationMethod::GetOriginal()
{
    Int32 arrayLen = 2;
    AutoPtr< ArrayOf<Char32> > ret = ArrayOf<Char32>::Alloc(arrayLen);
    for(Int32 i=0; i<arrayLen; i++)
    {
        (*ret)[i] = ORIGINAL[i];
    }
    return ret;
}

AutoPtr< ArrayOf<Char32> > CSingleLineTransformationMethod::GetReplacement()
{
    Int32 arrayLen = 2;
    AutoPtr< ArrayOf<Char32> > ret = ArrayOf<Char32>::Alloc(arrayLen);
    for(Int32 i=0; i<arrayLen; i++)
    {
        (*ret)[i] = REPLACEMENT[i];
    }
    return ret;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

