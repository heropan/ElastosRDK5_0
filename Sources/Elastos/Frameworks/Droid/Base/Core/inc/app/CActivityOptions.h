
#ifndef __CACTIVITYOPTIONS_H__
#define __CACTIVITYOPTIONS_H__

#include "_CActivityOptions.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"

using Elastos::Droid::Os::Runnable;
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

class AnimationStartedListenerRunnable
    : public Runnable
{
public:
    AnimationStartedListenerRunnable(
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener);

    virtual CARAPI Run();

private:
    AutoPtr<IActivityOptionsOnAnimationStartedListener> mListener;
};

CarClass(CActivityOptions)
{
public:
    CActivityOptions();

    CARAPI constructor();

    /** @hide */
    CARAPI constructor(
        /* [in] */ IBundle* opts);

    CARAPI GetPackageName(
        /* [out] */ String* pkgName);

    CARAPI SetPackageName(
        /* [in] */ const String& pkgName);

    /** @hide */
    CARAPI GetAnimationType(
        /* [out] */ Int32* type);

    CARAPI SetAnimationType(
        /* [in] */ Int32 type);

    /** @hide */
    CARAPI GetCustomEnterResId(
        /* [out] */ Int32* id);

    CARAPI SetCustomEnterResId(
        /* [in] */ Int32 id);

    /** @hide */
    CARAPI GetCustomExitResId(
        /* [out] */ Int32* id);

    CARAPI SetCustomExitResId(
        /* [in] */ Int32 id);

    /** @hide */
    CARAPI GetThumbnail(
        /* [out] */ IBitmap** thumbnail);

    CARAPI SetThumbnail(
        /* [in] */ IBitmap* thumbnail);

    /** @hide */
    CARAPI GetStartX(
        /* [out] */ Int32* x);

    CARAPI SetStartX(
        /* [in] */ Int32 x);

    /** @hide */
    CARAPI GetStartY(
        /* [out] */ Int32* y);

    CARAPI SetStartY(
        /* [in] */ Int32 y);

    /** @hide */
    CARAPI GetStartWidth(
        /* [out] */ Int32* width);

    CARAPI SetStartWidth(
        /* [in] */ Int32 width);

    /** @hide */
    CARAPI GetStartHeight(
        /* [out] */ Int32* height);

    CARAPI SetStartHeight(
        /* [in] */ Int32 height);

    /** @hide */
    CARAPI GetOnAnimationStartListener(
        /* [out] */ IRemoteCallback** cb);

    CARAPI SetOnAnimationStartListener(
        /* [in] */ IRemoteCallback* cb);

    /** @hide */
    CARAPI Abort();

    /**
     * Update the current values in this ActivityOptions from those supplied
     * in <var>otherOptions</var>.  Any values
     * defined in <var>otherOptions</var> replace those in the base options.
     */
    CARAPI Update(
        /* [in] */ IActivityOptions* otherOptions);

    /**
     * Returns the created options as a Bundle, which can be passed to
     * {@link android.content.Context#startActivity(android.content.Intent, android.os.Bundle)
     * Context.startActivity(Intent, Bundle)} and related methods.
     * Note that the returned Bundle is still owned by the ActivityOptions
     * object; you must not modify it, but can supply it to the startActivity
     * methods that take an options Bundle.
     */
    CARAPI ToBundle(
        /* [out] */ IBundle** bundle);

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
    static CARAPI MakeCustomAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 enterResId,
        /* [in] */ Int32 exitResId,
        /* [out] */ IActivityOptions** options);

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
    static CARAPI MakeCustomAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 enterResId,
        /* [in] */ Int32 exitResId,
        /* [in] */ IHandler* handler,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** options);

    /**
     * Create an ActivityOptions specifying an animation where the new
     * activity is scaled from a small originating area of the screen to
     * its final full representation.
     *
     * <p>If the Intent this is being used with has not set its
     * android.content.Intent#setSourceBounds Intent.setSourceBounds,
     * those bounds will be filled in for you based on the initial
     * bounds passed in here.
     *
     * @sa android.content.Intent#setSourceBounds Intent.setSourceBounds
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
    static CARAPI MakeScaleUpAnimation(
        /* [in] */ IView* source,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 startWidth,
        /* [in] */ Int32 startHeight,
        /* [out] */ IActivityOptions** options);

    /**
     * Create an ActivityOptions specifying an animation where a thumbnail
     * is scaled from a given position to the new activity window that is
     * being started.
     *
     * <p>If the Intent this is being used with has not set its
     * android.content.Intent#setSourceBounds Intent.setSourceBounds,
     * those bounds will be filled in for you based on the initial
     * thumbnail location and size provided here.
     *
     * @sa android.content.Intent#setSourceBounds Intent.setSourceBounds
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
    static CARAPI MakeThumbnailScaleUpAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [out] */ IActivityOptions** options);

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
    static CARAPI MakeThumbnailScaleUpAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** options);

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
    static CARAPI MakeThumbnailScaleDownAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [out] */ IActivityOptions** options);

    /** @hide */
    static CARAPI Abort(
        /* [in] */ IBundle* options);

    CARAPI SetListener(
        /* [in] */ IHandler* handler,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener);
private:
    static CARAPI MakeThumbnailAnimation(
        /* [in] */ IView* source,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
        /* [in] */ Boolean scaleUp,
        /* [out] */ IActivityOptions** options);

private:
    String mPackageName;
    Int32 mAnimationType;
    Int32 mCustomEnterResId;
    Int32 mCustomExitResId;
    AutoPtr<IBitmap> mThumbnail;
    Int32 mStartX;
    Int32 mStartY;
    Int32 mStartWidth;
    Int32 mStartHeight;
    AutoPtr<IRemoteCallback> mAnimationStartedListener;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYOPTIONS_H__
