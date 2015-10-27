

/**
 * Helper class to add wearable extensions to notifications.
 * <p class="note"> See
 * <a href="{@docRoot}wear/notifications/creating.html">Creating Notifications
 * for Android Wear</a> for more information on how to use this class.
 * <p>
 * To create a notification with wearable extensions:
 * <ol>
 *   <li>Create a {@link android.app.Notification.Builder}, setting any desired
 *   properties.
 *   <li>Create a {@link android.app.Notification.WearableExtender}.
 *   <li>Set wearable-specific properties using the
 *   {@code add} and {@code set} methods of {@link android.app.Notification.WearableExtender}.
 *   <li>Call {@link android.app.Notification.Builder#extend} to apply the extensions to a
 *   notification.
 *   <li>Post the notification to the notification system with the
 *   {@code NotificationManager.notify(...)} methods.
 * </ol>
 *
 * <pre class="prettyprint">
 * Notification notif = new Notification.Builder(mContext)
 *         .setContentTitle(&quot;New mail from &quot; + sender.toString())
 *         .setContentText(subject)
 *         .setSmallIcon(R.drawable.new_mail)
 *         .extend(new Notification.WearableExtender()
 *                 .setContentIcon(R.drawable.new_mail))
 *         .build();
 * NotificationManager notificationManger =
 *         (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
 * notificationManger.notify(0, notif);</pre>
 *
 * <p>Wearable extensions can be accessed on an existing notification by using the
 * {@code WearableExtender(Notification)} constructor,
 * and then using the {@code get} methods to access values.
 *
 * <pre class="prettyprint">
 * Notification.WearableExtender wearableExtender = new Notification.WearableExtender(
 *         notification);
 * List&lt;Notification&gt; pages = wearableExtender.getPages();</pre>
 */
public static final class WearableExtender implements Extender {
    /**
     * Sentinel value for an action index that is unset.
     */
    public static final int UNSET_ACTION_INDEX = -1;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification with
     * default sizing.
     * <p>For custom display notifications created using {@link #setDisplayIntent},
     * the default is {@link #SIZE_LARGE}. All other notifications size automatically based
     * on their content.
     */
    public static final int SIZE_DEFAULT = 0;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification
     * with an extra small size.
     * <p>This value is only applicable for custom display notifications created using
     * {@link #setDisplayIntent}.
     */
    public static final int SIZE_XSMALL = 1;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification
     * with a small size.
     * <p>This value is only applicable for custom display notifications created using
     * {@link #setDisplayIntent}.
     */
    public static final int SIZE_SMALL = 2;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification
     * with a medium size.
     * <p>This value is only applicable for custom display notifications created using
     * {@link #setDisplayIntent}.
     */
    public static final int SIZE_MEDIUM = 3;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification
     * with a large size.
     * <p>This value is only applicable for custom display notifications created using
     * {@link #setDisplayIntent}.
     */
    public static final int SIZE_LARGE = 4;

    /**
     * Size value for use with {@link #setCustomSizePreset} to show this notification
     * full screen.
     * <p>This value is only applicable for custom display notifications created using
     * {@link #setDisplayIntent}.
     */
    public static final int SIZE_FULL_SCREEN = 5;

    /** Notification extra which contains wearable extensions */
    private static final String EXTRA_WEARABLE_EXTENSIONS = "android.wearable.EXTENSIONS";

    // Keys within EXTRA_WEARABLE_OPTIONS for wearable options.
    private static final String KEY_ACTIONS = "actions";
    private static final String KEY_FLAGS = "flags";
    private static final String KEY_DISPLAY_INTENT = "displayIntent";
    private static final String KEY_PAGES = "pages";
    private static final String KEY_BACKGROUND = "background";
    private static final String KEY_CONTENT_ICON = "contentIcon";
    private static final String KEY_CONTENT_ICON_GRAVITY = "contentIconGravity";
    private static final String KEY_CONTENT_ACTION_INDEX = "contentActionIndex";
    private static final String KEY_CUSTOM_SIZE_PRESET = "customSizePreset";
    private static final String KEY_CUSTOM_CONTENT_HEIGHT = "customContentHeight";
    private static final String KEY_GRAVITY = "gravity";

    // Flags bitwise-ored to mFlags
    private static final int FLAG_CONTENT_INTENT_AVAILABLE_OFFLINE = 0x1;
    private static final int FLAG_HINT_HIDE_ICON = 1 << 1;
    private static final int FLAG_HINT_SHOW_BACKGROUND_ONLY = 1 << 2;
    private static final int FLAG_START_SCROLL_BOTTOM = 1 << 3;

    // Default value for flags integer
    private static final int DEFAULT_FLAGS = FLAG_CONTENT_INTENT_AVAILABLE_OFFLINE;

    private static final int DEFAULT_CONTENT_ICON_GRAVITY = Gravity.END;
    private static final int DEFAULT_GRAVITY = Gravity.BOTTOM;

    private ArrayList<Action> mActions = new ArrayList<Action>();
    private int mFlags = DEFAULT_FLAGS;
    private PendingIntent mDisplayIntent;
    private ArrayList<Notification> mPages = new ArrayList<Notification>();
    private Bitmap mBackground;
    private int mContentIcon;
    private int mContentIconGravity = DEFAULT_CONTENT_ICON_GRAVITY;
    private int mContentActionIndex = UNSET_ACTION_INDEX;
    private int mCustomSizePreset = SIZE_DEFAULT;
    private int mCustomContentHeight;
    private int mGravity = DEFAULT_GRAVITY;

    /**
     * Create a {@link android.app.Notification.WearableExtender} with default
     * options.
     */
    public WearableExtender() {
    }

    public WearableExtender(Notification notif) {
        Bundle wearableBundle = notif.extras.getBundle(EXTRA_WEARABLE_EXTENSIONS);
        if (wearableBundle != null) {
            List<Action> actions = wearableBundle.getParcelableArrayList(KEY_ACTIONS);
            if (actions != null) {
                mActions.addAll(actions);
            }

            mFlags = wearableBundle.getInt(KEY_FLAGS, DEFAULT_FLAGS);
            mDisplayIntent = wearableBundle.getParcelable(KEY_DISPLAY_INTENT);

            Notification[] pages = getNotificationArrayFromBundle(
                    wearableBundle, KEY_PAGES);
            if (pages != null) {
                Collections.addAll(mPages, pages);
            }

            mBackground = wearableBundle.getParcelable(KEY_BACKGROUND);
            mContentIcon = wearableBundle.getInt(KEY_CONTENT_ICON);
            mContentIconGravity = wearableBundle.getInt(KEY_CONTENT_ICON_GRAVITY,
                    DEFAULT_CONTENT_ICON_GRAVITY);
            mContentActionIndex = wearableBundle.getInt(KEY_CONTENT_ACTION_INDEX,
                    UNSET_ACTION_INDEX);
            mCustomSizePreset = wearableBundle.getInt(KEY_CUSTOM_SIZE_PRESET,
                    SIZE_DEFAULT);
            mCustomContentHeight = wearableBundle.getInt(KEY_CUSTOM_CONTENT_HEIGHT);
            mGravity = wearableBundle.getInt(KEY_GRAVITY, DEFAULT_GRAVITY);
        }
    }

    /**
     * Apply wearable extensions to a notification that is being built. This is typically
     * called by the {@link android.app.Notification.Builder#extend} method of
     * {@link android.app.Notification.Builder}.
     */
    @Override
    public Notification.Builder extend(Notification.Builder builder) {
        Bundle wearableBundle = new Bundle();

        if (!mActions.isEmpty()) {
            wearableBundle.putParcelableArrayList(KEY_ACTIONS, mActions);
        }
        if (mFlags != DEFAULT_FLAGS) {
            wearableBundle.putInt(KEY_FLAGS, mFlags);
        }
        if (mDisplayIntent != null) {
            wearableBundle.putParcelable(KEY_DISPLAY_INTENT, mDisplayIntent);
        }
        if (!mPages.isEmpty()) {
            wearableBundle.putParcelableArray(KEY_PAGES, mPages.toArray(
                    new Notification[mPages.size()]));
        }
        if (mBackground != null) {
            wearableBundle.putParcelable(KEY_BACKGROUND, mBackground);
        }
        if (mContentIcon != 0) {
            wearableBundle.putInt(KEY_CONTENT_ICON, mContentIcon);
        }
        if (mContentIconGravity != DEFAULT_CONTENT_ICON_GRAVITY) {
            wearableBundle.putInt(KEY_CONTENT_ICON_GRAVITY, mContentIconGravity);
        }
        if (mContentActionIndex != UNSET_ACTION_INDEX) {
            wearableBundle.putInt(KEY_CONTENT_ACTION_INDEX,
                    mContentActionIndex);
        }
        if (mCustomSizePreset != SIZE_DEFAULT) {
            wearableBundle.putInt(KEY_CUSTOM_SIZE_PRESET, mCustomSizePreset);
        }
        if (mCustomContentHeight != 0) {
            wearableBundle.putInt(KEY_CUSTOM_CONTENT_HEIGHT, mCustomContentHeight);
        }
        if (mGravity != DEFAULT_GRAVITY) {
            wearableBundle.putInt(KEY_GRAVITY, mGravity);
        }

        builder.getExtras().putBundle(EXTRA_WEARABLE_EXTENSIONS, wearableBundle);
        return builder;
    }

    @Override
    public WearableExtender clone() {
        WearableExtender that = new WearableExtender();
        that.mActions = new ArrayList<Action>(this.mActions);
        that.mFlags = this.mFlags;
        that.mDisplayIntent = this.mDisplayIntent;
        that.mPages = new ArrayList<Notification>(this.mPages);
        that.mBackground = this.mBackground;
        that.mContentIcon = this.mContentIcon;
        that.mContentIconGravity = this.mContentIconGravity;
        that.mContentActionIndex = this.mContentActionIndex;
        that.mCustomSizePreset = this.mCustomSizePreset;
        that.mCustomContentHeight = this.mCustomContentHeight;
        that.mGravity = this.mGravity;
        return that;
    }

    /**
     * Add a wearable action to this notification.
     *
     * <p>When wearable actions are added using this method, the set of actions that
     * show on a wearable device splits from devices that only show actions added
     * using {@link android.app.Notification.Builder#addAction}. This allows for customization
     * of which actions display on different devices.
     *
     * @param action the action to add to this notification
     * @return this object for method chaining
     * @see android.app.Notification.Action
     */
    public WearableExtender addAction(Action action) {
        mActions.add(action);
        return this;
    }

    /**
     * Adds wearable actions to this notification.
     *
     * <p>When wearable actions are added using this method, the set of actions that
     * show on a wearable device splits from devices that only show actions added
     * using {@link android.app.Notification.Builder#addAction}. This allows for customization
     * of which actions display on different devices.
     *
     * @param actions the actions to add to this notification
     * @return this object for method chaining
     * @see android.app.Notification.Action
     */
    public WearableExtender addActions(List<Action> actions) {
        mActions.addAll(actions);
        return this;
    }

    /**
     * Clear all wearable actions present on this builder.
     * @return this object for method chaining.
     * @see #addAction
     */
    public WearableExtender clearActions() {
        mActions.clear();
        return this;
    }

    /**
     * Get the wearable actions present on this notification.
     */
    public List<Action> getActions() {
        return mActions;
    }

    /**
     * Set an intent to launch inside of an activity view when displaying
     * this notification. The {@link PendingIntent} provided should be for an activity.
     *
     * <pre class="prettyprint">
     * Intent displayIntent = new Intent(context, MyDisplayActivity.class);
     * PendingIntent displayPendingIntent = PendingIntent.getActivity(context,
     *         0, displayIntent, PendingIntent.FLAG_UPDATE_CURRENT);
     * Notification notif = new Notification.Builder(context)
     *         .extend(new Notification.WearableExtender()
     *                 .setDisplayIntent(displayPendingIntent)
     *                 .setCustomSizePreset(Notification.WearableExtender.SIZE_MEDIUM))
     *         .build();</pre>
     *
     * <p>The activity to launch needs to allow embedding, must be exported, and
     * should have an empty task affinity. It is also recommended to use the device
     * default light theme.
     *
     * <p>Example AndroidManifest.xml entry:
     * <pre class="prettyprint">
     * &lt;activity android:name=&quot;com.example.MyDisplayActivity&quot;
     *     android:exported=&quot;true&quot;
     *     android:allowEmbedded=&quot;true&quot;
     *     android:taskAffinity=&quot;&quot;
     *     android:theme=&quot;@android:style/Theme.DeviceDefault.Light&quot; /&gt;</pre>
     *
     * @param intent the {@link PendingIntent} for an activity
     * @return this object for method chaining
     * @see android.app.Notification.WearableExtender#getDisplayIntent
     */
    public WearableExtender setDisplayIntent(PendingIntent intent) {
        mDisplayIntent = intent;
        return this;
    }

    /**
     * Get the intent to launch inside of an activity view when displaying this
     * notification. This {@code PendingIntent} should be for an activity.
     */
    public PendingIntent getDisplayIntent() {
        return mDisplayIntent;
    }

    /**
     * Add an additional page of content to display with this notification. The current
     * notification forms the first page, and pages added using this function form
     * subsequent pages. This field can be used to separate a notification into multiple
     * sections.
     *
     * @param page the notification to add as another page
     * @return this object for method chaining
     * @see android.app.Notification.WearableExtender#getPages
     */
    public WearableExtender addPage(Notification page) {
        mPages.add(page);
        return this;
    }

    /**
     * Add additional pages of content to display with this notification. The current
     * notification forms the first page, and pages added using this function form
     * subsequent pages. This field can be used to separate a notification into multiple
     * sections.
     *
     * @param pages a list of notifications
     * @return this object for method chaining
     * @see android.app.Notification.WearableExtender#getPages
     */
    public WearableExtender addPages(List<Notification> pages) {
        mPages.addAll(pages);
        return this;
    }

    /**
     * Clear all additional pages present on this builder.
     * @return this object for method chaining.
     * @see #addPage
     */
    public WearableExtender clearPages() {
        mPages.clear();
        return this;
    }

    /**
     * Get the array of additional pages of content for displaying this notification. The
     * current notification forms the first page, and elements within this array form
     * subsequent pages. This field can be used to separate a notification into multiple
     * sections.
     * @return the pages for this notification
     */
    public List<Notification> getPages() {
        return mPages;
    }

    /**
     * Set a background image to be displayed behind the notification content.
     * Contrary to the {@link android.app.Notification.BigPictureStyle}, this background
     * will work with any notification style.
     *
     * @param background the background bitmap
     * @return this object for method chaining
     * @see android.app.Notification.WearableExtender#getBackground
     */
    public WearableExtender setBackground(Bitmap background) {
        mBackground = background;
        return this;
    }

    /**
     * Get a background image to be displayed behind the notification content.
     * Contrary to the {@link android.app.Notification.BigPictureStyle}, this background
     * will work with any notification style.
     *
     * @return the background image
     * @see android.app.Notification.WearableExtender#setBackground
     */
    public Bitmap getBackground() {
        return mBackground;
    }

    /**
     * Set an icon that goes with the content of this notification.
     */
    public WearableExtender setContentIcon(int icon) {
        mContentIcon = icon;
        return this;
    }

    /**
     * Get an icon that goes with the content of this notification.
     */
    public int getContentIcon() {
        return mContentIcon;
    }

    /**
     * Set the gravity that the content icon should have within the notification display.
     * Supported values include {@link android.view.Gravity#START} and
     * {@link android.view.Gravity#END}. The default value is {@link android.view.Gravity#END}.
     * @see #setContentIcon
     */
    public WearableExtender setContentIconGravity(int contentIconGravity) {
        mContentIconGravity = contentIconGravity;
        return this;
    }

    /**
     * Get the gravity that the content icon should have within the notification display.
     * Supported values include {@link android.view.Gravity#START} and
     * {@link android.view.Gravity#END}. The default value is {@link android.view.Gravity#END}.
     * @see #getContentIcon
     */
    public int getContentIconGravity() {
        return mContentIconGravity;
    }

    /**
     * Set an action from this notification's actions to be clickable with the content of
     * this notification. This action will no longer display separately from the
     * notification's content.
     *
     * <p>For notifications with multiple pages, child pages can also have content actions
     * set, although the list of available actions comes from the main notification and not
     * from the child page's notification.
     *
     * @param actionIndex The index of the action to hoist onto the current notification page.
     *                    If wearable actions were added to the main notification, this index
     *                    will apply to that list, otherwise it will apply to the regular
     *                    actions list.
     */
    public WearableExtender setContentAction(int actionIndex) {
        mContentActionIndex = actionIndex;
        return this;
    }

    /**
     * Get the index of the notification action, if any, that was specified as being clickable
     * with the content of this notification. This action will no longer display separately
     * from the notification's content.
     *
     * <p>For notifications with multiple pages, child pages can also have content actions
     * set, although the list of available actions comes from the main notification and not
     * from the child page's notification.
     *
     * <p>If wearable specific actions were added to the main notification, this index will
     * apply to that list, otherwise it will apply to the regular actions list.
     *
     * @return the action index or {@link #UNSET_ACTION_INDEX} if no action was selected.
     */
    public int getContentAction() {
        return mContentActionIndex;
    }

    /**
     * Set the gravity that this notification should have within the available viewport space.
     * Supported values include {@link android.view.Gravity#TOP},
     * {@link android.view.Gravity#CENTER_VERTICAL} and {@link android.view.Gravity#BOTTOM}.
     * The default value is {@link android.view.Gravity#BOTTOM}.
     */
    public WearableExtender setGravity(int gravity) {
        mGravity = gravity;
        return this;
    }

    /**
     * Get the gravity that this notification should have within the available viewport space.
     * Supported values include {@link android.view.Gravity#TOP},
     * {@link android.view.Gravity#CENTER_VERTICAL} and {@link android.view.Gravity#BOTTOM}.
     * The default value is {@link android.view.Gravity#BOTTOM}.
     */
    public int getGravity() {
        return mGravity;
    }

    /**
     * Set the custom size preset for the display of this notification out of the available
     * presets found in {@link android.app.Notification.WearableExtender}, e.g.
     * {@link #SIZE_LARGE}.
     * <p>Some custom size presets are only applicable for custom display notifications created
     * using {@link android.app.Notification.WearableExtender#setDisplayIntent}. Check the
     * documentation for the preset in question. See also
     * {@link #setCustomContentHeight} and {@link #getCustomSizePreset}.
     */
    public WearableExtender setCustomSizePreset(int sizePreset) {
        mCustomSizePreset = sizePreset;
        return this;
    }

    /**
     * Get the custom size preset for the display of this notification out of the available
     * presets found in {@link android.app.Notification.WearableExtender}, e.g.
     * {@link #SIZE_LARGE}.
     * <p>Some custom size presets are only applicable for custom display notifications created
     * using {@link #setDisplayIntent}. Check the documentation for the preset in question.
     * See also {@link #setCustomContentHeight} and {@link #setCustomSizePreset}.
     */
    public int getCustomSizePreset() {
        return mCustomSizePreset;
    }

    /**
     * Set the custom height in pixels for the display of this notification's content.
     * <p>This option is only available for custom display notifications created
     * using {@link android.app.Notification.WearableExtender#setDisplayIntent}. See also
     * {@link android.app.Notification.WearableExtender#setCustomSizePreset} and
     * {@link #getCustomContentHeight}.
     */
    public WearableExtender setCustomContentHeight(int height) {
        mCustomContentHeight = height;
        return this;
    }

    /**
     * Get the custom height in pixels for the display of this notification's content.
     * <p>This option is only available for custom display notifications created
     * using {@link #setDisplayIntent}. See also {@link #setCustomSizePreset} and
     * {@link #setCustomContentHeight}.
     */
    public int getCustomContentHeight() {
        return mCustomContentHeight;
    }

    /**
     * Set whether the scrolling position for the contents of this notification should start
     * at the bottom of the contents instead of the top when the contents are too long to
     * display within the screen.  Default is false (start scroll at the top).
     */
    public WearableExtender setStartScrollBottom(boolean startScrollBottom) {
        setFlag(FLAG_START_SCROLL_BOTTOM, startScrollBottom);
        return this;
    }

    /**
     * Get whether the scrolling position for the contents of this notification should start
     * at the bottom of the contents instead of the top when the contents are too long to
     * display within the screen. Default is false (start scroll at the top).
     */
    public boolean getStartScrollBottom() {
        return (mFlags & FLAG_START_SCROLL_BOTTOM) != 0;
    }

    /**
     * Set whether the content intent is available when the wearable device is not connected
     * to a companion device.  The user can still trigger this intent when the wearable device
     * is offline, but a visual hint will indicate that the content intent may not be available.
     * Defaults to true.
     */
    public WearableExtender setContentIntentAvailableOffline(
            boolean contentIntentAvailableOffline) {
        setFlag(FLAG_CONTENT_INTENT_AVAILABLE_OFFLINE, contentIntentAvailableOffline);
        return this;
    }

    /**
     * Get whether the content intent is available when the wearable device is not connected
     * to a companion device.  The user can still trigger this intent when the wearable device
     * is offline, but a visual hint will indicate that the content intent may not be available.
     * Defaults to true.
     */
    public boolean getContentIntentAvailableOffline() {
        return (mFlags & FLAG_CONTENT_INTENT_AVAILABLE_OFFLINE) != 0;
    }

    /**
     * Set a hint that this notification's icon should not be displayed.
     * @param hintHideIcon {@code true} to hide the icon, {@code false} otherwise.
     * @return this object for method chaining
     */
    public WearableExtender setHintHideIcon(boolean hintHideIcon) {
        setFlag(FLAG_HINT_HIDE_ICON, hintHideIcon);
        return this;
    }

    /**
     * Get a hint that this notification's icon should not be displayed.
     * @return {@code true} if this icon should not be displayed, false otherwise.
     * The default value is {@code false} if this was never set.
     */
    public boolean getHintHideIcon() {
        return (mFlags & FLAG_HINT_HIDE_ICON) != 0;
    }

    /**
     * Set a visual hint that only the background image of this notification should be
     * displayed, and other semantic content should be hidden. This hint is only applicable
     * to sub-pages added using {@link #addPage}.
     */
    public WearableExtender setHintShowBackgroundOnly(boolean hintShowBackgroundOnly) {
        setFlag(FLAG_HINT_SHOW_BACKGROUND_ONLY, hintShowBackgroundOnly);
        return this;
    }

    /**
     * Get a visual hint that only the background image of this notification should be
     * displayed, and other semantic content should be hidden. This hint is only applicable
     * to sub-pages added using {@link android.app.Notification.WearableExtender#addPage}.
     */
    public boolean getHintShowBackgroundOnly() {
        return (mFlags & FLAG_HINT_SHOW_BACKGROUND_ONLY) != 0;
    }

    private void setFlag(int mask, boolean value) {
        if (value) {
            mFlags |= mask;
        } else {
            mFlags &= ~mask;
        }
    }
}