#pragma once

const char* const status_msg[] = {"Idle", "No SSID available", "Scan completed", "Connected", "Failed", "Connection lost", "Disconnected"};
const char* const scan_msg[] = {"No networks found", "Scan in progress", "Scan failed"};
const char* const mode_msg[] = {"Off", "Station", "AP", "Soft AP"};

void wifi_menu();
void wifi_status();
void init_wifi();