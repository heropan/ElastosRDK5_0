
#ifndef __ELASTOS_DROID_APP_ACTIVITY_TRANSITION_COORDINATOR_H__
#define __ELASTOS_DROID_APP_ACTIVITY_TRANSITION_COORDINATOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/ResultReceiver.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::ResultReceiver;
using Elastos::Droid::Utility::IArrayMap;
using Elastos::Utility::List;
using Elastos::Utility::HashMap;
using Elastos::Utility::ICollection;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace App {

// import android.content.Context;
// import android.graphics.Matrix;
// import android.graphics.Rect;
// import android.graphics.RectF;
// import android.os.Bundle;
// import android.os.Handler;
// import android.os.Parcelable;
// import android.os.ResultReceiver;
// import android.transition.Transition;
// import android.transition.TransitionSet;
// import android.view.GhostView;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.ViewGroupOverlay;
// import android.view.ViewParent;
// import android.view.ViewTreeObserver;
// import android.view.Window;
// import android.widget.ImageView;

/**
 * Base class for ExitTransitionCoordinator and EnterTransitionCoordinator, classes
 * that manage activity transitions and the communications coordinating them between
 * Activities. The ExitTransitionCoordinator is created in the
 * ActivityOptions#makeSceneTransitionAnimation. The EnterTransitionCoordinator
 * is created by ActivityOptions#createEnterActivityTransition by Activity when the window is
 * attached.
 *
 * Typical startActivity goes like this:
 * 1) ExitTransitionCoordinator created with ActivityOptions#makeSceneTransitionAnimation
 * 2) Activity#startActivity called and that calls startExit() through
 *    ActivityOptions#dispatchStartExit
 *    - Exit transition starts by setting transitioning Views to INVISIBLE
 * 3) Launched Activity starts, creating an EnterTransitionCoordinator.
 *    - The Window is made translucent
 *    - The Window background alpha is set to 0
 *    - The transitioning views are made INVISIBLE
 *    - MSG_SET_LISTENER is sent back to the ExitTransitionCoordinator.
 * 4) The shared element transition completes.
 *    - MSG_TAKE_SHARED_ELEMENTS is sent to the EnterTransitionCoordinator
 * 5) The MSG_TAKE_SHARED_ELEMENTS is received by the EnterTransitionCoordinator.
 *    - Shared elements are made VISIBLE
 *    - Shared elements positions and size are set to match the end state of the calling
 *      Activity.
 *    - The shared element transition is started
 *    - If the window allows overlapping transitions, the views transition is started by setting
 *      the entering Views to VISIBLE and the background alpha is animated to opaque.
 *    - MSG_HIDE_SHARED_ELEMENTS is sent to the ExitTransitionCoordinator
 * 6) MSG_HIDE_SHARED_ELEMENTS is received by the ExitTransitionCoordinator
 *    - The shared elements are made INVISIBLE
 * 7) The exit transition completes in the calling Activity.
 *    - MSG_EXIT_TRANSITION_COMPLETE is sent to the EnterTransitionCoordinator.
 * 8) The MSG_EXIT_TRANSITION_COMPLETE is received by the EnterTransitionCoordinator.
 *    - If the window doesn't allow overlapping enter transitions, the enter transition is started
 *      by setting entering views to VISIBLE and the background is animated to opaque.
 * 9) The background opacity animation completes.
 *    - The window is made opaque
 * 10) The calling Activity gets an onStop() call
 *    - onActivityStopped() is called and all exited Views are made VISIBLE.
 *
 * Typical finishAfterTransition goes like this:
 * 1) finishAfterTransition() creates an ExitTransitionCoordinator and calls startExit()
 *    - The Window start transitioning to Translucent with a new ActivityOptions.
 *    - If no background exists, a black background is substituted
 *    - The shared elements in the scene are matched against those shared elements
 *      that were sent by comparing the names.
 *    - The exit transition is started by setting Views to INVISIBLE.
 * 2) The ActivityOptions is received by the Activity and an EnterTransitionCoordinator is created.
 *    - All transitioning views are made VISIBLE to reverse what was done when onActivityStopped()
 *      was called
 * 3) The Window is made translucent and a callback is received
 *    - The background alpha is animated to 0
 * 4) The background alpha animation completes
 * 5) The shared element transition completes
 *    - After both 4 & 5 complete, MSG_TAKE_SHARED_ELEMENTS is sent to the
 *      EnterTransitionCoordinator
 * 6) MSG_TAKE_SHARED_ELEMENTS is received by EnterTransitionCoordinator
 *    - Shared elements are made VISIBLE
 *    - Shared elements positions and size are set to match the end state of the calling
 *      Activity.
 *    - The shared element transition is started
 *    - If the window allows overlapping transitions, the views transition is started by setting
 *      the entering Views to VISIBLE.
 *    - MSG_HIDE_SHARED_ELEMENTS is sent to the ExitTransitionCoordinator
 * 7) MSG_HIDE_SHARED_ELEMENTS is received by the ExitTransitionCoordinator
 *    - The shared elements are made INVISIBLE
 * 8) The exit transition completes in the finishing Activity.
 *    - MSG_EXIT_TRANSITION_COMPLETE is sent to the EnterTransitionCoordinator.
 *    - finish() is called on the exiting Activity
 * 9) The MSG_EXIT_TRANSITION_COMPLETE is received by the EnterTransitionCoordinator.
 *    - If the window doesn't allow overlapping enter transitions, the enter transition is started
 *      by setting entering views to VISIBLE.
 */
class ActivityTransitionCoordinator
    : ResultReceiver
    , IActivityTransitionCoordinator
{
public:
    static class SharedElementOriginalState {
    public:
        int mLeft;
        int mTop;
        int mRight;
        int mBottom;
        int mMeasuredWidth;
        int mMeasuredHeight;
        ImageView.ScaleType mScaleType;
        Matrix mMatrix;
        float mTranslationZ;
        float mElevation;
    };

protected:
    class ContinueTransitionListener extends Transition.TransitionListenerAdapter {
        @Override
        void onTransitionStart(Transition transition) {
            mIsStartingTransition = false;
            Runnable pending = mPendingTransition;
            mPendingTransition = null;
            if (pending != null) {
                startTransition(pending);
            }
        }
    };

private:
     static class FixedEpicenterCallback extends Transition.EpicenterCallback {
        Rect mEpicenter;

        void setEpicenter(Rect epicenter) { mEpicenter = epicenter; }

        @Override
        Rect onGetEpicenter(Transition transition) {
            return mEpicenter;
        }
    };

    static class GhostViewListeners implements ViewTreeObserver.OnPreDrawListener {
        View mView;
        ViewGroup mDecor;
        View mParent;
        Matrix mMatrix = new Matrix();

        GhostViewListeners(View view, View parent, ViewGroup decor) {
            mView = view;
            mParent = parent;
            mDecor = decor;
        }

        View getView() {
            return mView;
        }

        @Override
        boolean onPreDraw() {
            GhostView ghostView = GhostView.getGhost(mView);
            if (ghostView == null) {
                mParent.getViewTreeObserver().removeOnPreDrawListener(this);
            } else {
                GhostView.calculateMatrix(mView, mDecor, mMatrix);
                ghostView.setMatrix(mMatrix);
            }
            return true;
        }
    };

public:
    CAR_INTERFACE_DECL()

    ActivityTransitionCoordinator();

    virtual ~ActivityTransitionCoordinator();

    CARAPI constructor(
        /* [in] */ IWindow* window,
        /* [in] */ IArrayList* allSharedElementNames, //ArrayList<String>
        /* [in] */ ISharedElementCallback* listener,
        /* [in] */ Boolean isReturning);

    CARAPI GetDecor(
        /* [out] */ IViewGroup** vg);

    CARAPI SetEpicenter(
        /* [in] */ IView* view);

    CARAPI GetAcceptedNames(
        /* [out] */ IArrayList** list); //ArrayList<String>

    CARAPI GetMappedNames(
        /* [out] */ IArrayList** list); //ArrayList<String>

    CARAPI CopyMappedViews(
        /* [out] */ IArrayList** list); //ArrayList<View>

    CARAPI GetAllSharedElementNames(
        /* [out] */ IArrayList** list); //ArrayList<String>

    static CARAPI_(Boolean) IsInTransitionGroup(
        /* [in] */ IViewParent* viewParent,
        /* [in] */ IViewGroup* decor);

protected:
    CARAPI ViewsReady(
        /* [in] */ IArrayMap* sharedElements); //ArrayMap<String, View>

    CARAPI StripOffscreenViews();

    CARAPI GetWindow(
        /* [out] */ IWindow** window);

    /**
     * Sets the transition epicenter to the position of the first shared element.
     */
    CARAPI SetEpicenter();

    CARAPI_(AutoPtr<ITransition>) SetTargets(
        /* [in] */ ITransition* transition,
        /* [in] */ Boolean add);

    CARAPI_(AutoPtr<ITransition>) ConfigureTransition(
        /* [in] */ ITransition* transition,
        /* [in] */ Boolean includeTransitioningViews);

    static CARAPI_(AutoPtr<ITransition>) MergeTransitions(
        /* [in] */ ITransition* transition1,
        /* [in] */ ITransition* transition2);

    CARAPI_(AutoPtr<IArrayMap>) MapSharedElements( //ArrayMap<String, View>
        /* [in] */ IArrayList* accepted, //ArrayList<String>
        /* [in] */ IArrayList* localViews); //ArrayList<View>

    CARAPI SetResultReceiver(
        /* [in] */ IResultReceiver* resultReceiver);

    virtual CARAPI_(AutoPtr<ITransition>) GetViewsTransition() = 0;

    CARAPI GetSharedElementParentMatrix(
        /* [in] */ IView* view,
        /* [in] */ IMatrix* matrix);

    CARAPI_(AutoPtr<List<SharedElementOriginalState> >) SetSharedElementState(
        /* [in] */ IBundle* sharedElementState,
        /* [in] */ IArrayList* snapshots); //ArrayList<View>

    CARAPI NotifySharedElementEnd(
        /* [in] */ IArrayList* snapshots); //ArrayList<View>

    CARAPI ScheduleSetSharedElementEnd(
        /* [in] */ IArrayList* snapshots); //ArrayList<View>

    CARAPI_(AutoPtr<IArrayList>) CreateSnapshots( //ArrayList<View>
        /* [in] */ IBundle* state,
        /* [in] */ ICollection* names); //Collection<String>

    static CARAPI SetOriginalSharedElementState(
        /* [in] */ IArrayList* sharedElements, //ArrayList<View>
        /* [in] */ List<SharedElementOriginalState>* originalState); //ArrayList<SharedElementOriginalState>

    CARAPI_(AutoPtr<IBundle>) CaptureSharedElementState();

    CARAPI ClearState();

    CARAPI_(Int64) GetFadeDuration();

    CARAPI HideViews(
        /* [in] */ IArrayList* views); //ArrayList<View>

    CARAPI ShowViews(
        /* [in] */ IArrayList* views, //ArrayList<View>
        /* [in] */ Boolean setTransitionAlpha);

    /**
     * Captures placement information for Views with a shared element name for
     * Activity Transitions.
     *
     * @param view           The View to capture the placement information for.
     * @param name           The shared element name in the target Activity to apply the placement
     *                       information for.
     * @param transitionArgs Bundle to store shared element placement information.
     * @param tempBounds     A temporary Rect for capturing the current location of views.
     */
    CARAPI CaptureSharedElementState(
        /* [in] */ IView* view,
        /* [in] */ const String& name,
        /* [in] */ IBundle* transitionArgs,
        /* [in] */ IMatrix* tempMatrix,
        /* [in] */ IRectF* tempBounds);

    CARAPI StartTransition(
        /* [in] */ IRunnable* runnable);

    CARAPI TransitionStarted();

    CARAPI MoveSharedElementsToOverlay();

    CARAPI_(Boolean) MoveSharedElementWithParent();

    CARAPI MoveSharedElementsFromOverlay();

    CARAPI SetGhostVisibility(
        /* [in] */ Int32 visibility);

    CARAPI ScheduleGhostVisibilityChange(
        /* [in] */ Int32 visibility);

private:

    CARAPI SetSharedElementState(
        /* [in] */ IView* view,
        /* [in] */ const String& name,
        /* [in] */ IBundle* transitionArgs,
        /* [in] */ IMatrix* tempMatrix,
        /* [in] */ IRectF* tempRect,
        /* [in] */ ArrayOf<Int32>* decorLoc);

    static AutoPtr<SharedElementOriginalState> GetOldSharedElementState(
        /* [in] */ IView* view,
        /* [in] */ const String& name,
        /* [in] */ IBundle* transitionArgs);

    CARAPI ShowView(
        /* [in] */ IView* view,
        /* [in] */ Boolean setTransitionAlpha);

    static Int32 ScaleTypeToInt(
        /* [in] */ ImageViewScaleType scaleType);

protected:
    static const String KEY_SCREEN_LEFT;// = "shared_element:screenLeft";
    static const String KEY_SCREEN_TOP;// = "shared_element:screenTop";
    static const String KEY_SCREEN_RIGHT;// = "shared_element:screenRight";
    static const String KEY_SCREEN_BOTTOM;//= "shared_element:screenBottom";
    static const String KEY_TRANSLATION_Z;// = "shared_element:translationZ";
    static const String KEY_SNAPSHOT;// = "shared_element:bitmap";
    static const String KEY_SCALE_TYPE;// = "shared_element:scaleType";
    static const String KEY_IMAGE_MATRIX;// = "shared_element:imageMatrix";
    static const String KEY_ELEVATION;// = "shared_element:elevation";

    static const AutoPtr<ArrayOf<ImageViewScaleType> > SCALE_TYPE_VALUES;// = ImageView.ScaleType.values();

    AutoPtr<IArrayList> mAllSharedElementNames; //ArrayList<String>
    AutoPtr<IArrayList> mSharedElements;// = new ArrayList<View>();
    AutoPtr<IArrayList> mSharedElementNames;// = new ArrayList<String>();
    AutoPtr<IArrayList> mTransitioningViews;// = new ArrayList<View>();

    AutoPtr<ISharedElementCallback> mListener;
    AutoPtr<IResultReceiver> mResultReceiver;

    Boolean mIsReturning;

private:
    static const String TAG;// = "ActivityTransitionCoordinator";

    /**
     * For Activity transitions, the called Activity's listener to receive calls
     * when transitions complete.
     */
    static const String KEY_REMOTE_RECEIVER;// = "android:remoteReceiver";

    AutoPtr<IWindow> mWindow;

    AutoPtr<IFixedEpicenterCallback> mEpicenterCallback;// = new FixedEpicenterCallback();
    AutoPtr<IRunnable> mPendingTransition;
    Boolean mIsStartingTransition;
    List<AutoPtr<GhostViewListeners> > mGhostViewListeners;
    HashMap<AutoPtr<IView>, Float> > mOriginalAlphas;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_ACTIVITY_TRANSITION_COORDINATOR_H__