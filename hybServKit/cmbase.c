/******************************************************************************\
* Copyright (c)  2005  cmShare, All rights Reserved                           *
* FILE NAME:		       cmBase.c                                               *
* PROGRAMMER:	   	     mingo.chan                                             *
* Date of Creation:		 2005/03/12                                             *
* Date of Last Modify: 2005/05/11 (NO.1)                                      *
* DESCRIPTION: 			in common use method lib                                  *
\******************************************************************************/
#include "cmbase.h"
//------------------------------------------------------------------------------
// �ַ�������
//---------------------------------------------------------------------------
BOOL str_isNumeric(char *data)
{ char ch;
  if(!data)return FALSE;
  else ch=*data;
  if((ch>='0' && ch<='9')|| ch=='-'||ch=='+')ch=*data++;
  else return FALSE;
  while(ch>='0' && ch<='9')ch=*data++;
  if(ch=='.'){
    ch=*data++;
    while(ch>='0' && ch<='9')ch=*data++;
  }
  return (ch==0);
}

//�ַ���ת����������ϵͳ����atoiȡ����
// ������atoi�����atoi����ָ��NULL���׳��쳣��
int str_atoi(char *data)
{ int ret=0;
  if(data)
  { while(*data>='0' && *data<='9')
    {  ret=ret*10+ *data-'0';
       data++;
    }
  }
  return ret;
}

int str_htoi(char *strHex)
{ if(strHex)
  { int data=0;
    while(*strHex)
	{ unsigned char hex=*strHex++;
      if(hex>='0' && hex<='9') hex-='0';
      else if(hex>='a' && hex<='f') hex-=('a'-0x0a);
      else if(hex>='A' && hex<='F') hex-=('A'-0x0a);
	  else return -1;
	  data=(data<<4)|hex;
	}
    return data;
  }
  return -1;
}

double str_atof(char *p)
{ if(p)
  { double ret;
    unsigned data_i=0,negtive;
    while(*p && (*p<'0' || *p>'9') && *p!='.' && *p!='-')p++;
    if(*p=='-'){negtive=1;p++;}else negtive=0;
    while(*p>='0' && *p<='9')
    { data_i=data_i*10+(*p-'0');
      p++;
    }
    ret=data_i;
    if(*p=='.')
    { double xs=0.1;
      p++;
      while(*p>='0' && *p<='9')
      { ret=ret+xs*(*p-'0');
        xs/=10;
        p++;
      }
    }
    return (negtive)?-ret:ret;
  }else return 0;
}

int str_ftoa(float fdata,int digit,char *strfloat)
{ long d1=(long)fdata;
  int i=str_itoa(d1,strfloat);
	fdata=fdata-d1;
	if(fdata<0)fdata=-fdata;
	strfloat[i++]='.';
	while(digit-->0)
	{ fdata=fdata*10;
	  strfloat[i++]='0'+((long)fdata)%10;
	}
	strfloat[i]='\0';
  return i; //����ת�����ַ�������
}

int  str_itoa(int value,char *desStr)
{ int i=0;
  if(value>=-9 && value<=9)
  { if(value<0)
    { desStr[i++]='-';
      desStr[i++]='0'-value;
    }
    else
    { desStr[i++]='0'+value;
    }
  }
  else
  { char dtempbuf[16];
    int dLen=0;
    if(value<0){desStr[i++]='-';value=-value;}
    while(value)
    { dtempbuf[dLen++]='0'+(value%10);
      value/=10;
    }
    while(--dLen>=0)
    { desStr[i++]=dtempbuf[dLen];
    }
  }
  desStr[i]='\0';
  return i; //����ת�����ַ�������
}

#define HEX_CHAR(bytedata) (((bytedata)<10)?(bytedata)+'0':(bytedata)-10+'A')
int str_bytesToHex(void *srcData,int dataLen,char *hexBuf,int bufSize,char splitter)
{  int i,space;
   if(!srcData || !hexBuf)return 0;
   //һ���ֽ�������2��HexChar��1���ָ�������3���ַ���ɡ�
   if(splitter)space=bufSize/3;
   else space=bufSize>>1;
   if(dataLen>space)dataLen=space;
   for(i=space=0;i<dataLen;i++)
   { unsigned char chD=((unsigned char *)srcData)[i];
     unsigned char chH=chD>>4,chL=chD&0x0f;
     hexBuf[space++]=HEX_CHAR(chH);
     hexBuf[space++]=HEX_CHAR(chL);
     if(splitter)hexBuf[space++]=splitter;
   }
   if(space<bufSize)hexBuf[space]=0;
   return space;
}

int str_hexToBytes(char *strHex,void *dataBuf,int bufSize)
{ if(!strHex || !dataBuf || bufSize<=0)return 0;
  else {
   int ret=0;
   unsigned char halfByte,pendByte;
   BOOL  halfMatched=FALSE,halfPend=FALSE;
   while((halfByte=*strHex++)!=0)
   { if(halfByte>='0'&&halfByte<='9') halfByte=halfByte-'0';
     else if(halfByte>='a'&&halfByte<='f')halfByte=halfByte-'a'+10;
     else if(halfByte>='A'&&halfByte<='F')halfByte=halfByte-'A'+10;
     else{halfMatched=FALSE;continue;}
     if(halfMatched)
     { ((char *)dataBuf)[ret++]=(pendByte<<4)|halfByte;
        if(ret>=bufSize)return ret;
        halfPend=FALSE;
     }
     else
     { if(halfPend)
       { ((char *)dataBuf)[ret++]=pendByte;
         if(ret>=bufSize)return ret;
       }
       else
       { halfPend=TRUE;
       }
       pendByte=halfByte;
     }
     halfMatched=!halfMatched;
   }
   if(halfPend)((char *)dataBuf)[ret++]=pendByte;
   return ret;
 }
}

int str_hexToBin(char* strHex,char *binBuf,int bufSize)
{ BOOL spaced=TRUE;
  unsigned char ch;
  int i,j,k;
  if(!strHex || !binBuf || bufSize<=0)return 0;
  for(i=j=0;(ch=strHex[i])!=0;i++)
  { if(ch>='0' && ch<='9') ch=ch-'0';
    else if(ch>='A' && ch<='F') ch=ch-'A'+10;
    else if(ch>='a' && ch<='f') ch=ch-'a'+10;
    else
    { if(!spaced)
      { if(j<bufSize)binBuf[j++]=32;
        else return j;
        spaced=TRUE;
      }
      continue;
    }
    spaced=FALSE;
    for(k=0;k<4;k++)
    {  if(j<bufSize)binBuf[j++]=((ch>>(3-k))&0x01)+'0';
       else return j;
    }
  }
  if(j<bufSize)binBuf[j]=0;
  return j;
}

int str_hexToDec(char* strHex,char *decBuf,int bufSize)
{ BOOL spaced=TRUE;
  int i,j,k;
  unsigned long decdata=0;
  unsigned char ch,datatemp[12];
  if(!strHex || !decBuf || bufSize<=0)return 0;
  for(i=j=k=0;(ch=strHex[i])!=0;i++)
  { if(ch>='0' && ch<='9')  ch=ch-'0';
    else if(ch>='A' && ch<='F')  ch=ch-'A'+10;
    else if(ch>='a' && ch<='f')  ch=ch-'a'+10;
    else
    { if(!spaced)
      { label_save_data:
        spaced=TRUE;
        do
        { datatemp[k++]= (unsigned char)(decdata % 10)+'0';
          decdata=decdata /10;
        }while(decdata>0);
        while(k)
        { if(j<bufSize)decBuf[j++]=datatemp[--k];
          else return j;
        }
        if(ch)
        { if(j<bufSize)decBuf[j++]=32;
          else return j;
        }
        else break;
      }
      if(ch=='\0')break;
      else continue;
    }
    spaced=FALSE;
    decdata=(decdata<<4)|ch;
  }
  if(!spaced) goto label_save_data;
  if(j<bufSize)decBuf[j]=0;
  return j;
}

char *stristr(char *src,char *obj)
{ if(src && obj)
  { int state=0;
    while(*src && obj[state])
    { if((*src|0x20)==(obj[state]|0x20))
      { state++;
      }
      else if(state)
      { src-=state;
        state=0;
      }
      src++;
    }
    return (obj[state])?NULL:src-state;
  }else return NULL;
}

//�ַ����滻�����ִ�Сд)����pText�ַ����а�����src�Ӵ�ȫ���滻��des�ַ���
//�����滻�Ĵ���
int str_replace(char *pText,char *src,char *des)
{ int replacecount=0;
  pText=strstr(pText,src);
  if(pText)
  { int srcLen=strlen(src);
    int desLen=strlen(des);
    if(desLen==srcLen)
    { while(pText)
      { replacecount++;
        memcpy(pText,des,desLen);
        pText=strstr(pText+desLen,src);
      }
    }
    else
    { char *temstr=(char *)malloc(strlen(pText)+1);
      int blocklen;
      char *p1,*p2;
      strcpy(temstr,pText);
      p1=temstr;
      while(p1)
      { replacecount++;
        memcpy(pText,des,desLen);
        pText+=desLen;
        p1+=srcLen;
        p2=strstr(p1,src);
        blocklen=(p2)?(int)(p2-p1):strlen(p1);
        if(blocklen)
        { memcpy(pText,p1,blocklen);
          pText+=blocklen;
        }
        p1=p2;
      }
      *pText='\0';
      free(temstr);
    }
  }
  return replacecount; //�����滻�Ĵ���
}

//���ַ��������б��в��ң������ֻ��Ϊָ����splitter��'\0'��, ���طָ������λ��ָ�롣
//eg. str_keySeek("username=cm&user=chem&pwd=123456", "user",'=');
char  *str_keySeek(char *keyList,char *key,char splitter)
{ if(keyList && key && splitter)
  { int i=0,state=0;
    while(keyList[i])
    { if(keyList[i]==key[state])state++;
      else if(state)
      { if(key[state]=='\0' && keyList[i]==splitter)return keyList+i;
        else
        { i-=state;
          state=0;
        }
      }
      i++;
    }
    if(state && key[state]=='\0') return keyList+i;
  }
  return NULL;
}

//�ӷָ����ʾ���ַ����б��в���ָ���ַ����Ƿ���ڣ������ַ�����ַ
//char *ret=str_keySeek("11,111,1111,11111,222222,2222,22,2","111",',');
char  *str_itemSeek(char *itemlist,char *item,char splitter){
  if(itemlist && item){
    char *p,ch1,ch2;
    label_next:
    for(p=item,ch1=*itemlist,ch2=*p;ch1==ch2;ch1=*++itemlist,ch2=*++p){
      if(ch1=='\0')goto label_matched;
    }
    if(ch2=='\0' && ch1==splitter){
      label_matched:
      return itemlist-(int)(p-item);
    }
    while(ch1!=splitter){
      if(ch1=='\0')return NULL;
      else ch1=*++itemlist;
    }
    itemlist++;//jump over splitter
    goto label_next;

  }
  return NULL;
}

/*���ڴ��в���һ�����飬��������ָ�룬��û���ҵ��򷵻�NULL*/
void *mem_search(void *srcMem,int memSize,void *desData,int dataSize)
{ int i,state;
  unsigned char *pmem=(unsigned char *)srcMem;
  unsigned char *psearch=(unsigned char *)desData;
  if(!srcMem || !desData || !memSize || !dataSize)return NULL;
  for(i=0,state=0;i<memSize;i++)
  { if(pmem[i]==psearch[state])
    { state++;
      if(state>=dataSize)return pmem+i+1-dataSize;
    }
    else if(state)
    { i-=state;
      state=0;
    }
  }
  return NULL;
}

void mem_reverse(void *p_buf,int dLen)
{ unsigned char temp,*p=(unsigned char *)p_buf;
  int i,the_pos,steps=dLen >>1;
  for(i=0;i<steps;i++)
  { the_pos=dLen-i-1;
    temp=p[i];
    p[i]=p[the_pos];
    p[the_pos]=temp;
  }
}

typedef struct {HAND taskHandle;void (*asyncfunc)(void *);void *param;int msDelay;}TAsycExecConfig;
static void *thread_asyncexec_proc(void *param){
  TAsycExecConfig *asycExec=(TAsycExecConfig *)param;
  os_msSleep(asycExec->msDelay);
  asycExec->asyncfunc(asycExec->param);
  free(param);
  return NULL;
}

void async_exec(void (*asyncTask)(void *),void *param,int msDelay){
  TAsycExecConfig *asycExec=(TAsycExecConfig *)malloc(sizeof(TAsycExecConfig));
  asycExec->param=param;
  asycExec->msDelay=msDelay;
  asycExec->asyncfunc=asyncTask;
  os_createThread(&asycExec->taskHandle,thread_asyncexec_proc,asycExec);
}

void async_free(void *mem,int msDelay){
  async_exec(free,mem,msDelay);
}

void exit_with_exception(char *errmsg){
  if(errmsg)printf("\n%s\n",errmsg);
  abort();//force core dump
}
//---------------------------------------------------------------------------
//utf8�ַ�����1-6�����Ը���ÿ���ַ���һ���ֽ��ж������ַ�����
//0xxxxxxx
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
int str_lenOfUTF8(char *str)//����str�ַ���Ŀ
{ //������ұ�����256�����е���ֵ��ʾ�Դ�Ϊ��ʼ�ֽڵ�utf8�ַ�����
	static unsigned char utf8_lookup_table[] ={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};
	int len=0,size=0;
  while(*str)
  { if(size){str++;size--;}else{size=utf8_lookup_table[(unsigned char)*str];len++;}
  }
  return len;
}

char *str_xmlSeek(char *xmlbuf,char *key,int *len)
{  if(xmlbuf && key && *key)
   { int keyLen=strlen(key);
     char *match_first=NULL;
     while((xmlbuf=stristr(xmlbuf,key))!=NULL)
     { if(match_first)
       { if(*(xmlbuf-1)=='/' && *(xmlbuf-2)=='<' && *(xmlbuf+keyLen)=='>')
       	 { *len=xmlbuf-match_first-2;
       	   return match_first;
	 }else xmlbuf+=keyLen;
       }
       else
       { if(*(xmlbuf-1)=='<')
         { xmlbuf+=keyLen;
	   if(*xmlbuf=='>')match_first=xmlbuf+1;
	 }else xmlbuf+=keyLen;
       }
     }
   }
   return NULL;
}
//---------------------------------------------------------------------------
// Unixʱ������
//---------------------------------------------------------------------------
int str_fromTime(char *strTime,char *format,time_t timestamp)
{ const int strsize=32;
  return strftime(strTime,strsize,(format)?format:"%Y-%m-%d %H:%M:%S",localtime((const time_t *)&timestamp));
}

time_t str_toTime(char *strTime,char *format)
{ if(strTime && *strTime)
  { struct tm tm_time;
    strptime(strTime,(format)?format:"%Y-%m-%d %H:%M:%S", &tm_time);
    return mktime(&tm_time);
  }
  else return (time_t)0;
}

//get local hour from unix timestamp;
int tm_getLocalHour(time_t timestamp)
{ struct tm *p=localtime(&timestamp); //ʱ���ת��Ϊ����ʱ�䣨hour������ʱ����أ�
  return p->tm_hour;
}
//---------------------------------------------------------------------------
// Queue buffer api
//---------------------------------------------------------------------------
typedef struct{int size, readpos, writepos;char buffer[1];}TQueueBuffer;
/*��⣺
qb->redaposָ��Ҫ������δ����λ�ã�
qb->writeposָ��Ҫд����δд��λ�ã�
qb->size��ָ���еĴ洢�ռ䣬���ǿ��ÿռ�ֻ��qb->size-1����
��ˣ����п�ʱ�����ÿռ���qb->size-1��������ʱ��д����������Ҳ��qb->size-1��
���ǿ��д��qb->size�����ݣ���Ȼ���¶��������
*/
#define _QB_WP(hQb)        ((TQueueBuffer *)hQb)->writepos
#define _QB_RP(hQb)        ((TQueueBuffer *)hQb)->readpos
#define _QB_BUF(hQb)       ((TQueueBuffer *)hQb)->buffer
#define _QB_SIZE(hQb)      ((TQueueBuffer *)hQb)->size
#define _QB_USED_SIZE(hQb) ((_QB_RP(hQb)>_QB_WP(hQb))?(_QB_SIZE(hQb)+_QB_WP(hQb)-_QB_RP(hQb)):(_QB_WP(hQb)-_QB_RP(hQb)))
#define _QB_FREE_SIZE(hQb)  ((_QB_RP(hQb)>_QB_WP(hQb))?(_QB_RP(hQb)-_QB_WP(hQb)-1):(_QB_SIZE(hQb)+_QB_RP(hQb)-_QB_WP(hQb)-1))

HAND qb_create(int size)
{ HAND qb=(HAND)malloc(sizeof(TQueueBuffer)+size+1);//�����1��������
  if(qb)
  { _QB_WP(qb)=0;
    _QB_RP(qb)=0;
    _QB_SIZE(qb)=size;
  }
  return qb;
}

void  qb_destroy(HAND qb)
{ if(qb)free(qb);
}

int  qb_size(HAND qb)
{ return (qb)?_QB_SIZE(qb):0;
}

int  qb_isEmpty(HAND qb)
{ return qb && _QB_RP(qb)==_QB_WP(qb);
}

void qb_clear(HAND qb)
{ if(qb){_QB_RP(qb) = _QB_WP(qb) = 0;}
}

int qb_usedSize(HAND qb)
{ if(qb)
  { return _QB_USED_SIZE(qb);
  }else return 0;
}

int qb_freeSize(HAND qb)
{ if(qb)
  { return  _QB_FREE_SIZE(qb);
  }else return 0;
}

int qb_write(HAND qb,void *buf, int len)
{ if(qb && len>0)
  { int qbSize,wp_new;
    int remaims=_QB_FREE_SIZE(qb);
    if(len>remaims)
    { if(remaims>0)len=remaims;
    	else return 0;
    }
    qbSize=_QB_SIZE(qb);
    wp_new=_QB_WP(qb)+len;
    if(wp_new<qbSize)
    { if(buf)memcpy(_QB_BUF(qb)+_QB_WP(qb), buf, len);
    }
    else
    { wp_new-=qbSize;
      if(buf)
      { int BytesTail=len-wp_new;
      	memcpy(_QB_BUF(qb)+_QB_WP(qb), buf , BytesTail);
        if(wp_new)memcpy(_QB_BUF(qb),(char *)buf+BytesTail, wp_new );
      }
    }
    _QB_WP(qb) = wp_new;
    return len;  /*����ʵ��д����ֽ���*/
  }else return 0;
}

int qb_read(HAND qb,void *buf, int len)
{ //����bufΪNULL����bufΪNULLʱ�൱�ڴӶ��������ָ����Ŀ���ֽ�.
  if(qb && _QB_WP(qb)!=_QB_RP(qb) && len>0)
  { if(len==1)
    { if(buf) *(char *)buf=_QB_BUF(qb)[_QB_RP(qb)];
      if(++_QB_RP(qb)>=_QB_SIZE(qb))_QB_RP(qb)=0;
    }
    else
    { int rp_new,qb_size;
      int usedSize=_QB_USED_SIZE(qb);
      if(len>usedSize)len=usedSize;
      qb_size=_QB_SIZE(qb);
      rp_new=_QB_RP(qb)+len;
      if(rp_new<qb_size)
      { if(buf)memcpy(buf, _QB_BUF(qb)+_QB_RP(qb), len);
      }
      else
      { rp_new-=qb_size;
        if(buf)
        { int BytesTail=len-rp_new;
        	memcpy(buf, _QB_BUF(qb)+_QB_RP(qb), BytesTail );
          if(rp_new)memcpy((char *)buf+BytesTail, _QB_BUF(qb), rp_new);
        }
      }
      _QB_RP(qb)=rp_new;
    }
    return len;
  }else return 0;
}

int qb_peek(HAND qb,void *buf,int len)
{ if(qb && _QB_WP(qb)!=_QB_RP(qb) && len>0  && buf)
  { if(len==1)
    { *(char *)buf=_QB_BUF(qb)[_QB_RP(qb)];
    }
    else
    { int BytesTail,BytesAhead;
      int QueueBytes=_QB_USED_SIZE(qb);
      if(len>QueueBytes)len=QueueBytes;
      BytesAhead=_QB_RP(qb)+len-_QB_SIZE(qb);
      if(BytesAhead<=0)
      { memcpy(buf, _QB_BUF(qb)+_QB_RP(qb), len);
      }
      else
      { BytesTail=len-BytesAhead;
        memcpy(buf, _QB_BUF(qb)+_QB_RP(qb), BytesTail );
        memcpy((char *)buf+BytesTail, _QB_BUF(qb), BytesAhead);
      }
    }
    return len;
  }else return 0;
}

void *qb_usedSpace(HAND qb,int *size1, int *size2)
{ if(_QB_WP(qb)>=_QB_RP(qb))
  { if(size1)*size1=_QB_WP(qb)-_QB_RP(qb);
    if(size2)*size2=0;
  }
  else
  { if(size1)*size1=_QB_SIZE(qb)-_QB_RP(qb);
    if(size2)*size1=_QB_WP(qb);
  }
  return _QB_BUF(qb)+_QB_RP(qb);
}

void *qb_freeSpace(HAND qb,int *size1,int *size2)
{ if(_QB_WP(qb)<_QB_RP(qb))
  { if(size1)*size1=_QB_RP(qb)-_QB_WP(qb)-1;
		if(size2)*size2=0;
  }
  else if(_QB_RP(qb)==0)
  { if(size1)*size1=_QB_SIZE(qb)-_QB_WP(qb)-1;
		if(size2)*size2=0;
	}	
  else
	{	if(size1)*size1=_QB_SIZE(qb)-_QB_WP(qb);
		if(size2)*size2=_QB_RP(qb)-1;
	}
  return _QB_BUF(qb)+_QB_WP(qb);
}
#if 0
int qb_blockWritePrefetch(HAND qb,void **pbuf,int minimum)
{ //�Ӷ�����ȡ����ַ�����Ŀ��п��ָ���Լ��Ŀ��п��С�����п��ַͨ��pbuf��������,���п��Сͨ���������ء�
  //���minimum��������0���������п�ĳ���С��minimumʱ����������ֵ���������������εĵ�����
  //::һ���Ƕ����ܵĿ�д�ռ䲻��������0�����ڶ���������д�ռ䲻��������ȡ���������ռ��С����
  if(qb)
  { if(pbuf)*pbuf=_QB_BUF(qb)+_QB_WP(qb);
    if(_QB_WP(qb)<_QB_RP(qb))
    { int remains=_QB_RP(qb)-_QB_WP(qb)-1;
      return (remains<minimum)?0:remains;
    }
    else
    { if(_QB_RP(qb)==0)
      { int remains=_QB_SIZE(qb)-_QB_WP(qb)-1;
        return (remains<minimum)?0:remains;
      }else return _QB_SIZE(qb)-_QB_WP(qb);
    }
  }else return 0;
}

int qb_blockReadPrefetch(HAND qb,void **pbuf,int minimum)
{ //�Ӷ�����ȡ����ַ���������ݿ��ָ���Լ������ݿ��С�����ݿ��ַͨ��pbuf��������,���ݿ��Сͨ���������ء�
  //���minimum��������0���������ݿ�ĳ���С��minimumʱ����������ֵ���������������εĵ�����
  //::һ���Ƕ����ܵĿɶ����ݲ���������0�����ڶ������������ռ䲻��������ȡ���������ռ��С����
  if(qb)
  { if(pbuf)*pbuf=_QB_BUF(qb)+_QB_RP(qb);
    if(_QB_WP(qb)>=_QB_RP(qb))
    { int remains=_QB_WP(qb)-_QB_RP(qb);
      return (remains<minimum)?0:remains;
    }
    else
    { return _QB_SIZE(qb)-_QB_RP(qb);
    }
  }
  return 0;
}
#endif

BOOL qb_blockRead(HAND qb,void *buf,int len)
{ //�Ӷ����ж���ָ�������ҵ�ַ���������ݡ�����β������Ŀռ佫��������
  //����ʵ�ʶ�ȡ�ĳ��ȣ�Ҫô����len��ҪôΪ0;
  //����bufΪ�գ���ʾֻ�ı��ָ�룻
  if(qb && _QB_WP(qb)!=_QB_RP(qb) && len>0 )
  { int read_pos=_QB_RP(qb);
    if(_QB_WP(qb)<read_pos)
    { int tailspace=_QB_SIZE(qb)-read_pos;
      if(len<=tailspace)
      { if(buf)memcpy(buf,_QB_BUF(qb)+read_pos,len);
      	_QB_RP(qb)=(tailspace==len)?0:read_pos+len;
      	return len;
      }
      else
      { read_pos=0;
      }
    }
    if(len<=_QB_WP(qb)-read_pos)
    { if(buf)memcpy(buf,_QB_BUF(qb)+read_pos,len);
      _QB_RP(qb)=read_pos+len;
      return len;
    }
  }
  return 0;
}

BOOL qb_blockWrite(HAND qb,void *buf,int len)
{ //�������д��ָ�������ҵ�ַ���������ݡ�����β������Ŀռ佫��������
  //����ʵ��д��ĳ��ȣ�Ҫô����len��ҪôΪ0;
  //����bufΪ�գ���ʾֻ�ı�дָ�룻
  if(qb && len>0)
  { int write_pos=_QB_WP(qb);
    if(write_pos>=_QB_RP(qb))
    { int tailspace=_QB_SIZE(qb)-write_pos;
      if(len<tailspace)
      { if(buf)memcpy(_QB_BUF(qb)+write_pos,buf,len);
      	_QB_WP(qb)=write_pos+len;
      	return len;
      }
      else
      { if(_QB_RP(qb)==0)return 0;
        else if(len==tailspace)
        { if(buf)memcpy(_QB_BUF(qb)+write_pos,buf,len);
          _QB_WP(qb)=0;
      	  return len;
        }
        else write_pos=0;
      }
    }
    if(len<=_QB_RP(qb)-write_pos-1)
    { if(buf)memcpy(_QB_BUF(qb)+write_pos,buf,len);
      _QB_WP(qb)=write_pos+len;
      return len;
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
//TMailBox
//---------------------------------------------------------------------------

typedef struct{HAND qb,_msg_mutex,_msg_sem;}TcmMailBox;
typedef struct{int msgLen;/*char msgData[0];*/}TcmMailMsg;
static TcmMailBox  *g_mailBox=NULL;//Ŀǰ�ܹ�ֻ������һ������

void mb_create(int queue_size)
{ if(!g_mailBox)
  { g_mailBox=(TcmMailBox *)malloc(sizeof(TcmMailBox));
    g_mailBox->qb=qb_create(queue_size);
    os_createSemphore(&g_mailBox->_msg_mutex,1);/*�ź�����ֵΪ1����������*/
    os_createSemphore(&g_mailBox->_msg_sem,0);  /*�ź�����ֵΪ0����ʾ��Ϣ��*/
  }
}

void mb_destroy(void)
{ if(g_mailBox)
  { qb_destroy(g_mailBox->qb);
    os_destroySemphore(g_mailBox->_msg_mutex);
    os_destroySemphore(g_mailBox->_msg_sem);
    free(g_mailBox);
    g_mailBox=NULL;
  }
}

void mb_clear(void){
   if(g_mailBox){
      os_obtainSemphore(g_mailBox->_msg_mutex);
      qb_clear(g_mailBox->qb);
      os_releaseSemphore(g_mailBox->_msg_mutex);
   }
}

int  mb_post(void *msgData,int dataLen)//����һ��������Ϣ
{ if(msgData && dataLen)
  { int packet_size=sizeof(TcmMailMsg)+dataLen;
    os_obtainSemphore(g_mailBox->_msg_mutex);
    if(qb_freeSize(g_mailBox->qb)>=packet_size)
    { TcmMailMsg mb_msg;
      mb_msg.msgLen=dataLen;
      qb_write(g_mailBox->qb,&mb_msg,sizeof(TcmMailMsg));
      qb_write(g_mailBox->qb,msgData,dataLen);
      os_releaseSemphore(g_mailBox->_msg_mutex);
      os_releaseSemphore(g_mailBox->_msg_sem);
      return dataLen;
    }
    else
    { os_releaseSemphore(g_mailBox->_msg_mutex);
    }
  }
  return 0;
}

int mb_post2(void *msgHead,int headLen,void *msgData,int dataLen){ //����һ��������Ϣ
  if((headLen>0 && msgHead)||(dataLen>0 && msgData)){
    int packet_size=sizeof(TcmMailMsg)+headLen+dataLen;
    os_obtainSemphore(g_mailBox->_msg_mutex);
    if(qb_freeSize(g_mailBox->qb)>=packet_size)
    { TcmMailMsg mb_msg;
      mb_msg.msgLen=headLen+dataLen;
      qb_write(g_mailBox->qb,&mb_msg,sizeof(TcmMailMsg));
      if(headLen>0 && msgHead)qb_write(g_mailBox->qb,msgHead,headLen);
      if(dataLen>0 && msgData)qb_write(g_mailBox->qb,msgData,dataLen);
      os_releaseSemphore(g_mailBox->_msg_mutex);
      os_releaseSemphore(g_mailBox->_msg_sem);
      return dataLen;
    }
    else
    { os_releaseSemphore(g_mailBox->_msg_mutex);
    }
  }
  return 0;
}


int  mb_receive(void *msgBuf,int bufSize)//��ȡһ��������Ϣ
{ int got_msg_size=0;
  os_obtainSemphore(g_mailBox->_msg_sem);
  os_obtainSemphore(g_mailBox->_msg_mutex);
  if(qb_freeSize(g_mailBox->qb)>=sizeof(TcmMailMsg))
  { TcmMailMsg mb_msg;
    if(qb_read(g_mailBox->qb,&mb_msg,sizeof(TcmMailMsg))==sizeof(TcmMailMsg))
    { got_msg_size=(mb_msg.msgLen<=bufSize)?mb_msg.msgLen:bufSize;
      if(qb_read(g_mailBox->qb,msgBuf,got_msg_size)==got_msg_size)
      { if(got_msg_size<mb_msg.msgLen)qb_read(g_mailBox->qb,NULL,mb_msg.msgLen-got_msg_size);
      }
      else got_msg_size=0;
    }
  }
  os_releaseSemphore(g_mailBox->_msg_mutex);
  return got_msg_size;
}
//---------------------------------------------------------------------------
// Other api
//---------------------------------------------------------------------------
U8 mem_XOR(void *data,int dataLen){
  U8 ret=0;
  while(dataLen){
    ret^=*(U8 *)data;
    data=(U8 *)data+1;
    dataLen--;
  }
  return ret;
}
#if 1  //�������CRC8(�����ɶ���ʽX8+X5+X4+1)
static U8 CRC8_LOOKUP[] = {
0x00,0x5E,0xBC,0xE2,0x61,0x3F,0xDD,0x83,0xC2,0x9C,0x7E,0x20,0xA3,0xFD,0x1F,0x41,
0x9D,0xC3,0x21,0x7F,0xFC,0xA2,0x40,0x1E,0x5F,0x01,0xE3,0xBD,0x3E,0x60,0x82,0xDC,
0x23,0x7D,0x9F,0xC1,0x42,0x1C,0xFE,0xA0,0xE1,0xBF,0x5D,0x03,0x80,0xDE,0x3C,0x62,
0xBE,0xE0,0x02,0x5C,0xDF,0x81,0x63,0x3D,0x7C,0x22,0xC0,0x9E,0x1D,0x43,0xA1,0xFF,
0x46,0x18,0xFA,0xA4,0x27,0x79,0x9B,0xC5,0x84,0xDA,0x38,0x66,0xE5,0xBB,0x59,0x07,
0xDB,0x85,0x67,0x39,0xBA,0xE4,0x06,0x58,0x19,0x47,0xA5,0xFB,0x78,0x26,0xC4,0x9A,
0x65,0x3B,0xD9,0x87,0x04,0x5A,0xB8,0xE6,0xA7,0xF9,0x1B,0x45,0xC6,0x98,0x7A,0x24,
0xF8,0xA6,0x44,0x1A,0x99,0xC7,0x25,0x7B,0x3A,0x64,0x86,0xD8,0x5B,0x05,0xE7,0xB9,
0x8C,0xD2,0x30,0x6E,0xED,0xB3,0x51,0x0F,0x4E,0x10,0xF2,0xAC,0x2F,0x71,0x93,0xCD,
0x11,0x4F,0xAD,0xF3,0x70,0x2E,0xCC,0x92,0xD3,0x8D,0x6F,0x31,0xB2,0xEC,0x0E,0x50,
0xAF,0xF1,0x13,0x4D,0xCE,0x90,0x72,0x2C,0x6D,0x33,0xD1,0x8F,0x0C,0x52,0xB0,0xEE,
0x32,0x6C,0x8E,0xD0,0x53,0x0D,0xEF,0xB1,0xF0,0xAE,0x4C,0x12,0x91,0xCF,0x2D,0x73,
0xCA,0x94,0x76,0x28,0xAB,0xF5,0x17,0x49,0x08,0x56,0xB4,0xEA,0x69,0x37,0xD5,0x8B,
0x57,0x09,0xEB,0xB5,0x36,0x68,0x8A,0xD4,0x95,0xCB,0x29,0x77,0xF4,0xAA,0x48,0x16,
0xE9,0xB7,0x55,0x0B,0x88,0xD6,0x34,0x6A,0x2B,0x75,0x97,0xC9,0x4A,0x14,0xF6,0xA8,
0x74,0x2A,0xC8,0x96,0x15,0x4B,0xA9,0xF7,0xB6,0xE8,0x0A,0x54,0xD7,0x89,0x6B,0x35};
//����CRC8(8λѭ�������룩��CRC8���ԶԳ���Ϊ2^8bit(32Byte)��������100%���
U8 mem_CRC8(void *data,int dataLen){//���
   U8 ret = 0;
   while(dataLen){
     ret = CRC8_LOOKUP[ret^(*(U8 *)data)];
     data=(U8 *)data+1;
     dataLen--;
   }
   return ret;
}
#else  //ֱ�Ӽ���CRC8 (���ɶ���ʽX8+X5+X4+1)
U8 mem_CRC8_2(void *data,int dataLen){
  U8 i,ret=0;
  while(dataLen){
    ret ^= *(U8 *)data;
    data=(U8 *)data+1;
    dataLen--;
    for(i=0;i<8;i++){
      ret=(ret&0x01)?(ret>>1)^0x8C:ret>>1;
    }
  }
  return ret;
}
#endif

//����CRC16(16λѭ�������룩��CRC16���ԶԳ���Ϊ2^16bit(8KByte)��������100%���
//��ͬ�����ɶ���ʽ������˳�򶼻��в�ͬ��CRC�����������ﰴMODBUSЭ���CRC16Ϊ����
static U8 CRC16_LOOKUP_HI[]={//CRC��λ�ֽ�ֵLookupTable
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40};
static U8 CRC16_LOOKUP_LO[]={//CRC��λ�ֽ�ֵLookupTable
0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,0x07,0xC7,0x05,0xC5,0xC4,0x04,
0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,0x08,0xC8,
0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,
0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,0x11,0xD1,0xD0,0x10,
0xF0,0x30,0x31,0xF1,0x33,0xF3,0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,0x3B,0xFB,0x39,0xF9,0xF8,0x38,
0x28,0xE8,0xE9,0x29,0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,
0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,
0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,0xA5,0x65,0x64,0xA4,
0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,
0xB4,0x74,0x75,0xB5,0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,
0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,
0x9C,0x5C,0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,
0x88,0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,0x43,0x83,0x41,0x81,0x80,0x40};
U16 mem_CRC16_modbus(void *data,int dataLen)
{ U8 uchCRCHi=0xFF,uchCRCLo=0xFF; //CRC��λ����λ�ֽڳ�ʼ��
  unsigned uIndex;
  while (dataLen--){
    uIndex = uchCRCHi ^ (*(U8 *)data);
    data=(U8 *)data+1;
    uchCRCHi = uchCRCLo ^ CRC16_LOOKUP_HI[uIndex];
    uchCRCLo = CRC16_LOOKUP_LO[uIndex];
  }
  return (uchCRCLo << 8 | uchCRCHi);  //CRC16 for modbus Ҫ��ߵ��ֽڽ��� 
}

static U32 md5_k[64] ={
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
static int md5_s[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,
5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
//����16λMD5�루�����Ϊ16�ֽڶ��������ݣ����Խ�һ��ת��Ϊ32λ16�����ַ�������
void  mem_MD5(void* data,int dataLen,U8 ret[16]){
  static unsigned char MD5_AppendBuffer[128];
  U32 MD5_h[4]={0x67452301,0xefcdab89,0x98badcfe,0x10325476};
  int i,j;
  if(data && dataLen>0){//MD5_Fill(data,dataLen)
     U32 bit_size=dataLen<<3;
     int appendlen=dataLen&0x3f;
     //������ݳ���Ϊ64�����������Ͳ��ذ�β�����ݣ��Ӷ������������Ч��
     if(appendlen) memcpy(MD5_AppendBuffer,(char *)data+dataLen-appendlen,appendlen);
     MD5_AppendBuffer[appendlen++]=0x80;
     i=(appendlen<=56)?56-appendlen:120-appendlen;
     if(i){
       memset(&MD5_AppendBuffer[appendlen],0,i);
       appendlen+=i;
     }
     for(i=appendlen+8; appendlen<i; appendlen++){
       MD5_AppendBuffer[appendlen] = (char)bit_size;
       bit_size >>= 8;
     }
  }else return;
  for(j=0;j<2;j++){
    int ii,jj,block;
    if(j==0) block=dataLen>>6;
    else{block=((dataLen&0x3f)<=55)?1:2;data=MD5_AppendBuffer;}
    for(i=0;i<block;i++){
      U32 MD5_MM[16],AA=MD5_h[0],BB=MD5_h[1],CC=MD5_h[2],DD=MD5_h[3];
      U8 *pos=(unsigned char *)data+(i<<6);
      for(ii=0; ii < 16; ii++,pos+=4){//MD5_GetM(data, MD5_MM, i<<6);
        for(jj=3;jj>=0;jj--) MD5_MM[ii]=(MD5_MM[ii]<<8)| pos[jj];
      }
      for(jj = 0; jj < 64; jj ++){
        U32 temp1,temp2;
        if(jj<16){temp1=(BB&CC)|((~BB)&DD);temp2=jj;}
        else if(jj<32){temp1=(DD&BB)|((~DD)&CC);temp2=(5*jj+1)%16;}
        else if(jj<48){temp1=BB^CC^DD;temp2 = (3*jj + 5)%16;}
        else{temp1=CC^(BB|(~DD));temp2=(7*jj)%16;}
        temp1=AA + temp1 + MD5_MM[temp2] + md5_k[jj];
        temp2=DD;DD=CC;CC=BB;AA=temp2;
        BB += ((temp1<<md5_s[jj])|(temp1>>(32-md5_s[jj])));
      }
      MD5_h[0]+=AA;MD5_h[1]+=BB;MD5_h[2]+=CC;MD5_h[3]+=DD;
    }
  }
  for(i=0; i<4; i++){
    for(j=0; j<4; j++)ret[(i<<2)+j]=(U8)(MD5_h[i]>>(j<<3));
  }
}

// BKDR Hash Function
U32 BKDRHash(char *str)
{ U32 hash = 0,seed = 131; // 31 131 1313 13131 131313 etc..
  while (*str)
  { hash = hash * seed + (*str++);
  }
  return hash;
}
//---------------------------------------------------------------------------
// TDateTimer
//---------------------------------------------------------------------------
#define DTMR_MAGIC_NUMBER      0x44544D52U//DTMR
#define DTMR_NODE_NAME(node)   ((char *)node->ExtraData+node->nameOffset)
/*
 * DTMR���������ķֹ���
 * ȫ����������ȫ�����񣬰����ڵ�Ĵ����Լ��ڵ��ϵͳ���Լ�״̬���޸�
 * �ڵ��� : ����ڵ���û����ݣ���ExtraData��
 * Lock״̬�Ľڵ㽫��holdס������ʱ
 * �ڵ�ɾ���������������E���׶Σ�����һ�������ID��name���K�M��r�g�S�ȴ���ȫ���r���ڶ�������ȫ���r�󣬽��ڵ��Ƴ���Դ朱�K���ٽڵ���Դ
 * ����ɾ�������ĵ�һ���E�r�����ȡ��ȫ�����Լ��ڵ��������ڵڶ����EֻҪȡ��ȫ�������ɡ�
 * ��Ȼɾ�������ĵ�һ���Eû�������޸Ľڵ��ExtraData������ʵ�������������ɾ��ExtraData������Ҫ�ӽڵ�����
 * Disable�ڵ����ֻҪȡ��ȫ����������Ҫȡ�ýڵ�������
 */
typedef struct _TDateTimerNode  TDateTimerNode;
typedef struct
{ DTMR_TimeoutEvent OnTimeout;
  TDateTimerNode *_tsklist;//dummy
  TBinodeLink *_hashMapTable;
  U32  hashMapLength,dataProtectTime,magicNumber;
  HAND _timer_check_thread,_task_mutex,_sem_timer;
  char name[32];
}TDateTimer;

struct _TDateTimerNode
{ TDateTimerNode *up,*down,*prev,*next;//˳��Ҫ��
  TDateTimer *dtimer;
  HAND semlock;
  U32 mode,msLifeTime,msTimeOut,hashIndex,extraSize,nameOffset,nodeID[2];
  U8 ExtraData[1];//&ExtraData[nameOffset]������nodeName�ַ��������䳤�Ȳ�����extraSize��(extraSize<=nameOffset)����Ԥ����һ���ַ�����������
};

static void _DTMR_UpdateTimeout(TDateTimer *ttasks,TDateTimerNode *node,U32 msLifeTime)
{ TDateTimerNode *dummyHead=ttasks->_tsklist;
  TDateTimerNode *firstNode=dummyHead->next;
  TDateTimerNode *nextNode=node->next;
  U32 msTimeOut=(msLifeTime)?(os_msRunTime()+msLifeTime):0;
  node->msTimeOut=msTimeOut;
  if(node==firstNode){
    //������ڱ�ͷ����ʱ�������õ�����
     if(nextNode==dummyHead || msTimeOut<=nextNode->msTimeOut){
       os_releaseSemphore(ttasks->_sem_timer);//ͷ���ڵ��ʱ�������Ҫ���ź���
     }
     else{
       BINODE_REMOVE(node,prev,next); //��ʱ����������ɾ��
       firstNode=dummyHead->next;//first node changed and update
       goto label_resort;
     }
  }
  else{
    if(node!=nextNode){ //���Ѿ����뵽timelist�е���Ч�ڵ�
       if(msTimeOut<=nextNode->msTimeOut && nextNode!=dummyHead)return;//����Ҫ����
       else BINODE_REMOVE(node,prev,next); //��ʱ����������ɾ��
    }
    label_resort:
    //���²��������˫��������ʱ�����У�
    if(dummyHead==firstNode || msTimeOut<=firstNode->msTimeOut)
    { BINODE_INSERT(node,dummyHead,next,prev);
      os_releaseSemphore(ttasks->_sem_timer);//ͷ���ڵ��ʱ�������Ҫ���ź���
    }
    else
    { TDateTimerNode *lastNode=dummyHead->prev;
      while(msTimeOut<lastNode->msTimeOut)
      { lastNode=lastNode->prev;
      }
      BINODE_INSERT(node,lastNode,next,prev);
    }
  }
}

#define  dtmr_node_match_id_name (node->nodeID[0]==nodeIDL && node->nodeID[1]==nodeIDH &&(!nodeName||strcmp(nodeName,DTMR_NODE_NAME(node))==0))
#define  dtmr_node_match_name    (strcmp(nodeName,DTMR_NODE_NAME(node))==0)
#define  dtmr_node_match_id_data (node->nodeID[0]==nodeIDL && node->nodeID[1]==nodeIDH &&(!nodeData||memcmp(nodeData,(char *)node->ExtraData+dataOffset,dataSize)==0))
#define  dtmr_node_match_data    (memcmp(nodeData,(char *)node->ExtraData+dataOffset,dataSize)==0 && (node->nodeID[0] || node->nodeID[1] || node->ExtraData[node->extraSize]))
#define _DTMR_MarkToDelete(tskNode){\
    tskNode->nodeID[0]=0;\
    tskNode->nodeID[1]=0;\
    (DTMR_NODE_NAME(tskNode))[0]=0;\
    _DTMR_UpdateTimeout(tskNode->dtimer,tskNode,tskNode->dtimer->dataProtectTime);\
}

//ɾ���ڵ㣨����ֻҪִ�нڵ�ɾ�����������������еĵ�һ���輴�ɣ������ID��name���K�M��r�g�S�ȴ���ȫ���r��
//��һ�׶α��ȡ��ȫ�����Լ��ڵ�����
void  dtmr_delete(void *dnode){
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    TDateTimer *dtimer=tskNode->dtimer;
    if(dtimer && dtimer->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(tskNode->semlock)){
       if(os_obtainSemphore(dtimer->_task_mutex)){
         if(tskNode->nodeID[0]||tskNode->nodeID[1]||(DTMR_NODE_NAME(tskNode))[0]){
           _DTMR_MarkToDelete(tskNode);//��ʱ��ʽ��ȫɾ��
         }
         os_releaseSemphore(dtimer->_task_mutex);
       }
       os_releaseSemphore(tskNode->semlock);
    }
  }
}

void *dtmr_add(HAND dtimer,U32 nodeIDL,U32 nodeIDH,char *nodeName,void *nodeData,U32 dataSize,U32 msLifeTime,U32 *options){
  label_start:
  if(dtimer && ((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && (nodeIDL||nodeIDH||nodeName) && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex)){
    BOOL gotLocker=FALSE;
    int nameLen,hashIndex;
    TDateTimerNode *header,*node;
    U32 mode=(options)?*options:0;
    //��ͬʱָ����Ч��taskID��nodeNameʱ������ͨ��taskID������ϣ���ұ�;
    if(nodeIDL||nodeIDH){
      hashIndex=(nodeIDL+nodeIDH)%((TDateTimer *)dtimer)->hashMapLength;
      header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+hashIndex);
      for(node=header->down;node!=header;node=node->down){
        if(dtmr_node_match_id_name)break;
      }
    }
    else{
      hashIndex=BKDRHash(nodeName)%((TDateTimer *)dtimer)->hashMapLength;
      header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+hashIndex);
      for(node=header->down;node!=header;node=node->down){
        if(dtmr_node_match_name)break;
      }
    }
    if(node==header){//û���ҵ�ͬ���ڵ������´����ڵ�
      label_create_new_node:
      nameLen=(nodeName)?strlen(nodeName):0;//�ַ����Ľ������ſռ�����TDateTimerNode�ṹ����Ԥ��(�����ٶ������)
      node=(TDateTimerNode *)malloc(sizeof(TDateTimerNode)+dataSize+nameLen);
      if(!node){
        puts("[ERROR:dtmr_add]#######################################malloc fail!");
        exit(0);
      }
      if(nodeName)memcpy(&node->ExtraData[dataSize],nodeName,nameLen+1);
      else node->ExtraData[dataSize]='\0';
      node->nodeID[0]=nodeIDL;
      node->nodeID[1]=nodeIDH;
      node->hashIndex=hashIndex;
      node->extraSize=node->nameOffset=dataSize;
      node->dtimer=(TDateTimer *)dtimer;

      os_createSemphore(&node->semlock,(mode&DTMR_LOCK)?0:1); //������ʱͬʱ�����Ƿ����
      BINODE_INSERT(node,header,down,up);//����hash����ͷ����������������������ƥ�����½ڵ㣩��
      BINODE_ISOLATE(node,prev,next);
      if(options) *options&=~DTMR_EXIST;//��־�ڵ��ǲ����ڵ��½��ڵ�
    }
    else{ //ԭ���ʹ��ڵĽڵ㡣
      if(options) *options|=DTMR_EXIST;//��־�ڵ��Ѵ���
      //���Ǵ��ڵĽڵ�ʱ����ȡ�ýڵ���
      if((mode&DTMR_LOCK)||(mode&DTMR_OVERRIDE)){
        if(!os_tryObtainSemphore(node->semlock)){
          os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
          os_obtainSemphore(node->semlock);
          os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
          //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��/�޸�
          if(((nodeIDL||nodeIDH)&&!dtmr_node_match_id_name)||(!(nodeIDL||nodeIDH)&&!dtmr_node_match_name)){
            os_releaseSemphore(node->semlock);
            os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
            goto label_start;
          }
        }
        gotLocker=TRUE;
      }
      if(mode&DTMR_OVERRIDE){//������ͬ���ڵ�
         if(dataSize > node->nameOffset){
           _DTMR_MarkToDelete(node);
           os_releaseSemphore(node->semlock);
           goto label_create_new_node;
         }
         else{
           node->extraSize=dataSize;
         }
      }
      else{ //Ĭ������£�������ͬ���ڵ㣬���޸��κ����ݼ����ԣ�ֱ�ӷ����Ѵ��ڵĽڵ㡣
        if(mode&DTMR_LOCK)node->mode|=DTMR_LOCK;
        else os_releaseSemphore(node->semlock);
        os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
        return node->ExtraData;
      }
    }
    if(dataSize){
       if(nodeData)memcpy(node->ExtraData,nodeData,dataSize);
       //else memset(node->ExtraData,0,dataSize);//��ȫ���Ҫ����
    }
    node->mode=mode;
    node->msLifeTime=msLifeTime;
    if( (mode&DTMR_ENABLE) && msLifeTime){
      _DTMR_UpdateTimeout((TDateTimer *)dtimer,node,msLifeTime);//���½ڵ��������ʱ����λ��
    }
    else{
      if(node->next!=node)BINODE_REMOVE(node,next,prev);
      BINODE_ISOLATE(node,next,prev);
    }
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
    if(gotLocker && !(mode&DTMR_LOCK))os_releaseSemphore(node->semlock);
    return node->ExtraData;
  }else return NULL;
}

void *dtmr_find(HAND dtimer,U32 nodeIDL,U32 nodeIDH,char *nodeName,BOOL addLock){
  label_start:
  if(dtimer && ((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex)){
    TDateTimerNode *header,*node;
    if(nodeIDL || nodeIDH){
      int mIndex=(nodeIDL+nodeIDH) % ((TDateTimer *)dtimer)->hashMapLength;
      header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
      for(node=header->down;node!=header;node=node->down){
        if(dtmr_node_match_id_name)goto label_match_node;
      }
    }
    else if(nodeName)
    { int mIndex=BKDRHash(nodeName)%((TDateTimer *)dtimer)->hashMapLength;
      header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
      for(node=header->down;node!=header;node=node->down)
      { if(dtmr_node_match_name)
      	{ label_match_node:
          if(addLock){
            if(!os_tryObtainSemphore(node->semlock)){
              os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
              os_obtainSemphore(node->semlock);
              os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
              //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��
              if(((nodeIDL||nodeIDH)&&!dtmr_node_match_id_name)||(!(nodeIDL||nodeIDH)&&!dtmr_node_match_name)){
                os_releaseSemphore(node->semlock);
                os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
                goto label_start;
              }
            }
            node->mode|=DTMR_LOCK;
          }
          os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
      	  return node->ExtraData;
      	}
      }
    }
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
  }
  return NULL;
}

void *dtmr_findByName(HAND dtimer,char *nodeName,BOOL addLock){
  label_start:
  if(dtimer && nodeName && ((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex)){
    TDateTimerNode *header,*node;
    int mIndex=BKDRHash(nodeName)%((TDateTimer *)dtimer)->hashMapLength;
    header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
    for(node=header->down;node!=header;node=node->down){
      if(strcmp(nodeName,DTMR_NODE_NAME(node))==0){
        if(addLock){
          if(!os_tryObtainSemphore(node->semlock)){
            os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
            os_obtainSemphore(node->semlock);
            os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
            //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��
            if(strcmp(nodeName,DTMR_NODE_NAME(node))!=0){
              os_releaseSemphore(node->semlock);
              os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
              goto label_start;
            }
          }
          node->mode|=DTMR_LOCK;
        }
        os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
      	return node->ExtraData;
      }
    }
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
  }
  return NULL;
}

void *dtmr_findByID(HAND dtimer,U32 nodeIDL,U32 nodeIDH,BOOL addLock){
  label_start:
  if(dtimer && (nodeIDL || nodeIDH) &&((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex)){
    TDateTimerNode *header,*node;
    int mIndex=(nodeIDL+nodeIDH) % ((TDateTimer *)dtimer)->hashMapLength;
    header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
    for(node=header->down;node!=header;node=node->down){
      if(node->nodeID[0]==nodeIDL && node->nodeID[1]==nodeIDH){
        if(addLock){
          if(!os_tryObtainSemphore(node->semlock)){
            os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
            os_obtainSemphore(node->semlock);
            os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
            //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��
            if(node->nodeID[0]!=nodeIDL || node->nodeID[1]!=nodeIDH){
              os_releaseSemphore(node->semlock);
              os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
              goto label_start;
            }
          }
          node->mode|=DTMR_LOCK;
        }
        os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
        return node->ExtraData;
      }
    }
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
  }
  return NULL;
}

void *dtmr_findById(HAND dtimer,U32 nodeIDL,BOOL addLock){
  label_start:
  if(dtimer && nodeIDL &&((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex)){
    TDateTimerNode *header,*node;
    int mIndex=nodeIDL % ((TDateTimer *)dtimer)->hashMapLength;
    header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
    for(node=header->down;node!=header;node=node->down){
      if(node->nodeID[0]==nodeIDL){
        if(addLock){
          if(!os_tryObtainSemphore(node->semlock)){
            os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
            os_obtainSemphore(node->semlock);
            os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
            //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��
            if(node->nodeID[0]!=nodeIDL){
              os_releaseSemphore(node->semlock);
              os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
              goto label_start;
            }
          }
          node->mode|=DTMR_LOCK;
        }
        os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
        return node->ExtraData;
      }
    }
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
  }
  return NULL;
}

void *dtmr_findByData(HAND dtimer,U32 nodeIDL,U32 nodeIDH,void *nodeData,U32 dataSize,int dataOffset,BOOL addLock){
  label_start:
  if(dtimer && ((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex))
   { TDateTimerNode *header,*node;
     if(nodeIDL || nodeIDH)
     { int mIndex=(nodeIDL+nodeIDH) % ((TDateTimer *)dtimer)->hashMapLength;
       header=(TDateTimerNode *)(((TDateTimer *)dtimer)->_hashMapTable+mIndex);
       for(node=header->down;node!=header;node=node->down)
       { if(dtmr_node_match_id_data) goto label_match_node;
       }
     }
     else if(nodeData && dataSize)
     { header=((TDateTimer *)dtimer)->_tsklist;
       for(node=header->next;node!=header;node=node->next)
       { if(dtmr_node_match_data)
         { label_match_node:
           if(addLock){
             if(!os_tryObtainSemphore(node->semlock)){
               os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
               os_obtainSemphore(node->semlock);
               os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex);
               //����Ϊ�˷�ֹ�ж��ڼ䱻ɾ��
               if(((nodeIDL||nodeIDH)&&!dtmr_node_match_id_data)||(!(nodeIDL||nodeIDH)&&!dtmr_node_match_data)){
                 os_releaseSemphore(node->semlock);
                 os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
                 goto label_start;
               }
             }
             node->mode|=DTMR_LOCK;
           }
           os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
           return node->ExtraData;
         }
       }
     }
     os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
   }
   return NULL;
}

BOOL dtmr_lock(void *dnode){
  BOOL ret=FALSE;
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    TDateTimer *dtimer=tskNode->dtimer;
    if(dtimer && dtimer->magicNumber==DTMR_MAGIC_NUMBER){
      if(os_obtainSemphore(tskNode->semlock)){
        if(os_obtainSemphore(dtimer->_task_mutex)){
          if((tskNode->nodeID[0] || tskNode->nodeID[1] || (DTMR_NODE_NAME(tskNode))[0])){
            tskNode->mode|=DTMR_LOCK;
            ret=TRUE;
          }
          else{//�ڵ��Ѿ�ɾ��
            os_releaseSemphore(tskNode->semlock);
          }
          os_releaseSemphore(dtimer->_task_mutex);
        }
      }
    }
  }
  return ret;
}

void dtmr_unlock(void *dnode,U32 msUpdateLifeTime){
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    if((tskNode->mode&DTMR_LOCK)){
      TDateTimer *dtimer=tskNode->dtimer;
      if(dtimer && dtimer->magicNumber==DTMR_MAGIC_NUMBER){
        if(os_obtainSemphore(dtimer->_task_mutex)){
          if(msUpdateLifeTime){
            switch(msUpdateLifeTime){
               case (U32)DTMR_UNLOCK_DELETE:
                     if(tskNode->nodeID[0]||tskNode->nodeID[1]||(DTMR_NODE_NAME(tskNode))[0]){
                       _DTMR_MarkToDelete(tskNode);//��ʱ��ʽ��ȫɾ��
                     }
                     break;
               case (U32)DTMR_UNLOCK_DISABLE:
                     if(tskNode->mode&DTMR_ENABLE){
                         BINODE_REMOVE(tskNode,next,prev);
                         BINODE_ISOLATE(tskNode,next,prev);
                         tskNode->mode&=~DTMR_ENABLE;
                     }
                     break;
                   break;
              default:
                   tskNode->msLifeTime=msUpdateLifeTime;
                   if(tskNode->mode&DTMR_ENABLE)_DTMR_UpdateTimeout((TDateTimer *)dtimer,tskNode,msUpdateLifeTime);

            }
          }
          tskNode->mode&=~DTMR_LOCK;
          os_releaseSemphore(tskNode->semlock);
          os_releaseSemphore(dtimer->_task_mutex);
        }
      }
    }
  }
}

void dtmr_reload(void *dnode){
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    TDateTimer *dtimer=tskNode->dtimer;
    if(dtimer && dtimer->magicNumber==DTMR_MAGIC_NUMBER && os_obtainSemphore(dtimer->_task_mutex)){
      if((tskNode->nodeID[0] || tskNode->nodeID[1] || (DTMR_NODE_NAME(tskNode))[0])){
        if(tskNode->mode&DTMR_ENABLE) _DTMR_UpdateTimeout((TDateTimer *)dtimer,tskNode,tskNode->msLifeTime);
      }
      os_releaseSemphore(dtimer->_task_mutex);
    }
  }
}

BOOL dtmr_update(void *dnode,U32 msUpdateLifeTime,U32 options){
  if(dnode)
  { TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    TDateTimer *dtimer=tskNode->dtimer;
    if(dtimer && dtimer->magicNumber==DTMR_MAGIC_NUMBER){
      if((options&DTMR_LOCK) && !os_obtainSemphore(tskNode->semlock)){
        exit_with_exception("###obtain tskNode->semlock error!");//Should never reach here
      }
      if(os_obtainSemphore(dtimer->_task_mutex)){
        if((tskNode->nodeID[0] || tskNode->nodeID[1] || (DTMR_NODE_NAME(tskNode))[0])){
          if(msUpdateLifeTime)tskNode->msLifeTime=msUpdateLifeTime;
          if((options&DTMR_ENABLE)){
            if(msUpdateLifeTime ||!(tskNode->mode&DTMR_ENABLE)) _DTMR_UpdateTimeout((TDateTimer *)dtimer,tskNode,tskNode->msLifeTime);
          }
          else{
            if(tskNode->mode&DTMR_ENABLE){
              BINODE_REMOVE(tskNode,next,prev);
              BINODE_ISOLATE(tskNode,next,prev);
            }
          }
          tskNode->mode=options;
        }
        else{//�ڵ��Ѿ�ɾ��
          if((options&DTMR_LOCK)) os_releaseSemphore(tskNode->semlock);
          dnode=NULL; //mark fail for return
        }
        os_releaseSemphore(dtimer->_task_mutex);
        return (dnode)?TRUE:FALSE;
      }
    }
  }
  return FALSE;
}

int dtmr_getSize(void *dnode){
  if(dnode){
    TDateTimerNode *tsknode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    return tsknode->extraSize;
  }else return 0;
}

char *dtmr_getName(void *dnode){
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
     return DTMR_NODE_NAME(tskNode);
  }else return NULL;
}

U32 dtmr_getMode(void *dnode){
  if(dnode){
    TDateTimerNode *tskNode=T_PARENT_NODE(TDateTimerNode,ExtraData,dnode);
    return tskNode->mode;
  }else return 0;
}

void dtmr_test(HAND dtmr){
  TDateTimer *dtimer=(TDateTimer *)dtmr;
  TDateTimerNode *timerlist=dtimer->_tsklist;
  TDateTimerNode *node;
  int now_time=os_msRunTime();
  extern void MemoPrint(const char *format, ...);
  for(node=timerlist->next;node!=timerlist; node=node->next){
   // printf("ID:%d_%d, mode=%x, dutime=%ds, next=%x,pre=%x\r\n",node->nodeID[0],node->nodeID[1],node->mode,(node->msTimeOut>now_time)?((node->msTimeOut-now_time)/1000):0, (int)node->next,(int)node->prev);
     printf("%s: ID_%d_%d, mode=%x, dutime=%ds, next=%x,pre=%x\r\n",dtimer->name,node->nodeID[0],node->nodeID[1],node->mode,((int)node->msTimeOut-now_time)/1000, (int)node->next,(int)node->prev);
  }
}

static void *_DTMR_timer_check_proc(void *param){
  TDateTimer *dtimer=(TDateTimer *)param;
  TDateTimerNode *timerlist=dtimer->_tsklist;
  while(dtimer->_timer_check_thread){
    TDateTimerNode *dueNode;
    U32 now_time_ms,msWaitTime;
    if(!os_obtainSemphore(dtimer->_task_mutex))break;
    now_time_ms=os_msRunTime();
    while((dueNode=timerlist->next)!=timerlist && now_time_ms>=dueNode->msTimeOut){
      char *nodeName=DTMR_NODE_NAME(dueNode);
      if((dueNode->nodeID[0] || dueNode->nodeID[1] || nodeName[0])){
        if(dueNode->mode&DTMR_LOCK){
          //lock״̬�Ľڵ�Ҫholdס��������ʱ�������Ǳ�˭�ӵ�������
          label_postpone_locked_node:
          _DTMR_UpdateTimeout(dtimer,dueNode,1000);//�ӳ�1������˵��
          continue;
        }
        if(dtimer->OnTimeout){
          os_releaseSemphore(dtimer->_task_mutex);
          dtimer->OnTimeout(dtimer, dueNode->ExtraData,dueNode->nodeID,nodeName);
          os_obtainSemphore(dtimer->_task_mutex);
          if(timerlist->next!=dueNode || now_time_ms<dueNode->msTimeOut)continue;
          else if((dueNode->mode&DTMR_LOCK)) goto label_postpone_locked_node;
        }
        if((dueNode->mode&DTMR_CYCLE)){
          _DTMR_UpdateTimeout(dtimer,dueNode,dueNode->msLifeTime);
       	  //Log_AppendText("Locked_%d node_%x_%x timeout!",dueNode->locked,dueNode->nodeID[0],dueNode->nodeID[1]);
          continue;
        }

        if((dueNode->mode&DTMR_TIMEOUT_DELETE)){
          //ִ��ɾ���������̱߳�������ȡ�ýڵ���
          if(os_tryObtainSemphore(dueNode->semlock)){
            _DTMR_MarkToDelete(dueNode);
            os_releaseSemphore(dueNode->semlock);
            continue;
          }
          else{
            //ȡ�����ڵ������ӳ�ɾ�����´����һ��ᣩ
            goto label_postpone_locked_node;
          }
        }
        else{//ֹͣ��������ȡ�ýڵ���
          dueNode->mode&=~DTMR_ENABLE;
          BINODE_REMOVE(dueNode,prev,next);//�Ƴ�ʱ��������
          BINODE_ISOLATE(dueNode,prev,next);//�����ڵ�
          continue;
        }
      }
      //����ɾ�����ͷ���Ч�ڵ�(ID��Name��Ϊ�յĽڵ�)
      if(dueNode->next){
        BINODE_REMOVE(dueNode,prev,next);//�Ƴ�ʱ��������
        BINODE_REMOVE(dueNode,up,down);//�ӹ�ϣ������ɾ���ڵ�
        dueNode->next=NULL; //add only for debug
      }else puts("###################### Free fail, node alread destroyed��");
      //printf("free node:%lu\r\n",(long)dueNode);
      os_destroySemphore(dueNode->semlock);
      free(dueNode);
    }
    msWaitTime=(dueNode==timerlist)?50000:(dueNode->msTimeOut-now_time_ms);
    os_releaseSemphore(dtimer->_task_mutex);
    os_waitSemphore(dtimer->_sem_timer,msWaitTime);
  }
  os_destroySemphore(dtimer->_task_mutex);
  os_destroySemphore(dtimer->_sem_timer);
  free(dtimer);
  return NULL;
}

HAND dtmr_create(int hashLen,U32 msHoldTime,DTMR_TimeoutEvent OnTimeout,char *taskName)
{ int c_hashMapLength=(hashLen>0)?hashLen:256;
  int ttask_size=(sizeof(TDateTimer)&0x3)?(sizeof(TDateTimer)|0x03)+1:sizeof(TDateTimer);//address alignment
  int list_binode_size=sizeof(TBinodeLink)*2;
  int hash_binode_size=c_hashMapLength*sizeof(TBinodeLink);
  TDateTimer *dtimer=(TDateTimer *)malloc(ttask_size+list_binode_size+hash_binode_size);
  if(dtimer)
  { int i;
    dtimer->magicNumber=DTMR_MAGIC_NUMBER;
    dtimer->OnTimeout=OnTimeout;
    dtimer->dataProtectTime=(msHoldTime)?msHoldTime:3000;
    dtimer->hashMapLength=c_hashMapLength;
    dtimer->_tsklist=(TDateTimerNode *)((char *)dtimer+ttask_size);
    if(taskName && *taskName)strncpy(dtimer->name,taskName,32);
    else dtimer->name[0]='\0';
    BINODE_ISOLATE(dtimer->_tsklist,prev,next);
    dtimer->_hashMapTable=(TBinodeLink *)((char *)dtimer+ttask_size+list_binode_size);
    for(i=0;i<c_hashMapLength;i++)
    { TBinodeLink *node=dtimer->_hashMapTable+i;
      BINODE_ISOLATE(node,prev,next);
    }
    os_createSemphore(&dtimer->_task_mutex, 1);/*�ź�����ֵΪ1����������*/
    os_createSemphore(&dtimer->_sem_timer,0);
    os_createThread(&dtimer->_timer_check_thread,_DTMR_timer_check_proc, dtimer);
  }
  return (HAND)dtimer;
}

void dtmr_destroy(HAND dtimer)
{ if(dtimer && ((TDateTimer *)dtimer)->magicNumber==DTMR_MAGIC_NUMBER && ((TDateTimer *)dtimer)->_timer_check_thread && os_obtainSemphore(((TDateTimer *)dtimer)->_task_mutex))
  { TDateTimerNode *listhead=((TDateTimer *)dtimer)->_tsklist;
    TDateTimerNode *node=listhead->next;
    while(node!=listhead)
    { TDateTimerNode *nextNode=node->next;
      node->mode=0;
      node->dtimer=NULL;
      os_destroySemphore(node->semlock);
      free(node);
      node=nextNode;
    }
    BINODE_ISOLATE(listhead,prev,next);
    ((TDateTimer *)dtimer)->magicNumber=0;
    ((TDateTimer *)dtimer)->_timer_check_thread=0;
    os_releaseSemphore(((TDateTimer *)dtimer)->_task_mutex);
    os_releaseSemphore(((TDateTimer *)dtimer)->_sem_timer);
  }
}
//---------------------------------------------------------------------------
//TcmSocket
//---------------------------------------------------------------------------
#define TCP_CONNECTION_TIMEOUT_S          60    //unit:second
#define SOCKET_MAX_DGRAMSIZE              1500
#define SOCKET_MAX_LISTEN                 FD_SETSIZE  //��������󲢷���������������FD_SETSIZE,�޸�FD_SETSIZE��ϵͳͷ�ļ��С���
#define SOCKET_INBUFFER_SIZE_DEFAULT      655360U  //should be far larger than SOCKET_MAX_DGRAMSIZE

typedef struct t_tcp_client_socket
{ struct t_tcp_client_socket *next,*prev;
  TNetAddr addr;
  U32 timeout;
}TTcpClients;

typedef struct
{ HAND _client_close_mutex;
  TTcpClients actives,frees,clients[SOCKET_MAX_LISTEN];
}TServerConnections;

typedef struct _t_cmsocket
{ TNetAddr _localAddr,_remoteAddr;//������ڿ�ͷ��Ĭ�ϴӽṹ�忪ͷ����������ַ��
  TServerConnections *_connections;
  HAND _inqueue,_tag;
  U8  _zeroLocalIP,_zeroLocalPort;
  TSocketProtocol _protocol;
  HAND _socket_recv_thread;
  HAND _inqueue_read_mutex,_inqueue_read_sem,_socket_op_mutex;
  int  (*OnRecv)(HAND,TNetAddr *);
  void (*OnClientChange)(HAND,int,BOOL);
  void (*OnClose)(HAND);
}TcmSocket;

typedef struct
{ TNetAddr addr;
  int   datasize;
//  char  data[0];
}TcmPacketHead;

static int _SOCKET_ReceiveToQueue(HAND hSocket,TNetAddr *addr){
  #define PACKET_DATA_BUFF(packet)  (((char *)packet)+sizeof(TcmPacketHead))
  int total_received=0,iSocket=addr->socket;
  os_obtainSemphore(((TcmSocket *)hSocket)->_inqueue_read_mutex);
  while(1)//�����ｫsocket�����е�����ȫ�����꣬�Ծ�������ص�select��ȥ������
  { int block_size1,block_size2;
    TcmPacketHead *packet=(TcmPacketHead *)qb_freeSpace(((TcmSocket *)hSocket)->_inqueue,&block_size1,&block_size2);
    if(block_size1>=SOCKET_MAX_DGRAMSIZE+sizeof(TcmPacketHead))
    { int received_bytes;
      #if MSG_DONTWAIT
      int nonblock_flag=MSG_DONTWAIT;
      #else
      int nonblock_flag=0;
      #endif
      if(((TcmSocket *)hSocket)->_protocol==spUDP)
      { struct sockaddr peerAddr;
      	int addrLen=sizeof(peerAddr);
      	//����connectionless sockets���ӣ�ֻ��ͨ��recvfrom��ȡ��PeerAddress
     	  received_bytes=recvfrom(iSocket, PACKET_DATA_BUFF(packet), SOCKET_MAX_DGRAMSIZE, nonblock_flag ,&peerAddr, (os_socklen_t *)&addrLen);
     	  packet->addr.socket=iSocket;
     	  packet->addr.ip=((struct sockaddr_in *)&peerAddr)->sin_addr.s_addr;
        packet->addr.port=ntohs(((struct sockaddr_in *)&peerAddr)->sin_port);
      }
      else
      { //����connection-oriented��sockets���ӣ�ֻ��ͨ��getpeername��ȡ��PeerAddress
     	  received_bytes=recv(iSocket,PACKET_DATA_BUFF(packet),SOCKET_MAX_DGRAMSIZE,nonblock_flag);
     	  packet->addr=*addr;
      }
      if(received_bytes>0)
      { packet->datasize=received_bytes;
        qb_write(((TcmSocket *)hSocket)->_inqueue,NULL,received_bytes+sizeof(TcmPacketHead));
        #if 0
         packet->data[received_bytes]='\0';
         printf("###receive from socket(%d):\r\n",iSocket);
         puts(packet->data);
        #endif
        total_received+=received_bytes;
        os_releaseSemphore(((TcmSocket *)hSocket)->_inqueue_read_sem);
        if(!nonblock_flag ||(((TcmSocket *)hSocket)->_protocol!=spUDP && received_bytes<SOCKET_MAX_DGRAMSIZE))break;
      }else break;
    }
    else if(block_size2>=SOCKET_MAX_DGRAMSIZE+sizeof(TcmPacketHead))
    { //���ֻ�Ƕ���β��û���㹻�������ռ䣬�����β���ռ䣬�Ӷ���ͷ�����з��䡣
      if(block_size1>=sizeof(TcmPacketHead))
      { packet->addr.socket=0;
        packet->datasize=block_size1-sizeof(TcmPacketHead);
      }
      qb_write(((TcmSocket *)hSocket)->_inqueue,NULL,block_size1);
    }
    else
    { puts("####Socket inqueue is out of space!");
      break;//���кľ���û���㹻�Ŀռ䡣
    }
  }
  os_releaseSemphore(((TcmSocket *)hSocket)->_inqueue_read_mutex);
  return total_received;
}

void *_SOCKET_RecvProc(void *param)
{ TcmSocket *cmSocket=(TcmSocket *)param;
  S16 *pSocket=&cmSocket->_localAddr.socket;
  int maxfd=*pSocket;
  struct timeval waitimeout={15,0};//1seconds
  if(cmSocket->_protocol==spTcpServer)
  { TTcpClients *activeList=&cmSocket->_connections->actives;
    TTcpClients *freeList=&cmSocket->_connections->frees;
    TTcpClients *peer;
    fd_set fdset_connects;
    FD_ZERO(&fdset_connects);
    FD_SET(maxfd,&fdset_connects);
    while(*pSocket)
    { struct timeval wt=waitimeout;
      fd_set read_flags=fdset_connects;
      //select��socket�ļ������У�������UDP����TCP��ֻ��һ��socket������û����ͻ�ֱ�ӷ��ء�
      //select����ʱ����������Ϊ��ʱ�����ж�д�¼�������fdset��û�з����¼���λ���ᱻ���㣬����ļ�ʱ����timevalҲ�ᱻ�ı䣬���ÿ�����µ���selectʱ��Ҫ���¸�ֵfdset��timeval)
      int stat=select(maxfd+1, &read_flags,NULL,NULL,&wt);//wt�ᱻ�ı�
      if(stat>0)
      { U32 now_time=(U32)time(NULL);
      	peer=activeList->next;
      	while(peer!=activeList)
      	{ int sfd=peer->addr.socket;
      	  if(now_time>peer->timeout)
      	  { printf("client[%d] timeout\r\n", sfd);
      	    goto LABEL_CloseSocket;
      	  }
      	  else if(FD_ISSET(sfd,&read_flags))
    	    { if(cmSocket->OnRecv((HAND)cmSocket,&peer->addr)>0)
    	      { peer->timeout=now_time+TCP_CONNECTION_TIMEOUT_S;
    	      }
    	      else
    	      { LABEL_CloseSocket:
    	        { HAND _client_close_mutex=cmSocket->_connections->_client_close_mutex;
    	      	  if(os_obtainSemphore(_client_close_mutex))
    	       	  { TTcpClients *nextnode=peer->next;
                  BINODE_REMOVE(peer,prev,next);
                  BINODE_INSERT(peer,freeList,prev,next);
                  peer=nextnode;
                  os_closeSocket(sfd);
                  os_releaseSemphore(_client_close_mutex);
                }
                if(cmSocket->OnClientChange)cmSocket->OnClientChange(cmSocket,sfd,FALSE);
                FD_CLR((unsigned int)sfd, &fdset_connects);
                if(maxfd==sfd)maxfd=0;

                printf("client[%d] close\r\n", sfd);
              }
              continue;
    	      }
    	    }
      	  peer=peer->next;
      	}
      	if(maxfd==0)//need to update maxfd
      	{ maxfd=cmSocket->_localAddr.socket;
      	  peer=activeList->next;
      	  while(peer!=activeList)
      	  { if(peer->addr.socket>maxfd)maxfd=peer->addr.socket;
      	    peer=peer->next;
      	  }
      	}
    	if(FD_ISSET(cmSocket->_localAddr.socket, &read_flags))
   	  { struct sockaddr_in clientAddr;
    	  int sClient,clientAddrLen=sizeof(clientAddr);
    	  sClient=accept(cmSocket->_localAddr.socket,(struct sockaddr *)&clientAddr,(os_socklen_t *)&clientAddrLen);
    	  if (sClient <= 0)continue;
       	else if(freeList!=freeList->next)
      	{ TTcpClients *newpeer=freeList->next;
      	  BINODE_REMOVE(newpeer,prev,next);
      	  BINODE_INSERT(newpeer,activeList,next,prev);
      	  newpeer->addr.socket=sClient;
      	  newpeer->addr.ip=clientAddr.sin_addr.s_addr;
      	  newpeer->addr.port=ntohs(clientAddr.sin_port);
      	  newpeer->timeout=now_time+TCP_CONNECTION_TIMEOUT_S;
      	  FD_SET(sClient,&fdset_connects);
    	    if(sClient>maxfd)maxfd=sClient;
    	    printf("new connection client[%d] %s:%d\r\n", sClient,inet_ntoa(clientAddr.sin_addr),newpeer->addr.port);
       	  if(cmSocket->OnClientChange)cmSocket->OnClientChange(cmSocket,sClient,TRUE);
      	}
      	else
    	  { printf("max connections arrive!\n");
          send(sClient, "bye", 4, 0);
          os_closeSocket(sClient);
          continue;
    	  }
    	  printf("accepted=%d\r\n",sClient);
        }
      }
      else if(stat==0)
      { printf("select timeout!\r\n");
      	continue;
      }
      else
      {	printf("error:select fail!\r\n");
      	break;
      }
    }
    peer=activeList->next;
    while(peer!=activeList)
    { TTcpClients *delNode=peer;
      os_closeSocket(peer->addr.socket);
      peer=peer->next;
      BINODE_REMOVE(delNode,next,prev)
      BINODE_INSERT(delNode,freeList,next,prev)
    }
    BINODE_ISOLATE(activeList,prev,next);
  }
  else
  { while(*pSocket)
    { int stat;
      struct timeval wt=waitimeout;
      fd_set read_flags;
      FD_ZERO(&read_flags);
      FD_SET(maxfd,&read_flags);
      stat=select(maxfd+1, &read_flags,NULL,NULL,&wt);//waitd�ᱻ�ı�
      if(stat>0)
      { if(FD_ISSET(maxfd, &read_flags))
    	  { if(cmSocket->OnRecv((HAND)cmSocket,&cmSocket->_localAddr)<=0)
   	      { printf("client[%d] close\r\n", maxfd);
            break;
    	    }
    	  }
      }
      else if(stat==0)
      { //printf("select timeout!\r\n");
      	continue;
      }
      else
      {	printf("error:select fail!\r\n");
      	break;
      }
    }
  }

  if(*pSocket>0){ //�������ر���������Է�����������رյģ�
    if(cmSocket->OnClose)cmSocket->OnClose(cmSocket);
    os_closeSocket(*pSocket);
    *pSocket=0;
  }
  return NULL;
}

static BOOL _SOCKET_Switch(TcmSocket *cmSocket,BOOL OnOff)
{ if(!OnOff)
  { if(cmSocket->_localAddr.socket>0)
    { if(cmSocket->OnClose)cmSocket->OnClose(cmSocket);
      os_closeSocket(cmSocket->_localAddr.socket);  //�����ر�
      cmSocket->_localAddr.socket=0;
      os_joinThread(cmSocket->_socket_recv_thread);//�ȴ������̹߳ر�
    }
    return TRUE;
  }
  if(cmSocket->_localAddr.socket>0)return TRUE;
  else
  { BOOL _opened=FALSE;
    int newSocket,socket_type,protocol_value;
    if(cmSocket->_protocol==spTcpServer||cmSocket->_protocol==spTcpClient){protocol_value=IPPROTO_TCP;socket_type=SOCK_STREAM;}
    else if(cmSocket->_protocol==spUDP){protocol_value=IPPROTO_UDP;socket_type=SOCK_DGRAM;}
    else
    { socket_type=SOCK_RAW;
      if(cmSocket->_protocol==spICMP)protocol_value=IPPROTO_ICMP;
      else if(cmSocket->_protocol==spIGMP)protocol_value==IPPROTO_IGMP;
      else protocol_value=IPPROTO_RAW;
    }
    newSocket=socket(AF_INET, socket_type, protocol_value);
    if(newSocket>0)
    { struct sockaddr_in saddr;
      /*if (!cmSocket->_zeroLocalPort)
      {	//SO_REUSEADDR BOOL �����׽ӿں�һ������ʹ���еĵ�ַ����
        int values=1;
        setsockopt(newSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&values,sizeof(int));
      }*/
      saddr.sin_family = AF_INET;
      saddr.sin_addr.s_addr =(cmSocket->_zeroLocalIP)?SOCKET_HostIP(NULL):cmSocket->_localAddr.ip;
      saddr.sin_port = (cmSocket->_zeroLocalPort)?0:htons(cmSocket->_localAddr.port);
      memset(&saddr.sin_zero,0,8);

      if(bind(newSocket,(const struct sockaddr *)&saddr, sizeof(saddr) )!=-1)
      { if(cmSocket->_protocol==spTcpServer)
      	{ if(listen(newSocket,SOCKET_MAX_LISTEN)!=-1)_opened=TRUE;
      	}
      	else if(cmSocket->_protocol==spTcpClient || (cmSocket->_protocol==spUDP && cmSocket->_remoteAddr.ip &&  cmSocket->_remoteAddr.port))
      	{ //��UDP��������Э�飬��UDPЭ���µ���connectֻ���趨Ŀ��Ŀ����ַ���˴�����������������
          saddr.sin_family=AF_INET;
          saddr.sin_port=htons(cmSocket->_remoteAddr.port);
          saddr.sin_addr.s_addr=cmSocket->_remoteAddr.ip;
          memset(&saddr.sin_zero,0,8);
          _opened=(connect(newSocket,(const struct sockaddr *)&saddr, sizeof(saddr))==0);
        }else _opened=TRUE;
      }
      if(_opened)
      { void *_SOCKET_RecvProc(void *);
      	if(cmSocket->_zeroLocalIP || cmSocket->_zeroLocalPort)
      	{ int rt=sizeof(saddr);
     	  if(getsockname(newSocket, (struct sockaddr *)&saddr, (os_socklen_t *)&rt)!=-1)
          { if(cmSocket->_zeroLocalPort)cmSocket->_localAddr.port=ntohs(saddr.sin_port);
            if(cmSocket->_zeroLocalIP)cmSocket->_localAddr.ip=saddr.sin_addr.s_addr;
          }
        }
        os_createThread(&cmSocket->_socket_recv_thread,_SOCKET_RecvProc,cmSocket);
      }
      else
      { os_closeSocket(newSocket);
        newSocket=0;
      }
    }
    cmSocket->_localAddr.socket=newSocket;
    return _opened;
  }
}

BOOL SOCKET_Open(HAND hSocket)
{ if(hSocket && os_obtainSemphore(((TcmSocket *)hSocket)->_socket_op_mutex))
  {  BOOL ret=_SOCKET_Switch((TcmSocket *)hSocket,TRUE);
     os_releaseSemphore(((TcmSocket *)hSocket)->_socket_op_mutex);
     return ret;
  }else return FALSE;
}

void SOCKET_Close(HAND hSocket)
{ if(hSocket && os_obtainSemphore(((TcmSocket *)hSocket)->_socket_op_mutex))
  {  _SOCKET_Switch((TcmSocket *)hSocket,FALSE);
     os_releaseSemphore(((TcmSocket *)hSocket)->_socket_op_mutex);
  }
}

void SOCKET_Bind(HAND hSocket,U32 localIP,U16 localPort,U32 remoteIP,U16 remotePort)
{ if(hSocket && os_obtainSemphore(((TcmSocket *)hSocket)->_socket_op_mutex))
  { BOOL _active=(((TcmSocket *)hSocket)->_localAddr.socket>0);
    if(_active)_SOCKET_Switch((TcmSocket *)hSocket,FALSE); //�޸İ󶨵�ַǰ��ʱ�ȹر��������ָ�
    ((TcmSocket *)hSocket)->_zeroLocalIP=(localIP==0);
    ((TcmSocket *)hSocket)->_zeroLocalPort=(localPort==0);
    ((TcmSocket *)hSocket)->_localAddr.ip=localIP;
    ((TcmSocket *)hSocket)->_localAddr.port=localPort;
    ((TcmSocket *)hSocket)->_remoteAddr.ip=remoteIP;
    ((TcmSocket *)hSocket)->_remoteAddr.port=remotePort;
    if(_active)_SOCKET_Switch((TcmSocket *)hSocket,TRUE);
    os_releaseSemphore(((TcmSocket *)hSocket)->_socket_op_mutex);
  }
}

HAND SOCKET_Create(TSocketProtocol protocol,int inBufSzie)
{ TcmSocket *newsocket=(TcmSocket *)malloc(sizeof(TcmSocket));
  if(newsocket)
  { if(protocol==spTcpServer)
    { TServerConnections *peers=(TServerConnections *)malloc(sizeof(TServerConnections));
    	newsocket->_connections=peers;
    	if(peers)
    	{ int i;
    	  TTcpClients *freelist=&peers->frees;
    	  BINODE_ISOLATE(freelist,prev,next);
    	  BINODE_ISOLATE(&peers->actives,prev,next);
    	  for(i=0;i<SOCKET_MAX_LISTEN;i++)
    	  { TTcpClients *nextnode=&peers->clients[i];
    	    BINODE_INSERT(nextnode,freelist,next,prev)
    	  }
    	  os_createSemphore(&peers->_client_close_mutex,1);/*�ź�����ֵΪ1����������*/
      }else{free(newsocket);return NULL;}
    }else newsocket->_connections=NULL;
    newsocket->_inqueue=(inBufSzie)?qb_create((inBufSzie<SOCKET_INBUFFER_SIZE_DEFAULT)?SOCKET_INBUFFER_SIZE_DEFAULT:inBufSzie):NULL;
    newsocket->_protocol=protocol;
    memset(&newsocket->_localAddr,0,sizeof(TNetAddr));
    memset(&newsocket->_remoteAddr,0,sizeof(TNetAddr));
    newsocket->_tag=NULL;
    newsocket->_zeroLocalIP=0;
    newsocket->_zeroLocalPort=0;
    newsocket->_socket_recv_thread=0;    
    newsocket->OnClose=NULL;
    newsocket->OnClientChange=NULL;
    newsocket->OnRecv=_SOCKET_ReceiveToQueue;
    os_createSemphore(&newsocket->_inqueue_read_sem,0);
    os_createSemphore(&newsocket->_inqueue_read_mutex,1);
    os_createSemphore(&newsocket->_socket_op_mutex,1);
  }
  return newsocket;
}

void SOCKET_CloseTcpClient(HAND hSocket,TNetAddr *peerAddr)
{ if(hSocket && peerAddr)
  { TServerConnections *connections=((TcmSocket *)hSocket)->_connections;
    if(connections && os_obtainSemphore(connections->_client_close_mutex))
    { TTcpClients *activeList=&connections->actives;
      TTcpClients *node=activeList->next;
      while(node!=activeList)
      { if(node->addr.socket==peerAddr->socket)
        { if(node->addr.ip==peerAddr->ip && node->addr.port==peerAddr->port)
          { //tcp serverִ��shutdown��֪ͨpeersocket�رգ����յ�peersocket�رյ���Ϣ���ٵ���closesocket�ر�localsocket��
            //usleep(100000);
            shutdown(peerAddr->socket,2);
          }
          break;
        }else node=node->next;
      }
      os_releaseSemphore(connections->_client_close_mutex);
    }
  }
}

void SOCKET_Destroy(HAND hSocket)
{ if(hSocket)
  { TcmSocket *cmSocket=(TcmSocket *)hSocket;
    SOCKET_Close(hSocket);
    if(cmSocket->_connections)
    { os_destroySemphore(cmSocket->_connections->_client_close_mutex);
      free(cmSocket->_connections);
    }
    free(cmSocket);
    if(cmSocket->_inqueue)qb_destroy(cmSocket->_inqueue);
    os_destroySemphore(cmSocket->_inqueue_read_mutex);
    os_destroySemphore(cmSocket->_inqueue_read_sem);
    os_destroySemphore(cmSocket->_socket_op_mutex);
  }
}

BOOL SOCKET_SetSocketBuffer(HAND hSocket,int recvBufsize,int sendBufsize)
{ /* ������TCP�׽ӿڽ��ջ������Ĵ�Сʱ����������˳���Ǻ���Ҫ�ģ���ΪTCP�Ĵ��ڹ�ģѡ�����ڽ�������ʱ��SYN��Է������õ��ġ����ڿͻ���SO_RCVBUFѡ�������connect֮ǰ���ã����ڷ�������SO_RCVBUFѡ�������listenǰ�Լ�����socket�������á�
     ����ɹ����ã�ʵ�����õĻ�������С�����趨ֵ��2��������ͨ����Ӧ��getsockopt����ȡ���顣
     ��Ҫ�鿴socket��������Ĭ�����ã�����ͨ���鿴����ϵͳ�ļ���֪��
    /proc/sys/net/core/rmem_default	 socket����ʱĬ�ϵĶ�������)�����С��
    /proc/sys/net/core/wmem_default      socket����ʱĬ�ϵ�д������)�����С��
    /proc/sys/net/core/rmem_max	         socket����ͨ��setsockopt���õ������ջ����С��
    /proc/sys/net/core/wmem_max	         socket����ͨ��setsockopt���õ�����ͻ����С��
    ��Ҫ�����޸��������ã����Ա༭/etc/sysctl.conf�ļ������������Ҫ�޸ĵ���(�޸��걣���ļ���ͨ������/sbin/sysctl -pʹ֮������Ч)��
    net.core.rmem_default=124928
    net.core.rmem_max=124928
    net.core.wmem_default=124928
    net.core.wmem_max=124928
  */
  if(hSocket)
  { int localSocket=((TcmSocket *)hSocket)->_localAddr.socket;
    if(localSocket>0)
    { if(recvBufsize && setsockopt(localSocket,SOL_SOCKET,SO_RCVBUF,(char*)&recvBufsize,sizeof(int))!=0) return FALSE;
      if(sendBufsize && setsockopt(localSocket,SOL_SOCKET,SO_SNDBUF,(char*)&sendBufsize,sizeof(int))!=0) return FALSE;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL SOCKET_GetSocketBuffer(HAND hSocket,int *recvBufsize,int *sendBufsize)
{ if(hSocket)
  { int localSocket=((TcmSocket *)hSocket)->_localAddr.socket;
    if(localSocket>0)
    { int len=sizeof(int);
      if(recvBufsize && getsockopt(localSocket,SOL_SOCKET, SO_RCVBUF, (os_sockopt_t *)recvBufsize, (os_socklen_t *)&len)!=0)return FALSE;
      if(sendBufsize && getsockopt(localSocket,SOL_SOCKET, SO_SNDBUF, (os_sockopt_t *)sendBufsize, (os_socklen_t *)&len)!=0)return FALSE;
      return TRUE;
    }
  }
  return FALSE;
}

//����peerAddr������Ŀ��������IP��socket�������IP��peerPortΪ���㣬����Ϊ�㡣
int SOCKET_SendTo(HAND hSocket,void *pData, U32 dataLen,U32 peerAddr,U16 peerPort)
{ if(hSocket)
  { int localSocket=((TcmSocket *)hSocket)->_localAddr.socket;
    if(localSocket>0 && pData && dataLen)
    { if(peerPort)
      { struct sockaddr_in saddr;
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(peerPort);
        saddr.sin_addr.s_addr=peerAddr;
        memset(&saddr.sin_zero,0,8);
        return sendto(localSocket,(const char *)pData,dataLen,0,(const struct sockaddr *)&saddr, sizeof(saddr));
      }else return send(peerAddr,(const char *)pData,dataLen,0);
    }
  }
  return -1;
}

int SOCKET_Send(HAND hSocket,void *pData, U32 dataLen)
{ //ִ��send����ֻ�ǽ����ݷ���Э��ջ�ķ��ͻ���������������ִ�з��Ͷ�����
  //�������send�����;ݵĳ���(dataLen)С�ڵ��ڷ��ͻ�������ʣ��ռ��(freeLen),�򷵻�ʵ�ʷ��뷢�ͻ��ͳ������ֽ���(dataLen)����ʱ���漰�����������ģʽ��
  //�������send�����;ݵĳ���(dataLen)���ڷ��ͻ�������ʣ��ռ��(freeLen),�ڷ�����ģʽ�£��򷵻�ʵ�ʷ��뷢�ͻ��ͳ������ֽ���(freeLen)��������ģʽ�»��Զ����������ݷ��뷢�ͻ����������շ���dateLen��
  //������ͻ�������������socket����ģʽ�»�ȴ��������ݷ��뷢�ͻ����������ڷ�����ģʽ������������-1��
  //����ģʽ����ʽ���͵ģ����ܶ�󳤶ȵ����ݶ��ܷ��������ȴ��������ݷ�����ŷ��ء�
  //CentOSЭ��ջĬ�ϵ�TCP���ͻ���ԼΪ20KB��UDP���ͻ���ԼΪ64KB�������á�
  if(hSocket)
  { int localSocket=((TcmSocket *)hSocket)->_localAddr.socket;
    if(localSocket>0)return send(localSocket,(const char *)pData,dataLen,0);
  }
  return -1;
}

int SOCKET_Receive(HAND hSocket,char *recvBuf, int bufSize,TNetAddr *peerAddr)
{ TcmSocket *cmSocket=(TcmSocket *)hSocket;
  if(cmSocket && cmSocket->_localAddr.socket>0)
  { int readbytes=0;
    os_obtainSemphore(cmSocket->_inqueue_read_sem);
    os_obtainSemphore(cmSocket->_inqueue_read_mutex);
    while(1)
    { TcmPacketHead packet;
      if(qb_blockRead(cmSocket->_inqueue,&packet,sizeof(TcmPacketHead)))
      { if(packet.datasize>0)
        { if(packet.addr.socket>0)
          { readbytes=(packet.datasize<=bufSize)?packet.datasize:bufSize;
            if(qb_read(cmSocket->_inqueue,recvBuf,readbytes)==readbytes)
	    { if(readbytes<packet.datasize)qb_read(cmSocket->_inqueue,NULL,packet.datasize-readbytes);
	      if(peerAddr)*peerAddr=packet.addr;
	      break;
	    }else readbytes=0;
	  }
          else
          { qb_read(cmSocket->_inqueue,NULL,packet.datasize);
          }
        }
      }else break;
    }
    os_releaseSemphore(cmSocket->_inqueue_read_mutex);
    return readbytes;
  }else return 0;  
}

void SOCKET_SetTag(HAND hSocket,HAND tag)
{ if(hSocket)((TcmSocket *)hSocket)->_tag=tag;
}

HAND SOCKET_GetTag(HAND hSocket)
{ return (hSocket)?((TcmSocket *)hSocket)->_tag:NULL;
}

int SOCKET_GetMaxListen(void)
{ return SOCKET_MAX_LISTEN;
}

void SOCKET_SetEvents(HAND hSocket,TSocketRecvEvent onReceive,TSocketCloseEvent onClose,TSocketClientEvent onClientChange)
{ if(hSocket)
  { ((TcmSocket *)hSocket)->OnRecv=(onReceive)?onReceive:_SOCKET_ReceiveToQueue;
    ((TcmSocket *)hSocket)->OnClientChange=onClientChange;
    ((TcmSocket *)hSocket)->OnClose=onClose;
  }
}

BOOL SOCKET_CheckIPStr(char *ipstr)
{ if(ipstr && *ipstr)
  { if(inet_addr(ipstr)!=(U32)-1)return TRUE;
  }
  return FALSE;
}

BOOL SOCKET_IP2Str(U32 ip_data,char *ipstr)
{ if(!ip_data)ip_data=SOCKET_HostIP(NULL);
  strcpy(ipstr,inet_ntoa(*((struct in_addr *)&ip_data)));
  //��64λLinux�����±������ͷ�ļ�<arpa/inet.h>���������inet_ntoa�����Segmentation fault����
  return TRUE;
}

// this routine simply converts the address into an internet ip
U32 SOCKET_HostIP(char *host_name)
{ U32 ip_data=-1;
  if(host_name && *host_name)
  { ip_data=inet_addr(host_name);
    if(ip_data==(U32)-1)
    { struct hostent *host_ent=gethostbyname(host_name);
      if(host_ent) ip_data=*(U32 *)host_ent->h_addr;
      #if __BORLANDC__
      else if(WSAGetLastError()==WSANOTINITIALISED)MessageBox(NULL,"Either the application has not called WSAStartup, or WSAStartup failed!","warning!",MB_OK);
      #endif
    }
  }
  else
  { char localhostbuf[64];
    if(gethostname(localhostbuf,64)==0)
    { struct hostent *host_ent=gethostbyname(localhostbuf);
      if(host_ent) ip_data=*(U32 *)host_ent->h_addr;
    }
  }
  return ip_data;
}
static char *SOCKET_UrlSplit(char *url,int *port,char *hostnamebuf,int bufsize)
{  *port=80;//default port
   while(*url && *url!=':')url++;
   if(url[0]==':' && url[1]=='/' && url[2]=='/')
   { int i=0;
     url+=3;
     while(i<bufsize)
     { if(*url==':')
       { url++;
         *port=0;
	 while(*url>='0' && *url<='9'){*port=*port*10+(int)(*url-'0');url++;}
       }
       else if(*url=='/' || *url=='\0') {hostnamebuf[i]='\0';return url;}
       else hostnamebuf[i++]=*url++;
     }
   }
   return NULL;
}

static int SOCKET_ParseHttpHeap(char *header,int *pHeaderLength,int *pContentLength)
{ //����httpcode,��httpcode����0��ʾhttpheader��δ�����ꡣ
  //����httpcode����200ʱ��headerLength��contentLength����Ч��
  if(header[0]=='H' && header[1]=='T' && header[2]=='T' && header[3]=='P')
  { int httpcode=0;
    char *p=header+4;
    while(*p && *p!=32)p++;
    while(*p==32)p++;
    while(*p>='0'&&*p<='9')httpcode=httpcode*10+(int)(*p++-'0');
    if(httpcode==200)
    { int contentLength;
      char *q=stristr(p,"Content-Length:");
      if(q)
      { contentLength=0;
        q+=15;
      	while(*q==32)q++;
      	while(*q>='0'&&*q<='9')contentLength=contentLength*10+(int)(*q++-'0');
        p=strstr(q,"\r\n\r\n");
      }
      else
      { contentLength=-1; //suspend
        p=strstr(p,"\r\n\r\n");
      }
      if(p)
      { *pContentLength=contentLength;
      	*pHeaderLength=(int)(p-header+4);
      	return httpcode;
      }
    }
    else if(httpcode==302 || httpcode==301)
    { p=stristr(p,"location:");
      if(p)
      { p+=9;
      	while(*p==32)p++;
      	*pHeaderLength=p-header; //urlƫ����
        p=strstr(p,"\r\n");
        if(p)
        { *pContentLength=p-header-*pHeaderLength;//url����
          return httpcode;
    	}
      }
    }
    else return -1;
  }else return -1;
  return 0;	//header��������������һ�����ء�
}

static void socket_nonblock(int socket,BOOL bNonblock)
{ if(socket>0){
#if __BORLANDC__
    unsigned long blockMode=bNonblock;
    ioctlsocket(socket, FIONBIO, &blockMode);
#else
    U32 flags=fcntl(socket,F_GETFL,0); // Get socket flags
    if(flags&O_NONBLOCK)
    { if(bNonblock)return;
      else flags&=~O_NONBLOCK;
    }
    else
    { if(bNonblock)flags|=O_NONBLOCK;
      else return;
    }
    fcntl(socket,F_SETFL,flags);
#endif
  }
}

//POST�������һ���ֶ�ֵ�ḽ��\r\n������Ҫ���շ�ע�⡣
int SOCKET_HttpPost(char *URL,void *formData,int formSize,char *responseBuffer,int buffersize,int msTimeout)
{ struct sockaddr_in servaddr;
  #define HTTP_BUFFER_SIZE 1024
  #define MAXLEN_HOST_NAME   64
  U32 start_time=os_msRunTime();
  int sockfd,ret,svrport,readbytes,httpcode,remains,jmp=0;
  char httpbuf[HTTP_BUFFER_SIZE+1],strhostname[MAXLEN_HOST_NAME],*pbuf,*scriptName;
  label_start_post:
  readbytes=0;
  httpcode=0;
  remains=HTTP_BUFFER_SIZE;
  pbuf=httpbuf;
  if((scriptName=SOCKET_UrlSplit(URL,&svrport,strhostname,MAXLEN_HOST_NAME))!=NULL)
  { if((servaddr.sin_addr.s_addr=SOCKET_HostIP(strhostname))!=(U32)-1)
  	{ servaddr.sin_family = AF_INET;
      servaddr.sin_port = htons(svrport);
      memset(&servaddr.sin_zero,0,8);
    }else return -1;
  }else return -1;
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<=0)return -1;
  else socket_nonblock(sockfd,TRUE); //����������

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) ==-1)
  { struct timeval tm;
    fd_set set;
    int error=-1, len = sizeof(int);
    tm.tv_sec = msTimeout/1000;
    tm.tv_usec = (msTimeout%1000)*1000;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
    { getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (os_sockopt_t *)&error, (os_socklen_t *)&len);
      if(error) goto label_connect_error;
    }
    else
    { label_connect_error:
      printf("connect error!\n");
      os_closeSocket(sockfd);
      return -1;
    }
  }
  socket_nonblock(sockfd,FALSE);  //���û�����ģʽ
  msTimeout-=(os_msRunTime()-start_time);//�����������ĵ�ʱ��������ʣ��ʱ�䡣
  if(msTimeout<=0)return -1;

  if(!formData) //http get
  { //http������ͷ����Ӧͷ������/r/n/r/nΪ������־��
    //����Keep-Alive�ĳ�close������֤ͨ��contentLength��ɽ���������֤ͨ��Keep-Alive�ĳ�close��ɽ���;
    ret=sprintf(httpbuf,"GET %s HTTP/1.0\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUser-Agent: Mozilla/4.0\r\nHost: %s\r\nConnection: close\r\nCache-Control: no-cache\r\n\r\n",(scriptName[0])?scriptName:"/",strhostname);
    ret=send(sockfd,httpbuf,ret,0);
  }
  else  //http post
  { if(formSize<=0)formSize=strlen((char *)formData);
    ret=sprintf(httpbuf,"POST %s HTTP/1.0\r\nHost: %s\r\nAccept-Encoding: gzip,deflate\r\nAccept-Language: zh-cn\r\nAccept: */*\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n",(scriptName[0])?scriptName:"/",strhostname,formSize);
    if(ret+formSize+10<HTTP_BUFFER_SIZE)
    {  memcpy(httpbuf+ret,formData,formSize);
       ret+=formSize;
       ret+=sprintf(httpbuf+ret,"\r\n\r\n");
       ret = send(sockfd,httpbuf,ret,0);
    }
    else
    {  ret = send(sockfd,httpbuf,ret,0);
       if(ret>0)
       { ret=send(sockfd,(const char *)formData,formSize,0);
         if(ret>0)ret=send(sockfd,"\r\n\r\n",4,0);
       }
    }
  }
  if (ret<0)
  { printf("send error %d��Error message'%s'\n",errno, strerror(errno));
    os_closeSocket(sockfd);
    return -1;
  }
  while(1)
  { fd_set sockfd_set;
    struct timeval tv_timeout;
    tv_timeout.tv_sec=msTimeout/1000;
    tv_timeout.tv_usec=(msTimeout%1000)/1000;
    FD_ZERO(&sockfd_set);
    FD_SET(sockfd, &sockfd_set);
    ret=select(sockfd+1, &sockfd_set, NULL, NULL, &tv_timeout);
    if(ret>0)
    { ret=recv(sockfd, pbuf+readbytes,remains,0);
      if(ret>0)
      { readbytes+=ret;
      	remains-=ret;
      	if(!httpcode)
      	{ int contentLength,headerLength;
      	  pbuf[readbytes]='\0';
      	  httpcode=SOCKET_ParseHttpHeap(pbuf,&headerLength,&contentLength);
      	  if(httpcode==200)
      	  { readbytes-=headerLength;
      	    if(readbytes>buffersize)readbytes=buffersize;
      	    if(responseBuffer) memcpy(responseBuffer,pbuf+headerLength,readbytes);
      	    else
      	    { os_closeSocket(sockfd);
              return contentLength;
      	    }
            if(contentLength>0 && contentLength<buffersize)remains=contentLength-readbytes;
            else remains=buffersize-readbytes;
      	    pbuf=responseBuffer;
      	  }
      	  else if( (httpcode==301 || httpcode==302) && ++jmp<3 )
      	  { //֧��301/302����ת���ض�λ��ת��ʱִ��http-get��
      	    URL=pbuf+headerLength;
      	    URL[contentLength]='\0';
      	    os_closeSocket(sockfd);
      	    formData=NULL;//ȥ��formData����
      	    goto label_start_post;//to perform http-get
      	  }
      	  else if(httpcode!=0)break;
      	}
      	if(remains<=0)break;
      }else break;
    }else break;
  }
  os_closeSocket(sockfd);
  if(httpcode==200)
  { if(readbytes<buffersize)responseBuffer[readbytes]='\0';
    return readbytes;
  }
  else return 0;
}

int SOCKET_HttpGet(char *URL,char *responseBuffer,int buffersize,int msTimeout)
{ return SOCKET_HttpPost(URL,NULL,0,responseBuffer,buffersize,msTimeout);
}

#if (__linux__)
U32   os_msRunTime(void)
{ struct timeval tv;
  gettimeofday(&tv,NULL);
  return ((U32)tv.tv_usec/1000)+((U32)tv.tv_sec*1000);
}
#endif

#ifdef _mysql_h
//---------------------------------------------------------------------------
//ע�⣺ͬһ������(conn)�µĶ����ѯ���̲߳���ȫ�ģ����̲߳�ѯʱҪע�������
static pthread_mutex_t db_mutex_lock;
#define SIZE_SQL_FORMAT_BUFFER     640
static MYSQL *_dbconn=NULL;
//---------------------------------------------------------------------------
static void db_exception(void)
{ int _errno=mysql_errno(_dbconn);
  if (_errno)printf("Connection error %d: %s", _errno, mysql_error(_dbconn));
  abort();//force core dump
}

void db_open(char *dbhost,char *dbname,char *dbuser,char *password)
{ _dbconn=mysql_init(NULL);
  if(mysql_real_connect(_dbconn,dbhost,dbuser,password,dbname,0,NULL,0))
  { mysql_query(_dbconn,"set names utf8");
   	pthread_mutexattr_t attr;
  	pthread_mutexattr_init(&attr); 
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP); 
  	pthread_mutex_init(&db_mutex_lock,&attr);
  	pthread_mutexattr_destroy(&attr); 
  }	
  else
  {	printf("Error:can not connect to mysql!\r\n");
  	db_exception();
  }
}

MYSQL *db_conn(void)
{ return _dbconn;
}

void db_close(void)
{ if(_dbconn){mysql_close(_dbconn);_dbconn=NULL;}
}

void db_lock(BOOL lock)
{ if(lock)pthread_mutex_lock(&db_mutex_lock);else pthread_mutex_unlock(&db_mutex_lock);
}

BOOL db_checkSQL(char *sql)
{ if(sql){while(*sql){if(*sql=='\'') return FALSE;else sql++;}}
	return TRUE;
}

char *db_filterSQL(char *text)
{ if(text){{char *p=text;while(*p){if(*p=='\'') *p='`';else p++;}}}
	return text;
}

MYSQL_RES *db_query(char *sql)
{ pthread_mutex_lock(&db_mutex_lock);
  if(mysql_query(_dbconn,sql))
  { db_exception();
  	return NULL;
  }
  else
  {	MYSQL_RES *ret=((sql[0]|0x20)=='s')?mysql_store_result(_dbconn):(MYSQL_RES *)mysql_affected_rows(_dbconn);
  	pthread_mutex_unlock(&db_mutex_lock);
  	return ret;
  }
}

MYSQL_RES *db_queryf(const char *format, ...)
{	char sql_buf[SIZE_SQL_FORMAT_BUFFER+1];
	va_list arg_ptr;
  va_start(arg_ptr, format);
  if(vsprintf(sql_buf, format, arg_ptr)>SIZE_SQL_FORMAT_BUFFER)
  { puts("[ERROR]#################db_queryf###############out of range!");
  	puts(sql_buf);
  	abort();//force core dump ;	
  }
  va_end(arg_ptr);
  return db_query(sql_buf);
}

//---------------------------------------------------------------------------
#endif //_mysql_h
