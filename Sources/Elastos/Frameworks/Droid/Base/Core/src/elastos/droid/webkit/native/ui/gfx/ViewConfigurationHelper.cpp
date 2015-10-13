
#include "elastos/droid/webkit/native/ui/gfx/ViewConfigurationHelper.h"
//#include "elastos/droid/view/CViewConfigurationHelper.h"

using Elastos::Droid::View::IViewConfigurationHelper;
//using Elastos::Droid::View::CViewConfigurationHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

//=====================================================================
//           ViewConfigurationHelper::InnerComponentCallbacks
//=====================================================================
ViewConfigurationHelper::InnerComponentCallbacks::InnerComponentCallbacks(
    /* [in] */ ViewConfigurationHelper* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ViewConfigurationHelper::InnerComponentCallbacks::OnConfigurationChanged(
    /* [in] */ IConfiguration* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    // ==================before translated======================
    // updateNativeViewConfigurationIfNecessary();

    assert(NULL == mOwner);
    mOwner->UpdateNativeViewConfigurationIfNecessary();
    return NOERROR;
}

ECode ViewConfigurationHelper::InnerComponentCallbacks::OnLowMemory()
{
    assert(0);
    return NOERROR;
}

//=====================================================================
//                       ViewConfigurationHelper
//=====================================================================
const Float ViewConfigurationHelper::MIN_SCALING_SPAN_MM = 27.0f;
const Float ViewConfigurationHelper::MIN_SCALING_TOUCH_MAJOR_DIP = 48.0f;

ViewConfigurationHelper::ViewConfigurationHelper(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mAppContext = context.getApplicationContext();
    // mViewConfiguration = ViewConfiguration.get(mAppContext);

    assert(0);
    context->GetApplicationContext((IContext**)&mAppContext);

    AutoPtr<IViewConfigurationHelper> helper;
    //CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    helper->Get(mAppContext, (IViewConfiguration**)&mViewConfiguration);
}

ECode ViewConfigurationHelper::RegisterListener()
{
    // ==================before translated======================
    // mAppContext.registerComponentCallbacks(
    //         new ComponentCallbacks() {
    //             @Override
    //             public void onConfigurationChanged(Configuration configuration) {
    //                 updateNativeViewConfigurationIfNecessary();
    //             }
    //
    //             @Override
    //             public void onLowMemory() {
    //             }
    //         });
    assert(0);
    return NOERROR;
}

ECode ViewConfigurationHelper::UpdateNativeViewConfigurationIfNecessary()
{
    // ==================before translated======================
    // // The ViewConfiguration will differ only if the density has changed.
    // ViewConfiguration configuration = ViewConfiguration.get(mAppContext);
    // if (mViewConfiguration == configuration) return;
    //
    // mViewConfiguration = configuration;
    // nativeUpdateSharedViewConfiguration(
    //         getScaledMaximumFlingVelocity(),
    //         getScaledMinimumFlingVelocity(),
    //         getScaledTouchSlop(),
    //         getScaledDoubleTapSlop(),
    //         getScaledMinScalingSpan(),
    //         getScaledMinScalingTouchMajor());
    assert(0);
    return NOERROR;
}

Int32 ViewConfigurationHelper::GetDoubleTapTimeout()
{
    // ==================before translated======================
    // return ViewConfiguration.getDoubleTapTimeout();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetLongPressTimeout()
{
    // ==================before translated======================
    // return ViewConfiguration.getLongPressTimeout();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetTapTimeout()
{
    // ==================before translated======================
    // return ViewConfiguration.getTapTimeout();
    assert(0);
    return 0;
}

Float ViewConfigurationHelper::GetScrollFriction()
{
    // ==================before translated======================
    // return ViewConfiguration.getScrollFriction();
    assert(0);
    return 0.0f;
}

Int32 ViewConfigurationHelper::GetScaledMaximumFlingVelocity()
{
    // ==================before translated======================
    // return mViewConfiguration.getScaledMaximumFlingVelocity();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetScaledMinimumFlingVelocity()
{
    // ==================before translated======================
    // return mViewConfiguration.getScaledMinimumFlingVelocity();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetScaledTouchSlop()
{
    // ==================before translated======================
    // return mViewConfiguration.getScaledTouchSlop();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetScaledDoubleTapSlop()
{
    // ==================before translated======================
    // return mViewConfiguration.getScaledDoubleTapSlop();
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetScaledMinScalingSpan()
{
    // ==================before translated======================
    // final Resources res = mAppContext.getResources();
    // int id = res.getIdentifier("config_minScalingSpan", "dimen", "android");
    // // Fall back to a sensible default if the internal identifier does not exist.
    // if (id == 0) id = R.dimen.config_min_scaling_span;
    // try {
    //     return res.getDimensionPixelSize(id);
    // } catch (Resources.NotFoundException e) {
    //     assert false : "MinScalingSpan resource lookup failed.";
    //     return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_MM, MIN_SCALING_SPAN_MM,
    //             res.getDisplayMetrics());
    // }
    assert(0);
    return 0;
}

Int32 ViewConfigurationHelper::GetScaledMinScalingTouchMajor()
{
    // ==================before translated======================
    // final Resources res = mAppContext.getResources();
    // int id = res.getIdentifier("config_minScalingTouchMajor", "dimen", "android");
    // // Fall back to a sensible default if the internal identifier does not exist.
    // if (id == 0) id = R.dimen.config_min_scaling_touch_major;
    // try {
    //     return res.getDimensionPixelSize(id);
    // } catch (Resources.NotFoundException e) {
    //     assert false : "MinScalingTouchMajor resource lookup failed.";
    //     return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
    //             MIN_SCALING_TOUCH_MAJOR_DIP, res.getDisplayMetrics());
    // }
    assert(0);
    return 0;
}

AutoPtr<ViewConfigurationHelper> ViewConfigurationHelper::CreateWithListener(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // ViewConfigurationHelper viewConfigurationHelper = new ViewConfigurationHelper(context);
    // viewConfigurationHelper.registerListener();
    // return viewConfigurationHelper;
    assert(0);
    AutoPtr<ViewConfigurationHelper> empty;
    return empty;
}

ECode ViewConfigurationHelper::NativeUpdateSharedViewConfiguration(
    /* [in] */ Int32 scaledMaximumFlingVelocity,
    /* [in] */ Int32 scaledMinimumFlingVelocity,
    /* [in] */ Int32 scaledTouchSlop,
    /* [in] */ Int32 scaledDoubleTapSlop,
    /* [in] */ Int32 scaledMinScalingSpan,
    /* [in] */ Int32 scaledMinScalingTouchMajor)
{
    assert(0);
    return NOERROR;
}

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


