#pragma once
///公用的结构
using namespace System;

namespace XTP {
	namespace API {

		/// <summary>
		/// XTP_PROTOCOL_TYPE 是一个通讯传输协议方式
		/// </summary>
		public enum class PROTOCOL_TYPE
		{
			XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
			XTP_PROTOCOL_UDP		///<采用UDP方式传输
		};

		public ref struct RspInfoStruct
		{
			///错误代码
			int	error_id;
			///错误信息
			String^	error_msg;
		};

		///指定的合约
		public ref struct SpecificTickerStruct
		{
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息）例如"600000  "
			String^	ticker;
		};


	}
}