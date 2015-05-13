
#ifndef __TEXTVIEWMACRO_H__
#define __TEXTVIEWMACRO_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Libcore::ICU::ILocale;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Text::TextUtilsTruncateAt;
using Elastos::Droid::Text::ISpannableFactory;
using Elastos::Droid::Text::IEditableFactory;
using Elastos::Droid::Text::ITextPaint;
using Elastos::Droid::Text::ILayout;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::IInputFilter;
using Elastos::Droid::Text::Method::IWordIterator;
using Elastos::Droid::Text::Method::IMovementMethod;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::ICorrectionInfo;
using Elastos::Droid::Widget::BufferType;
using Elastos::Droid::Widget::IOnEditorActionListener;
using Elastos::Droid::Widget::IScroller;

#define ITEXTVIEW_METHODS_DECL()                                           \
    CARAPI SetTypefaceEx(                                                   \
        /* [in] */ ITypeface * pTf,                                         \
        /* [in] */ Int32 style);                                            \
                                                                            \
    CARAPI GetText(                                                         \
        /* [out] */ ICharSequence ** ppText);                               \
                                                                            \
    CARAPI GetLength(                                                       \
        /* [out] */ Int32 * pLength);                                       \
                                                                            \
    CARAPI GetEditableText(                                                 \
        /* [out] */ IEditable** editable);                                  \
                                                                            \
    CARAPI GetLineHeight(                                                   \
        /* [out] */ Int32 * pHeight);                                       \
                                                                            \
    CARAPI GetLayout(                                                       \
        /* [out] */ ILayout** layout);                                      \
                                                                            \
    CARAPI GetKeyListener(                                                  \
        /* [out] */ IKeyListener** listener);                               \
                                                                            \
    CARAPI SetKeyListener(                                                  \
        /* [int] */ IKeyListener* listener);                                \
                                                                            \
    CARAPI GetMovementMethod(                                               \
        /* [out] */ IMovementMethod** movement);                            \
                                                                            \
    CARAPI SetMovementMethod(                                               \
        /* [int] */ IMovementMethod* movement);                             \
                                                                            \
    CARAPI SetTransformationMethod(                                         \
    /* [in] */ ITransformationMethod* method);                              \
                                                                            \
    CARAPI GetTransformationMethod(                                         \
        /* [out] */ ITransformationMethod** method);                        \
                                                                            \
    CARAPI GetCompoundPaddingTop(                                           \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetCompoundPaddingBottom(                                        \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI GetCompoundPaddingLeft(                                          \
        /* [out] */ Int32 * pLeft);                                         \
                                                                            \
    CARAPI GetCompoundPaddingRight(                                         \
        /* [out] */ Int32 * pRight);                                        \
                                                                            \
    CARAPI GetCompoundPaddingStart(                                         \
        /* [out] */ Int32* start);                                          \
                                                                            \
    CARAPI GetCompoundPaddingEnd(                                           \
        /* [out] */ Int32* end);                                            \
    CARAPI GetExtendedPaddingTop(                                           \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetExtendedPaddingBottom(                                        \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI GetTotalPaddingLeft(                                             \
        /* [out] */ Int32 * pLeft);                                         \
                                                                            \
    CARAPI GetTotalPaddingRight(                                            \
        /* [out] */ Int32 * pRight);                                        \
                                                                            \
    CARAPI GetTotalPaddingStart(                                            \
        /* [out] */ Int32* start);                                          \
                                                                            \
    CARAPI GetTotalPaddingEnd(                                              \
        /* [out] */ Int32* end);                                            \
                                                                            \
    CARAPI GetTotalPaddingTop(                                              \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetTotalPaddingBottom(                                           \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI SetCompoundDrawables(                                            \
        /* [in] */ IDrawable * pLeft,                                       \
        /* [in] */ IDrawable * pTop,                                        \
        /* [in] */ IDrawable * pRight,                                      \
        /* [in] */ IDrawable * pBottom);                                    \
                                                                            \
    CARAPI SetCompoundDrawablesWithIntrinsicBounds(                         \
        /* [in] */ Int32 left,                                              \
        /* [in] */ Int32 top,                                               \
        /* [in] */ Int32 right,                                             \
        /* [in] */ Int32 bottom);                                           \
                                                                            \
    CARAPI SetCompoundDrawablesWithIntrinsicBoundsEx(                       \
        /* [in] */ IDrawable * pLeft,                                       \
        /* [in] */ IDrawable * pTop,                                        \
        /* [in] */ IDrawable * pRight,                                      \
        /* [in] */ IDrawable * pBottom);                                    \
                                                                            \
                                                                            \
    CARAPI SetCompoundDrawablesRelative(                                    \
        /* [in] */ IDrawable* start,                                        \
        /* [in] */ IDrawable* top,                                          \
        /* [in] */ IDrawable* end,                                          \
        /* [in] */ IDrawable* bottom);                                      \
                                                                            \
                                                                            \
    CARAPI SetCompoundDrawablesRelativeWithIntrinsicBounds(                 \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 top,                                               \
        /* [in] */ Int32 end,                                               \
        /* [in] */ Int32 bottom);                                           \
                                                                            \
    CARAPI SetCompoundDrawablesRelativeWithIntrinsicBoundsEx(               \
        /* [in] */ IDrawable* start,                                        \
        /* [in] */ IDrawable* top,                                          \
        /* [in] */ IDrawable* end,                                          \
        /* [in] */ IDrawable* bottom);                                      \
                                                                            \
    CARAPI GetCompoundDrawables(                                            \
        /* [out, callee] */ ArrayOf<IDrawable *> ** ppDrawables);           \
                                                                            \
    CARAPI GetCompoundDrawablesRelative(                                    \
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);               \
                                                                            \
    CARAPI SetCompoundDrawablePadding(                                      \
        /* [in] */ Int32 pad);                                              \
                                                                            \
    CARAPI GetCompoundDrawablePadding(                                      \
        /* [out] */ Int32 * pPad);                                          \
                                                                            \
    CARAPI GetAutoLinkMask(                                                 \
        /* [out] */ Int32 * pMask);                                         \
                                                                            \
    CARAPI SetTextAppearance(                                               \
        /* [in] */ IContext * pContxt,                                      \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI GetTextLocale(                                                   \
        /* [out] */ ILocale** locale);                                      \
                                                                            \
    CARAPI SetTextLocale(                                                   \
        /* [in] */ ILocale* locale);                                        \
                                                                            \
    CARAPI GetTextSize(                                                     \
        /* [out] */ Float * pSize);                                         \
                                                                            \
    CARAPI SetTextSize(                                                     \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI SetTextSizeEx(                                                   \
        /* [in] */ Int32 unit,                                              \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI GetTextScaleX(                                                   \
        /* [out] */ Float * pSize);                                         \
                                                                            \
    CARAPI SetTextScaleX(                                                   \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI SetTypeface(                                                     \
        /* [in] */ ITypeface * pTf);                                        \
                                                                            \
    CARAPI GetTypeface(                                                     \
        /* [out] */ ITypeface ** ppFace);                                   \
                                                                            \
    CARAPI GetUrls(                                                         \
        /* [out] */ ArrayOf<IURLSpan*>** urls);                             \
                                                                            \
    CARAPI SetOnEditorActionListener(                                       \
        /* [in] */ IOnEditorActionListener* l);                             \
                                                                            \
    CARAPI AddTextChangedListener(                                          \
        /* [in] */ ITextWatcher* watcher);                                  \
                                                                            \
    CARAPI RemoveTextChangedListener(                                       \
        /* [in] */ ITextWatcher* watcher);                                  \
                                                                            \
    CARAPI SetScroller(                                                     \
        /* [in] */ IScroller* s);                                           \
                                                                            \
    CARAPI GetOffsetForPosition(                                            \
        /* [in] */ Float x,                                                 \
        /* [in] */ Float y,                                                 \
        /* [out] */ Int32* pos);                                            \
                                                                            \
    CARAPI GetHorizontalOffsetForDrawables(                                 \
        /* [out] */ Int32* offset);                                         \
                                                                            \
    CARAPI HideErrorIfUnchanged();                                          \
                                                                            \
    CARAPI SetTextColorEx(                                                  \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetTextColor(                                                    \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetTextColors(                                                   \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI GetCurrentTextColor(                                             \
        /* [out] */ Int32 * pColor);                                        \
                                                                            \
    CARAPI SetHighlightColor(                                               \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI GetHighlightColor(                                               \
        /* [out] */ Int32* color);                                          \
                                                                            \
    CARAPI SetShowSoftInputOnFocus(                                         \
        /* [in] */ Boolean show);                                           \
                                                                            \
    CARAPI GetShowSoftInputOnFocus(                                         \
        /* [out] */ Boolean* show);                                         \
                                                                            \
    CARAPI SetShadowLayer(                                                  \
        /* [in] */ Float radius,                                            \
        /* [in] */ Float dx,                                                \
        /* [in] */ Float dy,                                                \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI GetShadowRadius(                                                 \
        /* [out] */ Float* radius);                                         \
                                                                            \
    CARAPI GetShadowDx(                                                     \
        /* [out] */ Float* dx);                                             \
                                                                            \
    CARAPI GetShadowDy(                                                     \
        /* [out] */ Float* dy);                                             \
                                                                            \
    CARAPI GetShadowColor(                                                  \
        /* [out] */ Int32* color);                                          \
                                                                            \
    CARAPI SetAutoLinkMask(                                                 \
        /* [in] */ Int32 mask);                                             \
                                                                            \
    CARAPI SetLinksClickable(                                               \
        /* [in] */ Boolean whether);                                        \
                                                                            \
    CARAPI GetLinksClickable(                                               \
        /* [out] */ Boolean * pWhether);                                    \
                                                                            \
    CARAPI SetHintTextColorEx(                                              \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetHintTextColor(                                                \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetHintTextColors(                                               \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI GetCurrentHintTextColor(                                         \
        /* [out] */ Int32 * pColor);                                        \
                                                                            \
    CARAPI SetLinkTextColorEx(                                              \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetLinkTextColor(                                                \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetLinkTextColors(                                               \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI SetGravity(                                                      \
        /* [in] */ Int32 gravity);                                          \
                                                                            \
    CARAPI GetGravity(                                                      \
        /* [out] */ Int32 * pGravity);                                      \
                                                                            \
    CARAPI GetPaintFlags(                                                   \
        /* [out] */ Int32 * pFlags);                                        \
                                                                            \
    CARAPI GetPaint(                                                        \
        /* [out] */ ITextPaint** pFlags);                                   \
                                                                            \
    CARAPI SetPaintFlags(                                                   \
        /* [in] */ Int32 flags);                                            \
                                                                            \
    CARAPI SetHorizontallyScrolling(                                        \
        /* [in] */ Boolean whether);                                        \
                                                                            \
    CARAPI GetHorizontallyScrolling(                                        \
        /* [out] */ Boolean* whether);                                      \
                                                                            \
    CARAPI SetMinLines(                                                     \
        /* [in] */ Int32 minlines);                                         \
                                                                            \
    CARAPI GetMinLines(                                                     \
        /* [out] */ Int32* minlines);                                       \
                                                                            \
    CARAPI SetMinHeight(                                                    \
        /* [in] */ Int32 minHeight);                                        \
                                                                            \
    CARAPI GetMinHeight(                                                    \
        /* [out] */ Int32* minHeight);                                      \
                                                                            \
    CARAPI SetMaxLines(                                                     \
        /* [in] */ Int32 maxlines);                                         \
                                                                            \
    CARAPI GetMaxLines(                                                     \
        /* [out] */ Int32* maxlines);                                       \
                                                                            \
    CARAPI SetMaxHeight(                                                    \
        /* [in] */ Int32 maxHeight);                                        \
                                                                            \
    CARAPI GetMaxHeight(                                                    \
        /* [out] */ Int32* maxHeight);                                      \
                                                                            \
    CARAPI SetLines(                                                        \
        /* [in] */ Int32 lines);                                            \
                                                                            \
    CARAPI SetHeight(                                                       \
        /* [in] */ Int32 pixels);                                           \
                                                                            \
    CARAPI SetMinEms(                                                       \
        /* [in] */ Int32 minems);                                           \
                                                                            \
    CARAPI GetMinEms(                                                       \
        /* [out] */ Int32* minems);                                         \
                                                                            \
    CARAPI SetMinWidth(                                                     \
        /* [in] */ Int32 minpixels);                                        \
                                                                            \
    CARAPI GetMinWidth(                                                     \
        /* [out] */ Int32* minpixels);                                      \
                                                                            \
    CARAPI SetMaxEms(                                                       \
        /* [in] */ Int32 maxems);                                           \
                                                                            \
    CARAPI GetMaxEms(                                                       \
        /* [out] */ Int32* maxems);                                         \
                                                                            \
    CARAPI SetMaxWidth(                                                     \
        /* [in] */ Int32 maxpixels);                                        \
                                                                            \
    CARAPI GetMaxWidth(                                                     \
        /* [out] */ Int32* maxpixels);                                      \
                                                                            \
    CARAPI SetEms(                                                          \
        /* [in] */ Int32 ems);                                              \
                                                                            \
    CARAPI SetWidth(                                                        \
        /* [in] */ Int32 pixels);                                           \
                                                                            \
    CARAPI SetLineSpacing(                                                  \
        /* [in] */ Float add,                                               \
        /* [in] */ Float mult);                                             \
                                                                            \
    CARAPI GetLineSpacingMultiplier(                                        \
        /* [out] */ Float* multiplier);                                     \
                                                                            \
    CARAPI GetLineSpacingExtra(                                             \
        /* [out] */ Float* extra);                                          \
                                                                            \
    CARAPI Append(                                                          \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI AppendEx(                                                        \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 end);                                              \
                                                                            \
    CARAPI SetFreezesText(                                                  \
        /* [in] */ Boolean freezesText);                                    \
                                                                            \
    CARAPI GetFreezesText(                                                  \
        /* [out] */ Boolean * pText);                                       \
                                                                            \
    CARAPI SetEditableFactory(                                              \
        /* [in] */ IEditableFactory* factory);                              \
                                                                            \
    CARAPI SetSpannableFactory(                                             \
        /* [in] */ ISpannableFactory* factory);                             \
                                                                            \
    CARAPI SetText(                                                         \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI SetTextKeepState(                                                \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI SetTextEx(                                                       \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetTextEx2(                                                      \
        /* [in] */ ArrayOf<Char32>* text,                                   \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 len);                                              \
                                                                            \
    CARAPI SetTextKeepStateEx(                                              \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetTextEx3(                                                      \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI SetTextEx4(                                                      \
        /* [in] */ Int32 resid,                                             \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetHint(                                                         \
        /* [in] */ ICharSequence * pHint);                                  \
                                                                            \
    CARAPI SetHintEx(                                                       \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI GetHint(                                                         \
        /* [out] */ ICharSequence ** ppHint);                               \
                                                                            \
    CARAPI SetInputType(                                                    \
        /* [in] */ Int32 type);                                             \
                                                                            \
    CARAPI SetRawInputType(                                                 \
        /* [in] */ Int32 type);                                             \
                                                                            \
    CARAPI GetInputType(                                                    \
        /* [out] */ Int32 * pType);                                         \
                                                                            \
    CARAPI SetImeOptions(                                                   \
        /* [in] */ Int32 imeOptions);                                       \
                                                                            \
    CARAPI GetImeOptions(                                                   \
        /* [out] */ Int32 * pOptions);                                      \
                                                                            \
    CARAPI SetImeActionLabel(                                               \
        /* [in] */ ICharSequence * pLabel,                                  \
        /* [in] */ Int32 actionId);                                         \
                                                                            \
    CARAPI GetImeActionLabel(                                               \
        /* [out] */ ICharSequence ** ppLabel);                              \
                                                                            \
    CARAPI GetImeActionId(                                                  \
        /* [out] */ Int32 * pId);                                           \
                                                                            \
    CARAPI OnEditorAction(                                                  \
        /* [in] */ Int32 actionCode);                                       \
                                                                            \
    CARAPI SetPrivateImeOptions(                                            \
        /* [in] */ const String& type);                                     \
                                                                            \
    CARAPI GetPrivateImeOptions(                                            \
        /* [out] */ String * pOptions);                                     \
                                                                            \
    CARAPI SetInputExtras(                                                  \
        /* [in] */ Int32 xmlResId);                                         \
                                                                            \
    CARAPI GetInputExtras(                                                  \
        /* [in] */ Boolean create,                                          \
        /* [out] */ IBundle ** ppBundle);                                   \
                                                                            \
    CARAPI GetError(                                                        \
        /* [out] */ ICharSequence ** ppError);                              \
                                                                            \
    CARAPI SetError(                                                        \
        /* [in] */ ICharSequence * pError);                                 \
                                                                            \
    CARAPI SetErrorEx(                                                      \
        /* [in] */ ICharSequence * pError,                                  \
        /* [in] */ IDrawable * pIcon);                                      \
                                                                            \
    CARAPI SetFilters(                                                      \
        /* [in] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>* filters);  \
                                                                            \
    CARAPI GetFilters(                                                      \
        /* [out, callee] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>** filters); \
                                                                            \
    CARAPI IsTextSelectable(                                                \
        /*[out]*/ Boolean* selectable);                                     \
                                                                            \
    CARAPI SetTextIsSelectable(                                             \
        /*[in]*/ Boolean selectable);                                       \
                                                                            \
    CARAPI GetLineCount(                                                    \
        /* [out] */ Int32 * pCount);                                        \
                                                                            \
    CARAPI GetLineBounds(                                                   \
        /* [in] */ Int32 line,                                              \
        /* [in] */ IRect * pBounds,                                         \
        /* [out] */ Int32 * pY);                                            \
                                                                            \
    CARAPI ExtractText(                                                     \
        /* [in] */ IExtractedTextRequest * pRequest,                        \
        /* [in] */ IExtractedText * pOutText,                               \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetExtractedText(                                                \
        /* [in] */ IExtractedText * pText);                                 \
                                                                            \
    CARAPI SetExtracting(                                                   \
        /* [in] */ IExtractedTextRequest * pReq);                           \
                                                                            \
    CARAPI OnCommitCompletion(                                              \
        /* [in] */ ICompletionInfo * pText);                                \
                                                                            \
    CARAPI OnCommitCorrection(                                              \
        /* [in] */ ICorrectionInfo* info);                                  \
                                                                            \
    CARAPI BeginBatchEdit();                                                \
                                                                            \
    CARAPI EndBatchEdit();                                                  \
                                                                            \
    CARAPI OnBeginBatchEdit();                                              \
                                                                            \
    CARAPI OnEndBatchEdit();                                                \
                                                                            \
    CARAPI OnPrivateIMECommand(                                             \
        /* [in] */ const String& action,                                    \
        /* [in] */ IBundle * pData,                                         \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetIncludeFontPadding(                                           \
        /* [in] */ Boolean includepad);                                     \
                                                                            \
    CARAPI GetIncludeFontPadding(                                           \
        /* [out] */ Boolean* includepad);                                   \
                                                                            \
    CARAPI BringPointIntoView(                                              \
        /* [in] */ Int32 offset,                                            \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI MoveCursorToVisibleOffset(                                       \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI GetSelectionStart(                                               \
        /* [out] */ Int32 * pStart);                                        \
                                                                            \
    CARAPI GetSelectionEnd(                                                 \
        /* [out] */ Int32 * pEnd);                                          \
                                                                            \
    CARAPI HasSelection(                                                    \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetAllCaps(                                                      \
        /* [in] */ Boolean allCaps);                                        \
                                                                            \
    CARAPI SetSingleLine();                                                 \
                                                                            \
    CARAPI SetSingleLineEx(                                                 \
        /* [in] */ Boolean singleLine);                                     \
                                                                            \
    CARAPI SetEllipsize(                                                    \
        /* [in] */ TextUtilsTruncateAt where);                              \
                                                                            \
    CARAPI SetMarqueeRepeatLimit(                                           \
        /* [in] */ Int32 marqueeLimit);                                     \
                                                                            \
    CARAPI GetMarqueeRepeatLimit(                                           \
        /* [out] */ Int32* marqueeLimit);                                   \
                                                                            \
    CARAPI GetEllipsize(                                                    \
        /* [out] */ TextUtilsTruncateAt* where);                            \
                                                                            \
    CARAPI SetSelectAllOnFocus(                                             \
        /* [in] */ Boolean selectAllOnFocus);                               \
                                                                            \
    CARAPI SetCursorVisible(                                                \
        /* [in] */ Boolean visible);                                        \
                                                                            \
    CARAPI IsCursorVisible(                                                 \
        /* [out] */ Boolean* visible);                                      \
                                                                            \
    CARAPI ClearComposingText();                                            \
                                                                            \
    CARAPI DidTouchFocusSelect(                                             \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI IsInputMethodTarget(                                             \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI OnTextContextMenuItem(                                           \
        /* [in] */ Int32 id,                                                \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI ResetErrorChangedFlag();                                         \
                                                                            \
    CARAPI GetTextServicesLocale(                                           \
        /* [out] */ ILocale** locale);                                      \
                                                                            \
    CARAPI GetWordIterator(                                                 \
        /* [out] */ IWordIterator** word);                                  \
                                                                            \
    CARAPI GetTextForAccessibility(                                         \
        /* [out] */ ICharSequence** ch);                                    \
                                                                            \
    CARAPI IsSuggestionsEnabled(                                            \
        /*[out]*/ Boolean* enabled);                                        \
                                                                            \
    CARAPI SetCustomSelectionActionModeCallback(                            \
        /*[in]*/ IActionModeCallback* actionModeCallback);                  \
                                                                            \
    CARAPI GetCustomSelectionActionModeCallback(                            \
        /*[out]*/ IActionModeCallback** actionModeCallback);                \
                                                                            \
    CARAPI RemoveMisspelledSpans(                                           \
        /* [in] */ ISpannable* spannable);                                  \
                                                                            \
    CARAPI OnLocaleChanged();                                               \
                                                                            \
    CARAPI InvalidateRegion(                                                \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 end,                                               \
        /* [in] */ Boolean invalidateCursor);                               \


#define ITEXTVIEW_METHODS_IMPL(className, superClass)                                          \
ECode className::SetTypefaceEx(                                                                 \
    /* [in] */ ITypeface * pTf,                                                                 \
    /* [in] */ Int32 style)                                                                     \
{                                                                                               \
    return superClass::SetTypeface(pTf, style);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetText(                                                                       \
    /* [out] */ ICharSequence ** ppText)                                                        \
{                                                                                               \
    VALIDATE_NOT_NULL(ppText);                                                                  \
    AutoPtr<ICharSequence> text = superClass::GetText();                                        \
    *ppText = text.Get();                                                                       \
    if (*ppText) {                                                                              \
        (*ppText)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLength(                                                                     \
    /* [out] */ Int32 * pLength)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pLength);                                                                 \
    *pLength = superClass::GetLength();                                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetEditableText(                                                               \
    /* [out] */ IEditable** editable)                                                           \
{                                                                                               \
    VALIDATE_NOT_NULL(editable);                                                                \
    AutoPtr<IEditable> temp = superClass::GetEditableText();                                    \
    *editable = temp;                                                                           \
    if (*editable) {                                                                            \
        (*editable)->AddRef();                                                                  \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLineHeight(                                                                 \
    /* [out] */ Int32 * pHeight)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pHeight);                                                                 \
    *pHeight = superClass::GetLineHeight();                                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLayout(                                                                     \
    /* [out] */ ILayout** layout)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(layout);                                                                  \
    AutoPtr<ILayout> temp = superClass::GetLayout();                                            \
    *layout = temp;                                                                             \
    if (*layout) {                                                                              \
        (*layout)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetKeyListener(                                                                \
    /* [out] */ IKeyListener** listener)                                                        \
{                                                                                               \
    VALIDATE_NOT_NULL(listener);                                                                \
    AutoPtr<IKeyListener> temp = superClass::GetKeyListener();                                  \
    *listener = temp;                                                                           \
    if (*listener) {                                                                            \
        (*listener)->AddRef();                                                                  \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetKeyListener(                                                                \
    /* [in] */ IKeyListener* input)                                                             \
{                                                                                               \
    return superClass::SetKeyListener(input);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetMovementMethod(                                                             \
    /* [out] */ IMovementMethod** movement)                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(movement);                                                                \
    AutoPtr<IMovementMethod> temp = superClass::GetMovementMethod();                            \
    *movement = temp;                                                                           \
    if (*movement) {                                                                            \
        (*movement)->AddRef();                                                                  \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMovementMethod(                                                             \
    /* [in] */ IMovementMethod* movement)                                                       \
{                                                                                               \
    return superClass::SetMovementMethod(movement);                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTransformationMethod(                                                       \
    /* [in] */ ITransformationMethod* method)                                                   \
{                                                                                               \
    return superClass::SetTransformationMethod(method);                                         \
}                                                                                               \
                                                                                                \
ECode className::GetTransformationMethod(                                                       \
    /* [out] */ ITransformationMethod** method)                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(method);                                                                  \
    AutoPtr<ITransformationMethod> temp = superClass::GetTransformationMethod();                \
    *method = temp;                                                                             \
    if (*method) {                                                                              \
        (*method)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingTop(                                                         \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetCompoundPaddingTop();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingBottom(                                                      \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetCompoundPaddingBottom();                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingLeft(                                                        \
    /* [out] */ Int32 * pLeft)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pLeft);                                                                   \
    *pLeft = superClass::GetCompoundPaddingLeft();                                              \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingRight(                                                       \
    /* [out] */ Int32 * pRight)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pRight);                                                                  \
    *pRight = superClass::GetCompoundPaddingRight();                                            \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingStart(                                                       \
    /* [out] */ Int32* start)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(start);                                                                   \
    *start = superClass::GetCompoundPaddingStart();                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingEnd(                                                         \
    /* [out] */ Int32* end)                                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(end);                                                                     \
    *end = superClass::GetCompoundPaddingEnd();                                                 \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetExtendedPaddingTop(                                                         \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetExtendedPaddingTop();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetExtendedPaddingBottom(                                                      \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetExtendedPaddingBottom();                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingLeft(                                                           \
    /* [out] */ Int32 * pLeft)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pLeft);                                                                   \
    *pLeft = superClass::GetTotalPaddingLeft();                                                 \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingRight(                                                          \
    /* [out] */ Int32 * pRight)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pRight);                                                                  \
    *pRight = superClass::GetTotalPaddingRight();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingStart(                                                          \
    /* [out] */ Int32* start)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(start);                                                                   \
    *start = superClass::GetTotalPaddingStart();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingEnd(                                                            \
    /* [out] */ Int32* end)                                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(end);                                                                     \
    *end = superClass::GetTotalPaddingEnd();                                                    \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingTop(                                                            \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetTotalPaddingTop();                                                   \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingBottom(                                                         \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetTotalPaddingBottom();                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawables(                                                          \
    /* [in] */ IDrawable * pLeft,                                                               \
    /* [in] */ IDrawable * pTop,                                                                \
    /* [in] */ IDrawable * pRight,                                                              \
    /* [in] */ IDrawable * pBottom)                                                             \
{                                                                                               \
    return superClass::SetCompoundDrawables(pLeft, pTop, pRight, pBottom);                      \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesWithIntrinsicBounds(                                       \
    /* [in] */ Int32 left,                                                                      \
    /* [in] */ Int32 top,                                                                       \
    /* [in] */ Int32 right,                                                                     \
    /* [in] */ Int32 bottom)                                                                    \
{                                                                                               \
    return superClass::SetCompoundDrawablesWithIntrinsicBounds(left, top, right, bottom);       \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesWithIntrinsicBoundsEx(                                     \
    /* [in] */ IDrawable * pLeft,                                                               \
    /* [in] */ IDrawable * pTop,                                                                \
    /* [in] */ IDrawable * pRight,                                                              \
    /* [in] */ IDrawable * pBottom)                                                             \
{                                                                                               \
    return superClass::SetCompoundDrawablesWithIntrinsicBounds(pLeft, pTop, pRight, pBottom);   \
}                                                                                               \
                                                                                                \
                                                                                                \
ECode className::SetCompoundDrawablesRelative(                                                  \
    /* [in] */ IDrawable* start,                                                                \
    /* [in] */ IDrawable* top,                                                                  \
    /* [in] */ IDrawable* end,                                                                  \
    /* [in] */ IDrawable* bottom)                                                               \
{                                                                                               \
    return superClass::SetCompoundDrawablesRelative(start, top, end, bottom);                   \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesRelativeWithIntrinsicBounds(                               \
    /* [in] */ Int32 start,                                                                     \
    /* [in] */ Int32 top,                                                                       \
    /* [in] */ Int32 end,                                                                       \
    /* [in] */ Int32 bottom)                                                                    \
{                                                                                               \
    return superClass::SetCompoundDrawablesRelativeWithIntrinsicBounds(start, top, end, bottom);\
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesRelativeWithIntrinsicBoundsEx(                             \
    /* [in] */ IDrawable* start,                                                                \
    /* [in] */ IDrawable* top,                                                                  \
    /* [in] */ IDrawable* end,                                                                  \
    /* [in] */ IDrawable* bottom)                                                               \
{                                                                                               \
    return superClass::SetCompoundDrawablesRelativeWithIntrinsicBoundsEx(start, top, end, bottom);\
}                                                                                               \
                                                                                                \
ECode className::GetCompoundDrawables(                                                          \
    /* [out, callee] */ ArrayOf<IDrawable*> ** ppDrawables)                                     \
{                                                                                               \
    AutoPtr<ArrayOf<IDrawable*> > drawables = superClass::GetCompoundDrawables();               \
    *ppDrawables = drawables.Get();                                                             \
    if (*ppDrawables != NULL) {                                                                 \
        (*ppDrawables)->AddRef();                                                               \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundDrawablesRelative(                                                  \
    /* [out, callee] */ ArrayOf<IDrawable *> ** ppDrawables)                                    \
{                                                                                               \
    AutoPtr<ArrayOf<IDrawable*> > drawables = superClass::GetCompoundDrawablesRelative();       \
    *ppDrawables = drawables.Get();                                                             \
    if (*ppDrawables != NULL) {                                                                 \
        (*ppDrawables)->AddRef();                                                               \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
ECode className::SetCompoundDrawablePadding(                                                    \
    /* [in] */ Int32 pad)                                                                       \
{                                                                                               \
    return superClass::SetCompoundDrawablePadding(pad);                                         \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundDrawablePadding(                                                    \
    /* [out] */ Int32 * pPad)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pPad);                                                                    \
    *pPad = superClass::GetCompoundDrawablePadding();                                           \
                                                                                                \
    return  NOERROR;                                                                            \
}                                                                                               \
                                                                                                \
ECode className::GetAutoLinkMask(                                                               \
    /* [out] */ Int32 * pMask)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pMask);                                                                   \
    *pMask = superClass::GetAutoLinkMask();                                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextAppearance(                                                             \
    /* [in] */ IContext * pContxt,                                                              \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetTextAppearance(pContxt, resid);                                       \
}                                                                                               \
                                                                                                \
ECode className::GetTextLocale(                                                                 \
    /* [out] */ ILocale** locale)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(locale);                                                                  \
    AutoPtr<ILocale> tmp = superClass::GetTextLocale();                                         \
    *locale = tmp.Get();                                                                        \
    if (*locale != NULL) {                                                                      \
        (*locale)->AddRef();                                                                    \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextLocale(                                                                 \
    /* [in] */ ILocale* locale)                                                                 \
{                                                                                               \
    return superClass::SetTextLocale(locale);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetTextSize(                                                                   \
    /* [out] */ Float * pSize)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pSize);                                                                   \
    *pSize = superClass::GetTextSize();                                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextSize(                                                                   \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextSize(size);                                                       \
}                                                                                               \
                                                                                                \
ECode className::SetTextSizeEx(                                                                 \
    /* [in] */ Int32 unit,                                                                      \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextSize(unit, size);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetTextScaleX(                                                                 \
    /* [out] */ Float * pSize)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pSize);                                                                   \
    *pSize = superClass::GetTextScaleX();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextScaleX(                                                                 \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextScaleX(size);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetTypeface(                                                                   \
    /* [in] */ ITypeface * pTf)                                                                 \
{                                                                                               \
    return superClass::SetTypeface(pTf);                                                        \
}                                                                                               \
                                                                                                \
ECode className::GetTypeface(                                                                   \
    /* [out] */ ITypeface ** ppFace)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(ppFace);                                                                  \
                                                                                                \
    AutoPtr<ITypeface> tmp = superClass::GetTypeface();                                         \
    *ppFace = tmp.Get();                                                                        \
    if (*ppFace != NULL) {                                                                      \
        (*ppFace)->AddRef();                                                                    \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetUrls(                                                                       \
    /* [out] */ ArrayOf<IURLSpan*>** urls)                                                      \
{                                                                                               \
    VALIDATE_NOT_NULL(urls);                                                                    \
                                                                                                \
    AutoPtr<ArrayOf<IURLSpan*> > tmp = superClass::GetUrls();                                   \
    *urls = tmp.Get();                                                                          \
    if (*urls != NULL) {                                                                        \
        (*urls)->AddRef();                                                                      \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetOnEditorActionListener(                                                     \
    /* [in] */ IOnEditorActionListener* l)                                                      \
{                                                                                               \
    return superClass::SetOnEditorActionListener(l);                                            \
}                                                                                               \
                                                                                                \
ECode className::AddTextChangedListener(                                                        \
    /* [in] */ ITextWatcher* watcher)                                                           \
{                                                                                               \
    return superClass::AddTextChangedListener(watcher);                                         \
}                                                                                               \
                                                                                                \
ECode className::RemoveTextChangedListener(                                                     \
    /* [in] */ ITextWatcher* watcher)                                                           \
{                                                                                               \
    return superClass::RemoveTextChangedListener(watcher);                                      \
}                                                                                               \
                                                                                                \
ECode className::SetScroller(                                                                   \
    /* [in] */ IScroller* s)                                                                    \
{                                                                                               \
    return superClass::SetScroller(s);                                                          \
}                                                                                               \
                                                                                                \
ECode className::GetOffsetForPosition(                                                          \
    /* [in] */ Float x,                                                                         \
    /* [in] */ Float y,                                                                         \
    /* [out] */ Int32* pos)                                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(pos);                                                                     \
    *pos = superClass::GetOffsetForPosition(x, y);                                              \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetHorizontalOffsetForDrawables(                                               \
    /* [out] */ Int32* offset)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(offset);                                                                  \
    *offset = superClass::GetHorizontalOffsetForDrawables();                                    \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::HideErrorIfUnchanged()                                                         \
{                                                                                               \
    return superClass::HideErrorIfUnchanged();                                                  \
}                                                                                               \
                                                                                                \
ECode className::SetTextColorEx(                                                                \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetTextColor(color);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetTextColor(                                                                  \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetTextColor(pColors);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetTextColors(                                                                 \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
                                                                                                \
    AutoPtr<IColorStateList> tmp = superClass::GetTextColors();                                 \
    *ppColors = tmp.Get();                                                                      \
    if (*ppColors != NULL) {                                                                    \
        (*ppColors)->AddRef();                                                                  \
    }                                                                                           \
    return NOERROR;                                                                             \
                                                                                                \
}                                                                                               \
                                                                                                \
ECode className::GetCurrentTextColor(                                                           \
    /* [out] */ Int32 * pColor)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pColor);                                                                  \
    *pColor = superClass::GetCurrentTextColor();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetHighlightColor(                                                             \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetHighlightColor(color);                                                \
}                                                                                               \
                                                                                                \
ECode className::GetHighlightColor(                                                             \
    /* [out] */ Int32* color)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(color);                                                                   \
    *color = superClass::GetHighlightColor();                                                   \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetShowSoftInputOnFocus(                                                       \
    /* [in] */ Boolean show)                                                                    \
{                                                                                               \
    return superClass::SetShowSoftInputOnFocus(show);                                           \
}                                                                                               \
                                                                                                \
ECode className::GetShowSoftInputOnFocus(                                                       \
    /* [out] */ Boolean* show)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(show);                                                                    \
    *show = superClass::GetShowSoftInputOnFocus();                                              \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetShadowLayer(                                                                \
    /* [in] */ Float radius,                                                                    \
    /* [in] */ Float dx,                                                                        \
    /* [in] */ Float dy,                                                                        \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetShadowLayer(radius, dx, dy, color);                                   \
}                                                                                               \
                                                                                                \
ECode className::GetShadowRadius(                                                               \
    /* [out] */ Float* radius)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(radius);                                                                  \
    *radius = superClass::GetShadowRadius();                                                    \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetShadowDx(                                                                   \
    /* [out] */ Float* dx)                                                                      \
{                                                                                               \
    VALIDATE_NOT_NULL(dx);                                                                      \
    *dx = superClass::GetShadowDx();                                                            \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetShadowDy(                                                                   \
    /* [out] */ Float* dy)                                                                      \
{                                                                                               \
    VALIDATE_NOT_NULL(dy);                                                                      \
    *dy = superClass::GetShadowDy();                                                            \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetShadowColor(                                                                \
    /* [out] */ Int32* color)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(color);                                                                   \
    *color = superClass::GetShadowColor();                                                      \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetAutoLinkMask(                                                               \
    /* [in] */ Int32 mask)                                                                      \
{                                                                                               \
    return superClass::SetAutoLinkMask(mask);                                                   \
}                                                                                               \
                                                                                                \
ECode className::SetLinksClickable(                                                             \
    /* [in] */ Boolean whether)                                                                 \
{                                                                                               \
    return superClass::SetLinksClickable(whether);                                              \
}                                                                                               \
                                                                                                \
ECode className::GetLinksClickable(                                                             \
    /* [out] */ Boolean * pWhether)                                                             \
{                                                                                               \
    VALIDATE_NOT_NULL(pWhether);                                                                \
    *pWhether = superClass::GetLinksClickable();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetHintTextColorEx(                                                            \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetHintTextColor(color);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetHintTextColor(                                                              \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetHintTextColor(pColors);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetHintTextColors(                                                             \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
    AutoPtr<IColorStateList> tmp = superClass::GetHintTextColors();                             \
    *ppColors = tmp.Get();                                                                      \
    if (*ppColors != NULL) {                                                                    \
        (*ppColors)->AddRef();                                                                  \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCurrentHintTextColor(                                                       \
    /* [out] */ Int32 * pColor)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pColor);                                                                  \
    *pColor = superClass::GetCurrentHintTextColor();                                            \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetLinkTextColorEx(                                                            \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetLinkTextColor(color);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetLinkTextColor(                                                              \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetLinkTextColor(pColors);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetLinkTextColors(                                                             \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
                                                                                                \
    AutoPtr<IColorStateList> tmp = superClass::GetLinkTextColors();                             \
    *ppColors = tmp.Get();                                                                      \
    if (*ppColors != NULL) {                                                                    \
        (*ppColors)->AddRef();                                                                  \
    }                                                                                           \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetGravity(                                                                    \
    /* [in] */ Int32 gravity)                                                                   \
{                                                                                               \
    return superClass::SetGravity(gravity);                                                     \
}                                                                                               \
                                                                                                \
ECode className::GetGravity(                                                                    \
    /* [out] */ Int32 * pGravity)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(pGravity);                                                                \
    *pGravity = superClass::GetGravity();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetPaintFlags(                                                                 \
    /* [out] */ Int32 * pFlags)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pFlags);                                                                  \
    *pFlags = superClass::GetPaintFlags();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetPaint(                                                                      \
    /* [out] */ ITextPaint** textPaint)                                                         \
{                                                                                               \
    VALIDATE_NOT_NULL(textPaint);                                                               \
    AutoPtr<ITextPaint> paint = superClass::GetPaint();                                         \
    *textPaint = paint.Get();                                                                   \
    if (*textPaint) {                                                                           \
        (*textPaint)->AddRef();                                                                 \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetPaintFlags(                                                                 \
    /* [in] */ Int32 flags)                                                                     \
{                                                                                               \
    return superClass::SetPaintFlags(flags);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetHorizontallyScrolling(                                                      \
    /* [in] */ Boolean whether)                                                                 \
{                                                                                               \
    return superClass::SetHorizontallyScrolling(whether);                                       \
}                                                                                               \
                                                                                                \
ECode className::GetHorizontallyScrolling(                                                      \
    /* [out] */ Boolean* whether)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(whether);                                                                 \
    *whether = superClass::GetHorizontallyScrolling();                                          \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMinLines(                                                                   \
    /* [in] */ Int32 minlines)                                                                  \
{                                                                                               \
    return superClass::SetMinLines(minlines);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetMinLines(                                                                   \
    /* [out] */ Int32* minLines)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(minLines);                                                                \
    *minLines = superClass::GetMinLines();                                                      \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMinHeight(                                                                  \
    /* [in] */ Int32 minHeight)                                                                 \
{                                                                                               \
    return superClass::SetMinHeight(minHeight);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetMinHeight(                                                                  \
    /* [out] */ Int32* minHeight)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(minHeight);                                                               \
    *minHeight = superClass::GetMinHeight();                                                    \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMaxLines(                                                                   \
    /* [in] */ Int32 maxlines)                                                                  \
{                                                                                               \
    return superClass::SetMaxLines(maxlines);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetMaxLines(                                                                   \
    /* [out] */ Int32* maxlines)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(maxlines);                                                                \
    *maxlines = superClass::GetMaxLines();                                                      \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMaxHeight(                                                                  \
    /* [in] */ Int32 maxHeight)                                                                 \
{                                                                                               \
    return superClass::SetMaxHeight(maxHeight);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetMaxHeight(                                                                  \
    /* [in] */ Int32* maxHeight)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(maxHeight);                                                               \
    *maxHeight = superClass::GetMaxHeight();                                                    \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetLines(                                                                      \
    /* [in] */ Int32 lines)                                                                     \
{                                                                                               \
    return superClass::SetLines(lines);                                                         \
}                                                                                               \
                                                                                                \
ECode className::SetHeight(                                                                     \
    /* [in] */ Int32 pixels)                                                                    \
{                                                                                               \
    return superClass::SetHeight(pixels);                                                       \
}                                                                                               \
                                                                                                \
ECode className::SetMinEms(                                                                     \
    /* [in] */ Int32 minems)                                                                    \
{                                                                                               \
    return superClass::SetMinEms(minems);                                                       \
}                                                                                               \
                                                                                                \
ECode className::GetMinEms(                                                                     \
    /* [out] */ Int32* minems)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(minems);                                                                  \
    *minems = superClass::GetMinEms();                                                          \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMinWidth(                                                                   \
    /* [in] */ Int32 minpixels)                                                                 \
{                                                                                               \
    return superClass::SetMinWidth(minpixels);                                                  \
}                                                                                               \
                                                                                                \
ECode className::GetMinWidth(                                                                   \
    /* [out] */ Int32* minpixels)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(minpixels);                                                               \
    *minpixels = superClass::GetMinWidth();                                                     \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMaxEms(                                                                     \
    /* [in] */ Int32 maxems)                                                                    \
{                                                                                               \
    return superClass::SetMaxEms(maxems);                                                       \
}                                                                                               \
                                                                                                \
ECode className::GetMaxEms(                                                                     \
    /* [in] */ Int32* maxems)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(maxems);                                                                  \
    *maxems = superClass::GetMaxEms();                                                          \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetMaxWidth(                                                                   \
    /* [in] */ Int32 maxpixels)                                                                 \
{                                                                                               \
    return superClass::SetMaxWidth(maxpixels);                                                  \
}                                                                                               \
                                                                                                \
ECode className::GetMaxWidth(                                                                   \
    /* [out] */ Int32* maxpixels)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(maxpixels);                                                               \
    *maxpixels = superClass::GetMaxWidth();                                                     \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetEms(                                                                        \
    /* [in] */ Int32 ems)                                                                       \
{                                                                                               \
    return superClass::SetEms(ems);                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetWidth(                                                                      \
    /* [in] */ Int32 pixels)                                                                    \
{                                                                                               \
    return superClass::SetWidth(pixels);                                                        \
}                                                                                               \
                                                                                                \
ECode className::SetLineSpacing(                                                                \
    /* [in] */ Float add,                                                                       \
    /* [in] */ Float mult)                                                                      \
{                                                                                               \
    return superClass::SetLineSpacing(add, mult);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetLineSpacingMultiplier(                                                      \
    /* [out] */ Float* multiplier)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(multiplier);                                                              \
    *multiplier = superClass::GetLineSpacingMultiplier();                                       \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLineSpacingExtra(                                                           \
    /* [out] */ Float* extra)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(extra);                                                                   \
    *extra = superClass::GetLineSpacingExtra();                                                 \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::Append(                                                                        \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::Append(pText);                                                           \
}                                                                                               \
                                                                                                \
ECode className::AppendEx(                                                                      \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ Int32 start,                                                                     \
    /* [in] */ Int32 end)                                                                       \
{                                                                                               \
    return superClass::Append(pText, start, end);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetFreezesText(                                                                \
    /* [in] */ Boolean freezesText)                                                             \
{                                                                                               \
    return superClass::SetFreezesText(freezesText);                                             \
}                                                                                               \
                                                                                                \
ECode className::GetFreezesText(                                                                \
    /* [out] */ Boolean * pText)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pText);                                                                   \
    *pText = superClass::GetFreezesText();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetEditableFactory(                                                            \
    /* [in] */ IEditableFactory* factory)                                                       \
{                                                                                               \
    return superClass::SetEditableFactory(factory);                                             \
}                                                                                               \
                                                                                                \
ECode className::SetSpannableFactory(                                                           \
    /* [in] */ ISpannableFactory* factory)                                                      \
{                                                                                               \
    return superClass::SetSpannableFactory(factory);                                            \
}                                                                                               \
                                                                                                \
ECode className::SetText(                                                                       \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::SetText(pText);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetTextKeepState(                                                              \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::SetTextKeepState(pText);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx(                                                                     \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetText(pText, type);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx2(                                                                    \
    /* [in] */ ArrayOf<Char32>* text,                                                           \
    /* [in] */ Int32 start,                                                                     \
    /* [in] */ Int32 len)                                                                       \
{                                                                                               \
    return superClass::SetText(text, start, len);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetTextKeepStateEx(                                                            \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetTextKeepState(pText, type);                                           \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx3(                                                                    \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetText(resid);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx4(                                                                    \
    /* [in] */ Int32 resid,                                                                     \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetText(resid, type);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetHint(                                                                       \
    /* [in] */ ICharSequence * pHint)                                                           \
{                                                                                               \
    return superClass::SetHint(pHint);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetHintEx(                                                                     \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetHint(resid);                                                          \
}                                                                                               \
                                                                                                \
ECode className::GetHint(                                                                       \
    /* [out] */ ICharSequence ** ppHint)                                                        \
{                                                                                               \
    VALIDATE_NOT_NULL(ppHint);                                                                  \
    AutoPtr<ICharSequence> hint = superClass::GetHint();                                        \
    *ppHint = hint.Get();                                                                       \
    if (*ppHint) {                                                                              \
        (*ppHint)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetInputType(                                                                  \
    /* [in] */ Int32 type)                                                                      \
{                                                                                               \
    return superClass::SetInputType(type);                                                      \
}                                                                                               \
                                                                                                \
ECode className::SetRawInputType(                                                               \
    /* [in] */ Int32 type)                                                                      \
{                                                                                               \
    return superClass::SetRawInputType(type);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetInputType(                                                                  \
    /* [out] */ Int32 * pType)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pType);                                                                   \
    *pType = superClass::GetInputType();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetImeOptions(                                                                 \
    /* [in] */ Int32 imeOptions)                                                                \
{                                                                                               \
    return superClass::SetImeOptions(imeOptions);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetImeOptions(                                                                 \
    /* [out] */ Int32 * pOptions)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(pOptions);                                                                \
    *pOptions = superClass::GetImeOptions();                                                    \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetImeActionLabel(                                                             \
    /* [in] */ ICharSequence * pLabel,                                                          \
    /* [in] */ Int32 actionId)                                                                  \
{                                                                                               \
    return superClass::SetImeActionLabel(pLabel, actionId);                                     \
}                                                                                               \
                                                                                                \
ECode className::GetImeActionLabel(                                                             \
    /* [out] */ ICharSequence ** ppLabel)                                                       \
{                                                                                               \
    VALIDATE_NOT_NULL(ppLabel);                                                                 \
    AutoPtr<ICharSequence> label = superClass::GetImeActionLabel();                             \
    *ppLabel = label.Get();                                                                     \
    if (*ppLabel) {                                                                             \
        (*ppLabel)->AddRef();                                                                   \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetImeActionId(                                                                \
    /* [out] */ Int32 * pId)                                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(pId);                                                                     \
    *pId = superClass::GetImeActionId();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::OnEditorAction(                                                                \
    /* [in] */ Int32 actionCode)                                                                \
{                                                                                               \
    return superClass::OnEditorAction(actionCode);                                              \
}                                                                                               \
                                                                                                \
ECode className::SetPrivateImeOptions(                                                          \
    /* [in] */ const String& type)                                                                     \
{                                                                                               \
    return superClass::SetPrivateImeOptions(type);                                              \
}                                                                                               \
                                                                                                \
ECode className::GetPrivateImeOptions(                                                          \
    /* [out] */ String * pOptions)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pOptions);                                                                \
    *pOptions = superClass::GetPrivateImeOptions();                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetInputExtras(                                                                \
    /* [in] */ Int32 xmlResId)                                                                  \
{                                                                                               \
    return superClass::SetInputExtras(xmlResId);                                                \
}                                                                                               \
                                                                                                \
ECode className::GetInputExtras(                                                                \
    /* [in] */ Boolean create,                                                                  \
    /* [out] */ IBundle ** ppBundle)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(ppBundle);                                                                \
    AutoPtr<IBundle> bundle = superClass::GetInputExtras(create);                               \
    *ppBundle = bundle.Get();                                                                   \
    if (*ppBundle) {                                                                            \
        (*ppBundle)->AddRef();                                                                  \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetError(                                                                      \
    /* [out] */ ICharSequence ** ppError)                                                       \
{                                                                                               \
    VALIDATE_NOT_NULL(ppError);                                                                 \
    AutoPtr<ICharSequence> error = superClass::GetError();                                      \
    *ppError = error.Get();                                                                     \
    if (*ppError) {                                                                             \
        (*ppError)->AddRef();                                                                   \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetError(                                                                      \
    /* [in] */ ICharSequence * pError)                                                          \
{                                                                                               \
    return superClass::SetError(pError);                                                        \
}                                                                                               \
                                                                                                \
ECode className::SetErrorEx(                                                                    \
    /* [in] */ ICharSequence * pError,                                                          \
    /* [in] */ IDrawable * pIcon)                                                               \
{                                                                                               \
    return superClass::SetError(pError, pIcon);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetFilters(                                                                    \
    /* [in] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>* filters)                           \
{                                                                                               \
    return superClass::SetFilters(filters);                                                     \
}                                                                                               \
                                                                                                \
ECode className::GetFilters(                                                                    \
    /* [out, callee] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>** filters)                 \
{                                                                                               \
    VALIDATE_NOT_NULL(filters);                                                                 \
    AutoPtr<ArrayOf<Elastos::Droid::Text::IInputFilter*> > tmp = superClass::GetFilters();      \
    *filters = tmp.Get();                                                                       \
    if (*filters) {                                                                             \
        (*filters)->AddRef();                                                                   \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::IsTextSelectable(                                                              \
    /* [out] */ Boolean* selectable)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(selectable);                                                              \
    *selectable = superClass::IsTextSelectable();                                               \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextIsSelectable(                                                           \
    /* [in] */ Boolean selectable)                                                              \
{                                                                                               \
    return superClass::SetTextIsSelectable(selectable);                                         \
}                                                                                               \
                                                                                                \
ECode className::GetLineCount(                                                                  \
    /* [out] */ Int32 * pCount)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pCount);                                                                  \
    *pCount = superClass::GetLineCount();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLineBounds(                                                                 \
    /* [in] */ Int32 line,                                                                      \
    /* [in] */ IRect * pBounds,                                                                 \
    /* [out] */ Int32 * pY)                                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(pY);                                                                      \
    *pY = superClass::GetLineBounds(line, pBounds);                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::ExtractText(                                                                   \
    /* [in] */ IExtractedTextRequest * pRequest,                                                \
    /* [in] */ IExtractedText * pOutText,                                                       \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::ExtractText(pRequest, pOutText);                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetExtractedText(                                                              \
    /* [in] */ IExtractedText * pText)                                                          \
{                                                                                               \
    return superClass::SetExtractedText(pText);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetExtracting(                                                                 \
    /* [in] */ IExtractedTextRequest * pReq)                                                    \
{                                                                                               \
    return superClass::SetExtracting(pReq);                                                     \
}                                                                                               \
                                                                                                \
ECode className::OnCommitCompletion(                                                            \
    /* [in] */ ICompletionInfo * pText)                                                         \
{                                                                                               \
    return superClass::OnCommitCompletion(pText);                                               \
}                                                                                               \
                                                                                                \
ECode className::OnCommitCorrection(                                                            \
    /* [in] */ ICorrectionInfo * pText)                                                         \
{                                                                                               \
    return superClass::OnCommitCorrection(pText);                                               \
}                                                                                               \
                                                                                                \
ECode className::BeginBatchEdit()                                                               \
{                                                                                               \
    return superClass::BeginBatchEdit();                                                        \
}                                                                                               \
                                                                                                \
ECode className::EndBatchEdit()                                                                 \
{                                                                                               \
    return superClass::EndBatchEdit();                                                          \
}                                                                                               \
                                                                                                \
ECode className::OnBeginBatchEdit()                                                             \
{                                                                                               \
    return superClass::OnBeginBatchEdit();                                                      \
}                                                                                               \
                                                                                                \
ECode className::OnEndBatchEdit()                                                               \
{                                                                                               \
    return superClass::OnEndBatchEdit();                                                        \
}                                                                                               \
                                                                                                \
ECode className::OnPrivateIMECommand(                                                           \
    /* [in] */ const String& action,                                                            \
    /* [in] */ IBundle * pData,                                                                 \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::OnPrivateIMECommand(action, pData);                                  \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetIncludeFontPadding(                                                         \
    /* [in] */ Boolean includepad)                                                              \
{                                                                                               \
    return superClass::SetIncludeFontPadding(includepad);                                       \
}                                                                                               \
                                                                                                \
ECode className::GetIncludeFontPadding(                                                         \
    /* [out] */ Boolean* includepad)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(includepad);                                                              \
    *includepad = superClass::GetIncludeFontPadding();                                          \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::BringPointIntoView(                                                            \
    /* [in] */ Int32 offset,                                                                    \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::BringPointIntoView(offset);                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::MoveCursorToVisibleOffset(                                                     \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::MoveCursorToVisibleOffset();                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetSelectionStart(                                                             \
    /* [out] */ Int32 * pStart)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pStart);                                                                  \
    *pStart = superClass::GetSelectionStart();                                                  \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetSelectionEnd(                                                               \
    /* [out] */ Int32 * pEnd)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pEnd);                                                                    \
    *pEnd = superClass::GetSelectionEnd();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::HasSelection(                                                                  \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::HasSelection();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetAllCaps(                                                                    \
    /* [in] */ Boolean allCaps)                                                                 \
{                                                                                               \
    return superClass::SetAllCaps(allCaps);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetSingleLine()                                                                \
{                                                                                               \
    return superClass::SetSingleLine();                                                         \
}                                                                                               \
                                                                                                \
ECode className::SetSingleLineEx(                                                               \
    /* [in] */ Boolean singleLine)                                                              \
{                                                                                               \
    return superClass::SetSingleLine(singleLine);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetEllipsize(                                                                  \
    /* [in] */ TextUtilsTruncateAt where)                                                       \
{                                                                                               \
    return superClass::SetEllipsize(where);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetMarqueeRepeatLimit(                                                         \
    /* [in] */ Int32 marqueeLimit)                                                              \
{                                                                                               \
    return superClass::SetMarqueeRepeatLimit(marqueeLimit);                                     \
}                                                                                               \
                                                                                                \
ECode className::GetMarqueeRepeatLimit(                                                         \
    /* [out] */ Int32* marqueeLimit)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(marqueeLimit);                                                            \
    *marqueeLimit = superClass::GetMarqueeRepeatLimit();                                        \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetEllipsize(                                                                  \
    /* [out] */ TextUtilsTruncateAt* where)                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(where);                                                                   \
    *where = superClass::GetEllipsize();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetSelectAllOnFocus(                                                           \
    /* [in] */ Boolean selectAllOnFocus)                                                        \
{                                                                                               \
    return superClass::SetSelectAllOnFocus(selectAllOnFocus);                                   \
}                                                                                               \
                                                                                                \
ECode className::SetCursorVisible(                                                              \
    /* [in] */ Boolean visible)                                                                 \
{                                                                                               \
    return superClass::SetCursorVisible(visible);                                               \
}                                                                                               \
                                                                                                \
ECode className::IsCursorVisible(                                                               \
    /* [out] */ Boolean* visible)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(visible);                                                                 \
    *visible = superClass::IsCursorVisible();                                                   \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::ClearComposingText()                                                           \
{                                                                                               \
    return superClass::ClearComposingText();                                                    \
}                                                                                               \
                                                                                                \
ECode className::DidTouchFocusSelect(                                                           \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::DidTouchFocusSelect();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::IsInputMethodTarget(                                                           \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::IsInputMethodTarget();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::OnTextContextMenuItem(                                                         \
    /* [in] */ Int32 id,                                                                        \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::OnTextContextMenuItem(id);                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::ResetErrorChangedFlag()                                                        \
{                                                                                               \
    superClass::ResetErrorChangedFlag();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTextServicesLocale(                                                         \
    /* [out] */ ILocale** locale)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(locale);                                                                  \
    AutoPtr<ILocale> tmp = superClass::GetTextServicesLocale();                                 \
    *locale = tmp.Get();                                                                        \
    if (*locale != NULL) {                                                                      \
        (*locale)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetWordIterator(                                                               \
    /* [out] */ IWordIterator** word)                                                           \
{                                                                                               \
    VALIDATE_NOT_NULL(word);                                                                    \
    AutoPtr<IWordIterator> tmp = superClass::GetWordIterator();                                 \
    *word = tmp.Get();                                                                          \
    if (*word != NULL) {                                                                        \
        (*word)->AddRef();                                                                      \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTextForAccessibility(                                                       \
    /* [out] */ ICharSequence** ch)                                                             \
{                                                                                               \
    VALIDATE_NOT_NULL(ch);                                                                      \
    AutoPtr<ICharSequence> _ch = superClass::GetTextForAccessibility();                         \
    *ch = _ch.Get();                                                                            \
    if (*ch != NULL) {                                                                          \
        (*ch)->AddRef();                                                                        \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::IsSuggestionsEnabled(                                                          \
    /*[out]*/ Boolean* enabled)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(enabled);                                                                 \
    *enabled = superClass::IsSuggestionsEnabled();                                              \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetCustomSelectionActionModeCallback(                                          \
    /*[in]*/ IActionModeCallback* actionModeCallback)                                           \
{                                                                                               \
    superClass::SetCustomSelectionActionModeCallback(actionModeCallback);                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCustomSelectionActionModeCallback(                                          \
    /*[out]*/ IActionModeCallback** actionModeCallback)                                         \
{                                                                                               \
    VALIDATE_NOT_NULL(actionModeCallback);                                                      \
    AutoPtr<IActionModeCallback> callback = superClass::GetCustomSelectionActionModeCallback(); \
    *actionModeCallback = callback.Get();                                                       \
    if (*actionModeCallback != NULL) {                                                          \
        (*actionModeCallback)->AddRef();                                                        \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::RemoveMisspelledSpans(                                                         \
    /* [in] */ ISpannable* spannable)                                                           \
{                                                                                               \
    return superClass::RemoveMisspelledSpans(spannable);                                        \
}                                                                                               \
                                                                                                \
ECode className::OnLocaleChanged()                                                              \
{                                                                                               \
    return superClass::OnLocaleChanged();                                                       \
}                                                                                               \
                                                                                                \
CARAPI className::InvalidateRegion(                                                             \
        /* [in] */ Int32 start,                                                                 \
        /* [in] */ Int32 end,                                                                   \
        /* [in] */ Boolean invalidateCursor)                                                    \
{                                                                                               \
    return superClass::InvalidateRegion(start, end, invalidateCursor);                          \
}                                                                                               \

#endif  //__TEXTVIEWMACRO_H__
