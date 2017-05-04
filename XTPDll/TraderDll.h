#pragma once
// XTPDll.h
#pragma once
#include "basicDll.h"

using namespace System;

namespace XTP {
	namespace API {


		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_SIDE_TYPE 是一个买卖方向类型
		/////////////////////////////////////////////////////////////////////////
		public enum class SIDE_TYPE
		{
			XTP_SIDE_BUY = 1,	///<买
			XTP_SIDE_SELL,		///<卖
			XTP_SIDE_BUY_OPEN,		///<买开
			XTP_SIDE_SELL_OPEN,		///<卖开
			XTP_SIDE_BUY_CLOSE,		///<买平
			XTP_SIDE_SELL_CLOSE		///<卖平
		};


		/////////////////////////////////////////////////////////////////////////
		///TXTPOrderTypeType是一个报单类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TOrderTypeType
		{
			XTP_ORDT_Normal = '0',	///正常
			XTP_ORDT_DeriveFromQuote = '1',		///报价衍生
			XTP_ORDT_DeriveFromCombination = '2',		///组合衍生
			XTP_ORDT_Combination = '3',		///组合报单
			XTP_ORDT_ConditionalOrder = '4',		///条件单
			XTP_ORDT_Swap = '5'		///互换单
		};
		/////////////////////////////////////////////////////////////////////////
		///TXTPTradeTypeType是一个成交类型类型
		/////////////////////////////////////////////////////////////////////////
		public enum class TTradeTypeType
		{
			XTP_TRDT_Common = '0',	///普通成交
			XTP_TRDT_OptionsExecution = '1',		///期权执行
			XTP_TRDT_OTC = '2',		///OTC成交
			XTP_TRDT_EFPDerived = '3',		///期转现衍生成交
			XTP_TRDT_CombinationDerived = '4',		///组合衍生成交
			XTP_TRDT_EFTPurchase = '5',		///ETF申购
			XTP_TRDT_EFTRedem = '6'		///ETF赎回
		};

		//////////////////////////////////////////////////////////////////////////
		///市场类型
		//////////////////////////////////////////////////////////////////////////
		public enum class MARKET_TYPE
		{
			XTP_MKT_INIT = 0,///<初始化值或者未知
			XTP_MKT_SZ_A = 1,///<深圳A股
			XTP_MKT_SH_A,    ///<上海A股
							 /*
							 XTP_MKT_SZ_B,    ///<深圳B股
							 XTP_MKT_SH_B,    ///<上海B股*/
			XTP_MKT_MAX  ///<市场类型个数
		};


		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_TE_RESUME_TYPE 是一个私有流重传方式
		/////////////////////////////////////////////////////////////////////////
		public enum class TE_RESUME_TYPE
		{
			XTP_TERT_RESTART = 0,	///<从本交易日开始重传
			XTP_TERT_RESUME,		///<从从上次收到的续传
			XTP_TERT_QUICK			///<只传送登录后私有流的内容
		};

		/////////////////////////////////////////////////////////////////////////
		///XTP_ORDER_SUBMIT_STATUS_TYPE是一个报单提交状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_SUBMIT_STATUS_TYPE
		{
			XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED = 1, ///<订单已经提交
			XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED,///<订单已经被接受
			XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED,///<订单已经被拒绝
			XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED,///<撤单已经提交
			XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED,///<撤单已经被拒绝
			XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED ///<撤单已经被接受
		};

		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_ORDER_ACTION_STATUS_TYPE 是一个报单操作状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_ACTION_STATUS_TYPE
		{
			XTP_ORDER_ACTION_STATUS_SUBMITTED = 1,	///<已经提交
			XTP_ORDER_ACTION_STATUS_ACCEPTED,		///<已经接受
			XTP_ORDER_ACTION_STATUS_REJECTED		///<已经被拒绝
		};

		/////////////////////////////////////////////////////////////////////////
		///XTP_ORDER_STATUS_TYPE是一个报单状态类型
		/////////////////////////////////////////////////////////////////////////
		public enum class ORDER_STATUS_TYPE
		{
			XTP_ORDER_STATUS_INIT = 0,///<初始化
			XTP_ORDER_STATUS_ALLTRADED = 1,           ///<全部成交
			XTP_ORDER_STATUS_PARTTRADEDQUEUEING,  ///<部分成交
			XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING, ///<部分撤单
			XTP_ORDER_STATUS_NOTRADEQUEUEING,   ///<未成交
			XTP_ORDER_STATUS_CANCELED,  ///<已撤单
			XTP_ORDER_STATUS_REJECTED,  ///<已拒绝
			XTP_ORDER_STATUS_UNKNOWN  ///<未知
		};
		/////////////////////////////////////////////////////////////////////////
		///@enum XTP_PRICE_TYPE 是一个价格类型
		/////////////////////////////////////////////////////////////////////////
		public enum class PRICE_TYPE
		{
			XTP_PRICE_LIMIT = 1,           ///<限价单
			XTP_PRICE_BEST_OR_CANCEL,      ///<即时成交剩余转撤销，市价单
			XTP_PRICE_BEST5_OR_LIMIT,      ///<最优五档即时成交剩余转限价，市价单
			XTP_PRICE_BEST5_OR_CANCEL,     ///<最优5档即时成交剩余转撤销，市价单
			XTP_PRICE_ALL_OR_CANCEL,       ///<全部成交或撤销,市价单
			XTP_PRICE_FORWARD_BEST,        ///<本方最优，市价单
			XTP_PRICE_REVERSE_BEST_LIMIT,  ///<对方最优剩余转限价，市价单
			XTP_PRICE_TYPE_MAX,			///<价格类型个数
		};

		public ref struct XTPOrderInsert
		{
			///XTP系统订单ID，无需用户填写
			UInt64                order_xtp_id;
			///报单引用，由客户自定义，小于1000000
			UInt32	            order_client_id;
			///合约代码 客户端请求不带空格，以'\0'结尾
			String^                    ticker;
			///交易市场
			MARKET_TYPE         market;
			///价格
			double                  price;
			///止损价（保留字段）
			double                  stop_price;
			///数量
			Int64                 quantity;
			///报单价格
			PRICE_TYPE          price_type;
			///买卖方向
			SIDE_TYPE           side;
		};

		//////////////////////////////////////////////////////////////////////////
		///报单查询
		//////////////////////////////////////////////////////////////////////////
		///报单查询请求-条件查询
		public ref struct XTPQueryOrder
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
			String^    ticker;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
			UInt64   begin_time;
			///格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			UInt64   end_time;
		};

		///查询成交回报请求-查询条件(和之前其实一样）
		public ref struct XTPQueryTrader
		{
			///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
			String^    ticker;
			///开始时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
			Int64   begin_time;
			///结束时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			Int64   end_time;
		};

		//////////////////////////////////////////////////////////////////////////
		///账户资金查询响应结构体
		//////////////////////////////////////////////////////////////////////////
		public ref struct QueryAssetRspStruct
		{
			///总资产
			double total_asset;
			///可用资金
			double buying_power;
			///证券资产
			double security_asset;
			///累计买入成交证券占用资金（保留字段）
			double fund_buy_amount;
			///累计买入成交交易费用（保留字段）
			double fund_buy_fee;
			///累计卖出成交证券所得资金（保留字段）
			double fund_sell_amount;
			///累计卖出成交交易费用（保留字段）
			double fund_sell_fee;
			//XTP系统预扣的资金（包括购买卖股票时预扣的交易资金+预扣手续费）
			double withholding_amount;
		};


		///报单响应结构体
		public ref struct OrderInfoStruct
		{
			///XTP系统订单ID
			UInt64                order_xtp_id;
			///报单引用，用户自定义，小于1000000
			UInt32	            order_client_id;
			///报单操作引用，用户自定义，小于1000000
			UInt32                order_cancel_client_id;
			///撤单在XTP系统中的id
			UInt32                order_cancel_xtp_id;
			///合约代码
			String^                    ticker;
			///交易市场
			MARKET_TYPE         market;
			///价格
			double                  price;
			///数量
			Int64                 quantity;
			///报单价格条件
			PRICE_TYPE          price_type;
			///买卖方向
			SIDE_TYPE           side;
			///今成交数量
			Int64                 qty_traded;
			///剩余数量，当撤单成功时，表示撤单数量
			Int64                 qty_left;
			///委托时间
			Int64                 insert_time;
			///最后修改时间
			Int64                 update_time;
			///撤销时间
			Int64                 cancel_time;
			///成交金额
			double                  trade_amount;
			///本地报单编号 OMS生成的单号
			String^                    order_local_id;
			///报单状态
			ORDER_STATUS_TYPE   order_status;
			///报单提交状态
			ORDER_SUBMIT_STATUS_TYPE   order_submit_status;
			///报单类型
			TOrderTypeType       order_type;
		};

		///撤单失败响应消息
		public ref struct OrderCancelInfoStruct
		{
			///撤单XTPID
			UInt64                 order_cancel_xtp_id;
			///原始订单XTPID
			UInt64                 order_xtp_id;
		};

		///报单成交结构体
		public ref struct TradeReportStruct
		{
			///XTP系统订单ID
			UInt64                 order_xtp_id;
			///报单引用，小于1000000
			UInt32                order_client_id;
			///合约代码
			String^                     ticker;
			///交易市场
			MARKET_TYPE          market;
			///订单号
			UInt64                 local_order_id;
			///成交编号，深交所唯一，上交所每笔交易唯一
			String^                    exec_id;
			///价格
			double                   price;
			///数量
			Int64                  quantity;
			///成交时间
			Int64                  trade_time;
			///成交金额
			double                   trade_amount;
			///成交序号 --回报记录号，每个交易所唯一
			UInt64                 report_index;
			///报单编号 --交易所单号
			String^                     order_exch_id;
			///成交类型  --成交回报中的执行类型
			TTradeTypeType        trade_type;
			///买卖方向
			SIDE_TYPE            side;
			///交易所交易员代码 
			String^                    branch_pbu;
		};


		//////////////////////////////////////////////////////////////////////////
		///查询股票持仓情况
		//////////////////////////////////////////////////////////////////////////
		public ref struct QueryStkPositionStruct
		{
			///证券代码
			String^	 ticker;
			///证券名称
			String^		ticker_name;
			///交易市场
			MARKET_TYPE     market;
			///当前持仓
			Int64             total_qty;
			///可用股份数
			Int64             sellable_qty;
			///持仓成本
			double              avg_price;
			///浮动盈亏
			double              unrealized_pnl;
		};



	}
}