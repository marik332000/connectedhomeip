/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2020 Texas Instruments Incorporated
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/**
 *    @file
 *          Platform-specific connectivity manager class for the Texas
 *          Instruments BL702 platform.
 */

#pragma once

#include <platform/ConnectivityManager.h>
#include <platform/internal/GenericConnectivityManagerImpl.h>
#include <platform/internal/GenericConnectivityManagerImpl_UDP.h>
#if INET_CONFIG_ENABLE_TCP_ENDPOINT
#include <platform/internal/GenericConnectivityManagerImpl_TCP.h>
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
#include <platform/internal/GenericConnectivityManagerImpl_BLE.h>
#else
#include <platform/internal/GenericConnectivityManagerImpl_NoBLE.h>
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/internal/GenericConnectivityManagerImpl_Thread.h>
#else
#include <platform/internal/GenericConnectivityManagerImpl_NoThread.h>
#endif
#include <platform/internal/GenericConnectivityManagerImpl_NoWiFi.h>

namespace chip {
namespace DeviceLayer {

/**
 * Concrete implementation of the ConnectivityManager singleton object for the CC13X2_26X2 platforms.
 */
class ConnectivityManagerImpl final : public ConnectivityManager,
                                      public Internal::GenericConnectivityManagerImpl<ConnectivityManagerImpl>,
                                      public Internal::GenericConnectivityManagerImpl_UDP<ConnectivityManagerImpl>,
#if INET_CONFIG_ENABLE_TCP_ENDPOINT
                                      public Internal::GenericConnectivityManagerImpl_TCP<ConnectivityManagerImpl>,
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
                                      public Internal::GenericConnectivityManagerImpl_BLE<ConnectivityManagerImpl>,
#else
                                      public Internal::GenericConnectivityManagerImpl_NoBLE<ConnectivityManagerImpl>,
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
                                      public Internal::GenericConnectivityManagerImpl_Thread<ConnectivityManagerImpl>,
#else
                                      public Internal::GenericConnectivityManagerImpl_NoThread<ConnectivityManagerImpl>,
#endif
                                      public Internal::GenericConnectivityManagerImpl_NoWiFi<ConnectivityManagerImpl>
{
    // Allow the ConnectivityManager interface class to delegate method calls to
    // the implementation methods provided by this class.
    friend class ConnectivityManager;

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
public:
    static WiFiStationState mWiFiStationState;
    static void GetWiFiMacAddress(uint8_t *);
#endif

private:
    // ===== Members that implement the ConnectivityManager abstract interface.

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    bool _IsWiFiStationConnected(void);
    bool _IsWiFiStationEnabled(void);
    ConnectivityManager::WiFiStationMode _GetWiFiStationMode(void);

    friend void DriveState(int arg);
    void NetworkChanged(void);
    bool _IsWiFiStationProvisioned(void);
    void _ClearWiFiStationProvision(void);
#endif

    CHIP_ERROR _Init(void);
    void _OnPlatformEvent(const ChipDeviceEvent * event);

    // ===== Members for internal use by the following friends.

    friend ConnectivityManager & ConnectivityMgr(void);
    friend ConnectivityManagerImpl & ConnectivityMgrImpl(void);

    static ConnectivityManagerImpl sInstance;
};

/**
 * Returns the public interface of the ConnectivityManager singleton object.
 *
 * chip applications should use this to access features of the ConnectivityManager object
 * that are common to all platforms.
 */
inline ConnectivityManager & ConnectivityMgr(void)
{
    return ConnectivityManagerImpl::sInstance;
}

/**
 * Returns the platform-specific implementation of the ConnectivityManager singleton object.
 *
 * chip applications can use this to gain access to features of the ConnectivityManager
 * that are specific to the BL702 platform
 */
inline ConnectivityManagerImpl & ConnectivityMgrImpl(void)
{
    return ConnectivityManagerImpl::sInstance;
}

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
inline bool ConnectivityManagerImpl::_IsWiFiStationConnected(void)
{
    return mWiFiStationState == kWiFiStationState_Connected;
}

inline bool ConnectivityManagerImpl::_IsWiFiStationEnabled(void)
{
    return true;
}

inline ConnectivityManager::WiFiStationMode ConnectivityManagerImpl::_GetWiFiStationMode(void)
{
    return kWiFiStationMode_Enabled;
}

#endif

} // namespace DeviceLayer
} // namespace chip
