#include "elastos/droid/app/SharedElementCallback.h"

namespace Elastos {
namespace Droid {
namespace App {

// using Elastos::Droid::content.Context;
// using Elastos::Droid::content.res.Resources;
// using Elastos::Droid::graphics.Bitmap;
// using Elastos::Droid::graphics.Canvas;
// using Elastos::Droid::graphics.Matrix;
// using Elastos::Droid::graphics.RectF;
// using Elastos::Droid::graphics.drawable.BitmapDrawable;
// using Elastos::Droid::os.Parcelable;
// using Elastos::Droid::transition.TransitionUtils;
// using Elastos::Droid::view.View;

// import java.util.List;
// import java.util.Map;

static const AutoPtr<ISharedElementCallback> SharedElementCallback::NULL_CALLBACK = new SharedElementCallback();

CAR_INTERFACE_IMPL(SharedElementCallback, Object, ISharedElementCallback)

SharedElementCallback::SharedElementCallback()
{}

SharedElementCallback::~SharedElementCallback()
{}

ECode SharedElementCallback::OnSharedElementStart(
    /* [in] */ IList* sharedElementNames, //List<String>
    /* [in] */ IList* sharedElements, //List<View>
    /* [in] */ IList* sharedElementSnapshots) //List<View>
{
    return NOERROR;
}

ECode SharedElementCallback::OnSharedElementEnd(
    /* [in] */ IList* sharedElementNames, //List<String>
    /* [in] */ IList* sharedElements, //List<View>
    /* [in] */ IList* sharedElementSnapshots) //List<View>
{
    return NOERROR;
}


ECode SharedElementCallback::OnRejectSharedElements(
    /* [in] */ IList* rejectedSharedElements) // List<View>
{
    return NOERROR;
}

ECode SharedElementCallback::OnMapSharedElements(
    /* [in] */ IList* names, //List<String>
    /* [in] */ IMap* sharedElements) //Map<String, View>
{
    return NOERROR;
}

ECode SharedElementCallback::OnCaptureSharedElementSnapshot(
    /* [in] */ IView* sharedElement,
    /* [in] */ IMatrix* viewToGlobalMatrix,
    /* [in] */ IRectF* screenBounds,
    /* [out] */ IParcelable** parcleable)
{
    if (mTempMatrix == null) {
        mTempMatrix = new Matrix(viewToGlobalMatrix);
    } else {
        mTempMatrix.set(viewToGlobalMatrix);
    }
    return TransitionUtils.createViewBitmap(sharedElement, mTempMatrix, screenBounds);
}

ECode SharedElementCallback::OnCreateSnapshotView(
    /* [in] */ IContext* context,
    /* [in] */ IParcelable* snapshot,
    /* [out] */ IView** view)
{
    View view = null;
    if (snapshot instanceof Bitmap) {
        Bitmap bitmap = (Bitmap) snapshot;
        view = new View(context);
        Resources resources = context.getResources();
        view.setBackground(new BitmapDrawable(resources, bitmap));
    }
    return view;
}


} // namespace App
} // namespace Droid
} // namespace Elastos