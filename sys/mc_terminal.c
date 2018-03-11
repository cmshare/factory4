#include "mc_routine.h"
//---------------------------------------------------------------------------
HAND dtmr_termLinks=NULL,dtmr_commLinks=NULL;
static HAND uwb_session_locker=NULL;
//commDataLinksͨ���Թ���Լ����hTaskID�洢��ϢID�����ֶ�ֵ�����������ݽṹ���͡�
//---------------------------------------------------------------------------
//���ն�����״̬�仯��֪ͨ�󶨵��û��ֻ�
/*
void device_stateNotifyUser(TTerminal *devTerm,U32 devID){
  MYSQL_RES *res=db_queryf("select mc_users.session,mc_devices.sn,mc_devices.state from (mc_users inner join mc_devices on mc_devices.username=mc_users.username) where mc_devices.id=%u ",(devTerm)?devTerm->id:devID);
  if(res){
     MYSQL_ROW row=mysql_fetch_row(res);
     if(row){
        U32 binded_usersession=atoi(row[0]);
        if(binded_usersession){
          TTerminal * usrnode=(TTerminal *)dtmr_find(dtmr_termLinks,binded_usersession,0,0,0);
          if(usrnode){//if binded user is online
            TMcMsg *reqmsg=msg_alloc(MSG_SUR_NOTIFY_STATE,sizeof(TMSG_SUR_NOTIFY_STATE));
            TMSG_SUR_NOTIFY_STATE *reqbody=(TMSG_SUR_NOTIFY_STATE *)reqmsg->body;
            if(row[1])memcpy(reqbody->device_sn,row[1],SIZE_SN_DEVICE+1);
            reqbody->state_type=0;
            reqbody->state_value=(devTerm)?devTerm->term_state:atoi(row[2]);
            msg_request(reqmsg,usrnode,NULL,0);
          }
        }  
      }
      mysql_free_result(res);
  } 
}
*/
//---------------------------------------------------------------------------
void staticMap_generate(TTerminal *terminal){
  U32 end_time=time(NULL);
  U32 start_time=((TTermDevice *)terminal)->onlinetime;
  U32 duration=end_time-start_time;
   // if(strcmp(terminal->name,"CAM-019522FF18000098")==0) printf("##########[%s]EngineOff...start_time:%u,end_time:%u,duration=%0.1f*60s\r\n\r\n",terminal->name,start_time,end_time,(float)duration/60);
  if(duration>60 && duration<24*60*60){
    char xml[255];
    int xmlLen=sprintf(xml,"<xml><action>create</action><device>%s</device><starttime>%u</starttime><endtime>%u</endtime><session>%u</session></xml>",terminal->name,start_time,end_time,terminal->sessionid);
    int ret=hsk_httpPost("http://"WEB_SERVER_HOST"/service/routine/trackmap.php",xml,xmlLen,NULL,0,6);
    //printf("####POST xml:%s  ########ret=%d\r\n",xml,ret);
  }
}
//---------------------------------------------------------------------------
//�ն��豸���ֻ���������ʱ����
static void terminal_HbTimeout(HAND ttasks,void *taskCode,U32 *taskID,char *taskName){
  TTerminal * terminal=(TTerminal *)taskCode;
 // Log_AppendText("\r\n[HeatBeatTimeout:%s]",terminal->name);
  switch(terminal->term_type){
    case TT_DEVICE:{//device
             printf("dev#%d offline:\n",terminal->id);
             //�ͷ�session,���޸��ն�state(ȷ��ȫ��ϵͳ����).
             TNetAddr *peerAddr=&terminal->loginAddr;
             db_queryf("update ss_device set sessionid=0,logouttime=unix_timestamp(),state=%d where id=%u",DEV_STATE_OFFLINE,terminal->id);
             if(hsk_isTcpAddr(peerAddr)) hsk_shuttownTcpClient(peerAddr);
             peerAddr->socket=0;//mark disconnected
             terminal->term_state=DEV_STATE_OFFLINE;//mark offline����Ҫֱ���޸�sessionidΪ��
             //device_stateNotifyUser(terminal,0);//֪ͨ���ֻ��ն�����ͷ������
           //staticMap_generate(terminal);
         }
         break;
    case TT_USER: //user
           db_queryf("update `ss_user` set sessionid=0,logouttime=unix_timestamp() where id=%u",terminal->id);
           TNetAddr *peerAddr=&terminal->loginAddr;
           if(hsk_isTcpAddr(peerAddr)) hsk_shuttownTcpClient(peerAddr);
           peerAddr->socket=0;//mark disconnected
           printf("user offline:%s\n",terminal->name);
         break;
  }
  DBLog_AppendData("\xFF\xFF\xFF\xFF\x01",5,terminal); //��ʱ�ǳ���־
}
//---------------------------------------------------------------------------
void session_init(void){
  os_createSemphore(&uwb_session_locker, 1);/*�ź�����ֵΪ1����������*/
}

void session_lock(BOOL lock){
  if(lock)os_obtainSemphore(uwb_session_locker);
  else os_releaseSemphore(uwb_session_locker);
}

U32 session_new(void){
  while(1)
  { U32 rand_session=rand();
    if(rand_session)
    { if(!dtmr_findById(dtmr_termLinks,rand_session,0))return rand_session;
    }
  }
}

//---------------------------------------------------------------------------
void terminal_init(void)
{ MYSQL_RES *res;
  U32 dtmrOptions=DTMR_ENABLE|DTMR_TIMEOUT_DELETE|DTMR_OVERRIDE;
  U32 local_UdpSocket=hsk_getUdpSocket();
  dtmr_termLinks=dtmr_create(1024,HEARTBEAT_OVERTIME_MS,terminal_HbTimeout,"dtmr_termLinks");
  dtmr_commLinks=dtmr_create(64,60000,NULL,"dtmr_commLinks");
  res=db_query("select id,username,sessionid,ip,port,groupid,sex from `ss_user` where sessionid<>0");
  if(res)
  { MYSQL_ROW row;
    while((row = mysql_fetch_row(res)))
    { U32 sessionid=atoi(row[2]);//field["session"]
      TTerminal *node=(TTerminal *)dtmr_add(dtmr_termLinks,sessionid,0,0,NULL,sizeof(TTermUser),HEARTBEAT_OVERTIME_MS,&dtmrOptions);
      if(node)
      { memset(node,0,sizeof(TTermUser));
        node->term_type=TT_USER;
        node->sessionid=sessionid;
        node->id=atoi(row[0]);//field["id"]
        strncpy(node->name,row[1],MAXLEN_USERNAME+1);//field["username"]
        node->loginAddr.socket=local_UdpSocket;
        node->loginAddr.ip=atoi(row[3]);//field["ip"]
        node->loginAddr.port=atoi(row[4]);//field["port"]
        node->group=atoi(row[5]);//field["groupid"]
        node->sex_type=atoi(row[6]);//field["sex"]
      }
    }   
    mysql_free_result(res); 
  }

  res=db_query("select id,sessionid,ip,port,groupid,state,logintime from `ss_device` where sessionid<>0");
  if(res)
  { MYSQL_ROW row;
    while((row = mysql_fetch_row(res)))
    { U32 sessionid=atoi(row[2]);//field["session"]
      TTerminal *node=(TTerminal *)dtmr_add(dtmr_termLinks,sessionid,0,0,NULL,sizeof(TTermDevice),HEARTBEAT_OVERTIME_MS,&dtmrOptions);
      if(node)
      { node->term_type=TT_DEVICE;
        node->sessionid=sessionid; 
        node->loginAddr.socket=local_UdpSocket;
        node->id=atoi(row[0]);//field["id"]
        strncpy(node->name,row[1],SIZE_SN_DEVICE+1);//field["sn"]
        node->loginAddr.ip=atoi(row[3]);//field["ip"]
        node->loginAddr.port=atoi(row[4]);//field["port"]
        node->group=atoi(row[5]);//field["groupid"]
        ((TTermDevice *)node)->onlinetime=atoi(row[6]);
      }
    }   
    mysql_free_result(res); 
  }
}

