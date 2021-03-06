// jhcPickStep.h : choose image size and frame skip
//
// Written by Jonathan H. Connell, jconnell@alum.mit.edu
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright 1999-2011 IBM Corporation
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

// To get the graphical layout: "open" file common.rc in development environment,
// use the commands in the "edit" menu to "copy" dialog IDC_JHCITERATOR, then 
// "paste" it into the Dialog folder on the ResourceView tab of current project. 

#if !defined(AFX_JHCPICKSTEP_H__5B021E83_A0C6_11D2_9645_002035223D8D__INCLUDED_)
/* CPPDOC_BEGIN_EXCLUDE */
#define AFX_JHCPICKSTEP_H__5B021E83_A0C6_11D2_9645_002035223D8D__INCLUDED_
/* CPPDOC_END_EXCLUDE */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "jhcGlobal.h"

#include "stdafx.h"          
#include "resource.h"

#include "Data/jhcParam.h"


///////////////////////////////////////////////////////////////////////////

//= Choose image size and frame skip.

class jhcPickStep : public CDialog
{
private:
  jhcParam *p;
  double f;

// Construction, etc.
public:
  jhcPickStep(CWnd* pParent = NULL);   // standard constructor

private:
// Dialog Data
  //{{AFX_DATA(jhcPickStep)
	enum { IDD = IDD_JHCITERATOR };
  int     m_pause;
  double  m_rate;
  int     m_step;
  int     m_start;
  int     m_stop;
  int     m_where;
  int     m_final;
	int 	  m_pstart;
	//}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(jhcPickStep)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
public:
  int EditStep (jhcParam& vps, double f =0.0);

private:
  void PostAll ();
  void ExtractAll ();

protected:

  // Generated message map functions
  //{{AFX_MSG(jhcPickStep)
  afx_msg void OnDefault();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JHCPICKSTEP_H__5B021E83_A0C6_11D2_9645_002035223D8D__INCLUDED_)
