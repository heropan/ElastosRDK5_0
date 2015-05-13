
#ifndef __ABSLISTVIEWMACRO_H__
#define __ABSLISTVIEWMACRO_H__

using Elastos::Core::ICharSequence;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::IAbsListView;
using Elastos::Droid::Widget::IRemoteViewsOnClickHandler;
using Elastos::Droid::Text::IEditable;

#define IABSLISTVIEW_INTERFACE_METHODS_DECL()                                     \
    CARAPI BeforeTextChanged(                                                     \
        /* [in] */  ICharSequence* s,                                             \
        /* [in] */  Int32 start,                                                  \
        /* [in] */  Int32 count,                                                  \
        /* [in] */  Int32 after);                                                 \
                                                                                  \
    CARAPI OnTextChanged(                                                         \
        /* [in] */  ICharSequence* s,                                             \
        /* [in] */  Int32 start,                                                  \
        /* [in] */  Int32 before,                                                 \
        /* [in] */  Int32 count);                                                 \
                                                                                  \
    CARAPI AfterTextChanged(                                                      \
        /* [in] */  IEditable* s);                                                \
                                                                                  \
    CARAPI OnGlobalLayout();                                                      \
                                                                                  \
    CARAPI OnTouchModeChanged(                                                    \
        /* [in] */  Boolean isInTouchMode);                                       \
                                                                                  \
    CARAPI OnFilterComplete(                                                      \
        /* [in] */  Int32 count);                                                 \
                                                                                  \
    CARAPI OnRemoteAdapterConnected(                                              \
        /* [out] */  Boolean* rst);                                               \
                                                                                  \
    CARAPI OnRemoteAdapterDisconnected();                                         \
                                                                                  \
    CARAPI DeferNotifyDataSetChanged();

#define IABSLISTVIEW_INTERFACE_METHODS_IMPL(className, superClass)                \
ECode className::BeforeTextChanged(                                               \
    /* [in] */  ICharSequence* s,                                                 \
    /* [in] */  Int32 start,                                                      \
    /* [in] */  Int32 count,                                                      \
    /* [in] */  Int32 after)                                                      \
{                                                                                 \
    return superClass::BeforeTextChanged(s, start, count, after);                 \
}                                                                                 \
                                                                                  \
ECode className::OnTextChanged(                                                   \
    /* [in] */  ICharSequence* s,                                                 \
    /* [in] */  Int32 start,                                                      \
    /* [in] */  Int32 before,                                                     \
    /* [in] */  Int32 count)                                                      \
{                                                                                 \
    return superClass::OnTextChanged(s, start, before, count);                    \
}                                                                                 \
                                                                                  \
ECode className::AfterTextChanged(                                                \
    /* [in] */  IEditable* s)                                                     \
{                                                                                 \
    return superClass::AfterTextChanged(s);                                       \
}                                                                                 \
                                                                                  \
ECode className::OnGlobalLayout()                                                 \
{                                                                                 \
    return superClass::OnGlobalLayout();                                          \
}                                                                                 \
                                                                                  \
ECode className::OnTouchModeChanged(                                              \
    /* [in] */  Boolean isInTouchMode)                                            \
{                                                                                 \
    return superClass::OnTouchModeChanged(isInTouchMode);                         \
}                                                                                 \
                                                                                  \
ECode className::OnFilterComplete(                                                \
    /* [in] */  Int32 count)                                                      \
{                                                                                 \
    return superClass::OnFilterComplete(count);                                   \
}                                                                                 \
                                                                                  \
ECode className::OnRemoteAdapterConnected(                                        \
    /* [out] */ Boolean* rst)                                                     \
{                                                                                 \
    VALIDATE_NOT_NULL(rst);                                                       \
    *rst = superClass::OnRemoteAdapterConnected();                                \
    return NOERROR;                                                               \
}                                                                                 \
                                                                                  \
ECode className::OnRemoteAdapterDisconnected()                                    \
{                                                                                 \
    return superClass::OnRemoteAdapterDisconnected();                             \
}                                                                                 \
                                                                                  \
ECode className::DeferNotifyDataSetChanged()                                      \
{                                                                                 \
    return superClass::DeferNotifyDataSetChanged();                               \
}

#define IABSLISTVIEW_METHODS_DECL()                                     \
    CARAPI SetFastScrollEnabled(                                        \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsFastScrollEnabled(                                         \
        /* [out] */ Boolean* isEnabled);                                \
                                                                        \
    CARAPI SetSmoothScrollbarEnabled(                                   \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsSmoothScrollbarEnabled(                                    \
        /* [out] */ Boolean* isEnabled);                                \
                                                                        \
    CARAPI SetOnScrollListener(                                         \
        /* [in] */ IAbsListViewOnScrollListener* l);                    \
                                                                        \
    CARAPI IsScrollingCacheEnabled(                                     \
        /* [out] */ Boolean* isEnabled);                                \
                                                                        \
    CARAPI SetScrollingCacheEnabled(                                    \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI SetTextFilterEnabled(                                        \
        /* [in] */ Boolean textFilterEnabled);                          \
                                                                        \
    CARAPI IsTextFilterEnabled(                                         \
        /* [out] */ Boolean* isEnabled);                                \
                                                                        \
    CARAPI IsStackFromBottom(                                           \
        /* [out] */ Boolean* isStackFromBottom);                        \
                                                                        \
    CARAPI SetStackFromBottom(                                          \
        /* [in] */ Boolean stackFromBottom);                            \
                                                                        \
    CARAPI SetFilterText(                                               \
        /* [in] */ const String& filterText);                                  \
                                                                        \
    CARAPI GetTextFilter(                                               \
        /* [out] */ ICharSequence** textFilter);                        \
                                                                        \
    CARAPI GetListPaddingTop(                                           \
        /* [out] */ Int32* top);                                        \
                                                                        \
    CARAPI GetListPaddingBottom(                                        \
        /* [out] */ Int32* bottom);                                     \
                                                                        \
    CARAPI GetListPaddingLeft(                                          \
        /* [out] */ Int32* left);                                       \
                                                                        \
    CARAPI GetListPaddingRight(                                         \
        /* [out] */ Int32* right);                                      \
                                                                        \
    CARAPI SetDrawSelectorOnTop(                                        \
        /* [in] */ Boolean onTop);                                      \
                                                                        \
    CARAPI SetSelector(                                                 \
        /* [in] */ Int32 resID);                                        \
                                                                        \
    CARAPI SetSelectorEx(                                               \
        /* [in] */ IDrawable* sel);                                     \
                                                                        \
    CARAPI GetSelector(                                                 \
        /* [out] */ IDrawable** selector);                              \
                                                                        \
    CARAPI SetScrollIndicators(                                         \
        /* [in] */ IView* up,                                           \
        /* [in] */ IView* down);                                        \
                                                                        \
    CARAPI PointToPosition(                                             \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y,                                             \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI PointToRowId(                                                \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y,                                             \
        /* [out] */ Int64* rowId);                                      \
                                                                        \
    CARAPI SmoothScrollToPosition(                                      \
        /* [in] */ Int32 position);                                     \
                                                                        \
    CARAPI SmoothScrollToPositionEx(                                    \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Int32 boundPosition);                                \
                                                                        \
    CARAPI SmoothScrollBy(                                              \
        /* [in] */ Int32 distance,                                      \
        /* [in] */ Int32 duration);                                     \
                                                                        \
    CARAPI InvalidateViews();                                           \
                                                                        \
    CARAPI ClearTextFilter();                                           \
                                                                        \
    CARAPI HasTextFilter(                                               \
        /* [out] */ Boolean* hasTextFilter);                            \
                                                                        \
    CARAPI SetTranscriptMode(                                           \
        /* [in] */ Int32 mode);                                         \
                                                                        \
    CARAPI GetTranscriptMode(                                           \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetCacheColorHint(                                           \
        /* [in] */ Int32 color);                                        \
                                                                        \
    CARAPI GetCacheColorHint(                                           \
        /* [out] */ Int32* colorHint);                                  \
                                                                        \
    CARAPI ReclaimViews(                                                \
        /* [in] */ IObjectContainer* views);                            \
                                                                        \
    CARAPI SetRecyclerListener(                                         \
        /* [in] */ IRecyclerListener* listener);                        \
                                                                        \
    CARAPI GetCheckedItemCount(                                         \
            /* [out] */ Int32* listener);                               \
                                                                        \
    CARAPI IsItemChecked(                                               \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Boolean* checked);                                  \
                                                                        \
    CARAPI GetCheckedItemPosition(                                      \
        /* [out] */ Int32* pos);                                        \
                                                                        \
    CARAPI GetCheckedItemPositions(                                     \
        /* [out] */ IObjectInt32Map** arr);                           \
                                                                        \
    CARAPI GetCheckedItemIds(                                           \
        /* [out, callee] */ ArrayOf<Int64>** arr);                      \
                                                                        \
    CARAPI ClearChoices();                                              \
                                                                        \
    CARAPI SetItemChecked(                                              \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Boolean value);                                      \
                                                                        \
    CARAPI GetChoiceMode(                                               \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetChoiceMode(                                               \
        /* [in] */ Int32 choiceMode);                                   \
                                                                        \
    CARAPI SetMultiChoiceModeListener(                                  \
        /* [in] */ IMultiChoiceModeListener* listener);                 \
                                                                        \
    CARAPI SetFastScrollAlwaysVisible(                                  \
        /* [in] */ Boolean alwaysShow);                                 \
                                                                        \
    CARAPI IsFastScrollAlwaysVisible(                                   \
        /* [out] */ Boolean* visible);                                  \
                                                                        \
    CARAPI SetFriction(                                                 \
        /* [in] */ Float friction);                                     \
                                                                        \
    CARAPI SetVelocityScale(                                            \
        /* [in] */ Float scale);                                        \
                                                                        \
    CARAPI SmoothScrollToPositionFromTop(                               \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Int32 offset,                                        \
        /* [in] */ Int32 duration);                                     \
                                                                        \
    CARAPI SmoothScrollToPositionFromTopEx(                             \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI SetRemoteViewsAdapter(                                       \
        /* [in] */ IIntent* intent);                                    \
                                                                        \
    CARAPI SetRemoteViewsOnClickHandler(                                \
        /* [in] */ IRemoteViewsOnClickHandler* handler);

#define IABSLISTVIEW_METHODS_IMPL(className, superClass)               \
ECode className::SetFastScrollEnabled(                                  \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetFastScrollEnabled(enabled);                   \
}                                                                       \
                                                                        \
ECode className::IsFastScrollEnabled(                                   \
    /* [out] */ Boolean* isEnabled)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isEnabled);                                       \
                                                                        \
    *isEnabled = superClass::IsFastScrollEnabled();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSmoothScrollbarEnabled(                             \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetSmoothScrollbarEnabled(enabled);              \
}                                                                       \
                                                                        \
ECode className::IsSmoothScrollbarEnabled(                              \
    /* [out] */ Boolean* isEnabled)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isEnabled);                                       \
    *isEnabled = superClass::IsSmoothScrollbarEnabled();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnScrollListener(                                   \
    /* [in] */ IAbsListViewOnScrollListener* l)                         \
{                                                                       \
    return superClass::SetOnScrollListener(l);                          \
}                                                                       \
                                                                        \
ECode className::IsScrollingCacheEnabled(                               \
    /* [out] */ Boolean* isEnabled)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isEnabled);                                       \
    *isEnabled = superClass::IsScrollingCacheEnabled();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollingCacheEnabled(                              \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetScrollingCacheEnabled(enabled);               \
}                                                                       \
                                                                        \
ECode className::SetTextFilterEnabled(                                  \
    /* [in] */ Boolean textFilterEnabled)                               \
{                                                                       \
    return superClass::SetTextFilterEnabled(textFilterEnabled);         \
}                                                                       \
                                                                        \
ECode className::IsTextFilterEnabled(                                   \
    /* [out] */ Boolean* isEnabled)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isEnabled);                                       \
    *isEnabled = superClass::IsTextFilterEnabled();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsStackFromBottom(                                     \
    /* [out] */ Boolean* isStackFromBottom)                             \
{                                                                       \
    VALIDATE_NOT_NULL(isStackFromBottom);                               \
    *isStackFromBottom = superClass::IsStackFromBottom();               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetStackFromBottom(                                    \
    /* [in] */ Boolean stackFromBottom)                                 \
{                                                                       \
    return superClass::SetStackFromBottom(stackFromBottom);             \
}                                                                       \
                                                                        \
ECode className::SetFilterText(                                         \
    /* [in] */ const String& filterText)                                       \
{                                                                       \
    return superClass::SetFilterText(filterText);                       \
}                                                                       \
                                                                        \
ECode className::GetTextFilter(                                         \
    /* [out] */ ICharSequence** textFilter)                             \
{                                                                       \
    VALIDATE_NOT_NULL(textFilter);                                      \
    AutoPtr<ICharSequence> temp = superClass::GetTextFilter();          \
    *textFilter = temp;                                                 \
    if (*textFilter) {                                                  \
        (*textFilter)->AddRef();                                        \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetListPaddingTop(                                     \
    /* [out] */ Int32* top)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(top);                                             \
    *top = superClass::GetListPaddingTop();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetListPaddingBottom(                                  \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bottom);                                          \
    *bottom = superClass::GetListPaddingBottom();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetListPaddingLeft(                                    \
    /* [out] */ Int32* left)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    *left = superClass::GetListPaddingLeft();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetListPaddingRight(                                   \
    /* [out] */ Int32* right)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(right);                                           \
    *right = superClass::GetListPaddingRight();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDrawSelectorOnTop(                                  \
    /* [in] */ Boolean onTop)                                           \
{                                                                       \
    return superClass::SetDrawSelectorOnTop(onTop);                     \
}                                                                       \
                                                                        \
ECode className::SetSelector(                                           \
    /* [in] */ Int32 resID)                                             \
{                                                                       \
    return superClass::SetSelector(resID);                              \
}                                                                       \
                                                                        \
ECode className::SetSelectorEx(                                         \
    /* [in] */ IDrawable* sel)                                          \
{                                                                       \
    return superClass::SetSelector(sel);                                \
}                                                                       \
                                                                        \
ECode className::GetSelector(                                           \
    /* [out] */ IDrawable** selector)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(selector);                                        \
    AutoPtr<IDrawable> temp = superClass::GetSelector();                \
    *selector = temp;                                                   \
    if (*selector) {                                                    \
        (*selector)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollIndicators(                                   \
    /* [in] */ IView* up,                                               \
    /* [in] */ IView* down)                                             \
{                                                                       \
    return superClass::SetScrollIndicators(up, down);                   \
}                                                                       \
                                                                        \
ECode className::PointToPosition(                                       \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y,                                                 \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::PointToPosition(x, y);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PointToRowId(                                          \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y,                                                 \
    /* [out] */ Int64* rowId)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(rowId);                                           \
    *rowId = superClass::PointToRowId(x, y);                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SmoothScrollToPosition(                                \
    /* [in] */ Int32 position)                                          \
{                                                                       \
    return superClass::SmoothScrollToPosition(position);                \
}                                                                       \
                                                                        \
ECode className::SmoothScrollToPositionEx(                              \
    /* [in] */ Int32 position,                                          \
    /* [in] */ Int32 boundPosition)                                     \
{                                                                       \
    return superClass::SmoothScrollToPosition(position, boundPosition); \
}                                                                       \
                                                                        \
ECode className::SmoothScrollBy(                                        \
    /* [in] */ Int32 distance,                                          \
    /* [in] */ Int32 duration)                                          \
{                                                                       \
    return superClass::SmoothScrollBy(distance, duration);              \
}                                                                       \
                                                                        \
ECode className::InvalidateViews()                                      \
{                                                                       \
    return superClass::InvalidateViews();                               \
}                                                                       \
                                                                        \
ECode className::ClearTextFilter()                                      \
{                                                                       \
    return superClass::ClearTextFilter();                               \
}                                                                       \
                                                                        \
ECode className::HasTextFilter(                                         \
    /* [out] */ Boolean* hasTextFilter)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(hasTextFilter);                                   \
    *hasTextFilter = superClass::HasTextFilter();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetTranscriptMode(                                     \
    /* [in] */ Int32 mode)                                              \
{                                                                       \
    return superClass::SetTranscriptMode(mode);                         \
}                                                                       \
                                                                        \
ECode className::GetTranscriptMode(                                     \
    /* [out] */ Int32* mode)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    *mode = superClass::GetTranscriptMode();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetCacheColorHint(                                     \
    /* [in] */ Int32 color)                                             \
{                                                                       \
    return superClass::SetCacheColorHint(color);                        \
}                                                                       \
                                                                        \
ECode className::GetCacheColorHint(                                     \
    /* [out] */ Int32* colorHint)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(colorHint);                                       \
    *colorHint = superClass::GetCacheColorHint();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ReclaimViews(                                          \
    /* [in] */ IObjectContainer* views)                                 \
{                                                                       \
    return superClass::ReclaimViews(views);                             \
}                                                                       \
                                                                        \
ECode className::SetRecyclerListener(                                   \
    /* [in] */ IRecyclerListener* listener)                             \
{                                                                       \
    return superClass::SetRecyclerListener(listener);                   \
}                                                                        \
                                                                         \
ECode className::GetCheckedItemCount(                               \
    /* [out] */ Int32* listener)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                       \
    *listener = superClass::GetCheckedItemCount();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                    \
ECode className::IsItemChecked(                                     \
    /* [in] */ Int32 position,                                      \
    /* [out] */ Boolean* checked)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(checked);                                       \
    return superClass::IsItemChecked(position, checked);          \
}                                                                       \
                                                                    \
ECode className::GetCheckedItemPosition(                            \
    /* [out] */ Int32* pos)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                       \
    return superClass::GetCheckedItemPosition(pos);                       \
}                                                                       \
                                                                    \
ECode className::GetCheckedItemPositions(                           \
    /* [out] */ IObjectInt32Map** arr)                           \
{                                                                   \
    VALIDATE_NOT_NULL(arr);                                                 \
    AutoPtr<IObjectInt32Map> l = superClass::GetCheckedItemPositions();   \
    *arr = l.Get();                                              \
    INTERFACE_ADDREF(*arr);                                         \
    return NOERROR;                                                \
}                                                                   \
                                                                    \
ECode className::GetCheckedItemIds(                                 \
    /* [out, callee] */ ArrayOf<Int64>** arr)                      \
{                                                                   \
    return superClass::GetCheckedItemIds(arr);                     \
}                                                                   \
                                                                    \
ECode className::ClearChoices()                                    \
{                                                                   \
    return superClass::ClearChoices();                              \
}                                                                   \
                                                                    \
ECode className::SetItemChecked(                                    \
    /* [in] */ Int32 position,                                      \
    /* [in] */ Boolean value)                                      \
{                                                                   \
    return superClass::SetItemChecked(position, value);            \
}                                                                   \
                                                                    \
ECode className::GetChoiceMode(                                     \
    /* [out] */ Int32* mode)                                       \
{                                                                  \
    VALIDATE_NOT_NULL(mode);                                       \
    return superClass::GetChoiceMode(mode);                       \
}                                                                   \
                                                                    \
ECode className::SetChoiceMode(                                     \
    /* [in] */ Int32 choiceMode)                                   \
{                                                                   \
    return superClass::SetChoiceMode(choiceMode);                   \
}                                                                   \
                                                                    \
ECode className::SetMultiChoiceModeListener(                        \
    /* [in] */ IMultiChoiceModeListener* listener)                 \
{                                                                   \
    return superClass::SetMultiChoiceModeListener(listener);       \
}                                                                   \
                                                                    \
ECode className::SetFastScrollAlwaysVisible(                        \
    /* [in] */ Boolean alwaysShow)                                 \
{                                                                   \
    return superClass::SetFastScrollAlwaysVisible(alwaysShow);     \
}                                                                   \
                                                                    \
ECode className::IsFastScrollAlwaysVisible(                         \
    /* [out] */ Boolean* visible)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(visible);                                       \
    *visible = superClass::IsFastScrollAlwaysVisible();             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                    \
ECode className::SetFriction(                                       \
    /* [in] */ Float friction)                                     \
{                                                                   \
    return superClass::SetFriction(friction);                     \
}                                                                   \
                                                                    \
ECode className::SetVelocityScale(                                  \
    /* [in] */ Float scale)                                        \
{                                                                   \
    return superClass::SetVelocityScale(scale);                     \
}                                                                   \
                                                                    \
ECode className::SmoothScrollToPositionFromTop(                     \
    /* [in] */ Int32 position,                                      \
    /* [in] */ Int32 offset,                                        \
    /* [in] */ Int32 duration)                                     \
{                                                                   \
    return superClass::SmoothScrollToPositionFromTop(position, offset, duration); \
}                                                                   \
                                                                    \
ECode className::SmoothScrollToPositionFromTopEx(                   \
    /* [in] */ Int32 position,                                      \
    /* [in] */ Int32 offset)                                       \
{                                                                   \
    return superClass::SmoothScrollToPositionFromTop(position, offset); \
}                                                                   \
                                                                    \
ECode className::SetRemoteViewsAdapter(                             \
    /* [in] */ IIntent* intent)                                    \
{                                                                   \
    return superClass::SetRemoteViewsAdapter(intent);              \
}                                                                   \
                                                                    \
ECode className::SetRemoteViewsOnClickHandler(                      \
    /* [in] */ IRemoteViewsOnClickHandler* handler)                  \
{                                                                   \
    return superClass::SetRemoteViewsOnClickHandler(handler);       \
}                                                                   \


#endif  //__ABSLISTVIEWMACRO_H__
