
/**
 * Constructor.
 * @hide
 */
ECode CSpellCheckerInfo::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service)
{
    mService = service;

    CResolveInfo* info = (CResolveInfo*)(service.Get());
    AutoPtr<IServiceInfo> si = info->serviceInfo;
    //mId = new ComponentName(si.packageName, si.name).flattenToShortString();
    CServiceInfo* csi = (CServiceInfo*)(si.Get());
    AutoPtr<IComponentName> componentName;
    CComponentName::New(csi->packageName, si->name, (IComponentName**)&componentName);
    componentName->FlattenToShortString(&mId);

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    Int32 label = 0;
    String settingsActivityComponent;

    AutoPtr<IXmlResourceParser> parser;
    //try {
        si->LoadXmlMetaData(pm, ISpellCheckerSession::SERVICE_META_DATA, (IXmlResourceParser**)&parser);
        //if (parser == NULL) {
        //    throw new XmlPullParserException("No "
        //            + SpellCheckerSession.SERVICE_META_DATA + " meta-data");
        //}
        assert(parser != NULL);

        AutoPtr<IResources> res;
        pm->GetResourcesForApplication(si->applicationInfo, (IResources**)&res);
        AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
        Int32 type;
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                && type != IXmlPullParser::START_TAG) {
        }

        String nodeName;
        parser->GetName(&nodeName);
        //if (!nodeName.Equals("spell-checker")) {
        //    throw new XmlPullParserException(
        //            "Meta-data does not start with spell-checker tag");
        //}
        assert(nodeName.Equals("spell-checker"));

/*        AutoPtr<ITypedArray> sa;
        res->ObtainAttributes(attrs,
                com.android.internal.R.styleable.SpellChecker, (ITypedArray**)&sa);
        sa->GetResourceId(com.android.internal.R.styleable.SpellChecker_label, 0, &label);
        sa->GetString(
                com.android.internal.R.styleable.SpellChecker_settingsActivity, &settingsActivityComponent);
        sa->Recycle();

        Int32 depth;
        parser->GetDepth(&depth);
        // Parse all subtypes
        while (((type = parser.next()) != XmlPullParser.END_TAG || parser.getDepth() > depth)
                && type != XmlPullParser.END_DOCUMENT) {
            if (type == XmlPullParser.START_TAG) {
                final String subtypeNodeName = parser.getName();
                if (!"subtype".equals(subtypeNodeName)) {
                    throw new XmlPullParserException(
                            "Meta-data in spell-checker does not start with subtype tag");
                }
                final TypedArray a = res.obtainAttributes(
                        attrs, com.android.internal.R.styleable.SpellChecker_Subtype);
                SpellCheckerSubtype subtype = new SpellCheckerSubtype(
                        a.getResourceId(com.android.internal.R.styleable
                                .SpellChecker_Subtype_label, 0),
                        a.getString(com.android.internal.R.styleable
                                .SpellChecker_Subtype_subtypeLocale),
                        a.getString(com.android.internal.R.styleable
                                .SpellChecker_Subtype_subtypeExtraValue));
                mSubtypes.add(subtype);
            }
        }*/
    //} catch (Exception e) {
    //    Slog.e(TAG, "Caught exception: " + e);
    //    throw new XmlPullParserException(
    //            "Unable to create context for: " + si.packageName);
    //} finally {
        if (parser != NULL) parser->Close();
    //}
    mLabel = label;
    mSettingsActivityName = settingsActivityComponent;

    return NOERROR;
}

/**
 * Constructor.
 * @hide
 */
ECode CSpellCheckerInfo::constructor(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mLabel);
    source->ReadString(&mId);
    source->ReadString(&mSettingsActivityName);
//    mService = ResolveInfo.CREATOR.createFromParcel(source);
//    source.readTypedList(mSubtypes, SpellCheckerSubtype.CREATOR);

    return NOERROR;
}

/**
 * Return a unique ID for this spell checker.  The ID is generated from
 * the package and class name implementing the method.
 */
ECode CSpellCheckerInfo::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

/**
 * Return the component of the service that implements.
 */
ECode CSpellCheckerInfo::GetComponent(
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(component);
    AutoPtr<IServiceInfo> serviceInfo;
    String packageName, name;
    serviceInfo = ((CResolveInfo*)(mService.Get()))->serviceInfo;
    packageName = ((CServiceInfo*)(serviceInfo.Get()))->packageName;
    name = ((CServiceInfo*)(serviceInfo.Get()))->name;
    CComponentName::New(packageName, name, component);

    return NOERROR;
}

/**
 * Return the .apk package that implements this.
 */
ECode CSpellCheckerInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ((CServiceInfo*)(serviceInfo.Get()))->packageName;
    return NOERROR;
}

ECode CSpellCheckerInfo::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
//    return new SpellCheckerInfo(source);
}

/**
 * Used to package this object into a {@link Parcel}.
 *
 * @param dest The {@link Parcel} to be written.
 * @param flags The flags used for parceling.
 */
//@Override
ECode CSpellCheckerInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mLabel);
    dest->WriteString(mId);
    dest->WriteString(mSettingsActivityName);
    mService->WriteToParcel(dest, flags);
//    dest.writeTypedList(mSubtypes);

    return NOERROR;
}

/**
 * Load the user-displayed label for this spell checker.
 *
 * @param pm Supply a PackageManager used to load the spell checker's resources.
 */
ECode CSpellCheckerInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** lable)
{
    VALIDATE_NOT_NULL(lable);
    if (mLabel == 0 || pm == NULL)
        return CStringWrapper::New(String(""), lable);

    String packageName, applicationInfo;
    AutoPtr<IServiceInfo> serviceInfo;

    applicationInfo = ((CServiceInfo*)(serviceInfo.Get()))->applicationInfo;
    GetPackageName(&packageName);

    return pm->GetText(packageName, mLabel, applicationInfo);
}

/**
 * Load the user-displayed icon for this spell checker.
 *
 * @param pm Supply a PackageManager used to load the spell checker's resources.
 */
ECode CSpellCheckerInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** drawable)
{
    return mService->LoadIcon(pm, drawable);
}

/**
 * Return the raw information about the Service implementing this
 * spell checker.  Do not modify the returned object.
 */
ECode CSpellCheckerInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = ((CResolveInfo*)(mService.Get()))->serviceInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

/**
 * Return the class name of an activity that provides a settings UI.
 * You can launch this activity be starting it with
 * an {@link android.content.Intent} whose action is MAIN and with an
 * explicit {@link android.content.ComponentName}
 * composed of {@link #getPackageName} and the class name returned here.
 *
 * <p>A null will be returned if there is no settings activity.
 */
ECode CSpellCheckerInfo::GetSettingsActivity(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mSettingsActivityName;
    return NOERROR;
}

/**
 * Return the count of the subtypes.
 */
ECode CSpellCheckerInfo::GetSubtypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mSubtypes.GetSize();
    return NOERROR;
}

/**
 * Return the subtype at the specified index.
 *
 * @param index the index of the subtype to return.
 */
ECode CSpellCheckerInfo::GetSubtypeAt(
    /* [in] */ Int32 index,
    /* [out] */ ISpellCheckerSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);
    *subtype = mSubtypes[index];
    REFCOUNT_ADD(*subtype);
    return NOERROR;
}

/**
 * Used to make this class parcelable.
 */
//@Override
ECode CSpellCheckerInfo::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;
    return NOERROR;
}
