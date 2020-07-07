#include <stdio.h>

typedef unsigned int (*handleMessage)(void*, void*, void*);

class CTestHandler 
{
public:
		CTestHandler(unsigned int uiMessageId){ m_uiMessageId=uiMessageId;  };
		~CTestHandler(){};
public:	
		unsigned int m_uiMessageId;
		handleMessage pHandleMessage;
		
};


class CTest1Handler :CTestHandler
{
	    //CTest1Handler(){};
public:
		CTest1Handler(unsigned int uiMessageId):CTestHandler(uiMessageId)
		{

            pHandleMessage= (handleMessage)& Handle;
			
		};
		~CTest1Handler(){};
		
		virtual unsigned int Handle(int*,void *,void*)=0;
		
};

class CTest2Handler :CTestHandler
{
	//CTest2Handler(){};
public:
		CTest2Handler(unsigned int uiMessageId):CTestHandler(uiMessageId)
		{ 
            pHandleMessage= (handleMessage) &Handle;
			
		};
		~CTest2Handler(){};
		
		virtual unsigned int Handle(char*,void *,void*)=0;
		
};

class CTest11Handler :public CTest1Handler
{
public:
		CTest11Handler(unsigned int uiMessageId):CTest1Handler(uiMessageId)
		{ }
		virtual unsigned int Handle(int*,void *,void*)
		{
			printf("this is CTest11Handler\n");
		}
};

class CTest22Handler :public CTest2Handler
{
public:
		CTest22Handler(unsigned int uiMessageId):CTest2Handler(uiMessageId)
		{ }
		virtual unsigned int Handle(char*,void *,void*)
		{
			printf("this is CTest22Handler\n");
		}
};

int main(int argc, char **argv)
{
	int aa=1;
	char *bb="bb";
	CTest1Handler *pHandler1= new CTest11Handler(11);
	CTest2Handler *pHandler2= new CTest22Handler(22);
	pHandler1->pHandleMessage(&aa,NULL,NULL);
    pHandler2->pHandleMessage(bb,NULL,NULL);
	return 0;
}
