# <img src="src/assets/cangaroo.png" width="48" height="48"> CANgaroo-jl

**SJTU RoboMaster 交龙战队（SJTU-RoboMaster-Team）基于 [`Schildkroet/CANgaroo`](https://github.com/Schildkroet/CANgaroo) 二次定制与维护的开源 CAN / CAN-FD 总线分析上位机（Windows / Linux）。**

## 🔩 支持的接口与硬件

| 接口 | Linux | Windows | 说明 |
| :--- | :---: | :---: | :--- |
| **SocketCAN** | ✅ | — | 支持内核 CAN 接口（`can0`、`vcan0` 等），在未开启 `fd on` 前亦可识别控制器的 CAN-FD 能力 |
| **Candlelight / CANable / CANnectivity** | ✅ | ✅ | CANable（Candlelight 固件）、MKS CANable、CANtact、CANnectivity 等 `gs_usb` 设备；支持多通道、CAN-FD 及固件版本读取 |
| **SLCAN** | ✅ | ✅ | CANable（SLCAN 固件）、Arduino CAN 等串口设备；支持扫描阶段识别固件版本（含 CANable 2.5） |
| **Vector** | — | ✅ | VN 系列等 Vector 设备（通过 Qt SerialBus，默认启用；运行时需本机安装 Vector XL Driver Library） |
| **PEAK PCAN** | — | ✅ | PCAN-USB / PCAN-USB Pro / PCAN-PCIe 等（需通过 `CONFIG+=peakcan` 编译并配置 PCAN-Basic SDK） |
| **Kvaser** | ✅ | ✅ | USB/CAN Leaf 等 Kvaser 设备（需通过 `CONFIG+=kvaser` 编译并配置 CANlib SDK） |

## ⚙️ 主要功能

* **实时 CAN / CAN-FD 解析与 DBC 支持**：支持标准 CAN 与高速 CAN-FD 帧（最高 64 字节载荷）；支持加载多份 `.dbc` 数据库进行实时信号解析与工程量换算。
* **多视图信号可视化（Graph View）**：支持时序波形（Time-series）、散点图（Scatter）、文本监测（Text）与仪表盘（Gauge）视图，支持缩放、悬浮数值提示与独立弹窗。
* **总线状态与硬件诊断**：内置 CAN 总线状态监控、多条件报文过滤与触发录制，支持通过 **Measurement > Hardware...** 查看所有已识别驱动、设备状态、总线错误状态、固件版本、能力掩码及支持的波特率，并可一键复制纯文本诊断报告。
* **报文发送与周期生成器**：支持单帧/手动报文发送（Raw Tx）、多任务周期调度生成（Tx Generator，支持随机载荷填充）。
* **Python 自动化脚本**：内置脚本编辑器与嵌入式 Python 解释器（基于 `pybind11`，多标签页共享同一解释器），支持使用已加载的 DBC 解码与收发总线报文；示例脚本见 [`examples/`](examples/)。
* **CAN 网关与日志回放**：支持双 CAN 口过滤转发网关（CAN Gateway），以及导入回放日志（Vector ASC、`candump`、PCAP、PCAPng），支持倍速控制、RX/TX 方向过滤与通道映射。
* **多格式轨迹导出**：支持导出为 Vector ASC、Vector MDF4、Linux `candump`、PCAP 与 Wireshark 兼容 PCAPng 格式。
* **原生深浅主题与在线更新检测**：兼容 Windows 11 系统原生深/浅色模式与自定义暗色主题；支持通过 **Help > Check for Updates...** 从当前仓库 [`SJTU-RoboMaster-Team/CANgaroo-jl`](https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl) 检测新版本。

<br>![Cangaroo Trace View](docs/view.png)<br>

## 🌐 界面语言

* 🇺🇸 English（默认）
* 🇨🇳 简体中文

## 🛠️ 从源码构建

### 🪟 Windows（MSYS2 UCRT64 + Qt 6）

推荐在 **MSYS2 UCRT64** 环境中构建（与仓库 CI 一致）：

1. 安装依赖包：
   ```bash
   pacman -S --needed \
     mingw-w64-ucrt-x86_64-toolchain \
     mingw-w64-ucrt-x86_64-qt6 \
     mingw-w64-ucrt-x86_64-qt6-tools \
     mingw-w64-ucrt-x86_64-make \
     mingw-w64-ucrt-x86_64-pkgconf \
     mingw-w64-ucrt-x86_64-ntldd \
     mingw-w64-ucrt-x86_64-python \
     mingw-w64-ucrt-x86_64-pybind11
   ```
2. 编译主程序（输出位于 `bin/cangaroo.exe`）：
   ```bash
   cd src
   qmake6 CONFIG+=release
   mingw32-make -j$(nproc)
   ```
3. 独立运行库部署（若需脱离 MSYS2 终端直接双击运行）：
   - 使用 `windeployqt6 --release --no-system-d3d-compiler bin/cangaroo.exe` 部署 Qt6 DLL 与插件。
   - 使用 `ntldd -R` 复制所需的 UCRT64 运行库及 `libpython3.*.dll`，并将 Python 标准库放到 `bin/lib/pythonX.Y/`（打包细节可参考 [`.github/workflows/cmake.yml`](.github/workflows/cmake.yml)）。

### 🐧 Linux

#### 安装依赖

| 发行版 | 命令 |
| :--- | :--- |
| **Ubuntu / Debian** | `sudo apt install build-essential qt6-base-dev qt6-charts-dev qt6-serialport-dev qt6-serialbus-dev qt6-svg-dev qt6-tools-dev qt6-l10n-tools libqt6opengl6-dev libnl-3-dev libnl-route-3-dev libusb-1.0-0-dev python3-dev pybind11-dev pkg-config` |
| **Fedora** | `sudo dnf install gcc-c++ make qt6-qtbase-devel qt6-qtcharts-devel qt6-qtserialport-devel qt6-qtserialbus-devel qt6-qtsvg-devel qt6-qttools-devel libnl3-devel libusb1-devel python3-devel pybind11-devel pkgconfig` |
| **Arch Linux** | `sudo pacman -S base-devel qt6-base qt6-charts qt6-serialport qt6-serialbus qt6-svg qt6-tools libnl libusb python pybind11 pkgconf` |

#### 编译

```bash
qmake6
make -j$(nproc)
```

生成的可执行文件位于 `bin/cangaroo`。

#### SocketCAN 权限配置

CANgaroo 通过 `ip link` 配置 SocketCAN 接口的波特率、采样点与 CAN-FD 参数，非 root 启动时会通过 `pkexec`（polkit）一次合并执行配置脚本（不使用 `sudo`）。

* 安装 `.deb` 包会自动部署 polkit 规则，将当前用户加入 `netdev` 用户组即可免密配置（需重新登录生效）：
  ```bash
  sudo usermod -aG netdev $USER
  ```
* 若从源码构建，可手动安装规则：
  ```bash
  sudo cp packaging/10-cangaroo-socketcan.rules /etc/polkit-1/rules.d/
  ```
* 若接口在 Setup 中勾选 **Configured by OS**，CANgaroo 不会修改系统接口参数，也无需提权。

#### USB 设备权限（`udev` 规则）

通过 `gs_usb` / Candlelight / CANable 接入设备时，建议配置 `udev` 规则以便普通用户直接打开：

创建 `/etc/udev/rules.d/99-cangaroo.rules`：

```text
# gs_usb / Candlelight / CANable (gs_usb firmware)
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1d50", ATTRS{idProduct}=="606b", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1d50", ATTRS{idProduct}=="606f", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1209", ATTRS{idProduct}=="ca01", MODE="0666", GROUP="plugdev", TAG+="uaccess"
```

重载规则并重新插拔设备：

```bash
sudo udevadm control --reload-rules && sudo udevadm trigger
```

> 注：需确保当前用户在 `plugdev` 组内（`sudo usermod -aG plugdev $USER`，重新登录生效）。

### 可选商用硬件驱动编译参数

* **PEAK PCAN（`CONFIG+=peakcan`，Windows）**：
  1. 下载 [PCAN-Basic SDK](https://www.peak-system.com/fileadmin/media/files/PCAN-Basic.zip) 并解压至 `src/driver/PeakCanDriver/pcan-basic-api/`。
  2. 使用 `qmake6 CONFIG+=peakcan` 编译。
  3. 将 `pcan-basic-api/x64/PCANBasic.dll` 复制到可执行文件同级目录。
* **Kvaser（`CONFIG+=kvaser`，Linux / Windows）**：
  * **Linux**：下载编译安装 [linuxcan](https://www.kvaser.com/downloads-kvaser/)（V5.51.461 或更新版本）后执行 `qmake6 CONFIG+=kvaser`。
  * **Windows**：安装 Kvaser CANlib SDK 后使用 `qmake6 CONFIG+=kvaser CANLIB_DIR="C:/path/to/Kvaser/Canlib"` 编译，并将 `Canlib/Bin/canlib32.dll` 放到可执行文件同级目录。
* **Vector（Windows，默认启用）**：
  * 无需编译期 SDK；目标机安装 [Vector XL Driver Library](https://www.vector.com/int/en/products/products-a-z/libraries-drivers/xl-driver-library/) 后即可通过 Qt SerialBus 使用。

### 运行单元测试

```bash
cd tests
qmake6
make -j$(nproc)   # Windows MSYS2 UCRT64 下使用 mingw32-make
make check
```

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
