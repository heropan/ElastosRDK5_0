
#ifndef __SELECTPOPUP_H__
#define __SELECTPOPUP_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Handles the popup UI for the <select> HTML tag support.
 */
class SelectPopup
{
public:
    /**
     * Shows the popup.
     */
    virtual CARAPI_(void) Show() = 0;

    /**
     * Hides the popup.
     */
    virtual CARAPI_(void) Hide() = 0;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__SELECTPOPUP_H__
