
#include "elastos/droid/graphics/CreateOutputStreamAdaptor.h"

using Elastos::IO::IFlushable;

namespace Elastos {
namespace Droid {
namespace Graphics {

class InputStreamAdaptor : public SkStream {
public:
    InputStreamAdaptor(
        /* [in] */ IInputStream* js,
        /* [in] */ ArrayOf<Byte>* ar)
        : mInputStream(js)
        , mByteArray(ar)
    {
        SkASSERT(ar);
        mCapacity = ar->GetLength();
        SkASSERT(mCapacity > 0);
        mBytesRead = 0;
        mIsAtEnd = false;
    }

    virtual size_t read(
        /* [in] */ void* buffer,
        /* [in] */ size_t size)
    {
        if (NULL == buffer) {
            if (0 == size) {
                return 0;
            } else {
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
                            mIsAtEnd = true;
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

    virtual bool isAtEnd() const
    {
        return mIsAtEnd;
    }

private:
    size_t doRead(
        /* [in] */ void* buffer,
        /* [in] */ size_t size)
    {
        size_t bytesRead = 0;
        // read the bytes
        do {
            Int32 requested = 0;
            if (size > static_cast<size_t>(mCapacity)) {
                requested = mCapacity;
            } else {
                // This is safe because requested is clamped to (jint)
                // mCapacity.
                requested = static_cast<Int32>(size);
            }

            Int32 n = 0;
            ECode ec = mInputStream->Read(mByteArray, 0, requested, &n);
            if (FAILED(ec)) {
                SkDebugf("---- read threw an exception\n");
                // Consider the stream to be at the end, since there was an error.
                mIsAtEnd = true;
                return 0;
            }

            if (n < 0) { // n == 0 should not be possible, see InputStream read() specifications.
                mIsAtEnd = true;
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

    size_t doSkip(
        /* [in] */ size_t size)
    {
        Int64 skipped = 0;
        ECode ec = mInputStream->Skip(size, &skipped);
        if (FAILED(ec)) {
            SkDebugf("------- skip threw an exception\n");
            return 0;
        }

        if (skipped < 0) {
            skipped = 0;
        }

        return (size_t)skipped;
    }

private:
    AutoPtr<IInputStream> mInputStream;      // the caller owns this object
    AutoPtr< ArrayOf<Byte> > mByteArray;     // the caller owns this object
    size_t        mCapacity;
    size_t        mBytesRead;
    bool          mIsAtEnd;
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

SkStreamRewindable* CopyJavaInputStream(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    SkAutoTUnref<SkStream> adaptor(CreateInputStreamAdaptor(stream, storage));
    if (NULL == adaptor.get()) {
        return NULL;
    }
    assert(0 && "TODO: need jni codes.");
    // return adaptor_to_mem_stream(adaptor.get());
    return NULL;
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

            ECode ec = mOutputStream->Write(storage, 0, requested);
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
    assert(0 && "TODO: need jni codes.");
    // return new SkOutputStream(stream, storage);
    return NULL;
}

} // namespace Graphics
} // namespace Droid
} // namespace Elastos
