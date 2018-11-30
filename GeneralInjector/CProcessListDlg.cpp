// CProcessListDlg.cpp : implementation file
//

#include "stdafx.h"
#include <TlHelp32.h>

#include "GeneralInjector.h"
#include "CProcessListDlg.h"
#include "afxdialogex.h"
#include "Helper.h"


// CProcessListDlg dialog

IMPLEMENT_DYNAMIC( CProcessListDlg, CDialogEx )

CProcessListDlg::CProcessListDlg( CWnd* pParent /*=nullptr*/ )
     : CDialogEx( IDD_PROCESS_LIST, pParent )
{

}

CProcessListDlg::~CProcessListDlg()
{
}

void CProcessListDlg::DoDataExchange( CDataExchange* pDX )
{
     CDialogEx::DoDataExchange( pDX );
     DDX_Control( pDX, IDC_LIST_PROCESS, m_ProcessListCtrl );
}


BEGIN_MESSAGE_MAP( CProcessListDlg, CDialogEx )
     ON_WM_SYSCOMMAND()
     ON_WM_PAINT()
     ON_WM_QUERYDRAGICON()
     ON_BN_CLICKED( IDC_BUTTON1, &CProcessListDlg::OnBnClickedButton1 )
END_MESSAGE_MAP()

void CProcessListDlg::AddData( int row, int col, LPTSTR str )

{
     LVITEM lv = { 0 };

     lv.iItem = row;
     lv.iSubItem = col;
     lv.pszText = (LPTSTR)str;
     lv.mask = LVIF_TEXT;

     if ( col == 0 )
          m_ProcessListCtrl.InsertItem( &lv );
     else
          m_ProcessListCtrl.SetItem( &lv );
}

void CProcessListDlg::EnumProcess()
{
     HANDLE hProcessSnap;
     PROCESSENTRY32 pe32;
     DWORD result = NULL;
     TCHAR tmpPath[MAX_PATH];
     DWORD ret = MAX_PATH;
     BOOL isWow64;
     BOOLEAN isWow64Emul;

     CString ProcessName;
     CString Pid;
     CString ImagePath;
     CString ImageType;
     DWORD rowCount = 0;

     // Take a snapshot of all processes in the system.
     hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
     if ( INVALID_HANDLE_VALUE == hProcessSnap ) return;

     pe32.dwSize = sizeof( PROCESSENTRY32 );
     if ( !Process32First( hProcessSnap, &pe32 ) )
     {
          CloseHandle( hProcessSnap );
     }

#ifndef _AMD64_
     isWow64Emul = Helper::IsWow64Emulator();
#endif

     do
     {
#ifdef _AMD64_
          if ( !Helper::IsProcessWow64( pe32.th32ProcessID, &isWow64 ) ||
               isWow64 )
               // 64 bit process only
               continue;
          ImageType = CString( IMAGE_64 );
#else
          if ( isWow64Emul )    // 32 bit injector in x64 platform
          {
               if ( !Helper::IsProcessWow64( pe32.th32ProcessID, &isWow64 ) ||
                    !isWow64 )
                    // 32 bit process only
                    continue;
          }
          ImageType = CString( IMAGE_32 );
#endif

          ProcessName = CString( pe32.szExeFile );
          Pid.Format( _T( "%d" ), pe32.th32ProcessID );
          ImagePath = ( Helper::GetProcessFullpath( pe32.th32ProcessID, tmpPath ) == TRUE ) ? CString( tmpPath ) : CString();

          // ImageType = CString( (LPTSTR)NULL );
          AddData( rowCount, COL_PROC_NAME, (LPTSTR)(LPCTSTR)ProcessName );
          AddData( rowCount, COL_PID, (LPTSTR)(LPCTSTR)Pid );
          AddData( rowCount, COL_IMAGE_PATH, (LPTSTR)(LPCTSTR)ImagePath );
          AddData( rowCount, COL_IMAGE_TYPE, (LPTSTR)(LPCTSTR)ImageType );

          rowCount++;
     } while ( Process32Next( hProcessSnap, &pe32 ) );

     CloseHandle( hProcessSnap );

}

void CProcessListDlg::UpdataProcessList()
{
     m_ProcessListCtrl.SetRedraw( FALSE );

     m_ProcessListCtrl.DeleteAllItems();

     EnumProcess();

     m_ProcessListCtrl.SetRedraw( TRUE );
}
// CProcessListDlg message handlers

BOOL CProcessListDlg::OnInitDialog()
{
     CDialogEx::OnInitDialog();

     m_ProcessListCtrl.InsertColumn( COL_PROC_NAME, _T( "Process Name" ), LVCFMT_LEFT, 150 );
     m_ProcessListCtrl.InsertColumn( COL_PID, _T( "PID" ), LVCFMT_LEFT, 100 );
     m_ProcessListCtrl.InsertColumn( COL_IMAGE_PATH, _T( "Image Path" ), LVCFMT_LEFT, 200 );
     m_ProcessListCtrl.InsertColumn( COL_IMAGE_TYPE, _T( "Image Type" ), LVCFMT_LEFT, 100 );
     m_ProcessListCtrl.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
          LVS_EX_FULLROWSELECT );

     AddData( 0, 0, _T( "a" ) );
     AddData( 0, 1, _T( "b" ) );
     AddData( 0, 2, _T( "c" ) );

     AddData( 1, 0, _T( "1" ) );
     AddData( 1, 1, _T( "2" ) );
     AddData( 1, 2, _T( "3" ) );

     UpdataProcessList();
     return TRUE;
}



void CProcessListDlg::OnBnClickedButton1()
{
     // TODO: Add your control notification handler code here
     UpdataProcessList();
     UpdateData( FALSE );
}
