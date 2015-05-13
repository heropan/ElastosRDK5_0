
#include "hardware/display/CDisplayManager.h"
#include "hardware/display/DisplayManagerGlobal.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

const char* CDisplayManager::TAG = "DisplayManager";
const Boolean CDisplayManager::DEBUG;

CDisplayManager::CDisplayManager()
{
}

CDisplayManager::~CDisplayManager()
{
}

ECode CDisplayManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    mGlobal = DisplayManagerGlobal::GetInstance();

    return NOERROR;
}

ECode CDisplayManager::GetDisplay(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplay** display)
{
    VALIDATE_NOT_NULL(display);

    Mutex::Autolock lock(mLock);

    AutoPtr<IDisplay> temp = GetOrCreateDisplayLocked(displayId, FALSE /*assumeValid*/);
    *display = temp;
    INTERFACE_ADDREF(*display);

    return NOERROR;
}

ECode CDisplayManager::GetDisplays(
    /* [out] */ ArrayOf<IDisplay*>** displays)
{
    return GetDisplaysEx(String(NULL), displays);
}

ECode CDisplayManager::GetDisplaysEx(
    /* [in] */ const String& category,
    /* [out] */ ArrayOf<IDisplay*>** displays)
{
    VALIDATE_NOT_NULL(displays);

    AutoPtr<ArrayOf<Int32> > displayIds;
    mGlobal->GetDisplayIds((ArrayOf<Int32>**)&displayIds);

    Mutex::Autolock lock(mLock);

    //try {
        if (category.IsNull()) {
            AddMatchingDisplaysLocked(mTempDisplays, displayIds, -1);
        }
        else if (category.Equals(DISPLAY_CATEGORY_PRESENTATION)) {
            AddMatchingDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_WIFI);
            AddMatchingDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_HDMI);
            AddMatchingDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_OVERLAY);
        }
        *displays = ArrayOf<IDisplay*>::Alloc(mTempDisplays.GetSize());
        if (*displays == NULL) {
            mTempDisplays.Clear();
            return E_OUT_OF_MEMORY_ERROR;
        }
        INTERFACE_ADDREF(*displays);
        List<AutoPtr<IDisplay> >::Iterator iter = mTempDisplays.Begin();
        for (Int32 i = 0; iter != mTempDisplays.End(); ++iter, i++) {
            (*displays)->Set(i, *iter);
        }

    //} finally {
        mTempDisplays.Clear();
    //}

    return NOERROR;
}

void CDisplayManager::AddMatchingDisplaysLocked(
    /* [in] */ List<AutoPtr<IDisplay> >& displays,
    /* [in] */ ArrayOf<Int32>* displayIds,
    /* [in] */ Int32 matchType)
{
    Int32 length = displayIds->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IDisplay> display = GetOrCreateDisplayLocked(
            (*displayIds)[i], TRUE /*assumeValid*/);
        Int32 type;
        if (display != NULL && (matchType < 0
            || (display->GetType(&type), type) == matchType)) {
            displays.PushBack(display);
        }
    }
}

AutoPtr<IDisplay> CDisplayManager::GetOrCreateDisplayLocked(
    /* [in] */ Int32 displayId,
    /* [in] */ Boolean assumeValid)
{
    AutoPtr<IDisplay> display;
    HashMap<Int32, AutoPtr<IDisplay> >::Iterator iter = mDisplays.Find(displayId);
    if(iter != mDisplays.End())
        display = iter->mSecond;
    Boolean isValid;
    if (display == NULL) {
        AutoPtr<ICompatibilityInfoHolder> infoHolder;
        mContext->GetCompatibilityInfo(
            displayId, (ICompatibilityInfoHolder**)&infoHolder);
        mGlobal->GetCompatibleDisplay(displayId, infoHolder, (IDisplay**)&display);
        if (display != NULL) {
            mDisplays[displayId] = display;
        }
    }
    else if (!assumeValid && !(display->IsValid(&isValid), isValid)) {
        display = NULL;
    }

    return display;
}

ECode CDisplayManager::RegisterDisplayListener(
    /* [in] */ IDisplayListener* listener,
    /* [in] */ IHandler* handler)
{
    return mGlobal->RegisterDisplayListener(listener, handler);
}

ECode CDisplayManager::UnregisterDisplayListener(
    /* [in] */ IDisplayListener* listener)
{
    return mGlobal->UnregisterDisplayListener(listener);
}

ECode CDisplayManager::ScanWifiDisplays()
{
    return mGlobal->ScanWifiDisplays();
}

ECode CDisplayManager::ConnectWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    return mGlobal->ConnectWifiDisplay(deviceAddress);
}

ECode CDisplayManager::DisconnectWifiDisplay()
{
    return mGlobal->DisconnectWifiDisplay();
}

ECode CDisplayManager::RenameWifiDisplay(
    /* [in] */ const String& deviceAddress,
    /* [in] */ const String& alias)
{
    return mGlobal->RenameWifiDisplay(deviceAddress, alias);
}

ECode CDisplayManager::ForgetWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    return mGlobal->ForgetWifiDisplay(deviceAddress);
}

ECode CDisplayManager::GetWifiDisplayStatus(
    /* [out] */ IWifiDisplayStatus** status)
{
    VALIDATE_NOT_NULL(status);
    return mGlobal->GetWifiDisplayStatus(status);
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
