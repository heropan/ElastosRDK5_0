
#ifndef __HARDWARERENDERER_H__
#define __HARDWARERENDERER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

class HardwareRenderer : public ElRefBase
{
public:
    /**
     * Interface used to receive callbacks whenever a view is drawn by
     * a hardware renderer instance.
     */
    interface HardwareDrawCallbacks
    {
    public:
        /**
         * Invoked before a view is drawn by a hardware renderer.
         *
         * @param canvas The Canvas used to render the view.
         */
        virtual CARAPI_(void) OnHardwarePreDraw(
            /* [in] */ IHardwareCanvas* canvas) = 0;

        /**
         * Invoked after a view is drawn by a hardware renderer.
         *
         * @param canvas The Canvas used to render the view.
         */
        virtual CARAPI_(void) OnHardwarePostDraw(
            /* [in] */ IHardwareCanvas* canvas) = 0;
    };
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__HARDWARERENDERER_H__
