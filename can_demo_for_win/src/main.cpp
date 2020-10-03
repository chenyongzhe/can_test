#include "ControlCAN.h"

#include <stdio.h>
#include <thread>
#include <windows.h>

VCI_BOARD_INFO pInfo;
int count=0;

void receive_func()
{
	int reclen=0;
	VCI_CAN_OBJ rec[3000];
	
    while(true) {
		if((reclen=VCI_Receive(VCI_USBCAN2,0,0,rec,3000,100))>0)//调用接收函数，如果有数据，进行数据处理显示。
		{
			for(int j=0;j<reclen;j++)
			{   
				printf("Received: Index:%04d  ",count);count++;//序号递增
				printf("CAN%d RX ID:0x%08X ", 1, rec[j].ID);//ID
				if(rec[j].ExternFlag==0) printf(" Standard ");//帧格式：标准帧
				if(rec[j].ExternFlag==1) printf(" Extend   ");//帧格式：扩展帧
				if(rec[j].RemoteFlag==0) printf(" Data   ");//帧类型：数据帧
				if(rec[j].RemoteFlag==1) printf(" Remote ");//帧类型：远程帧
				printf("DLC:0x%02X",rec[j].DataLen);
				printf(" data: 0x");
				for(int i = 0; i < rec[j].DataLen; i++)
				{
					printf(" %02X", rec[j].Data[i]);
				}
				printf(" TimeStamp:0x%08X",rec[j].TimeStamp);
				printf("\n");
			}
		}		
	}
}

void send_func() {
	VCI_CAN_OBJ send[1];
	send[0].ID=161;
	send[0].SendType=0;
	send[0].RemoteFlag=0;
	send[0].ExternFlag=0;
	send[0].DataLen=8;
    send[0].Data[0] = 1;
    send[0].Data[1] = 3;
    send[0].Data[2] = 0;
    send[0].Data[3] = 0;
    send[0].Data[4] = 0;
    send[0].Data[5] = 0;
    send[0].Data[6] = 0;
    send[0].Data[7] = 0;
			   /*
			   send[1].ID = 162;
			   //send[0].SendType=0;
			   //send[0].RemoteFlag=0;
			   //send[0].ExternFlag=0;
			   //send[0].DataLen=8;


			   send[1].Data[0] = 1;
			   send[1].Data[1] = 3;
			   send[1].Data[2] = 0;
			   send[1].Data[3] = 0;
			   send[1].Data[4] = 0;
			   send[1].Data[5] = 0;
			   send[1].Data[6] = 0;
			   send[1].Data[7] = 0;

			   */
 
 
              /*
	for(int i = 0; i < send[0].DataLen; i++) {
		send[0].Data[i] = i;
	}
              */


    VCI_CAN_OBJ sendsteer[1];
	sendsteer[0].ID=163;
	sendsteer[0].SendType=0;
	sendsteer[0].RemoteFlag=0;
	sendsteer[0].ExternFlag=0;
	sendsteer[0].DataLen=8;
    sendsteer[0].Data[0] = 1;
    sendsteer[0].Data[1] = 0;
    sendsteer[0].Data[2] = 40;
    sendsteer[0].Data[3] = 0;
    sendsteer[0].Data[4] = 0;
    sendsteer[0].Data[5] = 0;
    sendsteer[0].Data[6] = 0;
    sendsteer[0].Data[7] = 0;

    while(true) {
           

        if(VCI_Transmit(VCI_USBCAN2, 0, 0, send, 1) == 1)
		{
			//printf("Send: Index:%04d  ",count);count++;
			printf("CAN1 TX ID:0x%08X",send[0].ID);
			//if(send[0].ExternFlag==0) printf(" Standard ");
			//if(send[0].ExternFlag==1) printf(" Extend   ");
			//if(send[0].RemoteFlag==0) printf(" Data   ");
			//if(send[0].RemoteFlag==1) printf(" Remote ");
			//printf("DLC:0x%02X",send[0].DataLen);
			//printf(" data:0x");

			//for(int i = 0;i < send[0].DataLen; i++)
			//{
			//	printf(" %02X",send[0].Data[i]);
			//}

			//printf("\n");
			//send[0].ID+=1;
                                             // printf("send success\n");
          //  Sleep(500);
		} else {
			//break;

                               }
		
		Sleep(500);
		
              if(VCI_Transmit(VCI_USBCAN2, 0, 0, sendsteer, 1) == 1)
		{
			// printf("send success\n");
				  printf("CAN1 TX ID:0x%08X", sendsteer[0].ID);
		} else {
			//break;
		}
		

		//  Sleep(30);
                        


                     




    }

}

int main(int argc, char* argv[]) {
	printf(">>this is hello !\r\n");
    bool send = false, receive = true;
    printf("argc: %d argv: %s\n", argc, argv[1]);
    if(argc > 1) {
        printf("Send!!!");
        send = true;
        receive = false;
    }
	if(send && VCI_OpenDevice(VCI_USBCAN2,0,0)==1)
	{
		printf(">>open deivce1 success!\n");
	}

    if(receive && VCI_OpenDevice(VCI_USBCAN2,0,0)==1)
	{
		printf(">>open deivce2 success!\n");
	}

	if(send && VCI_ReadBoardInfo(VCI_USBCAN2,0,&pInfo)==1)
	{
        printf(">>Get VCI_ReadBoardInfo success!\n");
		printf("hw_Version: %08X", pInfo.hw_Version);printf("\n");
		printf("fw_Version: %08X", pInfo.fw_Version);printf("\n");
		printf("dr_Version: %08X", pInfo.dr_Version);printf("\n");
		printf("in_Version %08X", pInfo.in_Version);printf("\n");
		printf("irq_Num: %08X", pInfo.irq_Num);printf("\n");
		printf("can_Num: %08X", pInfo.can_Num);printf("\n");
        printf(">>Serial_Num: %s \n", pInfo.str_Serial_Num);
        printf(">>hw_Type: %s \n", pInfo.str_hw_Type);
	}

    if(receive && VCI_ReadBoardInfo(VCI_USBCAN2,0,&pInfo)==1)
	{
        printf(">>Get VCI_ReadBoardInfo success!\n");
		printf("hw_Version: %08X", pInfo.hw_Version);printf("\n");
		printf("fw_Version: %08X", pInfo.fw_Version);printf("\n");
		printf("dr_Version: %08X", pInfo.dr_Version);printf("\n");
		printf("in_Version %08X", pInfo.in_Version);printf("\n");
		printf("irq_Num: %08X", pInfo.irq_Num);printf("\n");
		printf("can_Num: %08X", pInfo.can_Num);printf("\n");
        printf(">>Serial_Num: %s \n", pInfo.str_Serial_Num);
        printf(">>hw_Type: %s \n", pInfo.str_hw_Type);
	}
    
	VCI_INIT_CONFIG config;
	config.AccCode=0;
	config.AccMask=0xFFFFFFFF;
	config.Filter=1;//接收所有帧
	config.Timing0=0x00;//波特率125 Kbps  0x03  0x1C
	config.Timing1= 0x1C;
	config.Mode=0;//正常模式  
	
	if(send && VCI_InitCAN(VCI_USBCAN2,0,0,&config)==1) {
		printf(">>Init Device1 CAN1 success\n");
	}
	if(send && VCI_StartCAN(VCI_USBCAN2,0,0)==1) {
		printf(">>Start Device1 CAN1 success\n");
	}

    
	if(receive && VCI_InitCAN(VCI_USBCAN2,0,0,&config)==1) {
		printf(">>Init Device2 CAN1 success\n");
		//VCI_CloseDevice(VCI_USBCAN2,0);
	}
	if(receive && VCI_StartCAN(VCI_USBCAN2,0,0)==1) {
		printf(">>Start Device2 CAN1 success\n");
	}
    
    if(receive) {
		printf(">>开启接收线程\n");
        std::thread receive(&receive_func);
        receive.join();
    }
    
    if(send) {
	
        std::thread send(&send_func);
        send.join();
    }
}
