
#include "ext/frameworkext.h"
#include "content/CComponentName.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CComponentName::constructor()
{
    return NOERROR;
}

ECode CComponentName::constructor(
    /* [in] */ const String& pkg,
    /* [in] */ const String& cls)
{
    if (pkg.IsNull()) {
        //throw new NullPointerException("package name is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (cls.IsNull()) {
        //throw new NullPointerException("class name is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mPackage = pkg;
    mClass = cls;
    return NOERROR;
}

ECode CComponentName::constructor(
    /* [in] */ IContext* pkg,
    /* [in] */ const String& cls)
{
    if (cls.IsNull()) {
        //throw new NullPointerException("class name is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    pkg->GetPackageName(&mPackage);
    mClass = cls;
    return NOERROR;
}

ECode CComponentName::constructor(
    /* [in] */ IContext* pkg,
    /* [in] */ IClassInfo* cls)
{
    pkg->GetPackageName(&mPackage);
    StringBuf* pStringBuf = StringBuf::Alloc(256);
    cls->GetName(pStringBuf);

    //TODO?: In java: cls.getName will get the content(package name + '.' + class name), so...
    mClass = mPackage + String(".") + String(pStringBuf->GetPayload());
    StringBuf::Free(pStringBuf);
    return NOERROR;
}

ECode CComponentName::Clone(
    /* [out] */ IComponentName** newComponentName)
{
    VALIDATE_NOT_NULL(newComponentName)
    return CComponentName::New(mPackage, mClass, newComponentName);
}

ECode CComponentName::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName)
    *packageName = mPackage;
    return NOERROR;
}

ECode CComponentName::GetClassName(
    /* [out] */ String* className)
{
    VALIDATE_NOT_NULL(className)
    *className = mClass;
    return NOERROR;
}

ECode CComponentName::GetShortClassName(
    /* [out] */ String* shortClassName)
{
    VALIDATE_NOT_NULL(shortClassName)
    if (mClass.StartWith(mPackage)) {
        Int32 PN = mPackage.GetLength();
        Int32 CN = mClass.GetLength();
        if (CN > PN && mClass.GetChar(PN) == '.') {
            *shortClassName = mClass.Substring(PN, CN);
            return NOERROR;
        }
    }
    *shortClassName = mClass;
    return NOERROR;
}

ECode CComponentName::FlattenToString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    StringBuilder sb;
    sb.AppendString(mPackage);
    sb.AppendChar('/');
    sb.AppendString(mClass);
    return sb.ToString(name);
}

ECode CComponentName::FlattenToShortString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    StringBuilder sb;
    sb.AppendString(mPackage);
    sb.AppendChar('/');
    String tmp;
    GetShortClassName(&tmp);
    sb.AppendString(tmp);
    return sb.ToString(name);
}

ECode CComponentName::UnflattenFromString(
    /* [in] */ const String& str,
    /* [out] */ IComponentName** componentName)
{
    VALIDATE_NOT_NULL(componentName);
    Int32 sep = str.IndexOf('/');
    if (sep < 0 || (sep + 1) >= str.GetLength()) {
        *componentName = NULL;
        return NOERROR;
    }
    String pkg = str.Substring(0, sep);
    String cls = str.Substring(sep + 1);
    if (!cls.IsNullOrEmpty() && cls.GetChar(0) == '.') {
        cls = pkg + cls;
    }
    return CComponentName::New(pkg, cls, componentName);
}

ECode CComponentName::ToShortString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    StringBuilder sb;
    sb.AppendChar('{');
    sb.AppendString(mPackage);
    sb.AppendChar('/');
    sb.AppendString(mClass);
    sb.AppendChar('}');
    return sb.ToString(name);
}

ECode CComponentName::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder sb;
    sb.AppendString(String("ComponentInfo{"));
    sb.AppendString(mPackage);
    sb.AppendChar('/');
    sb.AppendString(mClass);
    sb.AppendChar('}');
    sb.ToString(str);
    return NOERROR;
}

ECode CComponentName::Equals(
    /* [in] */ IComponentName* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)

    if (NULL != obj) {
        // Note: no null checks, because mPackage and mClass can
        // never be null.
        String pkg;
        String cls;
        obj->GetPackageName(&pkg);
        obj->GetClassName(&cls);
        if (mPackage.Equals(pkg) && mClass.Equals(cls)) {
            *isEqual = TRUE;
            return NOERROR;
        }
    }
    *isEqual = FALSE;
    return NOERROR;
}

ECode CComponentName::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    *hashCode = 0;
    if (!mPackage.IsNull()) {
        *hashCode += mPackage.GetHashCode() * 5;
    }
    if (!mClass.IsNull()) {
        *hashCode += mClass.GetHashCode();
    }
    return NOERROR;
}

ECode CComponentName::CompareTo(
    /* [in] */ IComponentName* that,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(that)
    VALIDATE_NOT_NULL(result)
    Int32 v = 0;
    String pkg;
    that->GetPackageName(&pkg);
    v = mPackage.Compare(pkg);
    if (v != 0) {
        *result = v;
        return NOERROR;
    }
    String cls;
    that->GetClassName(&cls);
    *result = mClass.Compare(cls);
    return NOERROR;
}

ECode CComponentName::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mPackage));
    FAIL_RETURN(source->ReadString(&mClass));
    return NOERROR;
}

ECode CComponentName::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mPackage));
    FAIL_RETURN(dest->WriteString(mClass));
    return NOERROR;
}

ECode CComponentName::WriteToParcel(
    /* [in] */ IComponentName* cn,
    /* [in] */ IParcel* dest)
{
    if (NULL != cn) {
        return IParcelable::Probe(cn)->WriteToParcel(dest);
    }
    else {
        return dest->WriteString(String(NULL));
    }
}

ECode CComponentName::ReadFromParcel(
    /* [in] */ IParcel* source,
    /* [out] */ IComponentName** cn)
{
    VALIDATE_NOT_NULL(cn)
    String pkg, cls;
    source->ReadString(&pkg);
    if (!pkg.IsNull()) {
        source->ReadString(&cls);
        return CComponentName::New(pkg, cls, cn);
    }
    *cn = NULL;
    return NOERROR;
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
