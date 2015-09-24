
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

const String BrowserAccessibilityManager::TAG("BrowserAccessibilityManager");

static AutoPtr< ArrayOf<Int32> > mTempLocation_init()
{
    AutoPtr< ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    return array;
}

BrowserAccessibilityManager::BrowserAccessibilityManager(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ ContentViewCore* contentViewCore)
    : mNativeObj(nativeBrowserAccessibilityManagerAndroid)
    , mContentViewCore(contentViewCore)
    , mAccessibilityFocusId(IView::NO_ID)
    , mIsHovering(FALSE)
    , mCurrentRootId(IView::NO_ID)
    , mLastHoverId(IView::NO_ID)
    , mTempLocation(mTempLocation_init())
    , mUserHasTouchExplored(FALSE)
    , mPendingScrollToMakeNodeVisible(FALSE)
{
    mContentViewCore->SetBrowserAccessibilityManager(this);
    mView = mContentViewCore->GetContainerView();
    mRenderCoordinates = mContentViewCore->GetRenderCoordinates();
    mContentViewCore->GetContext()
        ->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&mAccessibilityManager);
}

/**
 * Create a BrowserAccessibilityManager object, which is owned by the C++
 * BrowserAccessibilityManagerAndroid instance, and connects to the content view.
 * @param nativeBrowserAccessibilityManagerAndroid A pointer to the counterpart native
 *     C++ object that owns this object.
 * @param contentViewCore The content view that this object provides accessibility for.
 */
//@CalledByNative
AutoPtr<BrowserAccessibilityManager> BrowserAccessibilityManager::Create(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ ContentViewCore contentViewCore)
{
    // A bug in the KitKat framework prevents us from using these new APIs.
    // http://crbug.com/348088/
    // if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
    //     return new KitKatBrowserAccessibilityManager(
    //             nativeBrowserAccessibilityManagerAndroid, contentViewCore);

    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
        AutoPtr<BrowserAccessibilityManager> ret = new JellyBeanBrowserAccessibilityManager(
                nativeBrowserAccessibilityManagerAndroid, contentViewCore);
        return ret;
    }
    else {
        AutoPtr<BrowserAccessibilityManager> ret = new BrowserAccessibilityManager(
                nativeBrowserAccessibilityManagerAndroid, contentViewCore);
        return ret;
    }
}



//@CalledByNative
void BrowserAccessibilityManager::OnNativeObjectDestroyed()
{
    if (mContentViewCore->GetBrowserAccessibilityManager() == this) {
        mContentViewCore->SetBrowserAccessibilityManager(NULL);
    }

    mNativeObj = 0;
    mContentViewCore = NULL;
}

/**
 * @return An AccessibilityNodeProvider on JellyBean, and null on previous versions.
 */
AutoPtr<IAccessibilityNodeProvider> BrowserAccessibilityManager::GetAccessibilityNodeProvider()
{
    return NULL;
}

/**
 * @see AccessibilityNodeProvider#createAccessibilityNodeInfo(int)
 */
AutoPtr<IAccessibilityNodeInfo> BrowserAccessibilityManager::CreateAccessibilityNodeInfo(
    /* [in] */ Int32 virtualViewId)
{
    Boolean isEnabled;
    mAccessibilityManager->IsEnabled(&isEnabled);
    if (!isEnabled || mNativeObj == 0) {
        return NULL;
    }

    Int32 rootId = NativeGetRootId(mNativeObj);

    if (virtualViewId == IView::NO_ID) {
        return CreateNodeForHost(rootId);
    }

    if (!IsFrameInfoInitialized()) {
        return NULL;
    }

    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    AutoPtr<IAccessibilityNodeInfo> info;
    helper->Obtain(mView, (IAccessibilityNodeInfo**)&info);
    String pkName;
    mContentViewCore->GetContext()->GetPackageName(&pkName);
    info->SetPackageName(pkName);
    info->SetSource(mView, virtualViewId);

    if (virtualViewId == rootId) {
        info->SetParent(mView);
    }

    if (NativePopulateAccessibilityNodeInfo(mNativeObj, info, virtualViewId)) {
        return info;
    }
    else {
        info->Recycle();
        return NULL;
    }
}

/**
 * @see AccessibilityNodeProvider#findAccessibilityNodeInfosByText(String, int)
 */
List<AccessibilityNodeInfo> BrowserAccessibilityManager::FindAccessibilityNodeInfosByText(
    /* [in] */ const String& text,
    /* [in] */ Int32 virtualViewId)
{
    return new ArrayList<AccessibilityNodeInfo>();
}

/**
 * @see AccessibilityNodeProvider#performAction(int, int, Bundle)
 */
Boolean BrowserAccessibilityManager::PerformAction(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    // We don't support any actions on the host view or nodes
    // that are not (any longer) in the tree.
    Boolean isEnabled;
    mAccessibilityManager->IsEnabled(&isEnabled);
    if (!isEnabled || mNativeObj == 0
            || !NativeIsNodeValid(mNativeObj, virtualViewId)) {
        return FALSE;
    }

    switch (action) {
        case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS:
            if (mAccessibilityFocusId == virtualViewId) {
                return TRUE;
            }

            mAccessibilityFocusId = virtualViewId;
            SendAccessibilityEvent(mAccessibilityFocusId,
                    IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
            if (!mIsHovering) {
                NativeScrollToMakeNodeVisible(
                        mNativeObj, mAccessibilityFocusId);
            }
            else {
                mPendingScrollToMakeNodeVisible = TRUE;
            }
            return TRUE;
        case AccessibilityNodeInfo.ACTION_CLEAR_ACCESSIBILITY_FOCUS:
            if (mAccessibilityFocusId == virtualViewId) {
                SendAccessibilityEvent(mAccessibilityFocusId,
                        IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED);
                mAccessibilityFocusId = IView::NO_ID;
            }
            return TRUE;
        case IAccessibilityNodeInfo::ACTION_CLICK:
            NativeClick(mNativeObj, virtualViewId);
            SendAccessibilityEvent(virtualViewId,
                    IAccessibilityEvent::TYPE_VIEW_CLICKED);
            return TRUE;
        case IAccessibilityNodeInfo::ACTION_FOCUS:
            NativeFocus(mNativeObj, virtualViewId);
            return TRUE;
        case IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS:
            NativeBlur(mNativeObj);
            return TRUE;

        case IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT: {
            if (arguments == NULL)
                return FALSE;
            String elementType;
            arguments->GetString(
                IAccessibilityNodeInfo::ACTION_ARGUMENT_HTML_ELEMENT_STRING, &elementType);
            if (elementType == NULL)
                return FALSE;
            elementType = elementType.ToUpperCase(ILocale::US);
            return JumpToElementType(elementType, TRUE);
        }
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT: {
            if (arguments == NULL)
                return FALSE;
            String elementType;
            arguments->GetString(
                IAccessibilityNodeInfo::ACTION_ARGUMENT_HTML_ELEMENT_STRING,
                &elementType);
            if (elementType == NULL)
                return FALSE;
            elementType = elementType.ToUpperCase(ILocale::US);
            return JumpToElementType(elementType, FALSE);
        }

        default:
            break;
    }

    return FALSE;
}

/**
 * @see View#onHoverEvent(MotionEvent)
 */
Boolean BrowserAccessibilityManager::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean isEnabled;
    mAccessibilityManager->IsEnabled(&isEnabled);
    if (!isEnabled || mNativeObj == 0) {
        return FALSE;
    }

    Int32 action;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_HOVER_EXIT) {
        mIsHovering = FALSE;
        if (mPendingScrollToMakeNodeVisible) {
            NativeScrollToMakeNodeVisible(
                    mNativeObj, mAccessibilityFocusId);
        }
        mPendingScrollToMakeNodeVisible = FALSE;
        return TRUE;
    }

    mIsHovering = TRUE;
    mUserHasTouchExplored = TRUE;
    Float x;
    event->GetX(&x);
    Float y;
    event->GetY(&y);

    // Convert to CSS coordinates.
    Int32 cssX = (Int32) (mRenderCoordinates->FromPixToLocalCss(x));
    Int32 cssY = (Int32) (mRenderCoordinates->FromPixToLocalCss(y));

    // This sends an IPC to the render process to do the hit testing.
    // The response is handled by handleHover.
    NativeHitTest(mNativeObj, cssX, cssY);

    return TRUE;
}

/**
 * Called by ContentViewCore to notify us when the frame info is initialized,
 * the first time, since until that point, we can't use mRenderCoordinates to transform
 * web coordinates to screen coordinates.
 */
void BrowserAccessibilityManager::NotifyFrameInfoInitialized()
{
    // Invalidate the container view, since the chrome accessibility tree is now
    // ready and listed as the child of the container view.
    mView->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);

    // (Re-) focus focused element, since we weren't able to create an
    // AccessibilityNodeInfo for this element before.
    if (mAccessibilityFocusId != IView::NO_ID) {
        SendAccessibilityEvent(mAccessibilityFocusId,
                               IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
    }
}

Boolean BrowserAccessibilityManager::JumpToElementType(
    /* [in] */ const String& elementType,
    /* [in] */ Boolean forwards)
{
    Int32 id = NativeFindElementType(mNativeObj, mAccessibilityFocusId, elementType, forwards);
    if (id == 0)
        return FALSE;

    mAccessibilityFocusId = id;
    SendAccessibilityEvent(id, AccessibilityEvent.TYPE_VIEW_ACCESSIBILITY_FOCUSED);

    return TRUE;
}

void BrowserAccessibilityManager::SendAccessibilityEvent(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 eventType)
{
    // If we don't have any frame info, then the virtual hierarchy
    // doesn't exist in the view of the Android framework, so should
    // never send any events.
    Boolean isEnabled;
    mAccessibilityManager->IsEnabled(&isEnabled);
    if (!isEnabled || mNativeObj == 0
            || !IsFrameInfoInitialized()) {
        return;
    }

    // This is currently needed if we want Android to draw the yellow box around
    // the item that has accessibility focus. In practice, this doesn't seem to slow
    // things down, because it's only called when the accessibility focus moves.
    // TODO(dmazzoni): remove this if/when Android framework fixes bug.
    mView->PostInvalidate();

    // The container view is indicated by a virtualViewId of NO_ID; post these events directly
    // since there's no web-specific information to attach.
    if (virtualViewId == IView::NO_ID) {
        mView->SendAccessibilityEvent(eventType);
        return;
    }

    final AccessibilityEvent event = AccessibilityEvent.obtain(eventType);
    event.setPackageName(mContentViewCore.getContext().getPackageName());
    event.setSource(mView, virtualViewId);
    if (!nativePopulateAccessibilityEvent(mNativeObj, event, virtualViewId, eventType)) {
        event.recycle();
        return;
    }

    mView->RequestSendAccessibilityEvent(mView, event);
}

AutoPtr<IBundle> BrowserAccessibilityManager::GetOrCreateBundleForAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AutoPtr<IBundle> bundle;
    event->GetParcelableData((IInterface**)&bundle);
    if (bundle == NULL) {
        CBundle((IBundle**)&bundle);
        event->SetParcelableData(bundle);
    }

    return bundle;
}

AutoPtr<IAccessibilityNodeInfo> BrowserAccessibilityManager::CreateNodeForHost(
    /* [in] */ Int32 rootId)
{
    // Since we don't want the parent to be focusable, but we can't remove
    // actions from a node, copy over the necessary fields.
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);

    AutoPtr<IAccessibilityNodeInfo> result;
    helper->Obtain(mView, (IAccessibilityNodeInfo**)&result);
    AutoPtr<IAccessibilityNodeInfo> source;
    helper->Obtain(mView, (IAccessibilityNodeInfo**)&source);
    mView->OnInitializeAccessibilityNodeInfo(source);

    // Copy over parent and screen bounds.
    AutoPtr<IRect> rect;
    CRect((IRect**)&rect);
    source->GetBoundsInParent(rect);
    result->SetBoundsInParent(rect);
    source->GetBoundsInScreen(rect);
    result->SetBoundsInScreen(rect);

    // Set up the parent view, if applicable.
    AutoPtr<IViewParent> parent;
    mView->GetParentForAccessibility((IViewParent**)&parent);
    if (parent->Probe(EIID_IView)) {
        result->SetParent((View) parent);
    }

    // Populate the minimum required fields.
    Boolean isVisibleToUser;
    source->IsVisibleToUser(&isVisibleToUser);
    result->SetVisibleToUser(isVisibleToUser);
    Boolean isEnabled;
    source->IsEnabled(&isEnabled);
    result->SetEnabled(isEnabled);
    String pkName;
    source->GetPackageName(&pkName);
    result->SetPackageName(pkName);
    String className;
    source->GetClassName(&className);
    result->SetClassName(className);

    // Add the Chrome root node.
    if (IsFrameInfoInitialized()) {
        result->AddChild(mView, rootId);
    }

    return result;
}

Boolean BrowserAccessibilityManager::IsFrameInfoInitialized()
{
    return mRenderCoordinates->GetContentWidthCss() != 0.0 ||
           mRenderCoordinates->GetContentHeightCss() != 0.0;
}

//@CalledByNative
void BrowserAccessibilityManager::HandlePageLoaded(
    /* [in] */ Int32 id)
{
    if (mUserHasTouchExplored) return;

    if (mContentViewCore->ShouldSetAccessibilityFocusOnPageLoad()) {
        mAccessibilityFocusId = id;
        SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
    }
}

//@CalledByNative
void BrowserAccessibilityManager::HandleFocusChanged(
    /* [in] */ Int32 id)
{
    sendAccessibilityEvent(id, AccessibilityEvent.TYPE_VIEW_FOCUSED);

    // Update accessibility focus if not already set to this node.
    if (mAccessibilityFocusId != id) {
        SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
        mAccessibilityFocusId = id;
    }
}

//@CalledByNative
void BrowserAccessibilityManager::HandleCheckStateChanged(
    /* [in] */ Int32 id)
{
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_CLICKED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleTextSelectionChanged(
    /* [in] */ Int32 id)
{
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_TEXT_SELECTION_CHANGED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleEditableTextChanged(
    /* [in] */ Int32 id)
{
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_TEXT_CHANGED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleContentChanged(
    /* [in] */ Int32 id)
{
    Int32 rootId = NativeGetRootId(mNativeObj);
    if (rootId != mCurrentRootId) {
        mCurrentRootId = rootId;
        mView->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);
    }
    else {
        SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);
    }
}

//@CalledByNative
void BrowserAccessibilityManager::HandleNavigate()
{
    mAccessibilityFocusId = IView::NO_ID;
    mUserHasTouchExplored = FALSE;
    // Invalidate the host, since its child is now gone.
    mView->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleScrollPositionChanged(
    /* [in] */ Int32 id)
{
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_SCROLLED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleScrolledToAnchor(
    /* [in] */ Int32 id)
{
    if (mAccessibilityFocusId == id) {
        return;
    }

    mAccessibilityFocusId = id;
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
}

//@CalledByNative
void BrowserAccessibilityManager::HandleHover(
    /* [in] */ Int32 id)
{
    if (mLastHoverId == id) return;

    // Always send the ENTER and then the EXIT event, to match a standard Android View.
    SendAccessibilityEvent(id, IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER);
    SendAccessibilityEvent(mLastHoverId, IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT);
    mLastHoverId = id;
}

//@CalledByNative
void BrowserAccessibilityManager::AnnounceLiveRegionText(
    /* [in] */ const String& text)
{
    mView->AnnounceForAccessibility(text);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoParent(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 parentId)
{
    node->SetParent(mView, parentId);
}

//@CalledByNative
void BrowserAccessibilityManager::AddAccessibilityNodeInfoChild(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 childId)
{
    node->AddChild(mView, childId);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoBooleanAttributes(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean checked,
    /* [in] */ Boolean clickable,
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean focusable,
    /* [in] */ Boolean focused,
    /* [in] */ Boolean password,
    /* [in] */ Boolean scrollable,
    /* [in] */ Boolean selected,
    /* [in] */ Boolean visibleToUser)
{
    node->SetCheckable(checkable);
    node->SetChecked(checked);
    node->SetClickable(clickable);
    node->SetEnabled(enabled);
    node->SetFocusable(focusable);
    node->SetFocused(focused);
    node->SetPassword(password);
    node->SetScrollable(scrollable);
    node->SetSelected(selected);
    node->SetVisibleToUser(visibleToUser);

    node->AddAction(IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT);
    node->AddAction(IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT);

    if (focusable) {
        if (focused) {
            node->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS);
        }
        else {
            node->AddAction(IAccessibilityNodeInfo::ACTION_FOCUS);
        }
    }

    if (mAccessibilityFocusId == virtualViewId) {
        node->SetAccessibilityFocused(TRUE);
        node->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
    }
    else {
        node->SetAccessibilityFocused(FALSE);
        node->AddAction(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
    }

    if (clickable) {
        node->AddAction(IAccessibilityNodeInfo::ACTION_CLICK);
    }
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoClassName(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ const String& className)
{
    node->SetClassName(className);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoContentDescription(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ const String& contentDescription,
    /* [in] */ Boolean annotateAsLink)
{
    if (annotateAsLink) {
        AutoPtr<ISpannableString> spannable;
        CSpannableString(contentDescription, (ISpannableString**)&spannable);
        AutoPtr<IURLSpan> URLSpan;
        CURLSpan::New(String(""), (IURLSpan**)&URLSpan);
        Int32 length;
        spannable->GetLength(&length);
        spannable->SetSpan(URLSpan, 0, length, 0);
        node->SetContentDescription(spannable);
    }
    else {
        node->SetContentDescription(contentDescription);
    }
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoLocation(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 absoluteLeft,
    /* [in] */ Int32 absoluteTop,
    /* [in] */ Int32 parentRelativeLeft,
    /* [in] */ Int32 parentRelativeTop,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean isRootNode)
{
    // First set the bounds in parent.
    AutoPtr<IRect> boundsInParent;
    CRect::New(parentRelativeLeft, parentRelativeTop,
            parentRelativeLeft + width, parentRelativeTop + height, (IRect**)&boundsInParent);
    if (isRootNode) {
        // Offset of the web content relative to the View.
        boundsInParent->Offset(0, (Int32) mRenderCoordinates->GetContentOffsetYPix());
    }

    node->SetBoundsInParent(boundsInParent);

    // Now set the absolute rect, which requires several transformations.
    AutoPtr<IRect> rect;
    CRect::New(absoluteLeft, absoluteTop, absoluteLeft + width, absoluteTop + height, (IRect**)&rect);

    // Offset by the scroll position.
    rect->Offset(-(Int32) mRenderCoordinates->GetScrollX(),
                -(Int32) mRenderCoordinates->GetScrollY());

    // Convert CSS (web) pixels to Android View pixels
    Int32 left;
    rect->GetLeft(&left);
    rect->SetLeft((Int32) mRenderCoordinates->FromLocalCssToPix(left));
    Int32 top;
    rect->GetTop(&top);
    rect->SetTop((Int32) mRenderCoordinates->FromLocalCssToPix(top));
    Int32 bottom;
    rect->GetBottom(&bottom);
    rect->SetBottom((Int32) mRenderCoordinates->FromLocalCssToPix(bottom));
    Int32 right;
    rect->GetRight(&right);
    rect->SetRight((Int32) mRenderCoordinates->FromLocalCssToPix(right));

    // Offset by the location of the web content within the view.
    rect->Offset(0,
                (Int32) mRenderCoordinates->GetContentOffsetYPix());

    // Finally offset by the location of the view within the screen.
    AutoPtr< ArrayOf<Int32> > viewLocation = ArrayOf<Int32>::Alloc(2);
    mView->GetLocationOnScreen(viewLocation);
    rect->Offset((*viewLocation)[0], (*viewLocation)[1]);

    node->SetBoundsInScreen(rect);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoKitKatAttributes(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Boolean canOpenPopup,
    /* [in] */ Boolean contentInvalid,
    /* [in] */ Boolean dismissable,
    /* [in] */ Boolean multiLine,
    /* [in] */ Int32 inputType,
    /* [in] */ Int32 liveRegion)
{
    // Requires KitKat or higher.
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoCollectionInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical)
{
    // Requires KitKat or higher.
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoCollectionItemInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading)
{
    // Requires KitKat or higher.
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityNodeInfoRangeInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    // Requires KitKat or higher.
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventBooleanAttributes(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean checked,
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean password,
    /* [in] */ Boolean scrollable)
{
    event->SetChecked(checked);
    event->SetEnabled(enabled);
    event->SetPassword(password);
    event->SetScrollable(scrollable);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventClassName(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ const String& className)
{
    event->SetClassName(className);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventListAttributes(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 currentItemIndex,
    /* [in] */ Int32 itemCount)
{
    event->SetCurrentItemIndex(currentItemIndex);
    event->SetItemCount(itemCount);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventScrollAttributes(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 maxScrollX,
    /* [in] */ Int32 maxScrollY)
{
    event.setScrollX(scrollX);
    event.setScrollY(scrollY);
    event.setMaxScrollX(maxScrollX);
    event.setMaxScrollY(maxScrollY);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventTextChangedAttrs(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 addedCount,
    /* [in] */ Int32 removedCount,
    /* [in] */ const String& beforeText,
    /* [in] */ const String& text)
{
    event->SetFromIndex(fromIndex);
    event->SetAddedCount(addedCount);
    event->SetRemovedCount(removedCount);
    event->SetBeforeText(beforeText);
    AutoPtr<IObjectContainer> container;
    event->GetText((IObjectContainer**)&container);
    container->Add(text);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventSelectionAttrs(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 addedCount,
    /* [in] */ Int32 itemCount,
    /* [in] */ const String& text)
{
    event->SetFromIndex(fromIndex);
    event->SetAddedCount(addedCount);
    event->SetItemCount(itemCount);
    AutoPtr<IObjectContainer> container;
    event->GetText((IObjectContainer**)&container);
    container->Add(text);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventKitKatAttributes(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean canOpenPopup,
    /* [in] */ Boolean contentInvalid,
    /* [in] */ Boolean dismissable,
    /* [in] */ Boolean multiLine,
    /* [in] */ Int32 inputType,
    /* [in] */ Int32 liveRegion)
{
    // Backwards compatibility for KitKat AccessibilityNodeInfo fields.
    AutoPtr<IBundle> bundle = GetOrCreateBundleForAccessibilityEvent(event);
    bundle->PutBoolean(String("AccessibilityNodeInfo.canOpenPopup"), canOpenPopup);
    bundle->PutBoolean(String("AccessibilityNodeInfo.contentInvalid"), contentInvalid);
    bundle->PutBoolean(String("AccessibilityNodeInfo.dismissable"), dismissable);
    bundle->PutBoolean(String("AccessibilityNodeInfo.multiLine"), multiLine);
    bundle->PutInt32(String("AccessibilityNodeInfo.inputType"), inputType);
    bundle->PutInt32(String("AccessibilityNodeInfo.liveRegion"), liveRegion);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventCollectionInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical)
{
    // Backwards compatibility for KitKat AccessibilityNodeInfo fields.
    AutoPtr<IBundle> bundle = GetOrCreateBundleForAccessibilityEvent(event);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionInfo.rowCount"), rowCount);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionInfo.columnCount"), columnCount);
    bundle->PutBoolean(String("AccessibilityNodeInfo.CollectionInfo.hierarchical"), hierarchical);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventHeadingFlag(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean heading)
{
    // Backwards compatibility for KitKat AccessibilityNodeInfo fields.
    AutoPtr<IBundle> bundle = GetOrCreateBundleForAccessibilityEvent(event);
    bundle->PutBoolean(String("AccessibilityNodeInfo.CollectionItemInfo.heading"), heading);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventCollectionItemInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan)
{
    // Backwards compatibility for KitKat AccessibilityNodeInfo fields.
    AutoPtr<IBundle> bundle = GetOrCreateBundleForAccessibilityEvent(event);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionItemInfo.rowIndex"), rowIndex);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionItemInfo.rowSpan"), rowSpan);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionItemInfo.columnIndex"), columnIndex);
    bundle->PutInt32(String("AccessibilityNodeInfo.CollectionItemInfo.columnSpan"), columnSpan);
}

//@CalledByNative
void BrowserAccessibilityManager::SetAccessibilityEventRangeInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    // Backwards compatibility for KitKat AccessibilityNodeInfo fields.
    AutoPtr<IBundle> bundle = GetOrCreateBundleForAccessibilityEvent(event);
    bundle->PutInt32(String("AccessibilityNodeInfo.RangeInfo.type"), rangeType);
    bundle->PutFloat(String("AccessibilityNodeInfo.RangeInfo.min"), min);
    bundle->PutFloat(String("AccessibilityNodeInfo.RangeInfo.max"), max);
    bundle->PutFloat(String("AccessibilityNodeInfo.RangeInfo.current"), current);
}

Int32 BrowserAccessibilityManager::NativeGetRootId(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid)
{
}

Boolean BrowserAccessibilityManager::NativeIsNodeValid(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 id)
{
}

void BrowserAccessibilityManager::NativeHitTest(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
}

Boolean BrowserAccessibilityManager::NativePopulateAccessibilityNodeInfo(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [in] */ Int32 id)
{
}

Boolean BrowserAccessibilityManager::NativePopulateAccessibilityEvent(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 id,
    /* [in] */ Int32 eventType)
{
}

void BrowserAccessibilityManager::NativeClick(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 id)
{
}

void BrowserAccessibilityManager::NativeFocus(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 id)
{
}

void BrowserAccessibilityManager::NativeBlur(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid)
{
}

void BrowserAccessibilityManager::NativeScrollToMakeNodeVisible(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 id)
{
}

Int32 BrowserAccessibilityManager::NativeFindElementType(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ Int32 startId,
    /* [in] */ const String& elementType,
    /* [in] */ Boolean forwards)
{
}

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
