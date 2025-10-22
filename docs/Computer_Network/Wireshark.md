# Wireshark Install Setup 

Chappell University 
https://www.chappell-university.com/ 

Download and Install Wireshark in Windows OS 

Checkbox 

- Wireshark 
- Npcap (Real Time Packet Capture Driver)
- Tshark (CLI version of Wireshark)
- Extcap (This and subordinate menu are optional) 
    - Etwdump (Windows Event Tracing)
    - Randpktdump (For fun, not necessary)
    - UDPdump (UDP protocol) 

Npcap install setup will be later 
USBcap might decrease performance of OS so not recommended 

UI setup 
Open Wifi for PC / Ethernet for Laptop for analysis 

Packet size limit 
General recommendation is do NOT try to open size above 500MB. 
Around 200MB size of packet analysis is recommended. 

Session TCP / HTTP Session 
Session == 연결 Connection 

However in Wireshark it is called as Stream 
1st discovered TCP session index is 0, not 1. 

Mouse Right Click > Colorize Conversation > TCP Session 
Then only can get that TCP color 

Mouse Right Click > Follow > TCP Stream 
Then assemble packet to make data stream combined 

컬러링 적용 취소 / Ctrl Space 등등 
Reset Colorization 

Display Filter TCP.steam  eq0 

Ctrl 1 Ctrl 2 Ctrl 3 시 Color 설정 가능 

Segment 번호로 Sequence Number 설정 

따라가기 Follow > TCP Stream 
HTTP 이면 조립된 내용이 아주 잘 보임 

파일 축출 
File / Export Objects / HTTP 
All files exported 
Filename search and preview 
Then packet is assembled and displaved. Even saving is possible 

TCP UDP HTTP FOLLOW 

### Statistics of Wireshark 

Packets Number and Displayed 
If it gets thousands it is not big, but if over millions, 
very slow and hard to analyze 

통계 / 종단점 
Statistics / Endpoints 

Endpoints are using subjects of networks 
