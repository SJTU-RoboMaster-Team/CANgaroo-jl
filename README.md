# <img src="src/assets/cangaroo.png" width="48" height="48"> CANgaroo-jl

**SJTU RoboMaster 交龙战队（SJTU-RoboMaster-Team）基于 [`Schildkroet/CANgaroo`](https://github.com/Schildkroet/CANgaroo) 二次定制与维护的 CAN / CAN-FD 总线分析上位机，只面向 Windows + candleLight（`gs_usb`）固件开发。**

## 🔩 支持的接口与硬件

| 接口 | Windows | 说明 |
| :--- | :---: | :--- |
| **candleLight / CANable / CANnectivity** | ✅ | 走 WinUSB 的 `gs_usb` 设备：candleLight 固件的 STM32 适配器、CANable、MKS CANable、CANtact、CANnectivity 等；支持多通道、CAN-FD 与固件版本读取 |

仓库不再包含 SocketCAN、SLCAN、Vector、PEAK、Kvaser 驱动，也不再有 Linux 构建路径。

## ⚙️ 主要功能

* **实时 CAN / CAN-FD 解析与 DBC 支持**：标准帧与高速 CAN-FD 帧（最高 64 字节载荷）；可加载多份 `.dbc` 做实时信号解析与工程量换算。
* **多视图信号可视化（Graph View）**：时序波形、散点图、文本监测与仪表盘视图，支持缩放、悬浮数值提示与独立弹窗。
* **Trace 视图**：单一报文列表，可在「聚合」与「滚动日志」之间切换；支持绝对 / UTC / 相对 / 增量时间戳、按 ID/名称/正则过滤、方向与接口过滤，右键可直接按所选 CAN ID 创建触发规则。
* **总线状态与硬件诊断**：内置 CAN 总线状态监控；**Measurement > Hardware...** 显示所有已识别设备、总线状态、固件版本、能力掩码与可用波特率，可一键复制纯文本诊断报告。
* **报文发送与周期生成器**。Message View 默认显示在 Trace 页签右下角，是该分组的第一个标签。在 Message View 中选择接口，启动测量后点 **Send** 发送一帧，或打开 **Repeat** 并用 **Interval** 设置周期（1–100000 ms，默认 100 ms）。周期发送会使用最新的编辑内容，停止测量、接口断开或关闭 Message View 时自动停止。状态栏统计发送请求。同一页签内，Generator View 中选中的报文会载入 Message View。Generator View 仍支持多任务周期调度。
* **日志回放**：导入 Vector ASC、Linux `candump`、CAN/FD 原始帧 `.csv` 三种文本格式，统一由纯 QtCore 的 `TraceFileReader` 解析；回放窗口提供 Play / Pause / Resume / Stop / 单步 / 进度条定位，保留原始帧间隔 `Δt`，支持倍速、RX/TX 方向过滤与通道映射，并可 **Load to Trace** 直接载入 Trace/Graph 做离线分析。
* **大日志分段处理**：**Trace > Import full trace** 或回放载入超过 `trace/maxSize` 上限的日志时，弹窗提供「分段保存 / 在回放窗口中打开 / 仅加载前 N 帧」三种处理方式；回放窗口的 **Split Save...** 可按帧数拆分。
* **轨迹导出与连续录制**：三种格式 —— Vector ASC、Linux `candump`、CAN/FD 原始帧 CSV（`Timestamp_s, Epoch_us, Channel, Direction, BusType, ID_Hex, Extended, FD, BRS, ESI, RTR, ErrorFlags, DLC, Data_Hex`）；录制支持按大小分卷，可选择是否在离线回放期间同时录制。
* **触发捕获（黑匣子）**：**Measurement > Trigger Capture...** 支持总线错误帧、帧 ID/字节掩码匹配、看门狗丢帧超时、DBC 信号阈值四类触发规则；触发时导出「触发前 N 秒 + 触发后 M 秒」的切片文件，并在 Graph 时间轴上标注触发线。
* **Graph 双游标测量**：**Cursors (Δt/Δy)** 开关，左键放置游标 A，右键或 Shift+左键放置游标 B，显示 `Δt`、`1/|Δt|` 与首个信号的 `Δy`。
* **USB 掉线提示与手动重新绑定**：测量中接口异常断开时，主窗口顶部显示提示条并弹出非阻塞提示，不会静默自动重连；重新插好设备后点击 **Rebind & Resume**（顶部提示条或 **Measurement > Hardware...**），按 `Hardware Key`（Candle 设备为去掉接口后缀的 USB 设备路径 + 通道号；旧工作空间无该字段时按驱动 + 唯一接口名匹配，缺失或歧义会明确报错）重新绑定并继续测量，已有 Trace 与 Graph 曲线保留。
* **电机协议解码（Motor View）**：**Window → New → Motor View**（`Ctrl+Shift+M`）内置解码 DJI M3508 / M2006 / GM6020 与达妙 DM（MIT 模式）的反馈帧，一屏显示角度、转速、电流/转矩、温度与在线状态；Pmax/Vmax/Tmax 等按型号可配。配置会自动生成一份内置 DBC，Trace 显示信号子行、Graph 的 **Motor (built-in)** 树可直接画曲线，也可用 **Export DBC...** 导出 `.dbc`。详见 [docs/motor_protocols.md](docs/motor_protocols.md)。
* **原生深浅主题与在线更新检测**：跟随 Windows 11 系统原生深/浅色模式，也可以在设置里切换 Qt 窗口样式（QStyle）；**Help > Check for Updates...** 从本仓库检测新版本。GitHub API 的免认证请求频率受限时，会自动改用 Releases 原子源（`releases.atom`）完成检测；两者都不可用才提示打开发布页手动确认。

<br>![Cangaroo Trace View](docs/view.png)<br>

## 🌐 界面语言

* 🇺🇸 English（默认）
* 🇨🇳 简体中文

## 🛠️ 从源码构建

### 依赖

**Windows + MSYS2 UCRT64**（与 CI 一致）：

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-qt6 \
  mingw-w64-ucrt-x86_64-qt6-tools \
  mingw-w64-ucrt-x86_64-make \
  mingw-w64-ucrt-x86_64-ntldd
```

Qt 模块：`core gui widgets xml charts svg network`。驱动额外链接 `-lSetupApi -lOle32 -lwinusb`，无需第三方 SDK，也不依赖 Python。

### 编译

```bash
cd src
qmake6 CONFIG+=release
mingw32-make -j$(nproc)
```

产物为 `bin/cangaroo.exe`。

### 独立运行库部署

若要从资源管理器直接双击（不经过 MSYS2 终端）：

```bash
windeployqt6 --release --no-system-d3d-compiler bin/cangaroo.exe
ntldd -R bin/cangaroo.exe   # 复制未覆盖的 UCRT64 运行库到 bin/
```

CI 的 `build-windows` 作业执行同样的步骤，可参考 [`.github/workflows/cmake.yml`](.github/workflows/cmake.yml)。

### 设备驱动（WinUSB）

candleLight 设备在 Windows 上使用 WinUSB 驱动。烧录固件后如未被识别，可用 [Zadig](https://zadig.akeo.ie/) 将设备的驱动替换为 WinUSB（设备 VID/PID 为 `1d50:606f` 等 `gs_usb` 标识）。

### 运行单元测试

```bash
cd tests
qmake6
mingw32-make -j$(nproc)
```

然后逐个运行 `build/tests/*_test.exe`（`make check` 依赖 qmake 生成的 `target_wrapper.sh`，在 Windows `cmd` 下不可用）。

## 🔁 CI

* `build-windows`：MSYS2 UCRT64 上编译主程序与单元测试、跑完所有测试，并打包 `CANgaroo-<version>-windows-x64.zip`。
* `release`：仅在打 `V*` / `v*` 标签时运行，校验标签与 `src/src.pro` 的 `VERSION` 一致后发布草稿 Release。

## 📥 下载

发布包见当前仓库的 [Releases 页面](https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases)。

## 📜 致谢与许可

本软件遵循 **GNU General Public License v2.0 or later（GPLv2+）** 开源协议（见 [`LICENSE`](LICENSE)）。

* 原始作者：Hubert Denkmair `<hubert@denkmair.de>`
* 上游与历史贡献者：
  * Schildkroet（[https://github.com/Schildkroet/CANgaroo](https://github.com/Schildkroet/CANgaroo)）
  * Ethan Zonca `<e@ethanzonca.com>`
  * WeAct Studio
  * Wikilift（[https://github.com/wikilift/CANgaroo](https://github.com/wikilift/CANgaroo)）
  * Jayachandran Dharuman（[https://github.com/OpenAutoDiagLabs/cangaroo](https://github.com/OpenAutoDiagLabs/cangaroo)）
* 二次开发维护：SJTU RoboMaster 交龙战队（[https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl](https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl)）

## 免责声明

本软件按“现状”（AS IS）提供，不附带任何明示或暗示的担保（包括但不限于适销性、特定用途适用性及非侵权担保）。使用本软件调试硬件设备及实车总线所产生的任何风险、数据丢失或设备损坏由使用者自行承担。
