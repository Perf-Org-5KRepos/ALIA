// jhcChatBox.cpp : dialog for text entry and conversation history
//
// Written by Jonathan H. Connell, jconnell@alum.mit.edu
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 IBM Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 
///////////////////////////////////////////////////////////////////////////

#include <conio.h>

#include "Interface/jhcString.h"       // common video  

#include "Acoustic/jhcChatBox.h"

/* CPPDOC_BEGIN_EXCLUDE */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* CPPDOC_END_EXCLUDE */

/////////////////////////////////////////////////////////////////////////////
// jhcChatBox dialog

//= Standard constructor invokes base class constructor with layout.

jhcChatBox::jhcChatBox(CWnd* pParent /*=NULL*/)
	: CDialog(jhcChatBox::IDD, pParent)
{
  *entry = '\0';
  disable = 0;
  quit = 0;

	//{{AFX_DATA_INIT(jhcChatBox)
	//}}AFX_DATA_INIT
}


//= Make and show window immediately (modeless) at some location.
// need to call Interact regularly to update components

void jhcChatBox::Launch (int x, int y)
{
  Create(IDD);
  Mute(1);
  SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
}


//= Clear all parts of the interaction dialog and set mute status.
// call this first to make sure previous conversation finishes

void jhcChatBox::Reset (int disable)
{
  m_hist.Clear();
  Mute(disable);
  Interact();
}


//= Allow user input (or not).
// flushes any pending user text when activated
// typically called at the end of a conversation

void jhcChatBox::Mute (int gray)
{
  // record current state and clear input text
  disable = gray;
  m_input.SetWindowText(_T(""));
  *entry = '\0';
  quit = 0;

  // change state of typing panel
  if (gray > 0)
    m_input.SetReadOnly(true);
  else
  {
    m_input.SetReadOnly(false);
    m_input.SetFocus();
    ShowWindow(SW_SHOWNORMAL);         // in case minimized
  }  
}


//= Make sure all messages get handled (call regularly).
// returns 1 if okay, 0 if ESC has been pressed, -1 if application ending

int jhcChatBox::Interact ()
{
  MSG msg;

  // see if any message are waiting but do not block
  while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
  {
    // hard quit if user selects something from a menu 
    if (msg.message == WM_QUIT)
      return -1;
    if (msg.message == WM_COMMAND)          
    {
      quit = 1;
      return 0;
    }

    // handle ENTER and ESC key messages specially
    GetMessage(&msg, NULL, 0, 0);
    if ((msg.message == WM_KEYDOWN) && (msg.wParam == VK_RETURN))        
      OnOK();
    else if ((msg.message == WM_KEYDOWN) && (msg.wParam == VK_ESCAPE))   
      quit = 1;
    else
    {
      // handle most messages in the usual way
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return((quit > 0) ? 0 : 1);
}


//= Copy most recent user input (may miss some if not called regularly).
// returns pointer to passed string (cleared if nothing)

char *jhcChatBox::Get (char *input, int ssz)
{
  strcpy_s(input, ssz, entry);
  *entry = '\0';
  return input;
}


//= Record system response (typically) in conversation history.
// returns input string for convenience

const char *jhcChatBox::Post (const char *output, int user)
{
  m_hist.AddTurn(output, user);
  return output;
}


//= Force string (from file) into typing window.

void jhcChatBox::Inject (const char *line)
{
  jhcString in;
  int n = (int) strlen(line);

  // automatically strip final newline (if any)
  if (n > 0)
    if (line[n - 1] == '\n')
      n--;
  if (n <= 0)
    return;

  // send to input pane
  strncpy_s(in.ch, line, n);     // always terminates
  in.C2W();
  m_input.SetWindowText(in.Txt());

  // adjust cursor
  SendMessage(EM_SETSEL, 0, -1);
  SetFocus();
}


void jhcChatBox::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(jhcChatBox)
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_JHC_HIST, m_hist);
  DDX_Control(pDX, IDC_JHC_CHAT, m_input);
}


BEGIN_MESSAGE_MAP(jhcChatBox, CDialog)
	//{{AFX_MSG_MAP(jhcChatBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// jhcChatBox message handlers

//= Get text from edit control and add to chat history.
// also make an internal copy as a conventional char string

void jhcChatBox::OnOK()
{
  jhcString guts;
  int n;

  // ignore if input muted
  if (disable > 0)
    return;

  // extract the text part as an old-fashioned string
  if ((n = m_input.GetWindowTextLength()) <= 0)
    return;
  m_input.GetWindowText(guts.Txt(), n + 1);
  guts.Sync();
  strcpy_s(entry, guts.ch);

  // clear for next input (might be added in normalized form)
  m_input.SetWindowText(_T(""));
  m_input.SetFocus();
}
