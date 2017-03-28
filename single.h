/******************************************************************************
* 版权所有 (C)2009, Sandongcun开发组
*
* 文件名称：single.h
* 内容摘要：单件基类(非线程安全)
* 其它说明：注意在初次使用时时理论上存在冲突的可能，使用此单件类宜在单一线程中初
*           始化
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2009.6.  
******************************************************************************/
#ifndef _SINGLE_H
#define _SINGLE_H

template <class T>
class CSingle
{
public:
	static T *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance =  new T;
		}
		return m_pInstance;
	}

	static void FreeInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = 0;
		}
	}

    // 方便全局对象指针在独立模块中传递
    static void SetInstance(T *pInstance)
    {
        if(m_pInstance)
        {
            delete m_pInstance;
        }
        m_pInstance = pInstance;
    }
protected:
	CSingle(void){}
	~CSingle(void){}
//private:
	static T *m_pInstance;
};

#endif