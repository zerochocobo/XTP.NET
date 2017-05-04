#pragma once
#include "stdafx.h"
#include <msclr/marshal.h>
#include "TraderDll.h"
#include <functional>
#include "./xtpinclude/xtp_trader_api.h"
//#include <vcclr.h>
//#include <atlstr.h>
using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;
//using namespace std::placeholders; // for `_1`

namespace XTP {
	namespace API {

		///中间交易事件，用来互通两个类
		public ref class TraderEventSource
		{
		public:
			///公用事件和回调定义,公用的触发器，由事件源头触发这里，再转触发对C#曝露的类

			delegate void OnDisconnectedDelegate(UInt64, int);
			delegate void OnErrorDelegate(RspInfoStruct^);
			delegate void OnOrderDelegate(RspInfoStruct^, OrderInfoStruct^);
			delegate void OnQueryOrderDelegate(RspInfoStruct^, OrderInfoStruct^, int, bool);
			delegate void OnOrderCancelDelegate(RspInfoStruct^, OrderCancelInfoStruct^);
			delegate void OnTradeDelegate(TradeReportStruct^);
			delegate void OnQueryTradeDelegate(RspInfoStruct^, TradeReportStruct^, int, bool);
			delegate void OnQueryAssetDelegate(RspInfoStruct^, QueryAssetRspStruct^, int, bool);
			delegate void OnQueryPositionDelegate(RspInfoStruct^, QueryStkPositionStruct^, int, bool);
			event OnDisconnectedDelegate^ OnDisconnectedEvent;
			event OnErrorDelegate^ OnErrorEvent;
			event OnOrderDelegate^ OnOrderEvent;
			event OnQueryOrderDelegate^ OnQueryOrderEvent;
			event OnOrderCancelDelegate^ OnOrderCancelEvent;
			event OnTradeDelegate^ OnTradeEvent;
			event OnQueryTradeDelegate^ OnQueryTradeEvent;
			event OnQueryAssetDelegate^ OnQueryAssetEvent;
			event OnQueryPositionDelegate ^OnQueryPositionEvent;
			void OnDisconnected(UInt64 session_id, int reason) {
				OnDisconnectedEvent(session_id, reason);
			}
			void OnError(RspInfoStruct^ resp) {
				OnErrorEvent(resp);
			}
			void OnOrder(RspInfoStruct^ resp, OrderInfoStruct^ order) {
				OnOrderEvent(resp, order);
			}
			void OnQueryOrder(RspInfoStruct^ resp, OrderInfoStruct^ order, int request_id, bool is_last) {
				OnQueryOrderEvent(resp, order, request_id, is_last);
			}
			void OnOrderCancel(RspInfoStruct^ resp, OrderCancelInfoStruct^ cancel) {
				OnOrderCancelEvent(resp, cancel);
			}
			void OnTrade(TradeReportStruct ^ report) {
				OnTradeEvent(report);
			}
			void OnQueryTrade(RspInfoStruct^ resp, TradeReportStruct^ report, int request_id, bool is_last) {
				OnQueryTradeEvent(resp, report, request_id, is_last);
			}
			void OnQueryPosition(RspInfoStruct^ resp, QueryStkPositionStruct^ report, int request_id, bool is_last) {
				OnQueryPositionEvent(resp, report, request_id, is_last);
			}
			void OnQueryAsset(RspInfoStruct ^ resp, QueryAssetRspStruct ^ asset, int request_id, bool is_last) {
				OnQueryAssetEvent(resp, asset, request_id, is_last);
			}

		private:
			//MyOnSubMarketDataDelegate ^myEvent;
			//Object ^eventLock;
		};

		///全局事件,用来让两个类可以互通
		gcroot<TraderEventSource^> traderevent_managed = gcnew TraderEventSource();

		///交易事件定义，因为是普通类（需要继承TraderSpi）而非引用类，所以无法在内部定义delegate
		public class MyTraderSpi :public TraderSpi {
		public:
			//MyQuoteSpi() {}
			//MyQuoteSpi(EventSource^ managed) : m_managed(managed) {}
			#pragma region 共用方法
			///获取共用服务器返回状态
			RspInfoStruct^ GetRspInfoStruct(XTPRI* error_info) {
				RspInfoStruct^ resInfo = gcnew RspInfoStruct();
				resInfo->error_id = error_info->error_id;
				resInfo->error_msg = gcnew String(error_info->error_msg);
				return resInfo;
			}
			///两个事件共用的查询交易方法
			TradeReportStruct^ GetTradeReportStruct(XTPQueryTradeRsp* trade_info) {
				TradeReportStruct^ report = gcnew TradeReportStruct();
				///XTP系统订单ID
				report->order_xtp_id = trade_info->order_xtp_id;
				///报单引用，小于1000000
				report->order_client_id = trade_info->order_client_id;
				///合约代码
				report->ticker = gcnew String(trade_info->ticker);
				///交易市场
				report->market = (MARKET_TYPE)trade_info->market;
				///订单号
				report->local_order_id = trade_info->local_order_id;
				///成交编号，深交所唯一，上交所每笔交易唯一
				report->exec_id = gcnew String(trade_info->exec_id);
				///价格
				report->price = trade_info->price;
				///数量
				report->quantity = trade_info->quantity;
				///成交时间
				report->trade_time = trade_info->trade_time;
				///成交金额
				report->trade_amount = trade_info->trade_amount;
				///成交序号 --回报记录号，每个交易所唯一
				report->report_index = trade_info->report_index;
				///报单编号 --交易所单号
				report->order_exch_id = gcnew String(trade_info->order_exch_id);
				///成交类型  --成交回报中的执行类型
				report->trade_type = (TTradeTypeType)trade_info->trade_type;
				///买卖方向
				report->side = (SIDE_TYPE)trade_info->side;
				///交易所交易员代码 
				report->branch_pbu = gcnew String(trade_info->branch_pbu);
				return report;
			}

			//两个事件共用的查询报单方法
			OrderInfoStruct^ GetOrderInfoStruct(XTPOrderInfo *order_info) {
				OrderInfoStruct^ order = gcnew OrderInfoStruct();
				///XTP系统订单ID
				order->order_xtp_id = order_info->order_xtp_id;
				///报单引用，用户自定义，小于1000000
				order->order_client_id = order_info->order_client_id;
				///报单操作引用，用户自定义，小于1000000
				order->order_cancel_client_id = order_info->order_cancel_client_id;
				///撤单在XTP系统中的id
				order->order_cancel_xtp_id = order_info->order_cancel_xtp_id;
				///合约代码
				order->ticker = gcnew String(order_info->ticker);
				///交易市场
				order->market = (MARKET_TYPE)order_info->market;
				///价格
				order->price = order_info->price;
				///数量
				order->quantity = order_info->quantity;
				///报单价格条件
				order->price_type = (PRICE_TYPE)order_info->price_type;
				///买卖方向
				order->side = (SIDE_TYPE)order_info->side;
				///今成交数量
				order->qty_traded = order_info->qty_traded;
				///剩余数量，当撤单成功时，表示撤单数量
				order->qty_left = order_info->qty_left;
				///委托时间
				order->insert_time = order_info->insert_time;
				///最后修改时间
				order->update_time = order_info->update_time;
				///撤销时间
				order->cancel_time = order_info->cancel_time;
				///成交金额
				order->trade_amount = order_info->trade_amount;
				///本地报单编号 OMS生成的单号
				order->order_local_id = gcnew String(order_info->order_local_id);
				///报单状态
				order->order_status = (ORDER_STATUS_TYPE)order_info->order_status;
				///报单提交状态
				order->order_submit_status = (ORDER_SUBMIT_STATUS_TYPE)order_info->order_submit_status;
				///报单类型
				order->order_type = (TOrderTypeType)(order_info->order_type);
				//order->order_type = gcnew String((char*)order_info->order_type);
				return order;
			}
			#pragma endregion
			#pragma region 实现TraderSpi回调

			void OnError(XTPRI *error_info) {
				traderevent_managed->OnError(GetRspInfoStruct(error_info));
			}
			void OnDisconnected(UInt64 session_id,int reason) {
				traderevent_managed->OnDisconnected(session_id,reason);
			}
			///报单通知
			///@param order_info 订单响应具体信息，用户可以通过order_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单，order_info.qty_left字段在订单为未成交、部成、全成、废单状态时，表示此订单还没有成交的数量，在部撤、全撤状态时，表示此订单被撤的数量。order_info.order_cancel_xtp_id为其所对应的撤单ID，不为0时表示此单被撤成功
			///@param error_info 订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 每次订单状态更新时，都会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，在订单未成交、全部成交、全部撤单、部分撤单、已拒绝这些状态时会有响应，对于部分成交的情况，请由订单的成交回报来自行确认
			void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info) {
				traderevent_managed->OnOrder(GetRspInfoStruct(error_info), GetOrderInfoStruct(order_info));
			};
			///成交通知
			///@param trade_info 成交回报的具体信息，用户可以通过trade_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。对于上交所，exec_id可以唯一标识一笔成交。当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交了。对于深交所，exec_id是唯一的，暂时无此判断机制。eport_index+market字段可以组成唯一标识表示成交回报。
			///@remark 订单有成交发生的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			void OnTradeEvent(XTPTradeReport *trade_info) {
				traderevent_managed->OnTrade(GetTradeReportStruct(trade_info));
			};
			///撤单出错响应
			///@param cancel_info 撤单具体信息，包括撤单的order_cancel_xtp_id和待撤单的order_xtp_id
			///@param error_info 撤单被拒绝或者发生错误时错误代码和错误信息，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 此响应只会在撤单发生错误时被回调
			void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info) {
				OrderCancelInfoStruct^ cancel = gcnew OrderCancelInfoStruct();
				cancel->order_cancel_xtp_id = cancel_info->order_cancel_xtp_id;
				cancel->order_xtp_id = cancel_info->order_xtp_id;
				traderevent_managed->OnOrderCancel(GetRspInfoStruct(error_info), cancel);
			};

			///请求查询报单响应
			///@param order_info 查询到的报单
			///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last) {
				traderevent_managed->OnQueryOrder(GetRspInfoStruct(error_info), GetOrderInfoStruct(order_info), request_id, is_last);
			};

			///请求查询成交响应
			///@param trade_info 查询到的成交回报
			///@param error_info 查询成交回报发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last) {
				traderevent_managed->OnQueryTrade(GetRspInfoStruct(error_info), GetTradeReportStruct(trade_info),request_id,is_last);
			};

			///请求查询投资者持仓响应
			///@param position 查询到的持仓情况
			///@param error_info 查询账户持仓发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 由于用户可能持有多个股票，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last) {
				QueryStkPositionStruct^ posi = gcnew QueryStkPositionStruct();
				///证券代码
				posi->ticker = gcnew String(position->ticker);
				///证券名称
				posi->ticker_name = gcnew String(position->ticker_name, 0, sizeof(position->ticker_name), System::Text::Encoding::UTF8);
				///交易市场
				posi->market = (MARKET_TYPE)position->market;
				///当前持仓
				posi->total_qty = position->total_qty;
				///可用股份数
				posi->sellable_qty = position->sellable_qty;
				///持仓成本
				posi->avg_price = position->avg_price;
				///浮动盈亏
				posi->unrealized_pnl = position->unrealized_pnl;
				traderevent_managed->OnQueryPosition(GetRspInfoStruct(error_info), posi, request_id, is_last);
			};

			///请求查询资金账户响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param asset 查询到的资金账户情况
			///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last) {
				QueryAssetRspStruct ^ query_asset = gcnew QueryAssetRspStruct();
				query_asset->total_asset = asset->total_asset;
				query_asset->buying_power = asset->buying_power;
				query_asset->security_asset = asset->security_asset;
				query_asset->fund_buy_amount = asset->fund_buy_amount;
				query_asset->fund_buy_fee = asset->fund_buy_fee;
				query_asset->fund_sell_amount = asset->fund_sell_amount;
				query_asset->fund_sell_fee = asset->fund_sell_fee;
				query_asset->withholding_amount = asset->withholding_amount;
				traderevent_managed->OnQueryAsset(GetRspInfoStruct(error_info), query_asset, request_id, is_last);
			};

			#pragma endregion
		};

		///曝露给C#的引用类 
		public ref class TXPTrader
		{
		private:
			XTP::API::TraderApi * pUserApi;
			XTP::API::TraderSpi* pUserSpi;
		public:
			TXPTrader(int client_id, String^ account_key, String ^ log_path, TE_RESUME_TYPE resume_type) 
			{
				char* log_path_char = (char*)Marshal::StringToHGlobalAnsi(log_path).ToPointer();
				pUserApi = XTP::API::TraderApi::CreateTraderApi(client_id, log_path_char);
				pUserApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
				pUserApi->SetSoftwareVersion("1.1.6.2");

				char* account_key_char = (char*)Marshal::StringToHGlobalAnsi(account_key).ToPointer();
				pUserApi->SetSoftwareKey(account_key_char);
				Marshal::FreeHGlobal((IntPtr)account_key_char);

				pUserSpi = new XTP::API::MyTraderSpi();
				Marshal::FreeHGlobal((IntPtr)log_path_char);
				//pUserApi->RegisterSpi(&spi); // 注册事件类
				traderevent_managed->OnDisconnectedEvent += gcnew XTP::API::TraderEventSource::OnDisconnectedDelegate(this, &XTP::API::TXPTrader::OnDisconnected);
				traderevent_managed->OnErrorEvent += gcnew XTP::API::TraderEventSource::OnErrorDelegate(this, &XTP::API::TXPTrader::OnError);
				traderevent_managed->OnQueryAssetEvent += gcnew XTP::API::TraderEventSource::OnQueryAssetDelegate(this, &XTP::API::TXPTrader::OnQueryAsset);
				traderevent_managed->OnTradeEvent += gcnew XTP::API::TraderEventSource::OnTradeDelegate(this, &XTP::API::TXPTrader::OnTrade);
				traderevent_managed->OnOrderEvent += gcnew XTP::API::TraderEventSource::OnOrderDelegate(this, &XTP::API::TXPTrader::OnOrder);
				traderevent_managed->OnOrderCancelEvent += gcnew XTP::API::TraderEventSource::OnOrderCancelDelegate(this, &XTP::API::TXPTrader::OnOrderCancel);
				traderevent_managed->OnQueryOrderEvent += gcnew XTP::API::TraderEventSource::OnQueryOrderDelegate(this, &XTP::API::TXPTrader::OnQueryOrder);
				traderevent_managed->OnQueryTradeEvent += gcnew XTP::API::TraderEventSource::OnQueryTradeDelegate(this, &XTP::API::TXPTrader::OnQueryTrade);
				traderevent_managed->OnQueryPositionEvent += gcnew XTP::API::TraderEventSource::OnQueryPositionDelegate(this, &XTP::API::TXPTrader::OnQueryPosition);
				
				pUserApi->RegisterSpi(pUserSpi);
			}
			bool IsLogin = false;
			#pragma region 主动方法

			UInt64 Login(String ^ ip, int port, String ^ investor_id, String ^ password, PROTOCOL_TYPE protocal_type) {
				char* investor_id_char = (char*)Marshal::StringToHGlobalAnsi(investor_id).ToPointer();
				char* password_char = (char*)Marshal::StringToHGlobalAnsi(password).ToPointer();
				char* ip_char = (char*)Marshal::StringToHGlobalAnsi(ip).ToPointer();
				uint64_t loginResult = pUserApi->Login(ip_char, port, investor_id_char, password_char, (XTP_PROTOCOL_TYPE)protocal_type);//XTP_PROTOCOL_TCP
				Marshal::FreeHGlobal((IntPtr)investor_id_char);
				Marshal::FreeHGlobal((IntPtr)password_char);
				Marshal::FreeHGlobal((IntPtr)ip_char);
				if (loginResult > 0) {
					IsLogin = true;
				}
				return loginResult;
			}
			///通过报单在xtp系统中的ID获取下单的客户端id
			///@return 返回客户端id，可以用此方法过滤自己下的订单
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 由于系统允许同一用户在不同客户端上登录操作，每个客户端通过不同的client_id进行区分
			UInt64 GetClientIDByXTPID(UInt64 order_xtp_id) {
				return pUserApi->GetClientIDByXTPID(order_xtp_id);
			}
			///通过报单在xtp系统中的ID获取相关资金账户名
			///@return 返回资金账户名
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 只有资金账户登录成功后,才能得到正确的信息
			String^ GetAccountByXTPID(UInt64 order_xtp_id) {
				return gcnew String(pUserApi->GetAccountByXTPID(order_xtp_id));
			}
			void SubscribePublicTopic(TE_RESUME_TYPE resume_type) {
				pUserApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
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
			///登出请求
			///@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
			///@param session_id 资金账户对应的session_id,登录时得到
			int Logout(UInt64 session_id) {
				IsLogin = false;
				return pUserApi->Logout(session_id);
			}
			UInt64 InsertOrder(XTPOrderInsert ^order, UInt64 session_id) {
				XTPOrderInsertInfo *order_info = new XTPOrderInsertInfo();
				order_info->order_xtp_id = order->order_xtp_id;
				order_info->order_client_id = order->order_client_id;
				order_info->market = (XTP_MARKET_TYPE)order->market;
				order_info->price = order->price;
				order_info->stop_price = order->stop_price;
				order_info->quantity = order->quantity;
				order_info->price_type = (XTP_PRICE_TYPE)order->price_type;
				order_info->side = (XTP_SIDE_TYPE)order->side;
				IntPtr ticker = Marshal::StringToHGlobalAnsi(order->ticker);
				strcpy(order_info->ticker, static_cast<char*>(ticker.ToPointer()));
				static_cast<char*>(ticker.ToPointer());
				Marshal::FreeHGlobal(ticker);
				return pUserApi->InsertOrder(order_info,session_id);
			}
			///报单操作请求
			///@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order_xtp_id 需要撤销的委托单在XTP系统中的ID
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
			UInt64 CancelOrder(UInt64 order_xtp_id, UInt64 session_id) {
				return pUserApi->CancelOrder(order_xtp_id, session_id);
			}
			///根据报单ID请求查询报单
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryOrderByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id) {
				return pUserApi->QueryOrderByXTPID(order_xtp_id, session_id, request_id);
			}
			///请求查询报单
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应
			int QueryOrders(XTPQueryOrder ^query_param, UInt64 session_id, int request_id) {
				XTPQueryOrderReq *query = new XTPQueryOrderReq();
				IntPtr ticker = Marshal::StringToHGlobalAnsi(query_param->ticker);
				strcpy(query->ticker, static_cast<char*>(ticker.ToPointer()));
				Marshal::FreeHGlobal(ticker);
				query->begin_time = query_param->begin_time;
				query->end_time = query_param->end_time;
				return pUserApi->QueryOrders(query, session_id, request_id);
			}
			///根据委托编号请求查询相关成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			int QueryTradesByXTPID(UInt64 order_xtp_id, UInt64 session_id, int request_id) {
				return pUserApi->QueryTradesByXTPID(order_xtp_id, session_id, request_id);
			}
			///请求查询已成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应
			int QueryTrades(XTPQueryTrader ^query_param, UInt64 session_id, int request_id) {
				XTPQueryTraderReq *query = new XTPQueryTraderReq();
				IntPtr ticker = Marshal::StringToHGlobalAnsi(query_param->ticker);
				strcpy(query->ticker, static_cast<char*>(ticker.ToPointer()));
				Marshal::FreeHGlobal(ticker);
				query->begin_time = query_param->begin_time;
				query->end_time = query_param->end_time;
				return pUserApi->QueryTrades(query, session_id, request_id);
			}
			///请求查询投资者持仓
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param ticker 需要查询的持仓合约代码，可以为空，如果不为空，请不带空格，并以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息，如果合约代码为空，则默认查询所有持仓信息
			int QueryPosition(String^ ticker, UInt64 session_id, int request_id) {
				char* ticker_char = (char*)Marshal::StringToHGlobalAnsi(ticker).ToPointer();
				int result =  pUserApi->QueryPosition(ticker_char, session_id, request_id);
				Marshal::FreeHGlobal((IntPtr)ticker_char);
				return result;
			}
			///请求查询资产
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			int QueryAsset(UInt64 session_id, int request_id) {
				return pUserApi->QueryAsset(session_id, request_id);
			}

			#pragma endregion

			#pragma region 事件定义

			event XTP::API::TraderEventSource::OnDisconnectedDelegate^ OnDisconnectedEvent;
			event XTP::API::TraderEventSource::OnErrorDelegate^ OnErrorEvent;
			event XTP::API::TraderEventSource::OnQueryAssetDelegate^ OnQueryAssetEvent;
			event XTP::API::TraderEventSource::OnTradeDelegate^ OnTradeEvent;
			event XTP::API::TraderEventSource::OnQueryTradeDelegate^ OnQueryTradeEvent;
			event XTP::API::TraderEventSource::OnOrderDelegate^ OnOrderEvent;
			event XTP::API::TraderEventSource::OnQueryOrderDelegate^ OnQueryOrderEvent;
			event XTP::API::TraderEventSource::OnOrderCancelDelegate^ OnOrderCancelEvent;
			event XTP::API::TraderEventSource::OnQueryPositionDelegate^ OnQueryPositionEvent;
			
			#pragma endregion

		protected:
			#pragma region 事件触发


			void OnDisconnected(UInt64 session_id,int reason)
			{
				IsLogin = false;
				OnDisconnectedEvent(session_id,reason);
			}
			void OnError(RspInfoStruct^ resp)
			{
				OnErrorEvent(resp);
			}
			void OnQueryAsset(RspInfoStruct ^ resp, QueryAssetRspStruct ^ asset, int request_id, bool is_last) {
				OnQueryAssetEvent(resp, asset, request_id, is_last);
			}
			void OnOrder(RspInfoStruct^ resp, OrderInfoStruct^ order) {
				OnOrderEvent(resp, order);
			}
			void OnQueryOrder(RspInfoStruct^ resp, OrderInfoStruct^ order, int request_id, bool is_last) {
				OnQueryOrderEvent(resp, order, request_id, is_last);
			}
			void OnOrderCancel(RspInfoStruct^ resp, OrderCancelInfoStruct^ cancel) {
				OnOrderCancelEvent(resp, cancel);
			}
			void OnTrade(TradeReportStruct ^ report) {
				OnTradeEvent(report);
			}
			void OnQueryTrade(RspInfoStruct^ resp, TradeReportStruct^ report, int request_id, bool is_last) {
				OnQueryTradeEvent(resp, report, request_id, is_last);
			}
			void OnQueryPosition(RspInfoStruct^ resp, QueryStkPositionStruct^ report, int request_id, bool is_last) {
				OnQueryPositionEvent(resp, report, request_id, is_last);
			}
			#pragma endregion
		};

	}
}