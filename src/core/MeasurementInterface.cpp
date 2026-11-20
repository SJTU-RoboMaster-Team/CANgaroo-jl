/*

  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
  Copyright (c) 2026 Schildkroet

  This file is part of cangaroo.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "MeasurementInterface.h"

#include "core/Backend.h"
#include "driver/CanDriver.h"
#include "driver/BusInterface.h"

MeasurementInterface::MeasurementInterface()
  : _busif(0),
    _doConfigure(true),
    _enabled(true),
    _bitrate(500000),
    _samplePoint(875),
    _isCanFD(false),
    _fdBitrate(2000000),
    _fdSamplePoint(875),

    _isListenOnlyMode(false),
    _isOneShotMode(false),
    _isTripleSampling(false),
    _doAutoRestart(false),
    _autoRestartMs(100),

    _isCustomBitrate(false),
    _isCustomFdBitrate(false),

    _CustomBitrate(0x023407),
    _CustomFdBitrate(0x011508)
{

}

bool MeasurementInterface::loadXML(Backend &backend, QDomElement &el)
{
    (void) backend;

    _savedDriverName = el.attribute("driver");
    _savedInterfaceName = el.attribute("name");
    _savedHardwareKey = el.attribute("hardware-key");

    _doConfigure = el.attribute("configure", "0").toInt() != 0;

    _bitrate = el.attribute("bitrate", "500000").toInt();
    _samplePoint = el.attribute("sample-point", "875").toInt();
    _isCanFD = el.attribute("can-fd", "0").toInt() != 0;
    _fdBitrate = el.attribute("bitrate-fd", "500000").toInt();
    _fdSamplePoint = el.attribute("sample-point-fd", "875").toInt();

    _isListenOnlyMode = el.attribute("listen-only", "0").toInt() != 0;
    _isOneShotMode = el.attribute("one-shot", "0").toInt() != 0;
    _isTripleSampling = el.attribute("triple-sampling", "0").toInt() != 0;
    _doAutoRestart = el.attribute("auto-restart", "0").toInt() != 0;
    _autoRestartMs = el.attribute("auto-restart-time", "100").toInt();

    _isCustomBitrate = el.attribute("is-custom-bitrate", "0").toInt() != 0;
    _isCustomFdBitrate = el.attribute("is-custom-fdbitrate", "0").toInt() != 0;

    _CustomBitrate = el.attribute("custom-bitrate", "0").toInt();
    _CustomFdBitrate = el.attribute("custom-fdbitrate", "0").toInt();
    _enabled = el.attribute("enabled", "1").toInt() != 0;

    return true;
}

bool MeasurementInterface::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    (void) xml;

    if (_isResolved)
    {
        _savedDriverName = backend.getDriverName(_busif);
        _savedInterfaceName = backend.getInterfaceName(_busif);
        if (BusInterface *intf = backend.getInterfaceById(_busif))
        {
            _savedHardwareKey = intf->hardwareKey();
        }
    }

    root.setAttribute("driver", _savedDriverName);
    root.setAttribute("name", _savedInterfaceName);
    if (!_savedHardwareKey.isEmpty())
    {
        root.setAttribute("hardware-key", _savedHardwareKey);
    }

    root.setAttribute("configure", _doConfigure ? 1 : 0);

    root.setAttribute("bitrate", _bitrate);
    root.setAttribute("sample-point", _samplePoint);
    root.setAttribute("can-fd", _isCanFD ? 1 : 0);
    root.setAttribute("bitrate-fd", _fdBitrate);
    root.setAttribute("sample-point-fd", _fdSamplePoint);

    root.setAttribute("listen-only", _isListenOnlyMode ? 1 : 0);
    root.setAttribute("one-shot", _isOneShotMode ? 1 : 0);
    root.setAttribute("triple-sampling", _isTripleSampling ? 1 : 0);
    root.setAttribute("auto-restart", _doAutoRestart ? 1 : 0);
    root.setAttribute("auto-restart-time", _autoRestartMs);

    root.setAttribute("is-custom-bitrate", _isCustomBitrate ? 1 : 0);
    root.setAttribute("is-custom-fdbitrate", _isCustomFdBitrate ? 1 : 0);

    root.setAttribute("custom-bitrate", _CustomBitrate);
    root.setAttribute("custom-fdbitrate", _CustomFdBitrate);
    root.setAttribute("enabled", _enabled ? 1 : 0);

    return true;
}

unsigned MeasurementInterface::bitrate() const
{
    return _bitrate;
}

void MeasurementInterface::setBitrate(unsigned bitrate)
{
    _bitrate = bitrate;
}

BusInterfaceId MeasurementInterface::busInterface() const
{
    return _busif;
}

void MeasurementInterface::setBusInterface(BusInterfaceId busif)
{
    _busif = busif;
}

void MeasurementInterface::cloneFrom(MeasurementInterface &origin)
{
    *this = origin;
}

bool MeasurementInterface::doConfigure() const
{
    return _doConfigure;
}

void MeasurementInterface::setDoConfigure(bool doConfigure)
{
    _doConfigure = doConfigure;
}

bool MeasurementInterface::isListenOnlyMode() const
{
    return _isListenOnlyMode;
}

void MeasurementInterface::setListenOnlyMode(bool isListenOnlyMode)
{
    _isListenOnlyMode = isListenOnlyMode;
}

bool MeasurementInterface::isOneShotMode() const
{
    return _isOneShotMode;
}

void MeasurementInterface::setOneShotMode(bool isOneShotMode)
{
    _isOneShotMode = isOneShotMode;
}

bool MeasurementInterface::isTripleSampling() const
{
    return _isTripleSampling;
}

void MeasurementInterface::setTripleSampling(bool isTripleSampling)
{
    _isTripleSampling = isTripleSampling;
}

bool MeasurementInterface::isCanFD() const
{
    return _isCanFD;
}

void MeasurementInterface::setCanFD(bool isCanFD)
{
    _isCanFD = isCanFD;
}

int MeasurementInterface::samplePoint() const
{
    return _samplePoint;
}

void MeasurementInterface::setSamplePoint(int samplePoint)
{
    _samplePoint = samplePoint;
}

unsigned MeasurementInterface::fdBitrate() const
{
    return _fdBitrate;
}

void MeasurementInterface::setFdBitrate(unsigned fdBitrate)
{
    _fdBitrate = fdBitrate;
}

unsigned MeasurementInterface::fdSamplePoint() const
{
    return _fdSamplePoint;
}

void MeasurementInterface::setFdSamplePoint(unsigned fdSamplePoint)
{
    _fdSamplePoint = fdSamplePoint;
}

bool MeasurementInterface::doAutoRestart() const
{
    return _doAutoRestart;
}

void MeasurementInterface::setAutoRestart(bool doAutoRestart)
{
    _doAutoRestart = doAutoRestart;
}

int MeasurementInterface::autoRestartMs() const
{
    return _autoRestartMs;
}

void MeasurementInterface::setAutoRestartMs(int autoRestartMs)
{
    _autoRestartMs = autoRestartMs;
}

bool MeasurementInterface::isCustomBitrate() const
{
    return _isCustomBitrate;
}

void MeasurementInterface::setCustomBitrateEn(bool customBitrate)
{
    _isCustomBitrate = customBitrate;
}

bool MeasurementInterface::isCustomFdBitrate() const
{
    return _isCustomFdBitrate;
}

void MeasurementInterface::setCustomFdBitrateEn(bool customFdBitrate)
{
    _isCustomFdBitrate = customFdBitrate;
}

uint32_t MeasurementInterface::customBitrate() const
{
    return _CustomBitrate;
}

void MeasurementInterface::setCustomBitrate(uint32_t customBitrate)
{
    _CustomBitrate = customBitrate;
}

uint32_t MeasurementInterface::customFdBitrate() const
{
    return _CustomFdBitrate;
}

void MeasurementInterface::setCustomFdBitrate(uint32_t customFdBitrate)
{
    _CustomFdBitrate = customFdBitrate;
}

bool MeasurementInterface::isEnabled() const noexcept
{
    return _enabled;
}

void MeasurementInterface::setEnabled(bool enabled) noexcept
{
    _enabled = enabled;
}

bool    MeasurementInterface::isResolved() const noexcept { return _isResolved; }
void    MeasurementInterface::setResolved(bool resolved) noexcept { _isResolved = resolved; }
QString MeasurementInterface::savedDriverName() const { return _savedDriverName; }
QString MeasurementInterface::savedInterfaceName() const { return _savedInterfaceName; }

QString MeasurementInterface::savedHardwareKey() const
{
    return _savedHardwareKey;
}

void MeasurementInterface::setSavedIdentity(const QString &driverName, const QString &interfaceName, const QString &hardwareKey)
{
    _savedDriverName = driverName;
    _savedInterfaceName = interfaceName;
    _savedHardwareKey = hardwareKey;
}
