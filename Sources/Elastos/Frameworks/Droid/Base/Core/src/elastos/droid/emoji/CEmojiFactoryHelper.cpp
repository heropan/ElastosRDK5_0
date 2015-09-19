#include "CEmojiFactoryHelper.h"

namespace Elastos {
namespace Droid {
namespace Emoji {


ECode CEmojiFactoryHelper::NewInstance
    /* [in] */ const String& class_name
    /* [out] */ IEmojiFactory** emojiFactory)
{
    return CEmojiFactory::NewInstance(class_name, emojiFactory);
}

ECode CEmojiFactoryHelper::NewAvailableInstance(
    /* [out] */ IEmojiFactory** emojiFactory)
{
    return CEmojiFactory::NewAvailableInstance(emojiFactory);
}

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos
