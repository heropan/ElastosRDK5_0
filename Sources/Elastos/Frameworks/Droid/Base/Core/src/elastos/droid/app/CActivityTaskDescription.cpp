

/**
 * Information you can set and retrieve about the current activity within the recent task list.
 */
public static class TaskDescription implements Parcelable {
    /** @hide */
    public static final String ATTR_TASKDESCRIPTION_PREFIX = "task_description_";
    private static final String ATTR_TASKDESCRIPTIONLABEL =
            ATTR_TASKDESCRIPTION_PREFIX + "label";
    private static final String ATTR_TASKDESCRIPTIONCOLOR =
            ATTR_TASKDESCRIPTION_PREFIX + "color";
    private static final String ATTR_TASKDESCRIPTIONICONFILENAME =
            ATTR_TASKDESCRIPTION_PREFIX + "icon_filename";

    private String mLabel;
    private Bitmap mIcon;
    private String mIconFilename;
    private int mColorPrimary;

    /**
     * Creates the TaskDescription to the specified values.
     *
     * @param label A label and description of the current state of this task.
     * @param icon An icon that represents the current state of this task.
     * @param colorPrimary A color to override the theme's primary color.  This color must be opaque.
     */
    public TaskDescription(String label, Bitmap icon, int colorPrimary) {
        if ((colorPrimary != 0) && (Color.alpha(colorPrimary) != 255)) {
            throw new RuntimeException("A TaskDescription's primary color should be opaque");
        }

        mLabel = label;
        mIcon = icon;
        mColorPrimary = colorPrimary;
    }

    /** @hide */
    public TaskDescription(String label, int colorPrimary, String iconFilename) {
        this(label, null, colorPrimary);
        mIconFilename = iconFilename;
    }

    /**
     * Creates the TaskDescription to the specified values.
     *
     * @param label A label and description of the current state of this activity.
     * @param icon An icon that represents the current state of this activity.
     */
    public TaskDescription(String label, Bitmap icon) {
        this(label, icon, 0);
    }

    /**
     * Creates the TaskDescription to the specified values.
     *
     * @param label A label and description of the current state of this activity.
     */
    public TaskDescription(String label) {
        this(label, null, 0);
    }

    /**
     * Creates an empty TaskDescription.
     */
    public TaskDescription() {
        this(null, null, 0);
    }

    /**
     * Creates a copy of another TaskDescription.
     */
    public TaskDescription(TaskDescription td) {
        mLabel = td.mLabel;
        mIcon = td.mIcon;
        mColorPrimary = td.mColorPrimary;
        mIconFilename = td.mIconFilename;
    }

    private TaskDescription(Parcel source) {
        readFromParcel(source);
    }

    /**
     * Sets the label for this task description.
     * @hide
     */
    public void setLabel(String label) {
        mLabel = label;
    }

    /**
     * Sets the primary color for this task description.
     * @hide
     */
    public void setPrimaryColor(int primaryColor) {
        // Ensure that the given color is valid
        if ((primaryColor != 0) && (Color.alpha(primaryColor) != 255)) {
            throw new RuntimeException("A TaskDescription's primary color should be opaque");
        }
        mColorPrimary = primaryColor;
    }

    /**
     * Sets the icon for this task description.
     * @hide
     */
    public void setIcon(Bitmap icon) {
        mIcon = icon;
    }

    /**
     * Moves the icon bitmap reference from an actual Bitmap to a file containing the
     * bitmap.
     * @hide
     */
    public void setIconFilename(String iconFilename) {
        mIconFilename = iconFilename;
        mIcon = null;
    }

    /**
     * @return The label and description of the current state of this task.
     */
    public String getLabel() {
        return mLabel;
    }

    /**
     * @return The icon that represents the current state of this task.
     */
    public Bitmap getIcon() {
        if (mIcon != null) {
            return mIcon;
        }
        return loadTaskDescriptionIcon(mIconFilename);
    }

    /** @hide */
    public String getIconFilename() {
        return mIconFilename;
    }

    /** @hide */
    public Bitmap getInMemoryIcon() {
        return mIcon;
    }

    /** @hide */
    public static Bitmap loadTaskDescriptionIcon(String iconFilename) {
        if (iconFilename != null) {
            try {
                return ActivityManagerNative.getDefault().
                        getTaskDescriptionIcon(iconFilename);
            } catch (RemoteException e) {
            }
        }
        return null;
    }

    /**
     * @return The color override on the theme's primary color.
     */
    public int getPrimaryColor() {
        return mColorPrimary;
    }

    /** @hide */
    public void saveToXml(XmlSerializer out) throws IOException {
        if (mLabel != null) {
            out.attribute(null, ATTR_TASKDESCRIPTIONLABEL, mLabel);
        }
        if (mColorPrimary != 0) {
            out.attribute(null, ATTR_TASKDESCRIPTIONCOLOR, Integer.toHexString(mColorPrimary));
        }
        if (mIconFilename != null) {
            out.attribute(null, ATTR_TASKDESCRIPTIONICONFILENAME, mIconFilename);
        }
    }

    /** @hide */
    public void restoreFromXml(String attrName, String attrValue) {
        if (ATTR_TASKDESCRIPTIONLABEL.equals(attrName)) {
            setLabel(attrValue);
        } else if (ATTR_TASKDESCRIPTIONCOLOR.equals(attrName)) {
            setPrimaryColor((int) Long.parseLong(attrValue, 16));
        } else if (ATTR_TASKDESCRIPTIONICONFILENAME.equals(attrName)) {
            setIconFilename(attrValue);
        }
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        if (mLabel == null) {
            dest.writeInt(0);
        } else {
            dest.writeInt(1);
            dest.writeString(mLabel);
        }
        if (mIcon == null) {
            dest.writeInt(0);
        } else {
            dest.writeInt(1);
            mIcon.writeToParcel(dest, 0);
        }
        dest.writeInt(mColorPrimary);
        if (mIconFilename == null) {
            dest.writeInt(0);
        } else {
            dest.writeInt(1);
            dest.writeString(mIconFilename);
        }
    }

    public void readFromParcel(Parcel source) {
        mLabel = source.readInt() > 0 ? source.readString() : null;
        mIcon = source.readInt() > 0 ? Bitmap.CREATOR.createFromParcel(source) : null;
        mColorPrimary = source.readInt();
        mIconFilename = source.readInt() > 0 ? source.readString() : null;
    }

    public static final Creator<TaskDescription> CREATOR
            = new Creator<TaskDescription>() {
        public TaskDescription createFromParcel(Parcel source) {
            return new TaskDescription(source);
        }
        public TaskDescription[] newArray(int size) {
            return new TaskDescription[size];
        }
    };

    @Override
    public String toString() {
        return "TaskDescription Label: " + mLabel + " Icon: " + mIcon +
                " colorPrimary: " + mColorPrimary;
    }
}
