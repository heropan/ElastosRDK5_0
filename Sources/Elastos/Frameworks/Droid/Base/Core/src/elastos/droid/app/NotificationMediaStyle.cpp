

/**
 * Notification style for media playback notifications.
 *
 * In the expanded form, {@link Notification#bigContentView}, up to 5
 * {@link Notification.Action}s specified with
 * {@link Notification.Builder#addAction(int, CharSequence, PendingIntent) addAction} will be
 * shown as icon-only pushbuttons, suitable for transport controls. The Bitmap given to
 * {@link Notification.Builder#setLargeIcon(android.graphics.Bitmap) setLargeIcon()} will be
 * treated as album artwork.
 *
 * Unlike the other styles provided here, MediaStyle can also modify the standard-size
 * {@link Notification#contentView}; by providing action indices to
 * {@link #setShowActionsInCompactView(int...)} you can promote up to 2 actions to be displayed
 * in the standard view alongside the usual content.
 *
 * Notifications created with MediaStyle will have their category set to
 * {@link Notification#CATEGORY_TRANSPORT CATEGORY_TRANSPORT} unless you set a different
 * category using {@link Notification.Builder#setCategory(String) setCategory()}.
 *
 * Finally, if you attach a {@link android.media.session.MediaSession.Token} using
 * {@link android.app.Notification.MediaStyle#setMediaSession(MediaSession.Token)},
 * the System UI can identify this as a notification representing an active media session
 * and respond accordingly (by showing album artwork in the lockscreen, for example).
 *
 * To use this style with your Notification, feed it to
 * {@link Notification.Builder#setStyle(android.app.Notification.Style)} like so:
 * <pre class="prettyprint">
 * Notification noti = new Notification.Builder()
 *     .setSmallIcon(R.drawable.ic_stat_player)
 *     .setContentTitle(&quot;Track title&quot;)     // these three lines are optional
 *     .setContentText(&quot;Artist - Album&quot;)   // if you use
 *     .setLargeIcon(albumArtBitmap))      // setMediaSession(token)
 *     .setStyle(<b>new Notification.MediaStyle()</b>
 *         .setMediaSession(mySession))
 *     .build();
 * </pre>
 *
 * @see Notification#bigContentView
 */
public static class MediaStyle extends Style {
    static final int MAX_MEDIA_BUTTONS_IN_COMPACT = 3;
    static final int MAX_MEDIA_BUTTONS = 5;

    private int[] mActionsToShowInCompact = null;
    private MediaSession.Token mToken;

    public MediaStyle() {
    }

    public MediaStyle(Builder builder) {
        setBuilder(builder);
    }

    /**
     * Request up to 3 actions (by index in the order of addition) to be shown in the compact
     * notification view.
     *
     * @param actions the indices of the actions to show in the compact notification view
     */
    public MediaStyle setShowActionsInCompactView(int...actions) {
        mActionsToShowInCompact = actions;
        return this;
    }

    /**
     * Attach a {@link android.media.session.MediaSession.Token} to this Notification
     * to provide additional playback information and control to the SystemUI.
     */
    public MediaStyle setMediaSession(MediaSession.Token token) {
        mToken = token;
        return this;
    }

    /**
     * @hide
     */
    @Override
    public Notification buildStyled(Notification wip) {
        super.buildStyled(wip);
        if (wip.category == null) {
            wip.category = Notification.CATEGORY_TRANSPORT;
        }
        return wip;
    }

    /**
     * @hide
     */
    @Override
    public void populateContentView(Notification wip) {
        mBuilder.setBuilderContentView(wip, makeMediaContentView());
    }

    /**
     * @hide
     */
    @Override
    public void populateBigContentView(Notification wip) {
        mBuilder.setBuilderBigContentView(wip, makeMediaBigContentView());
    }

    /** @hide */
    @Override
    public void addExtras(Bundle extras) {
        super.addExtras(extras);

        if (mToken != null) {
            extras.putParcelable(EXTRA_MEDIA_SESSION, mToken);
        }
        if (mActionsToShowInCompact != null) {
            extras.putIntArray(EXTRA_COMPACT_ACTIONS, mActionsToShowInCompact);
        }
    }

    /**
     * @hide
     */
    @Override
    protected void restoreFromExtras(Bundle extras) {
        super.restoreFromExtras(extras);

        if (extras.containsKey(EXTRA_MEDIA_SESSION)) {
            mToken = extras.getParcelable(EXTRA_MEDIA_SESSION);
        }
        if (extras.containsKey(EXTRA_COMPACT_ACTIONS)) {
            mActionsToShowInCompact = extras.getIntArray(EXTRA_COMPACT_ACTIONS);
        }
    }

    private RemoteViews generateMediaActionButton(Action action) {
        final boolean tombstone = (action.actionIntent == null);
        RemoteViews button = new RemoteViews(mBuilder.mContext.getPackageName(),
                R.layout.notification_material_media_action);
        button.setImageViewResource(R.id.action0, action.icon);
        button.setDrawableParameters(R.id.action0, false, -1,
                0xFFFFFFFF,
                PorterDuff.Mode.SRC_ATOP, -1);
        if (!tombstone) {
            button.setOnClickPendingIntent(R.id.action0, action.actionIntent);
        }
        button.setContentDescription(R.id.action0, action.title);
        return button;
    }

    private RemoteViews makeMediaContentView() {
        RemoteViews view = mBuilder.applyStandardTemplate(
                R.layout.notification_template_material_media, false /* hasProgress */);

        final int numActions = mBuilder.mActions.size();
        final int N = mActionsToShowInCompact == null
                ? 0
                : Math.min(mActionsToShowInCompact.length, MAX_MEDIA_BUTTONS_IN_COMPACT);
        if (N > 0) {
            view.removeAllViews(com.android.internal.R.id.media_actions);
            for (int i = 0; i < N; i++) {
                if (i >= numActions) {
                    throw new IllegalArgumentException(String.format(
                            "setShowActionsInCompactView: action %d out of bounds (max %d)",
                            i, numActions - 1));
                }

                final Action action = mBuilder.mActions.get(mActionsToShowInCompact[i]);
                final RemoteViews button = generateMediaActionButton(action);
                view.addView(com.android.internal.R.id.media_actions, button);
            }
        }
        styleText(view);
        hideRightIcon(view);
        return view;
    }

    private RemoteViews makeMediaBigContentView() {
        final int actionCount = Math.min(mBuilder.mActions.size(), MAX_MEDIA_BUTTONS);
        RemoteViews big = mBuilder.applyStandardTemplate(getBigLayoutResource(actionCount),
                false /* hasProgress */);

        if (actionCount > 0) {
            big.removeAllViews(com.android.internal.R.id.media_actions);
            for (int i = 0; i < actionCount; i++) {
                final RemoteViews button = generateMediaActionButton(mBuilder.mActions.get(i));
                big.addView(com.android.internal.R.id.media_actions, button);
            }
        }
        styleText(big);
        hideRightIcon(big);
        applyTopPadding(big);
        big.setViewVisibility(android.R.id.progress, View.GONE);
        return big;
    }

    private int getBigLayoutResource(int actionCount) {
        if (actionCount <= 3) {
            return R.layout.notification_template_material_big_media_narrow;
        } else {
            return R.layout.notification_template_material_big_media;
        }
    }

    private void hideRightIcon(RemoteViews contentView) {
        contentView.setViewVisibility(R.id.right_icon, View.GONE);
    }

    /**
     * Applies the special text colors for media notifications to all text views.
     */
    private void styleText(RemoteViews contentView) {
        int primaryColor = mBuilder.mContext.getResources().getColor(
                R.color.notification_media_primary_color);
        int secondaryColor = mBuilder.mContext.getResources().getColor(
                R.color.notification_media_secondary_color);
        contentView.setTextColor(R.id.title, primaryColor);
        if (mBuilder.showsTimeOrChronometer()) {
            if (mBuilder.mUseChronometer) {
                contentView.setTextColor(R.id.chronometer, secondaryColor);
            } else {
                contentView.setTextColor(R.id.time, secondaryColor);
            }
        }
        contentView.setTextColor(R.id.text2, secondaryColor);
        contentView.setTextColor(R.id.text, secondaryColor);
        contentView.setTextColor(R.id.info, secondaryColor);
    }

    /**
     * @hide
     */
    @Override
    protected boolean hasProgress() {
        return false;
    }
}
