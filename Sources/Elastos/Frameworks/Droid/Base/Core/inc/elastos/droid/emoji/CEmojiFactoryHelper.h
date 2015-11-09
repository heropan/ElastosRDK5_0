#ifndef __ELASTOS_DROID_EMOJI_CEMOJIFACTORYHELPER_H__
#define __ELASTOS_DROID_EMOJI_CEMOJIFACTORYHELPER_H__

#include "elastos/droid/emoji/EmojiFactoryHelper.h"

namespace Elastos {
namespace Droid {
namespace Emoji {

CarClass(CEmojiFactoryHelper)
    , public EmojiFactoryHelper
{
public:
    CAR_OBJECT_DECL();

    CEmojiFactoryHelper();
    virtual ~CEmojiFactoryHelper();
};

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_EMOJI_CEMOJIFACTORYHELPER_H__
