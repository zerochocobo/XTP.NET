// XTPDll.h
#pragma once
#include "basicDll.h"

using namespace System;

namespace XTP {
	namespace API {

		/// <summary>
		/// XTP_EXCHANGE_TYPE 是交易所类型
		/// </summary>
		public enum class EXCHANGE_TYPE
		{
			XTP_EXCHANGE_SH = 1,	///<上证
			XTP_EXCHANGE_SZ,		///<深证
			XTP_EXCHANGE_MAX	///<不存在的交易所类型
		};

		///行情
		public ref struct MarketDataStruct
		{
			// 代码
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息）
			String^	ticker;

			// 股票等价格
			///最新价
			double	last_price;
			///昨收盘
			double	pre_close_price;
			///今开盘
			double	open_price;
			///最高价
			double	high_price;
			///最低价
			double	low_price;
			///今收盘
			double	close_price;

			// 期货等数据
			///昨持仓量（目前未填写）
			double	pre_open_interest;
			///持仓量（目前未填写）
			double	open_interest;
			///上次结算价（目前未填写）
			double	pre_settlement_price;
			///本次结算价（目前未填写）
			double	settlement_price;

			///涨停板价（目前未填写）
			double	upper_limit_price;
			///跌停板价（目前未填写）
			double	lower_limit_price;
			///昨虚实度（目前未填写）
			double	pre_delta;
			///今虚实度（目前未填写）
			double	curr_delta;

			/// 时间类
			Int64 data_time;

			// 量额数据
			///数量
			Int64	qty;
			///成交金额
			double	turnover;
			///当日均价
			double	avg_price;

			// 买卖盘
			///十档申买价
			array<double>^ bid;
			///十档申卖价
			array<double>^	ask;
			///十档申买量
			array<Int64>^	bid_qty;
			///十档申卖量
			array<Int64>^	ask_qty;

			// lts没有包含的数据（目前未填写）
			///成交笔数
			Int64 trades_count;
			///当前交易状态说明
			String^ ticker_status;
			///委托买入总量
			Int64 total_bid_qty;
			///委托卖出总量
			Int64 total_ask_qty;
			///加权平均委买价格
			double ma_bid_price;
			///加权平均委卖价格
			double ma_ask_price;
			///债券加权平均委买价格
			double ma_bond_bid_price;
			///债券加权平均委卖价格
			double ma_bond_ask_price;
			///债券到期收益率
			double yield_to_maturity;
			///ETF净值估值
			double iopv;
			///ETF申购笔数
			Int32 etf_buy_count;
			///ETF赎回笔数
			Int32 etf_sell_count;
			///ETF申购数量
			double etf_buy_qty;
			///ETF申购金额
			double etf_buy_money;
			///ETF赎回数量
			double etf_sell_qty;
			///ETF赎回金额
			double etf_sell_money;
			///权证执行的总数量
			double total_warrant_exec_qty;
			///权证跌停价格（元）
			double warrant_lower_price;
			///权证涨停价格（元）
			double warrant_upper_price;
			///买入撤单笔数
			Int32 cancel_buy_count;
			///卖出撤单笔数
			Int32 cancel_sell_count;
			///买入撤单数量
			double cancel_buy_qty;
			///卖出撤单数量
			double cancel_sell_qty;
			///买入撤单金额
			double cancel_buy_money;
			///卖出撤单金额
			double cancel_sell_money;
			///买入总笔数
			Int64 total_buy_count;
			///卖出总笔数
			Int64 total_sell_count;
			///买入委托成交最大等待时间
			Int32 duration_after_buy;
			///卖出委托成交最大等待时间
			Int32 duration_after_sell;
			///买方委托价位数
			Int32 num_bid_orders;
			///卖方委托价位数
			Int32 num_ask_orders;
			///成交时间（UA3113）
			Int32 exec_time;
			///闭市标志（UA103/UA104）
			String^ is_market_closed;
			///合约持仓量（UA103）
			double total_position;
			///市盈率1（UA103）
			double pe_ratio1;
			///市盈率2（UA103）
			double pe_ratio2;
		} ;

		public ref struct QuoteStaticInfoStruct {
			///交易所代码
			int exchange_id;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			String^ ticker;
			/// 合约名称
			String^ ticker_name;
			/// 合约类型
			int ticker_type;
			///昨收盘
			double  pre_close_price;
			///涨停板价
			double  upper_limit_price;
			///跌停板价
			double  lower_limit_price;
			///最小变动价位
			double  price_tick;
			/// 合约最小交易量(买)
			Int32  buy_qty_unit;
			/// 合约最小交易量(卖)
			Int32 sell_qty_unit;
		};
	}
}