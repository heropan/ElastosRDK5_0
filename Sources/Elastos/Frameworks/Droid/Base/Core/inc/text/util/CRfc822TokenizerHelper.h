
#ifndef __CRFC822TOKENIZERHELPER_H__
#define __CRFC822TOKENIZERHELPER_H__

#include "_CRfc822TokenizerHelper.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Text::Utility::IRfc822Token;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

CarClass(CRfc822TokenizerHelper)
{
public:
    CARAPI Tokenize(
        /* [in] */ ICharSequence* text,
        /* [in, out] */ IObjectContainer* ret);

    CARAPI TokenizeEx(
        /* [in] */ ICharSequence* text,
        /* [out, callee] */ ArrayOf<IRfc822Token*>** ret);
};

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __CRFC822TOKENIZERHELPER_H__
