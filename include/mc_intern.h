//---------------------------------------------------------------------------
#ifndef _MC_INTERN_H
#define _MC_INTERN_H
//---------------------------------------------------------------------------
enum{TT_USER=1,TT_DEVICE=2,TT_BOX=3};
enum{UDP=0,TCP=1};
//enum{SESSION_NULL=0,SESSION_OFFLINE=1,SESSION_RESEARVED1,SESSION_RESEARVED2,MIN_SESSIONID=10};
//����û���sessionidΪ0��ʾ���û��Ѿ�ɾ��/�����ڣ�Ϊ1��ʾ����;���ڵ���MIN_SESSIONID��ʾ���ߡ�
enum{ENCRYPTION_NONE=0,ENCRYPTION_AES128=1,ENCRYPTION_RAS1024=2};
enum{DEV_STATE_OFFLINE=0,DEV_STATE_ONLINE};
//---------------------------------------------------------------------------
#define SERVER_DYNAMIC_SESSION(msg)      ~((msg)->msgid+(msg)->synid)
#define MC_MSG_SIZE(msg)                  (sizeof(TMcMsg)+(msg)->bodylen+1)
#define MC_PACKET_SIZE(packet)            (sizeof(TMcPacket)+(packet)->msg.bodylen+1)
//#define RESPONSE_APPENDIX(packet)        *((void **)&packet->msg)
//---------------------------------------------------------------------------
#pragma pack (push,1)
//---------------------------------------------------------------------------
typedef struct{
  U32  id,sessionid;
  TNetAddr loginAddr;
  //����λ��ṹ���붨����޷������ͣ�����һλ��1�ᱻ���������ͳ�-1�� 
  U32 group:8;//���豸����	
  U32 sex_type:2;//0:����;1:��;2:Ů
  U32 term_type:2;//0��ʾdevice, 1��ʾuser
  U32 term_state:4; //for terminnal 0:����;1:����;2����;3:����
  U32 encrypt:2;//�ն���Ϣ��Ĭ�ϼ��ܷ�ʽ
  U32 msg_push_acceptable:1; //�Ƿ���ܷ�������Ϣ֪ͨ����
  char name[0];
}TTerminal;

typedef struct{
  TTerminal terminal;
  char username[(MAXLEN_USERNAME|0x3)+1];//��Terminal��name��ַ��Ӧ�������ֶ��룬
}TTermUser;

typedef struct
{ TTerminal terminal;
  char sn[(SIZE_SN_DEVICE|0x3)+1];//��Terminal��name��ַ��Ӧ�������ֶ��룬
  U32 onlinetime;//���ߵ�ʱ��
}TTermDevice;

typedef struct
{ U32 msgid;
  U32 sessionid; //�ỰID����¼���������Ϊ�����һ��Ψһ��session��ID��
  U32 synid:29;     //��ˮ�ţ�������˳��� 0 ��ʼѭ���ۼ�
  U32 encrypt:2;   //��Ϣ����ܷ�ʽ(0�������� 1��AES)
  U32 tcpshortconn:1;//�Ƿ������tcpӦ���ر�tcp����
  U32 bodylen;    //��Ϣ�峤��
  U8  body[0];   //��Ϣ������
}TMcMsg;

typedef struct
{ TTerminal *terminal;
  TNetAddr peerAddr;
  TMcMsg msg;
}TMcPacket;

typedef struct
{ U32 /*ack_msg,*/retry_counter;
  void *extraData;
  TMcPacket reqPacket;
}TSuspendRequest;

/*
typedef struct _TUserList{
 struct _TUserList *prev,*next;
 TTerminal *terminal;
}TUserList;

typedef struct
{ U32 ack_msg,retry_counter;
	TTerminal *terminal;
	TMcMsg    *reqMsg; //TMcPacket *srcPacket;
	void      *extraData;
}TSuspendRequest;
*/

//---------------------------------------------------------------------------
#pragma pack (pop)  
//---------------------------------------------------------------------------

#endif
