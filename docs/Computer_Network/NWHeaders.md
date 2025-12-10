# TCP / IP Headers  

### IP Version 4 Header 

```shell
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------+-------+---------------+-------------------------------+
|       |Header |   Type of     |                               |
|Version|Length |   Service     |         Total Length          |
+-------+-------+---------------+-----+-------------------------+
|        Identification         |Flags|      Fragment Offset    | 
+-------------------------------+-----+-------------------------+
| Time To Live  |    Protocol   |       Header Checksum         | 
+---------------+---------------+-------------------------------+
|                          Source IP Address                    |
+---------------------------------------------------------------+
|                     Destination IP Address                    |
+---------------------------------------------------------------+
|                      Options (0 ~ 40 Bytes)                   |
+---------------------------------------------------------------+
```
- `Version` IP Version 4 or IP Version 6 
- `Time To Live` TTL to avoid packet voyage forever in the internet. 

### TCP Header  

```shell 
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------------------------------+-------------------------------+
|          Source Port          |      Destination Port         |
+-------------------------------+-------------------------------+
|                          Sequence Number                      |
+---------------------------------------------------------------+
|                   Acknowledgement Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Header |  Reserved |U|A|P|R|S|F|        Window Size            |
|       |           |R|C|S|S|Y|I|                               |
|Length |  Bits     |G|K|H|T|N|N|    Advertisement Window       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Check Sum           |      Urgent Pointer           | 
+-------------------------------+-------------------------------+
|                      Options (0 ~ 40 Bytes)                   |
+---------------------------------------------------------------+
```

- `Sequence Number` This to Other bytestream number 
- `Acknowledge Number` Other to This bytestream number 
- `Windows Size` (RWND) Other's Kernel level Recv Buffer left space  
- `Check Sum` Whether data is damaged or not.  

Header Length 
Reserved Bits 
Control Bits 

### UDP Header 

```shell
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------------------------------+-------------------------------+
|          Source Port          |      Destination Port         |
+-------------------------------+-------------------------------+
|          UDP  Length          |      UDP  Check Sum           | 
+-------------------------------+-------------------------------+
```

