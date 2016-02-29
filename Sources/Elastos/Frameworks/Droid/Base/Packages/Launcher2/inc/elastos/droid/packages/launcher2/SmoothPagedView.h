#ifndef  __ELASTOS_DROID_PACKAGES_LAUNCHER2_SMOOTHPAGEDVIEW_H__
#define  __ELASTOS_DROID_PACKAGES_LAUNCHER2_SMOOTHPAGEDVIEW_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

class SmoothPagedView
    : public PagedView
{
public:
    class OvershootInterpolator
        : public Object
        , public IInterpolator
        , public ITimeInterpolator
    {
    public:
        CAR_INTERFACE_DECL();

        OvershootInterpolator();

        CARAPI_(void) SetDistance(
            /* [in] */ Int32 distance);

        CARAPI_(void) DisableSettle();

        CARAPI_(Float) GetInterpolation(
            /* [in] */ Float t);

    private:
        static const Float DEFAULT_TENSION;
        Float mTension;
    };

public:
    /**
     * Used to inflate the Workspace from XML.
     *
     * @param context The application's context.
     * @param attrs The attributes set containing the Workspace's customization values.
     */
    SmoothPagedView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Used to inflate the Workspace from XML.
     *
     * @param context The application's context.
     * @param attrs The attributes set containing the Workspace's customization values.
     * @param defStyle Unused.
     */
    SmoothPagedView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    // @Override
    CARAPI ComputeScroll();

protected:
    CARAPI_(Int32) GetScrollMode();

    /**
     * Initializes various states for this workspace.
     */
    // @Override
    CARAPI Init();

    // @Override
    CARAPI SnapToDestination();

    // @Override
    CARAPI SnapToPageWithVelocity(
        /* [in] */ Int32 whichPage,
        /* [in] */ Int32 velocity);

    // @Override
    CARAPI SnapToPage(
        /* [in] */ Int32 whichPage);

private:
    CARAPI_(void) SnapToPageWithVelocity(
        /* [in] */ Int32 whichPage,
        /* [in] */ Int32 velocity,
        /* [in] */ Boolean settle);

public:
    static const Int32 DEFAULT_MODE;
    static const Int32 X_LARGE_MODE;

    Int32 mScrollMode;

private:
    static const Float SMOOTHING_SPEED;
    static const Float SMOOTHING_CONSTANT;

    Float mBaseLineFlingVelocity;
    Float mFlingVelocityInfluence;

    AutoPtr<IInterpolator> mScrollInterpolator;
};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_SMOOTHPAGEDVIEW_H__