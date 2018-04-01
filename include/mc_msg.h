//---------------------------------------------------------------------------
#ifndef _MC_MSGID_H
#define _MC_MSGID_H
//--------------------------------------------------------------------------- 
//����˵��
//U:�û����ֻ�APP�û���
//V:�û�����ָAPP�ο��û���
//D:�豸������ͷ�豸��
//T:�նˣ������豸�ն�DT���û��ն�UT
//R:request; A: Acknowledge
//DSR��ʾRequest of Device to Server
//UDA��ʾAcknowledge of User to Device

#define MSG_ACK_MASK                0x80000000    //������ͨ��Ӧ��
#define MSG_ACK_GENERAL             MSG_ACK_MASK     //������ͨ��Ӧ��

#define MSG_TSR_HEARTBEAT           0x00000001    //�ֻ����ն��������󣬷��ط�����ͨ��Ӧ��

//#define MSG_SUR_NOTIFY_MSGBOX     0x00000001    //�������¼�֪ͨ����
//#define MSG_USA_NOTIFY_MSGBOX     (MSG_SUR_NOTIFY_MSGBOX|MSG_ACK_GENERAL) 

#define MSG_DSR_LOGIN                0x00000002    //�ն��豸��¼����
#define MSG_SDA_LOGIN                (MSG_DSR_LOGIN|MSG_ACK_GENERAL)  //�ն˵�¼��Ӧ

#define MSG_DSR_UPDATE_STATE         0x00000003   //�豸�ϱ�״̬����

#define MSG_STR_KICKOFF              0x00000004  //����������վ�����߻���֪ͨsessionidʧЧ


#define MSG_SDR_CALIBRATION_RANGING  0x000000C   //����궨���
#define MSG_DSA_CALIBRATION_RANGING  (MSG_SDR_CALIBRATION_RANGING|MSG_ACK_GENERAL)

#define MSG_DSR_CALIBRATION_RESULT   0x000000D   

#define MSG_SDR_CALIBRATE            0x000000E
#define MSG_DSA_CALIBRATE            (MSG_SDR_CALIBRATE|MSG_ACK_GENERAL)

#define MSG_USR_TEST                 0x0000F000   //����ͨ��Ӧ��

#define MSG_USR_REGIST               0x0000F001   //�ֻ�ע���˺�����
#define MSG_SUA_REGIST               (MSG_USR_REGIST|MSG_ACK_GENERAL)  //�ֻ�ע���˺���Ӧ

#define MSG_USR_LOGIN                0x0000F002    //�ֻ��û���¼����
#define MSG_SUA_LOGIN                (MSG_USR_LOGIN|MSG_ACK_GENERAL)  //�û���¼��Ӧ

#define MSG_USR_LOGOUT               0x0000F003   //�ֻ�ע�����󣬷��ط�����ͨ��Ӧ��

#define MSG_USR_SWITCHLAB            0x0000F007   //�л�Lab����

#define MSG_SUR_POSTDATA             0x0000F008    //���Ͷ�λ���ݰ�����Ҫ��Ӧ��

#define MSG_USR_GETBINDLIST          0x0000F009   //��ȡ��lab�б�
#define MSG_SUA_GETBINDLIST          (MSG_USR_GETBINDLIST|MSG_ACK_GENERAL)

#define MSG_USR_GETCONFIG            0x0000F00A   //��ȡһ��ָ��lab�����ò���
#define MSG_SUA_GETCONFIG            (MSG_USR_GETCONFIG|MSG_ACK_GENERAL)

#define MSG_USR_SETCONFIG            0x0000F00B   //����lab���ò������ֶ��궨��

#define MSG_USR_CALIBRATION_START    0x0000F00C   //�����Զ��궨(����ͨ��Ӧ��)

#define MSG_SUR_CALIBRATION_RESULT   0x0000F00D   //�Զ��궨���(����ͨ��Ӧ��)

#define MSG_SUR_CALIBRATION_PROGRESS 0x0000F00E   //�Զ��궨״̬֪ͨ



#define MSG_BSR_NOTIFY             0x00008001    //WEB����ƽ̨(�����)֪ͨ��������̨����

#define MSG_SDR_COMMJSON           0x00000100    //�ն�JSON��Ϣ����Ӧ��ͨ��Ӧ��

#define MSG_DSR_COMMJSON           0x00000101    //�ն�JSON��Ϣ�ϱ���Ӧ��ͨ��Ӧ��
                                  
#define MSG_USR_VERIFYCODE         0x00000010  //�ֻ�������֤������
#define MSG_SUA_VERIFYCODE         (MSG_USR_VERIFYCODE|MSG_ACK_GENERAL) //�ֻ�������֤����Ӧ
                                  
#define MSG_DSR_APPLYFORUID        0x00000063   //�豸����UID����
#define MSG_SDA_APPLYFORUID        (MSG_DSR_APPLYFORUID|MSG_ACK_GENERAL) 

#define MSG_USR_CHANGENICK         0x00000016   //�ֻ��޸��ǳ����󣬷��ط�����ͨ��Ӧ��
                                  
#define MSG_USR_CHANGEPSW          0x00000017   //�ֻ��޸���������
#define MSG_SUA_CHANGEPSW          (MSG_USR_CHANGEPSW|MSG_ACK_GENERAL) //�ֻ��޸�������Ӧ
                                  
#define MSG_USR_GETUSERINFO        0x00000018   //�ֻ���ȡ�û���������
#define MSG_SUA_GETUSERINFO        (MSG_USR_GETUSERINFO|MSG_ACK_GENERAL) //�ֻ���ȡ�û�������Ӧ

#define MSG_USR_CHANGEHEAD         0x00000015   //�ֻ��޸�ͷ�����󣬷��ط�����ͨ��Ӧ��
                                  
#define MSG_USR_GETUSERHEAD        0x00000019   //�ֻ���ȡ�û�ͷ����Ϣ����
#define MSG_SUA_GETUSERHEAD        (MSG_USR_GETUSERHEAD|MSG_ACK_GENERAL)//�ֻ���ȡ�û�ͷ����Ϣ��Ӧ
                                  
#define MSG_USR_CHANGESEX          0x0000001A   //�ֻ��޸��û��Ա�����

#define MSG_USR_ACCEPTMSGPUSH      0x0000001B   //�ֻ����ܷ���������Ϣ����:ʹ������

#define MSG_USR_ACCEPTLIVEPUSH     0x0000001C   //�ֻ����������ֻ���ֱ������:ʹ������

#define MSG_USR_CONFIGS            0x0000001D   //�ֻ���ȡϵͳ����
#define MSG_SUA_CONFIGS            (MSG_USR_CONFIGS|MSG_ACK_GENERAL) 

#define MSG_USR_BIND               0x00000020   //�ֻ���/������ն�����
#define MSG_SUA_BIND               (MSG_USR_BIND|MSG_ACK_GENERAL)  //�ֻ���/������ն���Ӧ

#define MSG_VSR_GETBINDUSER        0x00000022   //�ֻ���ѯ�ն˰��������
#define MSG_SVA_GETBINDUSER        (MSG_VSR_GETBINDUSER|MSG_ACK_GENERAL)    //�ֻ���ѯ�ն˰������Ӧ

 
#define MSG_USR_QUERY_GPS          0x00000049 //�ֻ���ѯ�ն�final location  (�ֻ�-->������)
#define MSG_SUA_QUERY_GPS          (MSG_USR_QUERY_GPS|MSG_ACK_GENERAL) //��Ӧ
                                 
#define MSG_DSR_NOTIFY_STATE       0x00000050 //�ն�״̬֪ͨ����   (�ն�-->������)
#define MSG_SDA_NOTIFY_STATE       (MSG_DSR_NOTIFY_STATE|MSG_ACK_GENERAL) //�ն�״̬֪ͨ��Ӧ

#define MSG_SUR_NOTIFY_STATE       0x00000032 //�ն�״̬֪ͨ���� (������--�ֻ�>)
#define MSG_USA_NOTIFY_STATE       (MSG_SUR_NOTIFY_STATE|MSG_ACK_GENERAL) //�ն�״̬֪ͨ��Ӧ
                                 
#define MSG_DSR_NOTIFY_STRIKE       0x00000060  //�ն���ײ֪ͨ����
#define MSG_SDA_NOTIFY_STRIKE      (MSG_DSR_NOTIFY_STRIKE|MSG_ACK_GENERAL) //�ն���ײ֪ͨ��Ӧ

#define MSG_USR_QUERY_VERSION       0x00000061   //�ֻ���ѯ�汾��Ϣ
#define MSG_SUA_QUERY_VERSION      (MSG_USR_QUERY_VERSION|MSG_ACK_GENERAL)  

#define MSG_USR_VERSION_DEPRECATED  0x00000070 //�汾��Ϣͬ������ 
#define MSG_SUA_VERSION_DEPRECATED  (MSG_USR_VERSION_DEPRECATED|MSG_ACK_GENERAL)  //�汾��Ϣͬ����Ӧ
                                 
#define MSG_DSR_NOTIFY_LOWPOWER   0x00000064   //�ն˵�ƿȱ��֪ͨ����

#define MSG_DSR_SYNC               0x00000062 //�ն���Ϣͬ������ (�ն�-->������)
#define MSG_SDA_SYNC               (MSG_DSR_SYNC|MSG_ACK_GENERAL)  //�ն���Ϣͬ����Ӧ
                                 
#define MSG_USR_WAKEUP             0x00000051  //�ֻ�����/�����ն����� ���ֻ�-->��������
#define MSG_SUA_WAKEUP             (MSG_USR_WAKEUP|MSG_ACK_GENERAL)//�ֻ�����/�����ն���Ӧ
                                 
#define MSG_SDR_WAKEUP             0x00000052  //����������/�����ն����󣨷�����-->�նˣ�
#define MSG_DSA_WAKEUP             (MSG_SDR_WAKEUP|MSG_ACK_GENERAL)  //����������/�����ն���Ӧ
                                 
#define MSG_USR_LIVE               0x00000040  //�ֻ�ֱ��/ȡ��ֱ������  �����ֻ�-->��������
#define MSG_SUA_LIVE               (MSG_USR_LIVE|MSG_ACK_GENERAL)  //�ֻ�ֱ��/ȡ��ֱ����Ӧ
                                 
#define MSG_SVR_LIVE               0x00000041  //������ֱ��/ȡ��ֱ������  (������-->��������ֻ�)
#define MSG_VSA_LIVE               (MSG_SVR_LIVE|MSG_ACK_GENERAL) //������ֱ��/ȡ��ֱ����Ӧ
                                 
#define MSG_VSR_LIVE_RET           0x00000044 //�ֻ�ֱ�����֪ͨ���󣨹ۿ��ֻ�-->��������
#define MSG_SVA_LIVE_RET           (MSG_VSR_LIVE_RET|MSG_ACK_GENERAL) //�ֻ�ֱ�����֪ͨ��Ӧ��������-->�ۿ��ֻ���
                                 
#define MSG_SUR_LIVE_RET           0x00000045 //������ֱ�����֪ͨ���󣨷�����-->���ֻ���
#define MSG_USA_LIVE_RET           (MSG_SUR_LIVE_RET|MSG_ACK_GENERAL) //������ֱ�����֪ͨ��Ӧ
                                 
#define MSG_USR_LIVE_STOP          0x00000046 //���ֻ�ȡ��ֱ�����󣨰��ֻ�-->��������
#define MSG_SUA_LIVE_STOP           (MSG_USR_LIVE_STOP|MSG_ACK_GENERAL) //���ֻ�ȡ��ֱ����Ӧ
                                 
#define MSG_VSR_LIVE               0x00000042  //�ֻ�����ֱ��/ȡ���ۿ�����  ���ο��ֻ�-->��������
#define MSG_SVA_LIVE               (MSG_VSR_LIVE|MSG_ACK_GENERAL)  //�ֻ�����ֱ��/ȡ���ۿ���Ӧ
                                 
#define MSG_SUR_LIVE               0x00000043  //����������ֱ��/ȡ���ۿ�����  (������-->��������ֻ�)
#define MSG_USA_LIVE               (MSG_SUR_LIVE|MSG_ACK_GENERAL) //����������ֱ��/ȡ���ۿ���Ӧ

#define MSG_USR_POST_ADVICE        0x00000047  //�ύͶ�߽�������

#define MSG_USR_QUERY_FLOWPACKAGE  0x00000048  //�ײ�����ʹ�ò�ѯ
#define MSG_SUA_QUERY_FLOWPACKAGE  (MSG_USR_QUERY_FLOWPACKAGE|MSG_ACK_GENERAL)
 
#define MSG_DSR_MCU_CHECKVERSION   0x00000071  //MCU�̼��汾������
#define MSG_SDA_MCU_CHECKVERSION   (MSG_DSR_MCU_CHECKVERSION|MSG_ACK_GENERAL)

#define MSG_DSR_MCU_UPGRADE        0x00000072  //MCU�̼�����������
#define MSG_SDA_MCU_UPGRADE        (MSG_DSR_MCU_UPGRADE|MSG_ACK_GENERAL)

#define MSG_DSR_UPLOAD_GPS         0x00000073  //�ն��ϱ�λ�õ���Ϣ����
#define MSG_SDA_UPLOAD_GPS         (MSG_DSR_UPLOAD_GPS|MSG_ACK_GENERAL)

#define MSG_DSR_UPLOAD_BEHAVIOR    0x00000074  //�ն��ϱ���ʻ��Ϊ����
#define MSG_SDA_UPLOAD_BEHAVIOR     (MSG_DSR_UPLOAD_BEHAVIOR|MSG_ACK_GENERAL)

#define MSG_DSR_UPLOAD_ICCID       0x00000075  //�ն��ϱ�SIM��ICCID
#define MSG_SDA_UPLOAD_ICCID      (MSG_DSR_UPLOAD_ICCID|MSG_ACK_GENERAL)

#define MSG_DSR_UPLOAD_IMSI        0x00000076  //�ն��ϱ�SIM��IMSI��
#define MSG_SDA_UPLOAD_IMSI        (MSG_DSR_UPLOAD_IMSI|MSG_ACK_GENERAL)

#define MSG_DSR_QUERY_SN808         0x00000065  //�ն˻�ȡ����808�豸ID����
#define MSG_SDA_QUERY_SN808         (MSG_DSR_QUERY_SN808|MSG_ACK_GENERAL)

#define MSG_USR_QUERY_SN808         0x00000066  //�û���ѯ����808�豸ID����
#define MSG_SUA_QUERY_SN808         (MSG_USR_QUERY_SN808|MSG_ACK_GENERAL)

#define MSG_DSR_QUERY_SNQQ          0x00000077  //�ն˻�ȡQQ�����豸ID&license����
#define MSG_SDA_QUERY_SNQQ          (MSG_DSR_QUERY_SNQQ|MSG_ACK_GENERAL)

#define MSG_USR_QUERY_ACCESSNO      0x00000067  //�ն˲�ѯ������������
#define MSG_SUA_QUERY_ACCESSNO      (MSG_USR_QUERY_ACCESSNO|MSG_ACK_GENERAL)

#define MSG_USR_QUERY_SN_FROM808    0x00000068  //�����û�ͨ��808�豸�Ų�ѯ�����豸��
#define MSG_SUA_QUERY_SN_FROM808    (MSG_USR_QUERY_SN_FROM808|MSG_ACK_GENERAL)

#define MSG_SDR_FORCE_SLEEP         0x00000078  //ǿ���豸���ߣ�Ӧ��ʽΪͨ��Ӧ��

#define MSG_DSR_UPDATE_BOXSN        0x00000080  //�ն��ϱ������ӵĺ��ӵ��豸�ţ�Ӧ��ʽΪͨ��Ӧ��

#define MSG_DSR_NOTIFY_SNAPSHOT     0x00000081  //ץ���ϴ����֪ͨ��Ӧ��ʽΪͨ��Ӧ��

#define MSG_USR_QUERY_STATE         0x00000053  //�ֻ����������ѯ�ն�״̬
#define MSG_SUA_QUERY_STATE         (MSG_USR_QUERY_STATE|MSG_ACK_GENERAL)

#define MSG_SDR_QUERY_STATE         0x00000054  //���������ն˲�ѯ״̬
#define MSG_DSA_QUERY_STATE         (MSG_SDR_QUERY_STATE|MSG_ACK_GENERAL)

//---------------------------------------------------------------------------
#pragma pack (push,1)
//---------------------------------------------------------------------------
enum {ERR_UNKNOWN=-1,ERR_NONE=0,ERR_OVERFREQUENCY,ERR_WRONG,ERR_TIMEOUT,ERR_OVERRUN};
enum {ACTION_NONE=0,ACTION_BDR_DEVSTATECHANGED=1,ACTION_BUR_USERSTATECHANGED=2,ACTION_BUR_GROUPMSG=3,ACTION_BUR_GROUPSMS=4,ACTION_BDR_FORCESLEEP=5,ACTION_BDR_COMMJSON=6};
enum {DEVSTATE_OFFLINE=0,DEVSTATE_SLEEP=1,DEVSTATE_WAKEUP=2,DEVSTATE_ONLINE=3};

//1:��Ԥ��; 2:ȱ��Ԥ�� 3:�����Ѿ��ľ����棨ֻ����һ�Σ� 4:���������ľ�Ԥ�������ڷ�ֵÿ������һ�Σ� 5:������������Ԥ��(���5��ÿ������һ��) 6:��ץ���ϴ���� 10��Ⱥ��-�����࣬11��Ⱥ��-�ƹ��࣬12��Ⱥ��-������
enum {WARNINGMSG_VIBRATE=1,WARNINGMSG_LOWPOWER=2,WARNINGMSG_FLOWDEPLETE=3,WARNINGMSG_LOWFLOW=4,WARNINGMSG_FLOWTOEXPIRE=5,WARNINGMSG_SNAPSHOT_1=6};

#define MAXLEN_SIMID     5
typedef struct{
 U32 devID;
 U8  simID[MAXLEN_SIMID];
 U32 version;
}TMSG_DSR_LOGIN;

typedef struct
{ char name[MAXLEN_USERNAME+1],passwd[SIZE_MD5+1];
}TMSG_USR_LOGIN;

typedef struct{
 U32 ack_msgid;
 U8  error;
}TMSG_ACK_GENERAL;//,TMSG_SVA_LIVE_RET,TMSG_SUA_VERIFYCODE,TMSG_SUA_REGIST,TMSG_SUA_CHANGEPSW,TMSG_SDA_NOTIFY_STATE,TMSG_USA_NOTIFY_STATE,TMSG_SDA_NOTIFY_STRIKE,TMSG_USA_NOTIFY_STRIKE,TMSG_SUA_WAKEUP,TMSG_DSA_WAKEUP,TMSG_USA_LIVE,TMSG_VSA_LIVE,TMSG_SDA_SYNC,TMSG_USA_LIVE_RET,TMSG_SDA_UPLOAD_GPS,TMSG_SDA_UPLOAD_BEHAVIOR,TMSG_SDA_UPLOAD_IMSI/*,TMSG_SDA_UPLOAD_ICCID*/;

typedef struct
{ U32 ack_synid;
  U8  error,state;
}TMSG_SUA_QUERY_STATE,TMSG_DSA_QUERY_STATE ;

typedef struct
{ U8  value;
}TMSG_SSR_SMS,TMSG_STR_SPYNOTIFY,TMSG_VSR_LIVE_RET,TMSG_DSR_NOTIFY_STATE,TMSG_DSR_NOTIFY_STRIKE,TMSG_SDR_WAKEUP,TMSG_USR_CHANGESEX,TMSG_USR_ACCEPTMSGPUSH,TMSG_USR_ACCEPTLIVEPUSH;

typedef struct{
  U32 sessionid;
  U32 hb_interval;
}TMSG_SDA_LOGIN,TMSG_SUA_LOGIN;

typedef struct
{ char phone[SIZE_PHONE_MOBILE+1];
  U32  groupid;
}TMSG_USR_VERIFYCODE;

typedef struct
{ char phone[SIZE_PHONE_MOBILE+1];
}TMSG_VSR_LIVE,TMSG_USR_GETUSERHEAD;

typedef struct
{ char username[MAXLEN_USERNAME+1];
  char psw[MAXLEN_PASSWORD+1];
  char nick[MAXLEN_NICKNAME+1];   //UTF-8����
  char verifycode[SIZE_VERIFY_CODE+1];
  U32  groupid;  //�û�����(��ǰ�̶�Ϊ����2)
}TMSG_USR_REGIST;

typedef struct
{ U32 timestamp; //����ʱ�䣨unixʱ�����
  U8  type;  //1:��Ԥ��; 2:ȱ��Ԥ�� 3:�����Ѿ��ľ����棨ֻ����һ�Σ� 4:���������ľ�Ԥ�������ڷ�ֵÿ������һ�Σ� 5:������������Ԥ��(���5��ÿ������һ��) 10��Ⱥ��-�����࣬11��Ⱥ��-�ƹ��࣬12��Ⱥ��-������
  char content[0];//utf-8,�����Ⱥ������Ϣ��type>=10)����Ϣ���ݰ�����Ϣ�������Ϣ���ݶ�Ӧ����ҳ��ַ�����֣�����ͨ���س����ָ���
}TMSG_SUR_NOTIFY_MSGBOX;


typedef struct
{ U32 ack_synid;
  U32 msg_count;
  TMSG_SUR_NOTIFY_MSGBOX msg_list[0]; //��Ϣ�б�
}TMSG_SUA_READ_OFFLINEMSG;


typedef struct
{ U32 datalen;
  U8  data[0];  
}TMSG_USR_CHANGEHEAD,T_VARDATA;

typedef struct
{ U32 ack_synid;
  char nickname[MAXLEN_NICKNAME+1];   //UTF-8����
  U8   sex; //0:����;1:��;2:Ů
  U8   msg_push_acceptable; //�Ƿ���ܷ�������Ϣ֪ͨ����
  U8   live_push_acceptable;//�Ƿ���������ֻ���ֱ������
  U32  score;//���֣����ң�
}TMSG_SUA_GETUSERINFO;

typedef struct
{ U32 usrgroup;
}TMSG_USR_QUERY_VERSION;

typedef struct
{ U32  ack_synid;
  T_VARDATA json;
}TMSG_SUA_CONFIGS,TMSG_SUA_QUERY_VERSION,TMSG_SUA_GETBINDLIST;

typedef struct
{ char nick[MAXLEN_NICKNAME+1];   //UTF-8����
}TMSG_USR_CHANGENICK;

typedef struct
{ U8   check_mode;//0��ʹ����֤�루�������ֶ���Ч��1��ʹ�þ�������֤����֤���ֶ���Ч��
  char username[MAXLEN_USERNAME+1];
  char verifycode[SIZE_VERIFY_CODE+1];
  char old_psw[MAXLEN_PASSWORD+1];
  char new_psw[MAXLEN_PASSWORD+1];
}TMSG_USR_CHANGEPSW;

typedef struct
{ U32 ack_synid;
  U32 data_size;
  U8  data[0]; 
}TMSG_SUA_GETUSERHEAD;

typedef struct
{ char device_sn[SIZE_SN_DEVICE+1];
  U8   action;
}TMSG_USR_BIND,TMSG_USR_WAKEUP;

typedef struct
{ char sn[SIZE_SN_DEVICE+1];
}TMSG_VSR_GETBINDUSER,TMSG_USR_QUERY_FLOWPACKAGE,TMSG_USR_QUERY_GPS,TMSG_USR_QUERY_STATE,TMSG_SDR_QUERY_STATE;

typedef struct
{ U32 ack_synid;
  U8 binded;
  char username[MAXLEN_USERNAME+1];
}TMSG_SVA_GETBINDUSER;

typedef struct
{  char device_sn[SIZE_SN_DEVICE+1];
   U32 state_type;//0:����״̬;1:UID�Ѿ����(stateֵ��Ч)
   U8 state_value;
}TMSG_SUR_NOTIFY_STATE;
typedef struct
{ U32 local_version;
}TMSG_DSR_MCU_CHECKVERSION;

typedef struct
{  U32 ack_synid;
   U8  error; // 0:������1:������
   S32 fw_size; //��������С;
   U32 fw_version;//�������汾;
}TMSG_SDA_MCU_CHECKVERSION;

typedef struct
{ U32 fw_version;//���������İ汾��
  U32 fw_offset; //����������ƫ����
  U32 fw_packet_size;//��������ݰ���С
}TMSG_DSR_MCU_UPGRADE;

typedef struct
{  U32 ack_synid;
   U8  error; // 0:������1:������
   U32 packet_size; //���ݰ���С;
   char packet_data[0];
}TMSG_SDA_MCU_UPGRADE;

typedef struct
{ S32 latitude;//γ��(�Զ�Ϊ��λ��γ��ֵ����10��6�η�����ȷ�������֮һ�ȣ�����Ϊ��γ������Ϊ��γ)
  S32 longitude;//����(�Զ�Ϊ��λ�ľ���ֵ����10��6�η�����ȷ�������֮һ�ȣ�����Ϊ����������Ϊ����)
  S16 altitude;//���θ߶ȣ�in meters according to WGS84 ellipsoid��
  U16 speed;//�����ٶȣ�over ground in m/s��
  U16 azimuth;//ˮƽ�淽λ��,����Ϊ0��˳ʱ�루in degree 0 ~ 359)
}TGPSLocation;
 

typedef struct
{ U8 type; //1:Ͷ��;2:����
  char username[MAXLEN_USERNAME+1];//Ͷ�����ֻ���
  char advice[512];//utf-8����
}TMSG_USR_POST_ADVICE;

/*
typedef struct
{ char iccid[SIZE_ICCID+1];//Ͷ�����ֻ���
}TMSG_DSR_UPLOAD_ICCID;
*/

typedef struct
{ char name[MAXLEN_PACKAGENAME];//����������
  U32  total_flow,used_flow;//����������ʹ������(��λMB)
  U32  start_time,end_time;//��ʼ���ڣ�unixʱ�����ʽ��
}TFlowPackageItem;

typedef struct
{ U32 ack_synid;
  U8  error;//0:û����-1:δ֪����;1:������Ч�򲻴���
  U32 number;//��ǰ����������
  TFlowPackageItem items[0];
}TMSG_SUA_QUERY_FLOWPACKAGE;

typedef struct
{ U32 action; //1:�豸��Ϣ�ı䣻2:�û���Ϣ�ı�;3:����Ⱥ����Ϣ
  U32 param_size;
  char param_data[0]; 
}TMSG_SSR_NOTIFY;

//---------------------------------------------------------------------------
#pragma pack (pop) 
//---------------------------------------------------------------------------
#endif
