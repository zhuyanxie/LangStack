
#include "base/MemoryStream.h"

namespace ls {

MemoryIOS::MemoryIOS(char* pBuffer, std::streamsize bufferSize):
    m_buf(pBuffer, bufferSize)
{
	init(&m_buf);
}


MemoryIOS::~MemoryIOS()
{
}


MemoryInputStream::MemoryInputStream(const char* pBuffer, std::streamsize bufferSize):
    MemoryIOS(const_cast<char*>(pBuffer), bufferSize),
    std::istream(&m_buf)
{
}


MemoryInputStream::~MemoryInputStream()
{
}


MemoryOutputStream::MemoryOutputStream(char* pBuffer, std::streamsize bufferSize):
    MemoryIOS(pBuffer, bufferSize),
    std::ostream(&m_buf)
{
}


MemoryOutputStream::~MemoryOutputStream()
{
}

}

