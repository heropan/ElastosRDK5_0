//./Elastos/LibCore/inc/Elastos/IO/Buffer.h
//put this file at the end of the include list
//so the type definition used in this file will be found
#ifndef ELASTOS_BYTEBUFFER_CALLBACK_DEC_HH
#define ELASTOS_BYTEBUFFER_CALLBACK_DEC_HH

struct ElaByteBufferCallback
{
    void* (*elastos_ByteBuffer_GetDirectBufferAddress)(IInterface* obj);
    Int64 (*elastos_ByteBuffer_GetDirectBufferCapacity)(IInterface* obj);
};


#endif //
