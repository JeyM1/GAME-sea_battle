#include "seabattle.h"

extern SOCKET sHandleServer;
extern SOCKET sHandleClient;
extern SOCKET newSHandle;
extern int SERVER_PORT_NUM;

//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: getSocketError()
 Synopsis: Function for getting last error from Windows.
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void getSocketError() {
   LPVOID lpMsgBuf;
   if (!FormatMessage(
       FORMAT_MESSAGE_ALLOCATE_BUFFER |
       FORMAT_MESSAGE_FROM_SYSTEM |
       FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       GetLastError(),
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       (LPTSTR) &lpMsgBuf,
       0,
       NULL )){
      MessageBox(NULL, "Error number not found",
                 "Error", MB_OK | MB_ICONSTOP );
      return;
   }
   // Display the string.
   MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error",
               MB_OK | MB_ICONSTOP);
   // Free the buffer.
   LocalFree(lpMsgBuf);
}
//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: init_server_socket()
 Synopsis: Function for initializing server-side socket.
 Returns:  1 if no errors, else 0
 -----------------------------------------------------------------------[>]-*/
int init_server_socket(){
    SOCKADDR_IN serverAddr;       // server's socket address
    SOCKADDR_IN clientAddr;       // client's socket address
    int sockAddrSize;             // size of socket address structure
    //SOCKET sHandleServer;          // socket file descriptor
    //SOCKET newSHandle;            // socket descriptor from accept
    WSADATA wsaData;              //data structure that is to receive details
                                  //of the Windows Sockets implementation
    // set up the local address
    sockAddrSize = sizeof(SOCKADDR_IN);
    memset(&serverAddr, 0, sockAddrSize);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // initiate use of WS2_32.DLL by a process
    if(WSAStartup(MAKEWORD(1, 1), &wsaData)) {
        getSocketError();
        getch();
        return 0;
    }
    // create a TCP-based socket
    if((sHandleServer = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        getSocketError();
        getch();
        return 0;
    }
    // bind socket to local address
    if(bind(sHandleServer, (SOCKADDR *)&serverAddr, sockAddrSize)  == -1 ){
        getSocketError();
        closesocket(sHandleServer);
        getch();
        return 0;
    }
    // create queue for client connection requests
    if(listen(sHandleServer, SERVER_MAX_CONNECTIONS) == -1) {
        getSocketError();
        closesocket(sHandleServer);
        getch();
        return 0;
    }
    system("cls");
    printf("Waiting for player to join on port %d...\n", SERVER_PORT_NUM);
    if((newSHandle = accept(sHandleServer, (SOCKADDR *)&clientAddr, &sockAddrSize)) == INVALID_SOCKET){
        getSocketError();
        closesocket(sHandleServer);
        getch();
        return 0;
    }
    return 1;
}

//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: init_client_socket()
 Synopsis: Function for initializing client-side socket.
 Returns:  1 if no errors, else 0
 -----------------------------------------------------------------------[>]-*/
int init_client_socket(char *serverName){
    SOCKADDR_IN serverAddr;      //server's socket address
    int sockAddrSize;            //size of socket address structure
    //SOCKET sHandle;              //socket descriptor
    WSADATA wsaData;             //data structure that is to receive
                                 //details of the Windows Sockets
                                 //implementation
    //initiate use of WS2_32.DLL by a process
    if(WSAStartup(MAKEWORD(1, 1), &wsaData)) {
        getSocketError();
        getch();
        return 0;
    }
    //create client's socket
    if((sHandleClient = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        getSocketError();
        getch();
        return 0;
    }
    // build server socket address
    sockAddrSize = sizeof(SOCKADDR_IN);
    memset(&serverAddr, 0, sockAddrSize);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    if((serverAddr.sin_addr.s_addr = inet_addr(serverName)) == INADDR_NONE){
        perror("Unknown server name");
        closesocket(sHandleClient);
        getch();
        return 0;
    }

    //connect to server
    if(connect(sHandleClient, (SOCKADDR *)&serverAddr, sockAddrSize) == -1){
        getSocketError();
        closesocket(sHandleClient);
        getch();
        return 0;
    }
    return 1;
}
