//---------------------------------------------------------------------------
#ifndef _CM_CONFIG_H
#define _CM_CONFIG_H
//---------------------------------------------------------------------------
#if 0            
  #define WEB_SERVER_HOST          "www.cmsoft.cn"
  #define ENABLE_FLOW_MONITOR       0
#else
  #define WEB_SERVER_HOST          "dev.tellfun.com"
  #define ENABLE_FLOW_MONITOR       0
  #define DEBUG_MODE
#endif
#define MEOW_SERVICE_VERSION        "1.0"
//---------------------------------------------------------------------------
#define SERVICE_PORT_TCP            8082  //TCP
#define SERVICE_PORT_UDP            8899  //UDP 
//---------------------------------------------------------------------------
#define ENABLE_SMS_MODULE           0
#define WEB_SNAPSHOT_ROOT           "/uploadfiles/share/"
//---------------------------------------------------------------------------
#define DATABASE_HOST               "localhost"
#define DATABASE_NAME               "smoke_sensor"
#define DATABASE_USER               "smokesvr" //"meow"
#define DATABASE_PASS               "smoke123456" //"miaomiao"
//---------------------------------------------------------------------------
#define SMS_SERVER                  "http://114.55.25.138/msg/HttpBatchSendSM"
#define SMS_ACCOUNT                 "mxkj2016"
#define SMS_PASSWORD                "mPlanet@qJt_2016"//"Njmxkj2016@"//
//---------------------------------------------------------------------------
#define NUM_UWB_PROCESS_THREAD      16 
//---------------------------------------------------------------------------
#define SIZE_NET_RECV_BUFFER        1024*1024*2
#define SIZE_UWB_RECV_BUFFER        1024*1024*4
#define MAXLEN_UDP_DATAGRAM         256     //UDP消息包允许最大长度(不小于1500)
#define MAXLEN_IP_FRAGMENT          1500    //IP最大分包长度(不小于1500,不要随意调整)
#define MAXLEN_MSG_PACKET           102400  //业务消息包内容允许最大长度
#define MAX_SOCKET_RECV_MEM         124928  //SOCKET接收缓冲区最大值(取/proc/sys/net/core/rmem_max)
#define MAX_SOCKET_SEND_MEM         124928  //SOCKET发送缓冲区最大值(取/proc/sys/net/core/wmem_max)
#define SERVER_BAND_WIDTH           5       //服务器流出带宽(单位Mbit)，据实填写。
#define NUM_DISPATCH_THREAD         8 
#define PacketBufferMaxSize         80
#define REQUEST_RETRY_COUNT         3 
#define REQMSG_RETRY_INTERVAL_MS    3000///3000
#define MIN_SMS_INTERVAL_MS         50000
#define HEARTBEAT_OVERTIME_MS       120000    //3次以上无心跳视为超时
#define VERIFYCODE_LIFETIME_MS      300000
#define SPAN_MSG_PUSH_FILTER_MS     10000
#define SIZE_SN_DEVICE              20
#define SIZE_MD5                    32
#define SIZE_PHONE_MOBILE           11
#define SIZE_VERIFY_CODE            6
#define MAX_BINDED_NUM              16     //一个用户最多绑定的lab数
#define MAXLEN_PASSWORD             12
#define MAXLEN_USERNAME             20
#define MAXLEN_NICKNAME             40
#define MAXLEN_PACKAGENAME          80
#define MAXLEN_URL                  80 
#define SERVER_DEFAULT_SESSION      0  
//---------------------------------------------------------------------------
#endif
