#ifndef __EMOJIFACTORYHELPER_H__
#define __EMOJIFACTORYHELPER_H__


#include "graphics/CBitmap.h"
#include "CEmojiFactory.h"

namespace Elastos {
namespace Droid {
namespace Emoji {

CarClass (CEmojiFactoryHelper)
{
public:
    /**
     * Constructs an instance of EmojiFactory corresponding to the name.
     *
     * @param class_name Name of the factory. This must include complete package name.
     * @return A concrete EmojiFactory instance corresponding to factory_name.
     * If factory_name is invalid, null is returned.
     */
    CARAPI NewInstance(
        /* [in] */ const String& class_name
        /* [out] */ IEmojiFactory** EmojiFactory);


    /**
     * Constructs an instance of available EmojiFactory.
     *
     * @return A concrete EmojiFactory instance. If there are several available
     * EmojiFactory class, preferred one is chosen by the system. If there isn't, null
     * is returned.
     */
    CARAPI NewAvailableInstance(
        /* [out] */ IEmojiFactory** EmojiFactory);
};

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos

#endif //__EMOJIFACTORYHELPER_H__
