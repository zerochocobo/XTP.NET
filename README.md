# XTP CLI（中泰证券极速交易接口.NET版本接口）

中泰证券XTP接口，用CLI封装C++，输出的DLL可供原生.NET程序调用

中泰XTP网址：https://www.xtphelp.com/

##基本说明

XTP接口暂时只支持c++开发的dll，用.NET调用非常痛苦，因此我用CLI在中间做了一层桥接，用将接收事件封装成了event，使得调用变得非常舒服。
开发过程是痛苦的，CLI资料很少，为了让后来人乘凉，在此开源。（项目中还残留了一些试错过程中用过的代码可以参考）
大部分我所用到的功能进行了封装，当前版本支持1.1.13.10。


##.NET调用示意

.NET调用sample暂不开源，调用方式简单示例如下（假设在WinForm窗体中测试初始化）：

using XTP.API;

        TXPQuote quote;
        TXPTrader trader;
        int client_id = 1;
        string log_file_path = "c:\\log\\";
        string key = "分配的秘钥";
        private void Form1_Load(object sender, EventArgs e)
        {
            quote = new TXPQuote(client_id, log_file_path);
            quote.OnSubMarketDataEvent += Quote_OnSubMarketDataEvent;
            quote.OnUnSubMarketDataEvent += Quote_OnUnSubMarketDataEvent;
            quote.OnMarketDataEvent += Quote_OnMarketDataEvent;
            quote.OnErrorEvent += Quote_OnErrorEvent;
            quote.OnDisconnectedEvent += Quote_OnDisconnectedEvent;
            quote.OnQueryAllTickersEvent += Quote_OnQueryAllTickersEvent;

            trader = new TXPTrader(client_id, key, log_file_path, TE_RESUME_TYPE.XTP_TERT_QUICK);
            trader.OnQueryAssetEvent += Trader_OnQueryAssetEvent;
            trader.OnQueryPositionEvent += Trader_OnQueryPositionEvent;
            trader.OnErrorEvent += Trader_OnErrorEvent;
            trader.OnDisconnectedEvent += Trader_OnDisconnectedEvent;
            trader.OnOrderEvent += Trader_OnOrderEvent;
            trader.OnTradeEvent += Trader_OnTradeEvent;
            trader.OnQueryOrderEvent += Trader_OnQueryOrderEvent;
            trader.OnQueryTradeEvent += Trader_OnQueryTradeEvent;
            trader.OnOrderCancelEvent += Trader_OnOrderCancelEvent;
        }


##其他注意事项：

-.NET版本：4.5.2
-XTPDll项目目标需要设为x64
-XTPDll项目下放置最新lib文件才能编译
-.NET项目输出需要包含最新dll文件才能运行
-尽量不要在事件中处理过于复杂的逻辑，一些耗时的任务（例如大量保存行情信息），可以单开线程处理。
-多线程环境.NET建议用4.x以上的System.Collections.Concurrent线程安全类

##项目维护：
仲仓戟（Dennis Zhong）



