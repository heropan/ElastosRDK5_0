
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//            InsertionHandleController::PastePopupMenu
//==================================================================

InsertionHandleController::PastePopupMenu::PastePopupMenu()
    : mPositionX(0)
    , mPositionY(0)
{
    CPopupWindow::New(mContext, NULL,
            android::R::attr::textSelectHandleWindowStyle,
            (IPopupWindow**)&mContainer);
    mContainer->SetSplitTouchEnabled(TRUE);
    mContainer->SetClippingEnabled(FALSE);

    mContainer->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
    mContainer->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);

    AutoPtr< ArrayOf<Int32> > POPUP_LAYOUT_ATTRS = ArrayOf<Int32>::Alloc(4);
    (*POPUP_LAYOUT_ATTRS)[0] = android::R::attr::textEditPasteWindowLayout;
    (*POPUP_LAYOUT_ATTRS)[1] = android::R::attr::textEditNoPasteWindowLayout;
    (*POPUP_LAYOUT_ATTRS)[2] = android::R::attr::textEditSidePasteWindowLayout;
    (*POPUP_LAYOUT_ATTRS)[3] = android::R::attr::textEditSideNoPasteWindowLayout;

    Int32 length = POPUP_LAYOUT_ATTRS->GetLength();
    mPasteViews = ArrayOf<IView>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IView> view;
        CView::New((IView**)&view);
        (*mPasteViews)[i] = view;
    }

    mPasteViewLayouts = ArrayOf<Int32>::Alloc(length);

    AutoPtr<ITypedArray> attrs;
    mContext->ObtainStyledAttributes(POPUP_LAYOUT_ATTRS, (ITypedArray**)&attrs);
    Int32 attrsLength;
    attrs->GetLength(&attrsLength);
    for (Int32 i = 0; i < attrsLength; ++i) {
        Int32 index;
        attrs->GetIndex(i, &index);
        attrs->GetResourceId(index, 0, &((*mPasteViewLayouts)[i]));
    }
    attrs->Recycle();
}

Int32 InsertionHandleController::PastePopupMenu::ViewIndex(
    /* [in] */ Boolean onTop)
{
    return (onTop ? 0 : 1 << 1) + (CanPaste() ? 0 : 1 << 0);
}

void InsertionHandleController::PastePopupMenu::UpdateContent(
    /* [in] */ Boolean onTop)
{
    const Int32 viewIndex = ViewIndex(onTop);
    AutoPtr<IView> view = (*mPasteViews)[viewIndex];

    if (view == NULL) {
        const Int32 layout = (*mPasteViewLayouts)[viewIndex];
        AutoPtr<ILayoutInflater> inflater;
        mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
        if (inflater != NULL) {
            inflater->Inflate(layout, NULL, (IView**)&view);
        }

        if (view == NULL) {
//            throw new IllegalArgumentException("Unable to inflate TextEdit paste window");
            assert(0);
        }

        const Int32 size = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
        AutoPtr<IViewGroupLayoutParams> params;
        CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
                IViewGroupLayoutParams::WRAP_CONTENT,
                (IViewGroupLayoutParams**)&params);
        view->SetLayoutParams(params);
        view->Measure(size, size);

        view->SetOnClickListener(this);

        (*mPasteViews)[viewIndex] = view;
    }

    mContainer->SetContentView(view);
}

void InsertionHandleController::PastePopupMenu::Show()
{
    UpdateContent(TRUE);
    PositionAtCursor();
}

void InsertionHandleController::PastePopupMenu::Hide()
{
    mContainer->Dismiss();
}

Boolean InsertionHandleController::PastePopupMenu::IsShowing()
{
    Boolean bFlag = FALSE;
    mContainer->IsShowing(&bFlag);
    return bFlag;
}

//@Override
ECode InsertionHandleController::PastePopupMenu::OnClick(
    /* [in] */ IView* v)
{
    if (CanPaste()) {
        Paste();
    }
    Hide();

    return NOERROR;
}

void InsertionHandleController::PastePopupMenu::PositionAtCursor()
{
    AutoPtr<IView> contentView;
    mContainer->GetContentView((IView**)&contentView);
    Int32 width;
    contentView->GetMeasuredWidth(&width);
    Int32 height;
    contentView->GetMeasuredHeight(&height);

    Int32 lineHeight = GetLineHeight();

    Int32 x;
    mHandle->GetAdjustedPositionX(&x);
    mPositionX = (Int32) (x - width / 2.0f);
    Int32 y;
    mHandle->GetAdjustedPositionY(&y);
    mPositionY = y - height - lineHeight;

    const AutoPtr< ArrayOf<Int32> > coords = ArrayOf<Int32>::Alloc(2);
    mParent->GetLocationInWindow(coords);
    (*coords)[0] += mPositionX;
    (*coords)[1] += mPositionY;

    Int32 screenWidth;
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    metrics->GetWidthPixels(&screenWidth);
    if ((*coords)[1] < 0) {
        UpdateContent(FALSE);
        // Update dimensions from new view
        mContainer->GetContentView((IView**)&contentView);
        contentView->GetMeasuredWidth(&width);
        contentView->GetMeasuredHeight(&height);

        // Vertical clipping, move under edited line and to the side of insertion cursor
        // TODO bottom clipping in case there is no system bar
        (*coords)[1] += height;
        (*coords)[1] += lineHeight;

        // Move to right hand side of insertion cursor by default. TODO RTL text.
        AutoPtr<IDrawable> handle;
        mHandle->GetDrawable((IDrawable**)&handle);
        Int32 intrinsicWidth;
        handle->GetIntrinsicWidth(&intrinsicWidth);
        const Int32 handleHalfWidth = intrinsicWidth / 2;

        Int32 positionX;
        mHandle->GetAdjustedPositionX(&positionX);
        if (positionX + width < screenWidth) {
            (*coords)[0] += handleHalfWidth + width / 2;
        }
        else {
            (*coords)[0] -= handleHalfWidth + width / 2;
        }
    }
    else {
        // Horizontal clipping
        (*coords)[0] = Math::Max(0, (*coords)[0]);
        (*coords)[0] = Math::Min(screenWidth - width, (*coords)[0]);
    }

    mContainer->ShowAtLocation(mParent, IGravity::NO_GRAVITY, (*coords)[0], (*coords)[1]);
}

//==================================================================
//                 InsertionHandleController
//==================================================================

InsertionHandleController::InsertionHandleController(
    /* [in] */ IView* parent,
    /* [in] */ PositionObserver* positionObserver)
    : mIsShowing(FALSE)
    , mAllowAutomaticShowing(FALSE)
    , mParent(parent)
    , mPositionObserver(positionObserver)
{
    parent->GetContext((IContext**)&mContext);
}

/** Allows the handle to be shown automatically when cursor position changes */
void InsertionHandleController::AllowAutomaticShowing()
{
    mAllowAutomaticShowing = TRUE;
}

/** Disallows the handle from being shown automatically when cursor position changes */
void InsertionHandleController::HideAndDisallowAutomaticShowing()
{
    Hide();
    mAllowAutomaticShowing = FALSE;
}

/**
 * Shows the handle.
 */
void InsertionHandleController::ShowHandle()
{
    CreateHandleIfNeeded();
    ShowHandleIfNeeded();
}

void InsertionHandleController::ShowPastePopup()
{
    if (mIsShowing) {
        mHandle->ShowPastePopupWindow();
    }
}

void InsertionHandleController::ShowHandleWithPastePopup()
{
    ShowHandle();
    ShowPastePopup();
}

/**
 * @return whether the handle is being dragged.
 */
Boolean InsertionHandleController::IsDragging()
{
    Boolean bFlag = FALSE;
    return mHandle != NULL && (mHandle->IsDragging(&bFlag), bFlag);
}

/** Shows the handle at the given coordinates, as long as automatic showing is allowed */
void InsertionHandleController::OnCursorPositionChanged()
{
    if (mAllowAutomaticShowing) {
        ShowHandle();
    }
}

/**
 * Moves the handle so that it points at the given coordinates.
 * @param x Handle x in physical pixels.
 * @param y Handle y in physical pixels.
 */
void InsertionHandleController::SetHandlePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mHandle->PositionAt((Int32) x, (Int32) y);
}

/**
 * If the handle is not visible, sets its visibility to View.VISIBLE and begins fading it in.
 */
void InsertionHandleController::BeginHandleFadeIn()
{
    mHandle->BeginFadeIn();
}

/**
 * Sets the handle to the given visibility.
 */
void InsertionHandleController::SetHandleVisibility(
    /* [in] */ Int32 visibility)
{
    mHandle->SetVisibility(visibility);
}

Int32 InsertionHandleController::GetHandleX()
{
    Int32 x;
    mHandle->GetAdjustedPositionX(&x);
    return x;
}

Int32 InsertionHandleController::GetHandleY()
{
    Int32 y;
    mHandle->GetAdjustedPositionY(&y);
    return y;
}

//@VisibleForTesting
AutoPtr<HandleView> InsertionHandleController::GetHandleViewForTest()
{
    return mHandle;
}

//@Override
void InsertionHandleController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }
}

//@Override
void InsertionHandleController::Hide()
{
    if (mIsShowing) {
        if (mHandle != NULL) {
            mHandle->Hide();
        }
        mIsShowing = FALSE;
    }
}

//@Override
Boolean InsertionHandleController::IsShowing()
{
    return mIsShowing;
}

//@Override
void InsertionHandleController::BeforeStartUpdatingPosition(
    /* [in] */ HandleView* handle)
{
}

//@Override
void InsertionHandleController::UpdatePosition(
    /* [in] */ HandleView* handle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    SetCursorPosition(x, y);
}

//@Override
void InsertionHandleController::OnDetached()
{
}

Boolean InsertionHandleController::CanPaste()
{
    AutoPtr<IClipboardManager> cm;
    mContext->GetSystemService(
            IContext::CLIPBOARD_SERVICE,
            (IClipboardManager**)&cm);
    Boolean bFlag = FALSE;
    cm->HasPrimaryClip(&bFlag);

    return bFlag;
}

void InsertionHandleController::CreateHandleIfNeeded()
{
    if (mHandle == NULL) {
        mHandle = new HandleView(this, HandleView::CENTER, mParent, mPositionObserver);
    }
}

void InsertionHandleController::ShowHandleIfNeeded()
{
    if (!mIsShowing) {
        mIsShowing = TRUE;
        mHandle->Show();
        SetHandleVisibility(HandleView::VISIBLE);
    }
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
