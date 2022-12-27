#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <time.h>
#include <sal.h>
#include"resource.h"
#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER
#define WM_USER_SHELLICON (WM_USER + 1)

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;
NOTIFYICONDATA data;

HWND hTime, hLevel, hDialog, hText, hEnterText, hFinish;
HWND hEdit1;
HWND hButton16;
HWND hModalDialog, hResultDialog;

HICON hIcon;

HANDLE hMutex;
HANDLE hThread;

TCHAR str_time[60];
TCHAR str_result[100];
TCHAR str_text[50];
TCHAR str_timer[60];

int points = 0;
int error = 0;

TCHAR lvl1[] = TEXT("af fa ffa aaf");
TCHAR lvl2[] = TEXT("Hello world how are you?");
TCHAR lvl3[] = TEXT("You go for a walk or use your car? Do you like water?");
TCHAR str_lvl[10];

int WINAPI _wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void OnTrayIcon(WPARAM wp, LPARAM lp) {
	if (lp == WM_LBUTTONDBLCLK) {
		Shell_NotifyIcon(NIM_DELETE, pNID); // ������� ������ �� ����
		ShowWindow(hDialog, SW_NORMAL); // ��������������� ����
		SetForegroundWindow(hDialog); // ������������� ���� �� �������� ����
	}
}

void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy) {
	if (state == SIZE_MINIMIZED) {
		ShowWindow(hwnd, SW_HIDE); // ������ ����
		Shell_NotifyIcon(NIM_ADD, pNID); // ��������� ������ � ����
	}
}

DWORD WINAPI TimeFunc(LPVOID lp) {
	HWND hProgress = (HWND)lp;
	struct Time {
		int Minutes = 0;
		int Seconds = 0;
	} Time;

	while (TRUE) {
		wsprintf(str_time, TEXT("%d:%d"), Time.Minutes, Time.Seconds);
		SetWindowText(hTime, str_time);
		if (Time.Seconds >= 60) // �� ��� 60 ������ ���� ������ �� ��� �������
		{
			Time.Seconds = 00;
			Time.Minutes++;
		}
		else {
			Time.Seconds++;
		}
		Sleep(5000);//�������� 5 ������
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		hTime = GetDlgItem(hWnd, IDC_TIME);//���� �������
		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);//���� ������
		hText = GetDlgItem(hWnd, IDC_TEXT);//������ ������
		hEnterText = GetDlgItem(hWnd, IDC_ENTERTEXT);//����� ��� ����� 
		hFinish = GetDlgItem(hWnd, IDOK);//������ ������ 
		hDialog = hWnd;
		hButton16 = GetDlgItem(hWnd, IDC_BUTTON16);

		// ������� ���������� ���������� ����������
		HINSTANCE hInst = GetModuleHandle(NULL);
		pNID->cbSize = sizeof(NOTIFYICONDATA); //������ ���������
		memset(pNID, 0, sizeof(NOTIFYICONDATA)); //������� �� �� ���������
		pNID->hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); //��������� ���������������� ������
		SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // ������������� ������ � ������� ���� ����������
		pNID->hWnd = hDialog; //���������� ����, ������� ����� �������� ������������ ���������, ��������������� � ������� � ����.	
		pNID->uCallbackMessage = WM_ICON;
		pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
		pNID->uID = ID_TRAYICON;
		TCHAR GUID[] = TEXT("{D98CD3E0-670D-4def-9B74-99FE6E683BFD}");
		hMutex = CreateMutex(NULL, FALSE, GUID);
		DWORD dwAnswer = (hMutex, 0);
		if (dwAnswer == WAIT_TIMEOUT) {
			MessageBox(hWnd, TEXT("���� �� ���� ��������� ������ ������, � �� �� ���� ��-��!!!"), TEXT("���������� ���������"), MB_OK | MB_ICONINFORMATION);
			EndDialog(hWnd, 0);
		}
	}
    break;
	case WM_COMMAND: {
		if (LPARAM(wParam) == IDC_START) {
			LRESULT result1 = SendDlgItemMessage(hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0);//��������� 1 ������ 
			LRESULT result2 = SendDlgItemMessage(hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0);//��������� 2 ������
			LRESULT result3 = SendDlgItemMessage(hWnd, IDC_RADIO3, BM_GETCHECK, 0, 0);//��������� 3 ������

			if (result1 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl1);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("1")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("1"));//��� 1 �������
			}
			if (result2 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl2);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("2")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("2"));//��� 2 �������
			}
			if (result3 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl3);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("3")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("3"));//��� 3 �������
			}

			hThread = CreateThread(NULL, 0, TimeFunc, hTime, 0, NULL);

			TCHAR str_1lvl[10] = TEXT("1");
			TCHAR str_2lvl[10] = TEXT("2");
			TCHAR str_3lvl[10] = TEXT("3");

			int compare = _tcscmp(str_lvl, str_1lvl);
			if (compare == 0) {
				wsprintf(str_text, TEXT("%s"), lvl1);
			}
			else {
				compare = _tcscmp(str_lvl, str_2lvl);
				if (compare == 0) {
					wsprintf(str_text, TEXT("%s"), lvl2);
				}
				else {
					compare = _tcscmp(str_lvl, str_3lvl);
					if (compare == 0) {
						wsprintf(str_text, TEXT("%s"), lvl3);
					}
				}
			}

		}

		if (LPARAM(wParam) == IDOK) {
			points = 0;
			error = 0;

			int count_result = GetWindowTextLength(hEnterText);
			GetWindowText(hEnterText, str_result, count_result + 1);
			for (int i = 0; i < 50; i++) {
				if (str_result[i] == str_text[i]) {
					points++;
				}
				else {
					error++;
				}//�������� ��������� ��� �������� 50
			}
			TCHAR str_points[10];
			wsprintf(str_points, TEXT("%d"), points);
			TCHAR str_error[10];
			wsprintf(str_error, TEXT("%d"), error);

			CloseHandle(hThread);
			EndDialog(hModalDialog, 0);
			ShowWindow(hResultDialog, SW_RESTORE);
			SendMessage(hEdit1, WM_SETTEXT, 0, LPARAM(str_time));
		}
	}
				   break;
	case WM_SIZE: {
		if (message == SIZE_MINIMIZED) {
			ShowWindow(hWnd, SW_HIDE); // ������ ����
			Shell_NotifyIcon(NIM_ADD, pNID); // ��������� ������ � ����
		}
	}
				break;
				//HANDLE_MSG(hWnd, WM_SIZE, Cls_OnSize);

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	if (message == WM_ICON) {
		OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}