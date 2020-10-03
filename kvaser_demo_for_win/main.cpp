#include "canlib.h"
#include "candemo.h"

#include <stdio.h>
#include <thread>
#include <windows.h>
#include <cstdlib>

unsigned int m_usedBaudRate = 500000;
canHandle m_usedChannel = 0;
unsigned int m_usedId;
unsigned int m_usedFlags = 0;

driverData m_channelData;
driverData *m_DriverConfig = &m_channelData;

void receive_func(canHandle channel)
{
    unsigned int j;
    long id;
    unsigned char data[8];
    unsigned int dlc;
    unsigned int flags;
    DWORD time;
    canHandle state = canBusOn(channel);
    while (true)
    {
        canStatus stat = canRead(m_channelData.channel[channel].hnd, &id, &data[0], &dlc, &flags, &time);
        switch (stat)
        {
        case canOK:
            printf("RxMsg: Ch:%d ID:%08lx DLC:%u Flg:%02x T:%08lx Data:",
                   m_channelData.channel[channel].channel, id, dlc, flags, time);
            if ((flags & canMSG_RTR) == 0)
            {
                for (j = 0; j < dlc; j++)
                {
                    printf("%02x ", data[j]);
                }
            }
            printf("\n");
            break;

        case canERR_NOMSG:
            // No more data on this handle
            break;

        default:
            PRINTF_ERR(("ERROR canRead() FAILED, Err= %d <line: %d>\n", stat, __LINE__));
            break;
        }
    }
}

void send_func(int channel, int send_id)
{
    canHandle state = canBusOn(channel);
    while (true)
    {
        unsigned char msg[8];
        int i;
        unsigned char msgsteer[8];
        
        /*
        for (i = 0; i < 8; i++)
        {
            msg[i] = (unsigned char)(rand()%10);
            printf("%02x ", msg[i]);
        }
        */
         msg[0] = (unsigned char)(1);
         msg[1] = (unsigned char)(3);
         msg[2] = (unsigned char)(0);
         msg[3] = (unsigned char)(0);
         msg[4] = (unsigned char)(0);
         msg[5] = (unsigned char)(0);
         msg[6] = (unsigned char)(0);
         msg[7] = (unsigned char)(0);


         msgsteer[0] = (unsigned char)(1);
         msgsteer[1] = (unsigned char)(0);
         msgsteer[2] = (unsigned char)(40);
         msgsteer[3] = (unsigned char)(0);
         msgsteer[4] = (unsigned char)(0);
         msgsteer[5] = (unsigned char)(0);
         msgsteer[6] = (unsigned char)(0);
         msgsteer[7] = (unsigned char)(0);
        printf("SEND: Ch:%u ID:%08x DLC:%u Flg:%02x Data:",
               channel, 161, sizeof(msg), m_usedFlags);
          for (i = 0; i < 8; i++)
        {
            // msg[i] = (unsigned char)(rand()%10);
            printf("%02x ", msg[i]);
        }
        

        printf("\n");
        printf("SEND: Ch:%u ID:%08x DLC:%u Flg:%02x Data:",
               channel, 162, sizeof(msgsteer), m_usedFlags);
          for (i = 0; i < 8; i++)
        {
            // msg[i] = (unsigned char)(rand()%10);
            printf("%02x ", msgsteer[i]);
        }
        

        canWrite(m_DriverConfig->channel[channel].hnd, 161,
                 msg, sizeof(msg), m_usedFlags);
        canWrite(m_DriverConfig->channel[channel].hnd, 162,
                 msgsteer, sizeof(msgsteer), m_usedFlags);
        Sleep(500);
    }
}

void InitDriver()
{
    int i;
    canStatus stat;
    memset(m_channelData.channel, 0, sizeof(m_channelData.channel));
    for (i = 0; i < MAX_CHANNELS; i++)
    {
        m_channelData.channel[i].isOnBus = 0;
        m_channelData.channel[i].driverMode = canDRIVER_NORMAL;
        m_channelData.channel[i].channel = -1;
        m_channelData.channel[i].hnd = canINVALID_HANDLE;
        m_channelData.channel[i].txAck = 0; // Default is TxAck off
    }
    m_channelData.channelCount = 0;

    //
    // Enumerate all installed channels in the system and obtain their names
    // and hardware types.
    //
    //initialize CANlib
    canInitializeLibrary();

    //get number of present channels
    stat = canGetNumberOfChannels((int *)&m_channelData.channelCount);

    for (i = 0; (unsigned int)i < m_channelData.channelCount; i++)
    {
        canHandle hnd;
        //obtain some hardware info from CANlib
        m_channelData.channel[i].channel = i;
        canGetChannelData(i, canCHANNELDATA_CHANNEL_NAME,
                          m_channelData.channel[i].name,
                          sizeof(m_channelData.channel[i].name));
        canGetChannelData(i, canCHANNELDATA_CARD_TYPE,
                          &m_channelData.channel[i].hwType,
                          sizeof(DWORD));

        //open CAN channel
        hnd = canOpenChannel(i, canOPEN_ACCEPT_VIRTUAL);
        if (hnd < 0)
        {
            // error
            PRINTF_ERR(("ERROR canOpenChannel() in initDriver() FAILED Err= %d. <line: %d>\n",
                        hnd, __LINE__));
        }
        else
        {
            m_channelData.channel[i].hnd = hnd;
            if ((stat = canIoCtl(hnd, canIOCTL_FLUSH_TX_BUFFER, NULL, NULL)) != canOK)
                PRINTF_ERR(("ERROR canIoCtl(canIOCTL_FLUSH_TX_BUFFER) FAILED, Err= %d <line: %d>\n",
                            stat, __LINE__));
        }

        //set up the bus
        if (i == 0)
        {
            switch (m_usedBaudRate)
            {
            case 1000000:
                m_usedBaudRate = canBITRATE_1M;
                break;
            case 500000:
                m_usedBaudRate = canBITRATE_500K;
                break;
            case 250000:
                m_usedBaudRate = canBITRATE_250K;
                break;
            case 125000:
                m_usedBaudRate = canBITRATE_125K;
                break;
            case 100000:
                m_usedBaudRate = canBITRATE_100K;
                break;
            case 62500:
                m_usedBaudRate = canBITRATE_62K;
                break;
            case 50000:
                m_usedBaudRate = canBITRATE_50K;
                break;
            default:
                printf("Baudrate set to 125 kbit/s. \n");
                m_usedBaudRate = canBITRATE_125K;
                break;
            }
        }

        //set the channels busparameters
        stat = canSetBusParams(hnd, m_usedBaudRate, 0, 0, 0, 0, 0);
        if (stat < 0)
        {
            PRINTF_ERR(("ERROR canSetBusParams() in InitDriver(). Err = %d <line: %d>\n",
                        stat, __LINE__));
        }
    }
    printf("\n");
}

void printDriverConfig()
{
    unsigned int i;

    printf("\nDriver Configuration:\n  ChannelCount=%u\n", m_DriverConfig->channelCount);
    for (i = 0; i < m_DriverConfig->channelCount; i++)
    {

        printf("  %s : Channel %d, isOnBus=%d, Baudrate=%u index=%u ",
               m_DriverConfig->channel[i].name,
               m_DriverConfig->channel[i].channel,
               m_DriverConfig->channel[i].isOnBus,
               m_DriverConfig->channel[i].hwIndex,
               m_usedBaudRate);

        switch (m_usedBaudRate)
        {
        case canBITRATE_1M:
            printf("canBITRATE_1M");
            break;
        case canBITRATE_500K:
            printf("canBITRATE_500K");
            break;
        case canBITRATE_250K:
            printf("canBITRATE_250K");
            break;
        case canBITRATE_125K:
            printf("canBITRATE_125K");
            break;
        case canBITRATE_100K:
            printf("canBITRATE_100K");
            break;
        case canBITRATE_62K:
            printf("canBITRATE_62K");
            break;
        case canBITRATE_50K:
            printf("canBITRATE_50K");
            break;
        default:
            printf("UNKNOWN");
        }
        printf("\n    ");

        if (m_DriverConfig->channel[i].driverMode == canDRIVER_NORMAL)
        {
            printf("Drivermode=canDRIVER_NORMAL\n");
        }
        else
        {
            printf("Drivermode=canDRIVER_SILENT\n");
        }
    }

    printf("\n\n");
}

int main(int argc, char *argv[])
{

    HANDLE th[MAX_CHANNELS + 1];

    InitDriver();
    printDriverConfig();
    if (argc > 1)
    {
        std::thread send(&send_func, 0, 1);
        send.join();
    }
    else
    {
        std::thread receive(&receive_func, 0);
        receive.join();
    }

    printf("\n");
}
