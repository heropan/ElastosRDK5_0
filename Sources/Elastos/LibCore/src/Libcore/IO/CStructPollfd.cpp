
#include "cmdef.h"
#include "CStructPollfd.h"

namespace Libcore {
namespace IO {

ECode CStructPollfd::GetFd(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    *fd = mFd;
    return NOERROR;
}

ECode CStructPollfd::SetFd(
    /* [in] */ Int32 fd)
{
    mFd = fd;
    return NOERROR;
}

ECode CStructPollfd::GetEvents(
    /* [out] */ Int16* events)
{
    VALIDATE_NOT_NULL(events);

    *events = mEvents;
    return NOERROR;
}

ECode CStructPollfd::SetEvents(
    /* [in] */ Int16 events)
{
    mEvents = events;
    return NOERROR;
}

ECode CStructPollfd::GetRevents(
    /* [out] */ Int16* revents)
{
    VALIDATE_NOT_NULL(revents);

    *revents = mRevents;
    return NOERROR;
}

ECode CStructPollfd::SetRevents(
    /* [in] */ Int16 revents)
{
    mRevents = revents;
    return NOERROR;
}

ECode CStructPollfd::GetUserData(
    /* [out] */ IInterface** userData)
{
    VALIDATE_NOT_NULL(userData);

    *userData = mUserData;
    INTERFACE_ADDREF(*userData);
    return NOERROR;
}

ECode CStructPollfd::SetUserData(
    /* [in] */ IInterface* userData)
{
    mUserData = userData;
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
