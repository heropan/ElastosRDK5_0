#include "elastos/droid/accessibilityservice/CAccessibilityServiceInfo.h"
#include "elastos/droid/content/CComponentName.h"
// #include "elastos/droid/view/accessibility/CAccessibilityEventHelper.h"
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/R.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Content::Pm::IPackageItemInfo;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::R;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
// using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
// using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::ICharSequence;
using Elastos::Core::Math;
using Elastos::Core::StringUtils;
using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::Etl::HashMap;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const String CAccessibilityServiceInfo::TAG_ACCESSIBILITY_SERVICE("accessibility-service");

CAR_INTERFACE_IMPL_2(CAccessibilityServiceInfo, Object, IAccessibilityServiceInfo, IParcelable)

CAR_OBJECT_IMPL(CAccessibilityServiceInfo)

// static HashMap<Int32, AutoPtr<CapabilityInfo> > InitsAvailableCapabilityInfos()
// {
//     HashMap<Int32, AutoPtr<CapabilityInfo> > availableCapabilityInfos = new HashMap<Int32, AutoPtr<CapabilityInfo> >();
//     AutoPtr<CapabilityInfo> info = new CapabilityInfo(
//             CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT,
//             R::string::capability_title_canRetrieveWindowContent,
//             R::string::capability_desc_canRetrieveWindowContent);
//     AutoPtr<IInteger32> obj;
//     CInteger32::New(IAccessibilityServiceInfo::CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT, (IInteger32**)&obj);
//     availableCapabilityInfos.Put(obj, info);

//     AutoPtr<CapabilityInfo> info2 = new CapabilityInfo(
//             CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION,
//             R::string::capability_title_canRequestTouchExploration,
//             R::string::capability_desc_canRequestTouchExploration);
//     obj = NULL;
//     CInteger32::New(IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION, (IInteger32**)&obj);
//     availableCapabilityInfos.Put(obj, info2);

//     AutoPtr<CapabilityInfo> info3 = new CapabilityInfo(
//             CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY,
//             R::string::capability_title_canRequestEnhancedWebAccessibility,
//             R::string::capability_desc_canRequestEnhancedWebAccessibility);
//     obj = NULL;
//     CInteger32::New(IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY, (IInteger32**)&obj);
//     availableCapabilityInfos.Put(obj, info3);

//     AutoPtr<CapabilityInfo> info4 = new CapabilityInfo(
//             CAPABILITY_CAN_REQUEST_FILTER_KEY_EVENTS,
//             R::string::capability_title_canRequestFilterKeyEvents,
//             R::string::capability_desc_canRequestFilterKeyEvents);
//     obj = NULL;
//     CInteger32::New(IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_FILTER_KEY_EVENTS, (IInteger32**)&obj);
//     availableCapabilityInfos.Put(obj, info4);

//     return availableCapabilityInfos;
// }

// static const HashMap<Int32, AutoPtr<CapabilityInfo> > CAccessibilityServiceInfo::sAvailableCapabilityInfos = InitsAvailableCapabilityInfos();

CAccessibilityServiceInfo::CAccessibilityServiceInfo()
    : mEventTypes(0)
    , mFeedbackType(0)
    , mNotificationTimeout(0)
    , mFlags(0)
    , mId(NULL)
    , mSettingsActivityName(NULL)
    , mCapabilities(0)
    , mDescriptionResId(0)
    , mNonLocalizedDescription(NULL)
{
}

CAccessibilityServiceInfo::~CAccessibilityServiceInfo()
{
}

ECode CAccessibilityServiceInfo::constructor()
{
    return NOERROR;
}

ECode CAccessibilityServiceInfo::constructor(
    /* [in] */ IResolveInfo* resolveInfo,
    /* [in] */ IContext* context)
{
    AutoPtr<IServiceInfo> serviceInfo;
    resolveInfo->GetServiceInfo((IServiceInfo**)&serviceInfo);
    String pkgName, name;
    IPackageItemInfo::Probe(serviceInfo)->GetPackageName(&pkgName);
    IPackageItemInfo::Probe(serviceInfo)->GetName(&name);
    AutoPtr<IComponentName> cname;
    CComponentName::New(pkgName, name, (IComponentName**)&cname);
    cname->FlattenToShortString(&mId);
    mResolveInfo = resolveInfo;

    AutoPtr<IXmlResourceParser> parser;

    // try {
        AutoPtr<IPackageManager> packageManager;
        context->GetPackageManager((IPackageManager**)&packageManager);
        IPackageItemInfo::Probe(serviceInfo)->LoadXmlMetaData(packageManager,
            IAccessibilityService::SERVICE_META_DATA,
            (IXmlResourceParser**)&parser);
        if (parser == NULL) {
            return NOERROR;
        }

        Int32 type = 0;
        while (type != IXmlPullParser::END_DOCUMENT && type != IXmlPullParser::START_TAG) {
            IXmlPullParser::Probe(parser)->Next(&type);
        }

        String nodeName;
        IXmlPullParser::Probe(parser)->GetName(&nodeName);
        if (!nodeName.Equals(TAG_ACCESSIBILITY_SERVICE)) {
            // throw new XmlPullParserException( "Meta-data does not start with"
            //         + TAG_ACCESSIBILITY_SERVICE + " tag");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

        AutoPtr<IAttributeSet> allAttributes = Xml::AsAttributeSet(IXmlPullParser::Probe(parser));

        AutoPtr<IResources> resources;
        AutoPtr<IApplicationInfo> appInfo;
        IComponentInfo::Probe(serviceInfo)->GetApplicationInfo((IApplicationInfo**)&appInfo);
        packageManager->GetResourcesForApplication(appInfo, (IResources**)&resources);

        AutoPtr<ITypedArray> asAttributes;
        AutoPtr<ArrayOf<Int32> > vals = ArrayOf<Int32>::Alloc(8);
        for (Int32 i = 0; i < 8; i++) {
            (*vals)[i] = R::styleable::AccessibilityService[i];
        }

        resources->ObtainAttributes(allAttributes,
                vals, (ITypedArray**)&asAttributes);
        asAttributes->GetInt32(
                R::styleable::AccessibilityService_accessibilityEventTypes,
                0, &mEventTypes);
        String packageNamez;
        asAttributes->GetString(
                R::styleable::AccessibilityService_packageNames, &packageNamez);
        if (packageNamez != NULL) {
            mPackageNames = NULL;
            AutoPtr< ArrayOf<String> > events;
            StringUtils::Split(packageNamez, String("(\\s)*,(\\s)*"), (ArrayOf<String>**)&mPackageNames);
        }
        asAttributes->GetInt32(
                R::styleable::AccessibilityService_accessibilityFeedbackType,
                0, &mFeedbackType);
        asAttributes->GetInt32(
                R::styleable::AccessibilityService_notificationTimeout,
                0, (Int32*)&mNotificationTimeout);
        asAttributes->GetInt32(
                R::styleable::AccessibilityService_accessibilityFlags, 0, &mFlags);
        asAttributes->GetString(
                R::styleable::AccessibilityService_settingsActivity, &mSettingsActivityName);
        Boolean res;
        if (asAttributes->GetBoolean(
                R::styleable::AccessibilityService_canRetrieveWindowContent, FALSE, &res), res) {
            mCapabilities |= IAccessibilityServiceInfo::CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT;
        }
        // if (asAttributes->GetBoolean(
        //         R::styleable::AccessibilityService_canRequestTouchExplorationMode, FALSE, &res), res) {
        //     mCapabilities |= IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION;
        // }
        // if (asAttributes->GetBoolean(
        //         R::styleable::AccessibilityService_canRequestEnhancedWebAccessibility, FALSE, &res), res) {
        //         mCapabilities |= IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY;
        // }
        // if (asAttributes->GetBoolean(
        //     R::styleable::AccessibilityService_canRequestFilterKeyEvents, FALSE, &res), res) {
        //     mCapabilities |= IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_FILTER_KEY_EVENTS;
        // }
        AutoPtr<ITypedValue> peekedValue;
        asAttributes->PeekValue(
                R::styleable::AccessibilityService_description,
                (ITypedValue**)&peekedValue);
        if (peekedValue != NULL) {
            peekedValue->GetResourceId(&mDescriptionResId);
            AutoPtr<ICharSequence> nonLocalizedDescription;
            peekedValue->CoerceToString((ICharSequence**)&nonLocalizedDescription);
            if (nonLocalizedDescription != NULL) {
                String des;
                nonLocalizedDescription->ToString(&des);
                mNonLocalizedDescription = des.Trim();
            }
        }
        asAttributes->Recycle();
    // } catch (NameNotFoundException e) {
    //     throw new XmlPullParserException( "Unable to create context for: "
    //             + serviceInfo.packageName);
    // } finally {
        if (parser != NULL) {
            parser->Close();
        }
    // }
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetEventTypes(
    /* [in] */ Int32 types)
{
    mEventTypes = types;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetEventTypes(
    /* [out] */ Int32* types)
{
    VALIDATE_NOT_NULL(types);
    *types = mEventTypes;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetPackageNames(
    /* [in] */ ArrayOf<String>* names)
{
    mPackageNames = names;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetPackageNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mPackageNames;
    REFCOUNT_ADD(*names);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFeedbackType(
    /* [in] */ Int32 type)
{
    mFeedbackType = type;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFeedbackType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mFeedbackType;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetNotificationTimeout(
    /* [in] */ Int64 timeout)
{
    mNotificationTimeout = timeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetNotificationTimeout(
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mNotificationTimeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::UpdateDynamicallyConfigurableProperties(
    /* [in] */ IAccessibilityServiceInfo* other)
{
    other->GetEventTypes(&mEventTypes);
    mPackageNames = NULL;
    other->GetPackageNames((ArrayOf<Elastos::String>**)&mPackageNames);
    other->GetFeedbackType(&mFeedbackType);
    other->GetNotificationTimeout(&mNotificationTimeout);
    other->GetFlags(&mFlags);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetComponentName(
    /* [in] */ IComponentName* component)
{
    return component->FlattenToShortString(&mId);
}

ECode CAccessibilityServiceInfo::GetId(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mId;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetResolveInfo(
    /* [out] */ IResolveInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mResolveInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetSettingsActivityName(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mSettingsActivityName;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetCanRetrieveWindowContent(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mCapabilities &
            IAccessibilityServiceInfo::CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT) != 0;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetCapabilities(
    /* [out] */ Int32* capabilities)
{
    VALIDATE_NOT_NULL(capabilities);
    *capabilities = mCapabilities;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetCapabilities(
    /* [in] */ Int32 capabilities)
{
    mCapabilities = capabilities;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetDescription(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mNonLocalizedDescription;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::LoadDescription(
    /* [in] */ IPackageManager* packageManager,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    if (mDescriptionResId == 0) {
        *str = mNonLocalizedDescription;
        return NOERROR;
    }
    AutoPtr<IServiceInfo> serviceInfo;
    mResolveInfo->GetServiceInfo((IServiceInfo**)&serviceInfo);
    AutoPtr<ICharSequence> description;
    AutoPtr<IApplicationInfo> appInfo;
    IComponentInfo::Probe(serviceInfo)->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String pkgName;
    IPackageItemInfo::Probe(serviceInfo)->GetPackageName(&pkgName);
    packageManager->GetText(pkgName, mDescriptionResId, appInfo, (ICharSequence**)&description);
    if (description != NULL) {
        String des;
        description->ToString(&des);
        *str = des.Trim();
        return NOERROR;
    }
    *str = String(NULL);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = 0;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest)

    dest->WriteInt32(mEventTypes);

    if (NULL != mPackageNames) {
        dest->WriteInt32(1);
        dest->WriteArrayOfString(mPackageNames);
    }
    else {
        dest->WriteInt32(0);
    }

    dest->WriteInt32(mFeedbackType);
    dest->WriteInt64(mNotificationTimeout);
    dest->WriteInt32(mFlags);
    dest->WriteString(mId);

    if (NULL != mResolveInfo) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> parcelable = IParcelable::Probe(mResolveInfo);
        FAIL_RETURN(parcelable->WriteToParcel(dest))
    }
    else {
        dest->WriteInt32(0);
    }

    dest->WriteString(mSettingsActivityName);
    dest->WriteInt32(mCapabilities);
    dest->WriteInt32(mDescriptionResId);
    dest->WriteString(mNonLocalizedDescription);

    return NOERROR;
}

ECode CAccessibilityServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source)

    Int32 value = 0;
    source->ReadInt32(&mEventTypes);

    if (source->ReadInt32(&value), value != 0) {
        source->ReadArrayOfString((ArrayOf<String>**)&mPackageNames);
    }

    source->ReadInt32(&mFeedbackType);
    source->ReadInt64(&mNotificationTimeout);
    source->ReadInt32(&mFlags);
    source->ReadString(&mId);

    if (source->ReadInt32(&value), value != 0) {
        AutoPtr<IResolveInfo> resolveInfo;
        IParcelable::Probe(resolveInfo)->ReadFromParcel(source);
        mResolveInfo = resolveInfo;
    }

    source->ReadString(&mSettingsActivityName);
    source->ReadInt32(&mCapabilities);
    source->ReadInt32(&mDescriptionResId);
    source->ReadString(&mNonLocalizedDescription);

    return NOERROR;
}

void CAccessibilityServiceInfo::InitFromParcel(
    /* [in] */ IParcel* parcel)
{
    ReadFromParcel(parcel);
}

ECode CAccessibilityServiceInfo::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    Int32 result;
    result = 31 * 1 + ((mId == NULL) ? 0 : mId.GetHashCode());
    *code = result;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    if (THIS_PROBE(IInterface) == obj) {
        *isEqual = TRUE;
        return NOERROR;
    }

    if (obj == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }

    if (!IAccessibilityServiceInfo::Probe(obj)) {
        *isEqual = FALSE;
        return NOERROR;
    }

    AutoPtr<CAccessibilityServiceInfo> other =
    (CAccessibilityServiceInfo*)IAccessibilityServiceInfo::Probe(obj);
    if (mId == NULL) {
        if (other->mId != NULL) {
            *isEqual = FALSE;
            return NOERROR;
        }
    }
    else if (mId != other->mId) {
        *isEqual = FALSE;
        return NOERROR;
    }

    *isEqual = TRUE;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<StringBuilder> stringBuilder = new StringBuilder();
    AppendEventTypes(stringBuilder, mEventTypes);
    stringBuilder->Append(", ");
    AppendPackageNames(stringBuilder, mPackageNames);
    stringBuilder->Append(", ");
    AppendFeedbackTypes(stringBuilder, mFeedbackType);
    stringBuilder->Append(", ");
    stringBuilder->Append("mNotificationTimeout: ");
    stringBuilder->Append(mNotificationTimeout);
    stringBuilder->Append(", ");
    AppendFlags(stringBuilder, mFlags);
    stringBuilder->Append(", ");
    stringBuilder->Append("mId: ");
    stringBuilder->Append(mId);
    stringBuilder->Append(", ");
    stringBuilder->Append("mResolveInfo: ");
    stringBuilder->Append(mResolveInfo);
    stringBuilder->Append(", ");
    stringBuilder->Append("mSettingsActivityName: ");
    stringBuilder->Append(mSettingsActivityName);
    stringBuilder->Append(", ");
    AppendCapabilities(stringBuilder, mCapabilities);

    *str = stringBuilder->ToString();
    return NOERROR;
}

void CAccessibilityServiceInfo::AppendFeedbackTypes(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 feedbackTypes)
{
    (*stringBuilder) += "feedbackTypes:";
    (*stringBuilder) += "[";

    while (feedbackTypes != 0) {
        Int32 feedbackTypeBit = (1 << Elastos::Core::Math::NumberOfTrailingZeros(feedbackTypes));
        String s;
        FeedbackTypeToString(feedbackTypeBit, &s);
        (*stringBuilder) += s;
        feedbackTypes &= ~feedbackTypeBit;
        if (feedbackTypes != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendPackageNames(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ ArrayOf<String>* packageNames)
{
    (*stringBuilder) += "packageNames:";
    (*stringBuilder) += "[";
    if (packageNames != NULL) {
        Int32 packageNameCount = packageNames->GetLength();
        for (Int32 i = 0; i < packageNameCount; i++) {
            (*stringBuilder) += (*packageNames)[i];
            if (i < packageNameCount - 1) {
                (*stringBuilder) += ", ";
            }
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendEventTypes(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 eventTypes)
{
    String info;
    assert(0 && "TODO");
    // Int32 eventTypeBit;
    // AutoPtr<IAccessibilityEventHelper> helper;
    // CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&helper);

    // (*stringBuilder) += "eventTypes:";
    // (*stringBuilder) += "[";
    // while (eventTypes != 0) {
    //     eventTypeBit = (1 << Elastos::Core::Math::NumberOfTrailingZeros(eventTypes));
    //     helper->EventTypeToString(eventTypeBit, &info);
    //     (*stringBuilder) += info;
    //     eventTypes &= ~eventTypeBit;
    //     if (eventTypes != 0) {
    //         (*stringBuilder) += ", ";
    //     }
    // }
    // (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendFlags(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 flags)
{
    (*stringBuilder) += "flags:";
    (*stringBuilder) += "[";
    while (flags != 0) {
        Int32 flagBit = (1 << Elastos::Core::Math::NumberOfTrailingZeros(flags));
        String s;
        FlagToString(flagBit, &s);
        (*stringBuilder) += s;
        flags &= ~flagBit;
        if (flags != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendCapabilities(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 capabilities)
{
    (*stringBuilder) += "capabilities:";
    (*stringBuilder) += "[";
    while (capabilities != 0) {
        Int32 capabilityBit = (1 << Elastos::Core::Math::NumberOfTrailingZeros(capabilities));
        String s;
        CapabilityToString(capabilityBit, &s);
        (*stringBuilder) += s;
        capabilities &= ~capabilityBit;
        if (capabilities != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

ECode CAccessibilityServiceInfo::FeedbackTypeToString(
    /* [in] */ Int32 feedbackType,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder builder("[");
    while (feedbackType != 0) {
        Int32 feedbackTypeFlag = 1 << Elastos::Core::Math::NumberOfTrailingZeros(feedbackType);
        feedbackType &= ~feedbackTypeFlag;
        switch (feedbackTypeFlag) {
            case IAccessibilityServiceInfo::FEEDBACK_AUDIBLE:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_AUDIBLE";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_HAPTIC:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_HAPTIC";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_GENERIC:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_GENERIC";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_SPOKEN:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_SPOKEN";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_VISUAL:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_VISUAL";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_BRAILLE:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_BRAILLE";
                break;
        }
    }
    builder += "]";
    *str = builder.ToString();
    return NOERROR;
}

ECode CAccessibilityServiceInfo::FlagToString(
    /* [in] */ Int32 flag,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    switch (flag) {
        case IAccessibilityServiceInfo::DEFAULT:
            *str = "DEFAULT";
            break;
        case IAccessibilityServiceInfo::FLAG_INCLUDE_NOT_IMPORTANT_VIEWS:
            *str = "FLAG_INCLUDE_NOT_IMPORTANT_VIEWS";
            break;
        case IAccessibilityServiceInfo::FLAG_REQUEST_TOUCH_EXPLORATION_MODE:
            *str = "FLAG_REQUEST_TOUCH_EXPLORATION_MODE";
            break;
        case IAccessibilityServiceInfo::FLAG_REQUEST_ENHANCED_WEB_ACCESSIBILITY:
            *str = "FLAG_REQUEST_ENHANCED_WEB_ACCESSIBILITY";
            break;
        case IAccessibilityServiceInfo::FLAG_REPORT_VIEW_IDS:
            *str = "FLAG_REPORT_VIEW_IDS";
            break;
        case IAccessibilityServiceInfo::FLAG_REQUEST_FILTER_KEY_EVENTS:
            *str = "FLAG_REQUEST_FILTER_KEY_EVENTS";
            break;
        case IAccessibilityServiceInfo::FLAG_RETRIEVE_INTERACTIVE_WINDOWS:
            *str = "FLAG_RETRIEVE_INTERACTIVE_WINDOWS";
            break;
        default:
            *str = String(NULL);
            break;
        }
    return NOERROR;
}

ECode CAccessibilityServiceInfo::CapabilityToString(
    /* [in] */ Int32 capability,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    switch (capability) {
        case IAccessibilityServiceInfo::CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT:
            *str = "CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT";
            break;
        case IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION:
            *str = "CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION";
            break;
        case IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY:
            *str = "CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY";
            break;
        case IAccessibilityServiceInfo::CAPABILITY_CAN_REQUEST_FILTER_KEY_EVENTS:
            *str = "CAPABILITY_CAN_FILTER_KEY_EVENTS";
            break;
        default:
            *str = "UNKNOWN";
            break;
        }
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetCapabilityInfos(
    /* [out] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    *infos = NULL;
    assert(0 && "TODO");

    // AutoPtr<ICollections> coll;
    // CCollections::AcquireSingleton((ICollections**)&coll);

    // if (mCapabilities == 0) {
    //     return coll->GetEmptyList(infos);
    // }
    // Int32 capabilities = mCapabilities;
    // AutoPtr<IList> capabilityInfos;
    // CArrayList::New((IList**)&capabilityInfos);
    // while (capabilities != 0) {
    //     Int32 capabilityBit = 1 << Elastos::Core::Math::NumberOfTrailingZeros(capabilities);
    //     capabilities &= ~capabilityBit;

    //     AutoPtr<IInteger32> obj;
    //     CInteger32::New(capabilityBit, (IInteger32**)&obj);
    //     AutoPtr<CapabilityInfo> capabilityInfo;
    //     sAvailableCapabilityInfos.Get(obj, (IInterface**)&capabilityInfo);

    //     if (capabilityInfo != NULL) {
    //         capabilityInfos->Add(capabilityInfo);
    //     }
    // }
    // *infos = capabilityInfos;
    // REFCOUNT_ADD(*infos);
    return NOERROR;
}

String CAccessibilityServiceInfo::CapabilityInfo::ToString()
{
    StringBuilder sb("{CapabilityInfo ");
    sb += mCapability;
    sb += " / ";
    sb += mTitleResId;
    sb += " / ";
    sb += mDescResId;
    sb += "}";
    return sb.ToString();
}

}
}
}
