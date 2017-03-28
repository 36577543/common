/******************************************************************************
* ��Ȩ���� (C)2009, Sandongcun������
*
* �ļ����ƣ�single.h
* ����ժҪ����������(���̰߳�ȫ)
* ����˵����ע���ڳ���ʹ��ʱʱ�����ϴ��ڳ�ͻ�Ŀ��ܣ�ʹ�ô˵��������ڵ�һ�߳��г�
*           ʼ��
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2009.6.  
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

    // ����ȫ�ֶ���ָ���ڶ���ģ���д���
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