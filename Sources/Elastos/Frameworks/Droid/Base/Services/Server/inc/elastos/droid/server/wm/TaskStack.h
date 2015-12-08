#ifndef __ELASTOS_DROID_SERVER_WM_TASKSTACK_H__
#define __ELASTOS_DROID_SERVER_WM_TASKSTACK_H__

#include "wm/CWindowManagerService.h"
#include "wm/DimLayer.h"
#include "wm/WindowStateAnimator.h"
#include "wm/AppWindowToken.h"

using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class Task;

class TaskStack : public Object
{
public:
    TaskStack(
        /* [in] */ CWindowManagerService* service,
        /* [in] */ Int32 stackId);

    CARAPI_(AutoPtr<DisplayContent>) GetDisplayContent();

    CARAPI_(AutoPtr< List<AutoPtr<Task> > >) GetTasks();

    CARAPI_(void) ResizeWindows();

    CARAPI_(Boolean) SetBounds(
        /* [in] */ IRect* bounds);

    CARAPI_(void) GetBounds(
        /* [in] */ IRect* out);

    CARAPI_(void) UpdateDisplayInfo();

    CARAPI_(Boolean) IsFullscreen();

    CARAPI_(Boolean) IsAnimating();

    /**
     * Put a Task in this stack. Used for adding and moving.
     * @param task The task to add.
     * @param toTop Whether to add it to the top or bottom.
     */
    CARAPI_(void) AddTask(
        /* [in] */ Task* task,
        /* [in] */ Boolean toTop);

    CARAPI_(void) MoveTaskToTop(
        /* [in] */ Task* task);

    CARAPI_(void) MoveTaskToBottom(
        /* [in] */ Task* task);

    /**
     * Delete a Task from this stack. If it is the last Task in the stack, move this stack to the
     * back.
     * @param task The Task to delete.
     */
    CARAPI_(void) RemoveTask(
        /* [in] */ Task* task);

    CARAPI AttachDisplayContent(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) DetachDisplay();

    CARAPI_(void) ResetAnimationBackgroundAnimator();

    CARAPI_(Boolean) AnimateDimLayers();

    CARAPI_(void) ResetDimmingTag();

    CARAPI_(void) SetDimmingTag();

    CARAPI_(Boolean) TestDimmingTag();

    CARAPI_(Boolean) IsDimming();

    CARAPI_(Boolean) IsDimming(
        /* [in] */ WindowStateAnimator* winAnimator);

    CARAPI_(void) StartDimmingIfNeeded(
        /* [in] */ WindowStateAnimator* newWinAnimator);

    CARAPI_(void) StopDimmingIfNeeded();

    CARAPI_(void) SetAnimationBackground(
        /* [in] */ WindowStateAnimator* winAnimator,
        /* [in] */ Int32 color);

    CARAPI_(void) SwitchUser(
        /* [in] */ Int32 userId);

    CARAPI_(void) Close();

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CARAPI_(Int64) GetDimBehindFadeDuration(
        /* [in] */ Int64 duration);

public:
    /** Unique identifier */
    Int32 mStackId;

    /** The particular window with FLAG_DIM_BEHIND set. If null, hide mDimLayer. */
    AutoPtr<WindowStateAnimator> mDimWinAnimator;

    /** Support for non-zero {@link android.view.animation.Animation#getBackgroundColor()} */
    AutoPtr<DimLayer> mAnimationBackgroundSurface;

    /** The particular window with an Animation with non-zero background color. */
    AutoPtr<WindowStateAnimator> mAnimationBackgroundAnimator;

    /** Set to false at the start of performLayoutAndPlaceSurfaces. If it is still false by the end
     * then stop any dimming. */
    Boolean mDimmingTag;

    /** Application tokens that are exiting, but still on screen for animations. */
    AppTokenList mExitingAppTokens;

    /** Detach this stack from its display when animation completes. */
    Boolean mDeferDetach;

private:
    /** Amount of time in milliseconds to animate the dim surface from one value to another,
     * when no window animation is driving it. */
    static const Int32 DEFAULT_DIM_DURATION = 200;

    /** The service */
    AutoPtr<CWindowManagerService> mService;

    /** The display this stack sits under. */
    AutoPtr<DisplayContent> mDisplayContent;

    /** The Tasks that define this stack. Oldest Tasks are at the bottom. The ordering must match
     * mTaskHistory in the ActivityStack with the same mStackId */
    List<AutoPtr<Task> > mTasks;

    /** For comparison with DisplayContent bounds. */
    AutoPtr<IRect> mTmpRect;

    /** Content limits relative to the DisplayContent this sits in. */
    AutoPtr<IRect> mBounds;

    /** Whether mBounds is fullscreen */
    Boolean mFullscreen;

    /** Used to support {@link android.view.WindowManager.LayoutParams#FLAG_DIM_BEHIND} */
    AutoPtr<DimLayer> mDimLayer;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_TASKSTACK_H__
