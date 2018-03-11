#include "mc_routine.h" 
//---------------------------------------------------------------------------
#pragma pack (push,1)
//---------------------------------------------------------------------------
typedef struct{
   U32   devID; //设备ID 
   U8    simID[5];
   U8    alarmState;//报警状态
   U8    batteryState;//电量1Byte
}TMSG_DSR_UPDATE_STATE;
//---------------------------------------------------------------------------
#pragma pack (pop) 
//-------------------------------------------------------------------------------------
void Handle_MSG_DSR_UPDATE_STATE(TMcPacket *packet){
  TMSG_DSR_UPDATE_STATE *content=(TMSG_DSR_UPDATE_STATE *)packet->msg.body;
  puts("######Handle_MSG_DSR_UPDATE_STATE");
  msg_ack_general(packet,0);
}
 

