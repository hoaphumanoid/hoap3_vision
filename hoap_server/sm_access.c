/*
 共有メモリ書き込み/読み込み関数
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MemMan.h"
#include "sm_access.h"

#define SEPARATOR " ,\t\n"

/*
 文字列を解析してデータを抽出し、パラメータで与えられた共有メモリの
 バッファにデータを書き込む。
 共有メモリの中でデータを書き込み、結果を読み出すバッファは、
   シーケンスモードでは、Data.RepDat
   一般コマンドモードでは、Data.IntDat
   シーケンスモードでは、Data.SeqDat
 を使用する。
*/

int send_sm(char* buf, DUNIT* unit)
{
	int Step, Header;
	int d_len = 0;
	
	if(!unit) return -1;

	/*次に実行されるまでの時間間隔*/
	if((d_len = getint(buf, &Step)) < 1) {
		printf("parameter Step error\n");
		return -1;
	}
	buf = buf + d_len;
//	printf("Step: %hu\n", (unsigned short)Step);

	/*コマンドの種類(一般コマンドor定型コマンド)*/
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
			/*定型コマンド*/
			int i;
			int MtrS[MotorMax];
			char SnsS[SensorMax];

			/*20個のモータへの送信データ*/
			for(i = 0; i < MotorMax; i++) {
				if((d_len = getint(buf, &MtrS[i])) < 1) {
					printf("parameter MtrS error\n");
					return -1;
				}
				buf = buf + d_len;
			}
			/*3個のモータへの送信データ*/
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
			/*一般コマンド:個々のデバイスにコマンドを送ることができる*/
			int DevID;
			char CmdAsc;
			short ArgB[ArgMax];
			short ArgW[ArgMax/2];
//			int ArgL[ArgMax/4];
			
			/*コマンドを送るデバイスID*/
			if((d_len = getint(buf, &DevID)) < 1) {
				printf("parameter DevID error\n");
				return -1;
			}
			buf = buf + d_len;
//			printf("DevID: %hu\n", (unsigned short)DevID);

			/*コマンド(A-z)*/
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

			/*コマンドによってその後に続くパラメータの数や種類が異なる*/
			switch (CmdAsc) {
			case 'R': 
				break; 
			case 'S': /*ucharデータ一個*/
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
				/*ushortデータ一個*/
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
				/*ushortデータ2個*/
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
			case 'k': /*ushortデータ一個*/
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
			/*必要なデータが揃ったら、共有メモリに書き込む*/
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
	unit->StepOver = 0; /*時間内に終了しなかったコマンドの数をカウントするカウンタをクリアする*/
	unit->ResTime = 0; /*レスポンスタイムをクリアする。これに数字が入ったら実行完了で結果が共有メモリに入っている。*/

	return 0;
}

/*文字列の先頭からセパレータまでの文字列をintに変換する*/
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

/*共有メモリの内容を読み出す*/
int read_sm(DUNIT *unit)
{
	int i, j;

	/*次の実行時間までの時間間隔*/
	printf("Step = %d\n", unit->Step);
	/*コマンドの型を指定する(一般コマンドor定型コマンド)*/
	printf("Header = %d\n", unit->Header);

	switch(unit->Header) {
	case 1:
	case 2:
		/*定型コマンド：全てのモータとセンサに指令を送る*/
		/*各モータへ送られた指令値*/
		printf("MtrS = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrS[i]);
		}
		printf("\n");
		/*各センサへ送られた指令値*/
		printf("SnsS = ");
		for(i = 0; i < SensorMax; i++) {
			printf("%c ", (char)unit->Ctl.SnsS[i]);
		}
		printf("\n");
		/*戻り値：各モータのポジションデータ*/
		/*上の指令送信完了していれば結果が入っている*/
		printf("MtrRp = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRp[i]);
		}
		printf("\n");
		/*戻り値：各モータの速度データ*/
		/*上の指令送信完了していれば結果が入っている*/
		printf("MtrRv = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRv[i]);
		}
		printf("\n");
		/*戻り値：各モータの速度データ*/
		/*上の指令送信完了していれば結果が入っている*/
		printf("MtrRi = ");
		for(i = 0; i < MotorMax; i++) {
			printf("%d ", (short)unit->Ctl.MtrRi[i]);
		}
		printf("\n");
		/*戻り値：各センサのデータ*/
		/*上の指令送信完了していれば結果が入っている*/
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

	case 3: /*一般コマンド*/
		/*コマンドを送ったデバイスID*/
		printf("Cmd.DevID = %hd\n", unit->Cmd.DevID);
		/*コマンド*/
		printf("Cmd.CmdAsc = %c\n", unit->Cmd.CmdAsc);

		switch(unit->Cmd.CmdAsc) {
		/*コマンドによって戻り値が異なる*/
		case 'R': 
			/*6個のushort*/
			printf("ResW = ");
			for(i = 0; i < 6; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("ResB = %hd\n", unit->Cmd.ResB[12]);
			break;	
				
		case 'S':
			/*一個のuchar*/
			printf("ArgB = %hd\n", unit->Cmd.ArgB[0]);
			break;

		case 'X':
			/*6個のuchar*/
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
			/*一個のushort*/
			printf("ArgW = %hd\n", (short)unit->Cmd.ArgW[0]);
			break;
		
		case 'N':
			/*一個のushort*/
			printf("ArgW = %hd, %hd\n", (short)unit->Cmd.ArgW[0], (short)unit->Cmd.ArgW[1]);
			break;
	
		case 'a':
		case 'd':
			/*3個のushort*/
			printf("ResW = ");
			for(i = 0; i < 3; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("\n");
			break;	
			
		case 'e':
		case 'h':
		case 'k':
			/*3個のushortと1個のuchar*/
			printf("ResW = ");
			for(i = 0; i < 3; i++) {
				printf("%hd ", (short)unit->Cmd.ResW[i]);
			}
			printf("\n");
			printf("ArgW = %hd\n", (short)unit->Cmd.ArgW[0]);
			break;	
			
		case 'n':
			/*1個のushort*/
			printf("ResW = %hu\n", unit->Cmd.ResW[0]);
			break;	

		default:
			break;	
		}
		/*時間内に処理が終らず遅れが出たコマンドの個数*/
		printf("StepOver = %d\n", unit->StepOver);
		/*レスポンスタイム*/
		printf("ResTime = %d\n", unit->ResTime);
	}

	return 0;
}

