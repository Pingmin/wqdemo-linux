/**
 * wqplaysdk demo
 *
 * Copyright (c) Vimicro and ZX Elec
 *
 * History:
 *   20130821 - new created, by Zhou Haitao
 *   20130821 - replace _OpenFile() by _OpenStream(), by Liu Zeping
 *
 * More:
 *   http://yunpan.cn/QXHiyqZk8G78W
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "vnplayer.h"

using namespace WQ;

#define NPORT 0

const int MAX_BUF_SIZE  = 10*1024;

void CALLBACK DecCBFun(long nPort,
char * pBuf,
long nSize,
FRAME_INFO * pFrameInfo,
long nReserved1,
long nReserved2)
{
  printf("DecCBFun\n");
  FILE* pf = fopen("test.yuv","ab+");
  fwrite(pBuf,nSize,1,pf);
  fclose(pf);
}

void CALLBACK NeedDecodePasswordFun(long nPort, int nErrorCode, void* pUser)
{
}

int main()
{
  unsigned int size = 0;
  unsigned char buf[MAX_BUF_SIZE] = {0,};
  FILE *fp = fopen("test.vdg", "rb");
  if (NULL == fp)
  {
    printf("fopen failed\n");
    return -1;
  }

  if(VN_PLAY_SetDecCallBack(NPORT, DecCBFun) == 0)
  {
    printf("VN_PLAY_SetDecCallBack failed\n");
    return -1;
  }
  if(VN_PLAY_SetNeedDecodePasswordCallbackFun(NPORT, NeedDecodePasswordFun,NULL) == 0)
  {
    printf("VN_PLAY_SetNeedDecodePasswordCallbackFun failed\n");
    return -1;
  }

  if (!VN_PLAY_SetDecodePassword(NPORT, "123456"))
  {
     printf("VN_PLAY_InputData failed\n");
     return -1;
  }

/***
  if(VN_PLAY_OpenFile(NPORT,"test.vdg")==0)
  {
    printf("VN_PLAY_OpenFile failed\n");
    return -1;
  }
***/
  if(VN_PLAY_OpenStream(NPORT, NULL, 0, 1024*1024) == 0)
  {
    printf("VN_PLAY_OpenStream failed\n");
    return -1;
  }

  if (VN_PLAY_Play(NPORT, 0) == 0)
  {
     printf("VN_PLAY_Play failed\n");
     return -1;
  }

  while (!feof(fp))
  {
/****
    float pos = VN_PLAY_GetPlayPos(NPORT);
    printf("pos = %f\n",pos);
    if(pos>0.95)
    {
      break;
    }
*****/

    memset(buf, 0, sizeof(buf));
    size = fread(buf, sizeof(unsigned char), sizeof(buf), fp);

    if (0 == size)
    {
      printf("fread failed\n");
      continue;
      //return -1;
    }

    if (VN_PLAY_InputData(NPORT, buf, size) == 0)
    {
      printf("VN_PLAY_InputData failed\n");
      //return -1;
    }

    usleep(100*1000);
  }

  if(VN_PLAY_Stop(NPORT) == 0)
  {
    printf("VN_PLAY_Stop failed\n");
    return -1;
  }
  if(VN_PLAY_SetDecCallBack(NPORT, 0) == 0)
  {
    printf("VN_PLAY_SetDecCallBack failed\n");
    return -1;
  }
  if(VN_PLAY_CloseStream(NPORT) == 0)
  {
    printf("VN_PLAY_CloseStream failed\n");
    return -1;
  }

  fclose(fp);
  fp = NULL;

  return 0;
}

