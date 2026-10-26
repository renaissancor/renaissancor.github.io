# Wireshark Install Setup

Notes from Chappell University (<https://www.chappell-university.com/>).

### Install (Windows)

| component | install? |
|---|---|
| Wireshark | yes |
| Npcap | yes: the real-time capture driver |
| TShark | yes: command-line Wireshark |
| Extcap: Etwdump (Windows event tracing), UDPdump | optional |
| Extcap: Randpktdump | no, random packets for fun |
| USBPcap | no: can slow the OS |

### Capture

- Capture on Wi-Fi for a desktop on wireless, Ethernet for a wired laptop: whichever carries the traffic.
- Keep capture files around **200 MB**; above **500 MB** analysis gets painful. Thousands of packets is small, millions is slow.

### Follow one TCP session

Wireshark calls a TCP session a **stream**, numbered from **0**.

| goal | how |
|---|---|
| show one session | display filter `tcp.stream eq 0` |
| reassemble its bytes | right-click → Follow → TCP Stream (HTTP is fully readable) |
| color one session | right-click → Colorize Conversation → TCP; `Ctrl+1` … `Ctrl+0` assign colors |
| undo coloring | View → Reset Colorization (`Ctrl+Space`) |

### Extract files

File → Export Objects → HTTP: lists every file carried over HTTP, with preview and save.

### Statistics

Statistics → Endpoints: every address that talked, with packet and byte counts.
