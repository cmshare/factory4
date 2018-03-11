#include "mc_routine.h"
#include "mc_dispatch.h"
//---------------------------------------------------------------------------
void mc_dispatchmsg(TMcPacket *packet){
//to map user message to handler 
  BEGIN_MESSAGE_MAP_ONLINE
     MESSAGE_HANDLER(MSG_ACK_GENERAL)
     MESSAGE_HANDLER(MSG_DSR_UPDATE_STATE)
     MESSAGE_HANDLER(MSG_TSR_HEARTBEAT)
     MESSAGE_HANDLER(MSG_USR_GETUSERHEAD)
     MESSAGE_HANDLER(MSG_USR_CHANGEHEAD)
     MESSAGE_HANDLER(MSG_USR_GETUSERINFO)
     MESSAGE_HANDLER(MSG_USR_GETBINDLIST)
     MESSAGE_HANDLER(MSG_USR_CHANGENICK)
     MESSAGE_HANDLER(MSG_USR_LOGOUT)
  BEGIN_MESSAGE_MAP_ANYWAY
     MESSAGE_HANDLER(MSG_DSR_LOGIN) 
     MESSAGE_HANDLER(MSG_USR_LOGIN)
     MESSAGE_HANDLER(MSG_USR_CONFIGS)
  END_MESSAGE_MAP
}
//---------------------------------------------------------------------------
/*todolist
##################todo_first_level:
��hsk_init -���Ӱ�����ص�����;hsk_readPacketȡ��hsk_readData;
���������ƣ�Vericode�ϲ���
���ƹ���Ϣweb�˷���ҳ��
�̶��߳��������Ԥ��
��FindTask Lock��������ƣ�����TaskData)
##################todo_second_level:  
�� ���˵�¼��������������Э��ʹ��TCP/UDP���ɡ�
�� ��¼������UDP�����Ա��¼�ͷ���UDP��ַ��������Ҳ������UDP�����Ա�ά�ֿͻ���NATӳ�����֤��������ʱ����ͨ��UDP��ַ�ҵ��ͻ��ˡ�
������MySQL���ӳ�ʱû��������ѯ����������MySQL�ӳ���ʱʱ��Ϊ1�ꡣ
��Ŀǰ�������ݽ��ղ��ö����̣߳������ݷ��Ͳ����ö����̣߳�����ͨ���ʵ�����socket�ķ��ͻ����С�������ⷢ�ʹ�����ʱ���߳�������
##################rules:  
�� 1. �ն�/�豸��״̬��ָ����ͷ��״̬��MiFI���ӵ�״̬����¼
  2. ����ͷͨ�������˺��ϱ�״̬ʱ��Ҫ���µ�����ͷ���ڴ��Լ����ݿ��е�״̬
  3. �յ�APP��������ָ��ʱ�����˺�����ȥ���Ѻ��ӣ�����ֱ�ӻ��Ѱ��豸
  5. ����������ʱ���������ݿ�״̬
  6. ���Ӻ���ר�õ�¼ָ��
  7. ���ӵ�¼������ͷ��¼����
  8. �����ӿڣ�����ͷ�ϱ����Ӱ����ϵ
��  ��
*/ 
