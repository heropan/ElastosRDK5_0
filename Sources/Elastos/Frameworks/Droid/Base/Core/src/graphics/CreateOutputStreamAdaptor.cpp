#include "graphics/CreateOutputStreamAdaptor.h"

using Elastos::IO::IFlushable;

namespace Elastos {
namespace Droid {
namespace Graphics {

class InputStreamAdaptor : public SkStream
{
public:
    InputStreamAdaptor(
        /* [in] */ IInputStream* is,
        /* [in] */ ArrayOf<Byte>* ar)
        : mInputStream(is)
        , mByteArray(ar)
    {
        SkASSERT(ar);
        mCapacity = ar->GetLength();
        SkASSERT(mCapacity > 0);
        mBytesRead = 0;
    }

    virtual bool rewind()
    {
        mBytesRead = 0;

        ECode ec = mInputStream->Reset();
        if (FAILED(ec)) {
            SkDebugf("------- reset threw an exception\n");
            return false;
        }
        return true;
    }

    size_t doRead(void* buffer, size_t size)
    {
        size_t bytesRead = 0;
        // read the bytes
        do {
            size_t requested = size;
            if (requested > mCapacity) {
                requested = mCapacity;
            }

            Int32 n;
            ECode ec = mInputStream->ReadBytesEx(mByteArray, 0, requested, &n);
            if (FAILED(ec)) {
                SkDebugf("---- read threw an exception\n");
                return 0;
            }

            if (n < 0) { // n == 0 should not be possible, see InputStream read() specifications.
                break;  // eof
            }

            memcpy(buffer, mByteArray->GetPayload(), n);

            buffer = (void*)((char*)buffer + n);
            bytesRead += n;
            size -= n;
            mBytesRead += n;
        } while (size != 0);

        return bytesRead;
    }

    size_t doSkip(size_t size)
    {
        Int64 skipped;
        ECode ec = mInputStream->Skip((Int64)size, &skipped);
        if (FAILED(ec)) {
            SkDebugf("------- skip threw an exception\n");
            return 0;
        }
        if (skipped < 0) {
            skipped = 0;
        }

        return (size_t)skipped;
    }

    size_t doSize()
    {
        Int32 avail;
        ECode ec = mInputStream->Available(&avail);
        if (FAILED(ec)) {
            SkDebugf("------- available threw an exception\n");
            avail = 0;
        }
        return avail;
    }

    virtual size_t read(void* buffer, size_t size)
    {
        if (NULL == buffer) {
            if (0 == size) {
                return this->doSize();
            }
            else {
                /*  InputStream.skip(n) can return <=0 but still not be at EOF
                    If we see that value, we need to call read(), which will
                    block if waiting for more data, or return -1 at EOF
                 */
                size_t amountSkipped = 0;
                do {
                    size_t amount = this->doSkip(size - amountSkipped);
                    if (0 == amount) {
                        char tmp;
                        amount = this->doRead(&tmp, 1);
                        if (0 == amount) {
                            // if read returned 0, we're at EOF
                            break;
                        }
                    }
                    amountSkipped += amount;
                } while (amountSkipped < size);
                return amountSkipped;
            }
        }
        return this->doRead(buffer, size);
    }

private:
    AutoPtr<IInputStream> mInputStream;
    AutoPtr< ArrayOf<Byte> > mByteArray;     // the caller owns this object
    size_t mCapacity;
    size_t mBytesRead;
};

SkStream* CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize)
{
    if (markSize) {
        stream->Mark(markSize);
    }
    return new InputStreamAdaptor(stream, storage);
}

class SkOutputStream : public SkWStream
{
public:
    SkOutputStream(
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage)
        : mOutputStream(stream)
        , mByteArray(storage)
        , mCapacity(storage->GetLength())
    {}

    virtual bool write(
        /* [in] */ const void* buffer,
        /* [in] */ size_t size)
    {
        ArrayOf<Byte>* storage = mByteArray;

        while (size > 0) {
            size_t requested = size;
            if (requested > (size_t)mCapacity) {
                requested = mCapacity;
            }

            memcpy(storage->GetPayload(), buffer, requested);

            ECode ec = mOutputStream->WriteBytesEx(*storage, 0, requested);
            if (FAILED(ec)) {
                SkDebugf("------- write threw an exception\n");
                return false;
            }

            buffer = (void*)((char*)buffer + requested);
            size -= requested;
        }
        return true;
    }

    virtual void flush()
    {
        assert(IFlushable::Probe(mOutputStream) != NULL);
        IFlushable::Probe(mOutputStream)->Flush();
    }

private:
    AutoPtr<IOutputStream> mOutputStream;  // the caller owns this object
    AutoPtr< ArrayOf<Byte> > mByteArray;     // the caller owns this object
    Int32 mCapacity;
};

SkWStream* CreateOutputStreamAdaptor(
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    return new SkOutputStream(stream, storage);
}

} // namespace Graphics
} // namespace Droid
} // namespace Elastos
