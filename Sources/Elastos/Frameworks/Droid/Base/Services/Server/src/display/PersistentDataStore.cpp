
#include "display/PersistentDataStore.h"
#include "util/Xml.h"
#include "util/XmlUtils.h"
#include <elastos/Slogger.h>

using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::IO::ICloseable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Hardware::Display::CWifiDisplay;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::XmlUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String PersistentDataStore::TAG("DisplayManager");

PersistentDataStore::PersistentDataStore()
    : mLoaded(FALSE)
    , mDirty(FALSE)
{
    AutoPtr<IFile> file;
    ASSERT_SUCCEEDED(CFile::New(
        String("/data/system/display-manager-state.xml"), (IFile**)&file));
    ASSERT_SUCCEEDED(CAtomicFile::New(file, (IAtomicFile**)&mAtomicFile));
}

void PersistentDataStore::SaveIfNeeded()
{
    if (mDirty) {
        Save();
        mDirty = FALSE;
    }
}

AutoPtr<IWifiDisplay> PersistentDataStore::GetRememberedWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    LoadIfNeeded();

    List< AutoPtr<IWifiDisplay> >::Iterator find = FindRememberedWifiDisplay(deviceAddress);
    if (find != mRememberedWifiDisplays.End()) {
        return *find;
    }
    return NULL;
}

AutoPtr< ArrayOf<IWifiDisplay*> > PersistentDataStore::GetRememberedWifiDisplays()
{
    LoadIfNeeded();
    AutoPtr< ArrayOf<IWifiDisplay*> > array = ArrayOf<IWifiDisplay*>::Alloc(
        mRememberedWifiDisplays.GetSize());
    List< AutoPtr<IWifiDisplay> >::Iterator iter = mRememberedWifiDisplays.Begin();
    for (Int32 i = 0; iter != mRememberedWifiDisplays.End(); ++iter, ++i) {
        array->Set(i, *iter);
    }
    return array;
}

AutoPtr<IWifiDisplay> PersistentDataStore::ApplyWifiDisplayAlias(
    /* [in] */ IWifiDisplay* display)
{
    if (display != NULL) {
        LoadIfNeeded();

        String deviceAddress;
        display->GetDeviceAddress(&deviceAddress);
        List< AutoPtr<IWifiDisplay> >::Iterator find =
            FindRememberedWifiDisplay(deviceAddress);
        String alias;
        if (find != mRememberedWifiDisplays.End()) {
            (*find)->GetDeviceAlias(&alias);
        }
        String alias2;
        display->GetDeviceAlias(&alias2);
        if (!alias2.Equals(alias)) {
            String deviceName;
            display->GetDeviceName(&deviceName);
            AutoPtr<IWifiDisplay> wifiDisp;
            CWifiDisplay::New(deviceAddress, deviceName, alias, (IWifiDisplay**)&wifiDisp);
            return wifiDisp;
        }
    }
    return display;
}

AutoPtr< ArrayOf<IWifiDisplay*> > PersistentDataStore::ApplyWifiDisplayAliases(
    /* [in] */ ArrayOf<IWifiDisplay*>* displays)
{
    AutoPtr< ArrayOf<IWifiDisplay*> > results = displays;
    if (results != NULL) {
        Int32 count = displays->GetLength();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IWifiDisplay> result = ApplyWifiDisplayAlias((*displays)[i]);
            if (result.Get() != (*displays)[i]) {
                if (results.Get() == displays) {
                    results = ArrayOf<IWifiDisplay*>::Alloc(count);
                    assert(results);
                    for (Int32 j = 0 ; j < count; j++) {
                        results->Set(j, (*displays)[j]);
                    }
                }
                results->Set(i, result);
            }
        }
    }
    return results;
}

Boolean PersistentDataStore::RememberWifiDisplay(
    /* [in] */ IWifiDisplay* display)
{
    LoadIfNeeded();

    String deviceAddress;
    display->GetDeviceAddress(&deviceAddress);
    List< AutoPtr<IWifiDisplay> >::Iterator find =
        FindRememberedWifiDisplay(deviceAddress);
    if (find != mRememberedWifiDisplays.End()) {
        AutoPtr<IWifiDisplay> other = *find;
        Boolean res;
        if (other->Equals(display, &res), res) {
            return FALSE; // already remembered without change
        }
        *find = display;
    }
    else {
        mRememberedWifiDisplays.PushBack(display);
    }
    SetDirty();
    return TRUE;
}

Boolean PersistentDataStore::ForgetWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    List<AutoPtr<IWifiDisplay> >::Iterator find =
        FindRememberedWifiDisplay(deviceAddress);
    if (find != mRememberedWifiDisplays.End()) {
        mRememberedWifiDisplays.Erase(find);
        SetDirty();
        return TRUE;
    }
    return FALSE;
}

List<AutoPtr<IWifiDisplay> >::Iterator PersistentDataStore::FindRememberedWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    List<AutoPtr<IWifiDisplay> >::Iterator iter = mRememberedWifiDisplays.Begin();
    for (; iter != mRememberedWifiDisplays.End(); ++iter) {
        String deviceAddress2;
        (*iter)->GetDeviceAddress(&deviceAddress2);
        if (deviceAddress2.Equals(deviceAddress)) {
            return iter;
        }
    }
    return mRememberedWifiDisplays.End();
}

void PersistentDataStore::LoadIfNeeded()
{
    if (!mLoaded) {
        Load();
        mLoaded = TRUE;
    }
}

void PersistentDataStore::SetDirty()
{
    mDirty = TRUE;
}

void PersistentDataStore::ClearState()
{
    mRememberedWifiDisplays.Clear();
}

void PersistentDataStore::Load()
{
    ClearState();

    AutoPtr<IInputStream> is;
    // try {
    ECode ec = mAtomicFile->OpenRead((IFileInputStream**)&is);
    if (FAILED(ec)) return;
    // } catch (FileNotFoundException ex) {
    //     return;
    // }

    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    AutoPtr<IBufferedInputStream> bis;
    ec = CBufferedInputStream::New(is, (IBufferedInputStream**)&bis);
    if (FAILED(ec)) goto _Exit_;
    ec = parser->SetInputEx(bis, String(NULL));
    if (FAILED(ec)) goto _Exit_;
    ec = LoadFromXml(parser);

_Exit_:
    if (ec == E_IO_EXCEPTION) {
        Slogger::W(TAG, "Failed to load display manager persistent store data.");
        ClearState();
    }
    else if (ec == E_XML_PULL_PARSER_EXCEPTION ) {
        Slogger::W(TAG, "Failed to load display manager persistent store data.");
        ClearState();
    }
    if (ICloseable::Probe(is)) {
        ICloseable::Probe(is)->Close();
    }
}

void PersistentDataStore::Save()
{
    AutoPtr<IFileOutputStream> os;
    AutoPtr<IFastXmlSerializer> serializer;
    AutoPtr<IBufferedOutputStream> bos;
    Boolean success = FALSE;
    FAIL_GOTO(mAtomicFile->StartWrite((IFileOutputStream**)&os), _Exit2_);
    FAIL_GOTO(CFastXmlSerializer::New((IFastXmlSerializer**)&serializer), _Exit1_);
    FAIL_GOTO(CBufferedOutputStream::New(os, (IBufferedOutputStream**)&bos), _Exit1_);
    FAIL_GOTO(serializer->SetOutput(bos, String("utf-8")), _Exit1_);
    FAIL_GOTO(SaveToXml(serializer), _Exit1_);
    FAIL_GOTO(serializer->Flush(), _Exit1_);
    success = TRUE;

_Exit1_:
    if (success) {
        FAIL_GOTO(mAtomicFile->FinishWrite(os), _Exit2_);
    }
    else {
        FAIL_GOTO(mAtomicFile->FailWrite(os), _Exit2_);
    }
    return;

_Exit2_:
    Slogger::W(TAG, "Failed to save display manager persistent store data.");
}

ECode PersistentDataStore::LoadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    FAIL_RETURN(XmlUtils::BeginDocument(parser, String("display-manager-state")));
    Int32 outerDepth;
    FAIL_RETURN(parser->GetDepth(&outerDepth));
    while (XmlUtils::NextElementWithin(parser, outerDepth)) {
        String name;
        FAIL_RETURN(parser->GetName(&name));
        if (name.Equals("remembered-wifi-displays")) {
            FAIL_RETURN(LoadRememberedWifiDisplaysFromXml(parser));
        }
    }
    return NOERROR;
}

ECode PersistentDataStore::LoadRememberedWifiDisplaysFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 outerDepth;
    FAIL_RETURN(parser->GetDepth(&outerDepth));
    while (XmlUtils::NextElementWithin(parser, outerDepth)) {
        String name;
        FAIL_RETURN(parser->GetName(&name));
        if (name.Equals("wifi-display")) {
            String deviceAddress;
            FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("deviceAddress"), &deviceAddress));
            String deviceName;
            FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("deviceName"), &deviceName));
            String deviceAlias;
            FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("deviceAlias"), &deviceAlias));
            if (deviceAddress.IsNull() || deviceName.IsNull()) {
                Slogger::E(TAG, "Missing deviceAddress or deviceName attribute on wifi-display.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            if (FindRememberedWifiDisplay(deviceAddress) != mRememberedWifiDisplays.End()) {
                Slogger::E(TAG, "Found duplicate wifi display device address.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }

            AutoPtr<IWifiDisplay> wifiDisplay;
            FAIL_RETURN(CWifiDisplay::New(
                deviceAddress, deviceName, deviceAlias, (IWifiDisplay**)&wifiDisplay));
            mRememberedWifiDisplays.PushBack(wifiDisplay);
        }
    }
    return NOERROR;
}

ECode PersistentDataStore::SaveToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    AutoPtr<IBoolean> boolObj;
    CBoolean::New(TRUE, (IBoolean**)&boolObj);
    FAIL_RETURN(serializer->StartDocument(String(NULL), boolObj));
    FAIL_RETURN(serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE));
    FAIL_RETURN(serializer->WriteStartTag(String(NULL), String("display-manager-state")));
    FAIL_RETURN(serializer->WriteStartTag(String(NULL), String("remembered-wifi-displays")));
    List<AutoPtr<IWifiDisplay> >::Iterator iter = mRememberedWifiDisplays.Begin();
    for (; iter != mRememberedWifiDisplays.End(); ++iter) {
        AutoPtr<IWifiDisplay> display = *iter;
        FAIL_RETURN(serializer->WriteStartTag(String(NULL), String("wifi-display")));
        String deviceAddress;
        display->GetDeviceAddress(&deviceAddress);
        FAIL_RETURN(serializer->WriteAttribute(String(NULL), String("deviceAddress"), deviceAddress));
        String deviceName;
        display->GetDeviceName(&deviceName);
        FAIL_RETURN(serializer->WriteAttribute(String(NULL), String("deviceName"), deviceName));
        String deviceAlias;
        display->GetDeviceAlias(&deviceAlias);
        if (!deviceAlias.IsNull()) {
            FAIL_RETURN(serializer->WriteAttribute(String(NULL), String("deviceAlias"), deviceAlias));
        }
        FAIL_RETURN(serializer->WriteEndTag(String(NULL), String("wifi-display")));
    }
    FAIL_RETURN(serializer->WriteEndTag(String(NULL), String("remembered-wifi-displays")));
    FAIL_RETURN(serializer->WriteEndTag(String(NULL), String("display-manager-state")));
    FAIL_RETURN(serializer->EndDocument());
    return NOERROR;
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
