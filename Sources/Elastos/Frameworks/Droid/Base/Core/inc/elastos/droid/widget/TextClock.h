
#ifndef __ELASTOS_DROID_WIDGET_TEXTCLOCK_H__
#define __ELASTOS_DROID_WIDGET_TEXTCLOCK_H__

namespace Elastos {
namespace Droid {
namespace Widget {


/**
 * <p><code>TextClock</code> can display the current date and/or time as
 * a formatted string.</p>
 *
 * <p>This view honors the 24-hour format system setting. As such, it is
 * possible and recommended to provide two different formatting patterns:
 * one to display the date/time in 24-hour mode and one to display the
 * date/time in 12-hour mode.</p>
 *
 * <p>It is possible to determine whether the system is currently in
 * 24-hour mode by calling {@link #is24HourModeEnabled()}.</p>
 *
 * <p>The rules used by this widget to decide how to format the date and
 * time are the following:</p>
 * <ul>
 *     <li>In 24-hour mode:
 *         <ul>
 *             <li>Use the value returned by {@link #getFormat24Hour()} when non-null</li>
 *             <li>Otherwise, use the value returned by {@link #getFormat12Hour()} when non-null</li>
 *             <li>Otherwise, use {@link #DEFAULT_FORMAT_24_HOUR}</li>
 *         </ul>
 *     </li>
 *     <li>In 12-hour mode:
 *         <ul>
 *             <li>Use the value returned by {@link #getFormat12Hour()} when non-null</li>
 *             <li>Otherwise, use the value returned by {@link #getFormat24Hour()} when non-null</li>
 *             <li>Otherwise, use {@link #DEFAULT_FORMAT_12_HOUR}</li>
 *         </ul>
 *     </li>
 * </ul>
 *
 * <p>The {@link CharSequence} instances used as formatting patterns when calling either
 * {@link #setFormat24Hour(CharSequence)} or {@link #setFormat12Hour(CharSequence)} can
 * contain styling information. To do so, use a {@link android.text.Spanned} object.</p>
 *
 * @attr ref android.R.styleable#TextClock_format12Hour
 * @attr ref android.R.styleable#TextClock_format24Hour
 * @attr ref android.R.styleable#TextClock_timeZone
 */

public class TextClock : public TextView
{
public:
    TextClock();

    /**
     * Creates a new clock using the default patterns
     * {@link #DEFAULT_FORMAT_24_HOUR} and {@link #DEFAULT_FORMAT_12_HOUR}
     * respectively for the 24-hour and 12-hour modes.
     *
     * @param context The Context the view is running in, through which it can
     *        access the current theme, resources, etc.
     */
    TextClock(
        /* [in] */ IContext* context);

    /**
     * Creates a new clock inflated from XML. This object's properties are
     * intialized from the attributes specified in XML.
     *
     * This constructor uses a default style of 0, so the only attribute values
     * applied are those in the Context's Theme and the given AttributeSet.
     *
     * @param context The Context the view is running in, through which it can
     *        access the current theme, resources, etc.
     * @param attrs The attributes of the XML tag that is inflating the view
     */
    TextClock(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Creates a new clock inflated from XML. This object's properties are
     * intialized from the attributes specified in XML.
     *
     * @param context The Context the view is running in, through which it can
     *        access the current theme, resources, etc.
     * @param attrs The attributes of the XML tag that is inflating the view
     * @param defStyle The default style to apply to this view. If 0, no style
     *        will be applied (beyond what is included in the theme). This may
     *        either be an attribute resource, whose value will be retrieved
     *        from the current theme, or an explicit style resource
     */
    TextClock(
        /* [in] */ IContext context,
        /* [in] */ IAttributeSet* attrs
        /* [in] */ Int32 defStyle);

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    /**
     * Returns the formatting pattern used to display the date and/or time
     * in 12-hour mode. The formatting pattern syntax is described in
     * {@link DateFormat}.
     *
     * @return A {@link CharSequence} or null.
     *
     * @see #setFormat12Hour(CharSequence)
     * @see #is24HourModeEnabled()
     */
    //@ExportedProperty
    AutoPtr<ICharSequence> GetFormat12Hour();

    /**
     * Specifies the formatting pattern used to display the date and/or time
     * in 12-hour mode. The formatting pattern syntax is described in
     * {@link DateFormat}.
     *
     * If this pattern is set to null, {@link #getFormat24Hour()} will be used
     * even in 12-hour mode. If both 24-hour and 12-hour formatting patterns
     * are set to null, {@link #DEFAULT_FORMAT_24_HOUR} and
     * {@link #DEFAULT_FORMAT_12_HOUR} will be used instead.
     *
     * @param format A date/time formatting pattern as described in {@link DateFormat}
     *
     * @see #getFormat12Hour()
     * @see #is24HourModeEnabled()
     * @see #DEFAULT_FORMAT_12_HOUR
     * @see DateFormat
     *
     * @attr ref android.R.styleable#TextClock_format12Hour
     */
    //@RemotableViewMethod
    virtual CARAPI SetFormat12Hour(
        /* [in] */ ICharSequence* format);

    /**
     * Returns the formatting pattern used to display the date and/or time
     * in 24-hour mode. The formatting pattern syntax is described in
     * {@link DateFormat}.
     *
     * @return A {@link CharSequence} or null.
     *
     * @see #setFormat24Hour(CharSequence)
     * @see #is24HourModeEnabled()
     */
    //@ExportedProperty
    AutoPtr<ICharSequence> GetFormat24Hour();

    /**
     * Specifies the formatting pattern used to display the date and/or time
     * in 24-hour mode. The formatting pattern syntax is described in
     * {@link DateFormat}.
     *
     * If this pattern is set to null, {@link #getFormat12Hour()} will be used
     * even in 24-hour mode. If both 24-hour and 12-hour formatting patterns
     * are set to null, {@link #DEFAULT_FORMAT_24_HOUR} and
     * {@link #DEFAULT_FORMAT_12_HOUR} will be used instead.
     *
     * @param format A date/time formatting pattern as described in {@link DateFormat}
     *
     * @see #getFormat24Hour()
     * @see #is24HourModeEnabled()
     * @see #DEFAULT_FORMAT_24_HOUR
     * @see DateFormat
     *
     * @attr ref android.R.styleable#TextClock_format24Hour
     */
    //@RemotableViewMethod
    virtual CARAPI SetFormat24Hour(
        /* [in] */ ICharSequence* format);

    /**
     * Indicates whether the system is currently using the 24-hour mode.
     *
     * When the system is in 24-hour mode, this view will use the pattern
     * returned by {@link #getFormat24Hour()}. In 12-hour mode, the pattern
     * returned by {@link #getFormat12Hour()} is used instead.
     *
     * If either one of the formats is null, the other format is used. If
     * both formats are null, the default values {@link #DEFAULT_FORMAT_12_HOUR}
     * and {@link #DEFAULT_FORMAT_24_HOUR} are used instead.
     *
     * @return true if time should be displayed in 24-hour format, false if it
     *         should be displayed in 12-hour format.
     *
     * @see #setFormat12Hour(CharSequence)
     * @see #getFormat12Hour()
     * @see #setFormat24Hour(CharSequence)
     * @see #getFormat24Hour()
     */
    Boolean Is24HourModeEnabled();

    /**
     * Indicates which time zone is currently used by this view.
     *
     * @return The ID of the current time zone or null if the default time zone,
     *         as set by the user, must be used
     *
     * @see TimeZone
     * @see java.util.TimeZone#getAvailableIDs()
     * @see #setTimeZone(String)
     */
    String GetTimeZone();

    /**
     * Sets the specified time zone to use in this clock. When the time zone
     * is set through this method, system time zone changes (when the user
     * sets the time zone in settings for instance) will be ignored.
     *
     * @param timeZone The desired time zone's ID as specified in {@link TimeZone}
     *                 or null to user the time zone specified by the user
     *                 (system time zone)
     *
     * @see #getTimeZone()
     * @see java.util.TimeZone#getAvailableIDs()
     * @see TimeZone#getTimeZone(String)
     *
     * @attr ref android.R.styleable#TextClock_timeZone
     */
    //@RemotableViewMethod
    virtual CARAPI SetTimeZone(
        /* [in] */ const String& timeZone);

protected:
    //@Override
    virtual CARAPI OnAttachedToWindow();

    //@Override
    virtual CARAPI OnDetachedFromWindow();

private:
    CARAPI_(void) PrivateInit();

    CARAPI_(void) CreateTime(
        /* [in] */ const String& timeZone);

    /**
     * Selects either one of {@link #getFormat12Hour()} or {@link #getFormat24Hour()}
     * depending on whether the user has selected 24-hour format.
     *
     * Calling this method does not schedule or unschedule the time ticker.
     */
    CARAPI_(void) ChooseFormat();

    /**
     * Selects either one of {@link #getFormat12Hour()} or {@link #getFormat24Hour()}
     * depending on whether the user has selected 24-hour format.
     *
     * @param handleTicker true if calling this method should schedule/unschedule the
     *                     time ticker, false otherwise
     */
    CARAPI_(void) ChooseFormat(
        /* [in] */ Boolean handleTicker);

    /**
     * Returns a if not null, else return b if not null, else return c.
     */
    static AutoPtr<ICharSequence> Abc(
        /* [in] */ ICharSequence* a,
        /* [in] */ ICharSequence* b,
        /* [in] */ ICharSequence* c);

    CARAPI_(void) RegisterReceiver();

    CARAPI_(void) RegisterObserver();

    CARAPI_(void) UnregisterReceiver();

    CARAPI_(void) UnregisterObserver();

    CARAPI_(void) OnTimeChanged();

private:
    /**
     * The default formatting pattern in 12-hour mode. This pattenr is used
     * if {@link #setFormat12Hour(CharSequence)} is called with a null pattern
     * or if no pattern was specified when creating an instance of this class.
     *
     * This default pattern shows only the time, hours and minutes, and an am/pm
     * indicator.
     *
     * @see #setFormat12Hour(CharSequence)
     * @see #getFormat12Hour()
     */
    static AutoPtr<ICharSequence> DEFAULT_FORMAT_12_HOUR; // = "h:mm aa"

    /**
     * The default formatting pattern in 24-hour mode. This pattenr is used
     * if {@link #setFormat24Hour(CharSequence)} is called with a null pattern
     * or if no pattern was specified when creating an instance of this class.
     *
     * This default pattern shows only the time, hours and minutes.
     *
     * @see #setFormat24Hour(CharSequence)
     * @see #getFormat24Hour()
     */
    static AutoPtr<ICharSequence> DEFAULT_FORMAT_24_HOUR; // = "k:mm"

    AutoPtr<ICharSequence> mFormat12; //DEFAULT_FORMAT_12_HOUR
    AutoPtr<ICharSequence> mFormat24; //DEFAULT_FORMAT_24_HOUR

    //@ExportedProperty
    AutoPtr<ICharSequence> mFormat;
    AutoPtr<ICharSequence> mHasSeconds;

    Boolean mAttached;

    AutoPtr<ICalendar> mTime;
    String mTimeZone;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
