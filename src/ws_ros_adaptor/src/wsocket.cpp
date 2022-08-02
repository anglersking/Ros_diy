#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <libwebsockets.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <fstream>
#include <cstdio> //C++
#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KMAG "\033[0;35m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define RESET "\033[0m"
 

static int destroy_flag = 0;
 
static void INT_HANDLER(int signo) {
    destroy_flag = 0;
}


int dataToFile(const char* name,const char* data){
	//unsigned char* buffer= data;
 	int fd = open(name,O_RDWR|O_CREAT,0666);
 	write(fd,data,strlen(data));
 	std::cout << "data size: " << strlen(data) << std::endl;
 	close(fd);
 	return 0;
}

/* *
 * websocket_write_back: write the string data to the destination wsi.
 */
int websocket_write_back(struct lws *wsi_in, char *str, int str_size_in) 
{
    if (str == NULL || wsi_in == NULL)
        return -1;
 
    int n;
    int len;
    unsigned char *out = NULL;
 
    if (str_size_in < 1) 
        len = strlen(str);
    else
        len = str_size_in;
 
    out = (unsigned char *)malloc(sizeof(unsigned char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
    //* setup the buffer*/
    memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
    //* write out*/
    n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);
 
    printf(KBLU"[websocket_write_back] %s\n"RESET, str);
    //* free the buffer*/
    free(out);
 
    return n;
}
 
 std::string readFile(struct lws *wsi,const char* name){
	int fd = open(name,O_RDWR|O_CREAT,0666);
	//unsigned char buffer[124800];
    char *buffer=new char[124800];
    std::string res;
	int readbytes = read(fd,(void*)buffer,1024);
	if(readbytes > 0){
		std::cout << buffer << std::endl;
        //websoc发
        websocket_write_back(wsi ,buffer, -1);
        memset(buffer,'\0', strlen(buffer) ); 
        
        delete[]buffer; 


     
        std::fstream file("feedback.json", std::ios::out);
        //std::fstream file("feedback.json", std::ios::out);
        //dataToFile("feedback.json",(char *)"");
       // remove("feedback.json");
        //sleep(1);

        
    close(fd);
    res=(char*)buffer;
    return  res;
	}
	std::cout << "未读取到数据" << std::endl;
	
	close(fd);
	return "error";
}
struct per_session_data {
    int fd;
};
static int ws_service_callback(
                         struct lws *wsi,
                         enum lws_callback_reasons reason, void *user,
                         void *in, size_t len)
{
 
    switch (reason) {
 
        case LWS_CALLBACK_ESTABLISHED:
            printf(KYEL"[Main Service] Connection established\n"RESET);
            break;
 
        //* If receive a data from client*/
        case LWS_CALLBACK_RECEIVE:
        //websoc收
            printf(KCYN_L"[Main Service] Server recvived:%s\n"RESET,(char *)in);
            dataToFile("control.json",(char *)in);
           
            readFile(wsi,"feedback.json");
            //remove("feedback.json");
           // std::fstream file("feedback.json", std::ios::out);
            //websocket_write_back(wsi ,(char*)in, -1);
           // websocket_write_back(wsi ,"NULL", -1);
            //strcpy(ch,fback.c_str());
            //* echo back to client*/
            break;
    case LWS_CALLBACK_CLOSED:
            printf(KYEL"[Main Service] Client close.\n"RESET);
        break;
 
    default:
            break;
    }
 
    return 0;
}
 
 
int main(void) {
    // server url will usd port 5000
    int port = 5000;
    const char *interface = NULL;
    struct lws_context_creation_info info;
    struct lws_protocols protocol;
    struct lws_context *context;
    // Not using ssl
    const char *cert_path = NULL;
    const char *key_path = NULL;
    // no special options
    int opts = 0;
 
 
    //* register the signal SIGINT handler */
    struct sigaction act;
    act.sa_handler = INT_HANDLER;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction( SIGINT, &act, 0);
 
    //* setup websocket protocol */
    protocol.name = "my-echo-protocol";
    protocol.callback = ws_service_callback;
    protocol.per_session_data_size=sizeof(struct per_session_data);
    protocol.rx_buffer_size = 0;
 
    //* setup websocket context info*/
    memset(&info, 0, sizeof info);
    info.port = port;
    info.iface = interface;
    info.protocols = &protocol;
    info.extensions = NULL;//lws_get_internal_extensions();
    info.ssl_cert_filepath = cert_path;
    info.ssl_private_key_filepath = key_path;
    info.gid = -1;
    info.uid = -1;
    info.options = opts;
 
    //* create libwebsocket context. */
    context = lws_create_context(&info);
    if (context == NULL) {
        printf(KRED"[Main] Websocket context create error.\n"RESET);
        return -1;
    }
 
    printf(KGRN"[Main] Websocket context create success.\n"RESET);
 
    //* websocket service */
    while ( !destroy_flag ) {
        lws_service(context, 50);
    }
    usleep(10);
    lws_context_destroy(context);
 
    return 0;
}