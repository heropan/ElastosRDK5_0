
#include "am/ProcessList.h"
#include "am/CActivityManagerService.h"
#include "wm/CWindowManagerService.h"
#include <elastos/Slogger.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::View::IDisplay;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {


ProcessList::ProcessList()
{
    Int32 oomAdj[] = {
            FOREGROUND_APP_ADJ, VISIBLE_APP_ADJ, PERCEPTIBLE_APP_ADJ,
            BACKUP_APP_ADJ, HIDDEN_APP_MIN_ADJ, HIDDEN_APP_MAX_ADJ
    };
    Int32 size = ARRAY_SIZE(oomAdj);
    mOomAdj = ArrayOf<Int32>::Alloc(size);
    mOomAdj->Copy(oomAdj, size);


    Int64 oomMinFreeLow[] = {
            8192, 12288, 16384,
            24576, 28672, 32768
    };
    size = ARRAY_SIZE(oomMinFreeLow);
    mOomMinFreeLow = ArrayOf<Int64>::Alloc(size);
    mOomMinFreeLow->Copy(oomMinFreeLow, size);


    Int64 oomMinFreeHigh[] =  {
            32768, 40960, 49152,
            57344, 65536, 81920
    };
    size = ARRAY_SIZE(oomMinFreeHigh);
    mOomMinFreeHigh = ArrayOf<Int64>::Alloc(size);
    mOomMinFreeHigh->Copy(oomMinFreeHigh, size);

    mOomMinFree = ArrayOf<Int64>::Alloc(mOomAdj->GetLength());

    // TODO
    // MemInfoReader minfo = new MemInfoReader();
    // minfo.readMemInfo();
    // mTotalMemMb = minfo.getTotalSize()/(1024*1024);
    // updateOomLevels(0, 0, FALSE);
}

ECode ProcessList::ApplyDisplaySize(
    /* [in] */ CWindowManagerService*  wm)
{
    if (!mHaveDisplaySize) {
        AutoPtr<IPoint> p;
        wm->GetInitialDisplaySize(IDisplay::DEFAULT_DISPLAY, (IPoint**)&p);
        Int32 x, y;
        p->GetX(&x);
        p->GetY(&y);
        if (x != 0 && y != 0) {
            UpdateOomLevels(x, y, TRUE);
            mHaveDisplaySize = TRUE;
        }
    }
    return NOERROR;
}

ECode ProcessList::UpdateOomLevels(
    /* [in] */ Int32 displayWidth,
    /* [in] */ Int32 displayHeight,
    /* [in] */ Boolean write)
{
    // Scale buckets from avail memory: at 300MB we use the lowest values to
    // 700MB or more for the top values.
    Float scaleMem = ((Float)(mTotalMemMb-300))/(700-300);

    // Scale buckets from screen size.
    Int32 minSize = 320*480;  //  153600
    Int32 maxSize = 1280*800; // 1024000  230400 870400  .264
    Float scaleDisp = ((Float)(displayWidth*displayHeight)-minSize)/(maxSize-minSize);
    //Slog.i("XXXXXX", "scaleDisp=" + scaleDisp + " dw=" + displayWidth + " dh=" + displayHeight);

    StringBuilder adjString;
    StringBuilder memString;

    Float scale = scaleMem > scaleDisp ? scaleMem : scaleDisp;
    if (scale < 0) scale = 0;
    else if (scale > 1) scale = 1;

    for (int i=0; i<mOomAdj->GetLength(); i++) {
        Int64 low = (*mOomMinFreeLow)[i];
        Int64 high = (*mOomMinFreeHigh)[i];
        (*mOomMinFree)[i] = (Int64)(low + ((high-low)*scale));

        if (i > 0) {
            adjString += (',');
            memString += (',');
        }
        adjString += ((*mOomAdj)[i]);
        memString += (((*mOomMinFree)[i]*1024)/PAGE_SIZE);
    }

    //Slog.i("XXXXXXX", "******************************* MINFREE: " + memString);
    if (write) {
        WriteFile(String("/sys/module/lowmemorykiller/parameters/adj"), adjString.ToString());
        WriteFile(String("/sys/module/lowmemorykiller/parameters/minfree"), memString.ToString());
    }
    // GB: 2048,3072,4096,6144,7168,8192
    // HC: 8192,10240,12288,14336,16384,20480
    return NOERROR;
}

Int64 ProcessList::GetMemLevel(
    /* [in] */ Int32 adjustment)
{
    for (int i=0; i<mOomAdj->GetLength(); i++) {
        if (adjustment <= (*mOomAdj)[i]) {
            return (*mOomMinFree)[i] * 1024;
        }
    }
    return (*mOomMinFree)[mOomAdj->GetLength()-1] * 1024;
}

ECode ProcessList::WriteFile(
    /* [in] */ const String& path,
    /* [in] */ const String& data)
{
    AutoPtr<IFileOutputStream> fos;
//    try {
    FAIL_RETURN(CFileOutputStream::New(path, (IFileOutputStream**)&fos));

    AutoPtr<ArrayOf<Byte> > datas = ArrayOf<Byte>::Alloc(data.GetByteLength());
    datas->Copy((Byte*)(data.string()), data.GetByteLength());
    fos->WriteBytes(*(datas.Get()));
//    } catch (IOException e) {
    Slogger::W(CActivityManagerService::TAG, "Unable to write %s", path.string());
//    } finally {
    if (fos != NULL) {
//    try {
    fos->Close();
//    } catch (IOException e) {
//    }
    }
//    }
    return NOERROR;
}

String ProcessList::ToString()
{
    return String("ProcessList");
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
