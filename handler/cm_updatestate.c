#include "mc_routine.h" 
//---------------------------------------------------------------------------
#pragma pack (push,1)
//---------------------------------------------------------------------------
#define MAXLEN_SIMID     5
typedef struct{
   U8    alarmState;//报警状态
   U8    batteryState;//电量1Byte
}TMSG_DSR_UPDATE_STATE;
//---------------------------------------------------------------------------
#pragma pack (pop) 
//-------------------------------------------------------------------------------------
void Handle_MSG_DSR_UPDATE_STATE(TMcPacket *packet){
  TMSG_DSR_UPDATE_STATE *content=(TMSG_DSR_UPDATE_STATE *)packet->msg.body;
  TTerminal *terminal=packet->terminal;
 /* char sn[16];

  puts("######Handle_MSG_DSR_UPDATE_STATE");
  str_bytesToHex(content->simID,MAXLEN_SIMID,sn,16,0);
  if(strcmp(sn,terminal->name)!=0){
    strcpy(terminal->name,sn);
    db_queryf("update `ss_device` set sn='%s' where id=%u",sn,terminal->id);
    puts("########### device sn changed");
  }
*/
  msg_ack_general(packet,0);
  db_queryf("update ss_device set alarming=%d,battery=%d,updatetime=unix_timestamp() where id=%u",content->alarmState,content->batteryState,terminal->id);
  db_queryf("insert into ss_devlog set devid=%u,alarming=%d,battery=%d,addtime=unix_timestamp()",terminal->id,content->alarmState,content->batteryState);

}
 

