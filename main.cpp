#include "ntp_client.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <errno.h>
#include <chrono>
#include <thread>
#include <Windows.h>
using namespace std;
static const string str[] = { "echo test","cl_clanid 43212735","cl_clanid 43212734","cl_clanid 43212733","cl_clanid 43212732","cl_clanid 43212731","cl_clanid 43212729","cl_clanid 43212726" };
//NTP�������б�
x_cstring_t xszt_host[] = {
	"ntp.tencent.com" ,
	"ntp1.tencent.com",
	"ntp2.tencent.com",
	"ntp3.tencent.com",
	"ntp4.tencent.com",
	"ntp5.tencent.com",
	"ntp1.aliyun.com" ,
	"ntp2.aliyun.com" ,
	"ntp3.aliyun.com" ,
	"ntp4.aliyun.com" ,
	"ntp5.aliyun.com" ,
	"ntp6.aliyun.com" ,
	"ntp7.aliyun.com" ,
	"time.edu.cn"     ,
	"s2c.time.edu.cn" ,
	"s2f.time.edu.cn" ,
	"s2k.time.edu.cn" ,
	X_NULL };
[[nodiscard]] bool OpenWSA(WSADATA& xwsa_data)
{
	int nRes = WSAStartup(MAKEWORD(2, 0), &xwsa_data);
	if (nRes != 0)
	{
		switch (nRes)
		{
		case WSASYSNOTREADY:cout << "����������ϵͳ��δ׼���ý�������ͨ�š�" << endl; break;
		case WSAVERNOTSUPPORTED:cout << "���ض���Windows�׽���ʵ��δ�ṩ�������Windows�׽���֧�ֵİ汾��" << endl; break;
		case WSAEINPROGRESS:cout << "Windows Sockets 1.1����ֹ�������ڽ����С�" << endl; break;
		case WSAEPROCLIM:cout << "Windows�׽���ʵ����֧�ֵ��������Ѵﵽ���ơ�" << endl; break;
		case WSAEFAULT:cout << "lpWSAData ����������Ч��ָ�롣" << endl; break;
		default:break;
		}
		return false;
	}
	else
	{
		cout << "WSA�����ѿ���" << endl;
		return true;
	}
}
int main()
{
	while (!FindWindowA("Valve001", 0))
	{
		static int retry = 0;
		cout << "û���ҵ�CSGO����! [" << ++retry << "]" << endl;
		Sleep(5000);
	}
	HWND m_hEngine = FindWindowA("Valve001", 0);
	//���������׽��ֹ���
	WSADATA xwsa_data;
	if (!OpenWSA(xwsa_data))
	{
		cout << "�������󼴽��˳�!" << endl;
		system("pause");
		exit(1);
	}
	//��ʼ��NTP��������
	xntp_cliptr_t xntp_this = X_NULL;
	xntp_this = ntpcli_open();//��ʼ��
	if (X_NULL == xntp_this)//�ж��Ƿ��ʼ���ɹ�
	{
		cout << "��ʼ��NTP�����������:" << errno << endl;
		system("pause");
		exit(1);
	}
	cout << "�ѳ�ʼ��NTP��������" << endl;
	//������Ϣ
	int host = 0;
	ntpcli_config(xntp_this, xszt_host[host], NTP_PORT);
	//��������
	cout << "���ڷ���NTP����..." << endl;
	xtime_vnsec_t xtm_vnsec = ntpcli_req_time(xntp_this, 5000);//��ȡ����ʱ��
	for (size_t i = 1; i <= 16; i++)
	{
		if (XTMVNSEC_IS_VALID(xtm_vnsec))
		{
			cout << "NTP�����ѳɹ�" << endl;
			xtime_vnsec_t xtm_ltime = time_vnsec();//��ȡϵͳʱ��
			xtime_descr_t xtm_local = time_vtod(xtm_ltime);//ת��ϵͳʱ��
			xtime_descr_t xtm_descr = time_vtod(xtm_vnsec);//ת������ʱ��
			//��ʾLOGO
			system("cls");
			cout << " ��----------------------------------------------------------------------------------------------------------��" << endl;
			cout << " |          _____                      _____                      _____                      _____          |" << endl;
			cout << " |         /\\    \\                    /\\    \\                    /\\    \\                    /\\    \\         |" << endl;
			cout << " |        /||\\    \\                  /||\\    \\                  /||\\____\\                  /||\\    \\        |" << endl;
			cout << " |       /||||\\    \\                /||||\\    \\                /|||||   |                 /||||\\    \\       |" << endl;
			cout << " |      /||||||\\    \\              /||||||\\    \\              /||||||   |                /||||||\\    \\      |" << endl;
			cout << " |     /|||/\\|||\\    \\            /|||/\\|||\\    \\            /|||||||   |               /|||/\\|||\\    \\     |" << endl;
			cout << " |    /|||/__\\|||\\    \\          /|||/  \\|||\\    \\          /|||/||||   |              /|||/  \\|||\\    \\    |" << endl;
			cout << " |    \\|||\\   \\|||\\    \\        /|||/    \\|||\\    \\        /|||/ ||||   |             /|||/    \\|||\\    \\   |" << endl;
			cout << " |  ___\\|||\\   \\|||\\    \\      /|||/    / \\|||\\    \\      /|||/  ||||___|______      /|||/    / \\|||\\    \\  |" << endl;
			cout << " | /\\   \\|||\\   \\|||\\    \\    /|||/    /   \\|||\\    \\    /|||/   |||||||||\\    \\    /|||/    /   \\|||\\ ___\\ |" << endl;
			cout << " |/||\\   \\|||\\   \\|||\\____\\  /|||/____/     \\|||\\____\\  /|||/    ||||||||||\\____\\  /|||/____/     \\||||   | |" << endl;
			cout << " |\\|||\\   \\|||\\   \\||/    /  \\|||\\    \\      \\||/    /  \\||/    / -----/|||/    /  \\|||\\    \\     /||||___| |" << endl;
			cout << " | \\|||\\   \\|||\\   \\/____/    \\|||\\    \\      \\/____/    \\/____/      /|||/    /    \\|||\\    \\   /|||/    / |" << endl;
			cout << " |  \\|||\\   \\|||\\    \\         \\|||\\    \\                            /|||/    /      \\|||\\    \\ /|||/    /  |" << endl;
			cout << " |   \\|||\\   \\|||\\____\\         \\|||\\    \\                          /|||/    /        \\|||\\    /|||/    /   |" << endl;
			cout << " |    \\|||\\  /|||/    /          \\|||\\    \\                        /|||/    /          \\|||\\  /|||/    /    |" << endl;
			cout << " |     \\|||\\/|||/    /            \\|||\\    \\                      /|||/    /            \\|||\\/|||/    /     |" << endl;
			cout << " |      \\||||||/    /              \\|||\\    \\                    /|||/    /              \\||||||/    /      |" << endl;
			cout << " |       \\||||/    /                \\|||\\____\\                  /|||/    /                \\||||/    /       |" << endl;
			cout << " |        \\||/    /                  \\||/    /                  \\||/    /                  \\||/    /        |" << endl;
			cout << " |         \\/____/                    \\/____/                    \\/____/                    \\/____/         |" << endl;
			cout << " ��----------------------------------------------------------------------------------------------------------��" << endl;
			//��ʾʱ��
			cout << "����ʱ��:[ " << xtm_descr.ctx_year << "��" << xtm_descr.ctx_month << "��" << xtm_descr.ctx_day << "�� ����" << xtm_descr.ctx_week << " " << xtm_descr.ctx_hour << "Сʱ" << xtm_descr.ctx_minute << "����" << xtm_descr.ctx_second << "��" << xtm_descr.ctx_msec << "���� ]\n";
			cout << "ϵͳʱ��:[ " << xtm_local.ctx_year << "��" << xtm_local.ctx_month << "��" << xtm_local.ctx_day << "�� ����" << xtm_local.ctx_week << " " << xtm_local.ctx_hour << "Сʱ" << xtm_local.ctx_minute << "����" << xtm_local.ctx_second << "��" << xtm_local.ctx_msec << "���� ]\n";
			//ʱ������
			int time = xtm_descr.ctx_second * 1000 + xtm_descr.ctx_msec;//ת���ɺ���
			auto start = std::chrono::high_resolution_clock::now();//��ʱ��ʼ
			static unsigned int team = 0;
			while (true)
			{
				auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);//�����ʱ
				register int nowtime = time + duration_ms.count();//��ǰʱ��
				if (nowtime < 60000)
				{
					if (nowtime % 1000 == 0)
					{
						team = nowtime % 8000 / 1000;
						cout << "��" << nowtime << "����������ʾ��" << team << "������" << endl;
						COPYDATASTRUCT m_cData;
						m_cData.dwData = 0;
						m_cData.cbData = strlen(str[team].c_str()) + 1;
						m_cData.lpData = (void*)str[team].c_str();
						SendMessageA(m_hEngine, WM_COPYDATA, 0, (LPARAM)&m_cData);
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
				}
				else
				{
					start = std::chrono::high_resolution_clock::now();
					time = 0;
				}
			}

			break;
		}
		else
		{
			cout << "��������:" << errno << "NTP������IP:" << xszt_host[host++] << endl;
			cout << "�Ժ�����[" << i << "]..." << endl;
			Sleep(5000);
		}
	}
	cout << "�������ش���!!!�������������Ӧ�ó���!" << endl;
	return 0;
}