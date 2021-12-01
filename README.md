# KFiler
## *`File Transfering`* with `Modern C++` and `winsock2`
### For `windows` only
|CLASS|FUNCTION|DOES|
---|---|---
|FileTransferer|GetPendings|returns pending files|
||StartTransfer(pure virtual)|Not Implemented|
||StopTransfer(pure virtual)|Not Implemented
||GetMaxThreadCount|returns maximum threads will be used
||GetUsingThreadCount|returns current working threads
|FileSender<br/>(*Inherited from FileTransferer*)|FileSender|Takes a port , creates a network server <br/>and calls listen on it
||~FileSender|Clears all active servers <br> disconnects and destroy the server
### How To Build
***
>Use Visual Studio 2022 or latest <br/>
>Open solution file <br/>
>Modify `main.cpp` how U want to using available functions<br/>
>Build in **release** mode and **`Run`**
