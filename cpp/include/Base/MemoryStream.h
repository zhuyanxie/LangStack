
#ifndef _LANGUAGE_STACK_MEMORY_STREAM_H_
#define _LANGUAGE_STACK_MEMORY_STREAM_H_

#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include "Defs.h"

namespace ls {

///\brief 继承std的basic_streambuf， 使用外部传入的buffer作为缓存
template <typename ch, typename tr>
class LS_EXPORT BasicMemoryStreamBuf : public std::basic_streambuf<ch, tr>
{
protected:
    typedef std::basic_streambuf<ch, tr> Base;
    typedef std::basic_ios<ch, tr> IOS;
    typedef ch char_type;
    typedef tr char_traits;
    typedef typename Base::int_type int_type;
    typedef typename Base::pos_type pos_type;
    typedef typename Base::off_type off_type;

public:
    BasicMemoryStreamBuf(char_type* pBuffer, std::streamsize bufferSize):
        m_buffer(pBuffer),
        m_bufferSize(bufferSize)
    {
        this->setg(m_buffer, m_buffer, m_buffer + m_bufferSize);
        this->setp(m_buffer, m_buffer + m_bufferSize);
    }

    ~BasicMemoryStreamBuf()
    {
    }

    ///\brief 获取已使用内存长度
    std::streamsize charsWritten() const
    {
        return static_cast<std::streamsize>(this->pptr() - this->pbase());
    }

    ///\brief 重置
    void reset()
    {
        this->setg(m_buffer, m_buffer, m_buffer + m_bufferSize);
        this->setp(m_buffer, m_buffer + m_bufferSize);
    }

private:
    char_type*          m_buffer;       ///< 缓存地址
    std::streamsize     m_bufferSize;   ///< 缓存长度

    BasicMemoryStreamBuf();
    BasicMemoryStreamBuf(const BasicMemoryStreamBuf&);
    BasicMemoryStreamBuf& operator = (const BasicMemoryStreamBuf&);
};

///\brief 缓存类型定义
typedef BasicMemoryStreamBuf<char, std::char_traits<char> > MemoryStreamBuf;

///\brief 内存输入输出流基类， 实际缓存
class LS_EXPORT MemoryIOS: public virtual std::ios
{
public:
    MemoryIOS(char* pBuffer, std::streamsize bufferSize);
    ~MemoryIOS();

    ///\brief 返回所在缓存指针
    MemoryStreamBuf* rdbuf();

protected:
    MemoryStreamBuf     m_buf;      ///< 缓存
};

///\brief 内存输入流
class LS_EXPORT MemoryInputStream: public MemoryIOS, public std::istream
{
public:
    MemoryInputStream(const char* pBuffer, std::streamsize bufferSize);
    ~MemoryInputStream();
};

///\brief 内存输出流
class LS_EXPORT MemoryOutputStream: public MemoryIOS, public std::ostream
{
public:
    MemoryOutputStream(char* pBuffer, std::streamsize bufferSize);
    ~MemoryOutputStream();

    std::streamsize charsWritten() const;
};

//
// inlines
//
inline MemoryStreamBuf* MemoryIOS::rdbuf()
{
    return &m_buf;
}

inline std::streamsize MemoryOutputStream::charsWritten() const
{
    return m_buf.charsWritten();
}

}
#endif /* MEMORYSTREAM_H_ */
