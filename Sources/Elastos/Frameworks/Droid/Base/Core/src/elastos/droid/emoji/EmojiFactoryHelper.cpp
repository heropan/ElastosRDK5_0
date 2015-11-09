#include "elastos/droid/emoji/EmojiFactoryHelper.h"

namespace Elastos {
namespace Droid {
namespace Emoji {

CAR_INTERFACE_IMPL(EmojiFactoryHelper, Object, IEmojiFactoryHelper);

EmojiFactoryHelper::EmojiFactoryHelper()
{}

EmojiFactoryHelper::~EmojiFactoryHelper()
{}

ECode EmojiFactoryHelper::NewInstance(
    /* [in] */ const String& class_name
    /* [out] */ IEmojiFactory** emojiFactory)
{
    return CEmojiFactory::NewInstance(class_name, emojiFactory);
}

ECode EmojiFactoryHelper::NewAvailableInstance(
    /* [out] */ IEmojiFactory** emojiFactory)
{
    return CEmojiFactory::NewAvailableInstance(emojiFactory);
}

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos
