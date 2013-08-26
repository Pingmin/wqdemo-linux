#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "vnplayer.h"

using namespace WQ;

#define NPORT 0

void CALLBACK DecCBFun(long nPort,
char * pBuf,
long nSize,
FRAME_INFO * pFrameInfo,
long nReserved1,
long nReserved2)
{
  printf("DecCBFun\n");
  FILE* pf = fopen("1.yuv","ab+");
  fwrite(pBuf,nSize,1,pf);
  fclose(pf);
}

void CALLBACK NeedDecodePasswordFun(long nPort, int nErrorCode, void* pUser)
{
}

int main()
{
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
  if(VN_PLAY_OpenFile(NPORT,"20130802_roi.vdg")==0)
  {
    printf("VN_PLAY_OpenFile failed\n");
    return -1;
  }
  if (VN_PLAY_Play(NPORT, 0) == 0)
  {
    printf("VN_PLAY_Play failed\n");
    return -1;
  }

  while(1)
  {
    float pos = VN_PLAY_GetPlayPos(NPORT);
    printf("pos = %f\n",pos);
    if(pos>0.95)
    {
    break;
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

  return 0;
}

