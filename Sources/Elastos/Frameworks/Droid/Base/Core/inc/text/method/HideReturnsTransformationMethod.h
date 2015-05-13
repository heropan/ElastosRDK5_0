#ifndef __HideReturnsTransformationMethod_H__
#define __HideReturnsTransformationMethod_H__

#include "text/method/ReplacementTransformationMethod.h"
#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * This transformation method causes any carriage return characters (\r)
 * to be hidden by displaying them as zero-width non-breaking space
 * characters (\uFEFF).
 */
//public class
class HideReturnsTransformationMethod : public ReplacementTransformationMethod
{
public:
    //static AutoPtr<IHideReturnsTransformationMethod> GetInstance();

protected:
    /**
     * The character to be replaced is \r.
     */
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetOriginal();

    /**
     * The character that \r is replaced with is \uFEFF.
     */
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetReplacement();

private:
    static Char32 ORIGINAL[];// = new char[] { '\r' };
    static Char32 REPLACEMENT[];// = new char[] { '\uFEFF' };
    //static AutoPtr<IHideReturnsTransformationMethod> sInstance;
};


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __HideReturnsTransformationMethod_H__
