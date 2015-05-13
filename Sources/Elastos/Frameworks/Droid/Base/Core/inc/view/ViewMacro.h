
#ifndef __VIEWMACRO_H__
#define __VIEWMACRO_H__

#include "ext/frameworkext.h"
#include "view/View.h"

using Elastos::Droid::Animation::ILayoutTransition;
using Elastos::Droid::Graphics::IInsets;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnFocusChangeListener;
using Elastos::Droid::View::IViewOnLayoutChangeListener;
using Elastos::Droid::View::IViewOnAttachStateChangeListener;
using Elastos::Droid::View::IViewOnFocusChangeListener;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::IViewGroupOnHierarchyChangeListener;
using Elastos::Droid::View::IViewOnCreateContextMenuListener;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::IViewOnGenericMotionListener;
using Elastos::Droid::View::IViewOnHoverListener;
using Elastos::Droid::View::IViewOnDragListener;
using Elastos::Droid::View::IViewOnSystemUiVisibilityChangeListener;
using Elastos::Droid::View::IAccessibilityDelegate;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::ITextSegmentIterator;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IDragEvent;
using Elastos::Droid::View::IContextMenu;
using Elastos::Droid::View::ITouchDelegate;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IDisplayList;
using Elastos::Droid::View::IDragShadowBuilder;
using Elastos::Droid::View::IViewPropertyAnimator;
using Elastos::Droid::View::IViewTreeObserver;

#define IVIEW_METHODS_DECL()                                            \
    CARAPI GetVerticalFadingEdgeLength(                                 \
        /* [out] */ Int32* length);                                     \
                                                                        \
    CARAPI SetFadingEdgeLength(                                         \
        /* [in] */ Int32 length);                                       \
                                                                        \
    CARAPI GetHorizontalFadingEdgeLength(                               \
        /* [out] */ Int32* length);                                     \
                                                                        \
    CARAPI GetVerticalScrollbarWidth(                                   \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI SetVerticalScrollbarPosition(                                \
        /* [in] */ Int32 position);                                     \
                                                                        \
    CARAPI GetVerticalScrollbarPosition(                                \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI GetHorizontalScrollbarHeight(                                \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI SetOnFocusChangeListener(                                    \
        /* [in] */ IViewOnFocusChangeListener* l);                      \
                                                                        \
    CARAPI AddOnLayoutChangeListener(                                   \
        /* [in] */ IViewOnLayoutChangeListener* listener);              \
                                                                        \
    CARAPI RemoveOnLayoutChangeListener(                                \
        /* [in] */ IViewOnLayoutChangeListener* listener);              \
                                                                        \
    CARAPI AddOnAttachStateChangeListener(                              \
        /* [in] */ IViewOnAttachStateChangeListener* listener);         \
                                                                        \
    CARAPI RemoveOnAttachStateChangeListener(                           \
        /* [in] */ IViewOnAttachStateChangeListener* listener);         \
                                                                        \
    CARAPI GetOnFocusChangeListener(                                    \
        /* [out] */ IViewOnFocusChangeListener** l);                    \
                                                                        \
    CARAPI SetOnClickListener(                                          \
        /* [in] */ IViewOnClickListener* l);                            \
                                                                        \
    CARAPI HasOnClickListeners(                                         \
        /* [out] */ Boolean* has);                                      \
                                                                        \
    CARAPI SetOnLongClickListener(                                      \
        /* [in] */ IViewOnLongClickListener* l);                        \
                                                                        \
    CARAPI SetOnCreateContextMenuListener(                              \
        /* [in] */ IViewOnCreateContextMenuListener* l);                \
                                                                        \
    CARAPI PerformClick(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CallOnClick(                                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PerformLongClick(                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI ShowContextMenu(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI ShowContextMenuEx(                                           \
        /* [in] */ Float x,                                             \
        /* [in] */ Float y,                                             \
        /* [in] */ Int32 metaState,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI StartActionMode(                                             \
        /* [in] */ IActionModeCallback* cback,                          \
        /* [out] */ IActionMode** mode);                                \
                                                                        \
    CARAPI SetOnKeyListener(                                            \
        /* [in] */ IViewOnKeyListener* l);                              \
                                                                        \
    CARAPI SetOnTouchListener(                                          \
        /* [in] */ IViewOnTouchListener* l);                            \
                                                                        \
    CARAPI SetOnGenericMotionListener(                                  \
        /* [in] */ IViewOnGenericMotionListener* l);                    \
                                                                        \
    CARAPI SetOnHoverListener(                                          \
        /* [in] */ IViewOnHoverListener* l);                            \
                                                                        \
    CARAPI SetOnDragListener(                                           \
        /* [in] */ IViewOnDragListener* l);                             \
                                                                        \
    CARAPI RequestRectangleOnScreen(                                    \
        /* [in] */ IRect* rectangle,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestRectangleOnScreenEx(                                  \
        /* [in] */ IRect* rectangle,                                    \
        /* [in] */ Boolean immediate,                                   \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI ClearFocus();                                                \
                                                                        \
    CARAPI HasFocus(                                                    \
        /* [out] */ Boolean* hasFocus);                                 \
                                                                        \
    CARAPI HasFocusable(                                                \
        /* [out] */ Boolean* hasFocusable);                             \
                                                                        \
    CARAPI AnnounceForAccessibility(                                    \
        /* [in] */ ICharSequence* text);                                \
                                                                        \
    CARAPI DispatchPopulateAccessibilityEvent(                          \
        /* [in] */ IAccessibilityEvent* event,                          \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnPopulateAccessibilityEvent(                                \
        /* [in] */ IAccessibilityEvent* event);                         \
                                                                        \
    CARAPI OnInitializeAccessibilityEvent(                              \
        /* [in] */ IAccessibilityEvent* event);                         \
                                                                        \
    CARAPI CreateAccessibilityNodeInfo(                                 \
        /* [out] */ IAccessibilityNodeInfo** info);                     \
                                                                        \
    CARAPI OnInitializeAccessibilityNodeInfo(                           \
        /* [in] */ IAccessibilityNodeInfo* info);                       \
                                                                        \
    CARAPI GetAccessibilityDelegate(                                    \
        /* [out] */ IAccessibilityDelegate** delegate);                 \
                                                                        \
    CARAPI SetAccessibilityDelegate(                                    \
        /* [in] */ IAccessibilityDelegate* delegate);                   \
                                                                        \
    CARAPI GetAccessibilityNodeProvider(                                \
        /* [out] */ IAccessibilityNodeProvider** provider);             \
                                                                        \
    CARAPI GetAccessibilityViewId(                                      \
        /* [out] */ Int32* id);                                         \
                                                                        \
    CARAPI GetAccessibilityWindowId(                                    \
        /* [out] */ Int32* id);                                         \
                                                                        \
    CARAPI GetLabelFor(                                                 \
        /* [out] */Int32* id);                                          \
                                                                        \
    CARAPI SetLabelFor(                                                 \
        /* [in] */Int32 id);                                            \
                                                                        \
    CARAPI GetContentDescription(                                       \
        /* [out] */ ICharSequence** description);                       \
                                                                        \
    CARAPI SetContentDescription(                                       \
        /* [in] */ ICharSequence* contentDescription);                  \
                                                                        \
    CARAPI IsFocused(                                                   \
        /* [out] */ Boolean* focused);                                  \
                                                                        \
    CARAPI FindFocus(                                                   \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI IsScrollContainer(                                           \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetScrollContainer(                                          \
        /* [in] */ Boolean isScrollContainer);                          \
                                                                        \
    CARAPI GetDrawingCacheQuality(                                      \
        /* [out] */ Int32* quality);                                    \
                                                                        \
    CARAPI SetDrawingCacheQuality(                                      \
        /* [in] */ Int32 quality);                                      \
                                                                        \
    CARAPI GetKeepScreenOn(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetKeepScreenOn(                                             \
        /* [in] */ Boolean keepScreenOn);                               \
                                                                        \
    CARAPI GetNextFocusLeftId(                                          \
        /* [out] */ Int32* nextFocusLeftId);                            \
                                                                        \
    CARAPI SetNextFocusLeftId(                                          \
        /* [in] */ Int32 nextFocusLeftId);                              \
                                                                        \
    CARAPI GetNextFocusRightId(                                         \
        /* [out] */ Int32* nextFocusRightId);                           \
                                                                        \
    CARAPI SetNextFocusRightId(                                         \
        /* [in] */ Int32 nextFocusRightId);                             \
                                                                        \
    CARAPI GetNextFocusUpId(                                            \
        /* [out] */ Int32* nextFocusUpId);                              \
                                                                        \
    CARAPI SetNextFocusUpId(                                            \
        /* [in] */ Int32 nextFocusUpId);                                \
                                                                        \
    CARAPI GetNextFocusDownId(                                          \
        /* [out] */ Int32* nextFocusDownId);                            \
                                                                        \
    CARAPI SetNextFocusDownId(                                          \
        /* [in] */ Int32 nextFocusDownId);                              \
                                                                        \
    CARAPI GetNextFocusForwardId(                                       \
        /* [out] */ Int32* id);                                         \
                                                                        \
    CARAPI SetNextFocusForwardId(                                       \
        /* [in] */ Int32 nextFocusForwardId);                           \
                                                                        \
    CARAPI IsShown(                                                     \
        /* [out] */ Boolean* shown);                                    \
                                                                        \
    CARAPI SetFitsSystemWindows(                                        \
        /* [in] */ Boolean fitSystemWindows);                           \
                                                                        \
    CARAPI GetFitsSystemWindows(                                        \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI FitsSystemWindows(                                           \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFitSystemWindows();                                   \
                                                                        \
    CARAPI MakeOptionalFitsSystemWindows();                             \
                                                                        \
    CARAPI GetVisibility(                                               \
        /* [out] */ Int32* visibility);                                 \
                                                                        \
    CARAPI SetVisibility(                                               \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI IsEnabled(                                                   \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetEnabled(                                                  \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI SetFocusable(                                                \
        /* [in] */ Boolean focusable);                                  \
                                                                        \
    CARAPI SetFocusableInTouchMode(                                     \
        /* [in] */ Boolean focusableInTouchMode);                       \
                                                                        \
    CARAPI SetSoundEffectsEnabled(                                      \
        /* [in] */ Boolean soundEffectsEnabled);                        \
                                                                        \
    CARAPI IsSoundEffectsEnabled(                                       \
        /* [out] */ Boolean* soundEffectsEnabled);                      \
                                                                        \
    CARAPI SetHapticFeedbackEnabled(                                    \
        /* [in] */ Boolean hapticFeedbackEnabled);                      \
                                                                        \
    CARAPI IsHapticFeedbackEnabled(                                     \
        /* [out] */ Boolean* hapticFeedbackEnabled);                    \
                                                                        \
    CARAPI GetRawLayoutDirection(                                       \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI SetLayoutDirection(                                          \
        /* [in] */ Int32 direction);                                    \
                                                                        \
    CARAPI GetLayoutDirection(                                          \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI IsLayoutRtl(                                                 \
        /* [out] */ Boolean* rtl);                                      \
                                                                        \
    CARAPI HasTransientState(                                           \
        /* [out] */ Boolean* has);                                      \
                                                                        \
    CARAPI SetHasTransientState(                                        \
        /* [in] */ Boolean hasTransientState);                          \
                                                                        \
    CARAPI SetWillNotDraw(                                              \
        /* [in] */ Boolean willNotDraw);                                \
                                                                        \
    CARAPI WillNotDraw(                                                 \
        /* [out] */ Boolean* willNotDraw);                              \
                                                                        \
    CARAPI SetWillNotCacheDrawing(                                      \
        /* [in] */ Boolean willNotCacheDrawing);                        \
                                                                        \
    CARAPI WillNotCacheDrawing(                                         \
        /* [out] */ Boolean* cache);                                    \
                                                                        \
    CARAPI IsClickable(                                                 \
        /* [out] */ Boolean* clickable);                                \
                                                                        \
    CARAPI SetClickable(                                                \
        /* [in] */ Boolean clickable);                                  \
                                                                        \
    CARAPI IsLongClickable(                                             \
        /* [out] */ Boolean* longClickable);                            \
                                                                        \
    CARAPI SetLongClickable(                                            \
        /* [in] */ Boolean longClickable);                              \
                                                                        \
    CARAPI SetPressed(                                                  \
        /* [in] */ Boolean pressed);                                    \
                                                                        \
    CARAPI IsPressed(                                                   \
        /* [out] */ Boolean* pressed);                                  \
                                                                        \
    CARAPI IsSaveEnabled(                                               \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetSaveEnabled(                                              \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI GetFilterTouchesWhenObscured(                                \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetFilterTouchesWhenObscured(                                \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsSaveFromParentEnabled(                                     \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetSaveFromParentEnabled(                                    \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsFocusable(                                                 \
        /* [out] */ Boolean* focusable);                                \
                                                                        \
    CARAPI IsFocusableInTouchMode(                                      \
        /* [out] */ Boolean* focusable);                                \
                                                                        \
    CARAPI FocusSearch(                                                 \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI DispatchUnhandledMove(                                       \
        /* [in] */ IView* focused,                                      \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetFocusables(                                               \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IObjectContainer** views);                          \
                                                                        \
    CARAPI AddFocusables(                                               \
        /* [in] */ IObjectContainer* views,                             \
        /* [in] */ Int32 direction);                                    \
                                                                        \
    CARAPI AddFocusablesEx(                                             \
        /* [in] */ IObjectContainer* views,                             \
        /* [in] */ Int32 direction,                                     \
        /* [in] */ Int32 focusableMode);                                \
                                                                        \
    CARAPI FindViewsWithText(                                           \
        /* [in, out] */ IObjectContainer* outViews,                     \
        /* [in] */ ICharSequence* searched,                             \
        /* [in] */ Int32 flags);                                        \
                                                                        \
    CARAPI GetTouchables(                                               \
        /* [out] */ IObjectContainer** views);                          \
                                                                        \
    CARAPI AddTouchables(                                               \
        /* [in] */ IObjectContainer* views);                            \
                                                                        \
    CARAPI RequestAccessibilityFocus(                                   \
        /* [out] */ Boolean* focus);                                    \
                                                                        \
    CARAPI ClearAccessibilityFocus();                                   \
                                                                        \
    CARAPI RequestFocus(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusEx(                                              \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusEx2(                                             \
        /* [in] */ Int32 direction,                                     \
        /* [in] */ IRect* previouslyFocusedRect,                        \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusFromTouch(                                       \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetImportantForAccessibility(                                \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetImportantForAccessibility(                                \
        /* [in] */ Int32 mode);                                         \
                                                                        \
    CARAPI IsImportantForAccessibility(                                 \
        /* [out] */ Boolean* exposed);                                  \
                                                                        \
    CARAPI GetParentForAccessibility(                                   \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI AddChildrenForAccessibility(                                 \
        /* [in] */ IObjectContainer* children);                         \
                                                                        \
    CARAPI IncludeForAccessibility(                                     \
        /* [out] */ Boolean* include);                                  \
                                                                        \
    CARAPI IsActionableForAccessibility(                                \
        /* [out] */ Boolean* actionable);                               \
                                                                        \
    CARAPI NotifyAccessibilityStateChanged();                           \
                                                                        \
    CARAPI ResetAccessibilityStateChanged();                            \
                                                                        \
    CARAPI PerformAccessibilityAction(                                  \
        /* [in] */ Int32 action,                                        \
        /* [in] */ IBundle* arguments,                                  \
        /* [out] */ Boolean* performed);                                \
                                                                        \
    CARAPI GetIterableTextForAccessibility(                             \
        /* [out] */ ICharSequence** text);                              \
                                                                        \
    CARAPI GetAccessibilityCursorPosition(                              \
        /* [out] */ Int32* pos);                                        \
                                                                        \
    CARAPI SetAccessibilityCursorPosition(                              \
        /* [in] */ Int32 position);                                     \
                                                                        \
    CARAPI GetIteratorForGranularity(                                   \
        /* [in] */ Int32 granularity,                                   \
        /* [out] */ ITextSegmentIterator** iterator);                   \
                                                                        \
    CARAPI DispatchStartTemporaryDetach();                              \
                                                                        \
    CARAPI OnStartTemporaryDetach();                                    \
                                                                        \
    CARAPI DispatchFinishTemporaryDetach();                             \
                                                                        \
    CARAPI OnFinishTemporaryDetach();                                   \
                                                                        \
    CARAPI GetKeyDispatcherState(                                       \
        /* [out] */ IDispatcherState** state);                          \
                                                                        \
    CARAPI DispatchKeyEventPreIme(                                      \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchKeyEvent(                                            \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchKeyShortcutEvent(                                    \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchTouchEvent(                                          \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnFilterTouchEventForSecurity(                               \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchTrackballEvent(                                      \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchGenericMotionEvent(                                  \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchPointerEvent(                                        \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchWindowFocusChanged(                                  \
        /* [in] */ Boolean hasFocus);                                   \
                                                                        \
    CARAPI OnWindowFocusChanged(                                        \
        /* [in] */ Boolean hasWindowFocus);                             \
                                                                        \
    CARAPI HasWindowFocus(                                              \
        /* [out] */ Boolean* focus);                                    \
                                                                        \
    CARAPI DispatchDisplayHint(                                         \
        /* [in] */ Int32 hint);                                         \
                                                                        \
    CARAPI DispatchWindowVisibilityChanged(                             \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI GetWindowVisibility(                                         \
        /* [out] */ Int32* visibility);                                 \
                                                                        \
    CARAPI GetWindowVisibleDisplayFrame(                                \
        /* [in] */ IRect* outRect);                                     \
                                                                        \
    CARAPI DispatchConfigurationChanged(                                \
        /* [in] */ IConfiguration* newConfig);                          \
                                                                        \
    CARAPI IsInTouchMode(                                               \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetContext(                                                  \
        /* [out] */ IContext** context);                                \
                                                                        \
    CARAPI OnKeyPreIme(                                                 \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyShortcut(                                               \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnCheckIsTextEditor(                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnCreateInputConnection(                                     \
        /* [in] */ IEditorInfo* outAttrs,                               \
        /* [out] */ IInputConnection** inputConnection);                \
                                                                        \
    CARAPI CheckInputConnectionProxy(                                   \
        /* [in] */ IView* view,                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CreateContextMenu(                                           \
        /* [in] */ IContextMenu* menu);                                 \
                                                                        \
    CARAPI OnTrackballEvent(                                            \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnGenericMotionEvent(                                        \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* handled);                                  \
                                                                        \
    CARAPI OnHoverEvent(                                                \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* handled);                                  \
                                                                        \
    CARAPI IsHovered(                                                   \
        /* [out] */ Boolean* hovered);                                  \
                                                                        \
    CARAPI SetHovered(                                                  \
        /* [in] */ Boolean hovered);                                    \
                                                                        \
    CARAPI OnHoverChanged(                                              \
        /* [in] */ Boolean hovered);                                    \
                                                                        \
    CARAPI OnTouchEvent(                                                \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI IsInScrollingContainer(                                      \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CancelLongPress();                                           \
                                                                        \
    CARAPI SetTouchDelegate(                                            \
        /* [in] */ ITouchDelegate* touchDelegate);                      \
                                                                        \
    CARAPI GetTouchDelegate(                                            \
        /* [out] */ ITouchDelegate** touchDelegate);                    \
                                                                        \
    CARAPI BringToFront();                                              \
                                                                        \
    CARAPI GetParent(                                                   \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI SetScrollX(                                                  \
        /* [in] */ Int32 value);                                        \
                                                                        \
    CARAPI SetScrollY(                                                  \
        /* [in] */ Int32 value);                                        \
                                                                        \
    CARAPI GetScrollX(                                                  \
        /* [out] */ Int32* x);                                          \
                                                                        \
    CARAPI GetScrollY(                                                  \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI GetWidth(                                                    \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetHeight(                                                   \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetDrawingRect(                                              \
        /* [in] */ IRect* outRect);                                     \
                                                                        \
    CARAPI GetMeasuredWidth(                                            \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetMeasuredWidthAndState(                                    \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetMeasuredHeight(                                           \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetMeasuredHeightAndState(                                   \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetMeasuredState(                                            \
        /* [out] */ Int32* state);                                      \
                                                                        \
    CARAPI GetMatrix(                                                   \
        /* [out] */ Elastos::Droid::Graphics::IMatrix** matrix);        \
                                                                        \
    CARAPI GetCameraDistance(                                           \
        /* [out] */ Float* distance);                                   \
                                                                        \
    CARAPI SetCameraDistance(                                           \
        /* [in] */ Float distance);                                     \
                                                                        \
    CARAPI GetRotation(                                                 \
        /* [out] */ Float* rotation);                                   \
                                                                        \
    CARAPI SetRotation(                                                 \
        /* [in] */ Float rotation);                                     \
                                                                        \
    CARAPI GetRotationY(                                                \
        /* [out] */ Float* y);                                          \
                                                                        \
    CARAPI SetRotationY(                                                \
        /* [in] */ Float rotationY);                                    \
                                                                        \
    CARAPI GetRotationX(                                                \
        /* [out] */ Float* x);                                          \
                                                                        \
    CARAPI SetRotationX(                                                \
        /* [in] */ Float rotationX);                                    \
                                                                        \
    CARAPI GetScaleX(                                                   \
        /* [out] */ Float* x);                                          \
                                                                        \
    CARAPI SetScaleX(                                                   \
        /* [in] */ Float scaleX);                                       \
                                                                        \
    CARAPI GetScaleY(                                                   \
        /* [out] */ Float* y);                                          \
                                                                        \
    CARAPI SetScaleY(                                                   \
        /* [in] */ Float scaleY);                                       \
                                                                        \
    CARAPI GetPivotX(                                                   \
        /* [out] */ Float* x);                                          \
                                                                        \
    CARAPI SetPivotX(                                                   \
        /* [in] */ Float pivotX);                                       \
                                                                        \
    CARAPI GetPivotY(                                                   \
        /* [out] */ Float* y);                                          \
                                                                        \
    CARAPI SetPivotY(                                                   \
        /* [in] */ Float pivotY);                                       \
                                                                        \
    CARAPI GetAlpha(                                                    \
        /* [out] */ Float* alpha);                                      \
                                                                        \
    CARAPI HasOverlappingRendering(                                     \
        /* [out] */ Boolean* overlap);                                  \
                                                                        \
    CARAPI SetAlpha(                                                    \
        /* [in] */ Float alpha);                                        \
                                                                        \
    CARAPI GetTop(                                                      \
        /* [out] */ Int32* top);                                        \
                                                                        \
    CARAPI SetTop(                                                      \
        /* [in] */ Int32 top);                                          \
                                                                        \
    CARAPI GetBottom(                                                   \
        /* [out] */ Int32* bottom);                                     \
                                                                        \
    CARAPI IsDirty(                                                     \
        /* [out] */ Boolean* dirty);                                    \
                                                                        \
    CARAPI SetBottom(                                                   \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI GetLeft(                                                     \
        /* [out] */ Int32* left);                                       \
                                                                        \
    CARAPI SetLeft(                                                     \
        /* [in] */ Int32 left);                                         \
                                                                        \
    CARAPI GetRight(                                                    \
        /* [out] */ Int32* right);                                      \
                                                                        \
    CARAPI SetRight(                                                    \
        /* [in] */ Int32 right);                                        \
                                                                        \
    CARAPI GetX(                                                        \
        /* [out] */ Float* x);                                          \
                                                                        \
    CARAPI SetX(                                                        \
        /* [in] */ Float x);                                            \
                                                                        \
    CARAPI GetY(                                                        \
        /* [out] */ Float* y);                                          \
                                                                        \
    CARAPI SetY(                                                        \
        /* [in] */ Float y);                                            \
                                                                        \
    CARAPI GetTranslationX(                                             \
        /* [out] */ Float* x);                                          \
                                                                        \
    CARAPI SetTranslationX(                                             \
        /* [in] */ Float translationX);                                 \
                                                                        \
    CARAPI GetTranslationY(                                             \
        /* [out] */ Float* y);                                          \
                                                                        \
    CARAPI SetTranslationY(                                             \
        /* [in] */ Float translationY);                                 \
                                                                        \
    CARAPI GetHitRect(                                                  \
        /* [in, out] */ IRect* outRect);                                \
                                                                        \
    CARAPI GetFocusedRect(                                              \
        /* [in, out] */ IRect* outRect);                                \
                                                                        \
    CARAPI GetGlobalVisibleRect(                                        \
        /* [in, out] */ IRect* outRect,                                 \
        /* [in, out] */ IPoint* outGlobalOffset,                        \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetGlobalVisibleRectEx(                                      \
        /* [in, out] */ IRect* outRect,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetLocalVisibleRect(                                         \
        /* [in, out] */ IRect* outRect,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OffsetTopAndBottom(                                          \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI OffsetLeftAndRight(                                          \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI GetLayoutParams(                                             \
        /* [out] */ IViewGroupLayoutParams** params);                   \
                                                                        \
    CARAPI SetLayoutParams(                                             \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI ResolveLayoutParams();                                       \
                                                                        \
    CARAPI ScrollTo(                                                    \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y);                                            \
                                                                        \
    CARAPI ScrollBy(                                                    \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y);                                            \
                                                                        \
    CARAPI Invalidate(                                                  \
        /* [in] */ IRect* dirty);                                       \
                                                                        \
    CARAPI InvalidateEx(                                                \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI InvalidateEx2();                                             \
                                                                        \
    CARAPI IsOpaque(                                                    \
        /* [out] */ Boolean* opaque);                                   \
                                                                        \
    CARAPI GetHandler(                                                  \
        /* [out] */ IHandler** apartment);                              \
                                                                        \
    CARAPI Post(                                                        \
        /* [in] */ IRunnable* action,                                   \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PostInvalidate();                                            \
                                                                        \
    CARAPI PostInvalidateEx(                                            \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI PostInvalidateDelayed(                                       \
        /* [in] */ Int64 delayMilliseconds);                            \
                                                                        \
    CARAPI PostInvalidateDelayedEx(                                     \
        /* [in] */ Int64 delayMilliseconds,                             \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI PostInvalidateOnAnimation();                                 \
                                                                        \
    CARAPI PostInvalidateOnAnimationEx(                                 \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI ComputeScroll();                                             \
                                                                        \
    CARAPI IsHorizontalFadingEdgeEnabled(                               \
        /* [out] */ Boolean* horizontalFadingEdgeEnabled);              \
                                                                        \
    CARAPI SetHorizontalFadingEdgeEnabled(                              \
        /* [in] */ Boolean horizontalFadingEdgeEnabled);                \
                                                                        \
    CARAPI IsVerticalFadingEdgeEnabled(                                 \
        /* [out] */ Boolean* verticalFadingEdgeEnabled);                \
                                                                        \
    CARAPI SetVerticalFadingEdgeEnabled(                                \
        /* [in] */ Boolean verticalFadingEdgeEnabled);                  \
                                                                        \
    CARAPI IsHorizontalScrollBarEnabled(                                \
        /* [out] */ Boolean* horizontalScrollBarEnabled);               \
                                                                        \
    CARAPI SetHorizontalScrollBarEnabled(                               \
        /* [in] */ Boolean horizontalScrollBarEnabled);                 \
                                                                        \
    CARAPI IsVerticalScrollBarEnabled(                                  \
        /* [out] */ Boolean* verticalScrollBarEnabled);                 \
                                                                        \
    CARAPI SetVerticalScrollBarEnabled(                                 \
        /* [in] */ Boolean verticalScrollBarEnabled);                   \
                                                                        \
    CARAPI SetScrollbarFadingEnabled(                                   \
        /* [in] */ Boolean fadeScrollbars);                             \
                                                                        \
    CARAPI IsScrollbarFadingEnabled(                                    \
        /* [out] */ Boolean* fadeScrollbars);                           \
                                                                        \
    CARAPI GetScrollBarDefaultDelayBeforeFade(                          \
        /* [out] */Int32* defaultDelay);                                \
                                                                        \
    CARAPI SetScrollBarDefaultDelayBeforeFade(                          \
        /* [in] */Int32 scrollBarDefaultDelayBeforeFade);               \
                                                                        \
    CARAPI GetScrollBarFadeDuration(                                    \
        /* [out] */Int32* duration);                                    \
                                                                        \
    CARAPI SetScrollBarFadeDuration(                                    \
        /* [in] */Int32 scrollBarFadeDuration);                         \
                                                                        \
    CARAPI GetScrollBarSize(                                            \
        /* [out] */Int32* size);                                        \
                                                                        \
    CARAPI SetScrollBarSize(                                            \
        /* [in] */Int32 scrollBarSize);                                 \
                                                                        \
    CARAPI SetScrollBarStyle(                                           \
        /* [in] */ Int32 style);                                        \
                                                                        \
    CARAPI GetScrollBarStyle(                                           \
        /* [out] */ Int32* style);                                      \
                                                                        \
    CARAPI CanScrollHorizontally(                                       \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* can);                                      \
                                                                        \
    CARAPI CanScrollVertically(                                         \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* can);                                      \
                                                                        \
    CARAPI ResolveRtlPropertiesIfNeeded();                              \
                                                                        \
    CARAPI ResetRtlProperties();                                        \
                                                                        \
    CARAPI OnScreenStateChanged(                                        \
        /* [in] */ Int32 screenState);                                  \
                                                                        \
    CARAPI OnRtlPropertiesChanged(                                      \
        /* [in] */ Int32 layoutDirection);                              \
                                                                        \
    CARAPI ResolveLayoutDirection(                                      \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CanResolveLayoutDirection(                                   \
        /* [out] */ Boolean*result);                                    \
                                                                        \
    CARAPI ResetResolvedLayoutDirection();                              \
                                                                        \
    CARAPI IsLayoutDirectionInherited(                                  \
        /* [out] */ Boolean*result);                                    \
                                                                        \
    CARAPI ResolvePadding();                                            \
                                                                        \
    CARAPI ResetResolvedPadding();                                      \
                                                                        \
    CARAPI AssignParent(                                                \
        /* [in] */ IViewParent* parent);                                \
                                                                        \
    CARAPI GetWindowToken(                                              \
        /* [out] */ IBinder** token);                                   \
                                                                        \
    CARAPI GetApplicationWindowToken(                                   \
        /* [out] */ IBinder** token);                                   \
                                                                        \
    CARAPI GetDisplay(                                                  \
        /* [out] */ IDisplay** display);                                \
                                                                        \
    CARAPI SaveHierarchyState(                                          \
        /* [in, out] */ IObjectInt32Map* container);                  \
                                                                        \
    CARAPI RestoreHierarchyState(                                       \
        /* [in] */ IObjectInt32Map* container);                       \
                                                                        \
    CARAPI GetDrawingTime(                                              \
        /* [out] */ Int64* time);                                       \
                                                                        \
    CARAPI SetDuplicateParentStateEnabled(                              \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsDuplicateParentStateEnabled(                               \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetLayerType(                                                \
        /* [in] */ Int32 layerType,                                     \
        /* [in] */ IPaint* paint);                                      \
                                                                        \
    CARAPI SetLayerPaint(                                               \
        /* [in] */ IPaint* paint);                                      \
                                                                        \
    CARAPI GetLayerType(                                                \
        /* [out] */ Int32* type);                                       \
                                                                        \
    CARAPI BuildLayer();                                                \
                                                                        \
    CARAPI SetDrawingCacheEnabled(                                      \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsDrawingCacheEnabled(                                       \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI CanHaveDisplayList(                                          \
        /* [out] */ Boolean* can);                                      \
                                                                        \
    CARAPI GetDisplayList(                                              \
        /* [out] */ IDisplayList** list);                               \
                                                                        \
    CARAPI GetDrawingCache(                                             \
        /* [out] */ IBitmap** bitmap);                                  \
                                                                        \
    CARAPI GetDrawingCacheEx(                                           \
        /* [in] */ Boolean autoScale,                                   \
        /* [out] */ IBitmap** bitmap);                                  \
                                                                        \
    CARAPI DestroyDrawingCache();                                       \
                                                                        \
    CARAPI SetDrawingCacheBackgroundColor(                              \
        /* [in] */ Int32 color);                                        \
                                                                        \
    CARAPI GetDrawingCacheBackgroundColor(                              \
        /* [out] */ Int32* color);                                      \
                                                                        \
    CARAPI BuildDrawingCache();                                         \
                                                                        \
    CARAPI BuildDrawingCacheEx(                                         \
        /* [in] */ Boolean autoScale);                                  \
                                                                        \
    CARAPI CreateSnapshot(                                              \
        /* [in] */ BitmapConfig quality,                                \
        /* [in] */ Int32 backgroundColor,                               \
        /* [in] */ Boolean skipChildren,                                \
        /* [out] */ IBitmap** bitmap);                                  \
                                                                        \
    CARAPI IsInEditMode(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI IsHardwareAccelerated(                                       \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI Draw(                                                        \
        /* [in] */ ICanvas* canvas);                                    \
                                                                        \
    CARAPI GetSolidColor(                                               \
        /* [out] */ Int32* color);                                      \
                                                                        \
    CARAPI OnFinishInflate();                                           \
                                                                        \
    CARAPI IsLayoutRequested(                                           \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI Layout(                                                      \
        /* [in] */ Int32 l,                                             \
        /* [in] */ Int32 t,                                             \
        /* [in] */ Int32 r,                                             \
        /* [in] */ Int32 b);                                            \
                                                                        \
    CARAPI GetResources(                                                \
        /* [out] */ IResources** resources);                            \
                                                                        \
    CARAPI UnscheduleDrawableEx(                                        \
        /* [in] */ IDrawable* who);                                     \
                                                                        \
    CARAPI OnResolveDrawables(                                          \
        /* [in] */ Int64 layoutDirection);                              \
                                                                        \
    CARAPI RefreshDrawableState();                                      \
                                                                        \
    CARAPI GetDrawableState(                                            \
        /* [out, callee] */ ArrayOf<Int32>** state);                    \
                                                                        \
    CARAPI JumpDrawablesToCurrentState();                               \
                                                                        \
    CARAPI SetBackgroundColor(                                          \
        /* [in] */ Int32 color);                                        \
                                                                        \
    CARAPI SetBackgroundResource(                                       \
        /* [in] */ Int32 resid);                                        \
                                                                        \
    CARAPI SetBackground(                                               \
        /* [in] */ IDrawable* background);                              \
                                                                        \
    CARAPI SetBackgroundDrawable(                                       \
        /* [in] */ IDrawable* d);                                       \
                                                                        \
    CARAPI GetBackground(                                               \
        /* [out] */ IDrawable** drawable);                              \
                                                                        \
    CARAPI SetPadding(                                                  \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI SetPaddingRelative(                                          \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 end,                                           \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI GetPaddingTop(                                               \
        /* [out] */ Int32* top);                                        \
                                                                        \
    CARAPI GetPaddingBottom(                                            \
        /* [out] */ Int32* bottom);                                     \
                                                                        \
    CARAPI GetPaddingLeft(                                              \
        /* [out] */ Int32* left);                                       \
                                                                        \
    CARAPI GetPaddingStart(                                             \
        /* [out] */ Int32* start);                                      \
                                                                        \
    CARAPI GetPaddingRight(                                             \
        /* [out] */ Int32* right);                                      \
                                                                        \
    CARAPI GetPaddingEnd(                                               \
        /* [out] */ Int32* end);                                        \
                                                                        \
    CARAPI IsPaddingRelative(                                           \
        /* [out] */ Boolean* relative);                                 \
                                                                        \
    CARAPI ResetPaddingToInitialValues();                               \
                                                                        \
    CARAPI GetOpticalInsets(                                            \
        /* [out] */ IInsets** sets);                                    \
                                                                        \
    CARAPI SetLayoutInsets(                                             \
        /* [in] */ IInsets* layoutInsets);                              \
                                                                        \
    CARAPI SetSelected(                                                 \
        /* [in] */ Boolean selected);                                   \
                                                                        \
    CARAPI IsSelected(                                                  \
        /* [out] */ Boolean* selected);                                 \
                                                                        \
    CARAPI SetActivated(                                                \
        /* [in] */ Boolean activated);                                  \
                                                                        \
    CARAPI IsActivated(                                                 \
        /* [out] */ Boolean* activated);                                \
                                                                        \
    CARAPI GetViewTreeObserver(                                         \
        /* [out] */ IViewTreeObserver** observer);                      \
                                                                        \
    CARAPI GetRootView(                                                 \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI GetLocationOnScreen(                                         \
        /* [out] */ Int32* x,                                           \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI GetLocationInWindow(                                         \
        /* [out] */ Int32* x,                                           \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI FindViewById(                                                \
        /* [in] */ Int32 id,                                            \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI SetId(                                                       \
        /* [in] */ Int32 id);                                           \
                                                                        \
    CARAPI FindViewByPredicate(                                         \
        /* [in] */ IPredicate* predicate,                               \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI FindViewByPredicateInsideOut(                                \
        /* [in] */ IView* start,                                        \
        /* [in] */ IPredicate* predicate,                               \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI GetId(                                                       \
        /* [out] */ Int32* id);                                         \
                                                                        \
    CARAPI IsRootNamespace(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetBaseline(                                                 \
        /* [out] */ Int32* result);                                     \
                                                                        \
    CARAPI RequestLayout();                                             \
                                                                        \
    CARAPI ForceLayout();                                               \
                                                                        \
    CARAPI Measure(                                                     \
        /* [in] */ Int32 widthMeasureSpec,                              \
        /* [in] */ Int32 heightMeasureSpec);                            \
                                                                        \
    CARAPI GetMinimumHeight(                                            \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI SetMinimumHeight(                                            \
        /* [in] */ Int32 minHeight);                                    \
                                                                        \
    CARAPI GetMinimumWidth(                                             \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI SetMinimumWidth(                                             \
        /* [in] */ Int32 minWidth);                                     \
                                                                        \
    CARAPI GetAnimation(                                                \
        /* [out] */ IAnimation** animation);                            \
                                                                        \
    CARAPI StartAnimation(                                              \
        /* [in] */ IAnimation* animation);                              \
                                                                        \
    CARAPI ClearAnimation();                                            \
                                                                        \
    CARAPI SetAnimation(                                                \
        /* [in] */ IAnimation* animation);                              \
                                                                        \
    CARAPI GatherTransparentRegion(                                     \
        /* [in] */ IRegion* region,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PlaySoundEffect(                                             \
        /* [in] */ Int32 soundConstant);                                \
                                                                        \
    CARAPI PerformHapticFeedback(                                       \
        /* [in] */ Int32 feedbackConstant,                              \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PerformHapticFeedbackEx(                                     \
        /* [in] */ Int32 feedbackConstant,                              \
        /* [in] */ Int32 flags,                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetSystemUiVisibility(                                       \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI GetSystemUiVisibility(                                       \
        /* [in] */ Int32* value);                                       \
                                                                        \
    CARAPI GetWindowSystemUiVisibility(                                 \
        /* [in] */ Int32* value);                                       \
                                                                        \
    CARAPI OnWindowSystemUiVisibilityChanged(                           \
        /* [in] */ Int32 visible);                                      \
                                                                        \
    CARAPI DispatchWindowSystemUiVisiblityChanged(                      \
        /* [in] */ Int32 visible);                                      \
                                                                        \
    CARAPI SetOnSystemUiVisibilityChangeListener(                       \
        /* [in] */ IViewOnSystemUiVisibilityChangeListener* l);         \
                                                                        \
    CARAPI DispatchSystemUiVisibilityChanged(                           \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI SetDisabledSystemUiVisibility(                               \
        /* [in] */ Int32 flags);                                        \
                                                                        \
    CARAPI StartDrag(                                                   \
        /* [in] */ IClipData* data,                                     \
        /* [in] */ IDragShadowBuilder* shadowBuilder,                   \
        /* [in] */ IInterface* myLocalState,                            \
        /* [in] */ Int32 flags,                                         \
        /* [in] */ Boolean* value);                                     \
                                                                        \
    CARAPI OnDragEvent(                                                 \
        /* [in] */ IDragEvent* event,                                   \
        /* [in] */ Boolean* value);                                     \
                                                                        \
    CARAPI DispatchDragEvent(                                           \
        /* [in] */ IDragEvent* event,                                   \
        /* [in] */ Boolean* value);                                     \
                                                                        \
    CARAPI OnCloseSystemDialogs(                                        \
        /* [in] */ const String& reason);                               \
                                                                        \
    CARAPI ApplyDrawableToTransparentRegion(                            \
        /* [in] */ IDrawable* dr,                                       \
        /* [in] */ IRegion* region);                                    \
                                                                        \
    CARAPI GetOverScrollMode(                                           \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetOverScrollMode(                                           \
        /* [in] */ Int32 overScrollMode);                               \
                                                                        \
    CARAPI GetRawTextDirection(                                         \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI SetTextDirection(                                            \
        /* [in] */ Int32 textDirection);                                \
                                                                        \
    CARAPI GetTextDirection(                                            \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI ResolveTextDirection(                                        \
        /* [out] */ Boolean* done);                                     \
                                                                        \
    CARAPI ResetResolvedTextDirection();                                \
                                                                        \
    CARAPI IsTextDirectionInherited(                                    \
        /* [out] */ Boolean* inherited);                                \
                                                                        \
    CARAPI GetRawTextAlignment(                                         \
        /* [out] */ Int32* textAlignment);                              \
                                                                        \
    CARAPI SetTextAlignment(                                            \
        /* [in] */ Int32 textAlignment);                                \
                                                                        \
    CARAPI GetTextAlignment(                                            \
        /* [out] */ Int32* alignment);                                  \
                                                                        \
    CARAPI ResolveTextAlignment(                                        \
        /* [out] */ Boolean* done);                                     \
                                                                        \
    CARAPI ResetResolvedTextAlignment();                                \
                                                                        \
    CARAPI IsTextAlignmentInherited(                                    \
        /* [out] */ Boolean* inherited);                                \
                                                                        \
    CARAPI SetTag(                                                      \
        /* [in] */ IInterface* tag);                                    \
                                                                        \
    CARAPI GetTag(                                                      \
        /* [out] */ IInterface** tag);                                  \
                                                                        \
    CARAPI HackTurnOffWindowResizeAnim(                                 \
        /* [in] */ Boolean off);                                        \
                                                                        \
    CARAPI Animate(                                                     \
        /* [out] */ IViewPropertyAnimator** animator);                  \
                                                                        \
    CARAPI GetInflaterContext(                                          \
        /* [out] */ IContext** context);                                \
                                                                        \
    CARAPI SetInflaterContext(                                          \
        /* [in] */ IContext* context);                                  \
                                                                        \
    CARAPI GetXmlPath(                                                  \
        /* [out] */ String* path);                                      \
                                                                        \
    CARAPI SetXmlPath(                                                  \
        /* [in] */ const String& path);                                 \
                                                                        \
    CARAPI SetTagEx(                                                    \
        /* [in] */ Int32 key,                                           \
        /* [in] */ IInterface* tag);                                    \
                                                                        \
    CARAPI GetTagEx(                                                    \
        /* [in] */ Int32 key,                                           \
        /* [out] */ IInterface** tag);                                  \
                                                                        \
    CARAPI SetTagInternal(                                              \
        /* [in] */ Int32 key,                                           \
        /* [in] */ IInterface* tag);                                    \

#define IVIEW_METHODS_IMPL(className, superClass)                       \
ECode className::GetVerticalFadingEdgeLength(                           \
    /* [out] */ Int32* length)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(length);                                          \
    *length = superClass::GetVerticalFadingEdgeLength();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFadingEdgeLength(                                   \
    /* [in] */ Int32 length)                                            \
{                                                                       \
    return superClass::SetFadingEdgeLength(length);                     \
}                                                                       \
                                                                        \
ECode className::GetHorizontalFadingEdgeLength(                         \
    /* [out] */ Int32* length)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(length);                                          \
    *length = superClass::GetHorizontalFadingEdgeLength();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetVerticalScrollbarWidth(                             \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
                                                                        \
    *width = superClass::GetVerticalScrollbarWidth();                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVerticalScrollbarPosition(                          \
    /* [in] */ Int32 position)                                          \
{                                                                       \
    return superClass::SetVerticalScrollbarPosition(position);          \
}                                                                       \
                                                                        \
ECode className::GetVerticalScrollbarPosition(                          \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
                                                                        \
    *position = superClass::GetVerticalScrollbarPosition();             \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHorizontalScrollbarHeight(                          \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
                                                                        \
    *height = superClass::GetHorizontalScrollbarHeight();               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnFocusChangeListener(                              \
    /* [in] */ IViewOnFocusChangeListener* l)                           \
{                                                                       \
    return superClass::SetOnFocusChangeListener(l);                     \
}                                                                       \
                                                                        \
ECode className::AddOnLayoutChangeListener(                             \
    /* [in] */ IViewOnLayoutChangeListener* l)                          \
{                                                                       \
    return superClass::AddOnLayoutChangeListener(l);                    \
}                                                                       \
                                                                        \
ECode className::RemoveOnLayoutChangeListener(                          \
    /* [in] */ IViewOnLayoutChangeListener* l)                          \
{                                                                       \
    return superClass::RemoveOnLayoutChangeListener(l);                 \
}                                                                       \
                                                                        \
ECode className::AddOnAttachStateChangeListener(                        \
    /* [in] */ IViewOnAttachStateChangeListener* l)                     \
{                                                                       \
    return superClass::AddOnAttachStateChangeListener(l);               \
}                                                                       \
                                                                        \
ECode className::RemoveOnAttachStateChangeListener(                     \
    /* [in] */ IViewOnAttachStateChangeListener* l)                     \
{                                                                       \
    return superClass::RemoveOnAttachStateChangeListener(l);            \
}                                                                       \
                                                                        \
ECode className::GetOnFocusChangeListener(                              \
    /* [out] */ IViewOnFocusChangeListener** l)                         \
{                                                                       \
    return superClass::GetOnFocusChangeListener(l);                     \
}                                                                       \
                                                                        \
ECode className::SetOnClickListener(                                    \
    /* [in] */ IViewOnClickListener* l)                                 \
{                                                                       \
    return superClass::SetOnClickListener(l);                           \
}                                                                       \
                                                                        \
ECode className::HasOnClickListeners(                                   \
    /* [out] */ Boolean* has)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(has);                                             \
    *has = superClass::HasOnClickListeners();                           \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnLongClickListener(                                \
    /* [in] */ IViewOnLongClickListener* l)                             \
{                                                                       \
    return superClass::SetOnLongClickListener(l);                       \
}                                                                       \
                                                                        \
ECode className::SetOnCreateContextMenuListener(                        \
    /* [in] */ IViewOnCreateContextMenuListener* l)                     \
{                                                                       \
    return superClass::SetOnCreateContextMenuListener(l);               \
}                                                                       \
                                                                        \
ECode className::PerformClick(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformClick();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CallOnClick(                                           \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::CallOnClick();                                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PerformLongClick(                                      \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformLongClick();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ShowContextMenu(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ShowContextMenu();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ShowContextMenuEx(                                     \
    /* [in] */ Float x,                                                 \
    /* [in] */ Float y,                                                 \
    /* [in] */ Int32 metaState,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ShowContextMenu(x, y, metaState);             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::StartActionMode(                                       \
    /* [in] */ IActionModeCallback* cback,                              \
    /* [out] */ IActionMode** mode)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    AutoPtr<IActionMode> tmMode = superClass::StartActionMode(cback);   \
    *mode = tmMode;                                                     \
    if (*mode) {                                                        \
        (*mode)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnKeyListener(                                      \
    /* [in] */ IViewOnKeyListener* l)                                   \
{                                                                       \
    return superClass::SetOnKeyListener(l);                             \
}                                                                       \
                                                                        \
ECode className::SetOnTouchListener(                                    \
    /* [in] */ IViewOnTouchListener* l)                                 \
{                                                                       \
    return superClass::SetOnTouchListener(l);                           \
}                                                                       \
                                                                        \
ECode className::SetOnGenericMotionListener(                            \
    /* [in] */ IViewOnGenericMotionListener* l)                         \
{                                                                       \
    return superClass::SetOnGenericMotionListener(l);                   \
}                                                                       \
                                                                        \
ECode className::SetOnHoverListener(                                    \
    /* [in] */ IViewOnHoverListener* l)                                 \
{                                                                       \
    return superClass::SetOnHoverListener(l);                           \
}                                                                       \
                                                                        \
ECode className::SetOnDragListener(                                     \
    /* [in] */ IViewOnDragListener* l)                                  \
{                                                                       \
    return superClass::SetOnDragListener(l);                            \
}                                                                       \
                                                                        \
ECode className::RequestRectangleOnScreen(                              \
    /* [in] */ IRect* rectangle,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestRectangleOnScreen(rectangle);          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestRectangleOnScreenEx(                            \
    /* [in] */ IRect* rectangle,                                        \
    /* [in] */ Boolean immediate,                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestRectangleOnScreen(rectangle, immediate);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ClearFocus()                                           \
{                                                                       \
    return superClass::ClearFocus();                                    \
}                                                                       \
                                                                        \
ECode className::HasFocus(                                              \
    /* [out] */ Boolean* hasFocus)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(hasFocus);                                        \
                                                                        \
    *hasFocus = superClass::HasFocus();                                 \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasFocusable(                                          \
    /* [out] */ Boolean* hasFocusable)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(hasFocusable);                                    \
    *hasFocusable = superClass::HasFocusable();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AnnounceForAccessibility(                              \
    /* [in] */ ICharSequence* text)                                     \
{                                                                       \
    return superClass::AnnounceForAccessibility(text);                  \
}                                                                       \
                                                                        \
ECode className::DispatchPopulateAccessibilityEvent(                    \
    /* [in] */ IAccessibilityEvent* event,                              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchPopulateAccessibilityEvent(event);    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnPopulateAccessibilityEvent(                          \
    /* [in] */ IAccessibilityEvent* event)                              \
{                                                                       \
    return superClass::OnPopulateAccessibilityEvent(event);             \
}                                                                       \
                                                                        \
ECode className::OnInitializeAccessibilityEvent(                        \
    /* [in] */ IAccessibilityEvent* event)                              \
{                                                                       \
    return superClass::OnInitializeAccessibilityEvent(event);           \
}                                                                       \
                                                                        \
ECode className::CreateAccessibilityNodeInfo(                           \
    /* [out] */ IAccessibilityNodeInfo** info)                          \
{                                                                       \
    VALIDATE_NOT_NULL(info);                                            \
    AutoPtr<IAccessibilityNodeInfo> tmp = superClass::CreateAccessibilityNodeInfo(); \
    *info = tmp;                                                        \
    if (*info != NULL) {                                                \
        (*info)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnInitializeAccessibilityNodeInfo(                     \
    /* [in] */ IAccessibilityNodeInfo* info)                            \
{                                                                       \
    return superClass::OnInitializeAccessibilityNodeInfo(info);         \
}                                                                       \
                                                                        \
ECode className::GetAccessibilityDelegate(                              \
    /* [out] */ IAccessibilityDelegate** delegate)                      \
{                                                                       \
    VALIDATE_NOT_NULL(delegate);                                        \
    AutoPtr<IAccessibilityDelegate> tmp = superClass::GetAccessibilityDelegate(); \
    *delegate = tmp;                                                    \
    if (*delegate != NULL) {                                            \
        (*delegate)->AddRef();                                          \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAccessibilityDelegate(                              \
    /* [in] */ IAccessibilityDelegate* delegate)                        \
{                                                                       \
    return superClass::SetAccessibilityDelegate(delegate);              \
}                                                                       \
                                                                        \
ECode className::GetAccessibilityNodeProvider(                          \
    /* [out] */ IAccessibilityNodeProvider** provider)                  \
{                                                                       \
    VALIDATE_NOT_NULL(provider);                                        \
    AutoPtr<IAccessibilityNodeProvider> tmp = superClass::GetAccessibilityNodeProvider(); \
    *provider = tmp;                                                    \
    if (*provider != NULL) {                                            \
        (*provider)->AddRef();                                          \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetAccessibilityViewId(                                \
    /* [out] */ Int32* id)                                              \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
    *id = superClass::GetAccessibilityViewId();                         \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetAccessibilityWindowId(                              \
    /* [out] */ Int32* id)                                              \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
    *id = superClass::GetAccessibilityViewId();                         \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLabelFor(                                           \
    /* [out] */Int32* id)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
    *id = superClass::GetLabelFor();                                    \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLabelFor(                                           \
    /* [in] */Int32 id)                                                 \
{                                                                       \
    return superClass::SetLabelFor(id);                                 \
}                                                                       \
                                                                        \
ECode className::GetContentDescription(                                 \
    /* [out] */ ICharSequence** description)                            \
{                                                                       \
    VALIDATE_NOT_NULL(description);                                     \
    AutoPtr<ICharSequence> tmp = superClass::GetContentDescription();   \
    *description = tmp;                                                 \
    if (*description != NULL) {                                         \
        (*description)->AddRef();                                       \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetContentDescription(                                 \
    /* [in] */ ICharSequence* contentDescription)                       \
{                                                                       \
    return superClass::SetContentDescription(contentDescription);       \
}                                                                       \
                                                                        \
ECode className::IsFocused(                                             \
    /* [out] */ Boolean* focused)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(focused);                                         \
    *focused = superClass::IsFocused();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FindFocus(                                             \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> tmp = superClass::FindFocus();                       \
    *view = tmp.Get();                                                  \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsScrollContainer(                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsScrollContainer();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollContainer(                                    \
    /* [in] */ Boolean isScrollContainer)                               \
{                                                                       \
    return superClass::SetScrollContainer(isScrollContainer);           \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheQuality(                                \
    /* [out] */ Int32* quality)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(quality);                                         \
    *quality = superClass::GetDrawingCacheQuality();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheQuality(                                \
    /* [in] */ Int32 quality)                                           \
{                                                                       \
    return superClass::SetDrawingCacheQuality(quality);                 \
}                                                                       \
                                                                        \
ECode className::GetKeepScreenOn(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetKeepScreenOn();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetKeepScreenOn(                                       \
    /* [in] */ Boolean keepScreenOn)                                    \
{                                                                       \
    return superClass::SetKeepScreenOn(keepScreenOn);                   \
}                                                                       \
                                                                        \
ECode className::GetNextFocusLeftId(                                    \
    /* [out] */ Int32* nextFocusLeftId)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusLeftId);                                 \
    *nextFocusLeftId = superClass::GetNextFocusLeftId();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusLeftId(                                    \
    /* [in] */ Int32 nextFocusLeftId)                                   \
{                                                                       \
    return superClass::SetNextFocusLeftId(nextFocusLeftId);             \
}                                                                       \
                                                                        \
ECode className::GetNextFocusRightId(                                   \
    /* [out] */ Int32* nextFocusRightId)                                \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusRightId);                                \
    *nextFocusRightId = superClass::GetNextFocusRightId();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusRightId(                                   \
    /* [in] */ Int32 nextFocusRightId)                                  \
{                                                                       \
    return superClass::SetNextFocusRightId(nextFocusRightId);           \
}                                                                       \
                                                                        \
ECode className::GetNextFocusUpId(                                      \
    /* [out] */ Int32* nextFocusUpId)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusUpId);                                   \
    *nextFocusUpId = superClass::GetNextFocusUpId();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusUpId(                                      \
    /* [in] */ Int32 nextFocusUpId)                                     \
{                                                                       \
    return superClass::SetNextFocusUpId(nextFocusUpId);                 \
}                                                                       \
                                                                        \
ECode className::GetNextFocusDownId(                                    \
    /* [out] */ Int32* nextFocusDownId)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusDownId);                                 \
    *nextFocusDownId = superClass::GetNextFocusDownId();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusDownId(                                    \
    /* [in] */ Int32 nextFocusDownId)                                   \
{                                                                       \
    return superClass::SetNextFocusDownId(nextFocusDownId);             \
}                                                                       \
                                                                        \
ECode className::GetNextFocusForwardId(                                 \
    /* [out] */ Int32* id)                                              \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
    *id = superClass::GetNextFocusForwardId();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusForwardId(                                 \
    /* [in] */ Int32 nextFocusForwardId)                                \
{                                                                       \
    return superClass::SetNextFocusForwardId(nextFocusForwardId);       \
}                                                                       \
                                                                        \
ECode className::IsShown(                                               \
    /* [out] */ Boolean* shown)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(shown);                                           \
    *shown = superClass::IsShown();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFitsSystemWindows(                                  \
    /* [in] */ Boolean fitSystemWindows)                                \
{                                                                       \
    return superClass::SetFitsSystemWindows(fitSystemWindows);          \
}                                                                       \
                                                                        \
ECode className::GetFitsSystemWindows(                                  \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetFitsSystemWindows();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FitsSystemWindows(                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::FitsSystemWindows();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFitSystemWindows()                              \
{                                                                       \
    return superClass::RequestFitSystemWindows();                       \
}                                                                       \
                                                                        \
ECode className::MakeOptionalFitsSystemWindows()                        \
{                                                                       \
    return superClass::MakeOptionalFitsSystemWindows();                 \
}                                                                       \
                                                                        \
ECode className::GetVisibility(                                         \
    /* [out] */ Int32* visibility)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(visibility);                                      \
                                                                        \
    *visibility = superClass::GetVisibility();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVisibility(                                         \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::SetVisibility(visibility);                       \
}                                                                       \
                                                                        \
ECode className::IsEnabled(                                             \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsEnabled();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetEnabled(                                            \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetEnabled(enabled);                             \
}                                                                       \
                                                                        \
ECode className::SetFocusable(                                          \
    /* [in] */ Boolean focusable)                                       \
{                                                                       \
    return superClass::SetFocusable(focusable);                         \
}                                                                       \
                                                                        \
ECode className::SetFocusableInTouchMode(                               \
    /* [in] */ Boolean focusableInTouchMode)                            \
{                                                                       \
    return superClass::SetFocusableInTouchMode(focusableInTouchMode);   \
}                                                                       \
                                                                        \
ECode className::SetSoundEffectsEnabled(                                \
    /* [in] */ Boolean soundEffectsEnabled)                             \
{                                                                       \
    return superClass::SetSoundEffectsEnabled(soundEffectsEnabled);     \
}                                                                       \
                                                                        \
ECode className::IsSoundEffectsEnabled(                                 \
    /* [out] */ Boolean* soundEffectsEnabled)                           \
{                                                                       \
    VALIDATE_NOT_NULL(soundEffectsEnabled);                             \
    *soundEffectsEnabled = superClass::IsSoundEffectsEnabled();         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHapticFeedbackEnabled(                              \
    /* [in] */ Boolean hapticFeedbackEnabled)                           \
{                                                                       \
    return superClass::SetHapticFeedbackEnabled(hapticFeedbackEnabled); \
}                                                                       \
                                                                        \
ECode className::IsHapticFeedbackEnabled(                               \
    /* [out] */ Boolean* hapticFeedbackEnabled)                         \
{                                                                       \
    VALIDATE_NOT_NULL(hapticFeedbackEnabled);                           \
    *hapticFeedbackEnabled = superClass::IsHapticFeedbackEnabled();     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRawLayoutDirection(                                 \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetRawLayoutDirection();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutDirection(                                    \
    /* [in] */ Int32 layoutDirection)                                   \
{                                                                       \
    return superClass::SetLayoutDirection(layoutDirection);             \
}                                                                       \
                                                                        \
ECode className::GetLayoutDirection(                                    \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetLayoutDirection();                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsLayoutRtl(                                           \
    /* [out] */ Boolean* rtl)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(rtl);                                             \
    *rtl = superClass::IsLayoutRtl();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasTransientState(                                     \
    /* [out] */ Boolean* has)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(has);                                             \
    *has = superClass::HasTransientState();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHasTransientState(                                  \
    /* [in] */ Boolean hasTransientState)                               \
{                                                                       \
    return superClass::SetHasTransientState(hasTransientState);         \
}                                                                       \
                                                                        \
ECode className::SetWillNotDraw(                                        \
    /* [in] */ Boolean willNotDraw)                                     \
{                                                                       \
    return superClass::SetWillNotDraw(willNotDraw);                     \
}                                                                       \
                                                                        \
ECode className::WillNotDraw(                                           \
    /* [out] */ Boolean* willNotDraw)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(willNotDraw);                                     \
    *willNotDraw = superClass::WillNotDraw();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetWillNotCacheDrawing(                                \
    /* [in] */ Boolean willNotCacheDrawing)                             \
{                                                                       \
    return superClass::SetWillNotCacheDrawing(willNotCacheDrawing);     \
}                                                                       \
                                                                        \
ECode className::WillNotCacheDrawing(                                   \
    /* [out] */ Boolean* cache)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(cache);                                           \
    *cache = superClass::WillNotCacheDrawing();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsClickable(                                           \
    /* [out] */ Boolean* clickable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(clickable);                                       \
    *clickable = superClass::IsClickable();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClickable(                                          \
    /* [in] */ Boolean clickable)                                       \
{                                                                       \
    return superClass::SetClickable(clickable);                         \
}                                                                       \
                                                                        \
ECode className::IsLongClickable(                                       \
    /* [out] */ Boolean* longClickable)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(longClickable);                                   \
    *longClickable = superClass::IsLongClickable();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLongClickable(                                      \
    /* [in] */ Boolean longClickable)                                   \
{                                                                       \
    return superClass::SetLongClickable(longClickable);                 \
}                                                                       \
                                                                        \
ECode className::SetPressed(                                            \
    /* [in] */ Boolean pressed)                                         \
{                                                                       \
    return superClass::SetPressed(pressed);                             \
}                                                                       \
                                                                        \
ECode className::IsPressed(                                             \
    /* [out] */ Boolean* pressed)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(pressed);                                         \
    *pressed = superClass::IsPressed();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsSaveEnabled(                                         \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsSaveEnabled();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSaveEnabled(                                        \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetSaveEnabled(enabled);                         \
}                                                                       \
                                                                        \
ECode className::GetFilterTouchesWhenObscured(                          \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::GetFilterTouchesWhenObscured();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFilterTouchesWhenObscured(                          \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetFilterTouchesWhenObscured(enabled);           \
}                                                                       \
                                                                        \
ECode className::IsSaveFromParentEnabled(                               \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsSaveFromParentEnabled();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSaveFromParentEnabled(                              \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetSaveFromParentEnabled(enabled);               \
}                                                                       \
                                                                        \
ECode className::IsFocusable(                                           \
    /* [out] */ Boolean* focusable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(focusable);                                       \
    *focusable = superClass::IsFocusable();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsFocusableInTouchMode(                                \
    /* [out] */ Boolean* focusable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(focusable);                                       \
    *focusable = superClass::IsFocusableInTouchMode();                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FocusSearch(                                           \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> tmp = View::FocusSearch(direction);                  \
    *view = tmp;                                                        \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchUnhandledMove(                                 \
    /* [in] */ IView* focused,                                          \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchUnhandledMove(focused, direction);    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetFocusables(                                         \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IObjectContainer** views)                               \
{                                                                       \
    VALIDATE_NOT_NULL(views);                                           \
    return superClass::GetFocusables(direction, views);                 \
}                                                                       \
                                                                        \
ECode className::AddFocusables(                                         \
    /* [in] */ IObjectContainer* views,                                 \
    /* [in] */ Int32 direction)                                         \
{                                                                       \
    return superClass::AddFocusables(views, direction);                 \
}                                                                       \
                                                                        \
ECode className::AddFocusablesEx(                                       \
    /* [in] */ IObjectContainer* views,                                 \
    /* [in] */ Int32 direction,                                         \
    /* [in] */ Int32 focusableMode)                                     \
{                                                                       \
    return superClass::AddFocusables(views, direction, focusableMode);  \
}                                                                       \
                                                                        \
ECode className::FindViewsWithText(                                     \
    /* [in, out] */ IObjectContainer* outViews,                         \
    /* [in] */ ICharSequence* searched,                                 \
    /* [in] */ Int32 flags)                                             \
{                                                                       \
    return superClass::FindViewsWithText(outViews, searched, flags);    \
}                                                                       \
                                                                        \
ECode className::GetTouchables(                                         \
    /* [out] */ IObjectContainer** views)                               \
{                                                                       \
    VALIDATE_NOT_NULL(views);                                           \
    return superClass::GetTouchables(views);                            \
}                                                                       \
                                                                        \
ECode className::AddTouchables(                                         \
    /* [in] */ IObjectContainer* views)                                 \
{                                                                       \
    return superClass::AddTouchables(views);                            \
}                                                                       \
                                                                        \
ECode className::RequestAccessibilityFocus(                             \
    /* [out] */ Boolean* focus)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(focus);                                           \
    *focus = superClass::RequestAccessibilityFocus();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ClearAccessibilityFocus()                              \
{                                                                       \
    return superClass::ClearAccessibilityFocus();                       \
}                                                                       \
                                                                        \
ECode className::RequestFocus(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = View::RequestFocus();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusEx(                                        \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = View::RequestFocus(direction);                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusEx2(                                       \
    /* [in] */ Int32 direction,                                         \
    /* [in] */ IRect* previouslyFocusedRect,                            \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestFocus(direction, previouslyFocusedRect);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusFromTouch(                                 \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestFocusFromTouch();                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetImportantForAccessibility(                          \
    /* [out] */ Int32* mode)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    *mode = superClass::GetImportantForAccessibility();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetImportantForAccessibility(                          \
    /* [in] */ Int32 mode)                                              \
{                                                                       \
    return superClass::SetImportantForAccessibility(mode);              \
}                                                                       \
                                                                        \
ECode className::IsImportantForAccessibility(                           \
    /* [out] */ Boolean* exposed)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(exposed);                                         \
    *exposed = superClass::IsImportantForAccessibility();               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParentForAccessibility(                             \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> tmp = superClass::GetParentForAccessibility(); \
    *parent = tmp;                                                      \
                                                                        \
    if (*parent != NULL) {                                              \
        (*parent)->AddRef();                                            \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AddChildrenForAccessibility(                           \
    /* [in] */ IObjectContainer* children)                              \
{                                                                       \
    return superClass::AddChildrenForAccessibility(children);           \
}                                                                       \
                                                                        \
ECode className::IncludeForAccessibility(                               \
    /* [out] */ Boolean* include)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(include);                                         \
    *include = superClass::IncludeForAccessibility();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsActionableForAccessibility(                          \
    /* [out] */ Boolean* actionable)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(actionable);                                      \
    *actionable = superClass::IsActionableForAccessibility();           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::NotifyAccessibilityStateChanged()                      \
{                                                                       \
    return superClass::NotifyAccessibilityStateChanged();               \
}                                                                       \
                                                                        \
ECode className::ResetAccessibilityStateChanged()                       \
{                                                                       \
    return superClass::ResetAccessibilityStateChanged();                \
}                                                                       \
                                                                        \
ECode className::PerformAccessibilityAction(                            \
    /* [in] */ Int32 action,                                            \
    /* [in] */ IBundle* arguments,                                      \
    /* [out] */ Boolean* performed)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(performed);                                       \
    *performed = superClass::PerformAccessibilityAction(action, arguments);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetIterableTextForAccessibility(                       \
    /* [out] */ ICharSequence** text)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(text);                                            \
    AutoPtr<ICharSequence> tmp = superClass::GetIterableTextForAccessibility(); \
    *text = tmp;                                                        \
    if (*text) {                                                        \
        (*text)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetAccessibilityCursorPosition(                        \
    /* [out] */ Int32* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetAccessibilityCursorPosition();                \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAccessibilityCursorPosition(                        \
    /* [in] */ Int32 position)                                          \
{                                                                       \
    return superClass::SetAccessibilityCursorPosition(position);        \
}                                                                       \
                                                                        \
ECode className::GetIteratorForGranularity(                             \
    /* [in] */ Int32 granularity,                                       \
    /* [out] */ ITextSegmentIterator** iterator)                        \
{                                                                       \
    VALIDATE_NOT_NULL(iterator);                                        \
    AutoPtr<ITextSegmentIterator> tmp = superClass::GetIteratorForGranularity(granularity); \
    *iterator = tmp;                                                    \
    if (*iterator) {                                                    \
        (*iterator)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchStartTemporaryDetach()                         \
{                                                                       \
    return superClass::DispatchStartTemporaryDetach();                  \
}                                                                       \
                                                                        \
ECode className::OnStartTemporaryDetach()                               \
{                                                                       \
    return superClass::OnStartTemporaryDetach();                        \
}                                                                       \
                                                                        \
ECode className::DispatchFinishTemporaryDetach()                        \
{                                                                       \
    return superClass::DispatchFinishTemporaryDetach();                 \
}                                                                       \
                                                                        \
ECode className::OnFinishTemporaryDetach()                              \
{                                                                       \
    return superClass::OnFinishTemporaryDetach();                       \
}                                                                       \
                                                                        \
ECode className::GetKeyDispatcherState(                                 \
    /* [out] */ IDispatcherState** state)                               \
{                                                                       \
    return superClass::GetKeyDispatcherState(state);                    \
}                                                                       \
                                                                        \
ECode className::DispatchKeyEventPreIme(                                \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchKeyEventPreIme(event);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchKeyEvent(                                      \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchKeyEvent(event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchKeyShortcutEvent(                              \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchKeyShortcutEvent(event);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTouchEvent(                                    \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchTouchEvent(event);                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnFilterTouchEventForSecurity(                         \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnFilterTouchEventForSecurity(event);         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTrackballEvent(                                \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchTrackballEvent(event);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchGenericMotionEvent(                            \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchGenericMotionEvent(event);            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchPointerEvent(                                  \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchPointerEvent(event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchWindowFocusChanged(                            \
    /* [in] */ Boolean hasFocus)                                        \
{                                                                       \
    return superClass::DispatchWindowFocusChanged(hasFocus);            \
}                                                                       \
                                                                        \
ECode className::OnWindowFocusChanged(                                  \
    /* [in] */ Boolean hasWindowFocus)                                  \
{                                                                       \
    return superClass::OnWindowFocusChanged(hasWindowFocus);            \
}                                                                       \
                                                                        \
ECode className::HasWindowFocus(                                        \
    /* [out] */ Boolean* focus)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(focus);                                           \
    *focus = superClass::HasWindowFocus();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchDisplayHint(                                   \
    /* [in] */ Int32 hint)                                              \
{                                                                       \
    return superClass::DispatchDisplayHint(hint);                       \
}                                                                       \
                                                                        \
ECode className::DispatchWindowVisibilityChanged(                       \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::DispatchWindowVisibilityChanged(visibility);     \
}                                                                       \
                                                                        \
ECode className::GetWindowVisibility(                                   \
    /* [out] */ Int32* visibility)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(visibility);                                      \
    *visibility = superClass::GetWindowVisibility();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWindowVisibleDisplayFrame(                          \
    /* [in] */ IRect* outRect)                                          \
{                                                                       \
    return superClass::GetWindowVisibleDisplayFrame(outRect);           \
}                                                                       \
                                                                        \
ECode className::DispatchConfigurationChanged(                          \
    /* [in] */ IConfiguration* newConfig)                               \
{                                                                       \
    return superClass::DispatchConfigurationChanged(newConfig);         \
}                                                                       \
                                                                        \
ECode className::IsInTouchMode(                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsInTouchMode();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetContext(                                            \
    /* [out] */ IContext** context)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(context);                                         \
                                                                        \
    AutoPtr<IContext> tmp = superClass::GetContext();                   \
    *context = tmp;                                                     \
    if (*context != NULL) {                                             \
        (*context)->AddRef();                                           \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyPreIme(                                           \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyPreIme(keyCode, event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyShortcut(                                         \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyPreIme(keyCode, event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCheckIsTextEditor(                                   \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnCheckIsTextEditor();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCreateInputConnection(                               \
    /* [in] */ IEditorInfo* outAttrs,                                   \
    /* [out] */ IInputConnection** inputConnection)                     \
{                                                                       \
    VALIDATE_NOT_NULL(inputConnection);                                 \
    AutoPtr<IInputConnection> ic = superClass::OnCreateInputConnection(outAttrs);   \
    *inputConnection = ic.Get(); \
    if (*inputConnection != NULL) (*inputConnection)->AddRef(); \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CheckInputConnectionProxy(                             \
    /* [in] */ IView* view,                                             \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::CheckInputConnectionProxy(view);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateContextMenu(                                     \
    /* [in] */ IContextMenu* menu)                                      \
{                                                                       \
    return superClass::CreateContextMenu(menu);                         \
}                                                                       \
                                                                        \
ECode className::OnTrackballEvent(                                      \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnTrackballEvent(event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnGenericMotionEvent(                                  \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnGenericMotionEvent(event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnHoverEvent(                                          \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnHoverEvent(event);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsHovered(                                             \
    /* [out] */ Boolean* hovered)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(hovered);                                         \
    *hovered = superClass::IsHovered();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHovered(                                            \
    /* [in] */ Boolean hovered)                                         \
{                                                                       \
    return superClass::SetHovered(hovered);                             \
}                                                                       \
                                                                        \
ECode className::OnHoverChanged(                                        \
    /* [in] */ Boolean hovered)                                         \
{                                                                       \
    return superClass::OnHoverChanged(hovered);                         \
}                                                                       \
                                                                        \
ECode className::OnTouchEvent(                                          \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnTouchEvent(event);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsInScrollingContainer(                                \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsInScrollingContainer();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CancelLongPress()                                      \
{                                                                       \
    return superClass::CancelLongPress();                               \
}                                                                       \
                                                                        \
ECode className::SetTouchDelegate(                                      \
    /* [in] */ ITouchDelegate* touchDelegate)                           \
{                                                                       \
    return superClass::SetTouchDelegate(touchDelegate);                 \
}                                                                       \
                                                                        \
ECode className::GetTouchDelegate(                                      \
    /* [out] */ ITouchDelegate** touchDelegate)                         \
{                                                                       \
    return superClass::GetTouchDelegate(touchDelegate);                 \
}                                                                       \
                                                                        \
ECode className::BringToFront()                                         \
{                                                                       \
    return superClass::BringToFront();                                  \
}                                                                       \
                                                                        \
ECode className::GetParent(                                             \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
                                                                        \
    AutoPtr<IViewParent> tmp = superClass::GetParent();                 \
    *parent = tmp;                                                      \
    if (*parent != NULL) {                                              \
        (*parent)->AddRef();                                            \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollX(                                            \
    /* [in] */ Int32 value)                                             \
{                                                                       \
    return superClass::SetScrollX(value);                               \
}                                                                       \
                                                                        \
ECode className::SetScrollY(                                            \
    /* [in] */ Int32 value)                                             \
{                                                                       \
    return superClass::SetScrollY(value);                               \
}                                                                       \
                                                                        \
ECode className::GetScrollX(                                            \
    /* [out] */ Int32* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetScrollX();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetScrollY(                                            \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetScrollY();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWidth(                                              \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetWidth();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHeight(                                             \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetHeight();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingRect(                                        \
    /* [in] */ IRect* outRect)                                          \
{                                                                       \
    return superClass::GetDrawingRect(outRect);                         \
}                                                                       \
                                                                        \
ECode className::GetMeasuredWidth(                                      \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetMeasuredWidth();                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMeasuredWidthAndState(                              \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetMeasuredWidthAndState();                    \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMeasuredHeight(                                     \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetMeasuredHeight();                          \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMeasuredHeightAndState(                             \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetMeasuredHeightAndState();                  \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMeasuredState(                                      \
    /* [out] */ Int32* state)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(state);                                           \
    *state = superClass::GetMeasuredState();                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMatrix(                                             \
    /* [out] */ Elastos::Droid::Graphics::IMatrix** matrix)             \
{                                                                       \
    VALIDATE_NOT_NULL(matrix);                                          \
    AutoPtr<Elastos::Droid::Graphics::IMatrix> tmp =                    \
        superClass::GetMatrix();                                        \
    *matrix = tmp;                                                      \
    if (*matrix != NULL) {                                              \
        (*matrix)->AddRef();                                            \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetCameraDistance(                                     \
    /* [out] */ Float* distance)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(distance);                                        \
    *distance = superClass::GetCameraDistance();                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetCameraDistance(                                     \
    /* [in] */ Float distance)                                          \
{                                                                       \
    return superClass::SetCameraDistance(distance);                     \
}                                                                       \
                                                                        \
ECode className::GetRotation(                                           \
    /* [out] */ Float* rotation)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(rotation);                                        \
    *rotation = superClass::GetRotation();                              \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetRotation(                                           \
    /* [in] */ Float rotation)                                          \
{                                                                       \
    return superClass::SetRotation(rotation);                           \
}                                                                       \
                                                                        \
ECode className::GetRotationY(                                          \
    /* [out] */ Float* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetRotationY();                                    \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetRotationY(                                          \
    /* [in] */ Float rotationY)                                         \
{                                                                       \
    return superClass::SetRotationY(rotationY);                         \
}                                                                       \
                                                                        \
ECode className::GetRotationX(                                          \
    /* [out] */ Float* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetRotationX();                                    \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetRotationX(                                          \
    /* [in] */ Float rotationX)                                         \
{                                                                       \
    return superClass::SetRotationX(rotationX);                         \
}                                                                       \
                                                                        \
ECode className::GetScaleX(                                             \
    /* [out] */ Float* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetScaleX();                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScaleX(                                             \
    /* [in] */ Float scaleX)                                            \
{                                                                       \
    return superClass::SetScaleX(scaleX);                               \
}                                                                       \
                                                                        \
ECode className::GetScaleY(                                             \
    /* [out] */ Float* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetScaleY();                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScaleY(                                             \
    /* [in] */ Float scaleY)                                            \
{                                                                       \
    return superClass::SetScaleY(scaleY);                               \
}                                                                       \
                                                                        \
ECode className::GetPivotX(                                             \
    /* [out] */ Float* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetPivotX();                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPivotX(                                             \
    /* [in] */ Float pivotX)                                            \
{                                                                       \
    return superClass::SetPivotX(pivotX);                               \
}                                                                       \
                                                                        \
ECode className::GetPivotY(                                             \
    /* [out] */ Float* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetPivotY();                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPivotY(                                             \
    /* [in] */ Float pivotY)                                            \
{                                                                       \
    return superClass::SetPivotY(pivotY);                               \
}                                                                       \
                                                                        \
ECode className::GetAlpha(                                              \
    /* [out] */ Float* alpha)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(alpha);                                           \
    *alpha = superClass::GetAlpha();                                    \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasOverlappingRendering(                               \
    /* [out] */ Boolean* overlap)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(overlap);                                         \
    *overlap = superClass::HasOverlappingRendering();                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAlpha(                                              \
    /* [in] */ Float alpha)                                             \
{                                                                       \
    return superClass::SetAlpha(alpha);                                 \
}                                                                       \
                                                                        \
ECode className::GetTop(                                                \
    /* [out] */ Int32* top)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(top);                                             \
    *top = superClass::GetTop();                                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTop(                                                \
    /* [in] */ Int32 top)                                               \
{                                                                       \
    return superClass::SetTop(top);                                     \
}                                                                       \
                                                                        \
ECode className::GetBottom(                                             \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bottom);                                          \
    *bottom = superClass::GetBottom();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsDirty(                                               \
    /* [out] */ Boolean* dirty)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(dirty);                                           \
    *dirty = superClass::IsDirty();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetBottom(                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetBottom(bottom);                               \
}                                                                       \
                                                                        \
ECode className::GetLeft(                                               \
    /* [out] */ Int32* left)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    *left = superClass::GetLeft();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLeft(                                               \
    /* [in] */ Int32 left)                                              \
{                                                                       \
    return superClass::SetLeft(left);                                   \
}                                                                       \
                                                                        \
ECode className::GetRight(                                              \
    /* [out] */ Int32* right)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(right);                                           \
    *right = superClass::GetRight();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetRight(                                              \
    /* [in] */ Int32 right)                                             \
{                                                                       \
    return superClass::SetRight(right);                                 \
}                                                                       \
                                                                        \
ECode className::GetX(                                                  \
    /* [out] */ Float* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetX();                                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetX(                                                  \
    /* [in] */ Float x)                                                 \
{                                                                       \
    return superClass::SetX(x);                                         \
}                                                                       \
                                                                        \
ECode className::GetY(                                                  \
    /* [out] */ Float* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetY();                                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetY(                                                  \
    /* [in] */ Float y)                                                 \
{                                                                       \
    return superClass::SetY(y);                                         \
}                                                                       \
                                                                        \
ECode className::GetTranslationX(                                       \
    /* [out] */ Float* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetTranslationX();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTranslationX(                                       \
    /* [in] */ Float translationX)                                      \
{                                                                       \
    return superClass::SetTranslationX(translationX);                   \
}                                                                       \
                                                                        \
ECode className::GetTranslationY(                                       \
    /* [out] */ Float* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetTranslationY();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTranslationY(                                       \
    /* [in] */ Float translationY)                                      \
{                                                                       \
    return superClass::SetTranslationY(translationY);                   \
}                                                                       \
                                                                        \
ECode className::GetHitRect(                                            \
    /* [in, out] */ IRect* outRect)                                     \
{                                                                       \
    return superClass::GetHitRect(outRect);                             \
}                                                                       \
                                                                        \
ECode className::GetFocusedRect(                                        \
    /* [in, out] */ IRect* outRect)                                     \
{                                                                       \
    return superClass::GetFocusedRect(outRect);                         \
}                                                                       \
                                                                        \
ECode className::GetGlobalVisibleRect(                                  \
    /* [in, out] */ IRect* outRect,                                     \
    /* [in, out] */ IPoint* outGlobalOffset,                            \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetGlobalVisibleRect(outRect, outGlobalOffset);\
                                                                        \
    return NOERROR;;                                                    \
}                                                                       \
                                                                        \
ECode className::GetGlobalVisibleRectEx(                                \
    /* [in, out] */ IRect* outRect,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetGlobalVisibleRect(outRect);                \
                                                                        \
    return NOERROR;;                                                    \
}                                                                       \
                                                                        \
ECode className::GetLocalVisibleRect(                                   \
    /* [in, out] */ IRect* outRect,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetLocalVisibleRect(outRect);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OffsetTopAndBottom(                                    \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetTopAndBottom(offset);                      \
}                                                                       \
                                                                        \
ECode className::OffsetLeftAndRight(                                    \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetLeftAndRight(offset);                      \
}                                                                       \
                                                                        \
ECode className::GetLayoutParams(                                       \
    /* [out] */ IViewGroupLayoutParams** params)                        \
{                                                                       \
    VALIDATE_NOT_NULL(params);                                          \
    AutoPtr<IViewGroupLayoutParams> tmp = superClass::GetLayoutParams();\
    *params = tmp;                                                      \
                                                                        \
    if (*params) {                                                      \
        (*params)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutParams(                                       \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::SetLayoutParams(params);                         \
}                                                                       \
                                                                        \
ECode className::ResolveLayoutParams()                                  \
{                                                                       \
    return superClass::ResolveLayoutParams();                           \
}                                                                       \
                                                                        \
ECode className::ScrollTo(                                              \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y)                                                 \
{                                                                       \
    return superClass::ScrollTo(x, y);                                  \
}                                                                       \
                                                                        \
ECode className::ScrollBy(                                              \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y)                                                 \
{                                                                       \
    return superClass::ScrollBy(x, y);                                  \
}                                                                       \
                                                                        \
ECode className::Invalidate(                                            \
    /* [in] */ IRect* dirty)                                            \
{                                                                       \
    return superClass::Invalidate(dirty);                               \
}                                                                       \
                                                                        \
ECode className::InvalidateEx(                                          \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::Invalidate(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::InvalidateEx2()                                        \
{                                                                       \
    return superClass::Invalidate();                                    \
}                                                                       \
                                                                        \
ECode className::IsOpaque(                                              \
    /* [out] */ Boolean* opaque)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(opaque);                                          \
    *opaque = superClass::IsOpaque();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHandler(                                            \
    /* [out] */ IHandler** apartment)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(apartment);                                       \
    AutoPtr<IHandler> tmp = superClass::GetHandler();                   \
    *apartment = tmp;                                                   \
    if (*apartment != NULL) (*apartment)->AddRef();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Post(                                                  \
    /* [in] */ IRunnable* action,                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::Post(action);                                 \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PostInvalidate()                                       \
{                                                                       \
    return superClass::PostInvalidate();                                \
}                                                                       \
                                                                        \
ECode className::PostInvalidateEx(                                      \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::PostInvalidate(left, top, right, bottom);        \
}                                                                       \
                                                                        \
ECode className::PostInvalidateDelayed(                                 \
    /* [in] */ Int64 delayMilliseconds)                                 \
{                                                                       \
    return superClass::PostInvalidateDelayed(delayMilliseconds);        \
}                                                                       \
                                                                        \
ECode className::PostInvalidateDelayedEx(                               \
    /* [in] */ Int64 delayMilliseconds,                                 \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::PostInvalidateDelayed(                           \
        delayMilliseconds, left, top, right, bottom);                   \
}                                                                       \
                                                                        \
ECode className::PostInvalidateOnAnimation()                            \
{                                                                       \
    return superClass::PostInvalidateOnAnimation();                     \
}                                                                       \
                                                                        \
ECode className::PostInvalidateOnAnimationEx(                           \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::PostInvalidateOnAnimation(                       \
        left, top, right, bottom);                                      \
}                                                                       \
                                                                        \
ECode className::ComputeScroll()                                        \
{                                                                       \
    return superClass::ComputeScroll();                                 \
}                                                                       \
                                                                        \
ECode className::IsHorizontalFadingEdgeEnabled(                         \
    /* [out] */ Boolean* horizontalFadingEdgeEnabled)                   \
{                                                                       \
    VALIDATE_NOT_NULL(horizontalFadingEdgeEnabled);                     \
    *horizontalFadingEdgeEnabled = superClass::IsHorizontalFadingEdgeEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHorizontalFadingEdgeEnabled(                        \
    /* [in] */ Boolean horizontalFadingEdgeEnabled)                     \
{                                                                       \
    return superClass::SetHorizontalFadingEdgeEnabled(                  \
        horizontalFadingEdgeEnabled);                                   \
}                                                                       \
                                                                        \
ECode className::IsVerticalFadingEdgeEnabled(                           \
    /* [out] */ Boolean* verticalFadingEdgeEnabled)                     \
{                                                                       \
    VALIDATE_NOT_NULL(verticalFadingEdgeEnabled);                       \
    *verticalFadingEdgeEnabled = superClass::IsVerticalFadingEdgeEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVerticalFadingEdgeEnabled(                          \
    /* [in] */ Boolean verticalFadingEdgeEnabled)                       \
{                                                                       \
    return superClass::SetVerticalFadingEdgeEnabled(                    \
        verticalFadingEdgeEnabled);                                     \
}                                                                       \
                                                                        \
ECode className::IsHorizontalScrollBarEnabled(                          \
    /* [out] */ Boolean* horizontalScrollBarEnabled)                    \
{                                                                       \
    VALIDATE_NOT_NULL(horizontalScrollBarEnabled);                      \
    *horizontalScrollBarEnabled = superClass::IsHorizontalScrollBarEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHorizontalScrollBarEnabled(                         \
    /* [in] */ Boolean horizontalScrollBarEnabled)                      \
{                                                                       \
    return superClass::SetHorizontalScrollBarEnabled(                   \
        horizontalScrollBarEnabled);                                    \
}                                                                       \
                                                                        \
ECode className::IsVerticalScrollBarEnabled(                            \
    /* [out] */ Boolean* verticalScrollBarEnabled)                      \
{                                                                       \
    VALIDATE_NOT_NULL(verticalScrollBarEnabled);                        \
    *verticalScrollBarEnabled = superClass::IsVerticalScrollBarEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVerticalScrollBarEnabled(                           \
    /* [in] */ Boolean verticalScrollBarEnabled)                        \
{                                                                       \
    return superClass::SetVerticalScrollBarEnabled(verticalScrollBarEnabled);\
}                                                                       \
                                                                        \
ECode className::SetScrollbarFadingEnabled(                             \
    /* [in] */ Boolean fadeScrollbars)                                  \
{                                                                       \
    return superClass::SetScrollbarFadingEnabled(fadeScrollbars);       \
}                                                                       \
                                                                        \
ECode className::IsScrollbarFadingEnabled(                              \
    /* [out] */ Boolean* fadeScrollbars)                                \
{                                                                       \
    VALIDATE_NOT_NULL(fadeScrollbars);                                  \
    *fadeScrollbars = superClass::IsScrollbarFadingEnabled();           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetScrollBarDefaultDelayBeforeFade(                    \
    /* [out] */Int32* defaultDelay)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(defaultDelay);                                    \
    *defaultDelay = superClass::GetScrollBarDefaultDelayBeforeFade();   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollBarDefaultDelayBeforeFade(                    \
    /* [in] */Int32 scrollBarDefaultDelayBeforeFade)                    \
{                                                                       \
    return superClass::SetScrollBarDefaultDelayBeforeFade(scrollBarDefaultDelayBeforeFade);  \
}                                                                       \
                                                                        \
ECode className::GetScrollBarFadeDuration(                              \
    /* [out] */Int32* duration)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(duration);                                        \
    *duration = superClass::GetScrollBarFadeDuration();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollBarFadeDuration(                              \
    /* [in] */Int32 scrollBarFadeDuration)                              \
{                                                                       \
    return superClass::SetScrollBarFadeDuration(scrollBarFadeDuration); \
}                                                                       \
                                                                        \
ECode className::GetScrollBarSize(                                      \
    /* [out] */Int32* size)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(size);                                            \
    *size = superClass::GetScrollBarSize();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollBarSize(                                      \
    /* [in] */Int32 scrollBarSize)                                      \
{                                                                       \
    return superClass::SetScrollBarSize(scrollBarSize);                 \
}                                                                       \
                                                                        \
ECode className::SetScrollBarStyle(                                     \
    /* [in] */ Int32 style)                                             \
{                                                                       \
    return superClass::SetScrollBarStyle(style);                        \
}                                                                       \
                                                                        \
ECode className::GetScrollBarStyle(                                     \
    /* [out] */ Int32* style)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(style);                                           \
    *style = superClass::GetScrollBarStyle();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CanScrollHorizontally(                                 \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* can)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(can);                                             \
    *can = superClass::CanScrollHorizontally(direction);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CanScrollVertically(                                   \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* can)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(can);                                             \
    *can = superClass::CanScrollVertically(direction);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResolveRtlPropertiesIfNeeded()                         \
{                                                                       \
    return superClass::ResolveRtlPropertiesIfNeeded();                  \
}                                                                       \
                                                                        \
ECode className::ResetRtlProperties()                                   \
{                                                                       \
    return superClass::ResetRtlProperties();                            \
}                                                                       \
                                                                        \
ECode className::OnScreenStateChanged(                                  \
    /* [in] */ Int32 screenState)                                       \
{                                                                       \
    return superClass::OnScreenStateChanged(screenState);               \
}                                                                       \
                                                                        \
ECode className::OnRtlPropertiesChanged(                                \
    /* [in] */ Int32 layoutDirection)                                   \
{                                                                       \
    return superClass::OnRtlPropertiesChanged(layoutDirection);         \
}                                                                       \
                                                                        \
ECode className::ResolveLayoutDirection(                                \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ResolveLayoutDirection();                     \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CanResolveLayoutDirection(                             \
    /* [out] */ Boolean*result)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::CanResolveLayoutDirection();                  \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResetResolvedLayoutDirection()                         \
{                                                                       \
    return superClass::ResetResolvedLayoutDirection();                  \
}                                                                       \
                                                                        \
ECode className::IsLayoutDirectionInherited(                            \
    /* [out] */ Boolean*result)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsLayoutDirectionInherited();                 \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResolvePadding()                                       \
{                                                                       \
    return superClass::ResolvePadding();                                \
}                                                                       \
                                                                        \
ECode className::ResetResolvedPadding()                                 \
{                                                                       \
    return superClass::ResetResolvedPadding();                          \
}                                                                       \
                                                                        \
ECode className::AssignParent(                                          \
    /* [in] */ IViewParent* parent)                                     \
{                                                                       \
    return superClass::AssignParent(parent);                            \
}                                                                       \
                                                                        \
ECode className::GetWindowToken(                                        \
    /* [out] */ IBinder** token)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(token);                                           \
    AutoPtr<IBinder> tmp = superClass::GetWindowToken();                \
    *token = tmp;                                                       \
    if (*token != NULL) {                                               \
        (*token)->AddRef();                                             \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetApplicationWindowToken(                             \
    /* [out] */ IBinder** token)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(token);                                           \
    AutoPtr<IBinder> tmp = superClass::GetApplicationWindowToken();     \
    *token = tmp;                                                       \
    if (*token) {                                                       \
        (*token)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDisplay(                                            \
    /* [out] */ IDisplay** display)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(display);                                         \
    AutoPtr<IDisplay> tmp = superClass::GetDisplay();                   \
    *display = tmp;                                                     \
    if (*display) {                                                     \
        (*display)->AddRef();                                           \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
ECode className::SaveHierarchyState(                                    \
    /* [in, out] */ IObjectInt32Map* container)                       \
{                                                                       \
    return superClass::SaveHierarchyState(container);                   \
}                                                                       \
                                                                        \
ECode className::RestoreHierarchyState(                                 \
    /* [in] */ IObjectInt32Map* container)                            \
{                                                                       \
    return superClass::RestoreHierarchyState(container);                \
}                                                                       \
                                                                        \
ECode className::GetDrawingTime(                                        \
    /* [out] */ Int64* time)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(time);                                            \
    *time = superClass::GetDrawingTime();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDuplicateParentStateEnabled(                        \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetDuplicateParentStateEnabled(enabled);         \
}                                                                       \
                                                                        \
ECode className::IsDuplicateParentStateEnabled(                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsDuplicateParentStateEnabled();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayerType(                                          \
    /* [in] */ Int32 layerType,                                         \
    /* [in] */ IPaint* paint)                                           \
{                                                                       \
    return superClass::SetLayerType(layerType, paint);                  \
}                                                                       \
                                                                        \
ECode className::SetLayerPaint(                                         \
    /* [in] */ IPaint* paint)                                           \
{                                                                       \
    return superClass::SetLayerPaint(paint);                            \
}                                                                       \
                                                                        \
ECode className::GetLayerType(                                          \
    /* [out] */ Int32* type)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(type);                                            \
    *type = superClass::GetLayerType();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::BuildLayer()                                           \
{                                                                       \
    return superClass::BuildLayer();                                    \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheEnabled(                                \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetDrawingCacheEnabled(enabled);                 \
}                                                                       \
                                                                        \
ECode className::IsDrawingCacheEnabled(                                 \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsDrawingCacheEnabled();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CanHaveDisplayList(                                    \
    /* [out] */ Boolean* can)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(can);                                             \
    *can = superClass::CanHaveDisplayList();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDisplayList(                                        \
    /* [out] */ IDisplayList** list)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(list);                                            \
    AutoPtr<IDisplayList> tmp = superClass::GetDisplayList();           \
    *list = tmp;                                                        \
    if(*list != NULL) {                                                 \
        (*list)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingCache(                                       \
    /* [out] */ IBitmap** bitmap)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    AutoPtr<IBitmap> tmp = superClass::GetDrawingCache();               \
    *bitmap = tmp;                                                      \
    if (*bitmap) {                                                      \
        (*bitmap)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheEx(                                     \
    /* [in] */ Boolean autoScale,                                       \
    /* [out] */ IBitmap** bitmap)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    AutoPtr<IBitmap> tmp = superClass::GetDrawingCache(autoScale);      \
    *bitmap = tmp;                                                      \
    if (*bitmap) {                                                      \
        (*bitmap)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DestroyDrawingCache()                                  \
{                                                                       \
    return superClass::DestroyDrawingCache();                           \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheBackgroundColor(                        \
    /* [in] */ Int32 color)                                             \
{                                                                       \
    return superClass::SetDrawingCacheBackgroundColor(color);           \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheBackgroundColor(                        \
    /* [out] */ Int32* color)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(color);                                           \
    *color = superClass::GetDrawingCacheBackgroundColor();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::BuildDrawingCache()                                    \
{                                                                       \
    return superClass::BuildDrawingCache();                             \
}                                                                       \
                                                                        \
ECode className::BuildDrawingCacheEx(                                   \
    /* [in] */ Boolean autoScale)                                       \
{                                                                       \
    superClass::BuildDrawingCache(autoScale);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateSnapshot(                                        \
    /* [in] */ BitmapConfig quality,                                    \
    /* [in] */ Int32 backgroundColor,                                   \
    /* [in] */ Boolean skipChildren,                                    \
    /* [out] */ IBitmap** bitmap)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    superClass::CreateSnapshot(quality, backgroundColor, skipChildren, bitmap);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsInEditMode(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsInEditMode();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsHardwareAccelerated(                                 \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsHardwareAccelerated();                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Draw(                                                  \
    /* [in] */ ICanvas* canvas)                                         \
{                                                                       \
    return superClass::Draw(canvas);                                    \
}                                                                       \
                                                                        \
ECode className::GetSolidColor(                                         \
    /* [out] */ Int32* color)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(color);                                           \
    *color = superClass::GetSolidColor();                               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnFinishInflate()                                      \
{                                                                       \
    return superClass::OnFinishInflate();                               \
}                                                                       \
                                                                        \
ECode className::IsLayoutRequested(                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsLayoutRequested();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Layout(                                                \
    /* [in] */ Int32 l,                                                 \
    /* [in] */ Int32 t,                                                 \
    /* [in] */ Int32 r,                                                 \
    /* [in] */ Int32 b)                                                 \
{                                                                       \
    return superClass::Layout(l, t, r, b);                              \
}                                                                       \
                                                                        \
ECode className::GetResources(                                          \
    /* [out] */ IResources** resources)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(resources);                                       \
    AutoPtr<IResources> tmp = superClass::GetResources();               \
    *resources = tmp;                                                   \
    if (*resources != NULL) {                                           \
        (*resources)->AddRef();                                         \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::UnscheduleDrawableEx(                                  \
    /* [in] */ IDrawable* who)                                          \
{                                                                       \
    return superClass::UnscheduleDrawable(who);                         \
}                                                                       \
                                                                        \
ECode className::OnResolveDrawables(                                    \
    /* [in] */ Int64 layoutDirection)                                   \
{                                                                       \
    return superClass::OnResolveDrawables(layoutDirection);             \
}                                                                       \
                                                                        \
ECode className::RefreshDrawableState()                                 \
{                                                                       \
    return superClass::RefreshDrawableState();                          \
}                                                                       \
                                                                        \
ECode className::GetDrawableState(                                      \
    /* [out, callee] */ ArrayOf<Int32>** state)                         \
{                                                                       \
    VALIDATE_NOT_NULL(state);                                           \
    *state = superClass::GetDrawableState()->Clone();                   \
    INTERFACE_ADDREF(*state);                                           \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::JumpDrawablesToCurrentState()                          \
{                                                                       \
    return superClass::JumpDrawablesToCurrentState();                   \
}                                                                       \
                                                                        \
ECode className::SetBackgroundColor(                                    \
    /* [in] */ Int32 color)                                             \
{                                                                       \
    return superClass::SetBackgroundColor(color);                       \
}                                                                       \
                                                                        \
ECode className::SetBackgroundResource(                                 \
    /* [in] */ Int32 resid)                                             \
{                                                                       \
    return superClass::SetBackgroundResource(resid);                    \
}                                                                       \
                                                                        \
ECode className::SetBackground(                                         \
    /* [in] */ IDrawable* background)                                   \
{                                                                       \
    return superClass::SetBackground(background);                       \
}                                                                       \
                                                                        \
ECode className::SetBackgroundDrawable(                                 \
    /* [in] */ IDrawable* d)                                            \
{                                                                       \
    return superClass::SetBackgroundDrawable(d);                        \
}                                                                       \
                                                                        \
ECode className::GetBackground(                                         \
    /* [out] */ IDrawable** drawable)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    AutoPtr<IDrawable> dr = superClass::GetBackground();                \
    *drawable = dr.Get();                                               \
    INTERFACE_ADDREF(*drawable);                                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPadding(                                            \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetPadding(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::SetPaddingRelative(                                    \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 end,                                               \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetPaddingRelative(start, top, end, bottom);     \
}                                                                       \
                                                                        \
ECode className::GetPaddingTop(                                         \
    /* [out] */ Int32* top)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(top);                                             \
    *top = superClass::GetPaddingTop();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingBottom(                                      \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bottom);                                          \
    *bottom = superClass::GetPaddingBottom();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingLeft(                                        \
    /* [out] */ Int32* left)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    *left = superClass::GetPaddingLeft();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingStart(                                       \
    /* [out] */ Int32* start)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(start);                                           \
    *start = superClass::GetPaddingStart();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingRight(                                       \
    /* [out] */ Int32* right)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(right);                                           \
    *right = superClass::GetPaddingRight();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingEnd(                                         \
    /* [out] */ Int32* end)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(end);                                             \
    *end = superClass::GetPaddingEnd();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsPaddingRelative(                                     \
    /* [out] */ Boolean* relative)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(relative);                                        \
    *relative = superClass::IsPaddingRelative();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResetPaddingToInitialValues()                          \
{                                                                       \
    return superClass::ResetPaddingToInitialValues();                   \
}                                                                       \
                                                                        \
ECode className::GetOpticalInsets(                                      \
    /* [out] */ IInsets** sets)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(sets);                                            \
    AutoPtr<IInsets> s = superClass::GetOpticalInsets();                \
    *sets = s;                                                          \
    INTERFACE_ADDREF(*sets);                                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutInsets(                                       \
    /* [in] */ IInsets* layoutInsets)                                   \
{                                                                       \
    return superClass::SetLayoutInsets(layoutInsets);                   \
}                                                                       \
                                                                        \
ECode className::SetSelected(                                           \
    /* [in] */ Boolean selected)                                        \
{                                                                       \
    return superClass::SetSelected(selected);                           \
}                                                                       \
                                                                        \
ECode className::IsSelected(                                            \
    /* [out] */ Boolean* selected)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(selected);                                        \
    *selected = superClass::IsSelected();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetActivated(                                          \
    /* [in] */ Boolean activated)                                       \
{                                                                       \
    return superClass::SetActivated(activated);                         \
}                                                                       \
                                                                        \
ECode className::IsActivated(                                           \
    /* [out] */ Boolean* activated)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(activated);                                       \
    *activated = superClass::IsActivated();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetViewTreeObserver(                                   \
    /* [out] */ IViewTreeObserver** observer)                           \
{                                                                       \
    VALIDATE_NOT_NULL(observer);                                        \
    AutoPtr<IViewTreeObserver> tmp = superClass::GetViewTreeObserver(); \
    *observer = tmp;                                                    \
    if (*observer != NULL) {                                            \
        (*observer)->AddRef();                                          \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRootView(                                           \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> tmp = superClass::GetRootView();                     \
    *view = tmp;                                                        \
    if (*view != NULL) {                                                \
        (*view)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLocationOnScreen(                                   \
    /* [out] */ Int32* x,                                               \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    VALIDATE_NOT_NULL(y);                                               \
                                                                        \
    return superClass::GetLocationOnScreen(x, y);                       \
}                                                                       \
                                                                        \
ECode className::GetLocationInWindow(                                   \
    /* [out] */ Int32* x,                                               \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    VALIDATE_NOT_NULL(y);                                               \
                                                                        \
    return superClass::GetLocationInWindow(x, y);                       \
}                                                                       \
                                                                        \
ECode className::FindViewById(                                          \
    /* [in] */ Int32 id,                                                \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
                                                                        \
    AutoPtr<IView> tmp = superClass::FindViewById(id);                  \
    *view = tmp;                                                        \
    INTERFACE_ADDREF(*view);                                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetId(                                                 \
    /* [in] */ Int32 id)                                                \
{                                                                       \
    return superClass::SetId(id);                                       \
}                                                                       \
                                                                        \
ECode className::FindViewByPredicate(                                   \
    /* [in] */ IPredicate* predicate,                                   \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
                                                                        \
    AutoPtr<IView> tmp = superClass::FindViewByPredicate(predicate);    \
    *view = tmp;                                                        \
    INTERFACE_ADDREF(*view);                                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FindViewByPredicateInsideOut(                          \
    /* [in] */ IView* start,                                            \
    /* [in] */ IPredicate* predicate,                                   \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
                                                                        \
    AutoPtr<IView> tmp = superClass::FindViewByPredicateInsideOut(start, predicate); \
    *view = tmp;                                                        \
    INTERFACE_ADDREF(*view);                                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetId(                                                 \
    /* [out] */ Int32* id)                                              \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
                                                                        \
    *id = superClass::GetId();                                          \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsRootNamespace(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
                                                                        \
    *result = superClass::IsRootNamespace();                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetBaseline(                                           \
    /* [out] */ Int32* result)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    return superClass::GetBaseline(result);                             \
}                                                                       \
                                                                        \
ECode className::RequestLayout()                                        \
{                                                                       \
    return superClass::RequestLayout();                                 \
}                                                                       \
                                                                        \
ECode className::ForceLayout()                                          \
{                                                                       \
    return superClass::ForceLayout();                                   \
}                                                                       \
                                                                        \
ECode className::Measure(                                               \
    /* [in] */ Int32 widthMeasureSpec,                                  \
    /* [in] */ Int32 heightMeasureSpec)                                 \
{                                                                       \
    return superClass::Measure(widthMeasureSpec, heightMeasureSpec);    \
}                                                                       \
                                                                        \
ECode className::GetMinimumHeight(                                      \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetMinimumHeight();                           \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetMinimumHeight(                                      \
    /* [in] */ Int32 minHeight)                                         \
{                                                                       \
    return superClass::SetMinimumHeight(minHeight);                     \
}                                                                       \
                                                                        \
ECode className::GetMinimumWidth(                                       \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetMinimumWidth();                             \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetMinimumWidth(                                       \
    /* [in] */ Int32 minWidth)                                          \
{                                                                       \
    return superClass::SetMinimumWidth(minWidth);                       \
}                                                                       \
                                                                        \
ECode className::GetAnimation(                                          \
    /* [out] */ IAnimation** animation)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(animation);                                       \
                                                                        \
    AutoPtr<IAnimation> tmp = superClass::GetAnimation();               \
    *animation = tmp;                                                   \
    INTERFACE_ADDREF(*animation);                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::StartAnimation(                                        \
    /* [in] */ IAnimation* animation)                                   \
{                                                                       \
    return superClass::StartAnimation(animation);                       \
}                                                                       \
                                                                        \
ECode className::ClearAnimation()                                       \
{                                                                       \
    return superClass::ClearAnimation();                                \
}                                                                       \
                                                                        \
ECode className::SetAnimation(                                          \
    /* [in] */ IAnimation* animation)                                   \
{                                                                       \
    return superClass::SetAnimation(animation);                         \
}                                                                       \
                                                                        \
ECode className::GatherTransparentRegion(                               \
    /* [in] */ IRegion* region,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GatherTransparentRegion(region);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PlaySoundEffect(                                       \
    /* [in] */ Int32 soundConstant)                                     \
{                                                                       \
    return superClass::PlaySoundEffect(soundConstant);                  \
}                                                                       \
                                                                        \
ECode className::PerformHapticFeedback(                                 \
    /* [in] */ Int32 feedbackConstant,                                  \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformHapticFeedback(feedbackConstant);      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PerformHapticFeedbackEx(                               \
    /* [in] */ Int32 feedbackConstant,                                  \
    /* [in] */ Int32 flags,                                             \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformHapticFeedback(feedbackConstant, flags);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSystemUiVisibility(                                 \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::SetSystemUiVisibility(visibility);               \
}                                                                       \
                                                                        \
ECode className::GetSystemUiVisibility(                                 \
    /* [out] */ Int32* value)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::GetSystemUiVisibility();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWindowSystemUiVisibility(                           \
    /* [out] */ Int32* value)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::GetWindowSystemUiVisibility();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnWindowSystemUiVisibilityChanged(                     \
    /* [in] */ Int32 visible)                                           \
{                                                                       \
    return superClass::OnWindowSystemUiVisibilityChanged(visible);      \
}                                                                       \
                                                                        \
ECode className::DispatchWindowSystemUiVisiblityChanged(                \
    /* [in] */ Int32 visible)                                           \
{                                                                       \
    return superClass::DispatchWindowSystemUiVisiblityChanged(visible); \
}                                                                       \
                                                                        \
ECode className::SetOnSystemUiVisibilityChangeListener(                 \
    /* [in] */ IViewOnSystemUiVisibilityChangeListener* l)              \
{                                                                       \
    return superClass::SetOnSystemUiVisibilityChangeListener(l);        \
}                                                                       \
                                                                        \
ECode className::DispatchSystemUiVisibilityChanged(                     \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::DispatchSystemUiVisibilityChanged(visibility);   \
}                                                                       \
                                                                        \
ECode className::SetDisabledSystemUiVisibility(                         \
    /* [in] */ Int32 flags)                                             \
{                                                                       \
    return superClass::SetDisabledSystemUiVisibility(flags);            \
}                                                                       \
                                                                        \
ECode className::StartDrag(                                             \
    /* [in] */ IClipData* data,                                         \
    /* [in] */ IDragShadowBuilder* shadowBuilder,                       \
    /* [in] */ IInterface* myLocalState,                                \
    /* [in] */ Int32 flags,                                             \
    /* [out] */ Boolean* value)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::StartDrag(data, shadowBuilder, myLocalState, flags);  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnDragEvent(                                           \
    /* [in] */ IDragEvent* event,                                       \
    /* [out] */ Boolean* value)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::OnDragEvent(event);                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchDragEvent(                                     \
    /* [in] */ IDragEvent* event,                                       \
    /* [out] */ Boolean* value)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::DispatchDragEvent(event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCloseSystemDialogs(                                  \
    /* [in] */ const String& reason)                                    \
{                                                                       \
    return superClass::OnCloseSystemDialogs(reason);                    \
}                                                                       \
                                                                        \
ECode className::ApplyDrawableToTransparentRegion(                      \
    /* [in] */ IDrawable* dr,                                           \
    /* [in] */ IRegion* region)                                         \
{                                                                       \
    return superClass::ApplyDrawableToTransparentRegion(dr, region);    \
}                                                                       \
                                                                        \
ECode className::GetOverScrollMode(                                     \
    /* [out] */ Int32* mode)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    *mode = superClass::GetOverScrollMode();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOverScrollMode(                                     \
    /* [in] */ Int32 overScrollMode)                                    \
{                                                                       \
    return superClass::SetOverScrollMode(overScrollMode);               \
}                                                                       \
                                                                        \
ECode className::GetRawTextDirection(                                   \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetRawTextDirection();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTextDirection(                                      \
    /* [in] */ Int32 textDirection)                                     \
{                                                                       \
    return superClass::SetTextDirection(textDirection);                 \
}                                                                       \
                                                                        \
ECode className::GetTextDirection(                                      \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetTextDirection();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResolveTextDirection(                                  \
    /* [out] */ Boolean* done)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(done);                                            \
    *done = superClass::ResolveTextDirection();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResetResolvedTextDirection()                           \
{                                                                       \
    return superClass::ResetResolvedTextDirection();                    \
}                                                                       \
                                                                        \
ECode className::IsTextDirectionInherited(                              \
    /* [out] */ Boolean* inherited)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(inherited);                                       \
    *inherited = superClass::IsTextDirectionInherited();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRawTextAlignment(                                   \
    /* [out] */ Int32* textAlignment)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(textAlignment);                                   \
    *textAlignment = superClass::GetRawTextAlignment();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTextAlignment(                                      \
    /* [in] */ Int32 textAlignment)                                     \
{                                                                       \
    return superClass::SetTextAlignment(textAlignment);                 \
}                                                                       \
                                                                        \
ECode className::GetTextAlignment(                                      \
    /* [out] */ Int32* alignment)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(alignment);                                       \
    *alignment = superClass::GetTextAlignment();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResolveTextAlignment(                                  \
    /* [out] */ Boolean* done)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(done);                                            \
    *done = superClass::ResolveTextAlignment();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResetResolvedTextAlignment()                           \
{                                                                       \
    return superClass::ResetResolvedTextAlignment();                    \
}                                                                       \
                                                                        \
ECode className::IsTextAlignmentInherited(                              \
    /* [out] */ Boolean* inherited)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(inherited);                                       \
    *inherited = superClass::IsTextAlignmentInherited();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTag(                                                \
    /* [in] */ IInterface* tag)                                         \
{                                                                       \
    return superClass::SetTag(tag);                                     \
}                                                                       \
                                                                        \
ECode className::GetTag(                                                \
    /* [in] */ IInterface** tag)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(tag);                                             \
    AutoPtr<IInterface> tmp = superClass::GetTag();                     \
    *tag = tmp;                                                         \
    INTERFACE_ADDREF(*tag);                                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HackTurnOffWindowResizeAnim(                           \
    /* [in] */ Boolean off)                                             \
{                                                                       \
    return superClass::HackTurnOffWindowResizeAnim(off);                \
}                                                                       \
                                                                        \
ECode className::Animate(                                               \
    /* [out] */ IViewPropertyAnimator** animator)                       \
{                                                                       \
    VALIDATE_NOT_NULL(animator);                                        \
    AutoPtr<IViewPropertyAnimator> tmp = superClass::Animate();         \
    *animator = tmp;                                                    \
    INTERFACE_ADDREF(*animator);                                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetInflaterContext(                                    \
    /* [out] */ IContext** context)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(context);                                         \
    return superClass::GetInflaterContext(context);                     \
}                                                                       \
                                                                        \
ECode className::SetInflaterContext(                                    \
    /* [in] */ IContext* context)                                       \
{                                                                       \
    return superClass::SetInflaterContext(context);                     \
}                                                                       \
                                                                        \
ECode className::GetXmlPath(                                            \
    /* [out] */ String* path)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(path);                                            \
    return superClass::GetXmlPath(path);                                \
}                                                                       \
                                                                        \
ECode className::SetXmlPath(                                            \
    /* [in] */ const String& path)                                      \
{                                                                       \
    return superClass::SetXmlPath(path);                                \
}                                                                       \
                                                                        \
ECode className::SetTagEx(                                              \
    /* [in] */ Int32 key,                                               \
    /* [in] */ IInterface* tag)                                         \
{                                                                       \
    return superClass::SetTag(key, tag);                                \
}                                                                       \
                                                                        \
ECode className::GetTagEx(                                              \
    /* [in] */ Int32 key,                                               \
    /* [in] */ IInterface** tag)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(tag);                                             \
    AutoPtr<IInterface> tmp = superClass::GetTag(key);                  \
    *tag = tmp;                                                         \
    INTERFACE_ADDREF(*tag);                                             \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTagInternal(                                        \
    /* [in] */ Int32 key,                                               \
    /* [in] */ IInterface* tag)                                         \
{                                                                       \
    return superClass::SetTagInternal(key, tag);                        \
}                                                                       \


#define IVIEWGROUP_METHODS_DECL()                                       \
    CARAPI GetDescendantFocusability(                                   \
        /* [out] */ Int32* focusability);                               \
                                                                        \
    CARAPI SetDescendantFocusability(                                   \
        /* [in] */ Int32 focusability);                                 \
                                                                        \
    CARAPI StartActionModeForChild(                                     \
        /* [in] */ IView* originalView,                                 \
        /* [in] */ IActionModeCallback* cb,                             \
        /* [out] */ IActionMode** mode);                                \
                                                                        \
    CARAPI OnRequestSendAccessibilityEvent(                             \
        /* [in] */ IView* child,                                        \
        /* [in] */ IAccessibilityEvent* event,                          \
        /* [out] */ Boolean* sent);                                     \
                                                                        \
    CARAPI ChildHasTransientStateChanged(                               \
        /* [in] */ IView* child,                                        \
        /* [in] */ Boolean childHasTransientState);                     \
                                                                        \
    CARAPI GetFocusedChild(                                             \
        /* [out] */ IView** focusedChild);                              \
                                                                        \
    CARAPI OnInterceptTouchEvent(                                       \
        /* [in] */ IMotionEvent* ev,                                    \
        /* [out] */ Boolean* value);                                    \
                                                                        \
    CARAPI SetClipChildren(                                             \
        /* [in] */ Boolean clipChildren);                               \
                                                                        \
    CARAPI SetClipToPadding(                                            \
        /* [in] */ Boolean clipToPadding);                              \
                                                                        \
    CARAPI AddView(                                                     \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI AddViewEx(                                                   \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 index);                                        \
                                                                        \
    CARAPI AddViewEx2(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 width,                                         \
        /* [in] */ Int32 height);                                       \
                                                                        \
    CARAPI AddViewEx3(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI AddViewEx4(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 index,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI SetOnHierarchyChangeListener(                                \
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);      \
                                                                        \
    CARAPI RemoveViewInLayout(                                          \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI RemoveViewsInLayout(                                         \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 count);                                        \
                                                                        \
    CARAPI RemoveViewAt(                                                \
        /* [in] */ Int32 index);                                        \
                                                                        \
    CARAPI RemoveViews(                                                 \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 count);                                        \
                                                                        \
    CARAPI RemoveAllViews();                                            \
                                                                        \
    CARAPI RemoveAllViewsInLayout();                                    \
                                                                        \
    CARAPI OffsetDescendantRectToMyCoords(                              \
        /* [in] */ IView* descendant,                                   \
        /* [in] */ IRect* rect);                                        \
                                                                        \
    CARAPI OffsetRectIntoDescendantCoords(                              \
        /* [in] */ IView* descendant,                                   \
        /* [in] */ IRect* rect);                                        \
                                                                        \
    CARAPI OffsetChildrenTopAndBottom(                                  \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI StartLayoutAnimation();                                      \
                                                                        \
    CARAPI ScheduleLayoutAnimation();                                   \
                                                                        \
    CARAPI SetLayoutAnimation(                                          \
        /* [in] */ ILayoutAnimationController* controller);             \
                                                                        \
    CARAPI GetLayoutAnimation(                                          \
        /* [out] */ ILayoutAnimationController** controller);           \
                                                                        \
    CARAPI IsAnimationCacheEnabled(                                     \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetAnimationCacheEnabled(                                    \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsAlwaysDrawnWithCacheEnabled(                               \
        /* [out] */ Boolean* always);                                   \
                                                                        \
    CARAPI SetAlwaysDrawnWithCacheEnabled(                              \
        /* [in] */ Boolean always);                                     \
                                                                        \
    CARAPI GetPersistentDrawingCache(                                   \
        /* [out] */ Int32* drawingCacheToKeep);                         \
                                                                        \
    CARAPI SetPersistentDrawingCache(                                   \
        /* [in] */ Int32 drawingCacheToKeep);                           \
                                                                        \
    CARAPI GenerateLayoutParams(                                        \
        /* [in] */ IAttributeSet* attrs,                                \
        /* [out] */ IViewGroupLayoutParams** layoutParams);             \
                                                                        \
    CARAPI IndexOfChild(                                                \
        /* [in] */ IView* child,                                        \
        /* [out] */ Int32* index);                                      \
                                                                        \
    CARAPI GetChildCount(                                               \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetChildAt(                                                  \
        /* [in] */ Int32 index,                                         \
        /* [out] */ IView** child);                                     \
                                                                        \
    CARAPI ClearDisappearingChildren();                                 \
                                                                        \
    CARAPI GetLayoutAnimationListener(                                  \
        /* [out] */ IAnimationListener** listener);                     \
                                                                        \
    CARAPI SetAddStatesFromChildren(                                    \
        /* [in] */ Boolean addsStates);                                 \
                                                                        \
    CARAPI AddStatesFromChildren(                                       \
        /* [out] */ Boolean* drawable);                                 \
                                                                        \
    CARAPI SetLayoutAnimationListener(                                  \
        /* [in] */ IAnimationListener* animationListener);              \
                                                                        \
    CARAPI OnInterceptHoverEvent(                                       \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* like);                                     \
                                                                        \
    CARAPI SetMotionEventSplittingEnabled(                              \
        /* [in] */ Boolean split);                                      \
                                                                        \
    CARAPI IsMotionEventSplittingEnabled(                               \
        /* [out] */ Boolean* dispatched);                               \
                                                                        \
    CARAPI SetLayoutTransition(                                         \
        /* [in] */ ILayoutTransition* transition);                      \
                                                                        \
    CARAPI GetLayoutTransition(                                         \
        /* [out] */ ILayoutTransition** transition);                    \
                                                                        \
    CARAPI InvalidateChildFast(                                         \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* dirty);                                       \
                                                                        \
    CARAPI GetLayoutMode(                                               \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetLayoutMode(                                               \
        /* [in] */ Int32 layoutMode);                                   \
                                                                        \
    CARAPI StartViewTransition(                                         \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI EndViewTransition(                                           \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI RequestTransitionStart(                                      \
        /* [in] */ ILayoutTransition* transition);                      \
                                                                        \
    CARAPI ShouldDelayChildPressedState(                                \
        /* [out] */ Boolean* compatibility);


#define IVIEWGROUP_METHODS_IMPL(className, superClass)                  \
ECode className::GetDescendantFocusability(                             \
    /* [out] */ Int32* focusability)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(focusability);                                    \
    *focusability = superClass::GetDescendantFocusability();            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDescendantFocusability(                             \
    /* [in] */ Int32 focusability)                                      \
{                                                                       \
    return superClass::SetDescendantFocusability(focusability);        \
}                                                                       \
                                                                        \
ECode className::StartActionModeForChild(                               \
    /* [in] */ IView* originalView,                                     \
    /* [in] */ IActionModeCallback* cb,                                 \
    /* [out] */ IActionMode** mode)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    AutoPtr<IActionMode> m = superClass::StartActionModeForChild(originalView, cb); \
    *mode = m;                                                          \
    if (*mode) (*mode)->AddRef();                                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnRequestSendAccessibilityEvent(                       \
    /* [in] */ IView* child,                                            \
    /* [in] */ IAccessibilityEvent* event,                              \
    /* [out] */ Boolean* sent)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(sent);                                           \
    *sent = superClass::OnRequestSendAccessibilityEvent(child, event);  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ChildHasTransientStateChanged(                         \
    /* [in] */ IView* child,                                            \
    /* [in] */ Boolean childHasTransientState)                          \
{                                                                       \
    return superClass::ChildHasTransientStateChanged(child, childHasTransientState); \
}                                                                       \
                                                                        \
ECode className::GetFocusedChild(                                       \
    /* [out] */ IView** focusedChild)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(focusedChild);                                    \
    AutoPtr<IView> view = superClass::GetFocusedChild();                \
    *focusedChild = view;                                               \
    if (*focusedChild) {                                                \
        (*focusedChild)->AddRef();                                      \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnInterceptTouchEvent(                                 \
    /* [in] */ IMotionEvent* ev,                                        \
    /* [out] */ Boolean* value)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = superClass::OnInterceptTouchEvent(ev);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClipChildren(                                       \
    /* [in] */ Boolean clipChildren)                                    \
{                                                                       \
    superClass::SetClipChildren(clipChildren);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClipToPadding(                                      \
    /* [in] */ Boolean clipToPadding)                                   \
{                                                                       \
    superClass::SetClipToPadding(clipToPadding);                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AddView(                                               \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::AddView(child);                                  \
}                                                                       \
                                                                        \
ECode className::AddViewEx(                                             \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 index)                                             \
{                                                                       \
    return superClass::AddView(child, index);                           \
}                                                                       \
                                                                        \
ECode className::AddViewEx2(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 width,                                             \
    /* [in] */ Int32 height)                                            \
{                                                                       \
    return superClass::AddView(child, width ,height);                   \
}                                                                       \
                                                                        \
ECode className::AddViewEx3(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(child, params);                          \
}                                                                       \
                                                                        \
ECode className::AddViewEx4(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 index,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(child, index, params);                   \
}                                                                       \
                                                                        \
ECode className::SetOnHierarchyChangeListener(                          \
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)           \
{                                                                       \
    return superClass::SetOnHierarchyChangeListener(listener);          \
}                                                                       \
                                                                        \
ECode className::RemoveViewInLayout(                                    \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::RemoveViewInLayout(view);                        \
}                                                                       \
                                                                        \
ECode className::RemoveViewsInLayout(                                   \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 count)                                             \
{                                                                       \
    return superClass::RemoveViewsInLayout(start, count);               \
}                                                                       \
                                                                        \
ECode className::RemoveViewAt(                                          \
    /* [in] */ Int32 index)                                             \
{                                                                       \
    return superClass::RemoveViewAt(index);                             \
}                                                                       \
                                                                        \
ECode className::RemoveViews(                                           \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 count)                                             \
{                                                                       \
    return superClass::RemoveAllViews();                                \
}                                                                       \
                                                                        \
ECode className::RemoveAllViews()                                       \
{                                                                       \
    return superClass::RemoveAllViews();                                \
}                                                                       \
                                                                        \
ECode className::RemoveAllViewsInLayout()                               \
{                                                                       \
    return superClass::RemoveAllViewsInLayout();                        \
}                                                                       \
                                                                        \
ECode className::OffsetDescendantRectToMyCoords(                        \
    /* [in] */ IView* descendant,                                       \
    /* [in] */ IRect* rect)                                             \
{                                                                       \
    return superClass::OffsetDescendantRectToMyCoords(descendant, rect);\
}                                                                       \
                                                                        \
ECode className::OffsetRectIntoDescendantCoords(                        \
    /* [in] */ IView* descendant,                                       \
    /* [in] */ IRect* rect)                                             \
{                                                                       \
    return superClass::OffsetRectIntoDescendantCoords(descendant, rect);\
}                                                                       \
                                                                        \
ECode className::OffsetChildrenTopAndBottom(                            \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetChildrenTopAndBottom(offset);              \
}                                                                       \
                                                                        \
ECode className::StartLayoutAnimation()                                 \
{                                                                       \
    return superClass::StartLayoutAnimation();                          \
}                                                                       \
                                                                        \
ECode className::ScheduleLayoutAnimation()                              \
{                                                                       \
    return superClass::ScheduleLayoutAnimation();                       \
}                                                                       \
                                                                        \
ECode className::SetLayoutAnimation(                                    \
    /* [in] */ ILayoutAnimationController* controller)                  \
{                                                                       \
    return superClass::SetLayoutAnimation(controller);                  \
}                                                                       \
                                                                        \
ECode className::GetLayoutAnimation(                                    \
    /* [out] */ ILayoutAnimationController** controller)                \
{                                                                       \
    VALIDATE_NOT_NULL(controller);                                      \
    AutoPtr<ILayoutAnimationController> ctrl=                           \
            superClass::GetLayoutAnimation();                           \
    *controller = ctrl.Get();                                           \
    if (*controller) (*controller)->AddRef();                           \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsAnimationCacheEnabled(                               \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsAnimationCacheEnabled();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAnimationCacheEnabled(                              \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetAnimationCacheEnabled(enabled);               \
}                                                                       \
                                                                        \
ECode className::IsAlwaysDrawnWithCacheEnabled(                         \
    /* [out] */ Boolean* always)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(always);                                          \
    *always = superClass::IsAlwaysDrawnWithCacheEnabled();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAlwaysDrawnWithCacheEnabled(                        \
    /* [in] */ Boolean always)                                          \
{                                                                       \
    return superClass::SetAlwaysDrawnWithCacheEnabled(always);          \
}                                                                       \
                                                                        \
ECode className::GetPersistentDrawingCache(                             \
    /* [out] */ Int32* drawingCacheToKeep)                              \
{                                                                       \
    VALIDATE_NOT_NULL(drawingCacheToKeep);                              \
    *drawingCacheToKeep = superClass::GetPersistentDrawingCache();      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPersistentDrawingCache(                             \
    /* [in] */ Int32 drawingCacheToKeep)                                \
{                                                                       \
    return superClass::SetPersistentDrawingCache(drawingCacheToKeep);   \
}                                                                       \
                                                                        \
ECode className::GenerateLayoutParams(                                  \
    /* [in] */ IAttributeSet* attrs,                                    \
    /* [out] */ IViewGroupLayoutParams** layoutParams)                  \
{                                                                       \
    VALIDATE_NOT_NULL(layoutParams);                                    \
                                                                        \
    return superClass::GenerateLayoutParams(attrs, layoutParams);       \
}                                                                       \
                                                                        \
ECode className::IndexOfChild(                                          \
    /* [in] */ IView* child,                                            \
    /* [out] */ Int32* index)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(index);                                           \
    *index = superClass::IndexOfChild(child);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetChildCount(                                         \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
                                                                        \
    *count = superClass::GetChildCount();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetChildAt(                                            \
    /* [in] */ Int32 index,                                             \
    /* [out] */ IView** child)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(child);                                           \
                                                                        \
    AutoPtr<IView> _child = ViewGroup::GetChildAt(index);               \
    *child = _child;                                                    \
                                                                        \
    if (*child != NULL) {                                               \
        (*child)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ClearDisappearingChildren()                            \
{                                                                       \
    return superClass::ClearDisappearingChildren();                     \
}                                                                       \
                                                                        \
ECode className::GetLayoutAnimationListener(                            \
    /* [out] */ IAnimationListener** listener)                          \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                        \
    AutoPtr<IAnimationListener> l =                                     \
            superClass::GetLayoutAnimationListener();                   \
    *listener = l.Get();                                                \
    if (*listener) (*listener)->AddRef();                               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAddStatesFromChildren(                              \
    /* [in] */ Boolean addsStates)                                      \
{                                                                       \
    return superClass::SetAddStatesFromChildren(addsStates);            \
}                                                                       \
                                                                        \
ECode className::AddStatesFromChildren(                                 \
    /* [out] */ Boolean* drawable)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    *drawable = superClass::AddStatesFromChildren();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutAnimationListener(                            \
    /* [in] */ IAnimationListener* animationListener)                   \
{                                                                       \
    return superClass::SetLayoutAnimationListener(animationListener);  \
}                                                                       \
                                                                        \
ECode className::OnInterceptHoverEvent(                                 \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* like)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(like);                                            \
    *like = superClass::OnInterceptHoverEvent(event);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetMotionEventSplittingEnabled(                        \
    /* [in] */ Boolean split)                                           \
{                                                                       \
    return superClass::SetMotionEventSplittingEnabled(split);           \
}                                                                       \
                                                                        \
ECode className::IsMotionEventSplittingEnabled(                         \
    /* [out] */ Boolean* dispatched)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(dispatched);                                      \
    *dispatched = superClass::IsMotionEventSplittingEnabled();          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutTransition(                                   \
    /* [in] */ ILayoutTransition* transition)                           \
{                                                                       \
    return superClass::SetLayoutTransition(transition);                 \
}                                                                       \
                                                                        \
ECode className::GetLayoutTransition(                                   \
    /* [out] */ ILayoutTransition** transition)                         \
{                                                                       \
    VALIDATE_NOT_NULL(transition);                                      \
    AutoPtr<ILayoutTransition> ransition = superClass::GetLayoutTransition();   \
    if (*transition != NULL) {                                          \
        (*transition)->AddRef();                                        \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::InvalidateChildFast(                                   \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* dirty)                                            \
{                                                                       \
    return superClass::InvalidateChildFast(child, dirty);               \
}                                                                       \
                                                                        \
ECode className::GetLayoutMode(                                         \
    /* [out] */ Int32* layoutMode)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(layoutMode);                                      \
    *layoutMode = superClass::GetLayoutMode();                          \
    return NOERROR;                                                    \
}                                                                       \
                                                                        \
ECode className::SetLayoutMode(                                         \
    /* [in] */ Int32 layoutMode)                                        \
{                                                                       \
    return superClass::SetLayoutMode(layoutMode);                      \
}                                                                       \
                                                                        \
ECode className::StartViewTransition(                                   \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::StartViewTransition(view);                       \
}                                                                       \
                                                                        \
ECode className::EndViewTransition(                                     \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::EndViewTransition(view);                         \
}                                                                       \
                                                                        \
ECode className::RequestTransitionStart(                                \
    /* [in] */ ILayoutTransition* transition)                           \
{                                                                       \
    return superClass::RequestTransitionStart(transition);             \
}                                                                       \
                                                                        \
ECode className::ShouldDelayChildPressedState(                          \
    /* [out] */ Boolean* compatibility)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(compatibility);                                   \
    *compatibility = superClass::ShouldDelayChildPressedState();        \
                                                                        \
    return NOERROR;                                                    \
}


#define IVIEWPARENT_METHODS_DECL()                                      \
    CARAPI RequestLayoutEx();                                           \
                                                                        \
    CARAPI IsLayoutRequestedEx(                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestTransparentRegion(                                    \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI InvalidateChild(                                             \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* r);                                           \
                                                                        \
    CARAPI InvalidateChildInParent(                                     \
        /* [in] */ ArrayOf<Int32>* location,                            \
        /* [in] */ IRect* r,                                            \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI GetParentEx(                                                 \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI RequestChildFocus(                                           \
        /* [in] */ IView* child,                                        \
        /* [in] */ IView* focused);                                     \
                                                                        \
    CARAPI RecomputeViewAttributes(                                     \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI ClearChildFocus(                                             \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI GetChildVisibleRect(                                         \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* r,                                            \
        /* [in] */ IPoint* offset,                                      \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI FocusSearchEx(                                               \
        /* [in] */ IView* focused,                                      \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IView** focus);                                     \
                                                                        \
    CARAPI BringChildToFront(                                           \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI FocusableViewAvailable(                                      \
        /* [in] */ IView* v);                                           \
                                                                        \
    CARAPI ShowContextMenuForChild(                                     \
        /* [in] */ IView* originalView,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CreateContextMenuEx(                                         \
        /* [in] */ IContextMenu* menu);                                 \
                                                                        \
    CARAPI StartActionModeForChildEx(                                   \
        /* [in] */ IView* originalView,                                 \
        /* [in] */ IActionModeCallback* callback,                       \
        /* [out] */ IActionMode** mode);                                \
                                                                        \
    CARAPI ChildDrawableStateChanged(                                   \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI RequestDisallowInterceptTouchEvent(                          \
        /* [in] */ Boolean disallowIntercept);                          \
                                                                        \
    CARAPI RequestChildRectangleOnScreen(                               \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* rectangle,                                    \
        /* [in] */ Boolean immediate,                                   \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestSendAccessibilityEvent(                               \
        /* [in] */ IView* child,                                        \
        /* [in] */ IAccessibilityEvent* event,                          \
        /* [out] */ Boolean* sent);                                     \
                                                                        \
    CARAPI ChildHasTransientStateChangedEx(                             \
        /* [in] */ IView* child,                                        \
        /* [in] */ Boolean hasTransientState);                          \
                                                                        \
    CARAPI RequestFitSystemWindowsEx();                                 \
                                                                        \
    CARAPI GetParentForAccessibilityEx(                                 \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI ChildAccessibilityStateChanged(                              \
        /* [in] */ IView* child);


#define IVIEWPARENT_METHODS_IMPL(className, superClass)                 \
ECode className::RequestLayoutEx()                                      \
{                                                                       \
    return superClass::RequestLayout();                                 \
}                                                                       \
                                                                        \
ECode className::IsLayoutRequestedEx(                                   \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsLayoutRequested();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestTransparentRegion(                              \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::RequestTransparentRegion(child);                 \
}                                                                       \
                                                                        \
ECode className::InvalidateChild(                                       \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* r)                                                \
{                                                                       \
    return superClass::InvalidateChild(child, r);                       \
}                                                                       \
                                                                        \
ECode className::InvalidateChildInParent(                               \
    /* [in] */ ArrayOf<Int32>* location,                                \
    /* [in] */ IRect* r,                                                \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> p = superClass::InvalidateChildInParent(location, r);\
    *parent = p;                                                        \
    if (*parent) (*parent)->AddRef();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParentEx(                                           \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> p = superClass::GetParent();                   \
    *parent = p;                                                        \
    if (*parent) (*parent)->AddRef();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestChildFocus(                                     \
    /* [in] */ IView* child,                                            \
    /* [in] */ IView* focused)                                          \
{                                                                       \
    return superClass::RequestChildFocus(child, focused);               \
}                                                                       \
                                                                        \
ECode className::RecomputeViewAttributes(                               \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::RecomputeViewAttributes(child);                  \
}                                                                       \
                                                                        \
ECode className::ClearChildFocus(                                       \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::ClearChildFocus(child);                          \
}                                                                       \
                                                                        \
ECode className::GetChildVisibleRect(                                   \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* r,                                                \
    /* [in] */ IPoint* offset,                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    return superClass::GetChildVisibleRect(child, r, offset, result);   \
}                                                                       \
                                                                        \
ECode className::FocusSearchEx(                                         \
    /* [in] */ IView* focused,                                          \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IView** focus)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(focus);                                           \
    AutoPtr<IView> v = superClass::FocusSearch(focused, direction);     \
    *focus = v;                                                         \
    if (*focus) (*focus)->AddRef();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::BringChildToFront(                                     \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::BringChildToFront(child);                        \
}                                                                       \
                                                                        \
ECode className::FocusableViewAvailable(                                \
    /* [in] */ IView* v)                                                \
{                                                                       \
    return superClass::FocusableViewAvailable(v);                       \
}                                                                       \
                                                                        \
ECode className::ShowContextMenuForChild(                               \
    /* [in] */ IView* originalView,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ShowContextMenuForChild(originalView);        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateContextMenuEx(                                   \
    /* [in] */ IContextMenu* menu)                                      \
{                                                                       \
    return superClass::CreateContextMenu(menu);                         \
}                                                                       \
                                                                        \
ECode className::StartActionModeForChildEx(                             \
    /* [in] */ IView* originalView,                                     \
    /* [in] */ IActionModeCallback* callback,                           \
    /* [out] */ IActionMode** mode)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    AutoPtr<IActionMode> m = superClass::StartActionModeForChild(originalView, callback);  \
    *mode = m.Get();                                                    \
    if (*mode != NULL) {                                                \
        (*mode)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ChildDrawableStateChanged(                             \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::ChildDrawableStateChanged(child);                \
}                                                                       \
                                                                        \
ECode className::RequestDisallowInterceptTouchEvent(                    \
    /* [in] */ Boolean disallowIntercept)                               \
{                                                                       \
    return superClass::RequestDisallowInterceptTouchEvent(disallowIntercept);   \
}                                                                       \
                                                                        \
ECode className::RequestChildRectangleOnScreen(                         \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* rectangle,                                        \
    /* [in] */ Boolean immediate,                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestChildRectangleOnScreen(child, rectangle, immediate);   \
                                                                        \
    return NOERROR;                                                    \
}                                                                       \
                                                                        \
ECode className::RequestSendAccessibilityEvent(                         \
    /* [in] */ IView* child,                                            \
    /* [in] */ IAccessibilityEvent* event,                              \
    /* [out] */ Boolean* sent)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(sent);                                            \
    *sent = superClass::RequestSendAccessibilityEvent(child, event);    \
                                                                        \
    return NOERROR;                                                    \
}                                                                       \
                                                                        \
ECode className::ChildHasTransientStateChangedEx(                       \
    /* [in] */ IView* child,                                            \
    /* [in] */ Boolean childHasTransientState)                          \
{                                                                       \
    return superClass::ChildHasTransientStateChanged(child, childHasTransientState); \
}                                                                       \
                                                                        \
ECode className::RequestFitSystemWindowsEx()                            \
{                                                                       \
    return superClass::RequestFitSystemWindows();                    \
}                                                                       \
                                                                        \
ECode className::GetParentForAccessibilityEx(                           \
    /* [in] */ IViewParent** parent)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> v = superClass::GetParentForAccessibility(); \
    *parent = v;                                                        \
    if (*parent) (*parent)->AddRef();                                   \
                                                                        \
    return NOERROR;                                                    \
}                                                                       \
                                                                        \
ECode className::ChildAccessibilityStateChanged(                        \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::ChildAccessibilityStateChanged(child);          \
}

#define IVIEWMANAGER_METHODS_DECL()                                     \
    CARAPI AddViewEx5(                                                  \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI UpdateViewLayout(                                            \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI RemoveView(                                                  \
        /* [in] */ IView* view);

#define IVIEWMANAGER_METHODS_IMPL(className, superClass)                \
ECode className::AddViewEx5(                                            \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(view, params);                           \
}                                                                       \
                                                                        \
ECode className::UpdateViewLayout(                                      \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::UpdateViewLayout(view, params);                  \
}                                                                       \
                                                                        \
ECode className::RemoveView(                                            \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::RemoveView(view);                                \
}

#define IDRAWABLECALLBACK_METHODS_DECL()                                \
    CARAPI InvalidateDrawable(                                          \
        /* [in] */ IDrawable* drawable);                                \
                                                                        \
    CARAPI ScheduleDrawable(                                            \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what,                                     \
        /* [in] */ Int64 when);                                         \
                                                                        \
    CARAPI UnscheduleDrawable(                                          \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what);

#define IDRAWABLECALLBACK_METHODS_IMPL(className, superClass)           \
ECode className::InvalidateDrawable(                                    \
    /* [in] */ IDrawable* drawable)                                     \
{                                                                       \
    return superClass::InvalidateDrawable(drawable);                    \
}                                                                       \
                                                                        \
ECode className::ScheduleDrawable(                                      \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what,                                         \
    /* [in] */ Int64 when)                                              \
{                                                                       \
    return superClass::ScheduleDrawable(who, what, when);               \
}                                                                       \
                                                                        \
ECode className::UnscheduleDrawable(                                    \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what)                                         \
{                                                                       \
    return superClass::UnscheduleDrawable(who, what);                   \
}

#define IKEYEVENTCALLBACK_METHODS_DECL()                                \
    CARAPI OnKeyDown(                                                   \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyLongPress(                                              \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyUp(                                                     \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyMultiple(                                               \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ Int32 count,                                         \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);

#define IKEYEVENTCALLBACK_METHODS_IMPL(className, superClass)           \
ECode className::OnKeyDown(                                             \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyDown(keyCode, event);                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyLongPress(                                        \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyLongPress(keyCode, event);               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyUp(                                               \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyUp(keyCode, event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyMultiple(                                         \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ Int32 repeatCount,                                       \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyMultiple(keyCode, repeatCount, event);   \
                                                                        \
    return NOERROR;                                                     \
}

#define IACCESSIBILITYEVENTSOURCE_METHODS_DECL()                        \
    CARAPI SendAccessibilityEvent(                                      \
        /* [in] */ Int32 eventType);                                    \
                                                                        \
    CARAPI SendAccessibilityEventUnchecked(                             \
        /* [in] */ IAccessibilityEvent* event);

#define IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(className, superClass)   \
ECode className::SendAccessibilityEvent(                                \
    /* [in] */ Int32 eventType)                                         \
{                                                                       \
    return superClass::SendAccessibilityEvent(eventType);               \
}                                                                       \
                                                                        \
ECode className::SendAccessibilityEventUnchecked(                       \
    /* [in] */ IAccessibilityEvent* event)                              \
{                                                                       \
    return superClass::SendAccessibilityEventUnchecked(event);          \
}


#endif  //__VIEWMACRO_H__
