#include <stm32f407zgt6_includes.h>

#define PrioTask0		1					/*任务优先级*/
#define StackSizeTask0	512					/*任务堆栈大小定义*/
uint32_t StackTask0[StackSizeTask0];		/*建立任务堆栈*/
void Task0(void);							/*任务函数*/

#define PrioTask1		2					/*任务优先级*/
#define StackSizeTask1	512					/*任务堆栈大小定义*/
uint32_t StackTask1[StackSizeTask1];		/*建立任务堆栈*/
void Task1(void);							/*任务函数*/

#define PrioTask2		3					/*任务优先级*/
#define StackSizeTask2	512					/*任务堆栈大小定义*/
uint32_t StackTask2[StackSizeTask2];		/*建立任务堆栈*/
void Task2(void);							/*任务函数*/

#define PrioTask3		4					/*任务优先级*/
#define StackSizeTask3	512					/*任务堆栈大小定义*/
uint32_t StackTask3[StackSizeTask3];		/*建立任务堆栈*/
void Task3(void);							/*任务函数*/

#define PrioTask4		4					/*任务优先级*/
#define StackSizeTask4	512					/*任务堆栈大小定义*/
uint32_t StackTask4[StackSizeTask4];		/*建立任务堆栈*/
void Task4(void);							/*任务函数*/


void board_init(void);

// main code
int main(int argc, char* argv[])
{
	board_init();
	OS_Init();
	OS_TASK_Create(Task0, &StackTask0[StackSizeTask0-1], PrioTask0);

	printf("code start\r\n");
	OS_Task_Start();
}

// board init
void board_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart_init(9600);
	
}

void Task0(void)
{
	// 创建其他需要的任务
	OS_TASK_Create(Task1, &StackTask1[StackSizeTask1-1], PrioTask1);
	OS_TASK_Create(Task2, &StackTask2[StackSizeTask2-1], PrioTask2);
	OS_TASK_Create(Task3, &StackTask3[StackSizeTask3-1], PrioTask3);
	// OS_TASK_Create(Task4, &StackTask4[StackSizeTask4-1], PrioTask4);
	while(1)
	{
		printf("Task 0 \r\n");
		OS_Task_TimeDly(100);
	}
}

void Task1(void)
{
	while (1)
	{
		printf("Task 1 \r\n");
		OS_Task_TimeDly(300);
	}
	
}

void Task2(void)
{
	while(1)
	{
		printf("Task2 \r\n");
		OS_Task_Supend(PrioTask2);	// 使自己进入挂起状态
	}
}
void Task3(void)
{
	while(1)
	{
		printf("Resume Task2 \r\n");
		OS_Task_Resume(PrioTask2);	// 恢复任务2
		OS_Task_TimeDly(800);
	}
}

void Task4(void)
{
	while(1)
	{
		printf("Task4 begin \r\n");
		// OS_Task_Supend(PrioTask4);	//test		
		// OS_Task_Switch();			//当使用OS_Task_Switch这个而不是OS_Task_TimeDly时程序并不会卡死
		// OS_Task_TimeDly(300);		//程序会卡死
	}
}

void SysTick_Handler(void)
{
    static uint8_t i;
    static uint16_t iCount=0;
    iCount++;
    if(iCount==500) {
        iCount=0; // while os_tick is 100, record 5 s
        // printf("5s has got\r\n");
    }

    for(i=0;i<OS_TASK_NUM+1;i++) {
        if(TCB[i].OS_TCB_Dly)
        {
			// printf("TCB[%d].OS_TCB_Dly=%d\r\n", i, TCB[i].OS_TCB_Dly);
            TCB[i].OS_TCB_Dly--;
            if(TCB[i].OS_TCB_Dly==0)
            {
                OS_SetPrioRdy(i);
            }
        }
    }
}
