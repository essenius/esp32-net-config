// Copyright 2024 Rik Essenius
// 
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.


// Run RixEsp32NetConfigPutDemo first to get the configuration written to Preferences

#include <Configuration.h>

namespace Esp32NetConfigCppTest {
    void printBssid(const uint8_t* bssid) {
        printf("\tbssid: ");
        if (bssid) {
            for (int i = 0; i < 6; i++)
                printf("%02x", bssid[i]);
        }
        else {
            printf("auto");
        }
        printf("\n");
    }

    void getDemo() {
        using namespace Esp32NetConfig;

        Preferences preferences;
        Configuration configuration(&preferences);
        configuration.begin();
        printf("Tls Config\n\troot CA cert: %s\n", configuration.tls.rootCaCertificate);
        printf("\tdevice cert: %s\n", configuration.tls.deviceCertificate);
        printf("\tdevice key: %s\n", configuration.tls.devicePrivateKey);

        printf("Mqtt Config\n\tbroker: %s\n", configuration.mqtt.broker);
        printf("\tport: %d\n", configuration.mqtt.port);
        printf("\tuser: %s\n", configuration.mqtt.user);
        printf("\tpassword: %s\n", configuration.mqtt.password);
        printf("\tuseTls: %s\n", configuration.mqtt.useTls ? "yes" : "no");

        printf("Wifi Config\n\tssid: %s\n", configuration.wifi.ssid);
        printf("\tpassword: %s\n", configuration.wifi.password);
        printf("\tdeviceName: %s\n", configuration.wifi.deviceName);
        printBssid(configuration.wifi.bssid);

        printf("Firmware Config\n\tbase URL: %s\n", configuration.firmware.baseUrl);

        printf("Ip Config\n\tlocal IP: %s\n", configuration.ip.localIp.toString().c_str());
        printf("\tgateway: %s\n", configuration.ip.gateway.toString().c_str());
        printf("\tsubnet mask: %s\n", configuration.ip.subnetMask.toString().c_str());
        printf("\tprimary DNS: %s\n", configuration.ip.primaryDns.toString().c_str());
        printf("\tsecondary DNS: %s\n", configuration.ip.secondaryDns.toString().c_str());
    }

    void putDemo() {
        using namespace Esp32NetConfig;

        constexpr TlsConfig ConfigTls{ "rootCA", "deviceCert", "deviceKey" };
        constexpr MqttConfig ConfigMqtt{ "broker", 1883, "user", "password", true };
        constexpr WifiConfig ConfigWifi{ "ssid", "password", "deviceName", nullptr };
        constexpr FirmwareConfig ConfigFirmware{ "http://localhost/firmware" };
        const IpConfig configIp{ {1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7}, {5, 6, 7, 8} };

        Preferences preferences;
        const Configuration configuration(&preferences);
        configuration.putIpConfig(&configIp);
        configuration.putMqttConfig(&ConfigMqtt);
        configuration.putTlsConfig(&ConfigTls);
        configuration.putWifiConfig(&ConfigWifi);
        configuration.putFirmwareConfig(&ConfigFirmware);
    }
}
#ifdef ESP32
Serial.begin(115200);
delay(500);

void setup() {
    Esp32NetConfigCppTest::putDemo();
    Esp32NetConfigCppTest::getDemo();
}

void loop() {
    // nothing to do, waiting forever.
}
#else
#include "gtest/gtest.h"

namespace Esp32NetConfigCppTest {
    TEST(RixEsp32NetConfigDemo, putAndGetDemo) {
        Preferences preferences;
        preferences.reset();
        putDemo();
        getDemo();
    }
}
#endif
