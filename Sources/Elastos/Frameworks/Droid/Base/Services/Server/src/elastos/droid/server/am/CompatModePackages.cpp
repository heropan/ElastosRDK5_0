
#include "elastos/droid/server/am/CompatModePackages.h"
#include "elastos/droid/app/AppGlobals.h"
#include "elastos/droid/os/Handler.h"
#include "util/Xml.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::IO::CFile;
using Elastos::IO::IOutputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::CInteger32;
using Elastos::Core::StringUtils;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Content::Res::CCompatibilityInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const Int32 CompatModePackages::COMPAT_FLAG_DONT_ASK;
const Int32 CompatModePackages::COMPAT_FLAG_ENABLED;

const Int32 CompatModePackages::MSG_WRITE = 300;//CActivityManagerService::FIRST_COMPAT_MODE_MSG;

ECode CompatModePackages::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case CompatModePackages::MSG_WRITE:
            mHost->SaveCompatModes();
            break;
        default:
            HandlerBase::HandleMessage(msg);
            break;
    }

    return NOERROR;
}

CompatModePackages::CompatModePackages(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IFile* systemDir)
    : TAG(CActivityManagerService::TAG)
    , DEBUG_CONFIGURATION(CActivityManagerService::DEBUG_CONFIGURATION)
    , mService(service)
{
    mPackages = new HashMap<String, AutoPtr<IInteger32> >();
    mHandler = new MyHandler(this);

    AutoPtr<IFile> file;
    CFile::New(systemDir, String("packages-compat.xml"), (IFile**)&file);
    CAtomicFile::New(file, (IAtomicFile**)&mFile);

    AutoPtr<IFileInputStream> fis;
//     try {
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    FAIL_GOTO(mFile->OpenRead((IFileInputStream**)&fis), Exit);
    FAIL_GOTO(parser->SetInput(fis, String(NULL)), Exit);
    {

        Int32 eventType;
        FAIL_GOTO(parser->GetEventType(&eventType), Exit);
        while (eventType != IXmlPullParser::START_TAG) {
            FAIL_GOTO(parser->Next(&eventType), Exit);
        }
        String tagName;
        parser->GetName(&tagName);
        if (CString("compat-packages").Equals(tagName)) {
            parser->Next(&eventType);
            do {
                if (eventType == IXmlPullParser::START_TAG) {
                    parser->GetName(&tagName);
                    Int32 depth;
                    parser->GetDepth(&depth);
                    if (depth == 2) {
                        if (CString("pkg").Equals(tagName)) {
                            String pkg;
                            parser->GetAttributeValue(String(NULL), String("name"), &pkg);
                            if (!pkg.IsNull()) {
                                String mode;
                                parser->GetAttributeValue(String(NULL), String("mode"), &mode);
                                Int32 modeInt = 0;
                                if (!mode.IsNull()) {
    //                                     try {
                                    modeInt = StringUtils::ParseInt32(mode);
    //                                     } catch (NumberFormatException e) {
    //                                     }
                                }
                                AutoPtr<IInteger32> modeInteger;
                                CInteger32::New(modeInt, (IInteger32**)&modeInteger);
                                mPackages->Insert(HashMap<String, AutoPtr<IInteger32> >::ValueType(pkg, modeInteger));
                            }
                        }
                    }
                }
                parser->Next(&eventType);
            } while (eventType != IXmlPullParser::END_DOCUMENT);
        }

    }

    // } catch (XmlPullParserException e) {
    //     Slog.w(TAG, "Error reading compat-packages", e);
    // } catch (java.io.IOException e) {
    //     if (fis != null) Slog.w(TAG, "Error reading compat-packages", e);
    // } finally {

Exit:
    if (fis != NULL) {
            // try {
        fis->Close();
            // } catch (java.io.IOException e1) {
            // }
    }
//      }
}

CompatModePackages::~CompatModePackages()
{
}

AutoPtr<HashMap<String, AutoPtr<IInteger32> > > CompatModePackages::GetPackages()
{
    return mPackages;
}

Int32 CompatModePackages::GetPackageFlags(
    /* [in] */ const String& packageName)
{
    HashMap<String, AutoPtr<IInteger32> >::Iterator item = mPackages->Find(packageName);
    if (item != mPackages->End()) {
        Int32 value;
        item->mSecond->GetValue(&value);
        return value;
    }

    return 0;
}

void CompatModePackages::HandlePackageAddedLocked(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean updated)
{
    AutoPtr<IApplicationInfo> ai;
//     try {
    AppGlobals::GetPackageManager()->GetApplicationInfo(packageName, 0, 0, (IApplicationInfo**)&ai);
//     } catch (RemoteException e) {
//     }
    if (ai == NULL) {
        return;
    }
    AutoPtr<ICompatibilityInfo> ci = CompatibilityInfoForPackageLocked(ai);
    Boolean support, neverSupport;
    ci->AlwaysSupportsScreen(&support);
    ci->NeverSupportsScreen(&neverSupport);
    Boolean mayCompat = !support && !neverSupport;

    if (updated) {
        // Update -- if the app no longer can run in compat mode, clear
        // any current settings for it.
        if (!mayCompat && mPackages->Find(packageName) != mPackages->End()) {
            mPackages->Erase(packageName);
            mHandler->RemoveMessages(MSG_WRITE);
            Boolean result;
            mHandler->SendEmptyMessageDelayed(MSG_WRITE, 10000, &result);
        }
    }
}

AutoPtr<ICompatibilityInfo> CompatModePackages::CompatibilityInfoForPackageLocked(
    /* [in] */ IApplicationInfo* ai)
{
    Int32 layout;
    mService->mConfiguration->GetScreenLayout(&layout);
    Int32 widthDp;
    mService->mConfiguration->GetSmallestScreenWidthDp(&widthDp);
    String pkgName;
    ai->GetPackageName(&pkgName);
    AutoPtr<ICompatibilityInfo> ci;
    CCompatibilityInfo::New(ai, layout,
            widthDp,
            (GetPackageFlags(pkgName)&COMPAT_FLAG_ENABLED) != 0, (ICompatibilityInfo**)&ci);
    Slogger::I(TAG, "*********** COMPAT FOR PKG  ai.packageName :%s  + ci ", pkgName.string());
    return ci;
}

Int32 CompatModePackages::ComputeCompatModeLocked(
    /* [in] */ IApplicationInfo* ai)
{
    String packageName;
    ai->GetPackageName(&packageName);
    Boolean enabled = (GetPackageFlags(packageName)&COMPAT_FLAG_ENABLED) != 0;
    Int32 layout;
    mService->mConfiguration->GetScreenLayout(&layout);
    Int32 widthDp;
    mService->mConfiguration->GetSmallestScreenWidthDp(&widthDp);
    AutoPtr<ICompatibilityInfo> info;
    CCompatibilityInfo::New(ai, layout, widthDp, enabled, (ICompatibilityInfo**)&info);
    Boolean support;
    info->AlwaysSupportsScreen(&support);
    if (support) {
        return IActivityManager::COMPAT_MODE_NEVER;
    }
    Boolean neverSupport;
    info->NeverSupportsScreen(&neverSupport);
    if (neverSupport) {
        return IActivityManager::COMPAT_MODE_ALWAYS;
    }
    return enabled ? IActivityManager::COMPAT_MODE_ENABLED
            : IActivityManager::COMPAT_MODE_DISABLED;
}

Boolean CompatModePackages::GetFrontActivityAskCompatModeLocked()
{
    AutoPtr<ActivityRecord> r = mService->mMainStack->GetTopRunningActivityLocked(NULL);
    if (r == NULL) {
        return FALSE;
    }
    return GetPackageAskCompatModeLocked(r->mPackageName);
}

Boolean CompatModePackages::GetPackageAskCompatModeLocked(
    /* [in] */ const String& packageName)
{
    return (GetPackageFlags(packageName)&COMPAT_FLAG_DONT_ASK) == 0;
}

void CompatModePackages::SetFrontActivityAskCompatModeLocked(
    /* [in] */ Boolean ask)
{
    AutoPtr<ActivityRecord> r = mService->mMainStack->GetTopRunningActivityLocked(NULL);
    if (r != NULL) {
        SetPackageAskCompatModeLocked(r->mPackageName, ask);
    }
}

void CompatModePackages::SetPackageAskCompatModeLocked(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean ask)
{
    Int32 curFlags = GetPackageFlags(packageName);
    Int32 newFlags = ask ? (curFlags&~COMPAT_FLAG_DONT_ASK) : (curFlags|COMPAT_FLAG_DONT_ASK);
    if (curFlags != newFlags) {
        if (newFlags != 0) {
            AutoPtr<IInteger32> newFlagsInteger;
            CInteger32::New(newFlags, (IInteger32**)&newFlagsInteger);
            mPackages->Insert(HashMap<String, AutoPtr<IInteger32> >::ValueType(packageName, newFlagsInteger));
        }
        else {
            mPackages->Erase(packageName);
        }

        mHandler->RemoveMessages(MSG_WRITE);
        Boolean result;
        mHandler->SendEmptyMessageDelayed(MSG_WRITE, 10000, &result);
    }
}

Int32 CompatModePackages::GetFrontActivityScreenCompatModeLocked()
{
    AutoPtr<ActivityRecord> r = mService->mMainStack->GetTopRunningActivityLocked(NULL);
    if (r == NULL) {
        return IActivityManager::COMPAT_MODE_UNKNOWN;
    }
    AutoPtr<IApplicationInfo> appInfo;
    r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    return ComputeCompatModeLocked(appInfo);
}

void CompatModePackages::SetFrontActivityScreenCompatModeLocked(
    /* [in] */ Int32 mode)
{
    AutoPtr<ActivityRecord> r = mService->mMainStack->GetTopRunningActivityLocked(NULL);
    if (r == NULL) {
        Slogger::W(TAG, "setFrontActivityScreenCompatMode failed: no top activity");
        return;
    }
    AutoPtr<IApplicationInfo> appInfo;
    r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    SetPackageScreenCompatModeLocked(appInfo, mode);
}

Int32 CompatModePackages::GetPackageScreenCompatModeLocked(
    /* [in] */ const String& packageName)
{
    AutoPtr<IApplicationInfo> ai;
//     try {
    AppGlobals::GetPackageManager()->GetApplicationInfo(packageName, 0, 0, (IApplicationInfo**)&ai);
//     } catch (RemoteException e) {
//     }
    if (ai == NULL) {
        return IActivityManager::COMPAT_MODE_UNKNOWN;
    }
    return ComputeCompatModeLocked(ai);
}

void CompatModePackages::SetPackageScreenCompatModeLocked(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 mode)
{
    AutoPtr<IApplicationInfo> ai;
//     try {
    AppGlobals::GetPackageManager()->GetApplicationInfo(packageName, 0, 0, (IApplicationInfo**)&ai);
//     } catch (RemoteException e) {
//     }
    if (ai == NULL) {
        Slogger::W(TAG, "setPackageScreenCompatMode failed: unknown package %s", packageName.string());
        return;
    }
    SetPackageScreenCompatModeLocked(ai, mode);
}

void CompatModePackages::SetPackageScreenCompatModeLocked(
    /* [in] */ IApplicationInfo* ai,
    /* [in] */ Int32 mode)
{
    String packageName;
    ai->GetPackageName(&packageName);

    Int32 curFlags = GetPackageFlags(packageName);

    Boolean enable;
    switch (mode) {
        case IActivityManager::COMPAT_MODE_DISABLED:
            enable = FALSE;
            break;
        case IActivityManager::COMPAT_MODE_ENABLED:
            enable = TRUE;
            break;
        case IActivityManager::COMPAT_MODE_TOGGLE:
            enable = (curFlags&COMPAT_FLAG_ENABLED) == 0;
            break;
        default:
            Slogger::W(TAG, "Unknown screen compat mode req #%d; ignoring", mode);
            return;
    }

    Int32 newFlags = curFlags;
    if (enable) {
        newFlags |= COMPAT_FLAG_ENABLED;
    } else {
        newFlags &= ~COMPAT_FLAG_ENABLED;
    }

    AutoPtr<ICompatibilityInfo> ci = CompatibilityInfoForPackageLocked(ai);
    Boolean support;
    ci->AlwaysSupportsScreen(&support);
    if (support) {
        Slogger::W(TAG, "Ignoring compat mode change of %s; compatibility never needed", packageName.string());
        newFlags = 0;
    }
    Boolean neverSupport;
    ci->NeverSupportsScreen(&neverSupport);
    if (neverSupport) {
        Slogger::W(TAG, "Ignoring compat mode change of %s; compatibility always needed", packageName.string());
        newFlags = 0;
    }

    if (newFlags != curFlags) {
        if (newFlags != 0) {
            AutoPtr<IInteger32> newFlagsInteger;
            CInteger32::New(newFlags, (IInteger32**)&newFlagsInteger);
            mPackages->Insert(HashMap<String, AutoPtr<IInteger32> >::ValueType(packageName, newFlagsInteger));
        }
        else {
            mPackages->Erase(packageName);
        }

        // Need to get compatibility info in new state.
        ci = CompatibilityInfoForPackageLocked(ai);

        mHandler->RemoveMessages(MSG_WRITE);
        Boolean result;
        mHandler->SendEmptyMessageDelayed(MSG_WRITE, 10000, &result);

        AutoPtr<ActivityRecord> starting = mService->mMainStack->GetTopRunningActivityLocked(NULL);

        // All activities that came from the package must be
        // restarted as if there was a config change.
        List< AutoPtr<ActivityRecord> >::ReverseIterator rit = mService->mMainStack->mHistory.RBegin();
        for (; rit != mService->mMainStack->mHistory.REnd(); ++rit) {
            AutoPtr<ActivityRecord> a = *rit;
            String pkgName;
            a->mInfo->GetPackageName(&pkgName);
            if (pkgName.Equals(packageName)) {
                a->mForceNewConfig = true;
                if (starting != NULL && a == starting && a->mVisible) {
                    a->StartFreezingScreenLocked(starting->mApp,
                            IActivityInfo::CONFIG_SCREEN_LAYOUT);
                }
            }
        }

        // Tell all processes that loaded this package about the change.
        List< AutoPtr<ProcessRecord> >::ReverseIterator prRit = mService->mLruProcesses.RBegin();
        for (; prRit != mService->mLruProcesses.REnd(); ++prRit) {
            AutoPtr<ProcessRecord> app = *prRit;
            HashSet<String>::Iterator fit = app->mPkgList.Find(packageName);
            if (fit == app->mPkgList.End()) {
                continue;
            }
//             try {
            if (app->mThread != NULL) {
                if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Sending to proc %s new compat %p",
                        app->mProcessName.string(), ci.Get());
                app->mThread->UpdatePackageCompatibilityInfo(packageName, ci);
            }
//             } catch (Exception e) {
//             }
        }

        if (starting != NULL) {
            mService->mMainStack->EnsureActivityConfigurationLocked(starting, 0);
            // And we need to make sure at this point that all other activities
            // are made visible with the correct configuration.
            mService->mMainStack->EnsureActivitiesVisibleLocked(starting, 0);
        }
    }
}

void CompatModePackages::SaveCompatModes()
{
    HashMap<String, AutoPtr<IInteger32> > pkgs;
    {
        AutoLock lock(mService->mLock);
        pkgs.Insert(mPackages->Begin(), mPackages->End());
    }

    AutoPtr<IFileOutputStream> fos;

//     try {
    mFile->StartWrite((IFileOutputStream**)&fos);
    AutoPtr<IFastXmlSerializer> serializer;
    CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
    AutoPtr<IXmlSerializer> out = IXmlSerializer::Probe(serializer);
    out->SetOutput(IOutputStream::Probe(fos), String("utf-8"));
    AutoPtr<IBoolean> start;
    CBoolean::New(TRUE, (IBoolean**)&start);
    out->StartDocument(String(NULL), start);
    out->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);
    out->WriteStartTag(String(NULL), String("compat-packages"));

    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    Int32 screenLayout;
    mService->mConfiguration->GetScreenLayout(&screenLayout);
    Int32 smallestScreenWidthDp;
    mService->mConfiguration->GetSmallestScreenWidthDp(&smallestScreenWidthDp);
    HashMap<String, AutoPtr<IInteger32> >::Iterator it;
    for (it = pkgs.Begin(); it != pkgs.End(); ++it) {
        String pkg = it->mFirst;
        AutoPtr<IInteger32> modeInteger = it->mSecond;
        Int32 mode;
        modeInteger->GetValue(&mode);
        if (mode == 0) {
            continue;
        }
        AutoPtr<IApplicationInfo> ai;
//             try {
        pm->GetApplicationInfo(pkg, 0, 0, (IApplicationInfo**)&ai);
//             } catch (RemoteException e) {
//             }
        if (ai == NULL) {
            continue;
        }
        AutoPtr<ICompatibilityInfo> info;
        CCompatibilityInfo::New(ai, screenLayout,
                smallestScreenWidthDp, FALSE, (ICompatibilityInfo**)&info);
        Boolean support;
        info->AlwaysSupportsScreen(&support);
        if (support) {
            continue;
        }
        Boolean neverSupport;
        info->NeverSupportsScreen(&neverSupport);
        if (neverSupport) {
            continue;
        }
        out->WriteStartTag(String(NULL), String("pkg"));
        out->WriteAttribute(String(NULL), String("name"), pkg);
        out->WriteAttribute(String(NULL), String("mode"), StringUtils::Int32ToString(mode));
        out->WriteEndTag(String(NULL), String("pkg"));
    }

    out->WriteEndTag(String(NULL), String("compat-packages"));
    out->EndDocument();

    mFile->FinishWrite(fos);
//     } catch (java.io.IOException e1) {
//         Slog.w(TAG, "Error writing compat packages", e1);
//         if (fos != null) {
//             mFile.failWrite(fos);
//         }
//     }
}

void CompatModePackages::HandleMsgWrite()
{
    SaveCompatModes();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
