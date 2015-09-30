
#include "elastos/droid/webkit/native/ui/UiUtils.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                               UiUtils
//=====================================================================
const String UiUtils::TAG("UiUtils");
const Float UiUtils::KEYBOARD_DETECT_BOTTOM_THRESHOLD_DP = 100;
AutoPtr<UiUtils::KeyboardShowingDelegate> UiUtils::sKeyboardShowingDelegate;

ECode UiUtils::SetKeyboardShowingDelegate(
    /* [in] */ KeyboardShowingDelegate* delegate)
{
    VALIDATE_NOT_NULL(delegate);
    // ==================before translated======================
    // sKeyboardShowingDelegate = delegate;
    assert(0);
    return NOERROR;
}

ECode UiUtils::ShowKeyboard(
    /* [in] */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // InputMethodManager imm =
    //         (InputMethodManager) view.getContext().getSystemService(
    //                 Context.INPUT_METHOD_SERVICE);
    // // Only shows soft keyboard if there isn't an open physical keyboard.
    // imm.showSoftInput(view, InputMethodManager.SHOW_IMPLICIT);
    assert(0);
    return NOERROR;
}

Boolean UiUtils::HideKeyboard(
    /* [in] */ IView* view)
{
    // ==================before translated======================
    // InputMethodManager imm =
    //         (InputMethodManager) view.getContext().getSystemService(
    //                 Context.INPUT_METHOD_SERVICE);
    // return imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
    assert(0);
    return FALSE;
}

Boolean UiUtils::IsKeyboardShowing(
    /* [in] */ IContext* context,
    /* [in] */ IView* view)
{
    // ==================before translated======================
    // if (sKeyboardShowingDelegate != null
    //         && sKeyboardShowingDelegate.disableKeyboardCheck(context, view)) {
    //     return false;
    // }
    //
    // View rootView = view.getRootView();
    // if (rootView == null) return false;
    // Rect appRect = new Rect();
    // rootView.getWindowVisibleDisplayFrame(appRect);
    //
    // final float density = context.getResources().getDisplayMetrics().density;
    // final float bottomMarginDp = Math.abs(rootView.getHeight() - appRect.height()) / density;
    // return bottomMarginDp > KEYBOARD_DETECT_BOTTOM_THRESHOLD_DP;
    assert(0);
    return FALSE;
}

Int32 UiUtils::InsertBefore(
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* newView,
    /* [in] */ IView* existingView)
{
    // ==================before translated======================
    // return insertView(container, newView, existingView, false);
    assert(0);
    return 0;
}

Int32 UiUtils::InsertAfter(
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* newView,
    /* [in] */ IView* existingView)
{
    // ==================before translated======================
    // return insertView(container, newView, existingView, true);
    assert(0);
    return 0;
}

AutoPtr<IBitmap> UiUtils::GenerateScaledScreenshot(
    /* [in] */ IView* currentView,
    /* [in] */ Int32 maximumDimension,
    /* [in] */ BitmapConfig bitmapConfig)
{
    // ==================before translated======================
    // Bitmap screenshot = null;
    // boolean drawingCacheEnabled = currentView.isDrawingCacheEnabled();
    // try {
    //     prepareViewHierarchyForScreenshot(currentView, true);
    //     if (!drawingCacheEnabled) currentView.setDrawingCacheEnabled(true);
    //     // Android has a maximum drawing cache size and if the drawing cache is bigger
    //     // than that, getDrawingCache() returns null.
    //     Bitmap originalBitmap = currentView.getDrawingCache();
    //     if (originalBitmap != null) {
    //         double originalHeight = originalBitmap.getHeight();
    //         double originalWidth = originalBitmap.getWidth();
    //         int newWidth = (int) originalWidth;
    //         int newHeight = (int) originalHeight;
    //         if (maximumDimension > 0) {
    //             double scale = maximumDimension / Math.max(originalWidth, originalHeight);
    //             newWidth = (int) Math.round(originalWidth * scale);
    //             newHeight = (int) Math.round(originalHeight * scale);
    //         }
    //         Bitmap scaledScreenshot =
    //                 Bitmap.createScaledBitmap(originalBitmap, newWidth, newHeight, true);
    //         if (scaledScreenshot.getConfig() != bitmapConfig) {
    //             screenshot = scaledScreenshot.copy(bitmapConfig, false);
    //             scaledScreenshot.recycle();
    //             scaledScreenshot = null;
    //         } else {
    //             screenshot = scaledScreenshot;
    //         }
    //     } else if (currentView.getMeasuredHeight() > 0 && currentView.getMeasuredWidth() > 0) {
    //         double originalHeight = currentView.getMeasuredHeight();
    //         double originalWidth = currentView.getMeasuredWidth();
    //         int newWidth = (int) originalWidth;
    //         int newHeight = (int) originalHeight;
    //         if (maximumDimension > 0) {
    //             double scale = maximumDimension / Math.max(originalWidth, originalHeight);
    //             newWidth = (int) Math.round(originalWidth * scale);
    //             newHeight = (int) Math.round(originalHeight * scale);
    //         }
    //         Bitmap bitmap = Bitmap.createBitmap(newWidth, newHeight, bitmapConfig);
    //         Canvas canvas = new Canvas(bitmap);
    //         canvas.scale((float) (newWidth / originalWidth),
    //                 (float) (newHeight / originalHeight));
    //         currentView.draw(canvas);
    //         screenshot = bitmap;
    //     }
    // } catch (OutOfMemoryError e) {
    //     Log.d(TAG, "Unable to capture screenshot and scale it down." + e.getMessage());
    // } finally {
    //     if (!drawingCacheEnabled) currentView.setDrawingCacheEnabled(false);
    //     prepareViewHierarchyForScreenshot(currentView, false);
    // }
    // return screenshot;
    assert(0);
    AutoPtr<IBitmap> empty;
    return empty;
}

UiUtils::UiUtils()
{
}

Int32 UiUtils::InsertView(
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* newView,
    /* [in] */ IView* existingView,
    /* [in] */ Boolean after)
{
    // ==================before translated======================
    // // See if the view has already been added.
    // int index = container.indexOfChild(newView);
    // if (index >= 0) return index;
    //
    // // Find the location of the existing view.
    // index = container.indexOfChild(existingView);
    // if (index < 0) return -1;
    //
    // // Add the view.
    // if (after) index++;
    // container.addView(newView, index);
    // return index;
    assert(0);
    return 0;
}

ECode UiUtils::PrepareViewHierarchyForScreenshot(
    /* [in] */ IView* view,
    /* [in] */ Boolean takingScreenshot)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // if (view instanceof ViewGroup) {
    //     ViewGroup viewGroup = (ViewGroup) view;
    //     for (int i = 0; i < viewGroup.getChildCount(); i++) {
    //         prepareViewHierarchyForScreenshot(viewGroup.getChildAt(i), takingScreenshot);
    //     }
    // } else if (view instanceof SurfaceView) {
    //     view.setWillNotDraw(!takingScreenshot);
    // }
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


