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
//NTP服务器列表
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
		case WSASYSNOTREADY:cout << "基础网络子系统尚未准备好进行网络通信。" << endl; break;
		case WSAVERNOTSUPPORTED:cout << "此特定的Windows套接字实现未提供所请求的Windows套接字支持的版本。" << endl; break;
		case WSAEINPROGRESS:cout << "Windows Sockets 1.1的阻止操作正在进行中。" << endl; break;
		case WSAEPROCLIM:cout << "Windows套接字实现所支持的任务数已达到限制。" << endl; break;
		case WSAEFAULT:cout << "lpWSAData 参数不是有效的指针。" << endl; break;
		default:break;
		}
		return false;
	}
	else
	{
		cout << "WSA服务已开启" << endl;
		return true;
	}
}
int main()
{
	while (!FindWindowA("Valve001", 0))
	{
		static int retry = 0;
		cout << "没有找到CSGO窗口! [" << ++retry << "]" << endl;
		Sleep(5000);
	}
	HWND m_hEngine = FindWindowA("Valve001", 0);
	//启用网络套接字功能
	WSADATA xwsa_data;
	if (!OpenWSA(xwsa_data))
	{
		cout << "遇到错误即将退出!" << endl;
		system("pause");
		exit(1);
	}
	//初始化NTP工作对象
	xntp_cliptr_t xntp_this = X_NULL;
	xntp_this = ntpcli_open();//初始化
	if (X_NULL == xntp_this)//判断是否初始化成功
	{
		cout << "初始化NTP工作对象错误:" << errno << endl;
		system("pause");
		exit(1);
	}
	cout << "已初始化NTP工作对象" << endl;
	//设置信息
	int host = 0;
	ntpcli_config(xntp_this, xszt_host[host], NTP_PORT);
	//发送请求
	cout << "正在发送NTP请求..." << endl;
	xtime_vnsec_t xtm_vnsec = ntpcli_req_time(xntp_this, 5000);//获取网络时间
	for (size_t i = 1; i <= 16; i++)
	{
		if (XTMVNSEC_IS_VALID(xtm_vnsec))
		{
			cout << "NTP请求已成功" << endl;
			xtime_vnsec_t xtm_ltime = time_vnsec();//获取系统时间
			xtime_descr_t xtm_local = time_vtod(xtm_ltime);//转换系统时间
			xtime_descr_t xtm_descr = time_vtod(xtm_vnsec);//转换网络时间
			//显示LOGO
			system("cls");
			cout << " █----------------------------------------------------------------------------------------------------------█" << endl;
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
			cout << " █----------------------------------------------------------------------------------------------------------█" << endl;
			//显示时间
			cout << "网络时间:[ " << xtm_descr.ctx_year << "年" << xtm_descr.ctx_month << "月" << xtm_descr.ctx_day << "日 星期" << xtm_descr.ctx_week << " " << xtm_descr.ctx_hour << "小时" << xtm_descr.ctx_minute << "分钟" << xtm_descr.ctx_second << "秒" << xtm_descr.ctx_msec << "毫秒 ]\n";
			cout << "系统时间:[ " << xtm_local.ctx_year << "年" << xtm_local.ctx_month << "月" << xtm_local.ctx_day << "日 星期" << xtm_local.ctx_week << " " << xtm_local.ctx_hour << "小时" << xtm_local.ctx_minute << "分钟" << xtm_local.ctx_second << "秒" << xtm_local.ctx_msec << "毫秒 ]\n";
			//时间设置
			int time = xtm_descr.ctx_second * 1000 + xtm_descr.ctx_msec;//转换成毫秒
			auto start = std::chrono::high_resolution_clock::now();//计时开始
			static unsigned int team = 0;
			while (true)
			{
				auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);//计算耗时
				register int nowtime = time + duration_ms.count();//当前时间
				if (nowtime < 60000)
				{
					if (nowtime % 1000 == 0)
					{
						team = nowtime % 8000 / 1000;
						cout << "第" << nowtime << "毫秒正在显示第" << team << "个组名" << endl;
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
			cout << "遇到错误:" << errno << "NTP服务器IP:" << xszt_host[host++] << endl;
			cout << "稍后重试[" << i << "]..." << endl;
			Sleep(5000);
		}
	}
	cout << "发生严重错误!!!请检查网络或重启应用程序!" << endl;
	return 0;
}