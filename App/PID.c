
# include "PID.h"

/**********************����*******************************/

typedef struct PID
{
 int SetPoint; //�趨Ŀ�� Desired Value
 double Proportion; //�������� Proportional Const
 double Integral; //���ֳ��� Integral Const
 double Derivative; //΢�ֳ��� Derivative Const
 int LastError; //Error[-1]
 int PrevError; //Error[-2]
} PID;

static PID sPID;
static PID *sptr = &sPID; 


/**********************����*******************************/

void IncPIDInit(void)
{
   sptr->LastError = 0; //Error[-1]
   sptr->PrevError = 0; //Error[-2]
   sptr->Proportion = P_DATA; //�������� Proportional Const
   sptr->Integral = I_DATA; //���ֳ��� Integral Const
   sptr->Derivative = D_DATA; //΢�ֳ��� Derivative Const
   sptr->SetPoint =100; //Ŀ���� 100
}

int g_CurrentVelocity;
int g_Flag;
//*****************************************************
//����ʽ PID �������
//*****************************************************

int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid; //��ǰ���
   iError = sptr->SetPoint - NextPoint; //��������
   iIncpid = (int)(sptr->Proportion * iError - sptr->Integral * sptr->LastError + sptr->Derivative * sptr->PrevError);
   sptr->PrevError = sptr->LastError; //�洢�������´μ���
   sptr->LastError = iError;
   return(iIncpid); //��������ֵ
   }

