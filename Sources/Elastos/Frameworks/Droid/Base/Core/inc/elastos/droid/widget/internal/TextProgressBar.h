
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_TEXTPROGRESSBAR_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_TEXTPROGRESSBAR_H__

#include "elastos/droid/widget/RelativeLayout.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

/**
 * Container that links together a {@link ProgressBar} and {@link Chronometer}
 * as children. It subscribes to {@link Chronometer#OnChronometerTickListener}
 * and updates the {@link ProgressBar} based on a preset finishing time.
 * <p>
 * This widget expects to contain two children with specific ids
 * {@link android.R.id.progress} and {@link android.R.id.text1}.
 * <p>
 * If the {@link Chronometer} {@link android.R.attr#layout_width} is
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}, then the
 * {@link android.R.attr#gravity} will be used to automatically move it with
 * respect to the {@link ProgressBar} position. For example, if
 * {@link android.view.Gravity#LEFT} then the {@link Chronometer} will be placed
 * just ahead of the leading edge of the {@link ProgressBar} position.
 */

class TextProgressBar : public RelativeLayout
{
public:

    TextProgressBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    TextProgressBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    TextProgressBar(
        /* [in] */ IContext* context);

    /**
     * Catch any interesting children when they are added.
     */
    using RelativeLayout::AddView;

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    /**
     * Set the expected termination time of the running {@link Chronometer}.
     * This value is used to adjust the {@link ProgressBar} against the elapsed
     * time.
     * <p>
     * Call this <b>after</b> adjusting the {@link Chronometer} base, if
     * necessary.
     *
     * @param durationBase Use the {@link SystemClock#elapsedRealtime} time
     *            base.
     */
    virtual CARAPI SetDurationBase(
        /* [in] */ Int64 durationBase);


    /**
     * Callback when {@link Chronometer} changes, indicating that we should
     * update the {@link ProgressBar} and change the layout if necessary.
     */
    virtual CARAPI OnChronometerTick(
        /* [in] */ IChronometer* chronometer);

protected:
    TextProgressBar();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context);

    static const String TAG;

    static const Int32 CHRONOMETER_ID = R::id::text1;
    static const Int32 PROGRESSBAR_ID = R::id::progress;

    AutoPtr<IChronometer> mChronometer;
    AutoPtr<IProgressBar> mProgressBar;

    Int64 mDurationBase;
    Int32 mDuration;

    Boolean mChronometerFollow;
    Int32 mChronometerGravity;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
