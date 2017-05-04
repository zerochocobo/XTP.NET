#pragma once
#include "stdafx.h"
//#include <msclr/marshal.h>
//#include "QuoteDll.h"
//#include <functional>
//#include "./xtpinclude/xtp_quote_api.h"
//#include <vcclr.h>
//#include <atlstr.h>
//using namespace msclr::interop;
//using namespace System;
//using namespace System::Runtime::InteropServices;
//using namespace std::placeholders; // for `_1`
/*
之前试过一些方法留存

//delegate void Del(int, float);

//class EventHandler
//{
//	public:
//		void addHandler(std::function<void(int)> callback)
//		{
//			//cout << "Handler added..." << endl;
//			// Let's pretend an event just occured
//			callback(1);
//		}
//};

//EventHandler *handler = new EventHandler();

//public class MyClass
//{
//public:
//	MyClass() {
//		private_x = 5;
//		handler->addHandler(std::bind(&MyClass::Callback, this, _1));
//	}
//
//	// Note: No longer marked `static`, and only takes the actual argument
//	void Callback(int x)
//	{
//		// No longer needs an explicit `instance` argument,
//		// as `this` is set up properly
//		//cout << x + private_x << endl;
//	}
//private:
//	int private_x;
//};


//public ref class QuoteSpiManagedClass {
//public:
//	// Allocate the native object on the C++ Heap via a constructor
//	QuoteSpiManagedClass() : m_Impl(new MyQuoteSpi) {}

//	// Deallocate the native object on a destructor
//	~QuoteSpiManagedClass() {
//		delete m_Impl;
//	}

//protected:
//	// Deallocate the native object on the finalizer just in case no destructor is called
//	!QuoteSpiManagedClass() {
//		delete m_Impl;
//	}
//public:
//	/*property String ^  get_PropertyA {
//		String ^ get() {
//			return gcnew String(m_Impl->Get_PropertyA());
//		}
//	}

//	void OnDisconnected(int reason) {
//		//pin_ptr<const WCHAR> str = PtrToStringChars(theString);
//		m_Impl->OnDisconnected(reason);
//	}
//	void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
//		XTP_EXCHANGE_TYPE exchange = ticker->exchange_id;
//		char* t = ticker->ticker;
//		String^ s = "";
//		m_Impl->OnSubMarketData(ticker, error_info, is_last);
//		//OnMyUnSubMarketData(1);
//	}
//	MyQuoteSpi * m_Impl;

//};

*/


//public ref class MQuoteSpi:QuoteSpi
//{
//	public:
//		void OnDisconnected(int reason) {};
//		void OnError(XTPRI error_info) {};
//		void OnSubMarketData(XTPRspInfoStruct ticker, XTPRI error_info, bool is_last) {};
//		void OnUnSubMarketData(XTPST ticker, XTPRI error_info, bool is_last) {};
//		void OnMarketData(XTPMD market_data) {};
//		void OnQueryAllTickers() {};
//		/*private:
//		QuoteSpi *qspi*/
//};

/*public ref class MyNamesSplitterClass
{
private:
System::String ^_FName, ^_LName;
public:
MyNamesSplitterClass(System::String ^FullName)
{
int pos = FullName->IndexOf(" ");
if (pos < 0)
throw gcnew System::Exception("Invalid full name!");
_FName = FullName->Substring(0, pos);
_LName = FullName->Substring(pos + 1, FullName->Length - pos - 1);
}

//定义out
//void foo([Out] Bar^% x);

void Print()
{
Console::WriteLine("First name: {0}\nLastName: {1}", _FName, _LName);
}
};*/
/*XTP::API::QuoteApi(Bridge _NativeCPlus)
{
_CSPtr->InfoChangeEvent += gcnew EventHandler<CSharpClass::EventHandler^>(this, &Cli::function);
}

Cli::function()
{
__raise NativeCPlus->OnEventInvoke(_Callbackfunction);
}*/
/*class MngdAdapter;

class QuoteBridge
{
private:
MngdAdapter* adapter;

public:
QuoteBridge();
~QuoteBridge();

void QuoteBridge::SetValue(int value);
int QuoteBridge::GetValue();

void PrintMessage(const char* sMsg);
};*/


// interface that has an event and a function to invoke the event  
/*interface struct I {
public:
event Del ^ E;
void fire(int, float);
};
*/
/*delegate void OnDisconnectedHandler(int, double);
delegate void OnSubMarketDataHandler(int);*/
