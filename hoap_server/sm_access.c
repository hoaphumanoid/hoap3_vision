/*
 ��ͭ����񤭹���/�ɤ߹��ߴؿ�
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MemMan.h"
#include "sm_access.h"

#define SEPARATOR " ,\t\n"

/*
 ʸ�������Ϥ��ƥǡ�������Ф����ѥ�᡼����Ϳ����줿��ͭ�����
 �Хåե��˥ǡ�����񤭹��ࡣ
 ��ͭ�������ǥǡ�����񤭹��ߡ���̤��ɤ߽Ф��Хåե��ϡ�
   �������󥹥⡼�ɤǤϡ�Data.RepDat
   ���̥��ޥ�ɥ⡼�ɤǤϡ�Data.IntDat
   �������󥹥⡼�ɤǤϡ�Data.SeqDat
 ����Ѥ��롣
*/

int send_sm(char* buf, DUNIT* unit)
{
	int Step, Header;
	int d_len = 0;
	
	if(!unit) return -1;

	/*���˼¹Ԥ����ޤǤλ��ֳִ�*/
	if((d_len = getint(buf, &Step)) < 1) {
		printf("parameter Step error\n");
		return -1;
	}
	buf = buf + d_len;
//	printf("Step: %hu\n", (unsigned short)Step);

	/*���ޥ�ɤμ���(���̥��ޥ��or�귿���ޥ��)*/
	if((d_len = getint(buf, &Header)) < 1) {
		printf("parameter Header error\n");
		return -1;
	}
	buf = buf + d_len;
//	printf("Header: %hu\n",(unsigned short)Header);

	switch(Header) {
	case 1:
	case 2:
		{
			/*�귿���ޥ��*/
			int i;
			int MtrS[MotorMax];
			char SnsS[SensorMax];

			/*20�ĤΥ⡼���ؤ������ǡ���*/
			for(i = 0; i < MotorMax; i++) {
				if((d_len = getint(buf, &MtrS[i])) < 1) {
					printf("parameter MtrS error\n");
					return -1;
				}
				buf = buf + d_len;
			}
			/*3�ĤΥ⡼���ؤ������ǡ���*/
			for(i = 0; i < SensorMax; i++) {
				while(strchr(SEPARATOR, buf[0]) != NULL) {
					if(buf[0] == '\0') {
						printf("parameter SnsS error\n");
						return -1;
					}
					buf++;
				}
				SnsS[i] =buf[0];
				buf++;
			}
			
//			printf("MtrS = ");
			for(i = 0; i < MotorMax; i++) {
				unit->Ctl.MtrS[i] = (unsigned short)MtrS[i];
//				printf("%hd ", (short)unit->Ctl.MtrS[i]);
			}
//			printf("\nSnsS = ");
			for(i = 0; i < SensorMax; i++) {
				unit->Ctl.SnsS[i] = (unsigned char)SnsS[i];
//				printf("%c ", (char)unit->Ctl.SnsS[i]);
			}
//			printf("\n");
			break;
		}
	case 3:
		{
			/*���̥��ޥ��:�ġ��ΥǥХ����˥��ޥ�ɤ����뤳�Ȥ��Ǥ���*/
			int DevID;
			char CmdAsc;
			short ArgB[ArgMax];
			short ArgW[ArgMax/2];
//			int ArgL[ArgMax/4];
			
			/*���ޥ�ɤ�����ǥХ���ID*/
			if((d_len = getint(buf, &DevID)) < 1) {
				printf("parameter DevID error\n");
				return -1;
			}
			buf = buf + d_len;
//			printf("DevID: %hu\n", (unsigned short)DevID);

			/*���ޥ��(A-z)*/
			while(strchr(SEPARATOR, buf[0]) != NULL) {
				if(buf[0] == '\0') {
					printf("parameter CmdAsc error\n");
					return -1;
				}
				buf++;
			}
			CmdAsc =buf[0];
//			printf("CmdAsc: %c\n",CmdAsc);
			buf= buf + 2;

			/*���ޥ�ɤˤ�äƤ��θ��³���ѥ�᡼���ο�����ब�ۤʤ�*/
			switch (CmdAsc) {
			case 'R': 
				break; 
			case 'S': /*uchar�ǡ������*/
				if(sscanf(buf, "%hd", &ArgB[0]) != 1) {
					printf("parameter ArgB error\n");
					return -1;
				}
				unit->Cmd.ArgB[0] = (unsigned char)ArgB[0];
//				printf("ArgB[0]: %hd\n", unit->Cmd.ArgB[0]);
				break;
			case 'X':
				break;
			case 'Y':
				break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'H':
			case 'L':
				/*ushort�ǡ������*/
//				printf("%s\n", buf);
				if((sscanf(buf, "%hd", &ArgW[0])) != 1) {
				printf("parameter ArgW error\n");
				return -1;
				}
				unit->Cmd.ArgW[0] = (unsigned short)ArgW[0];
//				printf("ArgW[0]: %hd\n", unit->Cmd.ArgW[0]);
				break;
			case 'I':
			case 'J':
				break;
			case 'N': 
				/*ushort�ǡ���2��*/
				if(sscanf(buf, "%hd, %hd", &ArgW[0], &ArgW[1]) != 2) {
					printf("parameter ArgW error\n");
					return -1;
				}
				unit->Cmd.ArgW[0] = (unsigned short)ArgW[0];
				unit->Cmd.ArgW[1] = (unsigned short)ArgW[1];
//				printf("ArgW[0]: %hd, ArgW[1]: %hd\n", (short)unit->Cmd.ArgW[0], (short)unit->Cmd.ArgW[1]);
				break;
			case 'a':
			case 'd':
				break;
			case 'e':
			case 'h':
			case 'k': /*ushort�ǡ������*/
				if(sscanf(buf, "%hd", &ArgW[0]) != 1) {
					printf("parameter ArgW error\n");
					return -1;
				}	
				unit->Cmd.ArgW[0] = (unsigned short)ArgW[0];
//				printf("ArgW[0]: %hd\n", unit->Cmd.ArgW[0]);
			case 'n':
			case 'o':
				break;

			case 's':
			case 't':
			case 'u':
			case 'v':
				if(sscanf(buf, "%hd", &ArgW[0]) != 1) {
					printf("parameter ArgW error\n");
					return -1;
				}	
				unit->Cmd.ArgW[0] = (unsigned short)ArgW[0];
//				printf("ArgW[0]: %hd\n", unit->Cmd.ArgW[0]);
				break;
			default:
				printf("paramter CmdAsc error\n");
				return -1;
			}
			/*ɬ�פʥǡ�����·�ä��顢��ͭ����˽񤭹���*/
			unit->Cmd.DevID = (unsigned char)DevID;
//		 	printf("write DevID %d\n", unit->Cmd.DevID);
			unit->Cmd.CmdAsc = (unsigned char)CmdAsc;
//		 	printf("write CmdAsc %c\n", unit->Cmd.CmdAsc);
		}
		break;
	default:
		return -1;
	}

	unit->Step = (unsigned short)Step;
// 	printf("write Step %d\n", unit->Step);
	unit->Header = (unsigned char)Header;
// 	printf("write Header %d\n", unit->Header);
	unit->StepOver = 0; /*������˽�λ���ʤ��ä����ޥ�ɤο��򥫥���Ȥ��륫���󥿤򥯥ꥢ����*/
	unit->ResTime = 0; /*�쥹�ݥ󥹥�����򥯥ꥢ���롣����˿��������ä���¹Դ�λ�Ƿ�̤���ͭ��������äƤ��롣*/

	return 0;
}

/*ʸ�������Ƭ���饻�ѥ졼���ޤǤ�ʸ�����int���Ѵ�����*/
int getint(char* buf, int *x)
{
	char c;
	int i = 0;
	char s[83] = {0};
	char* chk = NULL;
	int n = 0;

//	printf("%s\n", buf);
	while((chk == NULL) || (i == 0)) {
		c = buf[n];
		if(c == '\0' || c == EOF) break;
		if((chk = strchr(SEPARATOR, c)) == NULL) {
//			printf("%c", c);
			s[i] = c;
			i++;
		}
		n++;
	}
	if(i > 0) {
		*x = atoi(s);
//		printf("int %d, buf+%d\n", *x, n);
	}

	return n;
}

/*��ͭ��������Ƥ��ɤ߽Ф�*/
int read_sm(DUNIT *unit)
{
	int i, j;

	/*���μ¹Ի��֤ޤǤλ��ֳִ�*/
	printf("Step = %d\n", unit->Step);
	/*���ޥ�ɤη�����ꤹ��(���̥��ޥ��or�귿���ޥ��)*/
	printf("Header = %d\n", unit->Header);

	switch(unit->Header) {
	case 1:
	case 2:
		/*�귿���ޥ�ɡ����ƤΥ⡼���ȥ��󥵤˻��������*/
		/*�ƥ⡼��������줿������*/
		printf("MtrS = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrS[i]);
		}
		printf("\n");
		/*�ƥ��󥵤�����줿������*/
		printf("SnsS = ");
		for(i = 0; i < SensorMax; i++) {
			printf("%c ", (char)unit->Ctl.SnsS[i]);
		}
		printf("\n");
		/*����͡��ƥ⡼���Υݥ������ǡ���*/
		/*��λ���������λ���Ƥ���з�̤����äƤ���*/
		printf("MtrRp = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRp[i]);
		}
		printf("\n");
		/*����͡��ƥ⡼����®�٥ǡ���*/
		/*��λ���������λ���Ƥ���з�̤����äƤ���*/
		printf("MtrRv = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRv[i]);
		}
		printf("\n");
		/*����͡��ƥ⡼����®�٥ǡ���*/
		/*��λ���������λ���Ƥ���з�̤����äƤ���*/
		printf("MtrRi = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRi[i]);
		}
		printf("\n");
		/*����͡��ƥ��󥵤Υǡ���*/
		/*��λ���������λ���Ƥ���з�̤����äƤ���*/
		printf("SnsRB = ");
		for(i = 0; i < SensorMax; i++) {
			for(j = 0; j < SnsResMax; j++) {
				printf("%x ", unit->Ctl.SnsRB[i][j]);
			}
			printf("\n");
		}
		printf("ResTime = %d\n", unit->ResTime);

		printf("\n");
		break;

	case 3: /*���̥��ޥ��*/
		/*���ޥ�ɤ����ä��ǥХ���ID*/
		printf("Cmd.DevID = %hd\n", unit->Cmd.DevID);
		/*���ޥ��*/
		printf("Cmd.CmdAsc = %c\n", unit->Cmd.CmdAsc);

		switch(unit->Cmd.CmdAsc) {
		/*���ޥ�ɤˤ�ä�����ͤ��ۤʤ�*/
		case 'R': 
			/*6�Ĥ�ushort*/
			printf("ResW = ");
			for(i = 0; i < 6; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("ResB = %hd\n", unit->Cmd.ResB[12]);
			break;	
				
		case 'S':
			/*��Ĥ�uchar*/
			printf("ArgB = %hd\n", unit->Cmd.ArgB[0]);
			break;

		case 'X':
			/*6�Ĥ�uchar*/
			printf("ResB = ");
			for(i = 0; i < 6; i++) {
				printf("%hd ", unit->Cmd.ResB[i]);
			}
			printf("\n");
			break;	

		case 'A': 
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'L':
		case 'H':
			/*��Ĥ�ushort*/
			printf("ArgW = %hd\n", (short)unit->Cmd.ArgW[0]);
			break;
		
		case 'N':
			/*��Ĥ�ushort*/
			printf("ArgW = %hd, %hd\n", (short)unit->Cmd.ArgW[0], (short)unit->Cmd.ArgW[1]);
			break;
	
		case 'a':
		case 'd':
			/*3�Ĥ�ushort*/
			printf("ResW = ");
			for(i = 0; i < 3; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("\n");
			break;	
			
		case 'e':
		case 'h':
		case 'k':
			/*3�Ĥ�ushort��1�Ĥ�uchar*/
			printf("ResW = ");
			for(i = 0; i < 3; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("\n");
			printf("ArgW = %hd\n", (short)unit->Cmd.ArgW[0]);
			break;	
			
		case 'n':
			/*1�Ĥ�ushort*/
			printf("ResW = %hu\n", unit->Cmd.ResW[0]);
			break;	

		default:
			break;	
		}
		/*������˽��������餺�٤줬�Ф����ޥ�ɤθĿ�*/
		printf("StepOver = %d\n", unit->StepOver);
		/*�쥹�ݥ󥹥�����*/
		printf("ResTime = %d\n", unit->ResTime);
	}

	return 0;
}

