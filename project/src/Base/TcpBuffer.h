/******************************************************************************
Copyright (C) 2015 zhuyanxie

　　Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

　　The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


#ifndef _DAHUA_MOBILE_TCP_BUFFER_H_
#define _DAHUA_MOBILE_TCP_BUFFER_H_

namespace ls {

class CTcpBuffer
{
public:
    CTcpBuffer(uint32_t size)
		: m_buf(new char[size])
        , m_end(0)
        , m_size(size)
    {
    }

    ~CTcpBuffer()
    {
        if (m_buf)
        {
            delete m_buf;
            m_buf = NULL;
        }
    }

    bool putBuffer(uint8_t *buf, uint32_t len)
    {
        if (len > m_size - m_end)
        {
            return false;
        }

        memcpy(m_buf + m_end, buf, len);
        m_end += len;
        return true;
    }

    void addSize(uint32_t len)
    {
        m_end += len;
    }

    char *getBuffer()
    {
        return m_buf;
    }

    char *getBufferEnd()
    {
        return m_buf + m_end;
    }

    bool popBuffer(uint32_t len)
    {
        if (len == 0)
        {
            return true;
        }

        if (len > m_end)
        {
            return false;
        }

        memmove(m_buf, m_buf + len, m_end - len);
        m_end -= len;
        return true;
    }

    uint32_t size()
    {
        return m_end;
    }

    uint32_t getIdleSize()
    {
        return m_size - m_end;
    }

    bool clear()
    {
    	m_end = 0;
    	return true;
    }

private:
    char                *m_buf;
    uint32_t            m_end;
    uint32_t            m_size;
};

}

#endif /* _DAHUA_MOBILE_TCP_BUFFER_H_ */
