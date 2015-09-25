
#ifndef ASYNCHRONOUS_CLOSE_MONITOR_H_included
#define ASYNCHRONOUS_CLOSE_MONITOR_H_included

#include <pthread.h>

/**
 * AsynchronousCloseMonitor helps implement Java's asynchronous close semantics.
 *
 * AsynchronousCloseMonitor::init must be called before anything else.
 *
 * Every blocking I/O operation must be surrounded by an AsynchronousCloseMonitor
 * instance. For example:
 *
 *   {
 *     AsynchronousCloseMonitor monitor(fd);
 *     byteCount = ::read(fd, buf, sizeof(buf));
 *   }
 *
 * To interrupt all threads currently blocked on file descriptor 'fd', call signalBlockedThreads:
 *
 *   AsynchronousCloseMonitor::signalBlockedThreads(fd);
 *
 * To test to see if the interruption was due to the signalBlockedThreads call:
 *
 *   monitor.wasSignaled();
 */
class AsynchronousCloseMonitor
{
public:
    AsynchronousCloseMonitor(int fd);
    ~AsynchronousCloseMonitor();
    bool wasSignaled() const;

    static void init();

    static void signalBlockedThreads(int fd);

private:
    AsynchronousCloseMonitor* mPrev;
    AsynchronousCloseMonitor* mNext;
    pthread_t mThread;
    int mFd;
    bool mSignaled;

    // Disallow copy and assignment.
    AsynchronousCloseMonitor(const AsynchronousCloseMonitor&);
    void operator=(const AsynchronousCloseMonitor&);
};

#endif  // ASYNCHRONOUS_CLOSE_MONITOR_H_included
