//---------------------------------------------------------------------------
#ifndef UWB_MODEL_TDOA_H
#define UWB_MODEL_TDOA_H
//---------------------------------------------------------------------------
#define UWB_FRAME_SIZE                    sizeof(UWBRawFrame)
#define FRAME_CACHE_BITLEN                6
#define FRAME_ASSEMBLE_TIMEOUT_MS         2500
#define TAGLIST_HASH_LEN                  256
#define KALMAN_WIDOWN_SIZE                50
//---------------------------------------------------------------------------
//Ϊ���������ѹ����������ÿ����һ��������û�����һ�����ݣ�
//��������һ��ʱ���������������Χ(���ϵ)�����������Χ��������һ�����ݸ��û���
#define MAXLEN_BUFFER_LOCATE_POINTS       32    //��󻺳�Ķ�λ����
#define MAXLEN_BUFFER_LOCATE_TIMESPAN     1500  //��󻺳��ʱ����
//---------------------------------------------------------------------------
enum {UWBERR_UNKNOWN=-1,UWBERR_NONE=0,UWBERR_INVALID_FRAME_FORMAT,UWBERR_CHECKNUM,UWBERR_SESSION,UWBERR_INVALID_TAG,UWBERR_INVALID_ANCHOR};
enum {ANCHOR_TYPE_SYNC=0,ANCHOR_TYPE_DATA=1};
//---------------------------------------------------------------------------
#pragma pack (push,1)
//---------------------------------------------------------------------------
typedef struct{
  U16 accX;     //x-asis ���ٶ���Ϣ.
  U16 accY;     //y-asis ���ٶ���Ϣ
  U16 accZ;     //z-asis ���ٶ���Ϣ
  U16 angX;     //x-asis ���ٶ�
  U16 angY;     //y-asis ���ٶ�
  U16 angZ;     //z-asis ���ٶ�
}TAccData;

typedef struct{
  U32 x,y;
  TAccData accData;
}TAccAndPoint;

typedef struct{
  U32  tagID,labID;
  U16  fps;
  U16  pointCount;
  TAccAndPoint pointData[];
}TUWB_PackDataToUser;

typedef struct _UWB_RawFrame{
  U32 sessionID;   //�ػ�ID
  U32 anchorID;    //��վ/ê��ID
  U32 tagID;       //��ǩID
  U8  syncID;      //��ǩ��ͬ����ˮ��(�ɱ�ǩ�豸���ɣ�0-255ѭ���Լ�,ÿһ�ζ�λʱ����������վӦ�յ�ͬһ����ǩ����ͬSycID�Ķ�λ���ݣ�.
  U8  timeStamp[5];//��ǩ�Ķ�λʱ���(Byte order: little endian, unit: ms)
  //ʱ����ֶ�Ϊ156.25ps���ܹ�5���ֽ�����ʾ17.17s���������л�����빫ʽ
  TAccData accData;
  U8 checknum;  //�����ֽڵ����
}UWBRawFrame;
//---------------------------------------------------------------------------
#pragma pack (pop)
//---------------------------------------------------------------------------
typedef struct{
    int anchorID,tagID,syncID;
    U64 timeStamp;
   // U32 accX,accY,accZ;
    U32 recvTime;
}TDataProcessed;
//---------------------------------------------------------------------------
typedef struct {double x,y;}TPOINT;
typedef struct _TUWBLocalAreaBlock TUWBLocalAreaBlock;
//��վ��Ϣһ����ʼ���󣬾ͻ����̶������ˣ���������Ҫ�Ƕ�����������Ҫ������
typedef struct{
  TTerminal terminal;
  int index,mode;
  TPOINT position;
  TUWBLocalAreaBlock *lab;//belong to which UWB area block
  TDataProcessed frameCache[1<<FRAME_CACHE_BITLEN][1<<FRAME_CACHE_BITLEN];//tagNode data cache pool (hash map list)
}TUWBAnchor;
//---------------------------------------------------------------------------
typedef struct{
   int staFlag;
   int staCNT;//????
   int preloadCounter; //������
   double gafDataBuffer[3][36];
   double historyCoor[2][100];
   double precoor2[2];
   double preInputXY[2];
   double inputXY[2];
   double outputKX[2];
   double kC[3][2];
   double kP[2][2];
}KalmanTagParams;
//---------------------------------------------------------------------------
//���䣨���ã���Ϣû�г�ʱɾ������
//�ɹ����̨����ָ����㷨��̨��ʵ�֣�UWB local area block����ɾ�ĸ��£�
//�㷨�����̨����ʱ�������������ݿ��������м���״̬�ķ�����Ϣ��ֻ�����������õ����յ���λ���ݣ�ʱ�Ŷ�̬�����ڵ㲢�����ݿ�����������Ϣ��
typedef struct{
  U32 labID;
  U32 ab_mm,bc_mm,ac_mm,da_mm,db_mm,dc_mm;
  U16 algorithm;//0:TDOA
  U16 anchorCount;//default=4
}TUWBLabConfig;

struct _TUWBLocalAreaBlock{
  int id;
  int algorithm;//0:TDOA
  int anchorCount;//default=4
  int compensate21,compensate31,compensate41;
  U32 ab_mm,bc_mm,ac_mm,da_mm,db_mm,dc_mm;
  TBinodeLink listeningUsers;
  TUWBAnchor *anchors[0]; //anchor list of A,B,C,etc.
  //����������Ϣ����ʵ�ʵĻ�վ�ڵ����ڴ�
};
//---------------------------------------------------------------------------
//Tag�ڵ㣬���̶����κζ�λ����UWB local area block),�����޷��л��ڶ����UWB lab)
//Taglist  ��ϣ��
//���յ���ǩ����д��tagNode��bufamesʱ������������deadline�����߳�ɨ���Ƿ�ᳬʱ������ʱ��ɾ���ڵ㡣
typedef struct _TUWBTag{
  struct _TUWBTag  *up,*down,*prev,*next;
  int id; //tagID
  TPOINT coor1,coor2;
  TPOINT postion;//last caculated position ()
  KalmanTagParams kalmanTagParams;
  HAND semLock;     //������
  int  fps;//��ʱ��λ֡��(ÿ������bufCoordArrayʱ����һ��);
  int  bufPointIndex;
  U32  bufPointStartTime;//��ʼ����ʱ�䣨���ڼ��㶨λ֡�ʣ�,��λ���롣
  TAccAndPoint bufPointArray[MAXLEN_BUFFER_LOCATE_POINTS];//���嶨λ���ݰ�
  U32  deadline; //��ʱɾ����ʱ�䡣
}TUWBTag;
//---------------------------------------------------------------------------
void UWB_system_init(void);

void UWB_lab_init(TUWBLocalAreaBlock *lab);
void UWB_tag_init(TUWBTag *tag);
int  UWB_process_frame(UWBRawFrame *uwbFrame);
TPOINT *UWB_location_calculate(TUWBAnchor *curAchor,TUWBTag *tagNode,int curSyncID);
void UWB_location_post(TUWBLocalAreaBlock *lab,TUWBTag *tagNode,int pointCount);
BOOL UWB_frame_checkcrc(UWBRawFrame *frame);
void *UWBLab_load(U32 anchorID,U32 labID);
//---------------------------------------------------------------------------
BOOL load_lab_configs(int labID,TUWBLocalAreaBlock *configs,U32 *anchorIDs,int *anchorModes,int maxAnchorCount);
void UWBLab_checkDelete(TUWBLocalAreaBlock *lab);
BOOL UWBLab_switchUser(TTerminal *user,BOOL ownedUserLock,U32 newLabID);//��ʱ�����ǰ󶨹�ϵ
void UWB_update_labconfig(TUWBLabConfig *labcfg);
//---------------------------------------------------------------------------
void MemoPrint(const char *format, ...);
//---------------------------------------------------------------------------
#endif
