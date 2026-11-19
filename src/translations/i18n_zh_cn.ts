<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>Backend</name>
    <message>
        <location filename="../core/Backend.cpp" line="126"/>
        <source>Starting measurement</source>
        <translation>开始测量</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="178"/>
        <source>Listening on interface: %1</source>
        <translation>监听接口: %1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="203"/>
        <source>Closing interface: %1</source>
        <translation>关闭接口: %1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="213"/>
        <source>Measurement stopped</source>
        <translation>测量已停止</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="276"/>
        <source>All active interfaces disconnected. Measurement paused (trace preserved).</source>
        <translation>所有活动接口均已断开，测量已暂停（跟踪数据已保留）。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="304"/>
        <source>Missing: %1</source>
        <translation>缺失：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="307"/>
        <source>Ambiguous: %1</source>
        <translation>存在歧义：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="310"/>
        <source>No configured interfaces found to rebind.</source>
        <translation>未找到可重新绑定的已配置接口。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="313"/>
        <source>Manual rebind failed: %1</source>
        <translation>手动重新绑定失败：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="334"/>
        <source>Network </source>
        <translation>网络 </translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="412"/>
        <source>Unable to get driver for interface id: %1. This should never happen.</source>
        <translation>无法获取接口 ID: %1 的驱动程序。这不应该发生。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="461"/>
        <location filename="../core/Backend.cpp" line="479"/>
        <source>File not found or not readable.</source>
        <translation>文件未找到或不可读。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="489"/>
        <source>Failed to parse DBC file. Please check the log for details.</source>
        <translation>解析 DBC 文件失败。请查看日志了解详情。</translation>
    </message>
</context>
<context>
    <name>BaseTraceViewModel</name>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="79"/>
        <source>Index</source>
        <translation>序号</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="81"/>
        <source>Time</source>
        <translation>时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="83"/>
        <source>Channel</source>
        <translation>通道</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="85"/>
        <source>RX/TX</source>
        <translation>RX/TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="87"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="91"/>
        <source>Sender</source>
        <translation>发送方</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="93"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="97"/>
        <source>Data</source>
        <translation>数据</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="99"/>
        <source>Comment</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="246"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="246"/>
        <source>TX</source>
        <translation>TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="273"/>
        <source>Sleep</source>
        <translation>休眠</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="274"/>
        <source>Wakeup</source>
        <translation>唤醒</translation>
    </message>
</context>
<context>
    <name>BusInterface</name>
    <message>
        <location filename="../driver/BusInterface.cpp" line="90"/>
        <source>ready</source>
        <translation>就绪</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="91"/>
        <source>warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="92"/>
        <source>error passive</source>
        <translation>被动错误</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="93"/>
        <source>bus off</source>
        <translation>总线关闭</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="94"/>
        <source>stopped</source>
        <translation>已停止</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="95"/>
        <source>unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="96"/>
        <source>tx success</source>
        <translation>发送成功</translation>
    </message>
    <message>
        <location filename="../driver/BusInterface.cpp" line="97"/>
        <source>tx fail</source>
        <translation>发送失败</translation>
    </message>
</context>
<context>
    <name>BusListener</name>
    <message>
        <location filename="../driver/BusListener.cpp" line="67"/>
        <source>Interface #%1: %2, Version: %3</source>
        <translation>接口 #%1: %2, 版本: %3</translation>
    </message>
    <message>
        <location filename="../driver/BusListener.cpp" line="85"/>
        <source>Device connection lost or closed unexpectedly</source>
        <translation>设备连接丢失或被意外关闭</translation>
    </message>
    <message>
        <location filename="../driver/BusListener.cpp" line="86"/>
        <source>Error on interface: %1, Closed!!!</source>
        <translation>接口错误: %1, 已关闭!!!</translation>
    </message>
</context>
<context>
    <name>BusTrace</name>
    <message>
        <location filename="../core/BusTrace.cpp" line="169"/>
        <source>Trace exceeded %1 messages; oldest messages are being discarded and will be missing from saved traces. Increase the limit in Settings.</source>
        <translation>跟踪已超过 %1 条消息；最早的消息将被丢弃，且不会出现在保存的跟踪中。请在设置中提高上限。</translation>
    </message>
</context>
<context>
    <name>CanStatusWindow</name>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.ui" line="14"/>
        <source>Can Status</source>
        <translation>CAN 状态</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="79"/>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="99"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="79"/>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="99"/>
        <source>Value</source>
        <translation>值</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="138"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="208"/>
        <source>Recover from bus off</source>
        <translation>从 Bus-Off 恢复</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="254"/>
        <source>ready (listen only)</source>
        <translation>就绪（仅监听）</translation>
    </message>
</context>
<context>
    <name>CandleApiInterface</name>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="346"/>
        <source>CandleApi: getAvailableBitrates() failed</source>
        <translation>CandleApi: getAvailableBitrates() 失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="395"/>
        <source>CandleApi: set bittiming failed</source>
        <translation>CandleApi: 设置位定时失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="401"/>
        <source>CandleApi: no timing entry for bitrate=%1 samplePoint=%2 fclk=%3</source>
        <translation>CandleApi: 无比特率=%1 采样点=%2 fclk=%3 的定时条目</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="410"/>
        <source>CandleApi: get capabilities failed</source>
        <translation>CandleApi: 获取功能失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="422"/>
        <source>CandleApi: set data bittiming failed</source>
        <translation>CandleApi: 设置数据位定时失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="428"/>
        <source>CandleApi: no FD timing entry for bitrate=%1 samplePoint=%2 fclk=%3</source>
        <translation>CandleApi: 无比特率=%1 采样点=%2 fclk=%3 的FD定时条目</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="443"/>
        <source>CandleApi: device open failed</source>
        <translation>CandleApi: 设备打开失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="455"/>
        <source>CandleApi: set bitrate failed</source>
        <translation>CandleApi: 设置比特率失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="522"/>
        <source>CandleApi: FD sample point %1 invalid for %2 bit/s, using %3</source>
        <translation>CandleApi: FD采样点 %1 对 %2 bit/s 无效，使用 %3</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="534"/>
        <source>CandleApi: FD data bittiming failed, falling back to classic CAN</source>
        <translation>CandleApi: FD数据位定时失败，回退到经典CAN</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="542"/>
        <source>CandleApi: automatic TDC setup failed</source>
        <translation>CandleApi: 自动 TDC 设置失败</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="547"/>
        <source>CandleApi: CAN FD requested but device does not support it</source>
        <translation>CandleApi: 请求CAN FD但设备不支持</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="562"/>
        <source>CandleApi: channel %1 start failed (error %2)</source>
        <translation>CandleApi: 通道 %1 启动失败（错误 %2）</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="579"/>
        <source>CandleApi: device timestamp unavailable, using host clock</source>
        <translation>CandleApi: 设备时间戳不可用，使用主机时钟</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="748"/>
        <source>CandleApi: connection to %1 lost</source>
        <translation>CandleApi: 与 %1 的连接已丢失</translation>
    </message>
    <message>
        <location filename="../driver/CandleApiDriver/CandleApiInterface.cpp" line="887"/>
        <source>CandleApi: bus-off recovery request failed (error %1)</source>
        <translation>CandleApi: Bus-Off 恢复请求失败（错误 %1）</translation>
    </message>
</context>
<context>
    <name>ConditionalLoggingDialog</name>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="25"/>
        <source>Conditional Logging Configuration</source>
        <translation>条件日志配置</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="89"/>
        <source>Conditions (Trigger Logging)</source>
        <translation>条件 (触发日志)</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="92"/>
        <source>Signal</source>
        <translation>信号</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="92"/>
        <source>Operator</source>
        <translation>运算符</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="92"/>
        <source>Threshold</source>
        <translation>阈值</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="97"/>
        <source>Add Condition</source>
        <translation>添加条件</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="98"/>
        <source>Remove Condition</source>
        <translation>移除条件</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="102"/>
        <source>Use AND logic for all conditions</source>
        <translation>对所有条件使用 AND 逻辑</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="108"/>
        <source>CAN Selection (Network / Message / Signal)</source>
        <translation>CAN 选择 (网络 / 消息 / 信号)</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="111"/>
        <source>Hierarchy</source>
        <translation>层级结构</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="116"/>
        <source>Log File Path (Optional for filtering):</source>
        <translation>日志文件路径 (可选, 用于过滤):</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="120"/>
        <source>Browse...</source>
        <translation>浏览...</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="124"/>
        <source>Enable Conditional Logging (File Output)</source>
        <translation>启用条件日志 (文件输出)</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="129"/>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="130"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="272"/>
        <source>Select Log File</source>
        <translation>选择日志文件</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="272"/>
        <source>CSV Files (*.csv)</source>
        <translation>CSV 文件 (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="313"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../window/ConditionalLoggingDialog/ConditionalLoggingDialog.cpp" line="313"/>
        <source>Please select an output file for logging.</source>
        <translation>请选择日志输出文件。</translation>
    </message>
</context>
<context>
    <name>ConditionalLoggingManager</name>
    <message>
        <location filename="../core/ConditionalLoggingManager.cpp" line="211"/>
        <source>Conditional logging failed to open &apos;%1&apos;: %2</source>
        <translation>条件日志无法打开 '%1'：%2</translation>
    </message>
</context>
<context>
    <name>GatewayWindow</name>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="20"/>
        <source>CAN Gateway</source>
        <translation>CAN 网关</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="26"/>
        <source>Enable Gateway</source>
        <translation>启用网关</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="60"/>
        <source>Message:</source>
        <translation>消息：</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="77"/>
        <source>Source:</source>
        <translation>源：</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="94"/>
        <source>Destination:</source>
        <translation>目标：</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="128"/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="147"/>
        <source>Active Rules:</source>
        <translation>活动规则：</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="161"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="166"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="171"/>
        <source>Source</source>
        <translation>源</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="176"/>
        <source>Destination</source>
        <translation>目标</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="202"/>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <location filename="../window/GatewayWindow/GatewayWindow.ui" line="239"/>
        <source>OK</source>
        <translation>确定</translation>
    </message>
</context>
<context>
    <name>GenericCanSetupPage</name>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="19"/>
        <source>Form</source>
        <translation>Form</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="37"/>
        <source>Driver:</source>
        <translation>驱动:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="50"/>
        <location filename="../driver/GenericCanSetupPage.ui" line="82"/>
        <location filename="../driver/GenericCanSetupPage.ui" line="114"/>
        <source>TextLabel</source>
        <translation>TextLabel</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="69"/>
        <source>Interface:</source>
        <translation>接口：</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="101"/>
        <source>Interface Details:</source>
        <translation>接口详情:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="166"/>
        <source>Bitrate:</source>
        <translation>波特率:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="208"/>
        <source>Sample Point:</source>
        <translation>采样点:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="307"/>
        <source>Data Bitrate:</source>
        <translation>数据波特率:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="349"/>
        <source>Data Sample Point:</source>
        <translation>数据采样点:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="431"/>
        <source>Configured by OS</source>
        <translation>由操作系统配置</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="438"/>
        <source>Bus Monitoring Mode (Listen Only)</source>
        <translation>总线监听模式 (仅接收)</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="445"/>
        <source>One-Shot Mode</source>
        <translation>单次发送模式</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="459"/>
        <source>Auto-Restart on Bus-Off Condition</source>
        <translation>Bus-Off 时自动重启</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="466"/>
        <source>Custom Bitrate + Sample Point</source>
        <translation>自定义波特率 + 采样点</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="473"/>
        <source>Custom CAN FD Data Bitrate + Sample Point</source>
        <translation>自定义 CAN FD 数据波特率 + 采样点</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="234"/>
        <location filename="../driver/GenericCanSetupPage.ui" line="375"/>
        <source>HHHHHH</source>
        <oldsource>HHHH</oldsource>
        <translation>HHHHHH</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="189"/>
        <location filename="../driver/GenericCanSetupPage.ui" line="330"/>
        <source>Div+Seg1+Seg2 (Hex)</source>
        <oldsource>Seg1+Seg2 (Hex)</oldsource>
        <translation>Div+Seg1+Seg2 (Hex)</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="288"/>
        <source>CAN FD Data Phase Setting</source>
        <translation>CAN FD 数据段设置</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="147"/>
        <source>CAN Setting / CAN FD Arbitration Phase Setting</source>
        <translation>CAN 设置 / CAN FD 仲裁段设置</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="415"/>
        <source>Options:</source>
        <translation>选项:</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.ui" line="452"/>
        <source>Triple Sampling</source>
        <translation>三重采样</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.cpp" line="78"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.cpp" line="79"/>
        <source>(unavailable) %1</source>
        <translation>（不可用）%1</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.cpp" line="80"/>
        <source>Device is offline. Reconnect USB and use &apos;Rebind &amp; Resume&apos; or Rescan.</source>
        <translation>设备已离线。请重新连接 USB，然后使用“重新绑定并恢复”或重新扫描。</translation>
    </message>
</context>
<context>
    <name>GraphWindow</name>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="14"/>
        <source>Visualization View</source>
        <translation>可视化视图</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="22"/>
        <source>View Type:</source>
        <translation>视图类型:</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="32"/>
        <source>Duration:</source>
        <translation>持续时间:</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="40"/>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="45"/>
        <source>1 min</source>
        <translation>1 分钟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="50"/>
        <source>5 min</source>
        <translation>5 分钟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="55"/>
        <source>10 min</source>
        <translation>10 分钟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="60"/>
        <source>15 min</source>
        <translation>15 分钟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="65"/>
        <source>30 min</source>
        <translation>30 分钟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="73"/>
        <source>Zoom +</source>
        <translation>放大</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="76"/>
        <source>Zoom in along time axis</source>
        <translation>沿时间轴放大</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="83"/>
        <source>Zoom -</source>
        <translation>缩小</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="86"/>
        <source>Zoom out along time axis</source>
        <translation>沿时间轴缩小</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="93"/>
        <source>Reset Zoom</source>
        <translation>重置缩放</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="96"/>
        <source>Reset zoom and re-enable auto scroll</source>
        <translation>重置缩放并重新启用自动滚动</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="103"/>
        <source>Cursors (Δt/Δy)</source>
        <translation>游标 (Δt/Δy)</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="109"/>
        <source>Toggle dual measurement cursors (Left-click: A, Right/Shift-click: B)</source>
        <translation>切换双测量游标（左键：A，右键/Shift+左键：B）</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="124"/>
        <source>Reset All Signals</source>
        <translation>重置所有信号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="131"/>
        <source>Clear UI</source>
        <translation>清除界面</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="158"/>
        <source>Signals</source>
        <translation>信号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="164"/>
        <source>Search Signals/Messages...</source>
        <translation>搜索信号/消息...</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="172"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="177"/>
        <source>Unit</source>
        <translation>单位</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="182"/>
        <source>Comment</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="187"/>
        <source>System Node</source>
        <translation>系统节点</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="200"/>
        <source>Add to Graph</source>
        <translation>添加到图表</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="207"/>
        <source>Add to Condition</source>
        <translation>添加到条件</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="217"/>
        <source>Conditional Logging</source>
        <translation>条件日志</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="230"/>
        <source>Signal</source>
        <translation>信号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="235"/>
        <source>Operator</source>
        <translation>运算符</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="240"/>
        <source>Threshold</source>
        <translation>阈值</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="245"/>
        <source>Live Feedback</source>
        <translation>实时反馈</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="250"/>
        <source>Action</source>
        <translation>动作</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="263"/>
        <source>Show on Graph on Trigger</source>
        <translation>触发时在图表中显示</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="270"/>
        <source>Status:</source>
        <translation>状态:</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="282"/>
        <source>Condition not met</source>
        <translation>条件未满足</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="447"/>
        <source>Columns:</source>
        <translation>列数：</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="450"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="468"/>
        <source>Graph (Time-series)</source>
        <translation>图表（时间序列）</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="451"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="468"/>
        <source>Scatter Chart</source>
        <translation>散点图</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="452"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="468"/>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="453"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="468"/>
        <source>Gauge</source>
        <translation>仪表盘</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="593"/>
        <source>CONDITION MET - ACTIVE</source>
        <translation>条件已满足 - 激活</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="608"/>
        <source>Condition not met - Waiting...</source>
        <translation>条件未满足 - 等待中...</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="631"/>
        <source>Idle - Signal Lost</source>
        <translation>空闲 - 信号丢失</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1203"/>
        <source>CAN — %1</source>
        <translation>CAN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1229"/>
        <source>Unsigned</source>
        <translation>无符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1229"/>
        <source>Signed</source>
        <translation>有符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1244"/>
        <source>LIN — %1</source>
        <translation>LIN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1292"/>
        <source>Bus Load — %1</source>
        <translation>总线负载 — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1299"/>
        <source>Virtual</source>
        <translation>虚拟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1305"/>
        <source>0..100 %</source>
        <translation>0..100 %</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1306"/>
        <source>1 s rolling window</source>
        <translation>1 秒滚动窗口</translation>
    </message>
</context>
<context>
    <name>HardwareDialog</name>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="59"/>
        <source>CAN FD</source>
        <translation>CAN FD</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="60"/>
        <source>Listen only</source>
        <translation>仅监听</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="61"/>
        <source>Triple sampling</source>
        <translation>三倍采样</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="62"/>
        <source>One shot</source>
        <translation>单次发送</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="63"/>
        <source>Auto restart</source>
        <translation>自动重连</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="64"/>
        <source>Configurable timestamps</source>
        <translation>可配置时间戳</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="65"/>
        <source>Custom bitrate</source>
        <translation>自定义波特率</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="66"/>
        <source>Custom CAN FD bitrate</source>
        <translation>自定义 CAN FD 波特率</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="67"/>
        <source>Bus-off recovery</source>
        <translation>Bus-Off 恢复</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="68"/>
        <source>TDC</source>
        <translation>TDC</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="69"/>
        <source>LIN master</source>
        <translation>LIN 主节点</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="70"/>
        <source>LIN slave</source>
        <translation>LIN 从节点</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="71"/>
        <source>LIN monitor</source>
        <translation>LIN 监听</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="87"/>
        <source>&amp;Rescan</source>
        <translation>重新扫描(&amp;R)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="90"/>
        <source>Hardware</source>
        <translation>硬件设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="94"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="94"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="285"/>
        <source>Bus</source>
        <translation>总线</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="94"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="287"/>
        <source>Status</source>
        <translation>状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="94"/>
        <source>Bus State</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="95"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="286"/>
        <source>Version</source>
        <translation>版本</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="95"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="317"/>
        <source>Capabilities</source>
        <translation>支持能力</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="95"/>
        <source>Network</source>
        <translation>所属网络</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="113"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="383"/>
        <source>Ask every driver to look for new or removed devices</source>
        <translation>通知所有驱动重新扫描新增或移除的设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="115"/>
        <source>Rebind &amp;&amp; &amp;Resume</source>
        <translation>重新绑定并恢复(&amp;R)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="116"/>
        <source>Rescan USB/CAN hardware, rebind configured interfaces by Hardware Key, and resume measurement</source>
        <translation>重新扫描 USB/CAN 硬件，按硬件标识（Hardware Key）重新绑定已配置接口并恢复测量</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="117"/>
        <source>&amp;Copy Report</source>
        <translation>复制报告(&amp;C)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="118"/>
        <source>Copy the device list as plain text, e.g. for a bug report</source>
        <translation>以纯文本复制设备列表（便于反馈问题）</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="129"/>
        <source>Rebind &amp; Resume</source>
        <translation>重新绑定并恢复</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="130"/>
        <source>Failed to rebind interfaces.</source>
        <translation>重新绑定接口失败。</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="173"/>
        <source>no devices found</source>
        <translation>未发现设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="225"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="287"/>
        <source>Open</source>
        <translation>已打开</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="225"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="287"/>
        <source>Available</source>
        <translation>可用</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="268"/>
        <source>Select an interface to see its details.</source>
        <translation>请选择一个接口查看详细信息。</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="283"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="284"/>
        <source>Description</source>
        <translation>描述</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="289"/>
        <source>Bus state</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="290"/>
        <source>Interface ID</source>
        <translation>接口 ID</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="291"/>
        <source>Hardware Key</source>
        <translation>硬件标识</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="302"/>
        <source>, %1 Bd</source>
        <translation>, %1 Bd</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="304"/>
        <source>, %1 / %2 kBit/s (FD)</source>
        <translation>, %1 / %2 kBit/s (FD)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="306"/>
        <source>, %1 kBit/s</source>
        <translation>, %1 kBit/s</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="308"/>
        <source>, listen only</source>
        <translation>，仅监听</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="310"/>
        <source> (disabled)</source>
        <translation>（已禁用）</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="311"/>
        <source>Setup</source>
        <translation>配置</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="330"/>
        <source>Supported bitrates (%1)</source>
        <translation>支持的波特率 (%1)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="382"/>
        <source>Stop the measurement to rescan</source>
        <translation>请先停止测量再重新扫描</translation>
    </message>
</context>
<context>
    <name>LogModel</name>
    <message>
        <location filename="../core/LogModel.cpp" line="83"/>
        <source>Time</source>
        <translation>时间</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="85"/>
        <source>Level</source>
        <translation>级别</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="87"/>
        <source>Message</source>
        <translation>消息</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="186"/>
        <source>debug</source>
        <translation>调试</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="187"/>
        <source>info</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="188"/>
        <source>warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="189"/>
        <source>error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="190"/>
        <source>critical</source>
        <translation>严重</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="191"/>
        <source>fatal</source>
        <translation>致命</translation>
    </message>
</context>
<context>
    <name>LogWindow</name>
    <message>
        <location filename="../window/LogWindow/LogWindow.ui" line="20"/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location filename="../window/LogWindow/LogWindow.ui" line="69"/>
        <source>Export Logs...</source>
        <translation>导出日志...</translation>
    </message>
    <message>
        <location filename="../window/LogWindow/LogWindow.ui" line="79"/>
        <source>Clear</source>
        <translation>清空</translation>
    </message>
    <message>
        <location filename="../window/LogWindow/LogWindow.cpp" line="126"/>
        <source>Export Logs</source>
        <translation>导出日志</translation>
    </message>
    <message>
        <location filename="../window/LogWindow/LogWindow.cpp" line="126"/>
        <source>Log Files (*.log);;Text Files (*.txt);;All Files (*)</source>
        <translation>日志文件 (*.log);;文本文件 (*.txt);;所有文件 (*)</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="17"/>
        <location filename="../mainwindow.cpp" line="1837"/>
        <source>CANgaroo</source>
        <oldsource>cangaroo</oldsource>
        <translation>CANgaroo</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="56"/>
        <source>Start</source>
        <translation>启动</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="63"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="92"/>
        <source>Setup Interface...</source>
        <translation>配置接口...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="149"/>
        <source>&amp;File</source>
        <translation>&amp;文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="163"/>
        <source>&amp;Window</source>
        <translation>&amp;窗口</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="167"/>
        <source>&amp;New</source>
        <translation>&amp;新建</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="183"/>
        <source>&amp;Measurement</source>
        <translation>&amp;测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="194"/>
        <source>&amp;Help</source>
        <translation>&amp;帮助</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="202"/>
        <source>&amp;Trace</source>
        <translation>&amp;跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="216"/>
        <source>E&amp;xit</source>
        <oldsource>&amp;Exit</oldsource>
        <translation>&amp;退出</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="219"/>
        <source>Alt+F4</source>
        <translation>Alt+F4</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="224"/>
        <source>&amp;Trace View</source>
        <translation>&amp;跟踪视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="227"/>
        <source>Ctrl+Shift+T</source>
        <translation>Ctrl+Shift+T</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="232"/>
        <source>&amp;Log View</source>
        <translation>&amp;日志视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="235"/>
        <source>Ctrl+Shift+L</source>
        <translation>Ctrl+Shift+L</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="240"/>
        <source>&amp;Setup...</source>
        <translation>&amp;配置...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="243"/>
        <source>Ctrl+Alt+S</source>
        <translation>Ctrl+Alt+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="248"/>
        <source>Star&amp;t Measurement</source>
        <translation>&amp;启动测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="251"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="256"/>
        <source>Sto&amp;p Measurement</source>
        <translation>&amp;停止测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="259"/>
        <source>Shift+F5</source>
        <translation>Shift+F5</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="264"/>
        <source>&amp;Reload Interfaces</source>
        <translation>重新加载接口(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="282"/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="287"/>
        <source>&amp;Save Trace to file...</source>
        <translation>&amp;保存跟踪到文件...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="295"/>
        <source>&amp;Graph View</source>
        <translation>&amp;图表视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="298"/>
        <source>Ctrl+Shift+G</source>
        <translation>Ctrl+Shift+G</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="306"/>
        <source>&amp;New Workspace...</source>
        <translation>&amp;新建工作空间...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="309"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="314"/>
        <source>&amp;Save Workspace</source>
        <translation>&amp;保存工作空间</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="317"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="322"/>
        <source>Save Workspace &amp;As...</source>
        <translation>&amp;另存工作空间为...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="325"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="330"/>
        <source>&amp;Open Workspace...</source>
        <translation>&amp;打开工作空间...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="333"/>
        <source>Ctrl+O</source>
        <translation>Ctrl+O</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="338"/>
        <source>&amp;Clear</source>
        <translation>&amp;清除</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="341"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="346"/>
        <source>Can &amp;Status View</source>
        <translation>&amp;CAN状态视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="351"/>
        <source>Graph View</source>
        <translation>图表视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="356"/>
        <location filename="../mainwindow.cpp" line="1343"/>
        <source>Message View</source>
        <translation>消息视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="366"/>
        <source>Script View</source>
        <translation>脚本视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="371"/>
        <source>Replay View</source>
        <translation>回放视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="379"/>
        <source>Restore Window on startup</source>
        <translation>启动时恢复窗口</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="382"/>
        <source>Restore Window Position on startup</source>
        <translation>启动时恢复窗口位置</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="387"/>
        <source>&amp;Settings...</source>
        <translation>设置(&amp;S)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="361"/>
        <location filename="../mainwindow.cpp" line="1361"/>
        <source>Generator View</source>
        <translation>生成器视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1139"/>
        <source>Open workspace configuration</source>
        <translation>打开工作空间配置文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1140"/>
        <location filename="../mainwindow.cpp" line="1157"/>
        <source>Workspace config files (*.cangaroo)</source>
        <translation>工作空间配置文件 (*.cangaroo)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1156"/>
        <source>Save workspace configuration</source>
        <translation>保存工作空间配置文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1189"/>
        <source>The current workspace has been modified.</source>
        <translation>当前工作空间已被修改。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1190"/>
        <source>Do you want to save your changes?</source>
        <translation>是否要保存更改？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1848"/>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="158"/>
        <source>Export full trace</source>
        <translation>导出完整跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="136"/>
        <location filename="../mainwindow.cpp" line="1288"/>
        <source>Standalone Graph</source>
        <translation>独立图表</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="167"/>
        <source>Open Recent</source>
        <translation>最近打开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="175"/>
        <source>Open Standalone Graph Window (Ctrl+Shift+B)</source>
        <translation>打开独立图表窗口 (Ctrl+Shift+B)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="181"/>
        <source>Gateway</source>
        <translation>网关</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="460"/>
        <source>Clear Recent Files</source>
        <translation>清除最近文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="565"/>
        <source>&amp;Record Trace to File</source>
        <translation>录制跟踪到文件(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="568"/>
        <source>Record</source>
        <translation>录制</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="569"/>
        <source>Continuously record all traffic to a file (Ctrl+R)</source>
        <translation>将所有通信持续录制到文件 (Ctrl+R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="572"/>
        <source>Recording &amp;Options...</source>
        <translation>录制选项(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="573"/>
        <source>Open Recording &amp;Folder</source>
        <translation>打开录制文件夹(&amp;F)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="658"/>
        <source>● REC  %1  ·  %2  ·  %3</source>
        <translation>● REC  %1  ·  %2  ·  %3</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="662"/>
        <source>%1
%2 frames written</source>
        <translation>%1
已写入 %2 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="670"/>
        <source>Recording armed, starts with measurement</source>
        <translation>录制已就绪，将随测量开始</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="693"/>
        <source>&amp;Trigger Capture (Blackbox)...</source>
        <translation>触发捕获（黑匣子）(&amp;T)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="694"/>
        <source>Configure pre/post-trigger blackbox slice capture</source>
        <translation>配置触发前/触发后黑匣子切片捕获</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="698"/>
        <source>Trigger</source>
        <translation>触发</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="699"/>
        <source>Configure pre/post-trigger blackbox capture</source>
        <translation>配置触发前/触发后黑匣子捕获</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="714"/>
        <source>Trigger fired: %1 (capturing post-trigger window...)</source>
        <translation>触发已发生：%1（正在捕获触发后窗口...）</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="718"/>
        <source>Trigger capture saved (%1 frames): %2 [%3]</source>
        <translation>触发捕获已保存（%1 帧）：%2 [%3]</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="725"/>
        <source>Trigger capture failed: %1</source>
        <translation>触发捕获失败：%1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="754"/>
        <source>TRIG CAPTURING...</source>
        <translation>触发捕获中...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="758"/>
        <source>TRIG ARMED (%1)</source>
        <translation>触发已就绪（%1）</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="778"/>
        <source>Rebind &amp;&amp; Resume</source>
        <translation>重新绑定并恢复</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="780"/>
        <source>Dismiss</source>
        <translation>忽略</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="811"/>
        <source>USB/CAN device disconnected</source>
        <translation>USB/CAN 设备已断开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="813"/>
        <source>Interface &apos;%1&apos; disconnected (%2). Reconnect the USB adapter, then click &quot;Rebind &amp; Resume&quot; to restore measurement without losing trace/graph data.</source>
        <translation>接口 '%1' 已断开（%2）。请重新连接 USB 适配器，然后点击“重新绑定并恢复”，即可在不丢失跟踪/图表数据的情况下恢复测量。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="816"/>
        <source>Interface &apos;%1&apos; disconnected — please manually rebind.</source>
        <translation>接口 '%1' 已断开——请手动重新绑定。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="819"/>
        <source>USB / CAN Interface Disconnected</source>
        <translation>USB / CAN 接口已断开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="820"/>
        <source>Interface &apos;%1&apos; lost connection (%2).

Existing Trace and Graph data have been preserved.
Please reconnect the USB cable and click &quot;Rebind &amp; Resume&quot; in the top banner or Hardware dialog.</source>
        <translation>接口 '%1' 连接丢失（%2）。

已有的跟踪与图表数据已保留。
请重新插好 USB 线缆，然后在顶部提示条或“硬件”对话框中点击“重新绑定并恢复”。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="836"/>
        <source>Interfaces rebound and measurement resumed (trace preserved).</source>
        <translation>接口已重新绑定，测量已恢复（跟踪数据已保留）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="841"/>
        <source>Rebind &amp; Resume Failed</source>
        <translation>重新绑定并恢复失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="842"/>
        <source>Could not rebind configured interface(s):
%1

Please verify the USB adapter is plugged in and try again, or open Measurement Setup.</source>
        <translation>无法重新绑定已配置的接口：
%1

请确认 USB 适配器已插好后重试，或打开测量配置。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="844"/>
        <source>No matching hardware found.</source>
        <translation>未找到匹配的硬件。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="966"/>
        <source>Cannot open workspace settings file: %1</source>
        <translation>无法打开工作空间配置文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="974"/>
        <source>Cannot load settings from file: %1</source>
        <translation>无法从文件加载设置: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="996"/>
        <source>Could not read window %1 from file: %2</source>
        <translation>无法从文件 %2 读取窗口 %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1008"/>
        <source>Unable to read measurement setup from workspace config file: %1</source>
        <translation>无法从工作空间配置文件读取测量设置: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1017"/>
        <location filename="../mainwindow.cpp" line="1278"/>
        <source>CAN Gateway</source>
        <translation>CAN 网关</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1051"/>
        <source>Cannot save window settings to file: %1</source>
        <translation>无法将窗口设置保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1091"/>
        <source>Cannot save measurement setup to file: %1</source>
        <translation>无法将测量设置保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1117"/>
        <source>Saved workspace settings to file: %1</source>
        <translation>工作空间设置已保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1198"/>
        <source>Do not ask again (always discard)</source>
        <translation>不再询问 (始终丢弃)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1355"/>
        <source>BUS Status</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1385"/>
        <source>Python</source>
        <translation>Python</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1877"/>
        <source>Export Full Trace</source>
        <translation>导出完整跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1877"/>
        <source>Current trace is empty.</source>
        <translation>当前跟踪为空。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1883"/>
        <source>Trace Was Pruned</source>
        <translation>跟踪已被裁剪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1884"/>
        <source>The in-memory trace exceeded %1 frames and older frames were pruned.
For lossless long recordings, enable Continuous Recording (Ctrl+R) before measurement.</source>
        <translation>内存中的跟踪超过 %1 帧，较早的帧已被裁剪。
如需无损的长时间记录，请在测量前启用连续录制 (Ctrl+R)。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1895"/>
        <source>Import Trace File</source>
        <translation>导入跟踪文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1897"/>
        <source>All Supported (*.asc *.candump *.csv *.pcap *.pcapng *.mf4);;Vector ASC (*.asc);;CAN/FD Raw CSV (*.csv);;Linux candump (*.candump);;PCAP (*.pcap);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4);;All Files (*)</source>
        <translation>所有支持的格式 (*.asc *.candump *.csv *.pcap *.pcapng *.mf4);;Vector ASC (*.asc);;CAN/FD 原始帧 CSV (*.csv);;Linux candump (*.candump);;PCAP (*.pcap);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4);;所有文件 (*)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1915"/>
        <source>Import Failed</source>
        <translation>导入失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1917"/>
        <source>Failed to parse trace file or file is empty.</source>
        <translation>解析跟踪文件失败，或文件为空。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1927"/>
        <source>Large Trace File</source>
        <translation>大型跟踪文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1928"/>
        <source>This file contains %1 frames, exceeding the in-memory Trace limit (%2).</source>
        <translation>该文件包含 %1 帧，超过内存跟踪上限（%2）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1931"/>
        <source>Choose how to handle this large trace:</source>
        <translation>请选择如何处理该大型跟踪：</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1932"/>
        <source>Split &amp; Save Files...</source>
        <translation>分段保存文件...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1933"/>
        <source>Open in Replay Window</source>
        <translation>在回放窗口中打开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1934"/>
        <source>Load First %1 Frames</source>
        <translation>仅加载前 %1 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1942"/>
        <source>Split Trace by Frame Count</source>
        <translation>按帧数分割跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1943"/>
        <source>Maximum frames per output file:</source>
        <translation>每个输出文件的最大帧数：</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1953"/>
        <source>Save Split Trace Files</source>
        <translation>保存分段跟踪文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1955"/>
        <source>CAN/FD Raw CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1971"/>
        <source>Split Save Failed</source>
        <translation>分段保存失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1971"/>
        <source>No files written.</source>
        <translation>未写入任何文件。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1974"/>
        <source>Split Save Complete</source>
        <translation>分段保存完成</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1975"/>
        <source>Saved %1 split file(s) starting at:
%2</source>
        <translation>已保存 %1 个分段文件，起始文件：
%2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2004"/>
        <source>Imported %1 frames from %2</source>
        <translation>已从 %2 导入 %1 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1106"/>
        <source>Cannot open workspace file for writing: %1</source>
        <translation>无法打开工作空间文件进行写入: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1753"/>
        <source>Save Trace to file</source>
        <translation>将跟踪保存到文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1773"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1773"/>
        <source>Cannot open file for writing.</source>
        <translation>无法打开文件进行写入。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1849"/>
        <source>Chinese</source>
        <translation>Chinese</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="162"/>
        <source>Import full trace</source>
        <translation>导入完整跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1546"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1221"/>
        <source>Trace</source>
        <translation>跟踪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="174"/>
        <location filename="../mainwindow.cpp" line="1268"/>
        <location filename="../mainwindow.cpp" line="1337"/>
        <source>Graph</source>
        <translation>图表</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1349"/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1394"/>
        <source>Replay</source>
        <translation>回放</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1502"/>
        <source>About CANgaroo</source>
        <translation>关于CANgaroo</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="277"/>
        <source>Check for &amp;Updates...</source>
        <translation>检查更新(&amp;U)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1545"/>
        <source>Checking for updates from %1...</source>
        <translation>正在从 %1 检查更新...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1548"/>
        <location filename="../mainwindow.cpp" line="1587"/>
        <location filename="../mainwindow.cpp" line="1596"/>
        <location filename="../mainwindow.cpp" line="1616"/>
        <source>Check for Updates</source>
        <translation>检查更新</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1588"/>
        <source>You are already using the latest version (%1).

Update source: %2</source>
        <translation>当前已是最新版本 (%1)。

更新源: %2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1597"/>
        <source>You are running a development build (%1).

Latest version in %2: %3</source>
        <translation>当前正在运行开发构建版本 (%1)。

%2 中的最新版本: %3</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1602"/>
        <location filename="../mainwindow.cpp" line="1697"/>
        <source>Open Release Page</source>
        <translation>打开发布页面</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1617"/>
        <source>Failed to check for updates from %1.</source>
        <translation>从 %1 检查更新失败。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1619"/>
        <source>Open Releases Page</source>
        <translation>打开发布列表</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1632"/>
        <source>Update Available</source>
        <translation>发现新版本</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1637"/>
        <source>A new version of CANgaroo is available!</source>
        <translation>发现新版本 CANgaroo！</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1645"/>
        <source>Current version:</source>
        <translation>当前版本:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1649"/>
        <source> (tag %1)</source>
        <translation> (标签 %1)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1650"/>
        <source>Latest version:</source>
        <translation>最新版本:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1659"/>
        <source>Update source:</source>
        <translation>更新源:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1662"/>
        <source>Package:</source>
        <translation>安装包:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1668"/>
        <source>Release notes:</source>
        <translation>更新说明:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1680"/>
        <source>Detected from repository tag %1 (no published release notes yet).</source>
        <translation>检测到仓库标签 %1（暂未发布发行说明）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1692"/>
        <source>Download</source>
        <translation>下载</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1693"/>
        <source>Release Page</source>
        <translation>发布页面</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="269"/>
        <source>&amp;Hardware...</source>
        <translation>硬件设备(&amp;H)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="272"/>
        <source>Show all available devices with status, version and capabilities</source>
        <translation>显示所有可用设备的状态、版本与支持能力</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="78"/>
        <source>(unavailable) %1</source>
        <translation>（不可用）%1</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="124"/>
        <source>Setup</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="126"/>
        <source>Interfaces</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="128"/>
        <source>Database</source>
        <translation>数据库</translation>
    </message>
    <message>
        <location filename="../db/model/CanDbSignal.cpp" line="83"/>
        <source>Start bit %1 (little-endian, LSB), length %2</source>
        <translation>起始位 %1（小端，LSB），长度 %2</translation>
    </message>
    <message>
        <location filename="../db/model/CanDbSignal.cpp" line="86"/>
        <source>Start bit %1 (big-endian, MSB, as in DBC), LSB at bit %2, length %3</source>
        <translation>起始位 %1（大端，MSB，与 DBC 一致），LSB 位于位 %2，长度 %3</translation>
    </message>
    <message>
        <location filename="../main.cpp" line="125"/>
        <source>Workspace file not found: %1</source>
        <translation>未找到工作空间文件：%1</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="49"/>
        <source>Rx Frames</source>
        <translation>接收帧数</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="50"/>
        <source>Rx Errors</source>
        <translation>接收错误</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="51"/>
        <source>Rx Overrun</source>
        <translation>接收溢出</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="52"/>
        <source>Tx Frames</source>
        <translation>发送帧数</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="53"/>
        <source>Tx Errors</source>
        <translation>发送错误</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="54"/>
        <source>Tx Dropped</source>
        <translation>发送丢弃</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="55"/>
        <source>Load (%)</source>
        <translation>负载 (%)</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="56"/>
        <source>Bits</source>
        <translation>位数</translation>
    </message>
</context>
<context>
    <name>RawTxWindow</name>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="72"/>
        <source>Send</source>
        <translation>发送</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="88"/>
        <source>Send Repeat</source>
        <translation>循环发送</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="112"/>
        <source>HHHHHHHH</source>
        <translation>HHHHHHHH</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="159"/>
        <source>DLC</source>
        <translation>DLC</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="191"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="75"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="215"/>
        <source>RTR</source>
        <translation>RTR</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="204"/>
        <source>Extended ID</source>
        <translation>扩展帧</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="60"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="211"/>
        <source>ID (Hex):</source>
        <translation>ID (Hex):</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="68"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="212"/>
        <source>DLC:</source>
        <translation>DLC:</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="74"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="214"/>
        <source>Extended</source>
        <translation>扩展帧</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="86"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="213"/>
        <source>Interface:</source>
        <translation>接口：</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="143"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="218"/>
        <source>Signal</source>
        <translation>信号</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="32"/>
        <source>Message View</source>
        <translation>消息视图</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="45"/>
        <source>Signal Name</source>
        <translation>信号名称</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="50"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="143"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="218"/>
        <source>Value</source>
        <translation>值</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="55"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="143"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="218"/>
        <source>Unit</source>
        <translation>单位</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="146"/>
        <source>ID (Hex)</source>
        <translation>ID (Hex)</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="217"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="77"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="217"/>
        <source>BRS</source>
        <translation>BRS</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="230"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="76"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="216"/>
        <source>FD</source>
        <translation>FD</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="253"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="288"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="307"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="326"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="345"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="364"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="383"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="402"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="421"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="456"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="475"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="510"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="529"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="548"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="567"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="586"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="605"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="640"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="659"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="678"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="697"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="716"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="767"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="802"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="821"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="856"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="875"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="894"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="913"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="948"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="967"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1002"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1021"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1040"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1059"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1094"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1113"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1148"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1167"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1186"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1205"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1224"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1243"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1262"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1281"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1300"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1319"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1338"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1357"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1376"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1395"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1414"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1433"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1452"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1471"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1490"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1509"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1528"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1595"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1614"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1633"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1652"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1671"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1690"/>
        <source>hh</source>
        <extracomment>Hex Only</extracomment>
        <translation>hh</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="256"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="291"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="310"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="329"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="348"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="367"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="386"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="405"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="424"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="459"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="478"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="513"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="532"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="551"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="570"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="589"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="608"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="643"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="662"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="681"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="700"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="719"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="770"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="805"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="824"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="859"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="878"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="897"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="916"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="951"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="970"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1005"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1024"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1043"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1062"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1097"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1116"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1151"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1170"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1189"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1208"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1227"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1246"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1265"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1284"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1303"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1322"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1341"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1360"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1379"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1398"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1417"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1436"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1455"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1474"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1493"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1512"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1531"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1598"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1617"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1636"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1655"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1674"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1693"/>
        <source>00</source>
        <translation>00</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="272"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1725"/>
        <source>7</source>
        <translation>7</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="440"/>
        <source>56</source>
        <translation>56</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="494"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="932"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1837"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="624"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1757"/>
        <source>2</source>
        <translation>2</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="735"/>
        <source>8</source>
        <translation>8</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="751"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1773"/>
        <source>6</source>
        <translation>6</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="786"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1789"/>
        <source>4</source>
        <translation>4</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="840"/>
        <source>16</source>
        <translation>16</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="986"/>
        <source>32</source>
        <translation>32</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1078"/>
        <source>24</source>
        <translation>24</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1132"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1741"/>
        <source>3</source>
        <translation>3</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1547"/>
        <source>40</source>
        <translation>40</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1563"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1805"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1579"/>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1821"/>
        <source>5</source>
        <translation>5</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="1709"/>
        <source>48</source>
        <translation>48</translation>
    </message>
</context>
<context>
    <name>RecordingDialog</name>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="41"/>
        <source>Record Trace</source>
        <translation>录制跟踪</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="47"/>
        <source>Output</source>
        <translation>输出</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="52"/>
        <source>Browse...</source>
        <translation>浏览...</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="55"/>
        <source>Folder:</source>
        <translation>文件夹：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="58"/>
        <source>Placeholders: {date}, {time} (recording start) and {index} (file number when splitting)</source>
        <translation>占位符：{date}、{time} (录制开始时间) 和 {index} (分割时的文件编号)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="59"/>
        <source>File name:</source>
        <translation>文件名：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="62"/>
        <source>Vector ASC (*.asc)</source>
        <translation>Vector ASC (*.asc)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="63"/>
        <source>CAN/FD Raw CSV (*.csv)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="64"/>
        <source>Linux candump (*.candump)</source>
        <translation>Linux candump (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="65"/>
        <source>PCAPng (*.pcapng)</source>
        <translation>PCAPng (*.pcapng)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="67"/>
        <source>Format:</source>
        <translation>格式：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="72"/>
        <source>Example:</source>
        <translation>示例：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="77"/>
        <source>Split Files</source>
        <translation>分割文件</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="80"/>
        <source>Start a new file every</source>
        <translation>每隔以下大小开始新文件</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="84"/>
        <source> MB</source>
        <translation> MB</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="93"/>
        <source>Start</source>
        <translation>开始</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="96"/>
        <source>Record every measurement (stay armed after stop)</source>
        <translation>录制每次测量 (停止后保持就绪)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="100"/>
        <source>Also record during offline trace replay</source>
        <translation>离线跟踪回放期间同时录制</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="104"/>
        <source>Recording starts with the next measurement, or immediately if one is running. Changed options apply from the next recording.</source>
        <translation>录制将随下一次测量开始；如果测量正在运行，则立即开始。更改的选项将从下一次录制起生效。</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="141"/>
        <source>Recording Folder</source>
        <translation>录制文件夹</translation>
    </message>
</context>
<context>
    <name>ReplayWindow</name>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="68"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="194"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="273"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="588"/>
        <source>Play</source>
        <translation>播放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="69"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="195"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="81"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="204"/>
        <source>Playback speed multiplier</source>
        <translation>回放速度倍数</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="83"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="205"/>
        <source>Autoplay</source>
        <translation>自动播放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="84"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="206"/>
        <source>Automatically start/stop replay with measurement</source>
        <translation>随测量自动启动/停止回放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="86"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="207"/>
        <source>Loop</source>
        <translation>循环</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="87"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="208"/>
        <source>Restart replay after all messages were sent</source>
        <translation>所有消息发送完毕后重新开始回放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="75"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="203"/>
        <source>Speed:</source>
        <translation>速度:</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="67"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="193"/>
        <source>Load...</source>
        <translation>加载...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="70"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="196"/>
        <source>Step +1</source>
        <translation>单步 +1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="105"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="209"/>
        <source>No trace file loaded</source>
        <translation>未加载跟踪文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="106"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="197"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="644"/>
        <source>Load to Trace</source>
        <translation>载入跟踪</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="107"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="198"/>
        <source>Import enabled messages directly into Trace and Graph</source>
        <translation>将已启用的报文直接导入跟踪与图表</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="109"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="199"/>
        <source>Split Save...</source>
        <translation>分段保存...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="110"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="200"/>
        <source>Split large trace into smaller files by frame count</source>
        <translation>按帧数将大型跟踪分割为多个较小文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="141"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="210"/>
        <source>Trace file info...</source>
        <translation>跟踪文件信息...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="147"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="201"/>
        <source>Select All</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="148"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="202"/>
        <source>Deselect All</source>
        <translation>取消全选</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>Interface / CAN ID</source>
        <translation>接口 / CAN ID</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>TX</source>
        <translation>TX</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>Count</source>
        <translation>数量</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="155"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>Output</source>
        <translation>输出</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="194"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="563"/>
        <source>Pause</source>
        <translation>暂停</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="194"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="550"/>
        <source>Resume</source>
        <translation>继续</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="218"/>
        <source>Load Trace File</source>
        <translation>加载跟踪文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="220"/>
        <source>All Supported (*.asc *.candump *.csv *.pcap *.pcapng *.mf4);;Vector ASC (*.asc);;Linux candump (*.candump);;CAN/FD Raw CSV (*.csv);;PCAP (*.pcap);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4);;All Files (*)</source>
        <translation>所有支持的格式 (*.asc *.candump *.csv *.pcap *.pcapng *.mf4);;Vector ASC (*.asc);;Linux candump (*.candump);;CAN/FD 原始帧 CSV (*.csv);;PCAP (*.pcap);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4);;所有文件 (*)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="243"/>
        <source>Failed to parse trace file or file is empty.</source>
        <translation>解析跟踪文件失败，或文件为空。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="245"/>
        <source>Error: %1</source>
        <translation>错误：%1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="246"/>
        <source>ReplayWindow: failed to load %1: %2</source>
        <translation>回放窗口：加载 %1 失败：%2</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="285"/>
        <source>Warnings: %1</source>
        <translation>警告：%1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="644"/>
        <source>No enabled messages match the current filter.</source>
        <translation>没有符合当前过滤条件的已启用报文。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="653"/>
        <source>Large Trace File</source>
        <translation>大型跟踪文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="654"/>
        <source>Selected trace contains %1 messages, exceeding the in-memory limit (%2).</source>
        <translation>所选跟踪包含 %1 条报文，超过内存上限（%2）。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="657"/>
        <source>You can split it into smaller files, or load the first %1 messages into Trace.</source>
        <translation>可将其分割为多个较小文件，或仅将前 %1 条报文载入跟踪。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="658"/>
        <source>Split &amp; Save...</source>
        <translation>分段保存...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="659"/>
        <source>Load First %1</source>
        <translation>加载前 %1 条</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="673"/>
        <source>Loaded %1 replay messages into Trace.</source>
        <translation>已将 %1 条回放报文载入跟踪。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="686"/>
        <source>Split Trace by Frame Count</source>
        <translation>按帧数分割跟踪</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="687"/>
        <source>Maximum frames per output file:</source>
        <translation>每个输出文件的最大帧数：</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="699"/>
        <source>Save Split Trace Files</source>
        <translation>保存分段跟踪文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="701"/>
        <source>CAN/FD Raw CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;PCAPng (*.pcapng);;Vector MDF4 (*.mf4)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="728"/>
        <source>Split Save Failed</source>
        <translation>分段保存失败</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="728"/>
        <source>No files written.</source>
        <translation>未写入任何文件。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="733"/>
        <source>Split Save Complete</source>
        <translation>分段保存完成</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="734"/>
        <source>Saved %1 split file(s) starting at:
%2</source>
        <translation>已保存 %1 个分段文件，起始文件：
%2</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="280"/>
        <source>File: %1
Messages: %2
Duration: %3 s</source>
        <translation>文件：%1
消息数：%2
持续时间：%3 s</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="330"/>
        <source>%1 (LIN)</source>
        <translation>%1 (LIN)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="330"/>
        <source>%1 (CAN)</source>
        <translation>%1 (CAN)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="336"/>
        <source>Trace only</source>
        <translation>仅跟踪</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="346"/>
        <source>LIN frames can only be replayed to the trace</source>
        <translation>LIN帧只能回放到跟踪记录中</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="362"/>
        <source>ERROR</source>
        <translation>错误</translation>
    </message>
</context>
<context>
    <name>SLCANInterface</name>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="135"/>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="145"/>
        <source>CANable 2.5 (ElmueSoft Slcan firmware) with CANFD support</source>
        <translation>CANable 2.5（ElmueSoft Slcan 固件），支持 CAN FD</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="139"/>
        <source>CANable with CANFD support</source>
        <translation>支持 CAN-FD 的 CANable</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="140"/>
        <source>CANable with standard CAN support</source>
        <translation>支持标准 CAN 的 CANable</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="142"/>
        <source>WeAct Studio USB2CAN with CANFD support</source>
        <translation>支持 CAN-FD 的 WeAct Studio USB2CAN</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="143"/>
        <source>WeAct Studio USB2CAN with standard CAN support</source>
        <translation>支持标准 CAN 的 WeAct Studio USB2CAN</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="147"/>
        <source>SLCAN device</source>
        <translation>SLCAN设备</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="298"/>
        <source>SLCAN: %1 refused &quot;%2&quot;</source>
        <translation>SLCAN: %1 拒绝了 "%2"</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="308"/>
        <source>SLCAN: %1 did not answer &quot;%2&quot;</source>
        <translation>SLCAN: %1 未响应 "%2"</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="471"/>
        <source>SLCAN: failed to open %1: %2</source>
        <translation>SLCAN: 打开 %1 失败: %2</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="488"/>
        <source>SLCAN: %1 cannot run listen only, the channel opens in normal mode and acknowledges frames</source>
        <translation>SLCAN: %1 无法运行仅监听模式，通道将以正常模式打开并对帧进行应答</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="495"/>
        <source>SLCAN: %1 did not open the CAN channel</source>
        <translation>SLCAN: %1 未能打开 CAN 通道</translation>
    </message>
    <message>
        <location filename="../driver/SLCANDriver/SLCANInterface.cpp" line="780"/>
        <source>SLCAN: connection to %1 lost (%2)</source>
        <translation>SLCAN: 与 %1 的连接已丢失（%2）</translation>
    </message>
</context>
<context>
    <name>ScriptWindow</name>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="52"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="149"/>
        <source>Run</source>
        <translation>运行</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="53"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="150"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="54"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="151"/>
        <source>Clear</source>
        <translation>清空</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="55"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="152"/>
        <source>Load</source>
        <translation>加载</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="56"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="153"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="58"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="154"/>
        <source>AutoRun</source>
        <translation>AutoRun</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="59"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="155"/>
        <source>Start script with measurement</source>
        <translation>随测量启动脚本</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="75"/>
        <source>No script loaded</source>
        <translation>未加载脚本</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="87"/>
        <source># Python script
import cangaroo

for msg in cangaroo.get_trace(10):
    print(msg)</source>
        <translation># Python 脚本
import cangaroo

for msg in cangaroo.get_trace(10):
    print(msg)</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="93"/>
        <source>Script output...</source>
        <translation>脚本输出...</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="97"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="156"/>
        <source>Script input (press Enter to send)...</source>
        <translation>脚本输入 (按 Enter 发送)...</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="202"/>
        <source>Load Python Script</source>
        <translation>加载 Python 脚本</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="203"/>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="212"/>
        <source>Python Files (*.py);;All Files (*)</source>
        <translation>Python 文件 (*.py);;所有文件 (*)</translation>
    </message>
    <message>
        <location filename="../window/ScriptWindow/ScriptWindow.cpp" line="211"/>
        <source>Save Python Script</source>
        <translation>保存 Python 脚本</translation>
    </message>
</context>
<context>
    <name>SelectCanInterfacesDialog</name>
    <message>
        <location filename="../window/SetupDialog/SelectCanInterfacesDialog.ui" line="14"/>
        <source>Select CAN Interface(s)</source>
        <translation>选择 CAN 接口</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SelectCanInterfacesDialog.ui" line="82"/>
        <source>&amp;Create Interface...</source>
        <translation>&amp;创建接口...</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="36"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="42"/>
        <source>Appearance</source>
        <translation>外观</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="45"/>
        <source>Use native system styling (GNOME/Adwaita)</source>
        <translation>使用系统原生样式 (GNOME/Adwaita)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="46"/>
        <source>Defer to the selected style and the desktop theme instead of applying CANgaroo&apos;s own palette and colors.</source>
        <translation>使用所选样式和桌面主题，而不是 CANgaroo 自带的调色板和颜色。</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="63"/>
        <source>Theme:</source>
        <translation>主题:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="80"/>
        <source>Language:</source>
        <translation>语言:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="88"/>
        <source>Font size:</source>
        <translation>字体大小:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="93"/>
        <source>Behavior</source>
        <translation>行为</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="96"/>
        <source>Restore window layout on startup</source>
        <translation>启动时恢复窗口布局</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="100"/>
        <source>Clear trace on measurement start</source>
        <translation>测量开始时清除跟踪</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="104"/>
        <source>Do not ask to save workspace on close (always discard)</source>
        <translation>关闭时不询问是否保存工作区 (始终丢弃)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="111"/>
        <source>Trace Window</source>
        <translation>跟踪窗口</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="115"/>
        <source>Aggregated</source>
        <translation>聚合</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="116"/>
        <source>Rolling Log</source>
        <translation>滚动日志</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="118"/>
        <source>Default view:</source>
        <translation>默认视图：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="121"/>
        <source>Absolute</source>
        <translation>绝对</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="122"/>
        <source>Absolute (UTC)</source>
        <translation>绝对 (UTC)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="123"/>
        <source>Relative</source>
        <translation>相对</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="124"/>
        <source>Delta</source>
        <translation>增量</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="136"/>
        <source>Default timestamp:</source>
        <translation>默认时间戳：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="139"/>
        <source>Hex</source>
        <translation>十六进制</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="140"/>
        <source>ASCII</source>
        <translation>ASCII</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="142"/>
        <source>Data display:</source>
        <translation>数据显示：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="156"/>
        <source>Preferred save format:</source>
        <translation>首选保存格式：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="162"/>
        <source> messages</source>
        <translation> 条消息</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="163"/>
        <source>Maximum number of messages kept in memory. When exceeded, the oldest messages are discarded and are missing from saved traces. Larger values use more memory.</source>
        <translation>内存中保留的最大消息数。超出后，最早的消息将被丢弃，且不会出现在保存的跟踪中。数值越大占用内存越多。</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="167"/>
        <source>Max trace size:</source>
        <translation>最大跟踪大小：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="172"/>
        <source>Decoders</source>
        <translation>解码器</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="175"/>
        <source>Decode UDS on 29-bit (extended) CAN IDs</source>
        <translation>在29位（扩展）CAN ID上解码UDS</translation>
    </message>
</context>
<context>
    <name>SetupDialog</name>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="14"/>
        <source>Measurement Setup</source>
        <translation>测量设置</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="91"/>
        <source>Add Network</source>
        <translation>添加网络</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="98"/>
        <source>Remove Network</source>
        <translation>移除网络</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="105"/>
        <source>Refresh</source>
        <translation>刷新</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="130"/>
        <source>Network name:</source>
        <translation>网络名称:</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="156"/>
        <source>Interfaces assigned to this network:</source>
        <translation>分配到此网络的接口:</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="259"/>
        <source>Reload DBC</source>
        <translation>重新加载DBC</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="181"/>
        <source>Add &amp;Interface...</source>
        <translation>&amp;添加接口...</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="188"/>
        <source>&amp;Remove Interface</source>
        <translation>&amp;移除接口</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="239"/>
        <source>Add &amp;Database...</source>
        <translation>&amp;添加数据库...</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.ui" line="249"/>
        <source>&amp;Remove Database</source>
        <translation>&amp;移除数据库</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="305"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="390"/>
        <source>DBC Error</source>
        <translation>DBC 错误</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="306"/>
        <source>Failed to reload one or more DBC files:

%1</source>
        <translation>重新加载一个或多个 DBC 文件失败:

%1</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="312"/>
        <source>Failed to reload one or more LDF files:

%1</source>
        <translation>无法重新加载一个或多个 LDF 文件：

%1</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="350"/>
        <source>Duplicate DBC</source>
        <translation>DBC 重复</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="351"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="460"/>
        <source>The file is already loaded:
%1

Do you want to reload it?</source>
        <translation>文件已加载:
%1

是否要重新加载?</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="373"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="481"/>
        <source>Reload Failed</source>
        <translation>重新加载失败</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="374"/>
        <source>Failed to reload DBC:
%1

Reason: %2</source>
        <translation>重新加载 DBC 失败:
%1

原因: %2</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="387"/>
        <source>DBC Warning</source>
        <translation>DBC 警告</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="391"/>
        <source>Failed to load DBC file:
%1

Reason: %2</source>
        <translation>加载 DBC 文件失败:
%1

原因: %2</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="459"/>
        <source>Duplicate LDF</source>
        <translation>复制LDF</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="482"/>
        <source>Failed to reload LDF:
%1

Reason: %2</source>
        <translation>LDF重新加载失败:
%1

原因: %2</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="496"/>
        <source>LDF Warning</source>
        <translation>LDF警告</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="311"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="499"/>
        <source>LDF Error</source>
        <translation>LDF错误</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="257"/>
        <source>No Network</source>
        <translation>无网络</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="258"/>
        <source>You must Add a Network First!</source>
        <translation>您必须先添加一个网络！</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="500"/>
        <source>Failed to load LDF file:
%1

Reason: %2</source>
        <translation>LDF文件加载失败:
%1

原因: %2</translation>
    </message>
</context>
<context>
    <name>SetupDialogTreeModel</name>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="100"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="101"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="102"/>
        <source>Bitrate</source>
        <translation>波特率</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="103"/>
        <source>Filename</source>
        <translation>文件名</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="104"/>
        <source>Path</source>
        <translation>路径</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="161"/>
        <source>Network </source>
        <translation>网络 </translation>
    </message>
</context>
<context>
    <name>SignalSelectorDialog</name>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="37"/>
        <source>Select Data</source>
        <translation>选择数据</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="45"/>
        <source>Search signals or messages...</source>
        <translation>搜索信号或消息...</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="48"/>
        <source>Show selection only</source>
        <translation>仅显示选中项</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="55"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="55"/>
        <source>Details</source>
        <translation>详情</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="55"/>
        <source>Comment</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="116"/>
        <source>CAN — %1</source>
        <translation>CAN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="145"/>
        <source>LIN — %1</source>
        <translation>LIN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="130"/>
        <source>Unsigned</source>
        <translation>无符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="130"/>
        <source>Signed</source>
        <translation>有符号</translation>
    </message>
</context>
<context>
    <name>TimeSeriesVisualization</name>
    <message>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="573"/>
        <source>Cursors: Left-click = A, Right-click / Shift+Click = B</source>
        <translation>游标：左键 = A，右键 / Shift+左键 = B</translation>
    </message>
</context>
<context>
    <name>TraceFileReader</name>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="155"/>
        <location filename="../core/TraceFileReader.cpp" line="263"/>
        <location filename="../core/TraceFileReader.cpp" line="503"/>
        <location filename="../core/TraceFileReader.cpp" line="745"/>
        <location filename="../core/TraceFileReader.cpp" line="813"/>
        <location filename="../core/TraceFileReader.cpp" line="1130"/>
        <source>Loading cancelled by user.</source>
        <translation>加载已被用户取消。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="170"/>
        <source>Line %1: unrecognized candump syntax</source>
        <translation>第 %1 行：无法识别的 candump 语法</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="240"/>
        <source>No valid candump frames found in file.</source>
        <translation>文件中未找到有效的 candump 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="433"/>
        <source>No valid Vector ASC frames found in file.</source>
        <translation>文件中未找到有效的 Vector ASC 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="458"/>
        <source>CSV trace file is empty.</source>
        <translation>CSV 跟踪文件为空。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="486"/>
        <source>Invalid CSV trace header: missing required CAN columns (Timestamp_s/Epoch_us, ID_Hex, DLC, Data_Hex).</source>
        <translation>CSV 跟踪文件表头无效：缺少必需的 CAN 列（Timestamp_s/Epoch_us、ID_Hex、DLC、Data_Hex）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="535"/>
        <source>CSV line %1: invalid timestamp.</source>
        <translation>CSV 第 %1 行：时间戳无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="550"/>
        <source>CSV line %1: invalid hex ID &apos;%2&apos;.</source>
        <translation>CSV 第 %1 行：十六进制 ID '%2' 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="583"/>
        <source>CSV line %1: invalid DLC &apos;%2&apos;.</source>
        <translation>CSV 第 %1 行：DLC '%2' 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="594"/>
        <source>CSV line %1: data payload shorter than DLC (%2 &lt; %3).</source>
        <translation>CSV 第 %1 行：数据负载短于 DLC（%2 &lt; %3）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="610"/>
        <source>No valid CAN frames found in CSV file.</source>
        <translation>CSV 文件中未找到有效的 CAN 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="703"/>
        <source>PCAP file is truncated (missing global header).</source>
        <translation>PCAP 文件被截断（缺少全局头）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="720"/>
        <source>Invalid PCAP magic number (0x%1).</source>
        <translation>PCAP 魔数无效（0x%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="730"/>
        <source>PCAP global header is truncated.</source>
        <translation>PCAP 全局头被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="735"/>
        <source>Unsupported PCAP linktype %1 (expected 227 / LINKTYPE_CAN_SOCKETCAN).</source>
        <translation>不支持的 PCAP 链路类型 %1（期望 227 / LINKTYPE_CAN_SOCKETCAN）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="763"/>
        <source>Truncated PCAP packet payload.</source>
        <translation>PCAP 数据包负载被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="779"/>
        <source>No valid CAN frames found in PCAP file.</source>
        <translation>PCAP 文件中未找到有效的 CAN 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="838"/>
        <source>Invalid PCAPng byte-order magic (0x%1).</source>
        <translation>PCAPng 字节序魔数无效（0x%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="845"/>
        <source>Invalid PCAPng SHB block length (%1).</source>
        <translation>PCAPng SHB 块长度无效（%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="853"/>
        <source>PCAPng file does not begin with a Section Header Block.</source>
        <translation>PCAPng 文件未以 Section Header Block 开头。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="858"/>
        <source>Corrupted PCAPng block length (%1).</source>
        <translation>PCAPng 块长度已损坏（%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="921"/>
        <source>Truncated PCAPng packet payload.</source>
        <translation>PCAPng 数据包负载被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="944"/>
        <source>Missing PCAPng Section Header Block.</source>
        <translation>缺少 PCAPng Section Header Block。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="949"/>
        <source>No valid CAN frames found in PCAPng file.</source>
        <translation>PCAPng 文件中未找到有效的 CAN 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="989"/>
        <source>Invalid MDF4 header (missing &apos;MDF     &apos; magic).</source>
        <translation>MDF4 文件头无效（缺少 'MDF     ' 魔数）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="995"/>
        <source>Unsupported MDF version %1 (requires MDF 4.x).</source>
        <translation>不支持的 MDF 版本 %1（需要 MDF 4.x）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1002"/>
        <source>MDF4 file truncated before HDBLOCK.</source>
        <translation>MDF4 文件在 HDBLOCK 之前被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1008"/>
        <source>Invalid MDF4 HDBLOCK.</source>
        <translation>MDF4 HDBLOCK 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1021"/>
        <source>MDF4 file has no Data Group (##DG) block.</source>
        <translation>MDF4 文件没有数据组 (##DG) 块。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1027"/>
        <source>Invalid MDF4 DGBLOCK.</source>
        <translation>MDF4 DGBLOCK 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1036"/>
        <source>MDF4 file has missing Channel Group or Data block link.</source>
        <translation>MDF4 文件缺少通道组或数据块链接。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1042"/>
        <source>Invalid MDF4 CGBLOCK.</source>
        <translation>MDF4 CGBLOCK 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1065"/>
        <source>Invalid MDF4 CNBLOCK.</source>
        <translation>MDF4 CNBLOCK 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1085"/>
        <source>Unsupported MDF4 channel layout: expected standard CANgaroo CAN channels (t, CAN_ID, DLC, DataBytes).</source>
        <translation>不支持的 MDF4 通道布局：期望标准 CANgaroo CAN 通道（t、CAN_ID、DLC、DataBytes）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1093"/>
        <source>MDF4 DTBLOCK offset out of range.</source>
        <translation>MDF4 DTBLOCK 偏移超出范围。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1099"/>
        <source>MDF4 DTBLOCK header truncated.</source>
        <translation>MDF4 DTBLOCK 头被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1104"/>
        <source>Compressed MDF4 (##DZ) blocks are not supported.</source>
        <translation>不支持压缩的 MDF4 (##DZ) 块。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1109"/>
        <source>Unsupported MDF4 data block type (expected ##DT).</source>
        <translation>不支持的 MDF4 数据块类型（期望 ##DT）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1116"/>
        <source>MDF4 DTBLOCK is shorter than declared record count.</source>
        <translation>MDF4 DTBLOCK 短于声明的记录数。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1136"/>
        <source>Truncated MDF4 record at index %1.</source>
        <translation>MDF4 记录在索引 %1 处被截断。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1181"/>
        <source>MDF4 file contains no CAN records.</source>
        <translation>MDF4 文件不包含 CAN 记录。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1206"/>
        <source>Unsupported trace file format.</source>
        <translation>不支持的跟踪文件格式。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1216"/>
        <source>Unknown trace file extension for &apos;%1&apos;.</source>
        <translation>无法识别 '%1' 的跟踪文件扩展名。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1224"/>
        <source>Cannot open &apos;%1&apos;: %2</source>
        <translation>无法打开 '%1'：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1241"/>
        <source>Invalid split frame count (%1).</source>
        <translation>分段帧数无效（%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1246"/>
        <source>No messages to split.</source>
        <translation>没有可分割的报文。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1251"/>
        <source>Cannot create output directory &apos;%1&apos;.</source>
        <translation>无法创建输出目录 '%1'。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1292"/>
        <source>Cannot write &apos;%1&apos;: %2</source>
        <translation>无法写入 '%1'：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="1307"/>
        <source>Write error on &apos;%1&apos;: %2</source>
        <translation>写入 '%1' 时出错：%2</translation>
    </message>
</context>
<context>
    <name>TraceFilterDialog</name>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="41"/>
        <source>Trace Filter</source>
        <translation>跟踪过滤器</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="47"/>
        <source>Direction</source>
        <translation>方向</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="49"/>
        <source>Show TX</source>
        <translation>显示TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="50"/>
        <source>Show RX</source>
        <translation>显示RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="62"/>
        <source>Messages</source>
        <translation>消息</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="66"/>
        <source>Select All</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="67"/>
        <source>Deselect All</source>
        <translation>取消全选</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="81"/>
        <source>Interfaces</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="92"/>
        <source>Clear Filters</source>
        <translation>清除过滤器</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="140"/>
        <source>[LIN] %1</source>
        <translation>[LIN] %1</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="152"/>
        <source>(no databases loaded)</source>
        <translation>（未加载数据库）</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="171"/>
        <source>(no interfaces)</source>
        <translation>（无接口）</translation>
    </message>
</context>
<context>
    <name>TraceRecorder</name>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="186"/>
        <source>%1 cannot be recorded continuously</source>
        <translation>%1 无法连续录制</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="210"/>
        <source>Recording trace to %1</source>
        <translation>正在录制跟踪到 %1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="229"/>
        <source>Recording stopped: %1 frames (%2) in %3 file(s)</source>
        <translation>录制已停止：%3 个文件中共 %1 帧 (%2)</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="241"/>
        <source>Recording could not keep up with the bus: %1 frames were dropped</source>
        <translation>录制跟不上总线速度：已丢弃 %1 帧</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="257"/>
        <source>Trace recording failed: %1</source>
        <translation>跟踪录制失败：%1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="287"/>
        <source>Recording continues in %1</source>
        <translation>录制继续写入 %1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="351"/>
        <location filename="../core/TraceRecorder.cpp" line="365"/>
        <location filename="../core/TraceRecorder.cpp" line="427"/>
        <source>cannot write %1: %2</source>
        <translation>无法写入 %1：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="377"/>
        <source>cannot create folder &apos;%1&apos;</source>
        <translation>无法创建文件夹 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="385"/>
        <source>cannot open %1: %2</source>
        <translation>无法打开 %1：%2</translation>
    </message>
</context>
<context>
    <name>TraceWindow</name>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="14"/>
        <source>Trace View</source>
        <translation>跟踪视图</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="35"/>
        <source>View:</source>
        <translation>视图:</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="45"/>
        <source>Timestamps:</source>
        <translation>时间戳:</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="68"/>
        <source>Autoscroll</source>
        <translation>自动滚动</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="101"/>
        <source>Search:</source>
        <translation>搜索:</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="120"/>
        <source>Filter rows by CAN ID, message name, data, or regular expression</source>
        <translation>按 CAN ID、报文名称、数据或正则表达式过滤行</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="126"/>
        <source>ID / Name / Regex...</source>
        <translation>ID / 名称 / 正则...</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="154"/>
        <source>Filter</source>
        <translation>过滤器</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="175"/>
        <source>Monitor</source>
        <translation>监控</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="189"/>
        <source>UDS Protocol</source>
        <translation>UDS 协议</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="203"/>
        <source>J1939</source>
        <translation>J1939</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="84"/>
        <source>Clear</source>
        <translation>清空</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="141"/>
        <source>Aggregated</source>
        <translation>聚合视图</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="142"/>
        <source>Rolling Log</source>
        <translation>滚动日志</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="144"/>
        <source>Absolute</source>
        <translation>绝对时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="145"/>
        <source>Absolute (UTC)</source>
        <translation>绝对时间 (UTC)</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="146"/>
        <source>Relative</source>
        <translation>相对时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="147"/>
        <source>Delta</source>
        <translation>Delta</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="208"/>
        <source>Create Trigger Rule for ID 0x%1...</source>
        <translation>为 ID 0x%1 创建触发规则...</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="209"/>
        <source>Create Watchdog Trigger (200 ms) for ID 0x%1...</source>
        <translation>为 ID 0x%1 创建看门狗触发（200 ms）...</translation>
    </message>
</context>
<context>
    <name>TriggerCaptureDialog</name>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="63"/>
        <source>Trigger Capture (Blackbox)</source>
        <translation>触发捕获（黑匣子）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="68"/>
        <source>Output &amp; Capture Window</source>
        <translation>输出与捕获窗口</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="76"/>
        <source>Browse...</source>
        <translation>浏览...</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="80"/>
        <source>Output Folder:</source>
        <translation>输出文件夹：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="85"/>
        <source>CAN/FD Raw CSV (*.csv)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="86"/>
        <source>Vector ASC (*.asc)</source>
        <translation>Vector ASC (*.asc)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="87"/>
        <source>SocketCAN candump (*.candump)</source>
        <translation>SocketCAN candump (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="88"/>
        <source>PCAPng (*.pcapng)</source>
        <translation>PCAPng (*.pcapng)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="89"/>
        <source>Vector MDF4 (*.mf4)</source>
        <translation>Vector MDF4 (*.mf4)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="96"/>
        <source>Prefix / Format:</source>
        <translation>前缀 / 格式：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="116"/>
        <source>Pre:</source>
        <translation>触发前：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="118"/>
        <source>Post:</source>
        <translation>触发后：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="120"/>
        <source>Max Pre-Frames:</source>
        <translation>最大预缓冲帧数：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="122"/>
        <source>Ring Buffer:</source>
        <translation>环形缓冲：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="131"/>
        <source>One-shot (disarm after first capture)</source>
        <translation>单次（首次捕获后自动撤防）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="134"/>
        <source>Arm trigger capture on OK</source>
        <translation>点击确定后启用触发捕获</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="137"/>
        <source>Holdoff:</source>
        <translation>保持时间：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="142"/>
        <source>Behavior:</source>
        <translation>行为：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="146"/>
        <source>Trigger Rules (double-click a rule to edit)</source>
        <translation>触发规则（双击规则进行编辑）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="150"/>
        <source>On</source>
        <translation>启用</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="150"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="150"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="150"/>
        <source>Condition</source>
        <translation>条件</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="168"/>
        <source>Add Rule...</source>
        <translation>添加规则...</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="169"/>
        <source>Remove Rule</source>
        <translation>删除规则</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="182"/>
        <source>Trigger Capture</source>
        <translation>触发捕获</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="182"/>
        <source>Please select an output folder.</source>
        <translation>请选择输出文件夹。</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="244"/>
        <source>Select Trigger Output Folder</source>
        <translation>选择触发输出文件夹</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="253"/>
        <source>Rule %1</source>
        <translation>规则 %1</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="306"/>
        <source>HW Bus Error</source>
        <translation>硬件总线错误</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="307"/>
        <source>Frame Match</source>
        <translation>帧匹配</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="308"/>
        <source>Watchdog Timeout</source>
        <translation>看门狗超时</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="309"/>
        <source>DBC Signal</source>
        <translation>DBC 信号</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="311"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="318"/>
        <source>Any CAN/FD error frame</source>
        <translation>任意 CAN/FD 错误帧</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="318"/>
        <source>Error mask 0x%1</source>
        <translation>错误掩码 0x%1</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="331"/>
        <source>ID 0x%1..0x%2 silent &gt; %3 ms</source>
        <translation>ID 0x%1..0x%2 静默超过 %3 ms</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="348"/>
        <source>Edit Trigger Rule</source>
        <translation>编辑触发规则</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="353"/>
        <source>HW Bus Error Frame</source>
        <translation>硬件总线错误帧</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="354"/>
        <source>Software Frame / Mask Match</source>
        <translation>软件帧 / 掩码匹配</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="355"/>
        <source>Watchdog Frame-Loss Timeout</source>
        <translation>看门狗丢帧超时</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="356"/>
        <source>DBC Signal Threshold</source>
        <translation>DBC 信号阈值</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="363"/>
        <source>Disabled</source>
        <translation>已禁用</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="384"/>
        <source>Optional DBC message name</source>
        <translation>可选的 DBC 报文名称</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="386"/>
        <source>DBC signal name (required)</source>
        <translation>DBC 信号名称（必填）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="392"/>
        <source>Rule Name:</source>
        <translation>规则名称：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="393"/>
        <source>Trigger Kind:</source>
        <translation>触发类型：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="394"/>
        <source>ID Min (hex):</source>
        <translation>最小 ID（十六进制）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="395"/>
        <source>ID Max (hex):</source>
        <translation>最大 ID（十六进制）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="396"/>
        <source>Byte Index (-1 off):</source>
        <translation>字节索引（-1 为关闭）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="397"/>
        <source>Byte Mask (hex):</source>
        <translation>字节掩码（十六进制）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="398"/>
        <source>Byte Value (hex):</source>
        <translation>字节值（十六进制）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="399"/>
        <source>Compare Op:</source>
        <translation>比较运算：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="400"/>
        <source>Watchdog Timeout:</source>
        <translation>看门狗超时：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="401"/>
        <source>DBC Message:</source>
        <translation>DBC 报文：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="402"/>
        <source>DBC Signal:</source>
        <translation>DBC 信号：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="403"/>
        <source>Signal Threshold:</source>
        <translation>信号阈值：</translation>
    </message>
</context>
<context>
    <name>TriggerCaptureManager</name>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="255"/>
        <source>Watchdog timeout on 0x%1 (&gt; %2 ms)</source>
        <translation>0x%1 看门狗超时（&gt; %2 ms）</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="332"/>
        <source>Bus error frame triggered</source>
        <translation>总线错误帧触发</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="354"/>
        <source>Frame match on 0x%1</source>
        <translation>帧匹配：0x%1</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="366"/>
        <source>Signal %1.%2 = %3</source>
        <translation>信号 %1.%2 = %3</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="389"/>
        <source>Trigger Capture fired: %1 (pre-buffer: %2 frames)</source>
        <translation>触发捕获已触发：%1（预缓冲：%2 帧）</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="419"/>
        <source>Trigger capture failed: cannot create output folder &apos;%1&apos;.</source>
        <translation>触发捕获失败：无法创建输出文件夹 '%1'。</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="434"/>
        <source>Trigger capture failed: cannot open &apos;%1&apos;: %2</source>
        <translation>触发捕获失败：无法打开 '%1'：%2</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="448"/>
        <source>Trigger capture write error on &apos;%1&apos;: %2</source>
        <translation>触发捕获写入 '%1' 时出错：%2</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="460"/>
        <source>Trigger capture saved %1 frames to %2 (%3)</source>
        <translation>触发捕获已将 %1 帧保存到 %2（%3）</translation>
    </message>
</context>
<context>
    <name>TxGeneratorWindow</name>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="20"/>
        <source>Generator</source>
        <translation>生成器</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="28"/>
        <source>Interface:</source>
        <translation>接口:</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="64"/>
        <source>Available Messages (DBC)</source>
        <translation>可用消息 (DBC)</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="74"/>
        <source>Message List</source>
        <translation>消息列表</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="80"/>
        <source>Message Name or ID...</source>
        <translation>消息名称或 ID...</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="88"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="280"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="102"/>
        <source>Layout View</source>
        <translation>布局视图</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="110"/>
        <source>Zoom:</source>
        <translation>缩放:</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="133"/>
        <source>Compact Mode</source>
        <translation>紧凑模式</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="192"/>
        <source>Add DBC Message</source>
        <translation>添加DBC消息</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="212"/>
        <source>ID (Hex):</source>
        <translation>ID (Hex):</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="225"/>
        <source>HHHHHHHH</source>
        <translation>HHHHHHHH</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="228"/>
        <source>000</source>
        <translation>000</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="235"/>
        <source>DLC:</source>
        <translation>DLC:</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="252"/>
        <source>Add Manual</source>
        <translation>手动添加</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="262"/>
        <source>Active Cyclic Transmissions</source>
        <translation>活动的周期发送</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="275"/>
        <source>Status</source>
        <translation>状态</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="290"/>
        <source>Interface</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="295"/>
        <source>DLC</source>
        <translation>DLC</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="300"/>
        <source>Interval (ms)</source>
        <translation>间隔 (ms)</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="310"/>
        <source>RUN</source>
        <translation>RUN</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="320"/>
        <source>STOP</source>
        <translation>STOP</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="330"/>
        <source>Interval:</source>
        <translation>间隔:</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="367"/>
        <source>Send</source>
        <translation>发送</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="374"/>
        <source>Select All Rows</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="381"/>
        <source>Deselect All</source>
        <translation>取消全选</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="388"/>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="93"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="285"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="127"/>
        <source>Randomize Data</source>
        <translation>随机化数据</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="128"/>
        <source>Randomize data bytes for selected messages</source>
        <translation>为选中消息随机化数据字节</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="618"/>
        <source>Edit Message: %1 - %2</source>
        <translation>编辑消息: %1 - %2</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="806"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="850"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="806"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="850"/>
        <source>Start</source>
        <translation>启动</translation>
    </message>
</context>
<context>
    <name>UnifiedTraceViewModel</name>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="456"/>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="506"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="456"/>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="506"/>
        <source>TX</source>
        <translation>TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="460"/>
        <source>Tester</source>
        <translation>Tester</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="460"/>
        <source>ECU</source>
        <translation>ECU</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="532"/>
        <source>Sleep</source>
        <translation>休眠</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="533"/>
        <source>Wakeup</source>
        <translation>唤醒</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="540"/>
        <source>[tp] Single Frame</source>
        <translation>[tp] Single Frame</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="541"/>
        <source>[tp] First Frame</source>
        <translation>[tp] First Frame</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="542"/>
        <source>[tp] Consecutive Frame (SN: %1)</source>
        <translation>[tp] Consecutive Frame (SN: %1)</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="543"/>
        <source>[tp] Flow Control</source>
        <translation>[tp] Flow Control</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="547"/>
        <source>[raw]</source>
        <translation>[raw]</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="399"/>
        <source>Invalid release metadata received from GitHub (%1).</source>
        <translation>从 GitHub 收到的发行版元数据无效 (%1)。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="408"/>
        <source>Latest release entry in %1 is still marked as draft.</source>
        <translation>%1 中的最新发布记录仍处于草稿状态。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="422"/>
        <source>Could not parse release version &quot;%1&quot; from %2.</source>
        <translation>无法解析来自 %2 的版本号 &quot;%1&quot;。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="464"/>
        <source>Invalid tag metadata received from GitHub (%1).</source>
        <translation>从 GitHub 收到的标签元数据无效 (%1)。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="492"/>
        <source>No published releases or version tags were found in %1.</source>
        <translation>在 %1 中未找到已发布的版本或版本标签。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="590"/>
        <source>GitHub API rate limit exceeded. Please try again later or check the releases page directly.</source>
        <translation>GitHub API 请求频率受限，请稍后再试或直接打开发布页面查看。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="597"/>
        <source>HTTP %1: %2</source>
        <translation>HTTP %1: %2</translation>
    </message>
</context>
</TS>
