#ifndef __ELASTOS_DROID_WIDGET_CTEXTVIEW_H__
#define __ELASTOS_DROID_WIDGET_CTEXTVIEW_H__

#include "_Elastos_Droid_Widget_CTextView.h"

#include "elastos/droid/widget/TextView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CTextView), public TextView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);


    /**
     * Callback method to be invoked when the view tree is about to be drawn. At this point, all
     * views in the tree have been measured and given a frame. Clients can use this to adjust
     * their scroll bounds or even to request a new layout before drawing occurs.
     *
     * @return Return true to proceed with the current drawing pass, or false to cancel.
     *
     * @see android.view.View#onMeasure
     * @see android.view.View#onLayout
     * @see android.view.View#onDraw
     */
    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    /**
     * Sets the typeface and style in which the text should be displayed,
     * and turns on the fake bold and italic bits in the Paint if the
     * Typeface that you provided does not have all the bits in the
     * style that you specified.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    CARAPI SetTypeface(
        /* [in] */ ITypeface* tf,
        /* [in] */ Int32 style);

    /**
     * Return the text the TextView is displaying. If setText() was called with
     * an argument of BufferType.SPANNABLE or BufferType.EDITABLE, you can cast
     * the return value from this method to Spannable or Editable, respectively.
     *
     * Note: The content of the return value should not be modified. If you want
     * a modifiable one, you should make your own copy first.
     */
    CARAPI GetText(
        /* [out] */ ICharSequence** text);

    /**
     * Returns the length, in characters, of the text managed by this TextView
     */
    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetHorizontalOffsetForDrawables(
        /* [out] */ Int32* offset);

    /**
     * Return the text the TextView is displaying as an Editable object.  If
     * the text is not editable, null is returned.
     *
     * @see #getText
     */
    CARAPI GetEditableText(
        /* [out] */ IEditable** editable);

    /**
     * @return the height of one standard line in pixels.  Note that markup
     * within the text can cause individual lines to be taller or shorter
     * than this height, and e layout may contain additional first-
     * or last-line padding.
     */
    CARAPI GetLineHeight(
        /* [out] */ Int32* height);

    /**
     * @return the Layout that is currently being used to display the text.
     * This can be null if the text or width has recently changes.
     */
    CARAPI GetLayout(
        /* [out] */ ILayout** layout);

    /**
     * @return the current key listener for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI GetKeyListener(
        /* [out] */ IKeyListener** listener);

    /**
     * Sets the key listener to be used with this TextView.  This can be null
     * to disallow user input.  Note that this method has significant and
     * subtle interactions with soft keyboards and other input method:
     * see {@link KeyListener#getInputType() KeyListener.getContentType()}
     * for important details.  Calling this method will replace the current
     * content type of the text view with the content type returned by the
     * key listener.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     *
     * @attr ref android.R.styleable#TextView_numeric
     * @attr ref android.R.styleable#TextView_digits
     * @attr ref android.R.styleable#TextView_phoneNumber
     * @attr ref android.R.styleable#TextView_inputMethod
     * @attr ref android.R.styleable#TextView_capitalize
     * @attr ref android.R.styleable#TextView_autoText
     */
    CARAPI SetKeyListener(
        /* [in] */ IKeyListener* input);

    /**
     * @return the movement method being used for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI GetMovementMethod(
        /* [out] */ IMovementMethod** movement);

    /**
     * Sets the movement method (arrow key handler) to be used for
     * this TextView.  This can be null to disallow using the arrow keys
     * to move the cursor or scroll the view.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     */
    CARAPI SetMovementMethod(
        /* [in] */ IMovementMethod* movement);

    /**
     * @return the current transformation method for this TextView.
     * This will frequently be null except for single-line and password
     * fields.
     */
    CARAPI GetTransformationMethod(
        /* [out] */ ITransformationMethod** method);

    /**
     * Sets the transformation that is applied to the text that this
     * TextView is displaying.
     *
     * @attr ref android.R.styleable#TextView_password
     * @attr ref android.R.styleable#TextView_singleLine
     */
    CARAPI SetTransformationMethod(
        /* [in] */ ITransformationMethod* method);

    /**
     * Returns the top padding of the view, plus space for the top
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the bottom padding of the view, plus space for the bottom
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Returns the left padding of the view, plus space for the left
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingLeft(
        /* [out] */ Int32* left);

    /**
     * Returns the right padding of the view, plus space for the right
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingRight(
        /* [out] */ Int32* right);

    /**
     * Returns the start padding of the view, plus space for the start
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingStart(
        /* [out] */ Int32* start);

    /**
     * Returns the end padding of the view, plus space for the end
     * Drawable if any.
     */
    CARAPI GetCompoundPaddingEnd(
        /* [out] */ Int32* end);


    /**
     * Returns the extended top padding of the view, including both the
     * top Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    CARAPI GetExtendedPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the extended bottom padding of the view, including both the
     * bottom Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    CARAPI GetExtendedPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Returns the total left padding of the view, including the left
     * Drawable if any.
     */
    CARAPI GetTotalPaddingLeft(
        /* [out] */ Int32* left);

    /**
     * Returns the total right padding of the view, including the right
     * Drawable if any.
     */
    CARAPI GetTotalPaddingRight(
        /* [out] */ Int32* right);

    /**
     * Returns the total start padding of the view, including the start
     * Drawable if any.
     */
    CARAPI GetTotalPaddingStart(
        /* [out] */ Int32* start);

    /**
     * Returns the total end padding of the view, including the end
     * Drawable if any.
     */
    CARAPI GetTotalPaddingEnd(
        /* [out] */ Int32* end);

    /**
     * Returns the total top padding of the view, including the top
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    CARAPI GetTotalPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the total bottom padding of the view, including the bottom
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    CARAPI GetTotalPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there.  The Drawables must already have had
     * {@link Drawable#setBounds} called.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawables(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use 0 if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @param left Resource identifier of the left Drawable.
     * @param top Resource identifier of the top Drawable.
     * @param right Resource identifier of the right Drawable.
     * @param bottom Resource identifier of the bottom Drawable.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use null if you do not
     * want a Drawable there.  The Drawables must already have had
     * {@link Drawable#setBounds} called.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawablesRelative(
        /* [in] */ IDrawable* start,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* end,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use 0 if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @param start Resource identifier of the start Drawable.
     * @param top Resource identifier of the top Drawable.
     * @param end Resource identifier of the end Drawable.
     * @param bottom Resource identifier of the bottom Drawable.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawablesRelativeWithIntrinsicBounds(
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use null if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI SetCompoundDrawablesRelativeWithIntrinsicBounds(
        /* [in] */ IDrawable* start,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* end,
        /* [in] */ IDrawable* bottom);

    /**
     * Returns drawables for the left, top, right, and bottom borders.
     */
    CARAPI GetCompoundDrawables(
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);

    /**
     * Returns drawables for the start, top, end, and bottom borders.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    CARAPI GetCompoundDrawablesRelative(
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);

    /**
     * Sets the size of the padding between the compound drawables and
     * the text.
     *
     * @attr ref android.R.styleable#TextView_drawablePadding
     */
    CARAPI SetCompoundDrawablePadding(
        /* [in] */ Int32 pad);

    /**
     * Returns the padding between the compound drawables and the text.
     */
    CARAPI GetCompoundDrawablePadding(
        /* [out] */ Int32* pad);

    /**
     * Gets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    CARAPI GetAutoLinkMask(
        /* [out] */ Int32* mask);

    /**
     * Sets the text color, size, style, hint color, and highlight color
     * from the specified TextAppearance resource.
     */
    CARAPI SetTextAppearance(
        /* [in] */ IContext* contxt,
        /* [in] */ Int32 resid);

    /**
     * Get the default {@link Locale} of the text in this TextView.
     * @return the default {@link Locale} of the text in this TextView.
     */
    CARAPI GetTextLocale(
        /* [out] */ ILocale** locale);

    /**
     * Set the default {@link Locale} of the text in this TextView to the given value. This value
     * is used to choose appropriate typefaces for ambiguous characters. Typically used for CJK
     * locales to disambiguate Hanzi/Kanji/Hanja characters.
     *
     * @param locale the {@link Locale} for drawing text, must not be null.
     *
     * @see Paint#setTextLocale
     */
    CARAPI SetTextLocale(
        /* [in] */ ILocale* locale);

    /**
     * @return the size (in pixels) of the default text size in this TextView.
     */
    CARAPI GetTextSize(
        /* [out] */ Float* size);

    /**
     * Set the default text size to the given value, interpreted as "scaled
     * pixel" units.  This size is adjusted based on the current density and
     * user font size preference.
     *
     * @param size The scaled pixel size.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    CARAPI SetTextSize(
        /* [in] */ Float size);

    /**
     * Set the default text size to a given unit and value.  See {@link
     * TypedValue} for the possible dimension units.
     *
     * @param unit The desired dimension unit.
     * @param size The desired size in the given units.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    CARAPI SetTextSize(
        /* [in] */ Int32 unit,
        /* [in] */ Float size);

    /**
     * @return the extent by which text is currently being stretched
     * horizontally.  This will usually be 1.
     */
    CARAPI GetTextScaleX(
        /* [out] */ Float* size);

    /**
     * Sets the extent by which text should be stretched horizontally.
     *
     * @attr ref android.R.styleable#TextView_textScaleX
     */
    CARAPI SetTextScaleX(
        /* [in] */ Float size);

    /**
     * Sets the typeface and style in which the text should be displayed.
     * Note that not all Typeface families actually have bold and italic
     * variants, so you may need to use
     * {@link #setTypeface(Typeface, Int32)} to get the appearance
     * that you actually want.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    CARAPI SetTypeface(
        /* [in] */ ITypeface* tf);

    /**
     * @return the current typeface and style in which the text is being
     * displayed.
     */
    CARAPI GetTypeface(
        /* [out] */ ITypeface** face);

    /**
     * Sets the text color for all the states (normal, selected,
     * focused) to be this color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    CARAPI SetTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the text color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    CARAPI SetTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * Return the set of text colors.
     *
     * @return Returns the set of text colors.
     */
    CARAPI GetTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected for normal text.</p>
     *
     * @return Returns the current text color.
     */
    CARAPI GetCurrentTextColor(
        /* [out] */ Int32* color);

    /**
     * Sets the color used to display the selection highlight.
     *
     * @attr ref android.R.styleable#TextView_textColorHighlight
     */
    CARAPI SetHighlightColor(
        /* [in] */ Int32 color);

    /**
     * @return the color used to display the selection highlight
     *
     * @see #setHighlightColor(int)
     *
     * @attr ref android.R.styleable#TextView_textColorHighlight
     */
    CARAPI GetHighlightColor(
        /* [out] */ Int32* color);

    /**
     * Sets whether the soft input method will be made visible when this
     * TextView gets focused. The default is true.
     * @hide
     */
    CARAPI SetShowSoftInputOnFocus(
        /* [in] */ Boolean show);

    /**
     * Returns whether the soft input method will be made visible when this
     * TextView gets focused. The default is true.
     * @hide
     */
    CARAPI GetShowSoftInputOnFocus(
        /* [out] */ Boolean* show);

    /**
     * Gives the text a shadow of the specified radius and color, the specified
     * distance from its normal position.
     *
     * @attr ref android.R.styleable#TextView_shadowColor
     * @attr ref android.R.styleable#TextView_shadowDx
     * @attr ref android.R.styleable#TextView_shadowDy
     * @attr ref android.R.styleable#TextView_shadowRadius
     */
    CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Int32 color);

    /**
     * Gets the radius of the shadow layer.
     *
     * @return the radius of the shadow layer. If 0, the shadow layer is not visible
     *
     * @see #setShadowLayer(float, float, float, int)
     *
     * @attr ref android.R.styleable#TextView_shadowRadius
     */
    CARAPI GetShadowRadius(
        /* [out] */ Float* radius);

    /**
     * @return the horizontal offset of the shadow layer
     *
     * @see #setShadowLayer(float, float, float, int)
     *
     * @attr ref android.R.styleable#TextView_shadowDx
     */
    CARAPI GetShadowDx(
        /* [out] */ Float* dx);

    /**
     * @return the vertical offset of the shadow layer
     *
     * @see #setShadowLayer(float, float, float, int)
     *
     * @attr ref android.R.styleable#TextView_shadowDy
     */
    CARAPI GetShadowDy(
        /* [out] */ Float* dy);

    /**
     * @return the color of the shadow layer
     *
     * @see #setShadowLayer(float, float, float, int)
     *
     * @attr ref android.R.styleable#TextView_shadowColor
     */
    CARAPI GetShadowColor(
        /* [out] */ Int32* color);

    /**
     * @return the base paint used for the text.  Please use this only to
     * consult the Paint's properties and not to change them.
     */
    CARAPI GetPaint(
        /* [out] */ ITextPaint** paint);

    /**
     * Sets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    CARAPI SetAutoLinkMask(
        /* [in] */ Int32 mask);

    /**
     * Sets whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    CARAPI SetLinksClickable(
        /* [in] */ Boolean whether);

    /**
     * Returns whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    CARAPI GetLinksClickable(
        /* [out] */ Boolean* whether);

    /**
     * Returns the list of URLSpans attached to the text
     * (by {@link Linkify} or otherwise) if any.  You can call
     * {@link URLSpan#getURL} on them to find where they link to
     * or use {@link Spanned#getSpanStart} and {@link Spanned#getSpanEnd}
     * to find the region of the text they are attached to.
     */
    CARAPI GetUrls(
        /* [out, callee] */ ArrayOf<IURLSpan*>** urls);

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    CARAPI SetHintTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    CARAPI SetHintTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Return the color used to paint the hint text.</p>
     *
     * @return Returns the list of hint text colors.
     */
    CARAPI GetHintTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected to paint the hint text.</p>
     *
     * @return Returns the current hint text color.
     */
    CARAPI GetCurrentHintTextColor(
        /* [out] */ Int32* color);

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    CARAPI SetLinkTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    CARAPI SetLinkTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Returns the color used to paint links in the text.</p>
     *
     * @return Returns the list of link text colors.
     */
    CARAPI GetLinkTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * Sets the horizontal alignment of the text and the
     * vertical gravity that will be used when there is extra space
     * in the TextView beyond what is required for the text itself.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Returns the horizontal and vertical alignment of this TextView.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    /**
     * @return the flags on the Paint being used to display the text.
     * @see Paint#getFlags
     */
    CARAPI GetPaintFlags(
        /* [out] */ Int32* flags);

    /**
     * Sets flags on the Paint being used to display the text and
     * reflows the text if they are different from the old flags.
     * @see Paint#setFlags
     */
    CARAPI SetPaintFlags(
        /* [in] */ Int32 flags);

    /**
     * Sets whether the text should be allowed to be wider than the
     * View is.  If false, it will be wrapped to the width of the View.
     *
     * @attr ref android.R.styleable#TextView_scrollHorizontally
     */
    CARAPI SetHorizontallyScrolling(
        /* [in] */ Boolean whether);

    /**
     * Returns whether the text is allowed to be wider than the View is.
     * If false, the text will be wrapped to the width of the View.
     *
     * @attr ref android.R.styleable#TextView_scrollHorizontally
     * @hide
     */
    CARAPI GetHorizontallyScrolling(
        /* [out] */ Boolean* whether);

    /**
     * Makes the TextView at least this many lines tall
     *
     * @attr ref android.R.styleable#TextView_minLines
     */
    CARAPI SetMinLines(
        /* [in] */ Int32 minlines);

    /**
     * @return the minimum number of lines displayed in this TextView, or -1 if the minimum
     * height was set in pixels instead using {@link #setMinHeight(int) or #setHeight(int)}.
     *
     * @see #setMinLines(int)
     *
     * @attr ref android.R.styleable#TextView_minLines
     */
    CARAPI GetMinLines(
        /* [out] */ Int32* minlines);

    /**
     * Makes the TextView at least this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_minHeight
     */
    CARAPI SetMinHeight(
        /* [in] */ Int32 minHeight);

    /**
     * @return the minimum height of this TextView expressed in pixels, or -1 if the minimum
     * height was set in number of lines instead using {@link #setMinLines(int) or #setLines(int)}.
     *
     * @see #setMinHeight(int)
     *
     * @attr ref android.R.styleable#TextView_minHeight
     */
    CARAPI GetMinHeight(
        /* [out] */ Int32* minHeight);

    /**
     * Makes the TextView at most this many lines tall
     *
     * @attr ref android.R.styleable#TextView_maxLines
     */
    CARAPI SetMaxLines(
        /* [in] */ Int32 maxlines);

    /**
     * @return the maximum number of lines displayed in this TextView, or -1 if the maximum
     * height was set in pixels instead using {@link #setMaxHeight(int) or #setHeight(int)}.
     *
     * @see #setMaxLines(int)
     *
     * @attr ref android.R.styleable#TextView_maxLines
     */
    CARAPI GetMaxLines(
        /* [out] */ Int32* maxlines);

    /**
     * Makes the TextView at most this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_maxHeight
     */
    CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    /**
     * @return the maximum height of this TextView expressed in pixels, or -1 if the maximum
     * height was set in number of lines instead using {@link #setMaxLines(int) or #setLines(int)}.
     *
     * @see #setMaxHeight(int)
     *
     * @attr ref android.R.styleable#TextView_maxHeight
     */
    CARAPI GetMaxHeight(
        /* [out] */ Int32* maxHeight);

    /**
     * Makes the TextView exactly this many lines tall
     *
     * @attr ref android.R.styleable#TextView_lines
     */
    CARAPI SetLines(
        /* [in] */ Int32 lines);

    /**
     * Makes the TextView exactly this many pixels tall.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_height
     */
    CARAPI SetHeight(
        /* [in] */ Int32 pixels);

    /**
     * Makes the TextView at least this many ems wide
     *
     * @attr ref android.R.styleable#TextView_minEms
     */
    CARAPI SetMinEms(
        /* [in] */ Int32 minems);

    /**
     * @return the minimum width of the TextView, expressed in ems or -1 if the minimum width
     * was set in pixels instead (using {@link #setMinWidth(int)} or {@link #setWidth(int)}).
     *
     * @see #setMinEms(int)
     * @see #setEms(int)
     *
     * @attr ref android.R.styleable#TextView_minEms
     */
    CARAPI GetMinEms(
        /* [out] */ Int32* minems);

    /**
     * Makes the TextView at least this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_minWidth
     */
    CARAPI SetMinWidth(
        /* [in] */ Int32 minpixels);

    /**
     * @return the minimum width of the TextView, in pixels or -1 if the minimum width
     * was set in ems instead (using {@link #setMinEms(int)} or {@link #setEms(int)}).
     *
     * @see #setMinWidth(int)
     * @see #setWidth(int)
     *
     * @attr ref android.R.styleable#TextView_minWidth
     */
    CARAPI GetMinWidth(
        /* [out] */ Int32* minpixels);

    /**
     * Makes the TextView at most this many ems wide
     *
     * @attr ref android.R.styleable#TextView_maxEms
     */
    CARAPI SetMaxEms(
        /* [in] */ Int32 maxems);

    /**
     * Makes the TextView at most this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_maxWidth
     */
    CARAPI SetMaxWidth(
        /* [in] */ Int32 maxpixels);

    /**
     * @return the maximum width of the TextView, in pixels or -1 if the maximum width
     * was set in ems instead (using {@link #setMaxEms(int)} or {@link #setEms(int)}).
     *
     * @see #setMaxWidth(int)
     * @see #setWidth(int)
     *
     * @attr ref android.R.styleable#TextView_maxWidth
     */
    CARAPI GetMaxWidth(
        /* [out] */ Int32* maxpixels);

    /**
     * Makes the TextView exactly this many ems wide
     *
     * @attr ref android.R.styleable#TextView_ems
     */
    CARAPI SetEms(
        /* [in] */ Int32 ems);

    /**
     * Makes the TextView exactly this many pixels wide.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_width
     */
    CARAPI SetWidth(
        /* [in] */ Int32 pixels);

    /**
     * Sets line spacing for this TextView.  Each line will have its height
     * multiplied by <code>mult</code> and have <code>add</code> added to it.
     *
     * @attr ref android.R.styleable#TextView_lineSpacingExtra
     * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
     */
    CARAPI SetLineSpacing(
        /* [in] */ Float add,
        /* [in] */ Float mult);

    /**
     * Gets the line spacing multiplier
     *
     * @return the value by which each line's height is multiplied to get its actual height.
     *
     * @see #setLineSpacing(float, float)
     * @see #getLineSpacingExtra()
     *
     * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
     */
    CARAPI GetLineSpacingMultiplier(
        /* [out] */ Float* multiplier);

    /**
     * Gets the line spacing extra space
     *
     * @return the extra space that is added to the height of each lines of this TextView.
     *
     * @see #setLineSpacing(float, float)
     * @see #getLineSpacingMultiplier()
     *
     * @attr ref android.R.styleable#TextView_lineSpacingExtra
     */
    CARAPI GetLineSpacingExtra(
        /* [out] */ Float* extra);

    /**
     * Convenience method: Append the specified text to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    CARAPI Append(
        /* [in] */ ICharSequence* text);

    /**
     * Convenience method: Append the specified text slice to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Control whether this text view saves its entire text contents when
     * freezing to an icicle, in addition to dynamic state such as cursor
     * position.  By default this is false, not saving the text.  Set to true
     * if the text in the text view is not being saved somewhere else in
     * persistent storage (such as in a content provider) so that if the
     * view is later thawed the user will not lose their data.
     *
     * @param freezesText Controls whether a frozen icicle should include the
     * entire text data: true to include it, false to not.
     *
     * @attr ref android.R.styleable#TextView_freezesText
     */
    CARAPI SetFreezesText(
        /* [in] */ Boolean freezesText);

    /**
     * Return whether this text view is including its entire text contents
     * in frozen icicles.
     *
     * @return Returns true if text is included, false if it isn't.
     *
     * @see #setFreezesText
     */
    CARAPI GetFreezesText(
        /* [out] */ Boolean* text);

    /**
     * Sets the Factory used to create new Editables.
     */
    CARAPI SetEditableFactory(
        /* [in] */ IEditableFactory* factory);

    /**
     * Sets the Factory used to create new Spannables.
     */
    CARAPI SetSpannableFactory(
        /* [in] */ ISpannableFactory* factory);

    /**
     * Sets the string value of the TextView. TextView <em>does not</em> accept
     * HTML-like formatting, which you can do with text strings in XML resource files.
     * To style your strings, attach android.text.style.* objects to a
     * {@link android.text.SpannableString SpannableString}, or see the
     * <a href="{@docRoot}guide/topics/resources/available-resources.html#stringresources">
     * Available Resource Types</a> documentation for an example of setting
     * formatted text in the XML resource file.
     *
     * @attr ref android.R.styleable#TextView_text
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* text);

    /**
     * Like {@link #setText(CharSequence)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @param text The new text to place in the text view.
     *
     * @see #setText(CharSequence)
     */
    CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text);

    /**
     * Sets the text that this TextView is to display (see
     * {@link #setText(CharSequence)}) and also sets whether it is stored
     * in a styleable/spannable buffer and whether it is editable.
     *
     * @attr ref android.R.styleable#TextView_text
     * @attr ref android.R.styleable#TextView_bufferType
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    /**
     * Sets the TextView to display the specified slice of the specified
     * char array.  You must promise that you will not change the contents
     * of the array except for right before another call to setText(),
     * since the TextView has no way to know that the text
     * has changed and that it needs to invalidate and re-layout.
     */
    CARAPI SetText(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    /**
     * Like {@link #setText(CharSequence, android.widget.TextView.BufferType)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @see #setText(CharSequence, android.widget.TextView.BufferType)
     */
    CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    CARAPI SetText(
        /* [in] */ Int32 resid);

    CARAPI SetText(
        /* [in] */ Int32 resid,
        /* [in] */ BufferType type);

    /**
     * Sets the text to be displayed when the text of the TextView is empty.
     * Null means to use the normal empty text. The hint does not currently
     * participate in determining the size of the view.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    CARAPI SetHint(
        /* [in] */ ICharSequence* hint);

    /**
     * Sets the text to be displayed when the text of the TextView is empty,
     * from a resource.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    CARAPI SetHint(
        /* [in] */ Int32 resid);

    /**
     * Returns the hint that is displayed when the text of the TextView
     * is empty.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    CARAPI GetHint(
        /* [out] */ ICharSequence** hint);

    /**
     * Set the type of the content with a constant as defined for
     * {@link EditorInfo#inputType}.  This will take care of changing
     * the key listener, by calling {@link #setKeyListener(KeyListener)}, to
     * match the given content type.  If the given content type is
     * {@link EditorInfo#TYPE_NULL} then a soft keyboard will
     * not be displayed for this text view.
     *
     * @see #getInputType()
     * @see #setRawInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    CARAPI SetInputType(
        /* [in] */ Int32 type);

    /**
     * Directly change the content type integer of the text view, without
     * modifying any other state.
     * @see #setInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    CARAPI SetRawInputType(
        /* [in] */ Int32 type);

    /**
     * Get the type of the content.
     *
     * @see #setInputType(Int32)
     * @see android.text.InputType
     */
    CARAPI GetInputType(
        /* [out] */ Int32* type);

    /**
     * Change the editor type integer associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#imeOptions} when it
     * has focus.
     * @see #getImeOptions
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeOptions
     */
    CARAPI SetImeOptions(
        /* [in] */ Int32 imeOptions);

    /**
     * Get the type of the IME editor.
     *
     * @see #setImeOptions(Int32)
     * @see android.view.inputmethod.EditorInfo
     */
    CARAPI GetImeOptions(
        /* [out] */ Int32* options);

    /**
     * Change the custom IME action associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#actionLabel}
     * and {@link EditorInfo#actionId} when it has focus.
     * @see #getImeActionLabel
     * @see #getImeActionId
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeActionLabel
     * @attr ref android.R.styleable#TextView_imeActionId
     */
    CARAPI SetImeActionLabel(
        /* [in] */ ICharSequence* label,
        /* [in] */ Int32 actionId);

    /**
     * Get the IME action label previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    CARAPI GetImeActionLabel(
        /* [out] */ ICharSequence** label);

    /**
     * Get the IME action ID previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    CARAPI GetImeActionId(
        /* [out] */ Int32* id);

    /**
     * Set a special listener to be called when an action is performed
     * on the text view.  This will be called when the enter key is pressed,
     * or when an action supplied to the IME is selected by the user.  Setting
     * this means that the normal hard key event will not insert a newline
     * into the text view, even if it is multi-line; holding down the ALT
     * modifier will, however, allow the user to insert a newline character.
     */
    CARAPI SetOnEditorActionListener(
        /* [in] */ IOnEditorActionListener* l);

    /**
     * Called when an attached input method calls
     * {@link InputConnection#performEditorAction(Int32)
     * InputConnection.performEditorAction()}
     * for this text view.  The default implementation will call your action
     * listener supplied to {@link #setOnEditorActionListener}, or perform
     * a standard operation for {@link EditorInfo#IME_ACTION_NEXT
     * EditorInfo.IME_ACTION_NEXT} or {@link EditorInfo#IME_ACTION_DONE
     * EditorInfo.IME_ACTION_DONE}.
     *
     * <p>For backwards compatibility, if no IME options have been set and the
     * text view would not normally advance focus on enter, then
     * the NEXT and DONE actions received here will be turned into an enter
     * key down/up pair to go through the normal key handling.
     *
     * @param actionCode The code of the action being performed.
     *
     * @see #setOnEditorActionListener
     */
    CARAPI OnEditorAction(
        /* [in] */ Int32 actionCode);

    /**
     * Set the private content type of the text, which is the
     * {@link EditorInfo#privateImeOptions EditorInfo.privateImeOptions}
     * field that will be filled in when creating an input connection.
     *
     * @see #getPrivateImeOptions()
     * @see EditorInfo#privateImeOptions
     * @attr ref android.R.styleable#TextView_privateImeOptions
     */
    CARAPI SetPrivateImeOptions(
        /* [in] */ const String& type);

    /**
     * Get the private type of the content.
     *
     * @see #setPrivateImeOptions(String)
     * @see EditorInfo#privateImeOptions
     */
    CARAPI GetPrivateImeOptions(
        /* [out] */ String* options);

    /**
     * Set the extra input data of the text, which is the
     * {@link EditorInfo#extras TextBoxAttribute.extras}
     * Bundle that will be filled in when creating an input connection.  The
     * given integer is the resource ID of an XML resource holding an
     * {@link android.R.styleable#InputExtras &lt;input-extras&gt;} XML tree.
     *
     * @see #getInputExtras(Boolean)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    CARAPI SetInputExtras(
        /* [in] */ Int32 xmlResId);

    /**
     * Retrieve the input extras currently associated with the text view, which
     * can be viewed as well as modified.
     *
     * @param create If true, the extras will be created if they don't already
     * exist.  Otherwise, null will be returned if none have been created.
     * @see #setInputExtras(Int32)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    CARAPI GetInputExtras(
        /* [in] */ Boolean create,
        /* [out] */ IBundle** bundle);

    /**
     * Returns the error message that was set to be displayed with
     * {@link #setError}, or <code>null</code> if no error was set
     * or if it the error was cleared by the widget after user input.
     */
    CARAPI GetError(
        /* [out] */ ICharSequence** error);

    /**
     * Sets the right-hand compound drawable of the TextView to the "error"
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  If the
     * <code>error</code> is <code>null</code>, the error message and icon
     * will be cleared.
     */
    CARAPI SetError(
        /* [in] */ ICharSequence* error);

    /**
     * Sets the right-hand compound drawable of the TextView to the specified
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  The
     * drawable must already have had {@link Drawable#setBounds} set on it.
     * If the <code>error</code> is <code>null</code>, the error message will
     * be cleared (and you should provide a <code>null</code> icon as well).
     */
    CARAPI SetError(
        /* [in] */ ICharSequence* error,
        /* [in] */ IDrawable* icon);

    /**
     * Sets the list of input filters that will be used if the buffer is
     * Editable.  Has no effect otherwise.
     *
     * @attr ref android.R.styleable#TextView_maxLength
     */
    CARAPI SetFilters(
        /* [in] */ ArrayOf<IInputFilter*>* filters);

    /**
     * Returns the current list of input filters.
     */
    CARAPI GetFilters(
        /* [out, callee] */ ArrayOf<IInputFilter*>** filters);

    /**
     * When a TextView is used to display a useful piece of information to the user (such as a
     * contact's address), it should be made selectable, so that the user can select and copy this
     * content.
     *
     * Use {@link #setTextIsSelectable(boolean)} or the
     * {@link android.R.styleable#TextView_textIsSelectable} XML attribute to make this TextView
     * selectable (text is not selectable by default).
     *
     * Note that this method simply returns the state of this flag. Although this flag has to be set
     * in order to select text in non-editable TextView, the content of an {@link EditText} can
     * always be selected, independently of the value of this flag.
     *
     * @return True if the text displayed in this TextView can be selected by the user.
     *
     * @attr ref android.R.styleable#TextView_textIsSelectable
     */
    CARAPI IsTextSelectable(
        /* [out] */ Boolean* selectable);

    /**
     * Sets whether or not (default) the content of this view is selectable by the user.
     *
     * Note that this methods affect the {@link #setFocusable(boolean)},
     * {@link #setFocusableInTouchMode(boolean)} {@link #setClickable(boolean)} and
     * {@link #setLongClickable(boolean)} states and you may want to restore these if they were
     * customized.
     *
     * See {@link #isTextSelectable} for details.
     *
     * @param selectable Whether or not the content of this TextView should be selectable.
     */
    CARAPI SetTextIsSelectable(
        /* [in] */ Boolean selectable);

    /**
     * Return the number of lines of text, or 0 if the internal Layout has not
     * been built.
     */
    CARAPI GetLineCount(
        /* [out] */ Int32* count);

    /**
     * Return the baseline for the specified line (0...getLineCount() - 1)
     * If bounds is not null, return the top, left, right, bottom extents
     * of the specified line in it. If the internal Layout has not been built,
     * return 0 and set bounds to (0, 0, 0, 0)
     * @param line which line to examine (0..getLineCount() - 1)
     * @param bounds Optional. If not null, it returns the extent of the line
     * @return the Y-coordinate of the baseline
     */
    CARAPI GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds,
        /* [out] */ Int32* y);

    /**
     * Resets the mErrorWasChanged flag, so that future calls to {@link #setError(CharSequence)}
     * can be recorded.
     * @hide
     */
    CARAPI ResetErrorChangedFlag();

    /**
     * @hide
     */
    CARAPI HideErrorIfUnchanged();

    /**
     * If this TextView contains editable content, extract a portion of it
     * based on the information in <var>request</var> in to <var>outText</var>.
     * @return Returns true if the text was successfully extracted, else false.
     */
    CARAPI ExtractText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ IExtractedText* outText,
        /* [out] */ Boolean* result);

    /**
     * Apply to this text view the given extracted text, as previously
     * returned by {@link #extractText(ExtractedTextRequest, ExtractedText)}.
     */
    CARAPI SetExtractedText(
        /* [in] */ IExtractedText* text);

    /**
     * @hide
     */
    CARAPI SetExtracting(
        /* [in] */ IExtractedTextRequest* req);

    /**
     * Called by the framework in response to a text completion from
     * the current input method, provided by it calling
     * {@link InputConnection#commitCompletion
     * InputConnection.commitCompletion()}.  The default implementation does
     * nothing; text views that are supporting auto-completion should override
     * this to do their desired behavior.
     *
     * @param text The auto complete text the user has selected.
     */
    CARAPI OnCommitCompletion(
        /* [in] */ ICompletionInfo* text);

    /**
     * Called by the framework in response to a text auto-correction (such as fixing a typo using a
     * a dictionnary) from the current input method, provided by it calling
     * {@link InputConnection#commitCorrection} InputConnection.commitCorrection()}. The default
     * implementation flashes the background of the corrected word to provide feedback to the user.
     *
     * @param info The auto correct info about the text that was corrected.
     */
    CARAPI OnCommitCorrection(
        /* [in] */ ICorrectionInfo* info);

    CARAPI BeginBatchEdit();

    CARAPI EndBatchEdit();

    /**
     * Called by the framework in response to a request to begin a batch
     * of edit operations through a call to link {@link #beginBatchEdit()}.
     */
    CARAPI OnBeginBatchEdit();

    /**
     * Called by the framework in response to a request to end a batch
     * of edit operations through a call to link {@link #endBatchEdit}.
     */
    CARAPI OnEndBatchEdit();

    /**
     * Called by the framework in response to a private command from the
     * current method, provided by it calling
     * {@link InputConnection#performPrivateCommand
     * InputConnection.performPrivateCommand()}.
     *
     * @param action The action name of the command.
     * @param data Any additional data for the command.  This may be null.
     * @return Return true if you handled the command, else false.
     */
    CARAPI OnPrivateIMECommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data,
        /* [out] */ Boolean* result);

    /**
     * Set whether the TextView includes extra top and bottom padding to make
     * room for accents that go above the normal ascent and descent.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_includeFontPadding
     */
    CARAPI SetIncludeFontPadding(
        /* [in] */ Boolean includepad);

    /**
     * Gets whether the TextView includes extra top and bottom padding to make
     * room for accents that go above the normal ascent and descent.
     *
     * @see #setIncludeFontPadding(boolean)
     *
     * @attr ref android.R.styleable#TextView_includeFontPadding
     */
    CARAPI GetIncludeFontPadding(
        /* [out] */ Boolean* padding);

    /**
     * Move the point, specified by the offset, into the view if it is needed.
     * This has to be called after layout. Returns true if anything changed.
     */
    CARAPI BringPointIntoView(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* result);

    /**
     * Move the cursor, if needed, so that it is at an offset that is visible
     * to the user.  This will not move the cursor if it represents more than
     * one character (a selection range).  This will only work if the
     * TextView contains spannable text; otherwise it will do nothing.
     *
     * @return True if the cursor was actually moved, false otherwise.
     */
    CARAPI MoveCursorToVisibleOffset(
        /* [out] */ Boolean* result);

    /**
     * Convenience for {@link Selection#getSelectionStart}.
     */
    CARAPI GetSelectionStart(
        /* [out] */ Int32* start);

    /**
     * Convenience for {@link Selection#getSelectionEnd}.
     */
    CARAPI GetSelectionEnd(
        /* [out] */ Int32* end);

    /**
     * Return true if there is a selection inside this text view.
     */
    CARAPI HasSelection(
        /* [out] */ Boolean* result);

    /**
     * Sets the properties of this field (lines, horizontally scrolling,
     * transformation method) to be for a single-line input.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    CARAPI SetSingleLine();

    /**
     * Sets the properties of this field to transform input to ALL CAPS
     * display. This may use a "small caps" formatting if available.
     * This setting will be ignored if this field is editable or selectable.
     *
     * This call replaces the current transformation method. Disabling this
     * will not necessarily restore the previous behavior from before this
     * was enabled.
     *
     * @see #setTransformationMethod(TransformationMethod)
     * @attr ref android.R.styleable#TextView_textAllCaps
     */
    CARAPI SetAllCaps(
        /* [in] */ Boolean allCaps);

    /**
     * If true, sets the properties of this field (lines, horizontally
     * scrolling, transformation method) to be for a single-line input;
     * if false, restores these to the default conditions.
     * Note that calling this with false restores default conditions,
     * not necessarily those that were in effect prior to calling
     * it with true.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    CARAPI SetSingleLine(
        /* [in] */ Boolean singleLine);

    /**
     * Causes words in the text that are longer than the view is wide
     * to be ellipsized instead of broken in the middle.  You may also
     * want to {@link #setSingleLine} or {@link #setHorizontallyScrolling}
     * to constrain the text to a single line.  Use <code>null</code>
     * to turn off ellipsizing.
     *
     * @attr ref android.R.styleable#TextView_ellipsize
     */
    CARAPI SetEllipsize(
        /* [in] */ TextUtilsTruncateAt where);

    /**
     * Sets how many times to repeat the marquee animation. Only applied if the
     * TextView has marquee enabled. Set to -1 to repeat indefinitely.
     *
     * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
     */
    CARAPI SetMarqueeRepeatLimit(
        /* [in] */ Int32 marqueeLimit);

    /**
     * Gets the number of times the marquee animation is repeated. Only meaningful if the
     * TextView has marquee enabled.
     *
     * @return the number of times the marquee animation is repeated. -1 if the animation
     * repeats indefinitely
     *
     * @see #setMarqueeRepeatLimit(int)
     *
     * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
     */
    CARAPI GetMarqueeRepeatLimit(
        /* [out] */ Int32* marqueeLimit);

    /**
     * Returns where, if anywhere, words that are longer than the view
     * is wide should be ellipsized.
     */
    CARAPI GetEllipsize(
        /* [out] */ TextUtilsTruncateAt* size);

    /**
     * Set the TextView so that when it takes focus, all the text is
     * selected.
     *
     * @attr ref android.R.styleable#TextView_selectAllOnFocus
     */
    CARAPI SetSelectAllOnFocus(
        /* [in] */ Boolean selectAllOnFocus);

    /**
     * Set whether the cursor is visible.  The default is true.
     *
     * @attr ref android.R.styleable#TextView_cursorVisible
     */
    CARAPI SetCursorVisible(
        /* [in] */ Boolean visible);

    /**
     * @return whether or not the cursor is visible (assuming this TextView is editable)
     *
     * @see #setCursorVisible(boolean)
     *
     * @attr ref android.R.styleable#TextView_cursorVisible
     */
    CARAPI IsCursorVisible(
        /* [out] */ Boolean* visible);

    /**
     * Called when a context menu option for the text view is selected.  Currently
     * this will be one of: {@link android.R.id#selectAll},
     * {@link android.R.id#startSelectingText},
     * {@link android.R.id#cut}, {@link android.R.id#copy},
     * {@link android.R.id#paste}, {@link android.R.id#copyUrl},
     * or {@link android.R.id#switchInputMethod}.
     */
    CARAPI OnTextContextMenuItem(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* result);

    /**
     * Adds a TextWatcher to the list of those whose methods are called
     * whenever this TextView's text changes.
     * <p>
     * In 1.0, the {@link TextWatcher#afterTextChanged} method was erroneously
     * not called after {@link #setText} calls.  Now, doing {@link #setText}
     * if there are any text changed listeners forces the buffer type to
     * Editable if it would not otherwise be and does call this method.
     */
    CARAPI AddTextChangedListener(
        /* [in] */ ITextWatcher* watcher);

    /**
     * Removes the specified TextWatcher from the list of those whose
     * methods are called
     * whenever this TextView's text changes.
     */
    CARAPI RemoveTextChangedListener(
        /* [in] */ ITextWatcher* watcher);

    /**
     * Use {@link BaseInputConnection#removeComposingSpans
     * BaseInputConnection.removeComposingSpans()} to remove any IME composing
     * state from this text view.
     */
    CARAPI ClearComposingText();

    /**
     * Returns true, only while processing a touch gesture, if the initial
     * touch down event caused focus to move to the text view and as a result
     * its selection changed.  Only valid while processing the touch gesture
     * of interest.
     */
    CARAPI DidTouchFocusSelect(
        /* [out] */ Boolean* result);

    CARAPI SetScroller(
        /* [in] */ IScroller* s);

    /**
     * This is a temporary method. Future versions may support multi-locale text.
     * Caveat: This method may not return the latest text services locale, but this should be
     * acceptable and it's more important to make this method asynchronous.
     *
     * @return The locale that should be used for a word iterator and a spell checker
     * in this TextView, based on the current spell checker settings,
     * the current IME's locale, or the system default locale.
     * @hide
     */
    // TODO: Support multi-locale
    // TODO: Update the text services locale immediately after the keyboard locale is switched
    // by catching intent of keyboard switch event
    GetTextServicesLocale(
        /* [out] */ ILocale** locale);

    /**
     * This method is used by the ArrowKeyMovementMethod to jump from one word to the other.
     * Made available to achieve a consistent behavior.
     * @hide
     */
    CARAPI GetWordIterator(
        /* [out] */ IWordIterator** iterator);

    /**
     * Gets the text reported for accessibility purposes.
     *
     * @return The accessibility text.
     *
     * @hide
     */
    CARAPI GetTextForAccessibility(
        /* [out] */ ICharSequence** text);

    /**
     * Returns whether this text view is a current input method target.  The
     * default implementation just checks with {@link InputMethodManager}.
     */
    CARAPI IsInputMethodTarget(
        /* [out] */ Boolean* result);

    /**
     * Return whether or not suggestions are enabled on this TextView. The suggestions are generated
     * by the IME or by the spell checker as the user types. This is done by adding
     * {@link SuggestionSpan}s to the text.
     *
     * When suggestions are enabled (default), this list of suggestions will be displayed when the
     * user asks for them on these parts of the text. This value depends on the inputType of this
     * TextView.
     *
     * The class of the input type must be {@link InputType#TYPE_CLASS_TEXT}.
     *
     * In addition, the type variation must be one of
     * {@link InputType#TYPE_TEXT_VARIATION_NORMAL},
     * {@link InputType#TYPE_TEXT_VARIATION_EMAIL_SUBJECT},
     * {@link InputType#TYPE_TEXT_VARIATION_LONG_MESSAGE},
     * {@link InputType#TYPE_TEXT_VARIATION_SHORT_MESSAGE} or
     * {@link InputType#TYPE_TEXT_VARIATION_WEB_EDIT_TEXT}.
     *
     * And finally, the {@link InputType#TYPE_TEXT_FLAG_NO_SUGGESTIONS} flag must <i>not</i> be set.
     *
     * @return true if the suggestions popup window is enabled, based on the inputType.
     */
    CARAPI IsSuggestionsEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * If provided, this ActionMode.Callback will be used to create the ActionMode when text
     * selection is initiated in this View.
     *
     * The standard implementation populates the menu with a subset of Select All, Cut, Copy and
     * Paste actions, depending on what this View supports.
     *
     * A custom implementation can add new entries in the default menu in its
     * {@link android.view.ActionMode.Callback#onPrepareActionMode(ActionMode, Menu)} method. The
     * default actions can also be removed from the menu using {@link Menu#removeItem(int)} and
     * passing {@link android.R.id#selectAll}, {@link android.R.id#cut}, {@link android.R.id#copy}
     * or {@link android.R.id#paste} ids as parameters.
     *
     * Returning false from
     * {@link android.view.ActionMode.Callback#onCreateActionMode(ActionMode, Menu)} will prevent
     * the action mode from being started.
     *
     * Action click events should be handled by the custom implementation of
     * {@link android.view.ActionMode.Callback#onActionItemClicked(ActionMode, MenuItem)}.
     *
     * Note that text selection mode is not started when a TextView receives focus and the
     * {@link android.R.attr#selectAllOnFocus} flag has been set. The content is highlighted in
     * that case, to allow for quick replacement.
     */
    CARAPI SetCustomSelectionActionModeCallback(
        /* [in] */ IActionModeCallback* actionModeCallback);

    /**
     * Retrieves the value set in {@link #setCustomSelectionActionModeCallback}. Default is null.
     *
     * @return The current custom selection callback.
     */
    CARAPI GetCustomSelectionActionModeCallback(
        /* [out] */ IActionModeCallback** actionModeCallback);

    /**
     * Get the character offset closest to the specified absolute position. A typical use case is to
     * pass the result of {@link MotionEvent#getX()} and {@link MotionEvent#getY()} to this method.
     *
     * @param x The horizontal absolute position of a point on screen
     * @param y The vertical absolute position of a point on screen
     * @return the character offset for the character whose position is closest to the specified
     *  position. Returns -1 if there is no layout.
     */
    CARAPI GetOffsetForPosition(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Int32* offset);

    CARAPI RemoveMisspelledSpans(
        /* [in] */ ISpannable* spannable);

    CARAPI OnLocaleChanged();

    CARAPI InvalidateRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean invalidateCursor);
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTEXTVIEW_H__
