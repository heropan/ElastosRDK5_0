
#ifndef __SPELLCHECKERINFO_H__
#define __SPELLCHECKERINFO_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * This class is used to specify meta information of a spell checker.
 */
CarClass(CSpellCheckerInfo), public SpellCheckerInfo
{
public:

    /**
     * Constructor.
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IResolveInfo* service);

    /**
     * Constructor.
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IParcel* source);

    /**
     * Return a unique ID for this spell checker.  The ID is generated from
     * the package and class name implementing the method.
     */
    CARAPI GetId(
        /* [out] */ String* id);

    /**
     * Return the component of the service that implements.
     */
    CARAPI GetComponent(
        /* [out] */ IComponentName** name);

    /**
     * Return the .apk package that implements this.
     */
    CARAPI GetPackageName(
        /* [out] */ String* name);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Used to package this object into a {@link Parcel}.
     *
     * @param dest The {@link Parcel} to be written.
     * @param flags The flags used for parceling.
     */
    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Load the user-displayed label for this spell checker.
     *
     * @param pm Supply a PackageManager used to load the spell checker's resources.
     */
    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** lable);

    /**
     * Load the user-displayed icon for this spell checker.
     *
     * @param pm Supply a PackageManager used to load the spell checker's resources.
     */
    CARAPI LoadIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** drawable);

    /**
     * Return the raw information about the Service implementing this
     * spell checker.  Do not modify the returned object.
     */
    CARAPI GetServiceInfo(
        /* [out] */ IServiceInfo** info);

    /**
     * Return the class name of an activity that provides a settings UI.
     * You can launch this activity be starting it with
     * an {@link android.content.Intent} whose action is MAIN and with an
     * explicit {@link android.content.ComponentName}
     * composed of {@link #getPackageName} and the class name returned here.
     *
     * <p>A null will be returned if there is no settings activity.
     */
    CARAPI GetSettingsActivity(
        /* [out] */ String* name);

    /**
     * Return the count of the subtypes.
     */
    CARAPI GetSubtypeCount(
        /* [out] */ Int32* count);

    /**
     * Return the subtype at the specified index.
     *
     * @param index the index of the subtype to return.
     */
    CARAPI GetSubtypeAt(
        /* [in] */ Int32 index,
        /* [out] */ ISpellCheckerSubtype** subtype);

    /**
     * Used to make this class parcelable.
     */
    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

private:
    static const CString TAG;// = SpellCheckerInfo.class.getSimpleName();
    AutoPtr<IResolveInfo> mService;
    String mId;
    Int32 mLabel;

    /**
     * The spell checker setting activity's name, used by the system settings to
     * launch the setting activity.
     */
    String mSettingsActivityName;

    /**
     * The array of subtypes.
     */
    //ArrayList<SpellCheckerSubtype> mSubtypes = new ArrayList<SpellCheckerSubtype>();
    List<AutoPtr<ISpellCheckerSubtype> > mSubtypes;
};

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__SPELLCHECKERINFO_H__