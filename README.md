WIP calculator using the ESP32-S2

## Config
Configuration is done by storing files in SPIFFS. For example Wi-Fi setting are stored in `/config/wifi/ssid` and `/config/wifi/password`

## Controls
<pre>
| 1 | 2 | 3 | A |
| 4 | 5 | 6 | B |
| 7 | 8 | 9 | C |
| * | 0 | # | D |
</pre>
<pre>
| 1 | 2 | 3 |  +  |
| 4 | 5 | 6 |  -  |
| 7 | 8 | 9 | DEL |
| * | 0 | / | SHF |
</pre>
The keypad has only 16 buttons, so there's a lot of key combinations even for basic actions.
### Modifiers
#### D = SHIFT
<pre>
|  sqrt(  |   *10^  |         |    (    |
|    ←    |    ×    |    →    |    )    |
|         |         |    .    |  clear  |
|    ^    |   menu  |    =    |    ×    |
</pre>
### Other controls
<pre>
|         |    ↑    |         |         |
| ←       | select  |    →    |         |
|         |    ↓    |         |         |
| confirm | cancel  |         |         |
</pre>