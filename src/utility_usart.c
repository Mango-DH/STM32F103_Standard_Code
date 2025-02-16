#include "utility_usart.h"

//#pragma import(__use_no_semihosting)

//struct __FILE
//{
//	int a;
//};
// 
//FILE __stdout;
//void _sys_exit(int x)
//{
//	
//}

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
uint8_t data_to_send[64];



void Serial_Init(uint16_t bound)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}


void UART_Queue_Init(Queue* uartQueue, uint8_t* uartBuffer,uint8_t buffer_size) 
{
    Queue_Init(uartQueue, uartBuffer, sizeof(uint8_t), buffer_size);
}

size_t UART_Get_Received_Bytes_Count(Queue* uartQueue) 
{
    return uartQueue->count;
}


/**
* @name:  UART_Read_Bytes
* @brief: 队列作为数据缓存区，接收中断数据
* @param: Queue* uartQueue    队列指针
* @reval: MY_OK 				操作成功
*         MY_ERR_INVALID_PARAM 	 无效参数错误
**/
My_Err_Type UART_Read_Bytes(Queue* uartQueue) 
{	
	  int dequeued_value = 0;
		if (Serial_GetRxFlag() == 1)    //触发中断打印队列里接收到的数据
		{
			//printf("Queue Count: %zu\n", uartQueue->count);        //打印队列数据数量
				while (Queue_Dequeue(uartQueue, &dequeued_value)) 
				{
						printf("0x%02X\n", dequeued_value);  //Dequeued value
				}				
				//				Serial_RxData = Serial_GetRxData() ;        //江协缓冲区
				//				Serial_SendByte(Serial_RxData) ;
		}			
    return MY_OK;  // 成功读取指定数量的字节
}


/**
 * @name: Protocol_Rcv
 * @brief:逐字节接收数据并处理校验和
 * @param:uint8_t input                       逐个字节输入
 * @param:uint8_t* output                     最后的输出
 * @param:uint8_t* Len                        数据包长度
 **/
void Protocol_Rcv(uint8_t input, uint8_t* output, uint8_t* Len)
{
    static RcvState state = WAIT_FOR_HEADER;    //state：保存当前的状态
    static uint8_t length = 0;                  //length：保存接收到的数据包长度
    static uint8_t dataIndex = 0;               //dataIndex：当前接收到的数据索引
    static uint8_t checksum = 0;                //checksum：累加求和校验和

    switch (state) {
        case WAIT_FOR_HEADER:
            if (input == 0xEF) {
                state = WAIT_FOR_LENGTH;
                checksum = 0;  // 重置校验和
            }
            break;

        case WAIT_FOR_LENGTH:
            length = input;
            checksum += input;
            dataIndex = 0;
            state = WAIT_FOR_DATA;
            break;

        case WAIT_FOR_DATA:
            output[dataIndex++] = input;
            checksum += input;

            if (dataIndex >= length) {
                state = WAIT_FOR_CHECKSUM;
            }
            break;

        case WAIT_FOR_CHECKSUM:
            if (checksum == input) {   //输入的是校验和
                *Len = length;  // 更新数据包长度
            } else {
                *Len = 0;       // 校验和不正确
            }

            // 重置状态机
            state = WAIT_FOR_HEADER;
            break;

        default:
            state = WAIT_FOR_HEADER;
            break;
    }
}


/**
 * @name: Protocol_Rcv_Test
 * @brief: 检验 Protocol_Rcv()的正确性
 **/
void Protocol_Rcv_Test()
{
    // 模拟接收的数据包: 0xEF 0x03 0xA1 0xB2 0xC3 校验和 0x19
    uint8_t simulatedInput[] = {0xEF, 0x03, 0xA1, 0xB2, 0xC3, 0x19};
    uint8_t output[128] = {0};
    uint8_t len = 0;

    for (int i = 0; i < sizeof(simulatedInput); i++) {
        Protocol_Rcv(simulatedInput[i], output, &len);
    }

    if (len > 0) {
        printf("Received data (len=%d):\n", len);
        for (int i = 0; i < len; i++) {
            printf("0x%02X ", output[i]);
        }
        printf("\n");
    } else {
        printf("Checksum error or incomplete packet.\n");
    }
}


typedef union {
    float float_val;
    uint32_t uint_val;
} FloatUnion;


uint32_t data_ptr;
/**
 * @name:  Vofa_Wave_Print
 * @brief: 实现VOFA的Justfloat协议
 * @param: float* data_arr                   数据数组
 * @param: uint8_t channel                   发送通道
 **/
void Vofa_Wave_Print(float* data_arr, uint8_t channel) 
{	
	  FloatUnion converter;
    converter.float_val = data_arr[channel];
	
    uint8_t buffer[8];
    buffer[0] = (converter.uint_val >> 0)  & 0xFF;
    buffer[1] = (converter.uint_val >> 8)  & 0xFF;
    buffer[2] = (converter.uint_val >> 16) & 0xFF;
    buffer[3] = (converter.uint_val >> 24) & 0xFF;  

    // 发送数据包
    Serial_SendArray(buffer, 4);
}
