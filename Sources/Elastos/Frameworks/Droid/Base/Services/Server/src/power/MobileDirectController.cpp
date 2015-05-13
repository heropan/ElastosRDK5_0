
#include "power/MobileDirectController.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::IBufferedReader;
using Elastos::IO::CBufferedReader;
using Elastos::IO::IFileReader;
using Elastos::IO::CFileReader;
using Elastos::IO::IReader;
using Elastos::IO::ICloseable;
using Elastos::IO::IFileWriter;
using Elastos::IO::CFileWriter;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

const String MobileDirectController::TAG("MobileDirectController");
const Boolean MobileDirectController::DEBUG;
const String MobileDirectController::ENTWORK_ENABLE_PATH("/sys/class/sw_3g_module/modem/modem_power");
AutoPtr<MobileDirectController> MobileDirectController::sInstance;

AutoPtr<MobileDirectController> MobileDirectController::GetInstance()
{
    if(sInstance == NULL) {
        sInstance = new MobileDirectController();
    }
    return sInstance;
}

Boolean MobileDirectController::IsMobileModeAvailable()
{
    AutoPtr<IFile> file;
    CFile::New(ENTWORK_ENABLE_PATH, (IFile**)&file);
    Boolean isExist;
    file->Exists(&isExist);
    return isExist;
}

Boolean MobileDirectController::IsNetworkEnable()
{
    AutoPtr<IFile> file;
    CFile::New(ENTWORK_ENABLE_PATH, (IFile**)&file);
    AutoPtr<IBufferedReader> reader;
    // try {
    AutoPtr<IFileReader> fr;
    CFileReader::New(file, (IFileReader**)&fr);
    CBufferedReader::New(IReader::Probe(fr.Get()), (IBufferedReader**)&reader);
    String networkEnable;
    if (FAILED(reader->ReadLine(&networkEnable))) {
        goto failed;
    }
    if(!networkEnable.IsNull() && networkEnable.Equals("1")) {
        return TRUE;
    }
    // } catch (IOException e) {
    //     // TODO Auto-generated catch block
    //     e.printStackTrace();
    // } finally {
failed:
    // try {
    if(reader != NULL) ICloseable::Probe(reader.Get())->Close();
    // } catch (IOException e) {
    //     e.printStackTrace();
    // }
    // }
    return FALSE;
}

Boolean MobileDirectController::SetNetworkEnable(
    /* [in] */ Boolean enable)
{
    if(DEBUG) Slogger::D(TAG, "setNetworkEnable %d", enable);
    AutoPtr<IFile> file;
    CFile::New(ENTWORK_ENABLE_PATH, (IFile**)&file);
    AutoPtr<IFileWriter> writer;
    // try {
/*
        if(enable) {
             SystemProperties.set("ctl.start", "ril-daemon");
        } else {
             SystemProperties.set("ctl.stop", "ril-daemon");
             SystemProperties.set("ctl.stop", "pppd-gprs");
        }
*/
    CFileWriter::New(file, (IFileWriter**)&writer);
    // AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(1);
    if (FAILED(writer->WriteChars(enable? ArrayOf<Char32>((Char32*)"1", 1) : ArrayOf<Char32>((Char32*)"0", 1)))) {
        goto failed;
    }
    if (FAILED(ICloseable::Probe(writer.Get())->Close())) {
        return FALSE;
    }

    return TRUE;
    // } catch (IOException e) {
    //     e.printStackTrace();
    // } finally {
failed:
    if(writer != NULL) {
        // try {
        ICloseable::Probe(writer.Get())->Close();
        // } catch (IOException e) {
        //     e.printStackTrace();
        // }
    }
    // }
    return FALSE;
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
