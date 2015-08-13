	// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const Float ViewConfigurationHelper::MIN_SCALING_SPAN_MM = 27.0f;
const Float ViewConfigurationHelper::MIN_SCALING_TOUCH_MAJOR_DIP = 48.0f;

//===============================================================
//      ViewConfigurationHelper::InnerComponentCallbacks
//===============================================================
ViewConfigurationHelper::InnerComponentCallbacks::InnerComponentCallbacks(
    /* in */ ViewConfigurationHelper* owner)
    : mOwner(owner)
{
}

//@Override
ECode ViewConfigurationHelper::InnerComponentCallbacks::OnConfigurationChanged(
    /* in */ IConfiguration* configuration)
{
	VALIDATE_NOT_NULL(configuration);
	mOwner->UpdateNativeViewConfigurationIfNecessary();
	return NOERROR;
}

//@Override
ECode ViewConfigurationHelper::InnerComponentCallbacks::OnLowMemory()
{
	return NOERROR;
}

//===============================================================
//           		ViewConfigurationHelper
//===============================================================
ViewConfigurationHelper::ViewConfigurationHelper(
    /* in */ IContext* context)
{
    context->GetApplicationContext((IContext**)&mAppContext);
    CViewConfiguration::Get(mAppContext, (IViewConfiguration**)&mViewConfiguration);
}

ECode ViewConfigurationHelper::RegisterListener()
{
	AutoPtr<InnerComponentCallbacks> componentCallBacks = new InnerComponentCallbacks(this);
    mAppContext->RegisterComponentCallbacks(componentCallBacks);
    return NOERROR;
}

ECode ViewConfigurationHelper::UpdateNativeViewConfigurationIfNecessary()
{
	AutoPtr<IViewConfiguration> configuration;
	CViewConfiguration::Get(mAppContext, (IViewConfiguration**)&configuration);

    if (mViewConfiguration == configuration)
    	return NOERROR;

    mViewConfiguration = configuration;
	NativeUpdateSharedViewConfiguration(
        GetScaledMaximumFlingVelocity(),
        GetScaledMinimumFlingVelocity(),
        GetScaledTouchSlop(),
        GetScaledDoubleTapSlop(),
        GetScaledMinScalingSpan(),
        GetScaledMinScalingTouchMajor());

    return NOERROR;
}

Int32 ViewConfigurationHelper::GetDoubleTapTimeout()
{
	Int32 doubleTapTimeout;
    CViewConfiguration::GetDoubleTapTimeout(&doubleTapTimeout);
    return doubleTapTimeout;
}

Int32 ViewConfigurationHelper::GetLongPressTimeout()
{
	Int32 longPressTimeout;
    CViewConfiguration::GetLongPressTimeout(&longPressTimeout);
    return longPressTimeout;
}

Int32 ViewConfigurationHelper::GetTapTimeout()
{
	Int32 tapTimeout;
    CViewConfiguration::GetTapTimeout(&tapTimeout);
    return tapTimeout;
}

Float ViewConfigurationHelper::GetScrollFriction()
{
	Float scrollFriction;
    CViewConfiguration::GetScrollFriction(&scrollFriction);
    return scrollFriction;
}

Int32 ViewConfigurationHelper::GetScaledMaximumFlingVelocity()
{
	Int32 scaledMaximumFlingVelocity;
    CViewConfiguration::GetScaledMaximumFlingVelocity(&scaledMaximumFlingVelocity);
    return scaledMaximumFlingVelocity;
}

Int32 ViewConfigurationHelper::GetScaledMinimumFlingVelocity()
{
	Int32 scaledMinimumFlingVelocity;
    CViewConfiguration::GetScaledMinimumFlingVelocity(&scaledMinimumFlingVelocity);
    return scaledMinimumFlingVelocity;
}

Int32 ViewConfigurationHelper::GetScaledTouchSlop()
{
	Int32 scaledTouchSlop;
    CViewConfiguration::GetScaledTouchSlop(&scaledTouchSlop);
    return scaledTouchSlop;
}

Int32 ViewConfigurationHelper::GetScaledDoubleTapSlop()
{
	Int32 scaledDoubleTapSlop;
    CViewConfiguration::GetScaledDoubleTapSlop(&scaledDoubleTapSlop);
    return scaledDoubleTapSlop;
}

Int32 ViewConfigurationHelper::GetScaledMinScalingSpan()
{
    AutoPtr<IResources> res;
    mAppContext->GetResources((IResources**)&res);
    Int32 id;
    res->GetIdentifier(String("config_minScalingSpan"), String("dimen"), String("android"), &id);
    if (id == 0) id = R::dimen.config_min_scaling_span;
    //try {
    	Int32 dimensionPixelSize;
        res->GetDimensionPixelSize(id, &dimensionPixelSize);
        return dimensionPixelSize;
    //}
    //catch (Resources.NotFoundException e) {
    //    assert false : "MinScalingSpan resource lookup failed.";
	//	return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_MM, MIN_SCALING_SPAN_MM,
    //                res.getDisplayMetrics());
    //}
}

Int32 ViewConfigurationHelper::GetScaledMinScalingTouchMajor()
{
	AutoPtr<IResources> res;
    mAppContext->GetResources((IResources**)&res);
    Int32 id;
    res->GetIdentifier(String("config_minScalingTouchMajor"), String("dimen"), String("android"), &id);
    if (id == 0) id = R::dimen::config_min_scaling_touch_major;
    //try {
    	Int32 dimensionPixelSize;
        res->GetDimensionPixelSize(id, &dimensionPixelSize);
        return dimensionPixelSize;
    //}
    //catch (Resources.NotFoundException e) {
    //    assert false : "MinScalingTouchMajor resource lookup failed.";
	//	return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
    //                MIN_SCALING_TOUCH_MAJOR_DIP, res.getDisplayMetrics());
    //}
}

AutoPtr<ViewConfigurationHelper> ViewConfigurationHelper::CreateWithListener(
    /* in */ IContext* context)
{
    AutoPtr<IViewConfigurationHelper> viewConfigurationHelper;
	CViewConfigurationHelper::AcquireSingleton(context, (IViewConfigurationHelper**)&viewConfigurationHelper);
    viewConfigurationHelper->RegisterListener();
    return viewConfigurationHelper;
}

ECode ViewConfigurationHelper::NativeUpdateSharedViewConfiguration(
    /* in */ Int32 scaledMaximumFlingVelocity,
	/* in */ Int32 scaledMinimumFlingVelocity,
	/* in */ Int32 scaledTouchSlop,
	/* in */ Int32 scaledDoubleTapSlop,
	/* in */ Int32 scaledMinScalingSpan,
	/* in */ Int32 scaledMinScalingTouchMajor)
{
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

