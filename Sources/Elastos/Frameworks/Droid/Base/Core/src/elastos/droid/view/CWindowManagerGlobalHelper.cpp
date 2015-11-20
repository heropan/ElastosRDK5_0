#include "elastos/droid/view/CWindowManagerGlobalHelper.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_SINGLETON_IMPL(CWindowManagerGlobalHelper)

CAR_INTERFACE_IMPL(CWindowManagerGlobalHelper, Singleton, IWindowManagerGlobalHelper)

ECode CWindowManagerGlobalHelper::GetInstance(
    /* [out] */ IWindowManagerGlobal** wmg)
{
    VALIDATE_NOT_NULL(wmg);
    *wmg = CWindowManagerGlobal::GetInstance();
    REFCOUNT_ADD(*wmg)
    return NOERROR;
}

ECode CWindowManagerGlobalHelper::GetWindowManagerService(
    /* [out] */ IIWindowManager** windowManager)
{
    VALIDATE_NOT_NULL(windowManager);
    *windowManager = CWindowManagerGlobal::GetWindowManagerService();
    REFCOUNT_ADD(*windowManager)
    return NOERROR;
}

ECode CWindowManagerGlobalHelper::GetWindowSession(
    /* [out] */ IWindowSession** windowSession)
{
    VALIDATE_NOT_NULL(windowSession);
    *windowSession = CWindowManagerGlobal::GetWindowSession();
    REFCOUNT_ADD(*windowSession)
    return NOERROR;
}

ECode CWindowManagerGlobalHelper::PeekWindowSession(
    /* [out] */ IWindowSession** windowSession)
{
    VALIDATE_NOT_NULL(windowSession);
    *windowSession = CWindowManagerGlobal::PeekWindowSession();
    REFCOUNT_ADD(*windowSession)
    return NOERROR;
}

ECode CWindowManagerGlobalHelper::ShouldDestroyEglContext(
    /* [in] */ Int32 trimLevel,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CWindowManagerGlobal::ShouldDestroyEglContext(trimLevel);
    return NOERROR;
}

ECode CWindowManagerGlobalHelper::TrimForeground()
{
    CWindowManagerGlobal::TrimForeground();
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos