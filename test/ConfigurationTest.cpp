// Copyright 2021-2024 Rik Essenius
// 
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

#include <Preferences.h>
#include "Configuration.h"
#include "gtest/gtest.h"


namespace Esp32NetConfigCppTest {
    using Esp32NetConfig::Configuration;
    using Esp32NetConfig::FirmwareConfig;
    using Esp32NetConfig::IpConfig;
    using Esp32NetConfig::MqttConfig;
    using Esp32NetConfig::TlsConfig;
    using Esp32NetConfig::WifiConfig;

    TEST(ConfigurationTest, loadSecretsTest) {
        constexpr TlsConfig ConfigTls{ "rootCA", "deviceCert", "deviceKey" };
        constexpr MqttConfig ConfigMqtt{ "broker", 1883, "user", "password", true };
        constexpr WifiConfig ConfigWifi{ "ssid", "password", "deviceName", nullptr };
        constexpr FirmwareConfig ConfigFirmware{ "http://localhost/firmware" };
        const IpConfig configIp = Esp32NetConfig::IpAutoConfig;

        Preferences preferences;
        preferences.reset();
        Configuration configuration(&preferences);
        configuration.putIpConfig(&configIp);
        configuration.putMqttConfig(&ConfigMqtt);
        configuration.putTlsConfig(&ConfigTls);
        configuration.putWifiConfig(&ConfigWifi);
        configuration.putFirmwareConfig(&ConfigFirmware);

        EXPECT_EQ(nullptr, configuration.mqtt.broker) << "Broker empty";
        EXPECT_EQ(0u, configuration.mqtt.port) << "Port not filled";
        EXPECT_FALSE(configuration.mqtt.useTls);

        configuration.begin();

        EXPECT_EQ(8118, configuration.freeBufferSpace()) << "Free buffer space OK";

        EXPECT_EQ(INADDR_NONE, configuration.ip.localIp);
        EXPECT_STREQ("broker", configuration.mqtt.broker) << "Broker filled";
        EXPECT_EQ(1883u, configuration.mqtt.port) << "Port filled";
        EXPECT_STREQ("rootCA", configuration.tls.rootCaCertificate) << "Root CA certificate filled";
        EXPECT_TRUE(configuration.mqtt.useTls);
        EXPECT_STREQ("ssid", configuration.wifi.ssid) << "SSID filled";
        EXPECT_STREQ("http://localhost/firmware", configuration.firmware.baseUrl) << "Firmware base URL filled";
    }

    TEST(ConfigurationTest, mqttAndTlsTest) {
        Preferences preferences;
        preferences.reset();
        constexpr MqttConfig MqttConfig{ "broker", 2048, "user", "password", false };
        constexpr TlsConfig TlsConfig{ "abc", R"(defg)", R"(hijkl)" };
        Configuration configuration(&preferences);
        configuration.putMqttConfig(&MqttConfig);
        configuration.putTlsConfig(&TlsConfig);
        constexpr FirmwareConfig FirmwareConfig{ "http://localhost/firmware" };
        configuration.putFirmwareConfig(&FirmwareConfig);

        configuration.begin();

        EXPECT_STREQ("broker", configuration.mqtt.broker) << "Broker OK";
        EXPECT_EQ(2048u, configuration.mqtt.port) << "Port OK";
        EXPECT_STREQ("user", configuration.mqtt.user) << "User OK";
        EXPECT_STREQ("password", configuration.mqtt.password) << "Password OK";
        EXPECT_EQ(0u, configuration.ip.localIp) << "IP 0";
        EXPECT_EQ(0u, configuration.ip.secondaryDns) << "DNS2 0";
        EXPECT_STREQ("abc", configuration.tls.rootCaCertificate) << "rootCA ok";
        EXPECT_STREQ("defg", configuration.tls.deviceCertificate) << "device cert ok";
        EXPECT_STREQ("hijkl", configuration.tls.devicePrivateKey) << "device key ok";
        EXPECT_EQ(nullptr, configuration.wifi.deviceName) << "deviceName null";
        EXPECT_STREQ("http://localhost/firmware", configuration.firmware.baseUrl) << "firmware url ok";
    }

    TEST(ConfigurationTest, putNullTest) {
        Preferences preferences;
        preferences.reset();
        const Configuration configuration(&preferences);
        configuration.putIpConfig(nullptr);
        configuration.putMqttConfig(nullptr);
        configuration.putTlsConfig(nullptr);
        configuration.putWifiConfig(nullptr);
        configuration.putFirmwareConfig(nullptr);

        EXPECT_EQ(0, configuration.ip.subnetMask) << "subnet mask not set";
        EXPECT_EQ(nullptr, configuration.mqtt.broker) << "Broker filled";
        EXPECT_EQ(nullptr, configuration.tls.rootCaCertificate) << "Root CA certificate filled";
        EXPECT_EQ(nullptr, configuration.wifi.ssid) << "SSID filled";
        EXPECT_EQ(nullptr, configuration.firmware.baseUrl) << "Firmware base URL filled";
    }

    TEST(ConfigurationTest, WifiAndIpTest) {
        Preferences preferences;
        uint8_t bssidConfig[6] = { 0, 1, 2, 3, 4, 5 };
        const WifiConfig wifiConfig{ "ssid", "password", "deviceName", bssidConfig };
        const IpConfig ipConfig{ {1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7}, {5, 6, 7, 8} };
        Configuration configuration(&preferences);
        configuration.putWifiConfig(&wifiConfig);
        configuration.putIpConfig(&ipConfig);
        configuration.begin();
        EXPECT_STREQ("ssid", configuration.wifi.ssid) << "SSID OK";
        EXPECT_STREQ("password", configuration.wifi.password) << "Password OK";
        EXPECT_STREQ("deviceName", configuration.wifi.deviceName) << "Device name OK";
        const auto bssid = configuration.wifi.bssid;
        EXPECT_NE(nullptr, bssid) << "BSSID not null";
        for (unsigned int i = 0; i < sizeof bssidConfig; i++) {
            EXPECT_EQ(bssidConfig[i], bssid[i]) << "bssid[i] ok";
        }
        EXPECT_EQ(0x04030201, configuration.ip.localIp) << "localIP 0";
        EXPECT_EQ(0x05040302, configuration.ip.gateway) << "gateway ok";
        EXPECT_EQ(0x06050403, configuration.ip.subnetMask) << "subnet ok";
        EXPECT_EQ(0x07060504, configuration.ip.primaryDns) << "DNS1 ok";
        EXPECT_EQ(0x08070605, configuration.ip.secondaryDns) << "DNS2 ok";
        EXPECT_EQ(nullptr, configuration.tls.rootCaCertificate) << "rootCA null";
        EXPECT_EQ(nullptr, configuration.mqtt.broker) << "broker null";
        EXPECT_EQ(1883u, configuration.mqtt.port) << "port 1883";
    }
}
