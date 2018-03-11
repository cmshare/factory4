#include "mc_routine.h"
//---------------------------------------------------------------------------
extern void msg_init(void);
extern void db_init(void);
extern void net_init(void);
extern void terminal_init(void);
extern void upload_init(void);
extern void mcufw_init(void);
extern void routine_init(void);
extern void dblog_init(void);
extern void mc_schedule(void);
extern void UWB_system_init(void);
extern void session_init(void);
//---------------------------------------------------------------------------
int main(int argc, char* argv[]){
  //printf("SOCKET_MAX_LISTEN=%d\r\n",SOCKET_MAX_LISTEN);

  srand((int)time(NULL));

  session_init();

  db_init();//database initialize	

  dblog_init();

  msg_init(); //��Ϣ���г�ʼ��

  net_init();//initialize tcp and udp socket  
	
  terminal_init();//load and initialize devices and users;

  upload_init();

  routine_init();

  mc_schedule();

  pthread_exit(0);//ͨ��pthread_exitʵ���˳���ǰ�̣߳����̣߳������������̡�
  return 0;
}

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
