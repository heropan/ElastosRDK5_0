
#ifndef __ELASTOS_TEXT_CCHOICEFORMAT_H__
#define __ELASTOS_TEXT_CCHOICEFORMAT_H__

#include "_Elastos_Text_CChoiceFormat.h"
#include "ChoiceFormat.h"

using Elastos::Core::IStringBuffer;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;

namespace Elastos {
namespace Text {

CarClass(CChoiceFormat)
    , public ChoiceFormat
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CCHOICEFORMAT_H__
