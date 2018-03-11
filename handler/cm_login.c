#include "mc_routine.h"
//-------------------------------------------------------------------------------------
void Handle_MSG_DSR_LOGIN(TMcPacket *packet){
  TTerminal *terminal=NULL;
  U32  sessionid=0,loginTime,deviceGroup,deviceBox;
  TMSG_DSR_LOGIN *content=(TMSG_DSR_LOGIN *)packet->msg.body;
  U32  deviceID=content->devID;
  U8 error_code=1,deviceExists=0;
  MYSQL_RES *res;
  session_lock(TRUE);
  res=db_queryf("select sessionid,groupid,logintime from `ss_device` where id=%u",deviceID); 
  if(res){
    MYSQL_ROW row=mysql_fetch_row(res);
    if(row){  
      sessionid=atoi(row[0]);
      deviceGroup=atoi(row[1]);
      loginTime=atoi(row[2]);
      deviceExists=TRUE;
      if(sessionid){
        terminal=(TTerminal *)dtmr_findById(dtmr_termLinks,sessionid,TRUE); 
        if(terminal && (terminal->id!=deviceID ||terminal->term_type!=TT_DEVICE)){
          //上一次使用的session已经被其他设备占用（所查到的terminal是其他设备）。
          sessionid=0;
          dtmr_unlock(terminal,0); 
          terminal=NULL;
        }
      } 
    }
    mysql_free_result(res);   
  }
  if(deviceExists){
    if(!sessionid)sessionid=session_new(); 
    if(!terminal){
      U32 dtmrOptions=DTMR_LOCK|DTMR_ENABLE|DTMR_TIMEOUT_STOP|DTMR_NOVERRIDE;
      terminal=(TTerminal *)dtmr_add(dtmr_termLinks,sessionid,0,0,NULL,sizeof(TTermDevice),HEARTBEAT_OVERTIME_MS,&dtmrOptions);
      memset(terminal,0,sizeof(TTermDevice));
      loginTime=time(NULL);
    }
    terminal->term_type=TT_DEVICE;
    terminal->id=deviceID;
    terminal->loginAddr=packet->peerAddr;
    terminal->sessionid=sessionid;
    terminal->group=deviceGroup;
    terminal->encrypt=packet->msg.encrypt;//消息体默认加密方式
    error_code=0;
    db_queryf("update ss_device set sessionid=%u,ip=%u,port=%u,logintime=%u where id=%u",sessionid,packet->peerAddr.ip,packet->peerAddr.port,loginTime,deviceID);
  }
  
  packet->terminal=terminal;
  if(terminal) dtmr_unlock(terminal,HEARTBEAT_OVERTIME_MS);

  TMcMsg *msg=msg_alloc(MSG_SDA_LOGIN,sizeof(TMSG_SDA_LOGIN));
  TMSG_SDA_LOGIN *ackBody=(TMSG_SDA_LOGIN *)msg->body;
  ackBody->sessionid=(error_code==0)?sessionid:0;
  msg_send(msg,packet,NULL);
printf("login ret=%d session=%x\n",error_code,sessionid);

  session_lock(FALSE);
  //if(error_code==0) DBLog_AppendData("\xFF\xFF\xFF\xFF\x00",5,terminal); //登录日志
}
 

//-------------------------------------------------------------------------------------
void terminal_kickoff(TNetAddr *peer,U8 errcode){
  TMcMsg *msg=msg_alloc(MSG_STR_KICKOFF,sizeof(U8));
  *((U8 *)msg->body)=errcode;
  msg_sendto(msg,peer);
}
//-------------------------------------------------------------------------------------
void Handle_MSG_USR_LOGIN(TMcPacket *packet){
  TTerminal *terminal=NULL;
  TMSG_USR_LOGIN *content=(TMSG_USR_LOGIN *)packet->msg.body;
  char *username=content->name;
  char *passwd=content->passwd;
  U32 userid=0,sessionid=0,userGroup;
  U8 sex_type,error_code=0;
  char pwd_pattern[SIZE_MD5+3]="\0";
  session_lock(TRUE);
  if(passwd[0]!='\0' && Password_check(passwd)){
    int pwdlen=strlen(passwd); 
    if(pwdlen==SIZE_MD5) sprintf(pwd_pattern,"'%s'",passwd);
    else if(pwdlen<=MAXLEN_PASSWORD)sprintf(pwd_pattern,"md5('%s')",passwd);
  }
  if(db_checkSQL(username) && pwd_pattern[0]!='\0'){ 
     MYSQL_RES *res=db_queryf("select id,sessionid,groupid,sex from `ss_user` where username='%s' and password=%s",username,pwd_pattern);
     if(res){ 
        MYSQL_ROW row=mysql_fetch_row(res);
        if(row){
          userid=atoi(row[0]);
          sessionid=atoi(row[1]);
          userGroup=atoi(row[2]);
          sex_type=atoi(row[3]);
          if(sessionid){
            terminal=(TTerminal *)dtmr_findById(dtmr_termLinks,sessionid,TRUE);
            if(terminal){
              if(terminal->id!=userid || terminal->term_type!=TT_USER){
                //上一次使用的session已经被其他用户占用（所查到的terminal是其他用户）。
                sessionid=0;
                dtmr_unlock(terminal,0); 
                terminal=NULL;
              }
              else{
                TNetAddr *loginAddr=&terminal->loginAddr,*peerAddr=&packet->peerAddr;
                if(memcmp(loginAddr,peerAddr,sizeof(TNetAddr))!=0){
                //同一用户名多处登录的状况
                //将原先登录的用户踢下线
                if(loginAddr->ip!=peerAddr->ip || (hsk_isTcpAddr(peerAddr) && hsk_isTcpAddr(loginAddr) && loginAddr->socket!=peerAddr->socket)|| (!hsk_isTcpAddr(peerAddr) && !hsk_isTcpAddr(loginAddr) && loginAddr->port!=peerAddr->port)){
                  //TMcMsg *reqmsg=msg_alloc(MSG_SUR_KICKOFF,0);
                 // msg_request(reqmsg,terminal,NULL,0);
                  terminal_kickoff(loginAddr,-4);//已经在其它地方登录
                  //安全删除原先登录的用户节点
                  dtmr_unlock(terminal,DTMR_UNLOCK_DELETE);//删除后的节点无法被查找，但会保留足够长一段时间
                  terminal=NULL;
                  printf("#####################User kick off\n");
                  //sessionid=0; //sessionID可继续使用
                }
                else{
                  //对于TCP连接，socket已经被新连接覆盖的情况,无法通知原先登录的用户
                  *loginAddr=*peerAddr;//更新登录地址
                }
              }
            }
          }
        }
      }
      mysql_free_result(res);  
    }
  }
  if(!userid) error_code=1;//用户名或密码错误。
  else {
    if(!sessionid)sessionid=session_new(); 
    while(!terminal){
      U32 dtmrOptions=DTMR_LOCK|DTMR_ENABLE|DTMR_TIMEOUT_DELETE|DTMR_NOVERRIDE;
      terminal=(TTerminal *)dtmr_add(dtmr_termLinks,sessionid,0,0,NULL,sizeof(TTermUser),HEARTBEAT_OVERTIME_MS,&dtmrOptions);
      if(dtmrOptions&DTMR_EXIST){
        dtmr_unlock(terminal,0);
        terminal=NULL;
        sessionid=session_new(); 
      }
      else{
        memset(terminal,0,sizeof(TTermUser));
        terminal->term_type=TT_USER;
        terminal->id=userid;
        terminal->loginAddr=packet->peerAddr;
        terminal->sessionid=sessionid;
        terminal->group=userGroup;
        terminal->sex_type=sex_type;
        terminal->encrypt=packet->msg.encrypt;//消息体默认加密方式
        strncpy(terminal->name,username,MAXLEN_USERNAME+1);
      }
    }
    db_queryf("update `ss_user` set sessionid=%u,ip=%u,port=%u,logintime=unix_timestamp() where id=%u",sessionid,packet->peerAddr.ip,packet->peerAddr.port,userid);
  }
  packet->terminal=terminal;
  if(terminal) dtmr_unlock(terminal,HEARTBEAT_OVERTIME_MS);

  TMcMsg *msg=msg_alloc(packet->msg.msgid|MSG_ACK_GENERAL,sizeof(TMSG_SUA_LOGIN));
  TMSG_SUA_LOGIN *ackBody=(TMSG_SUA_LOGIN *)msg->body;
  //ackBody->error=error_code;
  ackBody->sessionid=(error_code==0)?sessionid:0;
  msg_send(msg,packet,NULL);

  session_lock(FALSE);
  if(error_code==0)DBLog_AppendData("\xFF\xFF\xFF\xFF\x00",5,terminal); //登录日志
}


void Handle_MSG_USR_LOGOUT(TMcPacket *packet){
  db_queryf("update `ss_user` set sessionid=0,logouttime=unix_timestamp() where id=%u",packet->terminal->id);
  dtmr_delete(packet->terminal);
  msg_ack_general(packet,0);
}




