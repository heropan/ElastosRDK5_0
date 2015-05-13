
#include "app/CActivityOptionsHelper.h"
#include "app/CActivityOptions.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CActivityOptionsHelper::MakeCustomAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 enterResId,
    /* [in] */ Int32 exitResId,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeCustomAnimation(
        context, enterResId, exitResId, activityoptions);
}

ECode CActivityOptionsHelper::MakeCustomAnimationEx(
    /* [in] */ IContext* context,
    /* [in] */ Int32 enterResId,
    /* [in] */ Int32 exitResId,
    /* [in] */ IHandler* handler,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeCustomAnimation(
        context, enterResId, exitResId, handler, listener, activityoptions);
}

ECode CActivityOptionsHelper::MakeScaleUpAnimation(
    /* [in] */ IView* source,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 startWidth,
    /* [in] */ Int32 startHeight,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeScaleUpAnimation(
        source, startX, startY, startWidth, startHeight, activityoptions);
}

ECode CActivityOptionsHelper::MakeThumbnailScaleUpAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeThumbnailScaleUpAnimation(
        source, thumbnail, startX, startY, activityoptions);
}

ECode CActivityOptionsHelper::MakeThumbnailScaleUpAnimationEx(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeThumbnailScaleUpAnimation(
        source, thumbnail, startX, startY, listener, activityoptions);
}

ECode CActivityOptionsHelper::MakeThumbnailScaleDownAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** activityoptions)
{
    return CActivityOptions::MakeThumbnailScaleDownAnimation(
        source, thumbnail, startX, startY, listener, activityoptions);
}

ECode CActivityOptionsHelper::Abort(
    /* [in] */ IBundle* options)
{
    return CActivityOptions::Abort(options);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
