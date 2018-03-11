#include "mc_routine.h" 
//---------------------------------------------------------------------------
static int svrUdpSocket=-1,svrTcpSocket=-1;
//---------------------------------------------------------------------------
static void SetSocketBuffer(int sockd,int recvBufsize,int sendBufsize)
{	if(recvBufsize)
	{ if(setsockopt(sockd,SOL_SOCKET,SO_RCVBUF,(char*)&recvBufsize,sizeof(int))==0)
		{ int len=sizeof(int),result;
	  	if(getsockopt(sockd,SOL_SOCKET, SO_RCVBUF, (char*)&result, (socklen_t *)&len)==0)
	  	{ result>>=1;//���õ�ֵ��ʵ�ʶ�����ֵ��������ϵ
	  		if(result!=recvBufsize)printf("SET SO_RCVBUF to %d,but limited to %d,pelease check config[/proc/sys/net/core/rmem_max]\r\n",recvBufsize,result);
	  	}
		}else printf("SET SO_RCVBUF failed\r\n");
  }	
  if(sendBufsize)
	{	if(setsockopt(sockd,SOL_SOCKET,SO_SNDBUF,(char*)&sendBufsize,sizeof(int))==0)
		{ int len=sizeof(int),result;
	  	if(getsockopt(sockd,SOL_SOCKET, SO_SNDBUF, (char*)&result, (socklen_t *)&len)==0)
	  	{ result>>=1;
	  		if(result!=sendBufsize)printf("SET SO_SNDBUF to %d,but limited to %d,pelease check config[/proc/sys/net/core/wmem_max]\r\n",sendBufsize,result);
	  	}	
		}
  }else printf("SET SO_SNDBUF failed\r\n");	
}
//---------------------------------------------------------------------------
static BOOL packet_checksum_and_decrypt(TMcPacket *packet){
  TMcMsg *msg=&packet->msg;
  int msgLen=MC_MSG_SIZE(msg);
  if(msg_ValidChecksum(msg,msgLen)){
    TTerminal *terminal=(msg->sessionid)?(TTerminal *)dtmr_findById(dtmr_termLinks,msg->sessionid,FALSE):NULL;
    if(terminal){
      //�����sessionid��IP��ַ�ǰ󶨵Ĺ�ϵ���������Ӧ��ʾsessionid�Ѿ�ʧЧ��
      //TCP��UDP�в�ͬ�Ķ˿ڣ���������ֻ��֤UDP�˿ڵİ󶨹�ϵ��
      if(packet->peerAddr.ip!=terminal->loginAddr.ip){
        terminal=NULL; //����豸��δ��¼�����ߵ�¼��ʱ��
      }
      else if(packet->peerAddr.port!=terminal->loginAddr.port){
         if((packet->peerAddr.socket==svrUdpSocket)==(terminal->loginAddr.socket==svrUdpSocket)) terminal=NULL;
      }
    }
    packet->terminal=terminal;
    if(msg->bodylen>0 && msg->encrypt>ENCRYPTION_NONE) msg_decrypt(msg);
    return TRUE;		
  }
  return FALSE;
}
//---------------------------------------------------------------------------
static BOOL msg_checksum_and_decrypt(TMcMsg *msg){
  int msgLen=MC_MSG_SIZE(msg);
  if(msg_ValidChecksum(msg,msgLen)){
    if(msg->bodylen>0 && msg->encrypt>ENCRYPTION_NONE) msg_decrypt(msg);
    return TRUE;		
  }
  return FALSE;
}
//---------------------------------------------------------------------------
#if 0 
static TMcPacket *NET_RecvPacket(void *dgramBuffer,int bufferSize){
   TMcPacket *packet=(TMcPacket *)dgramBuffer;
   TMcMsg *msg=&packet->msg;
   int msgsize,sliceLen=hsk_readData(msg,bufferSize-sizeof(TMcPacket),&packet->peerAddr);

  // printf("recv %d bytes\n",sliceLen);
   #ifdef DEBUG_MODE
    Log_AppendData(msg,sliceLen,&packet->peerAddr,FALSE);
   #endif

   if(sliceLen>sizeof(TMcMsg)){
      msgsize=MC_MSG_SIZE(msg);
      if(sliceLen>=msgsize && packet_checksum_and_decrypt(packet))return packet; 
      if(msgsize>MAXLEN_MSG_PACKET)msgsize=0;
   }
   else{
     if(!sliceLen)return NULL;
     msgsize=0;
   }
 	 
   if(packet->peerAddr.socket!=svrUdpSocket){ //���TCP�������ݽ�����װ 	
     packet=(TMcPacket *)hsk_assemble(&packet->peerAddr,msg,sliceLen,msgsize);
     if(packet){
        if(packet_checksum_and_decrypt(packet))return packet;
        else hsk_releaseTcpPacket((THskPacket *)packet,TRUE,FALSE); 
     }
   }
   return NULL;  	
}

static void *mc_dispatch_proc(void *param){ //û�п���ճ������
  extern void mc_dispatchmsg(TMcPacket *);
  const int bufferSize=sizeof(TMcPacket)+MAXLEN_IP_FRAGMENT;
  const void *pbuf=malloc(bufferSize);
  while(1)//process user request packet  
  { TMcPacket *packet=NET_RecvPacket(pbuf,bufferSize);
    if(packet)
    {  if(packet->terminal)
    	{ DBLog_AppendMsg(&packet->msg,packet->terminal,FALSE);
    	}
        mc_dispatchmsg(packet);
  	//((void (*)(TMcPacket *))param)(packet);
        if(packet->peerAddr.socket!=svrUdpSocket){
    	   hsk_releaseTcpPacket((THskPacket *)packet,(packet!=pbuf),packet->msg.tcpshortconn);
        }
    }	
  }
  return NULL;
}
#else
//---------------------------------------------------------------------------
static void *mc_dispatch_proc(void *param){
  extern void mc_dispatchmsg(TMcPacket *);
  const int bufferSize=sizeof(TMcPacket)+MAXLEN_IP_FRAGMENT;
  TMcPacket *headPacket=(TMcPacket *)malloc(bufferSize);
  TMcPacket *headPacketLimit=(TMcPacket *)((char *)headPacket+bufferSize);
  while(1){//process user request packet  
    TMcPacket *handledPacket=NULL;
    TMcMsg *msg=&headPacket->msg;
    int msgsize,sliceLen=hsk_readData(msg,bufferSize-sizeof(TMcPacket),&headPacket->peerAddr);
    #ifdef DEBUG_MODE
      Log_AppendData(msg,sliceLen,&headPacket->peerAddr,FALSE);
    #endif
    if(sliceLen>sizeof(TMcMsg)){
      msgsize=MC_MSG_SIZE(msg);
      if(sliceLen>=msgsize && (U32)msgsize<MAXLEN_MSG_PACKET && packet_checksum_and_decrypt(headPacket)){
        mc_dispatchmsg(handledPacket=headPacket);
        sliceLen-=msgsize;
        while(sliceLen>0){//����ճ��
           msg=(TMcMsg *)((char *)msg+msgsize);
           if(sliceLen>sizeof(TMcMsg)){
             msgsize=MC_MSG_SIZE(msg);
             if(sliceLen>=msgsize && (U32)msgsize<MAXLEN_MSG_PACKET && msg_checksum_and_decrypt(msg)){
               TMcPacket *packet=T_PARENT_NODE(TMcPacket,msg,msg);               
               packet->terminal=headPacket->terminal;
               packet->peerAddr=headPacket->peerAddr;  
               mc_dispatchmsg(handledPacket=packet);
               sliceLen-=msgsize;
             }
             else break;
           }
           else msgsize=0;
        }
      }
    }
    else msgsize=0;
 	 
    if(sliceLen>0 && headPacket->peerAddr.socket!=svrUdpSocket && (U32)msgsize<MAXLEN_MSG_PACKET){ //���TCP�������ݽ�����װ 	
      TMcPacket *packet=(TMcPacket *)hsk_assemble(&headPacket->peerAddr,msg,sliceLen,msgsize);
      if(packet){
        if(packet_checksum_and_decrypt(packet)){
          mc_dispatchmsg(handledPacket=packet);
        }
        else hsk_releaseTcpPacket((THskPacket *)packet,TRUE,FALSE); 
      }
    }
    if(handledPacket && handledPacket->peerAddr.socket!=svrUdpSocket){
      hsk_releaseTcpPacket((THskPacket *)handledPacket,handledPacket>headPacketLimit||handledPacket<headPacket,handledPacket->msg.tcpshortconn);
    }	
  }
  return NULL;
}
#endif
//---------------------------------------------------------------------------
void Handle_MSG_TSR_HEARTBEAT(TMcPacket *packet){
  dtmr_reload(packet->terminal);
  msg_ack_general(packet,0);
}
//---------------------------------------------------------------------------
void mc_schedule(void){
  int i;
  pthread_t _thread=0;
  for(i=0;i<NUM_DISPATCH_THREAD;i++){
    pthread_create(&_thread, NULL,mc_dispatch_proc,NULL);
    //printf("Create dispatch-thread-%02d...%s!\r\n",i+1,(_thread)?"OK":"failed");	
  }
  puts("Smoke sensor daemon service Ver "MEOW_SERVICE_VERSION" @"WEB_SERVER_HOST",running...");
}
//---------------------------------------------------------------------------
void net_init(void){
   if(hsk_init(SERVICE_PORT_TCP,SERVICE_PORT_UDP,MAXLEN_UDP_DATAGRAM,SIZE_NET_RECV_BUFFER)){
       svrUdpSocket=hsk_getUdpSocket();
       svrTcpSocket=hsk_getTcpSocket();
       SetSocketBuffer(svrUdpSocket,MAX_SOCKET_RECV_MEM,MAX_SOCKET_SEND_MEM);
       SetSocketBuffer(svrTcpSocket,MAX_SOCKET_RECV_MEM,MAX_SOCKET_SEND_MEM);
   }
   else{
      Log_AppendText("Net init error!");
      exit(0);//should not be here
   }		
}
//-----------------------------------------------------------------------------

