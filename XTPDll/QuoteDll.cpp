// XTPDll.cpp: 主项目文件。
#pragma once
#include "stdafx.h"
#include <msclr/marshal.h>
#include "QuoteDll.h"
#include <functional>
#include "./xtpinclude/xtp_quote_api.h"
using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace XTP {
	namespace API {

		///中间行情事件，用来互通两个类
		public ref class QuoteEventSource
		{
		public:
			///公用事件和回调定义.公用的触发器，由事件源头触发这里，再转触发对C#曝露的类

			delegate void OnDisconnectedDelegate(int);
			delegate void OnErrorDelegate(RspInfoStruct^);
			delegate void OnSubMarketDataDelegate(RspInfoStruct^, SpecificTickerStruct^,bool);
			delegate void OnUnSubMarketDataDelegate(RspInfoStruct^, SpecificTickerStruct^, bool);
			delegate void OnMarketDataDelegate(MarketDataStruct^);
			delegate void OnQueryAllTickersDelegate(RspInfoStruct^, QuoteStaticInfoStruct^,  bool);
			event OnDisconnectedDelegate^ OnDisconnectedEvent;
			event OnErrorDelegate^ OnErrorEvent;
			event OnSubMarketDataDelegate^ OnSubMarketDataEvent;
			event OnUnSubMarketDataDelegate^ OnUnSubMarketDataEvent;
			event OnMarketDataDelegate ^OnMarketDataEvent;
			event OnQueryAllTickersDelegate ^OnQueryAllTickersEvent;

			void OnDisconnected(int reason) {
				OnDisconnectedEvent(reason);
			}
			void OnError(RspInfoStruct^ resp) {
				OnErrorEvent(resp);
			}
			void OnSubMarketData(RspInfoStruct^ resp, SpecificTickerStruct^ ticker,bool is_last) {
				//if (myEvent != nullptr)
				OnSubMarketDataEvent(resp,ticker,is_last);
			}
			void OnUnSubMarketData(RspInfoStruct^ resp, SpecificTickerStruct^ ticker, bool is_last) {
				OnUnSubMarketDataEvent(resp, ticker, is_last);
			}
			void OnMarketData(MarketDataStruct^ data) {
				OnMarketDataEvent(data);
			}
			void OnQueryAllTickers(RspInfoStruct^ resp, QuoteStaticInfoStruct^ ticker_info,bool is_last) {
				OnQueryAllTickersEvent(resp, ticker_info,is_last);
			};
		private:
			//MyOnSubMarketDataDelegate ^myEvent;
			//Object ^eventLock;
		};
		///全局事件,用来让两个类可以互通
		gcroot<QuoteEventSource^> quoteevent_managed = gcnew QuoteEventSource();

		///行情事件定义，因为是普通类（需要继承QuoteSpi）而非引用类，所以无法在内部定义delegate
		public class MyQuoteSpi:public QuoteSpi {
		public:
			#pragma region 实现QuoteSpi回调
			void OnError(XTPRI *error_info) {
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
				quoteevent_managed->OnError(resInfo);
				delete resInfo;
			}
			void OnDisconnected(int reason) {
				quoteevent_managed->OnDisconnected(reason);
			}
			void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
				SpecificTickerStruct^ ticketInfo = gcnew SpecificTickerStruct();
				ticketInfo->exchange_id = ticker->exchange_id;
				ticketInfo->ticker = gcnew String(ticker->ticker);
				quoteevent_managed->OnSubMarketData(resInfo,ticketInfo,is_last);
				delete resInfo;
				delete ticketInfo;
			}
			void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
				SpecificTickerStruct^ ticketInfo = gcnew SpecificTickerStruct();
				ticketInfo->exchange_id = ticker->exchange_id;
				ticketInfo->ticker = gcnew String(ticker->ticker);
				quoteevent_managed->OnUnSubMarketData(resInfo, ticketInfo, is_last);
				delete resInfo;
				delete ticketInfo;
			}
			void OnMarketData(XTPMD *market_data) {
				#pragma region 收到市场股价数据

				// 收到市场股价数据

				MarketDataStruct^ data = gcnew MarketDataStruct();
				// 代码
				///交易所代码
				data->exchange_id = market_data->exchange_id;
				///合约代码（不包含交易所信息）
				data->ticker = gcnew String(market_data->ticker);
				// 股票等价格
				///最新价
				data->last_price = market_data->last_price;
				///昨收盘
				data->pre_close_price = market_data->pre_close_price;
				///今开盘
				data->open_price = market_data->open_price;
				///最高价
				data->high_price = market_data->high_price;
				///最低价
				data->low_price = market_data->low_price;
				///今收盘
				data->close_price = market_data->close_price;

				// 期货等数据
				///昨持仓量（目前未填写）
				data->pre_open_interest = market_data->pre_open_interest;
				///持仓量（目前未填写）
				data->open_interest = market_data->open_interest;
				///上次结算价（目前未填写）
				data->pre_settlement_price = market_data->pre_settlement_price;
				///本次结算价（目前未填写）
				data->settlement_price = market_data->settlement_price;

				///涨停板价（目前未填写）
				data->upper_limit_price = market_data->upper_limit_price;
				///跌停板价（目前未填写）
				data->lower_limit_price = market_data->lower_limit_price;
				///昨虚实度（目前未填写）
				data->pre_delta = market_data->pre_delta;
				///今虚实度（目前未填写）
				data->curr_delta = market_data->curr_delta;

				/// 时间类
				data->data_time = market_data->data_time;

				// 量额数据
				///数量
				data->qty = market_data->qty;
				///成交金额
				data->turnover = market_data->turnover;
				///当日均价
				data->avg_price = market_data->avg_price;

				// 买卖盘
				///十档申买价
				array<double>^ bid = gcnew array<double>(sizeof(market_data->bid));
				for (int i = 0; i < sizeof(market_data->bid); i++) {
					bid[i] = market_data->bid[i];
				}
				data->bid = bid;
				///十档申卖价
				array<double>^	ask = gcnew array<double>(sizeof(market_data->ask));
				for (int i = 0; i < sizeof(market_data->ask); i++) {
					ask[i] = market_data->ask[i];
				}
				data->ask = ask;
				///十档申买量
				array<int64_t>^	bid_qty = gcnew array<int64_t>(sizeof(market_data->bid_qty));
				for (int i = 0; i < sizeof(market_data->bid_qty); i++) {
					bid_qty[i] = market_data->bid_qty[i];
				}
				data->bid_qty = bid_qty;
				///十档申卖量
				array<int64_t>^	ask_qty = gcnew array<int64_t>(sizeof(market_data->ask_qty));
				for (int i = 0; i < sizeof(market_data->ask_qty); i++) {
					ask_qty[i] = market_data->ask_qty[i];
				}
				data->ask_qty = ask_qty;

				// lts没有包含的数据（目前未填写）
				///成交笔数
				data->trades_count = market_data->trades_count;
				///当前交易状态说明
				data->ticker_status = gcnew String(market_data->ticker_status);
				///委托买入总量
				data->total_bid_qty = market_data->total_bid_qty;
				///委托卖出总量
				data->total_ask_qty = market_data->total_ask_qty;
				///加权平均委买价格
				data->ma_bid_price = market_data->ma_bid_price;
				///加权平均委卖价格
				data->ma_ask_price = market_data->ma_ask_price;
				///债券加权平均委买价格
				data->ma_bond_bid_price = market_data->ma_bond_bid_price;
				///债券加权平均委卖价格
				data->ma_bond_ask_price = market_data->ma_bond_ask_price;
				///债券到期收益率
				data->yield_to_maturity = market_data->yield_to_maturity;
				///ETF净值估值
				data->iopv = market_data->iopv;
				///ETF申购笔数
				data->etf_buy_count = market_data->etf_buy_count;
				///ETF赎回笔数
				data->etf_sell_count = market_data->etf_sell_count;
				///ETF申购数量
				data->etf_buy_qty = market_data->etf_buy_qty;
				///ETF申购金额
				data->etf_buy_money = market_data->etf_buy_money;
				///ETF赎回数量
				data->etf_sell_qty = market_data->etf_sell_qty;
				///ETF赎回金额
				data->etf_sell_money = market_data->etf_sell_money;
				///权证执行的总数量
				data->total_warrant_exec_qty = market_data->total_warrant_exec_qty;
				///权证跌停价格（元）
				data->warrant_lower_price = market_data->warrant_lower_price;
				///权证涨停价格（元）
				data->warrant_upper_price = market_data->warrant_upper_price;
				///买入撤单笔数
				data->cancel_buy_count = market_data->cancel_buy_count;
				///卖出撤单笔数
				data->cancel_sell_count = market_data->cancel_sell_count;
				///买入撤单数量
				data->cancel_buy_qty = market_data->cancel_buy_qty;
				///卖出撤单数量
				data->cancel_sell_qty = market_data->cancel_sell_qty;
				///买入撤单金额
				data->cancel_buy_money = market_data->cancel_buy_money;
				///卖出撤单金额
				data->cancel_sell_money = market_data->cancel_sell_money;
				///买入总笔数
				data->total_buy_count = market_data->total_buy_count;
				///卖出总笔数
				data->total_sell_count = market_data->total_sell_count;
				///买入委托成交最大等待时间
				data->duration_after_buy = market_data->duration_after_buy;
				///卖出委托成交最大等待时间
				data->duration_after_sell = market_data->duration_after_sell;
				///买方委托价位数
				data->num_bid_orders = market_data->num_bid_orders;
				///卖方委托价位数
				data->num_ask_orders = market_data->num_ask_orders;
				///成交时间（UA3113）
				data->exec_time = market_data->exec_time;
				///闭市标志（UA103/UA104）
				data->is_market_closed = gcnew String(market_data->is_market_closed);
				///合约持仓量（UA103）
				data->total_position = market_data->total_position;
				///市盈率1（UA103）
				data->pe_ratio1 = market_data->pe_ratio1;
				///市盈率2（UA103）
				data->pe_ratio2 = market_data->pe_ratio2;

				#pragma endregion
				quoteevent_managed->OnMarketData(data);
				delete data;
			}
			void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last) {
				QuoteStaticInfoStruct^ data = gcnew QuoteStaticInfoStruct();
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				if (error_info == nullptr || error_info->error_id == 0) {
					resInfo->error_id = 0;
					resInfo->error_msg = "";
					#pragma region 收到查询股票代码数据
					// 收到市场股价数据
					// 代码
					///交易所代码
					data->exchange_id = ticker_info->exchange_id;
					///合约代码（不包含交易所信息）
					data->ticker = gcnew String(ticker_info->ticker);
					/// 合约名称
					data->ticker_name = gcnew String(ticker_info->ticker_name, 0, sizeof(ticker_info->ticker_name), System::Text::Encoding::UTF8);
					/// 合约类型
					data->ticker_type = ticker_info->ticker_type;
					///昨收盘
					data->pre_close_price = ticker_info->pre_close_price;
					///涨停板价
					data->upper_limit_price = ticker_info->upper_limit_price;
					///跌停板价
					data->lower_limit_price = ticker_info->lower_limit_price;
					///最小变动价位
					data->price_tick = ticker_info->price_tick;
					/// 合约最小交易量(买)
					data->buy_qty_unit = ticker_info->buy_qty_unit;
					/// 合约最小交易量(卖)
					data->sell_qty_unit = ticker_info->sell_qty_unit;
					#pragma endregion
				}
				else {
					resInfo->error_id = error_info->error_id;
					resInfo->error_msg = gcnew String(error_info->error_msg);
				}
				quoteevent_managed->OnQueryAllTickers(resInfo, data, is_last);
				delete data;
			}
			#pragma endregion
		};
		

		///曝露给C#的引用类 
		public ref class TXPQuote
		{
		private:
			XTP::API::QuoteApi * pUserApi;
			XTP::API::QuoteSpi* pUserSpi;
			//QuoteSpiManagedClass ^mc;
		public:
			TXPQuote(int client_id , String ^ log_path)  // XTP::API::MyQuoteSpi % spi
			{
				char* log_path_char = (char*)Marshal::StringToHGlobalAnsi(log_path).ToPointer();
				//printf(str2);
				pUserApi = XTP::API::QuoteApi::CreateQuoteApi(client_id, log_path_char);
				//mc = gcnew QuoteSpiManagedClass();
				//pUserSpi = mc->m_Impl;// 
				pUserSpi = new XTP::API::MyQuoteSpi();
				Marshal::FreeHGlobal((IntPtr)log_path_char);
				quoteevent_managed->OnDisconnectedEvent += gcnew XTP::API::QuoteEventSource::OnDisconnectedDelegate(this, &XTP::API::TXPQuote::OnDisconnected);
				quoteevent_managed->OnErrorEvent += gcnew XTP::API::QuoteEventSource::OnErrorDelegate(this, &XTP::API::TXPQuote::OnError);
				quoteevent_managed->OnSubMarketDataEvent += gcnew XTP::API::QuoteEventSource::OnSubMarketDataDelegate(this, &XTP::API::TXPQuote::OnSubMarketData);
				quoteevent_managed->OnUnSubMarketDataEvent += gcnew XTP::API::QuoteEventSource::OnUnSubMarketDataDelegate(this, &XTP::API::TXPQuote::OnUnSubMarketData);
				quoteevent_managed->OnMarketDataEvent += gcnew XTP::API::QuoteEventSource::OnMarketDataDelegate(this, &XTP::API::TXPQuote::OnMarketData);
				quoteevent_managed->OnQueryAllTickersEvent += gcnew XTP::API::QuoteEventSource::OnQueryAllTickersDelegate(this, &XTP::API::TXPQuote::OnQueryAllTickers);
				pUserApi->RegisterSpi(pUserSpi);
			}
			bool IsLogin = false;
			#pragma region 主动方法

			int Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type) {
				char* investor_id_char = (char*)Marshal::StringToHGlobalAnsi(investor_id).ToPointer();
				char* password_char = (char*)Marshal::StringToHGlobalAnsi(password).ToPointer();
				char* ip_char = (char*)Marshal::StringToHGlobalAnsi(ip).ToPointer();
				int loginResult = pUserApi->Login(ip_char, port, investor_id_char, password_char, (XTP_PROTOCOL_TYPE)protocal_type);//XTP_PROTOCOL_TCP
				Marshal::FreeHGlobal((IntPtr)investor_id_char);
				Marshal::FreeHGlobal((IntPtr)password_char);
				Marshal::FreeHGlobal((IntPtr)ip_char);
				if (loginResult == 0) {
					IsLogin = true;
				}
				return loginResult;
			}
			//订阅数据
			int SubscribeMarketData(array<String^>^ ticker, EXCHANGE_TYPE exchange, bool is_subscribe) {
				//marshal_context ^ context = gcnew marshal_context();
				char** tokensAsAnsi = new char*[ticker->Length];
				for (int i = 0; i < ticker->Length; i++)
				{
					//IntPtr p = Marshal::StringToHGlobalAnsi(ticker[i]);
					//tokensAsAnsi[i] = static_cast<char*>(p.ToPointer());
					//Marshal::FreeHGlobal(p);
					tokensAsAnsi[i] = static_cast<char*>(Marshal::StringToHGlobalAnsi(ticker[i]).ToPointer());
					//const char* str4 = context->marshal_as<const char*>(ticker[i]);
					//tokensAsAnsi[i] = str4;
				}
				// The marshalled results are freed when context goes out of scope
				int result = 0;
				if (is_subscribe) {
					result = pUserApi->SubscribeMarketData(tokensAsAnsi, ticker->Length, (XTP_EXCHANGE_TYPE)exchange);//订阅股票行情
				}
				else {
					result = pUserApi->UnSubscribeMarketData(tokensAsAnsi, ticker->Length, XTP_EXCHANGE_TYPE(exchange));//取消订阅股票行情
				}
				delete[] tokensAsAnsi;    // Please note you must use delete[] here!
										  //delete context;
				return result;
			}
			String^ GetTradingDay() {
				return  gcnew String(pUserApi->GetTradingDay());
			}
			//获取API版本号
			String^ GetApiVersion() {
				return  gcnew String(pUserApi->GetApiVersion());
			}
			///获取API的系统错误
			RspInfoStruct^ GetApiLastError() {
				XTPRI* error_info = pUserApi->GetApiLastError();
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
				return resInfo;
			}
			//查询所有股票代码
			int QueryAllTickers(EXCHANGE_TYPE exchange) {
				return pUserApi->QueryAllTickers((XTP_EXCHANGE_TYPE)exchange);
			}
			int Logout() {
				IsLogin = false;
				return pUserApi->Logout();
			}
			#pragma endregion
			#pragma region 对外曝露事件定义
			event XTP::API::QuoteEventSource::OnDisconnectedDelegate^ OnDisconnectedEvent;
			event XTP::API::QuoteEventSource::OnErrorDelegate^ OnErrorEvent;
			event XTP::API::QuoteEventSource::OnSubMarketDataDelegate^ OnSubMarketDataEvent;
			event XTP::API::QuoteEventSource::OnUnSubMarketDataDelegate^ OnUnSubMarketDataEvent;
			event XTP::API::QuoteEventSource::OnMarketDataDelegate^ OnMarketDataEvent;
			event XTP::API::QuoteEventSource::OnQueryAllTickersDelegate^ OnQueryAllTickersEvent;
			#pragma endregion

		protected:
			#pragma region 事件触发

			void OnDisconnected(int reason)
			{
				IsLogin = false;
				OnDisconnectedEvent(reason);
			}
			void OnError(RspInfoStruct^ resp)
			{
				OnErrorEvent(resp);
			}
			void OnSubMarketData(RspInfoStruct^ resp, SpecificTickerStruct^ ticker, bool is_last)
			{
				OnSubMarketDataEvent(resp, ticker, is_last);
			}
			void OnUnSubMarketData(RspInfoStruct^ resp, SpecificTickerStruct^ ticker, bool is_last)
			{
				OnUnSubMarketDataEvent(resp, ticker, is_last);
			}
			void OnMarketData(MarketDataStruct^ data)
			{
				OnMarketDataEvent(data);
			}
			void OnQueryAllTickers(RspInfoStruct^ resp, QuoteStaticInfoStruct^ ticker_info, bool is_last)
			{
				OnQueryAllTickersEvent(resp, ticker_info, is_last);
			}
			#pragma endregion

		};

	}
}
