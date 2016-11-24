## Little Instruction

***This is the <u>simplest</u> socket program for green-hand learning***

**No multithread    No synchronize    No multi-client**

Code fetched from [https://msdn.microsoft.com](https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx) and reformed to fit in concrete project

### function

```c++
void InitWinSock(); //Initialize Win Socket
void ResolveAddrPort(); //Resolve server's address or port
void ConnectToServer(); //Connect to server
void Prepare(); //including functions up here

void Send(std::string strSend); //Send content you want and it MUST be of type std::string
void Shutdown(); //Shutdown connection after send
void SendAndShutdown(std::string strSend); //including functions up here

void Receive(); //Receive information from server
void Cleanup(); //Free resources
void ReceiveAndCleanup(); //including functions up here

std::string GetRecvStr(); //transfer content of type c_str(char*) to what of type std::string
```

### How to use it?

*step1* `new` a `SocketClient` object and it possesses all variables and functions

*step2*  call for `Prepare`function and it will finish preparation so that you don't need to call every single function on your own

*step3* Call for `SendAndShutdown`function to send content you want and shutdown connection

*step4* Receive content from server and free resources

