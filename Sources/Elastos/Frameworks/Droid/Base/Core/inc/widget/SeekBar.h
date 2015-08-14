#ifndef __ELASTOS_DROID_WIDGET_SEEKBAR_H__
#define __ELASTOS_DROID_WIDGET_SEEKBAR_H__

#include "widget/AbsSeekBar.h"

namespace Elastos {
namespace Droid {
namespace Widget {

class SeekBar : public AbsSeekBar
{
public:
    SeekBar(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::seekBarStyle);

    /**
     * Sets a listener to receive notifications of changes to the SeekBar's progress level. Also
     * provides notifications of when the user starts and stops a touch gesture within the SeekBar.
     *
     * @param l The seek bar notification listener
     *
     * @see SeekBar.OnSeekBarChangeListener
     */
    CARAPI SetOnSeekBarChangeListener(
        /* [in] */ ISeekBarOnSeekBarChangeListener* l);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    SeekBar();

    Init(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::seekBarStyle);

    //java package access permission
    virtual CARAPI_(void) OnStartTrackingTouch();

    virtual CARAPI_(void) OnStopTrackingTouch();

    virtual CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

private:
    AutoPtr<ISeekBarOnSeekBarChangeListener> mOnSeekBarChangeListener;
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SEEKBAR_H__
