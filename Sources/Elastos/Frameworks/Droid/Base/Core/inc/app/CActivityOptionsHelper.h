
#ifndef __CACTIVITYOPTIONSHELPER_H__
#define __CACTIVITYOPTIONSHELPER_H__

#include "_CActivityOptionsHelper.h"

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IRemoteCallback;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityOptionsHelper)
{
public:
    /**
     * Create an ActivityOptions specifying a custom animation to run when
     * the activity is displayed.
     *
     * @param context Who is defining this.  This is the application that the
     * animation resources will be loaded from.
     * @param enterResId A resource ID of the animation resource to use for
     * the incoming activity.  Use 0 for no animation.
     * @param exitResId A resource ID of the animation resource to use for
     * the outgoing activity.  Use 0 for no animation.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     */
    CARAPI MakeCustomAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 enterResId,
        /* [in] */ Int32 exitResId,
        /* [out] */ IActivityOptions** activityoptions);

    /**
     * Create an ActivityOptions specifying a custom animation to run when
     * the activity is displayed.
     *
     * @param context Who is defining this.  This is the application that the
     * animation resources will be loaded from.
     * @param enterResId A resource ID of the animation resource to use for
     * the incoming activity.  Use 0 for no animation.
     * @param exitResId A resource ID of the animation resource to use for
     * the outgoing activity.  Use 0 for no animation.
     * @param handler If <var>listener</var> is non-null this must be a valid
     * Handler on which to dispatch the callback; otherwise it should be null.
     * @param listener Optional OnAnimationStartedListener to find out when the
     * requested animation has started running.  If for some reason the animation
     * is not executed, the callback will happen immediately.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     * @hide
     */
    CARAPI  MakeCustomAnimationEx(
        /* [in] */ IContext* context,
        /* [in] */ Int32 enterResId,
        /* [in] */ Int32 exitResId,
        /* [in] */ IHandler* handler,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** ActivityOptions);

    /**
     * Create an ActivityOptions specifying an animation where the new
     * activity is scaled from a small originating area of the screen to
     * its final full representation.
     *
     * <p>If the Intent this is being used with has not set its
     * {@link android.content.Intent#setSourceBounds Intent.setSourceBounds},
     * those bounds will be filled in for you based on the initial
     * bounds passed in here.
     *
     * @param source The View that the new activity is animating from.  This
     * defines the coordinate space for <var>startX</var> and <var>startY</var>.
     * @param startX The x starting location of the new activity, relative to <var>source</var>.
     * @param startY The y starting location of the activity, relative to <var>source</var>.
     * @param startWidth The initial width of the new activity.
     * @param startHeight The initial height of the new activity.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     */
    CARAPI MakeScaleUpAnimation(
        /* [in] */ IView* source,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 startWidth,
        /* [in] */ Int32 startHeight,
        /* [out] */ IActivityOptions** activityoptions);

    /**
     * Create an ActivityOptions specifying an animation where a thumbnail
     * is scaled from a given position to the new activity window that is
     * being started.
     *
     * <p>If the Intent this is being used with has not set its
     * {@link android.content.Intent#setSourceBounds Intent.setSourceBounds},
     * those bounds will be filled in for you based on the initial
     * thumbnail location and size provided here.
     *
     * @param source The View that this thumbnail is animating from.  This
     * defines the coordinate space for <var>startX</var> and <var>startY</var>.
     * @param thumbnail The bitmap that will be shown as the initial thumbnail
     * of the animation.
     * @param startX The x starting location of the bitmap, relative to <var>source</var>.
     * @param startY The y starting location of the bitmap, relative to <var>source</var>.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     */
    CARAPI MakeThumbnailScaleUpAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [out] */ IActivityOptions** activityoptions);

    /**
     * Create an ActivityOptions specifying an animation where a thumbnail
     * is scaled from a given position to the new activity window that is
     * being started.
     *
     * @param source The View that this thumbnail is animating from.  This
     * defines the coordinate space for <var>startX</var> and <var>startY</var>.
     * @param thumbnail The bitmap that will be shown as the initial thumbnail
     * of the animation.
     * @param startX The x starting location of the bitmap, relative to <var>source</var>.
     * @param startY The y starting location of the bitmap, relative to <var>source</var>.
     * @param listener Optional OnAnimationStartedListener to find out when the
     * requested animation has started running.  If for some reason the animation
     * is not executed, the callback will happen immediately.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     * @hide
     */
    CARAPI MakeThumbnailScaleUpAnimationEx(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** activityoptions);

    /**
     * Create an ActivityOptions specifying an animation where an activity window
     * is scaled from a given position to a thumbnail at a specified location.
     *
     * @param source The View that this thumbnail is animating to.  This
     * defines the coordinate space for <var>startX</var> and <var>startY</var>.
     * @param thumbnail The bitmap that will be shown as the final thumbnail
     * of the animation.
     * @param startX The x end location of the bitmap, relative to <var>source</var>.
     * @param startY The y end location of the bitmap, relative to <var>source</var>.
     * @param listener Optional OnAnimationStartedListener to find out when the
     * requested animation has started running.  If for some reason the animation
     * is not executed, the callback will happen immediately.
     * @return Returns a new ActivityOptions object that you can use to
     * supply these options as the options Bundle when starting an activity.
     * @hide
     */
    CARAPI MakeThumbnailScaleDownAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** activityoptions);

    /** @hide */
    CARAPI Abort(
        /* [in] */ IBundle* options);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYOPTIONSHELPER_H__
