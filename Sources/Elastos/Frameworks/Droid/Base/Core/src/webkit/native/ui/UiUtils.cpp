
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const String UiUtils::TAG("UiUtils");
const Float UiUtils::KEYBOARD_DETECT_BOTTOM_THRESHOLD_DP = 100;
AutoPtr<KeyboardShowingDelegate> UiUtils::sKeyboardShowingDelegate;

ECode UiUtils::SetKeyboardShowingDelegate(
    /* in */ KeyboardShowingDelegate* delegate)
{
    VALIDATE_NOT_NULL(delegate);
    sKeyboardShowingDelegate = delegate;
    return NOERROR;
}

ECode UiUtils::ShowKeyboard(
    /* in */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IInputMethodManager> imm;
    AutoPtr<IContext> context;
    view->GetContext((IContext**)&context);
    context->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInputMethodManager)&imm);

    // Only shows soft keyboard if there isn't an open physical keyboard.
    imm->ShowSoftInput(view, IInputMethodManager::SHOW_IMPLICIT);
    return NOERROR;
}

Boolean UiUtils::HideKeyboard(
    /* in */ IView* view)
{
    AutoPtr<IInputMethodManager> imm;
    AutoPtr<IContext> context;
    view->GetContext((IContext**)&context);
    context->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInputMethodManager)&imm);

    AutoPtr<IBinder> binder;
    view->GetWindowToken((IBinder**)&binder);

    Boolean result = FALSE;
    imm->HideSoftInputFromWindow(binder, 0, &result);
    return result;
}

Boolean UiUtils::IsKeyboardShowing(
    /* in */ IContext* context,
    /* in */ IView* view)
{
    if (NULL != sKeyboardShowingDelegate && sKeyboardShowingDelegate->DisableKeyboardCheck(context, view)) {
        return FALSE;
    }

    AutoPtr<IView> rootView;
    view->GetRootView((IView**)&rootView);

    if (NULL == rootView)
        return FALSE;

    AutoPtr<IRect> appRect;
    CRect::New((IRect**)&appRect);
    rootView->GetWindowVisibleDisplayFrame((IRect**)appRect);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);

    AutoPtr<IDisplayMetrics> metrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);

    Int32 rootheight;
    rootView->GetHeight(&rootheight);

    Int32 appHeight;
    appRect->Height(&appHeight);

    const Float density = metrics->density;
    const Float bottomMarginDp = Math:Abs(rootheight - appHeight) / density;
    return (Boolean)(bottomMarginDp > KEYBOARD_DETECT_BOTTOM_THRESHOLD_DP);
}

Int32 UiUtils::InsertBefore(
    /* in */ IViewGroup* container,
    /* in */ IView* newView,
    /* in */ IView* existingView)
{
    return InsertView(container, newView, existingView, FALSE);
}

Int32 UiUtils::InsertAfter(
    /* in */ IViewGroup* container,
    /* in */ IView* newView,
    /* in */ IView* existingView)
{
    return InsertView(container, newView, existingView, TRUE);
}

AutoPtr<IBitmap> UiUtils::GenerateScaledScreenshot(
    /* in */ IView* currentView,
    /* in */ Int32 maximumDimension,
    /* in */ IBitmap::IConfig* bitmapConfig)
{
    AutoPtr<IBitmap> screenshot = NULL;
    Boolean drawingCacheEnabled;
    currentView->IsDrawingCacheEnabled(&drawingCacheEnabled);

    //try {
        PrepareViewHierarchyForScreenshot(currentView, TRUE);

        if (!drawingCacheEnabled)
            currentView->SetDrawingCacheEnabled(TRUE);

        // Android has a maximum drawing cache size and if the drawing cache is bigger
        // than that, getDrawingCache() returns null.
        AutoPtr<IBitmap> originalBitmap
        currentView-GetDrawingCache((IBitmap**)&originalBitmap);

        Int32 currViewMeasureHeight;
        Int32 currViewMeasureWidth;
        currentView->GetMeasuredHeight(&currViewMeasureHeight);
        currentView->GetMeasuredWidth(&currViewMeasureWidth);

        if (originalBitmap != NULL) {
            Double originalHeight;
            originalBitmap->GetHeight(&originalHeight);

            Double originalWidth;
            originalBitmap->GetWidth(&originalWidth);

            Int32 newWidth = (Int32)originalWidth;
            Int32 newHeight = (Int32)originalHeight;

            if (maximumDimension > 0) {
                Double scale = maximumDimension / Math::Max(originalWidth, originalHeight);
                newWidth = (Int32)Math::Round(originalWidth * scale);
                newHeight = (Int32)Math::Round(originalHeight * scale);
            }

            AutoPtr<IBitmap> scaledScreenshot
            CBitmap::New(originalBitmap, newWidth, newHeight, TRUE, (IBitmap**)&scaledScreenshot);

            AutoPtr<IBitmap::IConfig> scaledCofig;
            scaledScreenshot->GetConfig((IBitmap::IConfig**)&scaledCofig);

            if (scaledCofig != bitmapConfig) {
                scaledScreenshot->Copy(bitmapConfig, FALSE, (IBitmap**)&screenshot);
                scaledScreenshot->Recycle();
                scaledScreenshot = NULL;
            }
            else {
                screenshot = scaledScreenshot;
            }
        }
        else if (currViewMeasureHeight > 0 && currViewMeasureWidth > 0) {
            Double originalHeight;
            Double originalWidth;
            currentView->GetMeasuredHeight(&originalHeight);
            currentView->GetMeasuredWidth(&originalWidth);

            Int32 newWidth = (Int32) originalWidth;
            Int32 newHeight = (Int32) originalHeight;
            if (maximumDimension > 0) {
                Double scale = maximumDimension / Math::Max(originalWidth, originalHeight);
                newWidth = (Int32) Math::Round(originalWidth * scale);
                newHeight = (Int32) Math::Round(originalHeight * scale);
            }

            AutoPtr<IBitmap> bitmap
            CBitmap::New(newWidth, newHeight, bitmapConfig, (IBitmap**)&bitmap);

            AutoPtr<ICanvas> canvas;
            CCanvas::New(bitmap, (ICanvas**)&canvas);

            canvas->Scale((Float) (newWidth / originalWidth),
                    (Float) (newHeight / originalHeight));
            currentView->Draw(canvas);
            screenshot = bitmap;
        }
    //} catch (OutOfMemoryError e) {
    //    Log.d(TAG, "Unable to capture screenshot and scale it down." + e.getMessage());
    //} finally {
        if (!drawingCacheEnabled)
            currentView->SetDrawingCacheEnabled(FALSE);
        PrepareViewHierarchyForScreenshot(currentView, FALSE);
    //}
    return screenshot;
}

UiUtils::UiUtils()
{
}

Int32 UiUtils::InsertView(
    /* in */ IViewGroup* container,
    /* in */ IView* newView,
    /* in */ IView* existingView,
    /* in */ Boolean after)
{
    // See if the view has already been added.
    Int32 index;
    container->IndexOfChild(newView, &index);
    if (index >= 0)
        return index;

    // Find the location of the existing view.
    container->IndexOfChild(existingView, &index);
    if (index < 0)
        return -1;

    // Add the view.
    if (after)
        ++index;

    container->AddView(newView, index);
    return index;
}

ECode UiUtils::PrepareViewHierarchyForScreenshot(
    /* in */ IView* view,
    /* in */ Boolean takingScreenshot)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IViewGroup> viewGroup = (IViewGroup*)view->Probe(EIID_IViewGroup);
    AutoPtr<ISurfaceView> surfaceView = (ISurfaceView*)view->Probe(EIID_ISurfaceView);
    if (NULL != viewGroup) {
        for (Int32 i = 0; i < viewGroup->GetChildCount(); ++i) {
            AutoPtr<IView> child;
            viewGroup->GetChildAt(i, (IView**)&child);
            PrepareViewHierarchyForScreenshot(child, takingScreenshot);
        }
    }
    else if (NULL != surfaceView) {
        view->SetWillNotDraw(!takingScreenshot);
    }
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

