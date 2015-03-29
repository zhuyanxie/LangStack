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


#ifndef METHODTEST_H_
#define METHODTEST_H_

namespace ls {
namespace test {

/**
 * 三个保留关键方法名：1、getClassName(), 2、attach(), 3、detach()
 */
class CMethodTest
{
public:
    CMethodTest(int a);
    ~CMethodTest();

    ///\brief   getClassName保留关键方法名，生成对应的java方法名[包名].[类名]
    const char* getClassName() {
        return "com.ls.test.MethodTest";
    }

    ///\brief   attach保留，注销回调
    void attach(ICallBack *callback);

    ///\brief   detach保留，注销回调
    void detach(ICallBack *callback);
};

}
}


#endif /* METHODTEST_H_ */
