#include <CEdgeEffect.h>
#include "ext/frameworkext.h"


namespace Elastos{
namespace Droid{
namespace Widget{

ECode CEdgeEffect::constructor(
    /* [in] */ IContext* context)
{
    return EdgeEffect::Init(context);
}

ECode CEdgeEffect::SetSize(
     /* [in] */ Int32 width,
     /* [in] */ Int32 height)
{
    EdgeEffect::SetSize(width, height);
    return NOERROR;
}

ECode CEdgeEffect::SetPosition(
     /* [in] */ Int32 x,
     /* [in] */ Int32 y)
 {
     EdgeEffect::SetPosition(x, y);
     return NOERROR;
 }

ECode CEdgeEffect::IsFinished(
     /* [out] */ Boolean* isFinished)
 {
     VALIDATE_NOT_NULL(isFinished);
     *isFinished = EdgeEffect::IsFinished();
     return NOERROR;
 }

ECode CEdgeEffect::Finish()
 {
     EdgeEffect::Finish();
     return NOERROR;
 }

ECode CEdgeEffect::OnPull(
     /* [in] */ Float deltaDistance)
 {
     EdgeEffect::OnPull(deltaDistance);
     return NOERROR;
 }

ECode CEdgeEffect::OnRelease()
 {
     EdgeEffect::OnRelease();
     return NOERROR;
 }

ECode CEdgeEffect::OnAbsorb(
     /* [in] */ Int32 velocity)
 {
     EdgeEffect::OnAbsorb(velocity);
     return NOERROR;
 }

ECode CEdgeEffect::Draw(
     /* [in] */ ICanvas* canvas,
     /* [out] */ Boolean* res)
 {
     VALIDATE_NOT_NULL(res);
     *res = EdgeEffect::Draw(canvas);
     return NOERROR;
 }

ECode CEdgeEffect::GetBounds(
     /* [in] */ Boolean reverse,
     /* [out] */ IRect** rect)
 {
     VALIDATE_NOT_NULL(rect);
     AutoPtr<IRect> temp = EdgeEffect::GetBounds(reverse);;
     *rect = temp;
    REFCOUNT_ADD(*rect);
     return NOERROR;
 }

}// namespace Widget
}// namespace Droid
}// namespace Elastos
