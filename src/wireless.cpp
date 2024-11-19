#include "wireless.h"
#include "ui.h"
#include "config.h"
#include "utils.h"
#include "WiFi.h"
#include <vector>

void wifi_menu()
{
	const char* o[] = {"Get status", "Scan for networks", "Connect", "Create AP"};
	while(true)
	{
		switch (list_menu(o,arraySize(o)))
		{
		case 0:
			wifi_status();
			break;
		case 1:
		{
			WiFi.mode(WIFI_STA);
			WiFi.disconnect();
			int16_t n = WiFi.scanNetworks();
			if (n > 0)
			{
				// I hate all of this
				std::vector<String> strings;
				strings.reserve(n);
				std::vector<const char *> stations;
				stations.reserve(n);
				
				for (uint8_t i = 0; i < n; i++) strings.push_back(WiFi.SSID(i));
				for (uint8_t i = 0; i < n; i++) stations.push_back(strings.at(i).c_str());

				int32_t pick = list_menu(stations.data(),n);
				if (pick == -1) break;
				WiFi.begin(stations[pick], text_input("","Password").c_str());
				WiFi.scanDelete();
			}
			else if (n <= 0) announce(scan_msg[n]);
			break;
		}
		case 2:
			WiFi.mode(WIFI_STA);
			WiFi.begin(text_input(read_config_string("/config/wifi/ssid").c_str(),"SSID").c_str(), text_input(read_config_string("/config/wifi/password").c_str(),"Password").c_str());
			break;
		case 3:
			announce("Not implemented");
			break;
		
		default:
			return;
			break;
		}
	}
}

void wifi_status()
{
	u8g2.clearBuffer();
	u8g2.setCursor(0, 0);
	u8g2.printf("Mode: %s", mode_msg[WiFi.getMode()]);
	if (WiFi.getMode() == WIFI_MODE_STA)
	{
		line_break();
		u8g2.printf("Status: %s", status_msg[WiFi.status()]);
		line_break();
		u8g2.print(WiFi.localIP());
	}
	u8g2.sendBuffer();
	keypad.wait_for_release();
}

void _on_connect(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
	announce("Connected");
}

void init_wifi()
{
	WiFi.mode(WIFI_STA);
	WiFi.onEvent(_on_connect, ARDUINO_EVENT_WIFI_STA_CONNECTED);
	if (read_config_value<bool>("/config/wifi/auto_connect", false))
	{
		WiFi.begin(read_config_string("/config/wifi/ssid").c_str(), read_config_string("/config/wifi/password").c_str());
	}
}
