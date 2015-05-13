#ifndef __INTENTMACRO_H__
#define __INTENTMACRO_H__

#define IINTENT_METHODS_DECL()   \
    CARAPI Clone(    \
        /* [out] */ IIntent** intent);    \
   \
    CARAPI CloneFilter(   \
        /* [out] */ IIntent** intent);   \
   \
    CARAPI GetAction(   \
        /* [out] */ String* action);   \
   \
    CARAPI GetData(   \
        /* [out] */ IUri** data);   \
   \
    CARAPI GetDataString(   \
        /* [out] */ String* dataString);   \
   \
    CARAPI GetScheme(   \
        /* [out] */ String* scheme);   \
   \
    CARAPI GetType(   \
        /* [out] */ String* type);   \
   \
    CARAPI ResolveType(   \
        /* [in] */ IContext* context,   \
        /* [out] */ String* type);   \
   \
    CARAPI ResolveTypeEx(   \
        /* [in] */ IContentResolver* resolver,   \
        /* [out] */ String* type);   \
   \
    CARAPI ResolveTypeIfNeeded(   \
        /* [in] */ IContentResolver* resolver,   \
        /* [out] */ String* type);   \
   \
    CARAPI HasCategory(   \
        /* [in] */ const String& category,   \
        /* [out] */ Boolean* hasCategory);   \
   \
    CARAPI GetCategories(   \
        /* [out, callee] */ ArrayOf<String>** categories);   \
   \
    CARAPI GetSelector(   \
        /* [out] */ IIntent** intent);   \
   \
    CARAPI GetClipData(   \
        /* [out] */ IClipData** clipData);   \
   \
    CARAPI SetExtrasClassLoader(   \
        /* [in] */ IClassLoader* loader);   \
   \
    CARAPI HasExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ Boolean* hasExtra);   \
   \
    CARAPI HasFileDescriptors(   \
        /* [out] */ Boolean* hasFileDescriptors);   \
   \
    CARAPI SetAllowFds(   \
        /* [in] */ Boolean allowFds);   \
   \
    CARAPI GetExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IInterface** obj);   \
   \
    CARAPI GetBooleanExtra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Boolean defaultValue,   \
        /* [out] */ Boolean* value);   \
   \
    CARAPI GetByteExtra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Byte defaultValue,   \
        /* [out] */ Byte* value);   \
   \
    CARAPI GetInt16Extra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Int16 defaultValue,   \
        /* [out] */ Int16* value);   \
   \
    CARAPI GetCharExtra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Char32 defaultValue,   \
        /* [out] */ Char32* value);   \
   \
    CARAPI GetInt32Extra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Int32 defaultValue,   \
        /* [out] */ Int32* value);   \
   \
    CARAPI GetInt64Extra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Int64 defaultValue,   \
        /* [out] */ Int64* value);   \
   \
    CARAPI GetFloatExtra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Float defaultValue,   \
        /* [out] */ Float* value);   \
   \
    CARAPI GetDoubleExtra(   \
        /* [in] */ const String& name,   \
        /* [in] */ Double defaultValue,   \
        /* [out] */ Double* value);   \
   \
    CARAPI GetStringExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ String* value);   \
   \
    CARAPI GetCharSequenceExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ ICharSequence** value);   \
   \
    CARAPI GetParcelableExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IParcelable** value);   \
   \
    CARAPI GetObjectStringMapExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IObjectStringMap** value);   \
   \
    CARAPI GetParcelableArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<IParcelable *>** value);   \
   \
    CARAPI GetParcelableArrayListExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IObjectContainer** value);   \
   \
    CARAPI GetIntegerArrayListExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IObjectContainer** value);   \
   \
    CARAPI GetStringArrayListExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IObjectContainer** value);   \
   \
    CARAPI GetCharSequenceArrayListExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IObjectContainer** value);   \
   \
    CARAPI GetBooleanArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Boolean>** value);   \
   \
    CARAPI GetByteArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Byte>** value);   \
   \
    CARAPI GetInt16ArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Int16>** value);   \
   \
    CARAPI GetCharArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Char32>** value);   \
   \
    CARAPI GetInt32ArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Int32>** value);   \
   \
    CARAPI GetInt64ArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Int64>** value);   \
   \
    CARAPI GetFloatArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Float>** value);   \
   \
    CARAPI GetDoubleArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<Double>** value);   \
   \
    CARAPI GetStringArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<String>** value);   \
   \
    CARAPI GetCharSequenceArrayExtra(   \
        /* [in] */ const String& name,   \
        /* [out, callee] */ ArrayOf<ICharSequence *>** value);   \
   \
    CARAPI GetBundleExtra(   \
        /* [in] */ const String& name,   \
        /* [out] */ IBundle** value);   \
   \
    CARAPI GetExtraEx(   \
        /* [in] */ const String& name,   \
        /* [in] */ IInterface* defaultValue,   \
        /* [out] */ IInterface** obj);   \
   \
    CARAPI GetExtras(   \
        /* [out] */ IBundle** extras);   \
   \
    CARAPI GetFlags(   \
        /* [out] */ Int32* flags);   \
   \
    CARAPI IsExcludingStopped(   \
        /* [out] */ Boolean* isStopped);   \
   \
    CARAPI GetPackage(   \
        /* [out] */ String* packageName);   \
   \
    CARAPI GetComponent(   \
        /* [out] */ IComponentName** componentName);   \
   \
    CARAPI GetSourceBounds(   \
        /* [out] */ IRect** sourceBounds);   \
   \
    CARAPI ResolveActivity(   \
        /* [in] */ IPackageManager* pm,   \
        /* [out] */ IComponentName** componentName);   \
    \
    CARAPI ResolveActivityInfo(   \
        /* [in] */ IPackageManager* pm,   \
        /* [in] */ Int32 flags,   \
        /* [out] */ IActivityInfo** result);   \
    \
    CARAPI SetAction(   \
        /* [in] */ const String& action);   \
    \
    CARAPI SetData(   \
        /* [in] */ IUri* data);   \
    \
    CARAPI SetDataAndNormalize(   \
        /* [in] */ IUri* data);   \
    \
    CARAPI SetType(   \
        /* [in] */ const String& type);   \
    \
    CARAPI SetTypeAndNormalize(   \
        /* [in] */ const String& type);   \
    \
    CARAPI SetDataAndType(   \
        /* [in] */ IUri* data,   \
        /* [in] */ const String& type);   \
    \
    CARAPI SetDataAndTypeAndNormalize(    \
        /* [in] */ IUri* data,    \
        /* [in] */ const String& type);    \
    \
    CARAPI AddCategory(    \
        /* [in] */ const String& category);    \
    \
    CARAPI RemoveCategory(    \
        /* [in] */ const String& category);    \
    \
    CARAPI SetSelector(    \
        /* [in] */ IIntent* selector);    \
    \
    CARAPI SetClipData(    \
        /* [in] */ IClipData* clip);    \
    \
    CARAPI PutBooleanExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Boolean value);    \
    \
    CARAPI PutByteExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Byte value);    \
    \
    CARAPI PutCharExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Char32 value);    \
    \
    CARAPI PutInt16Extra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Int16 value);    \
    \
    CARAPI PutInt32Extra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Int32 value);    \
    \
    CARAPI PutInt64Extra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Int64 value);    \
    \
    CARAPI PutFloatExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Float value);    \
    \
    CARAPI PutDoubleExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ Double value);    \
    \
    CARAPI PutStringExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ const String& value);    \
    \
    CARAPI PutCharSequenceExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ICharSequence* value);    \
    \
    CARAPI PutParcelableExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IParcelable* value);    \
    \
    CARAPI PutObjectStringMapExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IObjectStringMap* value);    \
    \
    CARAPI PutParcelableArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<IParcelable*>* value);    \
    \
    CARAPI PutParcelableArrayListExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IObjectContainer* value);    \
    \
    CARAPI PutIntegerArrayListExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IObjectContainer* value);    \
    \
    CARAPI PutStringArrayListExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IObjectContainer* value);    \
    \
    CARAPI PutCharSequenceArrayListExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IObjectContainer* value);    \
    \
    CARAPI PutBooleanArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Boolean>* value);    \
    \
    CARAPI PutByteArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Byte>* value);    \
    \
    CARAPI PutInt16ArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Int16>* value);    \
    \
    CARAPI PutCharArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Char32>* value);    \
    \
    CARAPI PutInt32ArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Int32>* value);    \
    \
    CARAPI PutInt64ArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Int64>* value);    \
    \
    CARAPI PutFloatArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Float>* value);    \
    \
    CARAPI PutDoubleArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<Double>* value);    \
    \
    CARAPI PutStringArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<String>* value);    \
    \
    CARAPI PutCharSequenceArrayExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ ArrayOf<ICharSequence *>* value);    \
    \
    CARAPI PutBundleExtra(    \
        /* [in] */ const String& name,    \
        /* [in] */ IBundle* value);    \
    \
    CARAPI PutExtras(    \
        /* [in] */ IIntent* src);    \
    \
    CARAPI PutExtrasEx(    \
        /* [in] */ IBundle* extras);    \
    \
    CARAPI ReplaceExtras(    \
        /* [in] */ IIntent* src);    \
    \
    CARAPI ReplaceExtrasEx(    \
        /* [in] */ IBundle* extras);    \
    \
    CARAPI RemoveExtra(    \
        /* [in] */ const String& name);    \
    \
    CARAPI SetFlags(    \
        /* [in] */ Int32 flags);    \
    \
    CARAPI AddFlags(    \
        /* [in] */ Int32 flags);    \
    \
    CARAPI SetPackage(    \
        /* [in] */ const String& packageName);    \
    \
    CARAPI SetComponent(    \
        /* [in] */ IComponentName* component);    \
    \
    CARAPI SetClassName(    \
        /* [in] */ IContext* packageContext,    \
        /* [in] */ const String& className);    \
    \
    CARAPI SetClassNameEx(    \
        /* [in] */ const String& packageName,    \
        /* [in] */ const String& className);    \
    \
    CARAPI SetClass(    \
        /* [in] */ IContext* packageContext,    \
        /* [in] */ const ClassID& clsId);    \
    \
    CARAPI SetSourceBounds(    \
        /* [in] */ IRect* r);    \
    \
    CARAPI FillIn(    \
        /* [in] */ IIntent* other,    \
        /* [in] */ Int32 flags,    \
        /* [out] */ Int32* result);    \
    \
    CARAPI FilterEquals(    \
        /* [in] */ IIntent* other,    \
        /* [out] */ Boolean* isEqual);    \
    \
    CARAPI FilterHashCode(    \
        /* [out] */ Int32* result);    \
    \
    CARAPI ToString(    \
        /* [out] */ String* str);    \
    \
    CARAPI ToInsecureString(    \
        /* [out] */ String* str);    \
    \
    CARAPI ToInsecureStringWithClip(    \
        /* [out] */ String* str);    \
    \
    CARAPI ToShortString(    \
        /* [in] */ Boolean secure,    \
        /* [in] */ Boolean comp,    \
        /* [in] */ Boolean extras,    \
        /* [in] */ Boolean clip,    \
        /* [out] */ String* str);    \
    \
    CARAPI ToShortStringEx(    \
        /* [in] */ IStringBuilder* sb,    \
        /* [in] */ Boolean secure,    \
        /* [in] */ Boolean comp,    \
        /* [in] */ Boolean extras,    \
        /* [in] */ Boolean clip);    \
    \
    CARAPI ToURI(    \
        /* [out] */ String* str);    \
    \
    CARAPI ToUri(    \
        /* [in] */ Int32 flags,    \
        /* [out] */ String* str);    \
    \
    CARAPI MigrateExtraStreamToClipData(    \
        /* [out] */ Boolean* result);

#define IINTENT_METHODS_IMPL(className, superClass)    \
ECode className::Clone(    \
    /* [out] */ IIntent** intent)    \
{    \
    return superClass::Clone(intent);    \
}    \
    \
ECode className::CloneFilter(    \
    /* [out] */ IIntent** intent)    \
{    \
    VALIDATE_NOT_NULL(intent);    \
    return superClass::CloneFilter(intent);    \
}    \
    \
ECode className::GetAction(    \
    /* [out] */ String* action)    \
{    \
    VALIDATE_NOT_NULL(action);    \
    return superClass::GetAction(action);    \
}    \
    \
ECode className::GetData(    \
    /* [out] */ IUri** data)    \
{    \
    VALIDATE_NOT_NULL(data);    \
    return superClass::GetData(data);    \
}    \
    \
ECode className::GetDataString(    \
    /* [out] */ String* dataString)    \
{    \
    VALIDATE_NOT_NULL(dataString);    \
    return superClass::GetDataString(dataString);    \
}    \
    \
ECode className::GetScheme(    \
    /* [out] */ String* scheme)    \
{    \
    VALIDATE_NOT_NULL(scheme);    \
    return superClass::GetScheme(scheme);    \
}    \
    \
ECode className::GetType(    \
    /* [out] */ String* type)    \
{    \
    VALIDATE_NOT_NULL(type);    \
    return superClass::GetType(type);    \
}    \
    \
ECode className::ResolveType(    \
    /* [in] */ IContext* context,    \
    /* [out] */ String* type)    \
{    \
    VALIDATE_NOT_NULL(type);    \
    return superClass::ResolveType(context, type);    \
}    \
    \
ECode className::ResolveTypeEx(    \
    /* [in] */ IContentResolver* resolver,    \
    /* [out] */ String* type)    \
{    \
    VALIDATE_NOT_NULL(type);    \
    return superClass::ResolveTypeEx(resolver, type);    \
}    \
    \
ECode className::ResolveTypeIfNeeded(    \
    /* [in] */ IContentResolver* resolver,    \
    /* [out] */ String* type)    \
{    \
    VALIDATE_NOT_NULL(type);    \
    return superClass::ResolveTypeIfNeeded(resolver, type);    \
}    \
    \
ECode className::HasCategory(    \
    /* [in] */ const String& category,    \
    /* [out] */ Boolean* hasCategory)    \
{    \
    VALIDATE_NOT_NULL(hasCategory);    \
    return superClass::HasCategory(category, hasCategory);    \
}    \
    \
ECode className::GetCategories(    \
    /* [out, callee] */ ArrayOf<String>** categories)    \
{    \
    VALIDATE_NOT_NULL(categories);    \
    return superClass::GetCategories(categories);    \
}    \
    \
ECode className::GetSelector(    \
    /* [out] */ IIntent** intent)    \
{    \
    VALIDATE_NOT_NULL(intent);    \
    return superClass::GetSelector(intent);    \
}    \
    \
ECode className::GetClipData(    \
    /* [out] */ IClipData** clipData)    \
{    \
    VALIDATE_NOT_NULL(clipData);    \
    return superClass::GetClipData(clipData);    \
}    \
    \
ECode className::SetExtrasClassLoader(    \
    /* [in] */ IClassLoader* loader)    \
{    \
    return superClass::SetExtrasClassLoader(loader);    \
}    \
    \
ECode className::HasExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ Boolean* hasExtra)    \
{    \
    VALIDATE_NOT_NULL(hasExtra);    \
    return superClass::HasExtra(name, hasExtra);    \
}    \
    \
ECode className::HasFileDescriptors(    \
    /* [out] */ Boolean* hasFileDescriptors)    \
{    \
    VALIDATE_NOT_NULL(hasFileDescriptors);    \
    return superClass::HasFileDescriptors(hasFileDescriptors);    \
}    \
    \
ECode className::SetAllowFds(    \
    /* [in] */ Boolean allowFds)    \
{    \
    return superClass::SetAllowFds(allowFds);    \
}    \
    \
ECode className::GetExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IInterface** obj)    \
{    \
    VALIDATE_NOT_NULL(obj);    \
    return superClass::GetExtra(name, obj);    \
}    \
    \
ECode className::GetBooleanExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Boolean defaultValue,    \
    /* [out] */ Boolean* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetBooleanExtra(name, defaultValue, value);    \
}    \
    \
ECode className::GetByteExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Byte defaultValue,    \
    /* [out] */ Byte* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetByteExtra(name, defaultValue, value);    \
}    \
    \
ECode className::GetInt16Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int16 defaultValue,    \
    /* [out] */ Int16* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt16Extra(name, defaultValue, value);    \
}    \
    \
ECode className::GetCharExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Char32 defaultValue,    \
    /* [out] */ Char32* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetCharExtra(name, defaultValue, value);    \
}    \
    \
ECode className::GetInt32Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int32 defaultValue,    \
    /* [out] */ Int32* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt32Extra(name, defaultValue, value);    \
}    \
    \
ECode className::GetInt64Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int64 defaultValue,    \
    /* [out] */ Int64* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt64Extra(name, defaultValue, value);    \
}    \
    \
ECode className::GetFloatExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Float defaultValue,    \
    /* [out] */ Float* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetFloatExtra(name, defaultValue, value);    \
}    \
    \
ECode className::GetDoubleExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Double defaultValue,    \
    /* [out] */ Double* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetDoubleExtra(name, defaultValue, value);    \
}    \
    \
ECode className::GetStringExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ String* value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetStringExtra(name, value);    \
}    \
    \
ECode className::GetCharSequenceExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ ICharSequence** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetCharSequenceExtra(name, value);    \
}    \
    \
ECode className::GetParcelableExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IParcelable** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetParcelableExtra(name, value);    \
}    \
    \
ECode className::GetObjectStringMapExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IObjectStringMap** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetObjectStringMapExtra(name, value);    \
}    \
    \
ECode className::GetParcelableArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<IParcelable *>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetParcelableArrayExtra(name, value);    \
}    \
    \
ECode className::GetParcelableArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IObjectContainer** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetParcelableArrayListExtra(name, value);    \
}    \
    \
ECode className::GetIntegerArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IObjectContainer** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetIntegerArrayListExtra(name, value);    \
}    \
    \
ECode className::GetStringArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IObjectContainer** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetStringArrayListExtra(name, value);    \
}    \
    \
ECode className::GetCharSequenceArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IObjectContainer** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetCharSequenceArrayListExtra(name, value);    \
}    \
    \
ECode className::GetBooleanArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Boolean>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetBooleanArrayExtra(name, value);    \
}    \
    \
ECode className::GetByteArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Byte>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetByteArrayExtra(name, value);    \
}    \
    \
ECode className::GetInt16ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Int16>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt16ArrayExtra(name, value);    \
}    \
    \
ECode className::GetCharArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Char32>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetCharArrayExtra(name, value);    \
}    \
    \
ECode className::GetInt32ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Int32>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt32ArrayExtra(name, value);    \
}    \
    \
ECode className::GetInt64ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Int64>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetInt64ArrayExtra(name, value);    \
}    \
    \
ECode className::GetFloatArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Float>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetFloatArrayExtra(name, value);    \
}    \
    \
ECode className::GetDoubleArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<Double>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetDoubleArrayExtra(name, value);    \
}    \
    \
ECode className::GetStringArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<String>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetStringArrayExtra(name, value);    \
}    \
    \
ECode className::GetCharSequenceArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [out, callee] */ ArrayOf<ICharSequence *>** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetCharSequenceArrayExtra(name, value);    \
}    \
    \
ECode className::GetBundleExtra(    \
    /* [in] */ const String& name,    \
    /* [out] */ IBundle** value)    \
{    \
    VALIDATE_NOT_NULL(value);    \
    return superClass::GetBundleExtra(name, value);    \
}    \
    \
ECode className::GetExtraEx(    \
    /* [in] */ const String& name,    \
    /* [in] */ IInterface* defaultValue,    \
    /* [out] */ IInterface** obj)    \
{    \
    VALIDATE_NOT_NULL(obj);    \
    return superClass::GetExtraEx(name, defaultValue, obj);    \
}    \
    \
ECode className::GetExtras(    \
    /* [out] */ IBundle** extras)    \
{    \
    VALIDATE_NOT_NULL(extras);    \
    return superClass::GetExtras(extras);    \
}    \
    \
ECode className::GetFlags(    \
    /* [out] */ Int32* flags)    \
{    \
    VALIDATE_NOT_NULL(flags);    \
    return superClass::GetFlags(flags);    \
}    \
    \
ECode className::IsExcludingStopped(    \
    /* [out] */ Boolean* isStopped)    \
{    \
    VALIDATE_NOT_NULL(isStopped);    \
    return superClass::IsExcludingStopped(isStopped);    \
}    \
    \
ECode className::GetPackage(    \
    /* [out] */ String* packageName)    \
{    \
    VALIDATE_NOT_NULL(packageName);    \
    return superClass::GetPackage(packageName);    \
}    \
    \
ECode className::GetComponent(    \
    /* [out] */ IComponentName** componentName)    \
{    \
    VALIDATE_NOT_NULL(componentName);    \
    return superClass::GetComponent(componentName);    \
}    \
    \
ECode className::GetSourceBounds(    \
    /* [out] */ IRect** sourceBounds)    \
{    \
    VALIDATE_NOT_NULL(sourceBounds);    \
    return superClass::GetSourceBounds(sourceBounds);    \
}    \
    \
ECode className::ResolveActivity(    \
    /* [in] */ IPackageManager* pm,    \
    /* [out] */ IComponentName** componentName)    \
{    \
    VALIDATE_NOT_NULL(componentName);    \
    return superClass::ResolveActivity(pm, componentName);    \
}    \
    \
ECode className::ResolveActivityInfo(    \
    /* [in] */ IPackageManager* pm,    \
    /* [in] */ Int32 flags,    \
    /* [out] */ IActivityInfo** result)    \
{    \
    VALIDATE_NOT_NULL(result);    \
    return superClass::ResolveActivityInfo(pm, flags, result);    \
}    \
    \
ECode className::SetAction(    \
    /* [in] */ const String& action)    \
{    \
    return superClass::SetAction(action);    \
}    \
    \
ECode className::SetData(    \
    /* [in] */ IUri* data)    \
{    \
    return superClass::SetData(data);    \
}    \
    \
ECode className::SetDataAndNormalize(    \
    /* [in] */ IUri* data)    \
{    \
    return superClass::SetDataAndNormalize(data);    \
}    \
    \
ECode className::SetType(    \
    /* [in] */ const String& type)    \
{    \
    return superClass::SetType(type);    \
}    \
    \
ECode className::SetTypeAndNormalize(    \
    /* [in] */ const String& type)    \
{    \
    return superClass::SetTypeAndNormalize(type);    \
}    \
    \
ECode className::SetDataAndType(    \
    /* [in] */ IUri* data,    \
    /* [in] */ const String& type)    \
{    \
    return superClass::SetDataAndType(data, type);    \
}    \
    \
ECode className::SetDataAndTypeAndNormalize(    \
    /* [in] */ IUri* data,    \
    /* [in] */ const String& type)    \
{    \
    return superClass::SetDataAndTypeAndNormalize(data, type);    \
}    \
    \
ECode className::AddCategory(    \
    /* [in] */ const String& category)    \
{    \
    return superClass::AddCategory(category);    \
}    \
    \
ECode className::RemoveCategory(    \
    /* [in] */ const String& category)    \
{    \
    return superClass::RemoveCategory(category);    \
}    \
    \
ECode className::SetSelector(    \
    /* [in] */ IIntent* selector)    \
{    \
    return superClass::SetSelector(selector);    \
}    \
    \
ECode className::SetClipData(    \
    /* [in] */ IClipData* clip)    \
{    \
    return superClass::SetClipData(clip);    \
}    \
    \
ECode className::PutBooleanExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Boolean value)    \
{    \
    return superClass::PutBooleanExtra(name, value);    \
}    \
    \
ECode className::PutByteExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Byte value)    \
{    \
    return superClass::PutByteExtra(name, value);    \
}    \
    \
ECode className::PutCharExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Char32 value)    \
{    \
    return superClass::PutCharExtra(name, value);    \
}    \
    \
ECode className::PutInt16Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int16 value)    \
{    \
    return superClass::PutInt16Extra(name, value);    \
}    \
    \
ECode className::PutInt32Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int32 value)    \
{    \
    return superClass::PutInt32Extra(name, value);    \
}    \
    \
ECode className::PutInt64Extra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Int64 value)    \
{    \
    return superClass::PutInt64Extra(name, value);    \
}    \
    \
ECode className::PutFloatExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Float value)    \
{    \
    return superClass::PutFloatExtra(name, value);    \
}    \
    \
ECode className::PutDoubleExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ Double value)    \
{    \
    return superClass::PutDoubleExtra(name, value);    \
}    \
    \
ECode className::PutStringExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ const String& value)    \
{    \
    return superClass::PutStringExtra(name, value);    \
}    \
    \
ECode className::PutCharSequenceExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ICharSequence* value)    \
{    \
    return superClass::PutCharSequenceExtra(name, value);    \
}    \
    \
ECode className::PutParcelableExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IParcelable* value)    \
{    \
    return superClass::PutParcelableExtra(name, value);    \
}    \
    \
ECode className::PutObjectStringMapExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IObjectStringMap* value)    \
{    \
    return superClass::PutObjectStringMapExtra(name, value);    \
}    \
    \
ECode className::PutParcelableArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<IParcelable *>* value)    \
{    \
    return superClass::PutParcelableArrayExtra(name, value);    \
}    \
    \
ECode className::PutParcelableArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IObjectContainer* value)    \
{    \
    return superClass::PutParcelableArrayListExtra(name, value);    \
}    \
    \
ECode className::PutIntegerArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IObjectContainer* value)    \
{    \
    return superClass::PutIntegerArrayListExtra(name, value);    \
}    \
    \
ECode className::PutStringArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IObjectContainer* value)    \
{    \
    return superClass::PutStringArrayListExtra(name, value);    \
}    \
    \
ECode className::PutCharSequenceArrayListExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IObjectContainer* value)    \
{    \
    return superClass::PutCharSequenceArrayListExtra(name, value);    \
}    \
    \
ECode className::PutBooleanArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Boolean>* value)    \
{    \
    return superClass::PutBooleanArrayExtra(name, value);    \
}    \
    \
ECode className::PutByteArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Byte>* value)    \
{    \
    return superClass::PutByteArrayExtra(name, value);    \
}    \
    \
ECode className::PutInt16ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Int16>* value)    \
{    \
    return superClass::PutInt16ArrayExtra(name, value);    \
}    \
    \
ECode className::PutCharArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Char32>* value)    \
{    \
    return superClass::PutCharArrayExtra(name, value);    \
}    \
    \
ECode className::PutInt32ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Int32>* value)    \
{    \
    return superClass::PutInt32ArrayExtra(name, value);    \
}    \
    \
ECode className::PutInt64ArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Int64>* value)    \
{    \
    return superClass::PutInt64ArrayExtra(name, value);    \
}    \
    \
ECode className::PutFloatArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Float>* value)    \
{    \
    return superClass::PutFloatArrayExtra(name, value);    \
}    \
    \
ECode className::PutDoubleArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<Double>* value)    \
{    \
    return superClass::PutDoubleArrayExtra(name, value);    \
}    \
    \
ECode className::PutStringArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<String>* value)    \
{    \
    return superClass::PutStringArrayExtra(name, value);    \
}    \
    \
ECode className::PutCharSequenceArrayExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ ArrayOf<ICharSequence *>* value)    \
{    \
    return superClass::PutCharSequenceArrayExtra(name, value);    \
}    \
    \
ECode className::PutBundleExtra(    \
    /* [in] */ const String& name,    \
    /* [in] */ IBundle* value)    \
{    \
    return superClass::PutBundleExtra(name, value);    \
}    \
    \
ECode className::PutExtras(    \
    /* [in] */ IIntent* src)    \
{    \
    return superClass::PutExtras(src);    \
}    \
    \
ECode className::PutExtrasEx(    \
    /* [in] */ IBundle* extras)    \
{    \
    return superClass::PutExtrasEx(extras);    \
}    \
    \
ECode className::ReplaceExtras(    \
    /* [in] */ IIntent* src)    \
{    \
    return superClass::ReplaceExtras(src);    \
}    \
    \
ECode className::ReplaceExtrasEx(    \
    /* [in] */ IBundle* extras)    \
{    \
    return superClass::ReplaceExtrasEx(extras);    \
}    \
    \
ECode className::RemoveExtra(    \
    /* [in] */ const String& name)    \
{    \
    return superClass::RemoveExtra(name);    \
}    \
    \
ECode className::SetFlags(    \
    /* [in] */ Int32 flags)    \
{    \
    return superClass::SetFlags(flags);    \
}    \
    \
ECode className::AddFlags(    \
    /* [in] */ Int32 flags)    \
{    \
    return superClass::AddFlags(flags);    \
}    \
    \
ECode className::SetPackage(    \
    /* [in] */ const String& packageName)    \
{    \
    return superClass::SetPackage(packageName);    \
}    \
    \
ECode className::SetComponent(    \
    /* [in] */ IComponentName* component)    \
{    \
    return superClass::SetComponent(component);    \
}    \
    \
ECode className::SetClassName(    \
    /* [in] */ IContext* packageContext,    \
    /* [in] */ const String& className)    \
{    \
    return superClass::SetClassName(packageContext, className);    \
}    \
    \
ECode className::SetClassNameEx(    \
    /* [in] */ const String& packageName,    \
    /* [in] */ const String& className)    \
{    \
    return superClass::SetClassNameEx(packageName, className);    \
}    \
    \
ECode className::SetClass(    \
    /* [in] */ IContext* packageContext,    \
    /* [in] */ const ClassID& clsId)    \
{    \
    return superClass::SetClass(packageContext, clsId);    \
}    \
    \
ECode className::SetSourceBounds(    \
    /* [in] */ IRect* r)    \
{    \
    return superClass::SetSourceBounds(r);    \
}    \
    \
ECode className::FillIn(    \
    /* [in] */ IIntent* other,    \
    /* [in] */ Int32 flags,    \
    /* [out] */ Int32* result)    \
{    \
    VALIDATE_NOT_NULL(result);    \
    return superClass::FillIn(other, flags, result);    \
}    \
    \
ECode className::FilterEquals(    \
    /* [in] */ IIntent* other,    \
    /* [out] */ Boolean* isEqual)    \
{    \
    VALIDATE_NOT_NULL(isEqual);    \
    return superClass::FilterEquals(other, isEqual);    \
}    \
    \
ECode className::FilterHashCode(    \
    /* [out] */ Int32* result)    \
{    \
    VALIDATE_NOT_NULL(result);    \
    return superClass::FilterHashCode(result);    \
}    \
    \
ECode className::ToString(    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToString(str);    \
}    \
    \
ECode className::ToInsecureString(    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToInsecureString(str);    \
}    \
    \
ECode className::ToInsecureStringWithClip(    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToInsecureStringWithClip(str);    \
}    \
    \
ECode className::ToShortString(    \
    /* [in] */ Boolean secure,    \
    /* [in] */ Boolean comp,    \
    /* [in] */ Boolean extras,    \
    /* [in] */ Boolean clip,    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToShortString(secure, comp, extras, clip, str);    \
}    \
    \
ECode className::ToShortStringEx(    \
    /* [in] */ IStringBuilder* sb,    \
    /* [in] */ Boolean secure,    \
    /* [in] */ Boolean comp,    \
    /* [in] */ Boolean extras,    \
    /* [in] */ Boolean clip)    \
{    \
    return superClass::ToShortStringEx(sb, secure, comp, extras, clip);    \
}    \
    \
ECode className::ToURI(    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToURI(str);    \
}    \
    \
ECode className::ToUri(    \
    /* [in] */ Int32 flags,    \
    /* [out] */ String* str)    \
{    \
    VALIDATE_NOT_NULL(str);    \
    return superClass::ToUri(flags, str);    \
}    \
    \
ECode className::MigrateExtraStreamToClipData(    \
    /* [out] */ Boolean* result)    \
{    \
    VALIDATE_NOT_NULL(result);    \
    return superClass::MigrateExtraStreamToClipData(result);    \
}

#endif //__INTENTMACRO_H__
