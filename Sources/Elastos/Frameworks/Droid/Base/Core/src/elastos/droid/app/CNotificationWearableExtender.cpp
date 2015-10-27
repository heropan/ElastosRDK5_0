
/**
* Wearable extender for notification actions. To add extensions to an action,
* create a new {@link android.app.Notification.Action.WearableExtender} object using
* the {@code WearableExtender()} constructor and apply it to a
* {@link android.app.Notification.Action.Builder} using
* {@link android.app.Notification.Action.Builder#extend}.
*
* <pre class="prettyprint">
* Notification.Action action = new Notification.Action.Builder(
*         R.drawable.archive_all, "Archive all", actionIntent)
*         .extend(new Notification.Action.WearableExtender()
*                 .setAvailableOffline(false))
*         .build();</pre>
*/
public static final class WearableExtender implements Extender {
/** Notification action extra which contains wearable extensions */
private static final String EXTRA_WEARABLE_EXTENSIONS = "android.wearable.EXTENSIONS";

private static final String KEY_FLAGS = "flags";

// Flags bitwise-ored to mFlags
private static final int FLAG_AVAILABLE_OFFLINE = 0x1;

// Default value for flags integer
private static final int DEFAULT_FLAGS = FLAG_AVAILABLE_OFFLINE;

private int mFlags = DEFAULT_FLAGS;

/**
 * Create a {@link android.app.Notification.Action.WearableExtender} with default
 * options.
 */
public WearableExtender() {
}

/**
 * Create a {@link android.app.Notification.Action.WearableExtender} by reading
 * wearable options present in an existing notification action.
 * @param action the notification action to inspect.
 */
public WearableExtender(Action action) {
    Bundle wearableBundle = action.getExtras().getBundle(EXTRA_WEARABLE_EXTENSIONS);
    if (wearableBundle != null) {
        mFlags = wearableBundle.getInt(KEY_FLAGS, DEFAULT_FLAGS);
    }
}

/**
 * Apply wearable extensions to a notification action that is being built. This is
 * typically called by the {@link android.app.Notification.Action.Builder#extend}
 * method of {@link android.app.Notification.Action.Builder}.
 */
@Override
public Action.Builder extend(Action.Builder builder) {
    Bundle wearableBundle = new Bundle();

    if (mFlags != DEFAULT_FLAGS) {
        wearableBundle.putInt(KEY_FLAGS, mFlags);
    }

    builder.getExtras().putBundle(EXTRA_WEARABLE_EXTENSIONS, wearableBundle);
    return builder;
}

@Override
public WearableExtender clone() {
    WearableExtender that = new WearableExtender();
    that.mFlags = this.mFlags;
    return that;
}

/**
 * Set whether this action is available when the wearable device is not connected to
 * a companion device. The user can still trigger this action when the wearable device is
 * offline, but a visual hint will indicate that the action may not be available.
 * Defaults to true.
 */
public WearableExtender setAvailableOffline(boolean availableOffline) {
    setFlag(FLAG_AVAILABLE_OFFLINE, availableOffline);
    return this;
}

/**
 * Get whether this action is available when the wearable device is not connected to
 * a companion device. The user can still trigger this action when the wearable device is
 * offline, but a visual hint will indicate that the action may not be available.
 * Defaults to true.
 */
public boolean isAvailableOffline() {
    return (mFlags & FLAG_AVAILABLE_OFFLINE) != 0;
}

private void setFlag(int mask, boolean value) {
    if (value) {
        mFlags |= mask;
    } else {
        mFlags &= ~mask;
    }
}
}