
#include "am/DeviceMonitor.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Core::ISystem;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const String DeviceMonitor::TAG("DeviceMonitor");
const Int32 DeviceMonitor::SAMPLE_COUNT;
const Int32 DeviceMonitor::INTERVAL;
const Int32 DeviceMonitor::MAX_FILES;

ECode DeviceMonitor::MonitorThread::Run()
{
    mHost->Monitor();
    return NOERROR;
}

DeviceMonitor::DeviceMonitor()
    : mRunning(FALSE)
{
    AutoPtr<ArrayOf<Byte> > mBuffer = ArrayOf<Byte>::Alloc(1024);

    AutoPtr<IThread> thread = new MonitorThread(this);
    thread->Start();
}

void DeviceMonitor::Init()
{
    CFile::New(String("/proc"), (IFile**)&PROC);
    CFile::New(String("/data/anr/"), (IFile**)&BASE);

    AutoPtr<IFile> zFile, iFile, mFile, sFile;
    CFile::New(PROC, String("zoneinfo"), (IFile**)&zFile);
    CFile::New(PROC, String("interrupts"), (IFile**)&iFile);
    CFile::New(PROC, String("meminfo"), (IFile**)&mFile);
    CFile::New(PROC, String("slabinfo"), (IFile**)&sFile);
    PATHS = ArrayOf<IFile*>::Alloc(4);
    PATHS->Set(0, zFile);
    PATHS->Set(1, iFile);
    PATHS->Set(2, mFile);
    PATHS->Set(3, sFile);

    instance= new DeviceMonitor();
}

/**
 * Loops continuously. Pauses until someone tells us to start monitoring.
 */
//@SuppressWarnings("InfiniteLoopStatement")
void DeviceMonitor::Monitor()
{
    while (TRUE) {
        WaitForStart();

        Purge();

        for (Int32 i = 0; i < SAMPLE_COUNT; i++) {
            // try {
            Dump();
            // } catch (IOException e) {
            //     Slog.w(TAG, "Dump failed.", e);
            // }
            Pause();
        }

        Stop();
    }
}

/**
 * Deletes old files.
 */
void DeviceMonitor::Purge()
{
    AutoPtr<ArrayOf<IFile*> > files;
    BASE->ListFiles((ArrayOf<IFile*>**)&files);
    Int32 count = files->GetLength() - MAX_FILES;
    if (count > 0) {
    // TODO:
    //     Arrays.sort(files);
        for (Int32 i = 0; i < count; i++) {
            Boolean deleted;
            (*files)[i]->Delete(&deleted);
            if (!deleted) {
                Slogger::W(TAG, "Couldn't delete %p.", (*files)[i]);
            }
        }
    }
}

/**
 * Dumps the current device stats to a new file.
 */
void DeviceMonitor::Dump()
{
    AutoPtr<IFile> file;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    CFile::New(BASE, StringUtils::Int64ToString(millis), (IFile**)&file);
    AutoPtr<IFileOutputStream> fout;
    CFileOutputStream::New(file, (IFileOutputStream**)&fout);
    AutoPtr<IOutputStream> out = IOutputStream::Probe(fout);
    // try {
        // Copy /proc/*/stat
    AutoPtr<ArrayOf<IFile*> > directs;
    PROC->ListFiles((ArrayOf<IFile*>**)&directs);
    for (Int32 i = 0; i < directs->GetLength(); ++i) {
    AutoPtr<IFile> processDirectory = (*directs)[i];
        if (IsProcessDirectory(processDirectory)) {
            AutoPtr<IFile> pFile;
            CFile::New(processDirectory, String("stat"), (IFile**)&pFile);
            Dump(pFile, out);
        }
    }

    // Copy other files.
    for (Int32 j = 0; j < PATHS->GetLength(); ++j) {
        AutoPtr<IFile> file = (*PATHS)[j];
        Dump(file, out);
    }
    // } finally {
    CloseQuietly(out);
    // }
}

/**
 * Returns true if the given file represents a process directory.
 */
Boolean DeviceMonitor::IsProcessDirectory(
    /* [in] */ IFile* file)
{
    // try {
    String name;
    file->GetName(&name);
    Int32 value;
    ECode ec = StringUtils::ParseInt32(name, &value);
    if (FAILED(ec)) return FALSE;
    Boolean isDes = FALSE;
    file->IsDirectory(&isDes);
    return isDes;
    // } catch (NumberFormatException e) {
    //     return false;
    // }
}

/**
 * Copies from a file to an output stream.
 */
void DeviceMonitor::Dump(
    /* [in] */ IFile* from,
    /* [in] */ IOutputStream* out)
{
    WriteHeader(from, out);

    AutoPtr<IFileInputStream> in;
    // try {
    CFileInputStream::New(from, (IFileInputStream**)&in);
    Int32 count;
    while (in->ReadBytes(mBuffer, &count), count != -1) {
        out->WriteBytes(*mBuffer, 0, count);
    }
    // } finally {
    CloseQuietly(in);
    // }
}

/**
 * Writes a header for the given file.
 */
void DeviceMonitor::WriteHeader(
    /* [in] */ IFile* file,
    /* [in] */ IOutputStream* out)
{
    String fStr;
    file->ToString(&fStr);
    String header = String("*** ") + fStr + "\n";
    // TODO:
    // out->WriteBytes(header.getBytes());
}

/**
 * Closes the given resource. Logs exceptions.
 * @param closeable
 */
void DeviceMonitor::CloseQuietly(
    /* [in] */ ICloseable* closeable)
{
    // try {
    if (closeable != NULL) {
        closeable->Close();
    }
    // } catch (IOException e) {
    //     Slog.w(TAG, e);
    // }
}

void DeviceMonitor::Pause()
{
    // try {
    Thread::Sleep(INTERVAL);
    // } catch (InterruptedException e) { /* ignore */ }
}

void DeviceMonitor::Stop()
{
    Object::Autolock lock(mLock);
    mRunning = FALSE;
}

void DeviceMonitor::WaitForStart()
{
    Object::Autolock lock(mLock);
    while (!mRunning) {
        // try {
        mLock.Wait();
        // } catch (InterruptedException e) { /* ignore */ }
    }
}

void DeviceMonitor::StartMonitoring()
{
    Object::Autolock lock(mLock);
    if (!mRunning) {
        mRunning = TRUE;
        mLock.NotifyAll();
    }
}

/**
 * Starts monitoring if it hasn't started already.
 */
void DeviceMonitor::Start()
{
    instance->StartMonitoring();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
