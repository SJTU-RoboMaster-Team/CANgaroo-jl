<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>Backend</name>
    <message>
        <location filename="../core/Backend.cpp" line="122"/>
        <source>Starting measurement</source>
        <translation>开始测量</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="158"/>
        <source>Listening on interface: %1</source>
        <translation>监听接口: %1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="183"/>
        <source>Closing interface: %1</source>
        <translation>关闭接口: %1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="193"/>
        <source>Measurement stopped</source>
        <translation>测量已停止</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="256"/>
        <source>All active interfaces disconnected. Measurement paused (trace preserved).</source>
        <translation>所有活动接口均已断开，测量已暂停（Trace 数据已保留）。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="284"/>
        <source>Missing: %1</source>
        <translation>缺失：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="287"/>
        <source>Ambiguous: %1</source>
        <translation>存在歧义：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="290"/>
        <source>No configured interfaces found to rebind.</source>
        <translation>未找到可重新绑定的已配置接口。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="293"/>
        <source>Manual rebind failed: %1</source>
        <translation>手动重新绑定失败：%1</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="314"/>
        <source>Network </source>
        <translation>网络 </translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="431"/>
        <source>Unable to get driver for interface id: %1. This should never happen.</source>
        <translation>无法获取接口 ID: %1 的驱动程序。这不应该发生。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="483"/>
        <source>File not found or not readable.</source>
        <translation>文件未找到或不可读。</translation>
    </message>
    <message>
        <location filename="../core/Backend.cpp" line="493"/>
        <source>Failed to parse DBC file. Please check the log for details.</source>
        <translation>解析 DBC 文件失败。请查看日志了解详情。</translation>
    </message>
</context>
<context>
    <name>BaseTraceViewModel</name>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="77"/>
        <source>Index</source>
        <translation>序号</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="79"/>
        <source>Time</source>
        <translation>时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="81"/>
        <source>Channel</source>
        <translation>通道</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="83"/>
        <source>RX/TX</source>
        <translation>RX/TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="85"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="89"/>
        <source>Sender</source>
        <translation>发送方</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="91"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="95"/>
        <source>Data</source>
        <translation>数据</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="97"/>
        <source>Comment</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="249"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/BaseTraceViewModel.cpp" line="249"/>
        <source>TX</source>
        <translation>TX</translation>
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
        <location filename="../core/BusTrace.cpp" line="172"/>
        <source>Trace exceeded %1 messages; oldest messages are being discarded and will be missing from saved traces. Increase the limit in Settings.</source>
        <translation>Trace 已超过 %1 条报文；最早的报文将被丢弃，且不会出现在保存的 Trace 中。请在设置中提高上限。</translation>
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
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="137"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="207"/>
        <source>Recover from bus off</source>
        <translation>从 Bus-Off 恢复</translation>
    </message>
    <message>
        <location filename="../window/CanStatusWindow/CanStatusWindow.cpp" line="244"/>
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
        <location filename="../driver/GenericCanSetupPage.cpp" line="90"/>
        <location filename="../driver/GenericCanSetupPage.cpp" line="117"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.cpp" line="91"/>
        <location filename="../driver/GenericCanSetupPage.cpp" line="118"/>
        <source>(unavailable) %1</source>
        <translation>（不可用）%1</translation>
    </message>
    <message>
        <location filename="../driver/GenericCanSetupPage.cpp" line="92"/>
        <location filename="../driver/GenericCanSetupPage.cpp" line="119"/>
        <source>Device is offline. Reconnect USB and use &apos;Rebind &amp; Resume&apos; or Rescan.</source>
        <translation>设备已离线。请重新连接 USB，然后使用“重新绑定并恢复”或重新扫描。</translation>
    </message>
</context>
<context>
    <name>GraphWindow</name>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.ui" line="14"/>
        <source>Visualization View</source>
        <translation>Graph 视图</translation>
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
        <translation>搜索信号/报文...</translation>
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
        <translation>添加到 Graph</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="371"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="431"/>
        <source>Columns:</source>
        <translation>列数：</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="434"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="463"/>
        <source>Graph (Time-series)</source>
        <translation>图表（时间序列）</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="435"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="463"/>
        <source>Scatter Chart</source>
        <translation>散点图</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="436"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="463"/>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="437"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="463"/>
        <source>Gauge</source>
        <translation>仪表盘</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="451"/>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="988"/>
        <source>MSG</source>
        <translation>报文</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1022"/>
        <source>CAN — %1</source>
        <translation>CAN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1065"/>
        <source>Motor (built-in)</source>
        <translation>电机（内置）</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1004"/>
        <source>Unsigned</source>
        <translation>无符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1004"/>
        <source>Signed</source>
        <translation>有符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1037"/>
        <source>Bus Load — %1</source>
        <translation>总线负载 — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1044"/>
        <source>Virtual</source>
        <translation>虚拟</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1050"/>
        <source>0..100 %</source>
        <translation>0..100 %</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/GraphWindow.cpp" line="1051"/>
        <source>1 s rolling window</source>
        <translation>1 秒滚动窗口</translation>
    </message>
</context>
<context>
    <name>HardwareDialog</name>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="60"/>
        <source>CAN FD</source>
        <translation>CAN FD</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="61"/>
        <source>Listen only</source>
        <translation>仅监听</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="62"/>
        <source>Triple sampling</source>
        <translation>三倍采样</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="63"/>
        <source>One shot</source>
        <translation>单次发送</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="64"/>
        <source>Auto restart</source>
        <translation>自动重连</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="65"/>
        <source>Configurable timestamps</source>
        <translation>可配置时间戳</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="66"/>
        <source>Custom bitrate</source>
        <translation>自定义波特率</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="67"/>
        <source>Custom CAN FD bitrate</source>
        <translation>自定义 CAN FD 波特率</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="68"/>
        <source>Bus-off recovery</source>
        <translation>Bus-Off 恢复</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="69"/>
        <source>TDC</source>
        <translation>TDC</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="85"/>
        <source>&amp;Rescan</source>
        <translation>重新扫描(&amp;R)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="88"/>
        <source>Hardware</source>
        <translation>硬件设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="92"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="92"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="284"/>
        <source>Bus</source>
        <translation>总线</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="92"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="286"/>
        <source>Status</source>
        <translation>状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="92"/>
        <source>Bus State</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="93"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="285"/>
        <source>Version</source>
        <translation>版本</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="93"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="314"/>
        <source>Capabilities</source>
        <translation>支持能力</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="93"/>
        <source>Network</source>
        <translation>所属网络</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="111"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="380"/>
        <source>Ask every driver to look for new or removed devices</source>
        <translation>通知所有驱动重新扫描新增或移除的设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="113"/>
        <source>Rebind &amp;&amp; &amp;Resume</source>
        <translation>重新绑定并恢复(&amp;R)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="114"/>
        <source>Rescan USB/CAN hardware, rebind configured interfaces by Hardware Key, and resume measurement</source>
        <translation>重新扫描 USB/CAN 硬件，按 Hardware Key 重新绑定已配置接口并恢复测量</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="115"/>
        <source>&amp;Copy Report</source>
        <translation>复制报告(&amp;C)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="116"/>
        <source>Copy the device list as plain text, e.g. for a bug report</source>
        <translation>以纯文本复制设备列表（便于反馈问题）</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="127"/>
        <source>Rebind &amp; Resume</source>
        <translation>重新绑定并恢复</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="128"/>
        <source>Failed to rebind interfaces.</source>
        <translation>重新绑定接口失败。</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="171"/>
        <source>no devices found</source>
        <translation>未发现设备</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="223"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="286"/>
        <source>Open</source>
        <translation>已打开</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="223"/>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="286"/>
        <source>Available</source>
        <translation>可用</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="267"/>
        <source>Select an interface to see its details.</source>
        <translation>请选择一个接口查看详细信息。</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="282"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="283"/>
        <source>Description</source>
        <translation>描述</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="288"/>
        <source>Bus state</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="289"/>
        <source>Interface ID</source>
        <translation>接口 ID</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="290"/>
        <source>Hardware Key</source>
        <translation>Hardware Key</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="301"/>
        <source>, %1 / %2 kBit/s (FD)</source>
        <translation>, %1 / %2 kBit/s (FD)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="303"/>
        <source>, %1 kBit/s</source>
        <translation>, %1 kBit/s</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="305"/>
        <source>, listen only</source>
        <translation>，仅监听</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="307"/>
        <source> (disabled)</source>
        <translation>（已禁用）</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="308"/>
        <source>Setup</source>
        <translation>配置</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="327"/>
        <source>Supported bitrates (%1)</source>
        <translation>支持的波特率 (%1)</translation>
    </message>
    <message>
        <location filename="../window/HardwareDialog/HardwareDialog.cpp" line="379"/>
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
        <location filename="../core/LogModel.cpp" line="190"/>
        <source>debug</source>
        <translation>调试</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="191"/>
        <source>info</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="192"/>
        <source>warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="193"/>
        <source>error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="194"/>
        <source>critical</source>
        <translation>严重</translation>
    </message>
    <message>
        <location filename="../core/LogModel.cpp" line="195"/>
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
        <location filename="../window/LogWindow/LogWindow.cpp" line="127"/>
        <source>Export Logs</source>
        <translation>导出日志</translation>
    </message>
    <message>
        <location filename="../window/LogWindow/LogWindow.cpp" line="127"/>
        <source>Log Files (*.log);;Text Files (*.txt);;All Files (*)</source>
        <translation>日志文件 (*.log);;文本文件 (*.txt);;所有文件 (*)</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="17"/>
        <location filename="../mainwindow.cpp" line="1808"/>
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
        <location filename="../mainwindow.ui" line="182"/>
        <source>&amp;Measurement</source>
        <translation>&amp;测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="193"/>
        <source>&amp;Help</source>
        <translation>&amp;帮助</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="201"/>
        <source>&amp;Trace</source>
        <translation>&amp;Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="215"/>
        <source>E&amp;xit</source>
        <oldsource>&amp;Exit</oldsource>
        <translation>&amp;退出</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="218"/>
        <source>Alt+F4</source>
        <translation>Alt+F4</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="223"/>
        <source>&amp;Trace View</source>
        <translation>&amp;Trace 视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="226"/>
        <source>Ctrl+Shift+T</source>
        <translation>Ctrl+Shift+T</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="231"/>
        <source>&amp;Log View</source>
        <translation>&amp;日志视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="234"/>
        <source>Ctrl+Shift+L</source>
        <translation>Ctrl+Shift+L</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="239"/>
        <source>&amp;Setup...</source>
        <translation>&amp;配置...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="242"/>
        <source>Ctrl+Alt+S</source>
        <translation>Ctrl+Alt+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="247"/>
        <source>Star&amp;t Measurement</source>
        <translation>&amp;启动测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="250"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="255"/>
        <source>Sto&amp;p Measurement</source>
        <translation>&amp;停止测量</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="258"/>
        <source>Shift+F5</source>
        <translation>Shift+F5</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="263"/>
        <source>&amp;Reload Interfaces</source>
        <translation>重新加载接口(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="281"/>
        <source>&amp;About</source>
        <translation>&amp;关于</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="286"/>
        <source>&amp;Save Trace to file...</source>
        <translation>&amp;保存 Trace 到文件...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="294"/>
        <source>&amp;Graph View</source>
        <translation>&amp;Graph 视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="297"/>
        <source>Ctrl+Shift+G</source>
        <translation>Ctrl+Shift+G</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="305"/>
        <source>&amp;New Workspace...</source>
        <translation>&amp;新建工作空间...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="308"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="313"/>
        <source>&amp;Save Workspace</source>
        <translation>&amp;保存工作空间</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="316"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="321"/>
        <source>Save Workspace &amp;As...</source>
        <translation>&amp;另存工作空间为...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="324"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="329"/>
        <source>&amp;Open Workspace...</source>
        <translation>&amp;打开工作空间...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="332"/>
        <source>Ctrl+O</source>
        <translation>Ctrl+O</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="337"/>
        <source>&amp;Clear</source>
        <translation>&amp;清除</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="340"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="345"/>
        <source>Can &amp;Status View</source>
        <translation>&amp;CAN状态视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="350"/>
        <source>Graph View</source>
        <translation>Graph 视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="355"/>
        <location filename="../mainwindow.cpp" line="1328"/>
        <source>Message View</source>
        <translation>报文视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="365"/>
        <source>Replay View</source>
        <translation>回放视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="373"/>
        <source>Restore Window on startup</source>
        <translation>启动时恢复窗口</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="376"/>
        <source>Restore Window Position on startup</source>
        <translation>启动时恢复窗口位置</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="381"/>
        <source>&amp;Settings...</source>
        <translation>设置(&amp;S)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="360"/>
        <location filename="../mainwindow.cpp" line="1358"/>
        <source>Generator View</source>
        <translation>生成器视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1110"/>
        <source>Open workspace configuration</source>
        <translation>打开工作空间配置文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1111"/>
        <location filename="../mainwindow.cpp" line="1128"/>
        <source>Workspace config files (*.cangaroo)</source>
        <translation>工作空间配置文件 (*.cangaroo)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1127"/>
        <source>Save workspace configuration</source>
        <translation>保存工作空间配置文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1160"/>
        <source>The current workspace has been modified.</source>
        <translation>当前工作空间已被修改。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1161"/>
        <source>Do you want to save your changes?</source>
        <translation>是否要保存更改？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1874"/>
        <location filename="../mainwindow.cpp" line="1915"/>
        <source>English</source>
        <translation>英文</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="155"/>
        <location filename="../mainwindow.cpp" line="1824"/>
        <source>Export full trace</source>
        <translation>导出完整 Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="116"/>
        <location filename="../mainwindow.cpp" line="1256"/>
        <location filename="../mainwindow.cpp" line="1818"/>
        <location filename="../mainwindow.cpp" line="1881"/>
        <source>Standalone Graph</source>
        <translation>独立 Graph 窗口</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="122"/>
        <location filename="../mainwindow.cpp" line="1370"/>
        <location filename="../mainwindow.cpp" line="1820"/>
        <source>Motor View</source>
        <translation>电机视图</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="166"/>
        <location filename="../mainwindow.cpp" line="1830"/>
        <source>Open Recent</source>
        <translation>最近打开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="174"/>
        <location filename="../mainwindow.cpp" line="1837"/>
        <source>Open Standalone Graph Window (Ctrl+Shift+B)</source>
        <translation>打开独立 Graph 窗口 (Ctrl+Shift+B)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="393"/>
        <source>Clear Recent Files</source>
        <translation>清除最近文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="494"/>
        <location filename="../mainwindow.cpp" line="1842"/>
        <source>&amp;Record Trace to File</source>
        <translation>录制 Trace 到文件(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="497"/>
        <location filename="../mainwindow.cpp" line="1843"/>
        <source>Record</source>
        <translation>录制</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="498"/>
        <location filename="../mainwindow.cpp" line="1844"/>
        <source>Continuously record all traffic to a file (Ctrl+R)</source>
        <translation>将所有通信持续录制到文件 (Ctrl+R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="501"/>
        <location filename="../mainwindow.cpp" line="1847"/>
        <source>Recording &amp;Options...</source>
        <translation>录制选项(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="502"/>
        <location filename="../mainwindow.cpp" line="1849"/>
        <source>Open Recording &amp;Folder</source>
        <translation>打开录制文件夹(&amp;F)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="587"/>
        <source>● REC  %1  ·  %2  ·  %3</source>
        <translation>● REC  %1  ·  %2  ·  %3</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="591"/>
        <source>%1
%2 frames written</source>
        <translation>%1
已写入 %2 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="601"/>
        <source>Recording armed, starts with measurement</source>
        <translation>录制已就绪，将随测量开始</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="624"/>
        <location filename="../mainwindow.cpp" line="1853"/>
        <source>&amp;Trigger Capture (Blackbox)...</source>
        <translation>触发捕获（黑匣子）(&amp;T)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="625"/>
        <location filename="../mainwindow.cpp" line="1854"/>
        <source>Configure pre/post-trigger blackbox slice capture</source>
        <translation>配置触发前/触发后黑匣子切片捕获</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="629"/>
        <location filename="../mainwindow.cpp" line="1858"/>
        <source>Trigger</source>
        <translation>触发</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="630"/>
        <location filename="../mainwindow.cpp" line="1859"/>
        <source>Configure pre/post-trigger blackbox capture</source>
        <translation>配置触发前/触发后黑匣子捕获</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="645"/>
        <source>Trigger fired: %1 (capturing post-trigger window...)</source>
        <translation>触发已发生：%1（正在捕获触发后窗口...）</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="649"/>
        <source>Trigger capture saved (%1 frames): %2 [%3]</source>
        <translation>触发捕获已保存（%1 帧）：%2 [%3]</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="656"/>
        <source>Trigger capture failed: %1</source>
        <translation>触发捕获失败：%1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="685"/>
        <source>TRIG CAPTURING...</source>
        <translation>触发捕获中...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="689"/>
        <source>TRIG ARMED (%1)</source>
        <translation>触发已就绪（%1）</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="709"/>
        <location filename="../mainwindow.cpp" line="1863"/>
        <source>Rebind &amp;&amp; Resume</source>
        <translation>重新绑定并恢复</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="711"/>
        <location filename="../mainwindow.cpp" line="1865"/>
        <source>Dismiss</source>
        <translation>忽略</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="748"/>
        <location filename="../mainwindow.cpp" line="767"/>
        <source>USB/CAN device disconnected</source>
        <translation>USB/CAN 设备已断开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="770"/>
        <source>Interface &apos;%1&apos; disconnected (%2). Reconnect the USB adapter, then click &quot;Rebind &amp; Resume&quot; to restore measurement without losing trace/graph data.</source>
        <translation>接口 &apos;%1&apos; 已断开（%2）。请重新连接 USB 适配器，然后点击“重新绑定并恢复”，即可在不丢失 Trace/Graph 数据的情况下恢复测量。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="746"/>
        <source>Interface &apos;%1&apos; disconnected — please manually rebind.</source>
        <translation>接口 &apos;%1&apos; 已断开——请手动重新绑定。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="750"/>
        <source>USB / CAN Interface Disconnected</source>
        <translation>USB / CAN 接口已断开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="751"/>
        <source>Interface &apos;%1&apos; lost connection (%2).

Existing Trace and Graph data have been preserved.
Please reconnect the USB cable and click &quot;Rebind &amp; Resume&quot; in the top banner or Hardware dialog.</source>
        <translation>接口 &apos;%1&apos; 连接丢失（%2）。

已有的 Trace 与 Graph 数据已保留。
请重新插好 USB 线缆，然后在顶部提示条或“硬件”对话框中点击“重新绑定并恢复”。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="779"/>
        <source>Interfaces rebound and measurement resumed (trace preserved).</source>
        <translation>接口已重新绑定，测量已恢复（Trace 数据已保留）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="784"/>
        <source>Rebind &amp; Resume Failed</source>
        <translation>重新绑定并恢复失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="785"/>
        <source>Could not rebind configured interface(s):
%1

Please verify the USB adapter is plugged in and try again, or open Measurement Setup.</source>
        <translation>无法重新绑定已配置的接口：
%1

请确认 USB 适配器已插好后重试，或打开测量配置。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="787"/>
        <source>No matching hardware found.</source>
        <translation>未找到匹配的硬件。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="901"/>
        <source>Ignoring scriptwindow section from workspace; Python scripting is no longer available</source>
        <translation>工作区中的 scriptwindow 段已忽略：Python 脚本功能已移除</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="945"/>
        <source>Cannot open workspace settings file: %1</source>
        <translation>无法打开工作空间配置文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="953"/>
        <source>Cannot load settings from file: %1</source>
        <translation>无法从文件加载设置: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="975"/>
        <source>Could not read window %1 from file: %2</source>
        <translation>无法从文件 %2 读取窗口 %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="987"/>
        <source>Unable to read measurement setup from workspace config file: %1</source>
        <translation>无法从工作空间配置文件读取测量设置: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1460"/>
        <source>CANgaroo
Open Source CAN bus analyzer
https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl
(Upstream: https://github.com/Schildkroet/CANgaroo)

Version %1

(c)2024-2026 Schildkroet
(c)2015-2017 Hubert Denkmair
(c)2018-2022 Ethan Zonca
(c)2024 WeAct Studio
(c)2025 Wikilift
(c)2026 Jayachandran Dharuman

CANgaroo is free software licensed
under the GPL v2 license.</source>
        <translation>CANgaroo
开源 CAN 总线分析仪
https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl
(上游项目: https://github.com/Schildkroet/CANgaroo)

版本 %1

(c)2024-2026 Schildkroet
(c)2015-2017 Hubert Denkmair
(c)2018-2022 Ethan Zonca
(c)2024 WeAct Studio
(c)2025 Wikilift
(c)2026 Jayachandran Dharuman

CANgaroo 是基于 GPL v2 许可证的自由软件。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1960"/>
        <source>All Supported (*.asc *.candump *.csv);;Vector ASC (*.asc);;CAN/FD Raw CSV (*.csv);;Linux candump (*.candump);;All Files (*)</source>
        <translation>所有支持的格式 (*.asc *.candump *.csv);;Vector ASC (*.asc);;CAN/FD 原始帧 CSV (*.csv);;Linux candump (*.candump);;所有文件 (*)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2015"/>
        <source>CAN/FD Raw CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1029"/>
        <source>Cannot save window settings to file: %1</source>
        <translation>无法将窗口设置保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="128"/>
        <location filename="../mainwindow.cpp" line="1822"/>
        <source>Close Tab</source>
        <translation>关闭标签页</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1069"/>
        <source>Cannot save measurement setup to file: %1</source>
        <translation>无法将测量设置保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1088"/>
        <source>Saved workspace settings to file: %1</source>
        <translation>工作空间设置已保存到文件: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1169"/>
        <source>Do not ask again (always discard)</source>
        <translation>不再询问 (始终丢弃)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1352"/>
        <source>BUS Status</source>
        <translation>总线状态</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1707"/>
        <source>Vector ASC (*.asc)</source>
        <translation>Vector ASC 文件 (*.asc)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1708"/>
        <source>CAN/FD Raw CSV (*.csv)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1709"/>
        <source>Linux candump (*.candump)</source>
        <translation>Linux candump 文件 (*.candump)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1787"/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1788"/>
        <source>Could not load the translation for %1. The UI language was not changed.</source>
        <translation>无法加载 %1 的翻译，界面语言未更改。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1940"/>
        <source>Export Full Trace</source>
        <translation>导出完整 Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1940"/>
        <source>Current trace is empty.</source>
        <translation>当前 Trace 为空。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1946"/>
        <source>Trace Was Pruned</source>
        <translation>Trace 已被裁剪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1947"/>
        <source>The in-memory trace exceeded %1 frames and older frames were pruned.
For lossless long recordings, enable Continuous Recording (Ctrl+R) before measurement.</source>
        <translation>内存中的 Trace 超过 %1 帧，较早的帧已被裁剪。
如需无损的长时间记录，请在测量前启用连续录制 (Ctrl+R)。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1958"/>
        <source>Import Trace File</source>
        <translation>导入 Trace 文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1975"/>
        <source>Import Failed</source>
        <translation>导入失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1977"/>
        <source>Failed to parse trace file or file is empty.</source>
        <translation>解析 Trace 文件失败，或文件为空。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1987"/>
        <source>Large Trace File</source>
        <translation>大型 Trace 文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1988"/>
        <source>This file contains %1 frames, exceeding the in-memory Trace limit (%2).</source>
        <translation>该文件包含 %1 帧，超过内存 Trace 上限（%2）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1991"/>
        <source>Choose how to handle this large trace:</source>
        <translation>请选择如何处理该大型 Trace：</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1992"/>
        <source>Split &amp; Save Files...</source>
        <translation>分段保存文件...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1993"/>
        <source>Open in Replay Window</source>
        <translation>在回放窗口中打开</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1994"/>
        <source>Load First %1 Frames</source>
        <translation>仅加载前 %1 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2002"/>
        <source>Split Trace by Frame Count</source>
        <translation>按帧数分割 Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2003"/>
        <source>Maximum frames per output file:</source>
        <translation>每个输出文件的最大帧数：</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2013"/>
        <source>Save Split Trace Files</source>
        <translation>保存分段 Trace 文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2031"/>
        <source>Split Save Failed</source>
        <translation>分段保存失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2031"/>
        <source>No files written.</source>
        <translation>未写入任何文件。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2034"/>
        <source>Split Save Complete</source>
        <translation>分段保存完成</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2035"/>
        <source>Saved %1 split file(s) starting at:
%2</source>
        <translation>已保存 %1 个分段文件，起始文件：
%2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="2064"/>
        <source>Imported %1 frames from %2</source>
        <translation>已从 %2 导入 %1 帧</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1077"/>
        <source>Cannot open workspace file for writing: %1</source>
        <translation>无法打开工作空间文件进行写入: %1</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="994"/>
        <source>Ignoring gatewaywindow section from workspace; the CAN gateway is no longer available</source>
        <translation>工作区中的 gatewaywindow 段已忽略：CAN 网关功能已移除</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1720"/>
        <source>Save Trace to file</source>
        <translation>将 Trace 保存到文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1740"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1740"/>
        <source>Cannot open file for writing.</source>
        <translation>无法打开文件进行写入。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1873"/>
        <location filename="../mainwindow.cpp" line="1916"/>
        <source>Chinese</source>
        <translation>中文</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="160"/>
        <location filename="../mainwindow.cpp" line="1826"/>
        <source>Import full trace</source>
        <translation>导入完整 Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1505"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="874"/>
        <location filename="../mainwindow.cpp" line="1192"/>
        <source>Trace</source>
        <translation>Trace</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="173"/>
        <location filename="../mainwindow.cpp" line="876"/>
        <location filename="../mainwindow.cpp" line="1246"/>
        <location filename="../mainwindow.cpp" line="1312"/>
        <location filename="../mainwindow.cpp" line="1836"/>
        <source>Graph</source>
        <translation>Graph</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1346"/>
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1364"/>
        <source>Replay</source>
        <translation>回放</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1477"/>
        <source>About CANgaroo</source>
        <translation>关于CANgaroo</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="276"/>
        <source>Check for &amp;Updates...</source>
        <translation>检查更新(&amp;U)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1504"/>
        <source>Checking for updates from %1...</source>
        <translation>正在从 %1 检查更新...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1507"/>
        <location filename="../mainwindow.cpp" line="1546"/>
        <location filename="../mainwindow.cpp" line="1555"/>
        <location filename="../mainwindow.cpp" line="1575"/>
        <source>Check for Updates</source>
        <translation>检查更新</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1547"/>
        <source>You are already using the latest version (%1).

Update source: %2</source>
        <translation>当前已是最新版本 (%1)。

更新源: %2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1556"/>
        <source>You are running a development build (%1).

Latest version in %2: %3</source>
        <translation>当前正在运行开发构建版本 (%1)。

%2 中的最新版本: %3</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1561"/>
        <location filename="../mainwindow.cpp" line="1656"/>
        <source>Open Release Page</source>
        <translation>打开发布页面</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1576"/>
        <source>Failed to check for updates from %1.</source>
        <translation>从 %1 检查更新失败。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1578"/>
        <source>Open Releases Page</source>
        <translation>打开发布列表</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1591"/>
        <source>Update Available</source>
        <translation>发现新版本</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1596"/>
        <source>A new version of CANgaroo is available!</source>
        <translation>发现新版本 CANgaroo！</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1604"/>
        <source>Current version:</source>
        <translation>当前版本:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1608"/>
        <source> (tag %1)</source>
        <translation> (标签 %1)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1609"/>
        <source>Latest version:</source>
        <translation>最新版本:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1618"/>
        <source>Update source:</source>
        <translation>更新源:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1621"/>
        <source>Package:</source>
        <translation>安装包:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1627"/>
        <source>Release notes:</source>
        <translation>更新说明:</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1639"/>
        <source>Detected from repository tag %1 (no published release notes yet).</source>
        <translation>检测到仓库标签 %1（暂未发布发行说明）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1651"/>
        <source>Download</source>
        <translation>下载</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="1652"/>
        <source>Release Page</source>
        <translation>发布页面</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="268"/>
        <source>&amp;Hardware...</source>
        <translation>硬件设备(&amp;H)...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="271"/>
        <source>Show all available devices with status, version and capabilities</source>
        <translation>显示所有可用设备的状态、版本与支持能力</translation>
    </message>
</context>
<context>
    <name>MotorConfigDialog</name>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="74"/>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="179"/>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="186"/>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="205"/>
        <source>Motor Configuration</source>
        <translation>电机配置</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="77"/>
        <source>e.g. Front Left</source>
        <translation>例如：左前</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="80"/>
        <source>DJI M3508</source>
        <translation>DJI M3508</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="81"/>
        <source>DJI M2006</source>
        <translation>DJI M2006</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="82"/>
        <source>DJI GM6020</source>
        <translation>DJI GM6020</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="83"/>
        <source>Damiao DM (MIT)</source>
        <translation>达妙 DM（MIT）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="103"/>
        <source>Damiao MIT limits</source>
        <translation>达妙 MIT 量程</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="126"/>
        <source>Pmax (rad):</source>
        <translation>Pmax（rad）：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="127"/>
        <source>Vmax (rad/s):</source>
        <translation>Vmax（rad/s）：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="128"/>
        <source>Tmax (N·m):</source>
        <translation>Tmax（N·m）：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="130"/>
        <source>Invert direction</source>
        <translation>方向取反</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="132"/>
        <source>Enabled</source>
        <translation>启用</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="136"/>
        <source>Name:</source>
        <translation>名称：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="137"/>
        <source>Model:</source>
        <translation>型号：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="138"/>
        <source>Feedback CAN ID:</source>
        <translation>反馈 CAN ID：</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="179"/>
        <source>Please enter a motor name.</source>
        <translation>请输入电机名称。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="187"/>
        <source>Please enter a valid 11 bit CAN ID, e.g. 0x201.</source>
        <translation>请输入有效的 11 位 CAN ID，例如 0x201。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="206"/>
        <source>CAN ID %1 is outside the usual feedback range %2-%3 for the selected model. Use it anyway?</source>
        <translation>CAN ID %1 超出所选型号的常见反馈范围 %2-%3，仍要使用吗？</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="230"/>
        <source>Enter the motor CAN ID (Master ID / feedback frame ID), 0x000-0x7FF.</source>
        <translation>输入电机 CAN ID（Master ID / 反馈帧 ID），0x000–0x7FF。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="232"/>
        <source>DJI feedback frame ID, usually 0x205-0x20B (0x200 + ESC ID).</source>
        <translation>DJI 反馈帧 ID，通常为 0x205–0x20B（0x200 + 电调 ID）。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorConfigDialog.cpp" line="234"/>
        <source>DJI feedback frame ID, usually 0x201-0x208 (0x200 + ESC ID).</source>
        <translation>DJI 反馈帧 ID，通常为 0x201–0x208（0x200 + 电调 ID）。</translation>
    </message>
</context>
<context>
    <name>MotorView</name>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="186"/>
        <source>MotorView: unknown motor model &quot;%1&quot; in workspace, using %2.</source>
        <translation>MotorView：工作区中的电机型号“%1”未知，改用 %2。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="195"/>
        <source>MotorView: invalid feedback CAN ID &quot;%1&quot; for motor &quot;%2&quot;, keeping %3.</source>
        <translation>MotorView：电机“%2”的反馈 CAN ID“%1”无效，保留 %3。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="237"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="238"/>
        <source>Model</source>
        <translation>型号</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="239"/>
        <source>CAN ID</source>
        <translation>CAN ID</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="240"/>
        <source>Angle (°)</source>
        <translation>角度（°）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="241"/>
        <source>Speed (rpm)</source>
        <translation>转速（rpm）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="242"/>
        <source>Current (A)</source>
        <translation>电流（A）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="243"/>
        <source>Torque (N·m)</source>
        <translation>转矩（N·m）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="244"/>
        <source>Temp (°C)</source>
        <translation>温度（°C）</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="245"/>
        <source>Status</source>
        <translation>状态</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="248"/>
        <source>Add Motor...</source>
        <translation>添加电机...</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="249"/>
        <source>Add a motor to decode its feedback frames</source>
        <translation>添加电机以解码其反馈帧</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="250"/>
        <source>Edit...</source>
        <translation>编辑...</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="251"/>
        <source>Edit the selected motor</source>
        <translation>编辑选中的电机</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="252"/>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="253"/>
        <source>Remove the selected motor</source>
        <translation>移除选中的电机</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="254"/>
        <source>Clear Values</source>
        <translation>清空数值</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="255"/>
        <source>Reset the decoded values without removing the motors</source>
        <translation>只清空解码数值，不移除电机</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="256"/>
        <source>Export DBC...</source>
        <translation>导出 DBC...</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="257"/>
        <source>Save the auto-generated database for these motors as a .dbc file</source>
        <translation>将当前电机的自动生成数据库保存为 .dbc 文件</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="344"/>
        <source>Remove Motor</source>
        <translation>移除电机</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="345"/>
        <source>Remove motor &quot;%1&quot;?</source>
        <translation>确定移除电机“%1”？</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="375"/>
        <location filename="../window/MotorView/MotorView.cpp" line="379"/>
        <location filename="../window/MotorView/MotorView.cpp" line="385"/>
        <location filename="../window/MotorView/MotorView.cpp" line="392"/>
        <location filename="../window/MotorView/MotorView.cpp" line="401"/>
        <source>Export DBC</source>
        <translation>导出 DBC</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="380"/>
        <source>Configure at least one motor first.</source>
        <translation>请先配置至少一个电机。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="385"/>
        <source>DBC Files (*.dbc)</source>
        <translation>DBC 文件 (*.dbc)</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="393"/>
        <source>Cannot open %1 for writing.</source>
        <translation>无法打开 %1 写入。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="401"/>
        <source>Cannot write %1.</source>
        <translation>无法写入 %1。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="487"/>
        <source>raw torque: %1</source>
        <translation>原始转矩：%1</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="503"/>
        <source>MOS: %1 °C / Rotor: %2 °C</source>
        <translation>MOS：%1 °C / 转子：%2 °C</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="513"/>
        <source>Online</source>
        <translation>在线</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="513"/>
        <source>Offline</source>
        <translation>离线</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="599"/>
        <source>Motor Configuration</source>
        <translation>电机配置</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="600"/>
        <source>CAN ID %1 is already used by motor &quot;%2&quot;. Choose a different ID.</source>
        <translation>CAN ID %1 已被电机“%2”占用，请选择其他 ID。</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="648"/>
        <source>DJI M3508</source>
        <translation>DJI M3508</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="650"/>
        <source>DJI M2006</source>
        <translation>DJI M2006</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="652"/>
        <source>DJI GM6020</source>
        <translation>DJI GM6020</translation>
    </message>
    <message>
        <location filename="../window/MotorView/MotorView.cpp" line="654"/>
        <source>Damiao DM (MIT)</source>
        <translation>达妙 DM（MIT）</translation>
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
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="108"/>
        <source>Setup</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="110"/>
        <source>Interfaces</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeItem.cpp" line="112"/>
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
        <location filename="../main.cpp" line="83"/>
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
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="115"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="312"/>
        <source>Send</source>
        <translation>发送</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="88"/>
        <source>Send Repeat</source>
        <translation>周期发送</translation>
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
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="90"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="307"/>
        <source>RTR</source>
        <translation>RTR</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="204"/>
        <source>Extended ID</source>
        <translation>扩展帧</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="75"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="303"/>
        <source>ID (Hex):</source>
        <translation>ID (Hex):</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="83"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="304"/>
        <source>DLC:</source>
        <translation>DLC:</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="89"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="306"/>
        <source>Extended</source>
        <translation>扩展帧</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="101"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="305"/>
        <source>Interface:</source>
        <translation>接口：</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="117"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="313"/>
        <source>Repeat</source>
        <translation>周期发送</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="119"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="314"/>
        <source>Interval</source>
        <translation>间隔</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="123"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="315"/>
        <source> ms</source>
        <translation> 毫秒</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="153"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="526"/>
        <source>Repeat stopped because measurement ended.</source>
        <translation>测量已结束，周期发送已停止。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="203"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="310"/>
        <source>Signal</source>
        <translation>信号</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="437"/>
        <source>Payload bytes must be hexadecimal values from 00 to FF.</source>
        <translation>数据字节必须是 00 至 FF 之间的十六进制值。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="487"/>
        <source>The selected interface changed during Repeat.</source>
        <translation>周期发送期间所选接口发生变化。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="501"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="582"/>
        <source>The selected interface is not open.</source>
        <translation>所选接口未打开。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="516"/>
        <source>Ready to send</source>
        <translation>可以发送</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="511"/>
        <source>Interface is closed. Start measurement and select an open interface.</source>
        <translation>接口未打开。请启动测量并选择已打开的接口。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="519"/>
        <source>TX requested: %1 (every %2 ms)</source>
        <translation>已请求发送 %1 帧（每 %2 毫秒）</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="520"/>
        <source>TX requested: %1</source>
        <translation>已请求发送 %1 帧</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="523"/>
        <source>Repeat stopped. TX requested: %1</source>
        <translation>周期发送已停止。已请求发送 %1 帧。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="529"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="547"/>
        <source>Send failed: %1</source>
        <translation>发送失败：%1</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="591"/>
        <source>CAN FD is not enabled on the selected interface.</source>
        <translation>所选接口未启用 CAN FD。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="32"/>
        <source>Message View</source>
        <translation>报文视图</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="45"/>
        <source>Signal Name</source>
        <translation>信号名称</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="50"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="203"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="310"/>
        <source>Value</source>
        <translation>值</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="55"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="203"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="310"/>
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
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="92"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="309"/>
        <source>BRS</source>
        <translation>BRS</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxWindow.ui" line="230"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="91"/>
        <location filename="../window/RawTxWindow/RawTxWindow.cpp" line="308"/>
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
    <message>
        <location filename="../window/RawTxWindow/RawTxMessageBuilder.cpp" line="39"/>
        <source>Invalid payload length for the selected frame format.</source>
        <translation>载荷长度不符合所选帧格式。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxMessageBuilder.cpp" line="42"/>
        <source>CAN FD does not support remote frames.</source>
        <translation>CAN FD 不支持远程帧。</translation>
    </message>
    <message>
        <location filename="../window/RawTxWindow/RawTxMessageBuilder.cpp" line="45"/>
        <source>BRS requires CAN FD.</source>
        <translation>BRS 仅适用于 CAN FD。</translation>
    </message>
</context>
<context>
    <name>RecordingDialog</name>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="41"/>
        <source>Record Trace</source>
        <translation>录制 Trace</translation>
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
        <translation>Vector ASC 文件 (*.asc)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="63"/>
        <source>CAN/FD Raw CSV (*.csv)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="64"/>
        <source>Linux candump (*.candump)</source>
        <translation>Linux candump 文件 (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="66"/>
        <source>Format:</source>
        <translation>格式：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="71"/>
        <source>Example:</source>
        <translation>示例：</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="76"/>
        <source>Split Files</source>
        <translation>分割文件</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="79"/>
        <source>Start a new file every</source>
        <translation>每隔以下大小开始新文件</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="83"/>
        <source> MB</source>
        <translation> MB</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="92"/>
        <source>Start</source>
        <translation>开始</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="95"/>
        <source>Record every measurement (stay armed after stop)</source>
        <translation>录制每次测量 (停止后保持就绪)</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="99"/>
        <source>Also record during offline trace replay</source>
        <translation>离线 Trace 回放期间同时录制</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="103"/>
        <source>Recording starts with the next measurement, or immediately if one is running. Changed options apply from the next recording.</source>
        <translation>录制将随下一次测量开始；如果测量正在运行，则立即开始。更改的选项将从下一次录制起生效。</translation>
    </message>
    <message>
        <location filename="../window/RecordingDialog/RecordingDialog.cpp" line="140"/>
        <source>Recording Folder</source>
        <translation>录制文件夹</translation>
    </message>
</context>
<context>
    <name>ReplayWindow</name>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="70"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="196"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="272"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="575"/>
        <source>Play</source>
        <translation>播放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="71"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="197"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="83"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="206"/>
        <source>Playback speed multiplier</source>
        <translation>回放速度倍数</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="85"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="207"/>
        <source>Autoplay</source>
        <translation>自动播放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="86"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="208"/>
        <source>Automatically start/stop replay with measurement</source>
        <translation>随测量自动启动/停止回放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="88"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="209"/>
        <source>Loop</source>
        <translation>循环</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="89"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="210"/>
        <source>Restart replay after all messages were sent</source>
        <translation>所有报文发送完毕后重新开始回放</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="77"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="205"/>
        <source>Speed:</source>
        <translation>速度:</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="69"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="195"/>
        <source>Load...</source>
        <translation>加载...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="72"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="198"/>
        <source>Step +1</source>
        <translation>单步 +1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="107"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="211"/>
        <source>No trace file loaded</source>
        <translation>未加载 Trace 文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="108"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="199"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="631"/>
        <source>Load to Trace</source>
        <translation>载入 Trace</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="109"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="200"/>
        <source>Import enabled messages directly into Trace and Graph</source>
        <translation>将已启用的报文直接导入 Trace 与 Graph</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="111"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="201"/>
        <source>Split Save...</source>
        <translation>分段保存...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="112"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="202"/>
        <source>Split large trace into smaller files by frame count</source>
        <translation>按帧数将大型 Trace 分割为多个较小文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="143"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="212"/>
        <source>Trace file info...</source>
        <translation>Trace 文件信息...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="149"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="203"/>
        <source>Select All</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="150"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="204"/>
        <source>Deselect All</source>
        <translation>取消全选</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>Interface / CAN ID</source>
        <translation>接口 / CAN ID</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>TX</source>
        <translation>TX</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>Count</source>
        <translation>数量</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="157"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="213"/>
        <source>Output</source>
        <translation>输出</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="196"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="550"/>
        <source>Pause</source>
        <translation>暂停</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="196"/>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="537"/>
        <source>Resume</source>
        <translation>继续</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="220"/>
        <source>Load Trace File</source>
        <translation>加载 Trace 文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="222"/>
        <source>All Supported (*.asc *.candump *.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;CAN/FD Raw CSV (*.csv);;All Files (*)</source>
        <translation>所有支持的格式 (*.asc *.candump *.csv);;Vector ASC (*.asc);;Linux candump (*.candump);;CAN/FD 原始帧 CSV (*.csv);;所有文件 (*)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="688"/>
        <source>CAN/FD Raw CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv);;Vector ASC (*.asc);;Linux candump (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="242"/>
        <source>Failed to parse trace file or file is empty.</source>
        <translation>解析 Trace 文件失败，或文件为空。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="244"/>
        <source>Error: %1</source>
        <translation>错误：%1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="245"/>
        <source>ReplayWindow: failed to load %1: %2</source>
        <translation>回放窗口：加载 %1 失败：%2</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="284"/>
        <source>Warnings: %1</source>
        <translation>警告：%1</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="631"/>
        <source>No enabled messages match the current filter.</source>
        <translation>没有符合当前过滤条件的已启用报文。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="640"/>
        <source>Large Trace File</source>
        <translation>大型 Trace 文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="641"/>
        <source>Selected trace contains %1 messages, exceeding the in-memory limit (%2).</source>
        <translation>所选 Trace 包含 %1 条报文，超过内存上限（%2）。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="644"/>
        <source>You can split it into smaller files, or load the first %1 messages into Trace.</source>
        <translation>可将其分割为多个较小文件，或仅将前 %1 条报文载入 Trace。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="645"/>
        <source>Split &amp; Save...</source>
        <translation>分段保存...</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="646"/>
        <source>Load First %1</source>
        <translation>加载前 %1 条</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="660"/>
        <source>Loaded %1 replay messages into Trace.</source>
        <translation>已将 %1 条回放报文载入 Trace。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="673"/>
        <source>Split Trace by Frame Count</source>
        <translation>按帧数分割 Trace</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="674"/>
        <source>Maximum frames per output file:</source>
        <translation>每个输出文件的最大帧数：</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="686"/>
        <source>Save Split Trace Files</source>
        <translation>保存分段 Trace 文件</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="713"/>
        <source>Split Save Failed</source>
        <translation>分段保存失败</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="713"/>
        <source>No files written.</source>
        <translation>未写入任何文件。</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="718"/>
        <source>Split Save Complete</source>
        <translation>分段保存完成</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="719"/>
        <source>Saved %1 split file(s) starting at:
%2</source>
        <translation>已保存 %1 个分段文件，起始文件：
%2</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="279"/>
        <source>File: %1
Messages: %2
Duration: %3 s</source>
        <translation>文件：%1
报文数：%2
持续时间：%3 s</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="325"/>
        <source>%1 (CAN)</source>
        <translation>%1 (CAN)</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="331"/>
        <source>Trace only</source>
        <translation>仅 Trace</translation>
    </message>
    <message>
        <location filename="../window/ReplayWindow/ReplayWindow.cpp" line="349"/>
        <source>ERROR</source>
        <translation>错误</translation>
    </message>
</context>
<context>
    <name>ScatterVisualization</name>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="54"/>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="114"/>
        <source>Scatter (Distribution View)</source>
        <translation>散点图（分布视图）</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="69"/>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="118"/>
        <source>Time [s]</source>
        <translation>时间 [s]</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="75"/>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="120"/>
        <source>Value</source>
        <translation>数值</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="477"/>
        <source>Export to CSV</source>
        <translation>导出 CSV</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="478"/>
        <source>Export to Image</source>
        <translation>导出图片</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="488"/>
        <source>Export Scatter CSV</source>
        <translation>导出散点数据 CSV</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="488"/>
        <source>CSV Files (*.csv)</source>
        <translation>CSV 文件 (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="508"/>
        <source>Export Scatter Image</source>
        <translation>导出散点图</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/ScatterVisualization.cpp" line="508"/>
        <source>PNG Images (*.png);;JPEG Images (*.jpeg)</source>
        <translation>PNG 图片 (*.png);;JPEG 图片 (*.jpeg)</translation>
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
    <message>
        <location filename="../window/SetupDialog/SelectCanInterfacesDialog.cpp" line="32"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SelectCanInterfacesDialog.cpp" line="32"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SelectCanInterfacesDialog.cpp" line="32"/>
        <source>Description</source>
        <translation>描述</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="37"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="43"/>
        <source>Appearance</source>
        <translation>外观</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="47"/>
        <source>Light</source>
        <translation>浅色</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="48"/>
        <source>Dark</source>
        <translation>深色</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="49"/>
        <source>Follow system</source>
        <translation>跟随系统</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="55"/>
        <source>Theme:</source>
        <translation>主题:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="72"/>
        <source>Language:</source>
        <translation>语言:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="80"/>
        <source>Font size:</source>
        <translation>字体大小:</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="85"/>
        <source>Behavior</source>
        <translation>行为</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="88"/>
        <source>Restore window layout on startup</source>
        <translation>启动时恢复窗口布局</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="92"/>
        <source>Clear trace on measurement start</source>
        <translation>测量开始时清除 Trace</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="96"/>
        <source>Do not ask to save workspace on close (always discard)</source>
        <translation>关闭时不询问是否保存工作区 (始终丢弃)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="103"/>
        <source>Trace Window</source>
        <translation>Trace 窗口</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="107"/>
        <source>Aggregated</source>
        <translation>聚合</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="108"/>
        <source>Rolling Log</source>
        <translation>滚动日志</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="110"/>
        <source>Default view:</source>
        <translation>默认视图：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="113"/>
        <source>Absolute</source>
        <translation>绝对</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="114"/>
        <source>Absolute (UTC)</source>
        <translation>绝对 (UTC)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="115"/>
        <source>Relative</source>
        <translation>相对</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="116"/>
        <source>Delta</source>
        <translation>增量</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="128"/>
        <source>Default timestamp:</source>
        <translation>默认时间戳：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="131"/>
        <source>Hex</source>
        <translation>Hex</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="132"/>
        <source>ASCII</source>
        <translation>ASCII</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="134"/>
        <source>Data display:</source>
        <translation>数据显示：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="140"/>
        <source>Vector ASC (*.asc)</source>
        <translation>Vector ASC 文件 (*.asc)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="141"/>
        <source>CAN/FD Raw CSV (*.csv)</source>
        <translation>CAN/FD 原始帧 CSV (*.csv)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="142"/>
        <source>Linux candump (*.candump)</source>
        <translation>Linux candump 文件 (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="150"/>
        <source>Preferred save format:</source>
        <translation>首选保存格式：</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="156"/>
        <source> messages</source>
        <translation> 条报文</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="157"/>
        <source>Maximum number of messages kept in memory. When exceeded, the oldest messages are discarded and are missing from saved traces. Larger values use more memory.</source>
        <translation>内存中保留的最大报文数。超出后，最早的报文将被丢弃，且不会出现在保存的 Trace 中。数值越大占用内存越多。</translation>
    </message>
    <message>
        <location filename="../window/SettingsDialog/SettingsDialog.cpp" line="161"/>
        <source>Max trace size:</source>
        <translation>最大 Trace 大小：</translation>
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
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="326"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="405"/>
        <source>DBC Error</source>
        <translation>DBC 错误</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="327"/>
        <source>Failed to reload one or more DBC files:

%1</source>
        <translation>重新加载一个或多个 DBC 文件失败:

%1</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="365"/>
        <source>Duplicate DBC</source>
        <translation>DBC 重复</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="366"/>
        <source>The file is already loaded:
%1

Do you want to reload it?</source>
        <translation>文件已加载:
%1

是否要重新加载?</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="388"/>
        <source>Reload Failed</source>
        <translation>重新加载失败</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="389"/>
        <source>Failed to reload DBC:
%1

Reason: %2</source>
        <translation>重新加载 DBC 失败:
%1

原因: %2</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="402"/>
        <source>DBC Warning</source>
        <translation>DBC 警告</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="406"/>
        <source>Failed to load DBC file:
%1

Reason: %2</source>
        <translation>加载 DBC 文件失败:
%1

原因: %2</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="414"/>
        <source>Load CAN Databases</source>
        <translation>加载 CAN 数据库</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="414"/>
        <source>Vector DBC Files (*.dbc)</source>
        <translation>Vector DBC 文件 (*.dbc)</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="278"/>
        <source>No Network</source>
        <translation>无网络</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="55"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="57"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="120"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="124"/>
        <source>Add...</source>
        <translation>添加...</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="56"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="58"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="122"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="126"/>
        <source>Delete</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="59"/>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="128"/>
        <source>Reload</source>
        <translation>重新加载</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialog.cpp" line="279"/>
        <source>You must Add a Network First!</source>
        <translation>您必须先添加一个网络！</translation>
    </message>
</context>
<context>
    <name>SetupDialogTreeModel</name>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="99"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="100"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="101"/>
        <source>Bitrate</source>
        <translation>波特率</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="102"/>
        <source>Filename</source>
        <translation>文件名</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="103"/>
        <source>Path</source>
        <translation>路径</translation>
    </message>
    <message>
        <location filename="../window/SetupDialog/SetupDialogTreeModel.cpp" line="160"/>
        <source>Network </source>
        <translation>网络 </translation>
    </message>
</context>
<context>
    <name>SignalSelectorDialog</name>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="35"/>
        <source>Select Data</source>
        <translation>选择数据</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="43"/>
        <source>Search signals or messages...</source>
        <translation>搜索信号或报文...</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="46"/>
        <source>Show selection only</source>
        <translation>仅显示选中项</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="53"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="53"/>
        <source>Details</source>
        <translation>详情</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="53"/>
        <source>Comment</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="114"/>
        <source>CAN — %1</source>
        <translation>CAN — %1</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="128"/>
        <source>Unsigned</source>
        <translation>无符号</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/SignalSelectorDialog.cpp" line="128"/>
        <source>Signed</source>
        <translation>有符号</translation>
    </message>
</context>
<context>
    <name>TimeSeriesVisualization</name>
    <message>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="51"/>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="123"/>
        <source>Time Series Graph</source>
        <translation>时间序列图</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="66"/>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="127"/>
        <source>Time [s]</source>
        <translation>时间 [s]</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="72"/>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="129"/>
        <source>Value</source>
        <translation>数值</translation>
    </message>
    <message>
        <location filename="../window/GraphWindow/TimeSeriesVisualization.cpp" line="585"/>
        <source>Cursors: Left-click = A, Right-click / Shift+Click = B</source>
        <translation>游标：左键 = A，右键 / Shift+左键 = B</translation>
    </message>
</context>
<context>
    <name>TraceFileReader</name>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="148"/>
        <location filename="../core/TraceFileReader.cpp" line="256"/>
        <location filename="../core/TraceFileReader.cpp" line="460"/>
        <source>Loading cancelled by user.</source>
        <translation>加载已被用户取消。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="163"/>
        <source>Line %1: unrecognized candump syntax</source>
        <translation>第 %1 行：无法识别的 candump 语法</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="233"/>
        <source>No valid candump frames found in file.</source>
        <translation>文件中未找到有效的 candump 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="390"/>
        <source>No valid Vector ASC frames found in file.</source>
        <translation>文件中未找到有效的 Vector ASC 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="415"/>
        <source>CSV trace file is empty.</source>
        <translation>CSV Trace 文件为空。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="443"/>
        <source>Invalid CSV trace header: missing required CAN columns (Timestamp_s/Epoch_us, ID_Hex, DLC, Data_Hex).</source>
        <translation>CSV Trace 文件表头无效：缺少必需的 CAN 列（Timestamp_s/Epoch_us、ID_Hex、DLC、Data_Hex）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="492"/>
        <source>CSV line %1: invalid timestamp.</source>
        <translation>CSV 第 %1 行：时间戳无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="503"/>
        <source>CSV line %1: unsupported BusType &apos;%2&apos; (only CAN is supported).</source>
        <translation>CSV 第 %1 行：不支持的 BusType &apos;%2&apos;（仅支持 CAN）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="512"/>
        <source>CSV line %1: invalid hex ID &apos;%2&apos;.</source>
        <translation>CSV 第 %1 行：十六进制 ID &apos;%2&apos; 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="545"/>
        <source>CSV line %1: invalid DLC &apos;%2&apos;.</source>
        <translation>CSV 第 %1 行：DLC &apos;%2&apos; 无效。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="556"/>
        <source>CSV line %1: data payload shorter than DLC (%2 &lt; %3).</source>
        <translation>CSV 第 %1 行：数据负载短于 DLC（%2 &lt; %3）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="572"/>
        <source>No valid CAN frames found in CSV file.</source>
        <translation>CSV 文件中未找到有效的 CAN 帧。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="594"/>
        <source>Unsupported trace file format.</source>
        <translation>不支持的 Trace 文件格式。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="604"/>
        <source>Unknown trace file extension for &apos;%1&apos;.</source>
        <translation>无法识别 &apos;%1&apos; 的 Trace 文件扩展名。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="611"/>
        <source>Cannot open &apos;%1&apos;: %2</source>
        <translation>无法打开 &apos;%1&apos;：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="628"/>
        <source>Invalid split frame count (%1).</source>
        <translation>分段帧数无效（%1）。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="633"/>
        <source>No messages to split.</source>
        <translation>没有可分割的报文。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="638"/>
        <source>Cannot create output directory &apos;%1&apos;.</source>
        <translation>无法创建输出目录 &apos;%1&apos;。</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="677"/>
        <source>Cannot write &apos;%1&apos;: %2</source>
        <translation>无法写入 &apos;%1&apos;：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceFileReader.cpp" line="692"/>
        <source>Write error on &apos;%1&apos;: %2</source>
        <translation>写入 &apos;%1&apos; 时出错：%2</translation>
    </message>
</context>
<context>
    <name>TraceFilterDialog</name>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="39"/>
        <source>Trace Filter</source>
        <translation>Trace 过滤器</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="45"/>
        <source>Direction</source>
        <translation>方向</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="47"/>
        <source>Show TX</source>
        <translation>显示TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="48"/>
        <source>Show RX</source>
        <translation>显示RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="60"/>
        <source>Messages</source>
        <translation>报文</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="64"/>
        <source>Select All</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="65"/>
        <source>Deselect All</source>
        <translation>取消全选</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="79"/>
        <source>Interfaces</source>
        <translation>接口</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="90"/>
        <source>Clear Filters</source>
        <translation>清除过滤器</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="123"/>
        <source>(no databases loaded)</source>
        <translation>（未加载数据库）</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceFilterDialog.cpp" line="142"/>
        <source>(no interfaces)</source>
        <translation>（无接口）</translation>
    </message>
</context>
<context>
    <name>TraceRecorder</name>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="196"/>
        <source>Recording trace to %1</source>
        <translation>正在录制 Trace 到 %1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="215"/>
        <source>Recording stopped: %1 frames (%2) in %3 file(s)</source>
        <translation>录制已停止：%3 个文件中共 %1 帧 (%2)</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="227"/>
        <source>Recording could not keep up with the bus: %1 frames were dropped</source>
        <translation>录制跟不上总线速度：已丢弃 %1 帧</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="243"/>
        <source>Trace recording failed: %1</source>
        <translation>Trace 录制失败：%1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="273"/>
        <source>Recording continues in %1</source>
        <translation>录制继续写入 %1</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="318"/>
        <location filename="../core/TraceRecorder.cpp" line="332"/>
        <location filename="../core/TraceRecorder.cpp" line="388"/>
        <source>cannot write %1: %2</source>
        <translation>无法写入 %1：%2</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="344"/>
        <source>cannot create folder &apos;%1&apos;</source>
        <translation>无法创建文件夹 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../core/TraceRecorder.cpp" line="352"/>
        <source>cannot open %1: %2</source>
        <translation>无法打开 %1：%2</translation>
    </message>
</context>
<context>
    <name>TraceWindow</name>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.ui" line="14"/>
        <source>Trace View</source>
        <translation>Trace 视图</translation>
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
        <location filename="../window/TraceWindow/TraceWindow.ui" line="84"/>
        <source>Clear</source>
        <translation>清空</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="100"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="151"/>
        <source>Aggregated</source>
        <translation>聚合视图</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="101"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="152"/>
        <source>Rolling Log</source>
        <translation>滚动日志</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="103"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="161"/>
        <source>Absolute</source>
        <translation>绝对时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="104"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="162"/>
        <source>Absolute (UTC)</source>
        <translation>绝对时间 (UTC)</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="105"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="163"/>
        <source>Relative</source>
        <translation>相对时间</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="106"/>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="164"/>
        <source>Delta</source>
        <translation>增量</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="191"/>
        <source>Create Trigger Rule for ID 0x%1...</source>
        <translation>为 ID 0x%1 创建触发规则...</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/TraceWindow.cpp" line="192"/>
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
        <translation>Vector ASC 文件 (*.asc)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="87"/>
        <source>Linux candump (*.candump)</source>
        <translation>Linux candump 文件 (*.candump)</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="94"/>
        <source>Prefix / Format:</source>
        <translation>前缀 / 格式：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="114"/>
        <source>Pre:</source>
        <translation>触发前：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="116"/>
        <source>Post:</source>
        <translation>触发后：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="118"/>
        <source>Max Pre-Frames:</source>
        <translation>最大预缓冲帧数：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="120"/>
        <source>Ring Buffer:</source>
        <translation>环形缓冲：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="129"/>
        <source>One-shot (disarm after first capture)</source>
        <translation>单次（首次捕获后自动撤防）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="132"/>
        <source>Arm trigger capture on OK</source>
        <translation>点击确定后启用触发捕获</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="135"/>
        <source>Holdoff:</source>
        <translation>保持时间：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="140"/>
        <source>Behavior:</source>
        <translation>行为：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="144"/>
        <source>Trigger Rules (double-click a rule to edit)</source>
        <translation>触发规则（双击规则进行编辑）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="148"/>
        <source>On</source>
        <translation>启用</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="148"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="148"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="148"/>
        <source>Condition</source>
        <translation>条件</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="166"/>
        <source>Add Rule...</source>
        <translation>添加规则...</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="167"/>
        <source>Remove Rule</source>
        <translation>删除规则</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="180"/>
        <source>Trigger Capture</source>
        <translation>触发捕获</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="180"/>
        <source>Please select an output folder.</source>
        <translation>请选择输出文件夹。</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="242"/>
        <source>Select Trigger Output Folder</source>
        <translation>选择触发输出文件夹</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="251"/>
        <source>Rule %1</source>
        <translation>规则 %1</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="304"/>
        <source>HW Bus Error</source>
        <translation>硬件总线错误</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="305"/>
        <source>Frame Match</source>
        <translation>帧匹配</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="306"/>
        <source>Watchdog Timeout</source>
        <translation>看门狗超时</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="307"/>
        <source>DBC Signal</source>
        <translation>DBC 信号</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="309"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="316"/>
        <source>Any CAN/FD error frame</source>
        <translation>任意 CAN/FD 错误帧</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="316"/>
        <source>Error mask 0x%1</source>
        <translation>错误掩码 0x%1</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="329"/>
        <source>ID 0x%1..0x%2 silent &gt; %3 ms</source>
        <translation>ID 0x%1..0x%2 静默超过 %3 ms</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="346"/>
        <source>Edit Trigger Rule</source>
        <translation>编辑触发规则</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="351"/>
        <source>HW Bus Error Frame</source>
        <translation>硬件总线错误帧</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="352"/>
        <source>Software Frame / Mask Match</source>
        <translation>软件帧 / 掩码匹配</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="353"/>
        <source>Watchdog Frame-Loss Timeout</source>
        <translation>看门狗丢帧超时</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="354"/>
        <source>DBC Signal Threshold</source>
        <translation>DBC 信号阈值</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="361"/>
        <source>Disabled</source>
        <translation>已禁用</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="382"/>
        <source>Optional DBC message name</source>
        <translation>可选的 DBC 报文名称</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="384"/>
        <source>DBC signal name (required)</source>
        <translation>DBC 信号名称（必填）</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="390"/>
        <source>Rule Name:</source>
        <translation>规则名称：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="391"/>
        <source>Trigger Kind:</source>
        <translation>触发类型：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="392"/>
        <source>ID Min (hex):</source>
        <translation>最小 ID (Hex):</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="393"/>
        <source>ID Max (hex):</source>
        <translation>最大 ID (Hex):</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="394"/>
        <source>Byte Index (-1 off):</source>
        <translation>字节索引（-1 为关闭）：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="395"/>
        <source>Byte Mask (hex):</source>
        <translation>字节掩码 (Hex):</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="396"/>
        <source>Byte Value (hex):</source>
        <translation>字节值 (Hex):</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="397"/>
        <source>Compare Op:</source>
        <translation>比较运算：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="398"/>
        <source>Watchdog Timeout:</source>
        <translation>看门狗超时：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="399"/>
        <source>DBC Message:</source>
        <translation>DBC 报文：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="400"/>
        <source>DBC Signal:</source>
        <translation>DBC 信号：</translation>
    </message>
    <message>
        <location filename="../window/TriggerCaptureDialog/TriggerCaptureDialog.cpp" line="401"/>
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
        <translation>触发捕获失败：无法创建输出文件夹 &apos;%1&apos;。</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="430"/>
        <source>Trigger capture failed: cannot open &apos;%1&apos;: %2</source>
        <translation>触发捕获失败：无法打开 &apos;%1&apos;：%2</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="444"/>
        <source>Trigger capture write error on &apos;%1&apos;: %2</source>
        <translation>触发捕获写入 &apos;%1&apos; 时出错：%2</translation>
    </message>
    <message>
        <location filename="../core/TriggerCaptureManager.cpp" line="456"/>
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
        <translation>可用报文 (DBC)</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="74"/>
        <source>Message List</source>
        <translation>报文列表</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="80"/>
        <source>Message Name or ID...</source>
        <translation>报文名称或 ID...</translation>
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
        <translation>添加 DBC 报文</translation>
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
        <translation>运行</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.ui" line="320"/>
        <source>STOP</source>
        <translation>停止</translation>
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
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="100"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="143"/>
        <source> ms</source>
        <translation> 毫秒</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="609"/>
        <source>Edit Message: %1 - %2</source>
        <translation>编辑报文: %1 - %2</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="800"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="844"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="800"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="844"/>
        <source>Start</source>
        <translation>启动</translation>
    </message>
    <message>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="815"/>
        <location filename="../window/TxGeneratorWindow/TxGeneratorWindow.cpp" line="855"/>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
</context>
<context>
    <name>UnifiedTraceViewModel</name>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="288"/>
        <source>RX</source>
        <translation>RX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="288"/>
        <source>TX</source>
        <translation>TX</translation>
    </message>
    <message>
        <location filename="../window/TraceWindow/UnifiedTraceViewModel.cpp" line="309"/>
        <source>[raw]</source>
        <translation>[原始]</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="435"/>
        <source>Invalid release metadata received from GitHub (%1).</source>
        <translation>从 GitHub 收到的发行版元数据无效 (%1)。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="444"/>
        <source>Latest release entry in %1 is still marked as draft.</source>
        <translation>%1 中的最新发布记录仍处于草稿状态。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="458"/>
        <location filename="../core/UpdateChecker.cpp" line="635"/>
        <source>Could not parse release version &quot;%1&quot; from %2.</source>
        <translation>无法解析来自 %2 的版本号 &quot;%1&quot;。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="500"/>
        <source>Invalid tag metadata received from GitHub (%1).</source>
        <translation>从 GitHub 收到的标签元数据无效 (%1)。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="528"/>
        <location filename="../core/UpdateChecker.cpp" line="620"/>
        <source>No published releases or version tags were found in %1.</source>
        <translation>在 %1 中未找到已发布的版本或版本标签。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="612"/>
        <source>Invalid release feed received from GitHub (%1).</source>
        <translation>从 GitHub 收到的发布源数据无效（%1）。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="764"/>
        <source>GitHub API rate limit exceeded and the release feed could not be read (%1). Please try again later or check the releases page directly.</source>
        <translation>GitHub API 请求频率受限，且无法读取发布源（%1）。请稍后再试或直接打开发布页面查看。</translation>
    </message>
    <message>
        <location filename="../core/UpdateChecker.cpp" line="762"/>
        <location filename="../core/UpdateChecker.cpp" line="772"/>
        <source>HTTP %1: %2</source>
        <translation>HTTP %1: %2</translation>
    </message>
</context>
</TS>
