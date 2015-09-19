
#ifndef __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKENIZERHELPER_H__
#define __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKENIZERHELPER_H__

#include "_Elastos_Droid_Text_Utility_CRfc822TokenizerHelper.h"

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

    CARAPI Tokenize(
        /* [in] */ ICharSequence* text,
        /* [out, callee] */ ArrayOf<IRfc822Token*>** ret);
};

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_TEXT_UTILITY_CRFC822TOKENIZERHELPER_H__
