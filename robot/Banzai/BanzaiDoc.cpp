// BanzaiDoc.cpp : top level GUI framework does something
//
// Written by Jonathan H. Connell, jconnell@alum.mit.edu
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright 2015-2020 IBM Corporation
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

#include "stdafx.h"
#include "Banzai.h"

#include "BanzaiDoc.h"

#include <direct.h>                  // for _getcwd in Windows


// JHC: for filtered video file open

extern CBanzaiApp theApp;


// JHC: for basic functionality

#include "Data/jhcImgIO.h"             // common video
#include "Interface/jhcMessage.h"
#include "Interface/jhcPickString.h"
#include "Interface/jhcPickVals.h"
#include "Interface/jhcString.h"
#include "Interface/jms_x.h"
#include "Processing/jhcFilter.h"


// whether to do faster background video capture

#define FASTVID 1    // some cameras need zero?


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBanzaiDoc

IMPLEMENT_DYNCREATE(CBanzaiDoc, CDocument)

BEGIN_MESSAGE_MAP(CBanzaiDoc, CDocument)
	//{{AFX_MSG_MAP(CBanzaiDoc)
	ON_COMMAND(ID_FILE_CAMERA, OnFileCamera)
	ON_COMMAND(ID_FILE_CAMERAADJUST, OnFileCameraadjust)
	ON_COMMAND(ID_FILE_OPENEXPLICIT, OnFileOpenexplicit)
	ON_COMMAND(ID_PARAMETERS_VIDEOCONTROL, OnParametersVideocontrol)
	ON_COMMAND(ID_PARAMETERS_IMAGESIZE, OnParametersImagesize)
	ON_COMMAND(ID_TEST_PLAYVIDEO, OnTestPlayvideo)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPENVIDEO, OnFileOpenvideo)
	ON_COMMAND(ID_PARAMETERS_SAVEDEFAULTS, OnParametersSavedefaults)
	ON_COMMAND(ID_PARAMETERS_LOADDEFAULTS, OnParametersLoaddefaults)
	//}}AFX_MSG_MAP
  ON_COMMAND(ID_FILE_KINECTSENSOR, &CBanzaiDoc::OnFileKinectsensor)
  ON_COMMAND(ID_FILE_KINECTHIRES, &CBanzaiDoc::OnFileKinecthires)
  ON_COMMAND(ID_FILE_SAVESOURCE, &CBanzaiDoc::OnFileSavesource)
  ON_COMMAND(ID_UTILITIES_PLAYDEPTH, &CBanzaiDoc::OnUtilitiesPlaydepth)
  ON_COMMAND(ID_UTILITIES_PLAYBOTH, &CBanzaiDoc::OnUtilitiesPlayboth)
  ON_COMMAND(ID_INTEREST_VIVIDVIEW, &CBanzaiDoc::OnInterestVividview)
  ON_COMMAND(ID_ANIMATION_IDLE, &CBanzaiDoc::OnAnimationIdle)
  ON_COMMAND(ID_ANIMATION_NEUTRAL, &CBanzaiDoc::OnAnimationNeutral)
  ON_COMMAND(ID_ARM_GOTOPOSE, &CBanzaiDoc::OnArmGotopose)
  ON_COMMAND(ID_UTILITIES_TEST, &CBanzaiDoc::OnUtilitiesTest)
  ON_COMMAND(ID_ARM_SWINGJOINT, &CBanzaiDoc::OnArmSwingjoint)
  ON_COMMAND(ID_ARM_SWINGPARAMS, &CBanzaiDoc::OnArmSwingparams)
  ON_COMMAND(ID_ARM_LIMP, &CBanzaiDoc::OnArmLimp)
  ON_COMMAND(ID_ARM_HANDFORCE, &CBanzaiDoc::OnArmHandforce)
  ON_COMMAND(ID_FORCE_DRAGHAND, &CBanzaiDoc::OnForceDraghand)
  ON_COMMAND(ID_FORCE_DRAGROBOT, &CBanzaiDoc::OnForceDragrobot)
  ON_COMMAND(ID_DEMO_DEMOOPTIONS, &CBanzaiDoc::OnDemoDemooptions)
  ON_COMMAND(ID_DEMO_TEXTFILE, &CBanzaiDoc::OnDemoTextfile)
  ON_COMMAND(ID_DEMO_INTERACTIVE, &CBanzaiDoc::OnDemoInteractive)
  ON_COMMAND(ID_PARAMETERS_MOVECMD, &CBanzaiDoc::OnParametersMovecmd)
  ON_COMMAND(ID_PARAMETERS_TURNCMD, &CBanzaiDoc::OnParametersTurncmd)
  ON_COMMAND(ID_PARAMETERS_BASEPROGRESS, &CBanzaiDoc::OnParametersBaseprogress)
  ON_COMMAND(ID_PARAMETERS_BASERAMP, &CBanzaiDoc::OnParametersBaseramp)
  ON_COMMAND(ID_DEMO_RESETROBOT, &CBanzaiDoc::OnDemoResetrobot)
  ON_COMMAND(ID_PARAMETERS_LIFTCMD, &CBanzaiDoc::OnParametersLiftcmd)
  ON_COMMAND(ID_PARAMETERS_LIFTRAMP, &CBanzaiDoc::OnParametersLiftramp)
  ON_COMMAND(ID_PARAMETERS_BATTERYLEVEL, &CBanzaiDoc::OnParametersBatterylevel)
  ON_COMMAND(ID_PARAMETERS_GRABCMD, &CBanzaiDoc::OnParametersGrabcmd)
  ON_COMMAND(ID_PARAMETERS_GRABRAMP, &CBanzaiDoc::OnParametersGrabramp)
  ON_COMMAND(ID_PARAMETERS_ARMHOME, &CBanzaiDoc::OnParametersArmhome)
  ON_COMMAND(ID_GROUNDING_HANDCMD, &CBanzaiDoc::OnGroundingHandcmd)
  ON_COMMAND(ID_GROUNDING_WRISTCMD, &CBanzaiDoc::OnGroundingWristcmd)
  ON_COMMAND(ID_GROUNDING_NECKCMD, &CBanzaiDoc::OnGroundingNeckcmd)
  ON_COMMAND(ID_PROFILING_ARMRAMP, &CBanzaiDoc::OnProfilingArmramp)
  ON_COMMAND(ID_RAMP_NECKRAMP, &CBanzaiDoc::OnRampNeckramp)
  ON_COMMAND(ID_DEPTH_PERSONMAP, &CBanzaiDoc::OnDepthPersonmap)
  ON_COMMAND(ID_DEPTH_TRACKHEAD, &CBanzaiDoc::OnDepthTrackhead)
  ON_COMMAND(ID_PEOPLE_SPEAKING, &CBanzaiDoc::OnPeopleSpeaking)
  ON_COMMAND(ID_PARAMETERS_WATCHING, &CBanzaiDoc::OnParametersWatching)
  ON_COMMAND(ID_PARAMETERS_ORIENTING, &CBanzaiDoc::OnParametersOrienting)
  ON_COMMAND(ID_PARAMETERS_TARGETTIME, &CBanzaiDoc::OnParametersTargettime)
  ON_COMMAND(ID_ATTENTION_ENROLLPHOTO, &CBanzaiDoc::OnAttentionEnrollphoto)
  ON_COMMAND(ID_ATTENTION_ENROLLLIVE, &CBanzaiDoc::OnAttentionEnrolllive)
  ON_COMMAND(ID_PEOPLE_SOCIALEVENTS, &CBanzaiDoc::OnPeopleSocialevents)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanzaiDoc construction/destruction

CBanzaiDoc::CBanzaiDoc()
{
  // JHC: move console and chat windows
  prt.SetTitle("ALIA console", 1);
  SetWindowPos(GetConsoleWindow(), HWND_TOP, 1090, 5, 673, 1000, SWP_SHOWWINDOW);
  chat.Launch(710, 5);

  // JHC: assume app called with command line argument fo file to open
  cmd_line = 1;
  strcpy_s(rname, "saved.bmp");

  // break out components
  eb  = &(ec.body);

  // video parameters
  eb->BindVideo(&v);
  v.Shift = 2;

  // load configuration file(s)
  _getcwd(cwd, 200);
  sprintf_s(ifile, "%s\\Banzai_vals.ini", cwd);
  swing_params(ifile);
  interact_params(ifile);
  ec.Defaults(ifile);      // load defaults on start
}


// load parameters for testing single joint trajectories

int CBanzaiDoc::swing_params (const char *fname)
{
  jhcParam *ps = &jps;
  int ok;

  ps->SetTag("jt_swing", 0);
  ps->SetTitle("Pick joint movement parameters");
  ps->NextSpec4( &jnum,    1,   "Joint number");
  ps->NextSpecF( &acc,   180.0, "Acceleration (dps^2)");
  ps->NextSpecF( &slope,  10.0, "Servo slope (degs)");
  ps->NextSpecF( &start,  60.0, "Initial angle");
  ps->NextSpecF( &chg,   -90.0, "Angle change");
  ps->NextSpecF( &rate,    1.0, "Motion rate");

  ps->NextSpecF( &fchk,    3.0, "Motion lead factor");
  ps->NextSpecF( &gap,     0.3, "Time between swings");
  ok = ps->LoadDefs(fname);
  ps->RevertAll();
  return ok;
}


CBanzaiDoc::~CBanzaiDoc()
{
  if (cmd_line <= 0)
  {
    ips.SaveVals(ifile);
    ec.SaveVals(ifile);    // save defaults on exit
    jps.SaveVals(ifile);
  }
}


BOOL CBanzaiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

  // change this value to non-zero for externally distributed code
  // cripple = -1 for full debugging
  //         =  0 for normal full control, expiration warning
  //         =  1 for restricted operation, expiration warning
  //         =  2 for restricted operation, expiration enforced
  cripple = 0;
  ver = ec.Version();
  LockAfter(6, 2020, 1, 2020);

  // JHC: if this function is called, app did not start with a file open
  // JHC: initializes display object which depends on document
  cmd_line = 0;
  if (d.Valid() <= 0)   
    d.BindTo(this);

	return TRUE;
}


// JHC: possibly run start up demo if called with command line file
// RunDemo is called from main application Banzai.cpp
// will have already called OnOpenDocument to set up video

void CBanzaiDoc::RunDemo()
{
  if (cmd_line <= 0)
    return;
  if (d.Valid() <= 0)   
    d.BindTo(this);

  OnDemoInteractive();
//  OnCloseDocument();    // possibly auto-exit when done
}


// only allow demo code to run for a short while 
// make sure clock can't be reset permanently
// more of an annoyance than any real security

int CBanzaiDoc::LockAfter (int mon, int yr, int smon, int syr)
{
  char *tail;
  int cyr, cmon;

  // provide "backdoor" - override if directly in "jhc" directory
  if ((tail = strrchr(cwd, '\\')) != NULL)
    if (strcmp(tail, "\\jhc") == 0)
      if (cripple > 0)
        cripple = 0;

  // determine current month and year
  CTime today = CTime::GetCurrentTime();
  cyr = today.GetYear();
  cmon = today.GetMonth();

  // see if past expiration date (or before issue date)
  if ((cyr > yr)  || ((cyr == yr)  && (cmon > mon)) ||
      (cyr < syr) || ((cyr == syr) && (cmon < smon))) 
  {
    if (cripple > 1)
      Fatal("IBM Banzai %4.2f\nExpired as of %d/%d\njconnell@us.ibm.com",
            ver, mon, yr);
    Complain("IBM Banzai %4.2f\nOut-of-date as of %d/%d\njconnell@us.ibm.com", 
             ver, mon, yr);
  }
  return 1;
}


// what to do for functions that have been disabled

int CBanzaiDoc::LockedFcn ()
{
  if (cripple <= 0)
    return 0;
  Tell("Function not user-accessible in this version");
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
// CBanzaiDoc serialization

void CBanzaiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


/////////////////////////////////////////////////////////////////////////////
// CBanzaiDoc diagnostics

#ifdef _DEBUG
void CBanzaiDoc::AssertValid() const
{
	CDocument::AssertValid();
}


void CBanzaiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
//                               Video Source                              //
/////////////////////////////////////////////////////////////////////////////

// JHC: have to go into ClassWizard to override this function
// user has already chosen a file name so open it, init display
// this is also the function called by selecting from the MRU list

BOOL CBanzaiDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  jhcString fn(lpszPathName);
  char alt[250];
  const char *start;
  char *ch = alt;

  // defeat command line argument at startup
  if (strchr("!?", fn.ch[fn.Len() - 1]) != NULL)
    return TRUE;
  
  // possibly convert text back from "safe" form in MRU list
  if ((start = strstr(fn.ch, "=> ")) != NULL) 
  {
    strcpy_s(alt, start + 3);
    while (*ch != '\0')
      if (*ch == ';')
        *ch++ = ':';
      else if (*ch == '|')
        *ch++ = '/';
      else
        ch++;
    fn.Set(alt);
  }

  // open source based on passed (modified) string
  d.Clear(1, "Configuring source ...");
	if (v.SetSource(fn.ch) > 0)
    d.StatusText("");
  else
    ShowFirst();
	return TRUE;
}


// JHC: use default video driver

void CBanzaiDoc::OnFileCamera() 
{
  jhcString mru;

  d.Clear(1, "Configuring camera ...");
  if (v.SetSource("*.dx") <= 0)  // was vfw
  {
    d.StatusText("");
    return;
  }
  sprintf_s(mru.ch, "C:/=> %s", v.File());
  mru.C2W();
  theApp.AddToRecentFileList(mru.Txt());
	ShowFirst();
}


// JHC: let user pick driver from menu and select all options

void CBanzaiDoc::OnFileCameraadjust() 
{
  jhcString mru;

  d.Clear(1, "Configuring camera ...");
  if (v.SetSource("*.dx+") <= 0)  // was vfw
  {
    d.StatusText("");
    return;
  }
  sprintf_s(mru.ch, "C:/=> %s", v.File());
  mru.C2W();
  theApp.AddToRecentFileList(mru.Txt());
  ShowFirst();
}



// Connect to combined color and depth sensor

void CBanzaiDoc::OnFileKinectsensor()
{
  jhcString mru;

  d.Clear(1, "Configuring Kinect sensor ...");
  if (v.SetSource("0.kin") <= 0)  
  {
    d.StatusText("");
    return;
  }
  sprintf_s(mru.ch, "C:/=> %s", v.File());
  mru.C2W();
  theApp.AddToRecentFileList(mru.Txt());
	ShowFirst();
}


// Get slow high-resolution color and fast depth 

void CBanzaiDoc::OnFileKinecthires()
{
  jhcString mru;

  d.Clear(1, "Configuring Kinect sensor ...");
  if (v.SetSource("0.kin2") <= 0)  
  {
    d.StatusText("");
    return;
  }
  sprintf_s(mru.ch, "C:/=> %s", v.File());
  mru.C2W();
  theApp.AddToRecentFileList(mru.Txt());
	ShowFirst();
}


// JHC: like normal Open but filters files for video types

void CBanzaiDoc::OnFileOpenvideo() 
{
  jhcString fname("");

  d.Clear(1, "Configuring video source ...");
  if (v.SelectFile(fname.ch, 500) <= 0)
  {
    d.StatusText("");
    return;
  }
  ShowFirst();
  fname.C2W();
  theApp.AddToRecentFileList(fname.Txt());
}


// JHC: let user type a file name, wildcard pattern, or vfw spec
// e.g. repeat foo.ras, show all *.bmp, choose Bt848\*.vfw
//      http://62.153.249.21/live/wildpark/live300k.asx

void CBanzaiDoc::OnFileOpenexplicit() 
{
  jhcString mru;
  char *ch = mru.ch + 6;

  // ask user for text then try opening source
  d.Clear(1, "Configuring video source ...");
  if (v.AskSource() <= 0)
  {
    d.StatusText("");
    return;
  }
  ShowFirst();

  // convert text into "safe" form for MRU list
  sprintf_s(mru.ch, "C:/=> %s", v.File());
  while (*ch != '\0')
    if (*ch == ':')
      *ch++ = ';';
    else if (*ch == '/')
      *ch++ = '|';
    else
      ch++;
  mru.C2W();
  theApp.AddToRecentFileList(mru.Txt());
}


/////////////////////////////////////////////////////////////////////////////
//                              Video Utilities                            //
/////////////////////////////////////////////////////////////////////////////

// JHC: show first frame of a new video source

void CBanzaiDoc::ShowFirst()
{
  jhcImg col, d8;

  // make sure some stream and set size for receiving images
  if (!v.Valid())
    return;
  eb->BindVideo(&v);

  // adjust pause and playback for list of images
  v.PauseNum = 0;
  if (v.IsClass("jhcListVSrc") > 0)
  {
    v.PauseNum = 1;
    v.DispRate = 0.001;
  }

  // try to read images
  v.Rewind();
  eb->UpdateImgs();
  v.Rewind();

  // get and show pretty color
  eb->SmallSize(col);
  eb->ImgSmall(col);
  d.Clear();
  d.ShowGrid(col, 0, 0, 0, v.Name());
  if (v.Dual() > 0)
  {
    // depth also
    eb->DepthSize(d8);
    eb->Depth8(d8);
    d.ShowGrid(d8,  1, 0, 0, "Depth");
  }
  d.StatusText("Ready");
}


// JHC: ask user for start, step. rate, etc. 

void CBanzaiDoc::OnParametersVideocontrol() 
{
  d.StatusText("Configuring video source ...");
  if (v.AskStep() <= 0)
    d.StatusText("");
  else
    ShowFirst();
}


// JHC: ask user for sizes and whether monochrome

void CBanzaiDoc::OnParametersImagesize() 
{
  d.StatusText("Configuring video source ...");
  if (v.AskSize() <= 0)
    d.StatusText("");
  else
    ShowFirst();
}


// JHC: see if video source is valid, if not try opening camera

int CBanzaiDoc::ChkStream (int dual)
{
  if (v.Valid() && ((dual <= 0) || v.Dual()))
    return 1;
  d.StatusText("Configuring Kinect sensor ...");
  if (v.SetSource("0.kin") <= 0)
  {
    d.StatusText("");
    return 0;
  }
  v.PauseNum = 0;
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//                              Video Playback                             //
/////////////////////////////////////////////////////////////////////////////

// JHC: make up proper return image then start playback
// can use this as a template for all application functions

void CBanzaiDoc::OnTestPlayvideo() 
{
  if (!ChkStream(0))
    return;

  // get correct image sizes
  eb->BindVideo(&v);

  // loop over selected set of frames  
  d.Clear(1, "Live color image ...");
  v.Rewind(FASTVID);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get frame, pause if right mouse click
      if (eb->UpdateImgs() <= 0)
        break;

      // show frame on screen
      d.ShowGrid(eb->Color(), 0, 0, 0, "%d: %s  --  Color", v.Last(), v.FrameName());
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
  res.Clone(eb->Color());	
  sprintf_s(rname, "%s_color.bmp", v.FrameName());
}


// Play just the depth images from a dual source

void CBanzaiDoc::OnUtilitiesPlaydepth()
{
  jhcImg d8;

  if (!ChkStream())
    return;

  // get correct image sizes
  eb->BindVideo(&v);
  eb->DepthSize(d8);

  // loop over selected set of frames  
  d.Clear(1, "Live depth image ...");
  v.Rewind(FASTVID);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get frame, pause if right mouse click
      if (eb->UpdateImgs() <= 0)
        break;
      eb->Depth8(d8);

      // show frame on screen
      d.ShowGrid(d8, 0, 0, 0, "%d: %s  --  Depth", v.Last(), v.FrameName());
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
  res.Clone(d8);	
  sprintf_s(rname, "%s_depth.bmp", v.FrameName());
}


// Play both color and depth from current source

void CBanzaiDoc::OnUtilitiesPlayboth()
{
  jhcImg d8;

  if (!ChkStream())
    return;

  // get correct image sizes
  eb->BindVideo(&v);
  eb->DepthSize(d8);

  // loop over selected set of frames  
  d.Clear(1, "Color and depth ...");
  v.Rewind(FASTVID);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get frame, pause if right mouse click
      if (eb->UpdateImgs() <= 0)
        break;
      eb->Depth8(d8);

      // show frame on screen
      d.ShowGrid(eb->Color(), 0, 0, 0, "%d: %s", v.Last(), v.FrameName());
      d.ShowGrid(d8, 0, 1, 0, "Depth");
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
}


/////////////////////////////////////////////////////////////////////////////
//                               Saving Images                             //
/////////////////////////////////////////////////////////////////////////////

// JHC: save last displayed image at user selected location
// should generally change this to some more meaningful image

void CBanzaiDoc::OnFileSaveAs() 
{
  jhcString sel, rn(rname), idir(cwd);
  CFileDialog dlg(FALSE, NULL, rn.Txt());  
  jhcImgIO fio;
  jhcImg d8;

  // show result first
  if (!res.Valid())
    return;
  d.Clear();
  d.ShowGrid(res, 0, 0, 0, "Last result");

  // pop standard file choosing box starting at video directory
  if (*(v.Dir()) != '\0')
    idir.Set(v.Dir());
  (dlg.m_ofn).lpstrInitialDir = idir.Txt();
  if (dlg.DoModal() != IDOK)
    return;

  // show main image
  sel.Set((dlg.m_ofn).lpstrFile);
  fio.Save(sel.ch, res, 1);
  d.ShowGrid(res, 0, 0, 0, "Saved as %s", fio.Name());
}


// Save most recent sensor inputs

void CBanzaiDoc::OnFileSavesource()
{
  jhcString sel, idir(cwd), init("situation.bmp");
  CFileDialog dlg(FALSE, NULL, init.Txt());  
  jhcImgIO fio;
  jhcName name;
  jhcImg col, d8;

  // get pretty images from Celia body
  eb->SmallSize(col);
  eb->DepthSize(d8);
  eb->ImgSmall(col);
  eb->Depth8(d8);

  // show result first
  d.Clear();
  d.ShowGrid(col, 0, 0, 0, "Last input");
  d.ShowGrid(d8,  1, 0, 0, "Depth");

  // pop standard file choosing box starting at video directory
  if (*(v.Dir()) != '\0')
    idir.Set(v.Dir());
  (dlg.m_ofn).lpstrInitialDir = idir.Txt();
  if (dlg.DoModal() != IDOK)
    return;
  sel.Set((dlg.m_ofn).lpstrFile);
  name.ParseName(sel.ch);

  // save raw images then indicate name for each
  fio.SaveDual(name.File(), eb->Color(), eb->Range());
  d.ShowGrid(col, 0, 0, 0, "Saved as: %s", name.Name());
  d.ShowGrid(d8,  1, 0, 0, "Saved as: %s", fio.Name());
}


/////////////////////////////////////////////////////////////////////////////
//                            Motion Profiling                             //
/////////////////////////////////////////////////////////////////////////////

// Motion profiling for change of gaze direction

void CBanzaiDoc::OnRampNeckramp()
{
	jhcPickVals dlg;

  dlg.EditParams((eb->neck).rps); 
}


// Trapezoidal profiling for lift stage

void CBanzaiDoc::OnParametersLiftramp()
{
	jhcPickVals dlg;

  dlg.EditParams((eb->lift).fps); 
}


// Trapezoidal profiling for arm positioning

void CBanzaiDoc::OnProfilingArmramp()
{
	jhcPickVals dlg;

  dlg.EditParams((eb->arm).tps); 
}


// Speed of finger motion and force adjustment step

void CBanzaiDoc::OnParametersGrabramp()
{
	jhcPickVals dlg;

  dlg.EditParams((eb->arm).fps); 
}


// Trapezoidal profiling for translation and rotation

void CBanzaiDoc::OnParametersBaseramp()
{
	jhcPickVals dlg;

  dlg.EditParams((eb->base).mps); 
}


/////////////////////////////////////////////////////////////////////////////
//                            Ballistic Grounding                          //
/////////////////////////////////////////////////////////////////////////////


// Set battery notification level, drop detection, etc. 

void CBanzaiDoc::OnParametersBatterylevel()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).eps); 
}


// Parameters for changing gaze direction of head

void CBanzaiDoc::OnGroundingNeckcmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).nps);
}


// How to interpret words in fork lift stage commands

void CBanzaiDoc::OnParametersLiftcmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).lps); 
}


// How to interpret words in translation commands

void CBanzaiDoc::OnParametersMovecmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).tps); 
}


// How to interpret words in rotation commands

void CBanzaiDoc::OnParametersTurncmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).rps); 
}


// How to determine if a translation or rotation has failed

void CBanzaiDoc::OnParametersBaseprogress()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).pps); 
}


// Set pose for extend arm and hand

void CBanzaiDoc::OnParametersArmhome()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).aps); 
}


// Parameters for changing position of hand

void CBanzaiDoc::OnGroundingHandcmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).hps);
}


// Parameters for changing orientation of hand

void CBanzaiDoc::OnGroundingWristcmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).wps);
}


// Parameters for holding and releasing objects

void CBanzaiDoc::OnParametersGrabcmd()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.ball).gps); 
}


/////////////////////////////////////////////////////////////////////////////
//                         Application Parameters                          //
/////////////////////////////////////////////////////////////////////////////

// save parameters to file

void CBanzaiDoc::OnParametersSavedefaults() 
{
  jhcString sel, init(ifile), idir(cwd);
  CFileDialog dlg(FALSE, NULL, init.Txt());

  (dlg.m_ofn).lpstrInitialDir = idir.Txt();
  (dlg.m_ofn).lpstrFilter = _T("Initialization Files\0*.ini\0Text Files\0*.txt\0All Files (*.*)\0*.*\0");
  if (dlg.DoModal() == IDOK)
  {
    sel.Set((dlg.m_ofn).lpstrFile);   
    ips.SaveVals(sel.ch);
    ec.SaveVals(sel.ch);            // change to relevant object
    jps.SaveVals(sel.ch); 
  }
}


// load parameters from file

void CBanzaiDoc::OnParametersLoaddefaults() 
{
  jhcString sel, init(ifile), idir(cwd);
  CFileDialog dlg(TRUE, NULL, init.Txt());

  (dlg.m_ofn).lpstrInitialDir = idir.Txt();
  (dlg.m_ofn).lpstrFilter = _T("Initialization Files\0*.ini\0Text Files\0*.txt\0All Files (*.*)\0*.*\0");
  if (dlg.DoModal() == IDOK)
  {
    sel.Set((dlg.m_ofn).lpstrFile);
    interact_params(sel.ch);
    ec.Defaults(sel.ch);            // change to relevant object
    swing_params(sel.ch);
  }
}


/////////////////////////////////////////////////////////////////////////////
//                          Application Functions                          //
/////////////////////////////////////////////////////////////////////////////

// Show color and texture enhanced image

void CBanzaiDoc::OnInterestVividview()
{
  jhcFilter f;
  jhcImg col, boost, mono, cvt;
  double gain0 = 2.0, gain = 2.0;
  int sc = 9, dmin = 35; 

  if (!ChkStream(0))
    return;

  // get correct image sizes
  eb->Reset(0);
  eb->BigSize(col);

  // set local image sizes
  f.SetSize(col);
  cvt.SetSize(col);
  boost.SetSize(col);
  mono.SetSize(col, 1);

  // loop over selected set of frames  
  d.Clear(1, "Vivid view ...");
  v.Rewind(1);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get frame, pause if right mouse click
      if (eb->UpdateImgs() <= 0)
        break;
      eb->ImgBig(col);

      // smooth incoming image over time
      f.Flywheel(col);
      MaxColor(boost, f.est, gain0);

      // find maximum channel brightness in local region
      // then boost all color channels so something maxes out
      MaxAll(mono, f.est);
      BoxMax(mono, mono, sc);
      BoxAvg(mono, mono, sc, sc, 1.0 / gain);
      LimitMin(mono, mono, dmin);
      NormBy(cvt, f.est, mono, 0.5);

      // show frame on screen
      d.ShowGrid(cvt,   0, 0, 0, "Region %d boost x%3.1f", sc, gain);   
      d.ShowGrid(boost, 1, 0, 0, "Pixel boost x %3.1f", gain0);
      d.ShowGrid(col,   0, 1, 0, "%ld of %s", v.Last(), v.FrameName());
      d.ShowGrid(f.est, 1, 1, 0, "Temporal smoothing");
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
  res.Clone(cvt);	
  sprintf_s(rname, "%s_vivid.bmp", v.FrameName());
}


/////////////////////////////////////////////////////////////////////////////
//                               Arm Functions                             //
/////////////////////////////////////////////////////////////////////////////

// Pick joint and rate as well as start and end angles for swing test 

void CBanzaiDoc::OnArmSwingparams()
{
	jhcPickVals dlg;

  dlg.EditParams(jps);
}


// Move some joint back and forth to set acceleration

void CBanzaiDoc::OnArmSwingjoint()
{
  jhcEliArm *arm = &(eb->arm);
	jhcPickVals dlg;
  jhcMatrix a0(7), a(7);
  jhcMatrix *goal;
//  jhcArr targ, err;
  jhcArr pos, vel;
  DWORD t0;
  double mt;
  int w0 = d.gwid, h0 = d.ght, ms = 33, cyc = 4, i0 = -1; 
  int sz, n, mn, gn, top, bot, stop, j, i;
//  int top2, bot2;

  // start up robot
  d.Clear(1, "Swinging joint ...");
  d.ResetGrid(3, 640, 200);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;
  eb->InitPose();
  jnum = __max(0, __min(jnum, 7));

  // change to starting angle
  d.ClearGrid(0, 0);
  d.StringGrid(0, 0, "Setting start angle ...");
  arm->ArmConfig(a0);
  a0.VSet(jnum, start);
  arm->SetConfig(a0);
  
  // adjust controller parameters
  (arm->jt[jnum]).astd = acc;
  (arm->jt[jnum]).SetStiff(slope);

  // see how long move is expected to take
  a.Copy(a0);
  a.VInc(jnum, chg);
  mt = arm->CfgTime(a, a0, rate);
  mn = ROUND(0.5 * 1000.0 * mt / ms);
  gn = ROUND(0.5 * 1000.0 * gap / ms);
  n = mn + gn;

  // build performance arrays
  sz = cyc * n;
//  targ.InitSize(sz);
//  err.InitSize(sz);
  pos.InitSize(sz);
  vel.InitSize(sz);
  
  // loop over selected set of frames  
  d.ResetGrid();
  d.gwid = 1000;
  t0 = jms_now();
  try
  {
    while (d.AnyHit() == 0)
    {
      // get current sensor values
      if (eb->Update(0, 0) <= 0)
        break;

      // figure out elapsed time
      i = ROUND(0.5 * (jms_now() - t0) / (double) ms);
      if (i >= sz)
        break;

      // generate move command 
      if (((i / n) & 0x01) == 0)
        goal = &a;
      else
        goal = &a0;
      arm->CfgTarget(*goal, rate);

      // get command vs. actual position
      for (j = i0 + 1; j <= i; j++)
      {
//        targ.ASet(j, ROUND(100.0 * arm->CtrlPos(jnum)));
//        err.ASet( j, ROUND(100.0 * (arm->JtAng(jnum) - (arm->jt[jnum]).RampPos())));
        pos.ASet( j, ROUND(100.0 * arm->JtAng(jnum)));
        vel.ASet( j, ROUND(100.0 * arm->CtrlVel(jnum)));
      }

      // check error in stop regions
      stop = 0;
/*
      for (j = 0; j < sz; j++)
        if ((j % n) > mn)
          stop = __max(stop, abs(err.ARef(j)));
*/

      // graph target and response
      d.ght = 360;
      top = pos.MaxVal();
//      top2 = targ.MaxVal();
//      top = __max(top, top2);
      bot = pos.MinVal();
//      bot2 = targ.MinVal();
//      bot = __min(bot, bot2);
      if (bot < 0)
        top = __min(-top, bot);
      d.GraphGrid(pos, 0, 0, top, 5, "%s  --  Command vs. actual position ", arm->JtName(jnum));
      d.GraphVal(0, top);
//      d.GraphOver(targ, top, 4);
      for (j = 1; j <= cyc; j++)
      {
        d.GraphMark(j * n, 2);
        d.GraphMark(j * n - gn);
      }
/*
      // graph position error and 1 degree line
      d.ght = 120;
      top = err.MaxAbs();
      top = __max(100, top);
      d.GraphGrid(err, 0, 1, -top, 5, "Position error (max = %4.2f, final = %4.2f degs)", 
                                      0.01 * top, 0.01 * stop);
      d.GraphVal(100, -top, 8);
      d.GraphVal(0, -top, 8);
      d.GraphVal(-100, -top, 8);
      for (j = 1; j <= cyc; j++)
      {
        d.GraphMark(j * n, 2);
        d.GraphMark(j * n - gn);
      }
*/
      // graph trajectory velocity
      d.ght = 120;
      top = vel.MaxAbs();
      d.GraphBelow(vel, -top, 4, "Command velocity (%4.2f secs)", mt); 
      d.GraphVal(0, -top);
      for (j = 1; j <= cyc; j++)
      {
        d.GraphMark(j * n, 2);
        d.GraphMark(j * n - gn);
      }

      // send command
      eb->Issue(fchk);
      jms_sleep(ms - 8);
      i0 = i;
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.gwid = w0;
  d.ght = h0;
  d.StatusText("Stopped.");
}


// Establish target pose then achieve it form various starting points

void CBanzaiDoc::OnArmGotopose()
{
  jhcEliArm *arm = &(eb->arm);
  jhcMatrix a0(7), a(7), a2(7), rates(6), err(6);
  char diff[80], goal[80], start[80];
  double mt0, mt, t, tol = 2.0;
  int hit, n, ms = 33, state = 1; 

  // start up robot
  d.Clear(1, "Pose trajectories ...");
  d.ResetGrid(3, 640, 240);
  d.StringGrid(0, 0, "0: Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;
  (eb->neck).Freeze();
  arm->Limp();

  // loop over selected set of frames  
  eb->Beep();
  try
  {
    while (1)
    {
      // check for user interactions the get current sensor values
      if ((hit = d.AnyHit()) != 0)
      {
        if ((hit <= -3) || (state == 5))   // quit
          break;
        if (state < 6)                     // cycle 3-6
          state++;
        else
          state = 3;
      }
      if (eb->Update(0, 0) <= 0)
        break;

      // figure out what to do
      if (state <= 1)
      {
        // manhandle arm to goal
        arm->Limp();
        arm->ArmConfig(a0);
        a0.ListVec(goal, "%5.1f", 80);
        d.StringGrid(0, 0, "%d: Move arm to goal pose -- Hit any key to continue ...", state);
        d.StringBelow("Goal = %s", goal);
      }
      else if (state == 2)
      {
        // remember goal pose
        arm->CfgTarget(a0);
        a0.ListVec(goal, "%5.1f", 80);
        d.StringGrid(0, 0, "%d: Frozen at goal -- Hit any key to continue ...", state);
        d.StringBelow("Goal = %s", goal);
      }
      else if (state == 3)
      {
        // manhandle arm to start
        arm->Limp();
        arm->ArmConfig(a);
        a.ListVec(start, "%5.1f", 80);
        d.StringGrid(0, 0, "%d: Shift arm to start position -- Hit any key to continue ...", state);
        d.StringBelow("Start = %s", start);
      }
      else if (state == 4)
      {
        // remember start and pick joint rates to achieve goal pose
        n = 0;
        arm->CfgTarget(a);
        a.ListVec(start, "%5.1f", 80);
        mt0 = arm->CfgTime(a0, a);
        arm->CfgRate(rates, a0, a, mt0);
        mt = arm->CfgTime(a0, a, rates);
        d.StringGrid(0, 0, "%d: Frozen at start -- Hit any key to continue ...", state);
        d.StringBelow("Start = %s", start);
        d.StringBelow("Expect %4.2f seconds to goal", mt);
      }
      else if (state == 5)
      {
        // if not done then drive toward goal
        t = 0.001 * ms * n++;
        arm->ArmConfig(a2);
        arm->CfgErr(err, a0, 0);
        err.ListVec(diff, "%5.1f", 80); 
        if ((arm->CfgOffset(a0) <= tol) || (t > (10.0 * mt)))
          state++;
        else
          arm->CfgTarget(a0, rates);
        d.StringGrid(0, 0, "%d: Moving to goal -- Hit any key to EXIT ...", state);
        d.StringBelow("Joint errors: %s", diff);
        d.StringBelow("Elapsed %4.2f seconds -- %4.2f expected", t, mt);
      }
      else 
      {
        // goal achieved
        arm->CfgTarget(a2);
        d.StringGrid(0, 0, "%d: Finished with goal -- Hit any key to continue ...", state);
        d.StringBelow("Joint errors: %s", diff);
        d.StringBelow("Elapsed %4.2f seconds -- %4.2f expected", 0.001 * ms * n, mt);
      }

      // send command
      eb->Issue();
      jms_sleep(ms);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
}


/////////////////////////////////////////////////////////////////////////////
//                             Force Functions                             //
/////////////////////////////////////////////////////////////////////////////

// Show direction and magnitude of endpoint force

void CBanzaiDoc::OnArmHandforce()
{
  jhcEliArm *arm = &(eb->arm);
  jhcImg fxy, fz;
  jhcMatrix a0(7), fraw(4), fdir(4);
  double rng = 16.0, pel = 0.1, box = 4.0, z0 = -10.0, ang = 30.0;
  int mid, k, th, ht, y, dot = 13, wid = 70, ms = 33;

  // make up display images
  mid = ROUND(rng / pel);
  k = 2 * mid;
  fxy.SetSize(k, k);
  fz.SetSize(wid, k);
  th = ROUND(2.0 * box / pel);

  // start up robot
  d.Clear(1, "Hand forces ...");
  d.ResetGrid(3, 320, 50);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;

  // change to starting angle
  d.ClearGrid(0, 0);
  d.StringGrid(0, 0, "Pointing arm forward ...");
  eb->InitPose();
  arm->ArmConfig(a0);
  a0.VSet(1, ang);
  arm->SetConfig(a0);
  
  // loop over selected set of frames  
  eb->Beep();
  try
  {
    while (d.AnyHit() == 0)
    {
      // get current sensor values
      if (eb->Update(0, 0) <= 0)
        break;
      arm->ForceVect(fraw, z0, 1);
      arm->ForceVect(fdir, z0);

      // build xy force display image with axes and dot (raw is cross)
      fxy.FillArr(255);
      DrawLine(fxy, mid, 0, mid, k, 3, 0);
      DrawLine(fxy, 0, mid, k, mid, 3, 0);
      RectCent(fxy, mid, mid, th, th, 0.0, 1, 0);
      BlockCent(fxy, mid + ROUND(fdir.X() / pel), mid + ROUND(fdir.Y() / pel), dot, dot, 50);
      Cross(fxy, mid + fraw.X() / pel, mid + fraw.Y() / pel, dot, dot, 1, 215); 

      // build z force display image with bar and mid line (raw is cross)
      fz.FillArr(255);
      ht = ROUND(fdir.Z() / pel);
      if (fdir.Z() >= 0.0)
        RectFill(fz, 0, mid, wid, ht, 128);
      else
        RectFill(fz, 0, mid + ht, wid, -ht, 200);
      DrawLine(fz, 0, mid, wid, mid, 3, 0);
      y = mid + ROUND(box / pel);
      DrawLine(fz, 0, y, wid, y, 1, 0);
      y = mid - ROUND(box / pel);
      DrawLine(fz, 0, y, wid, y, 1, 0);
      Cross(fz, 0.5 * wid, mid + fraw.Z() / pel, dot, dot, 1, 215);
      
      // show forces
      d.ShowGrid(fxy, 0, 0, 2, "Force:  X = %5.1f    Y = %5.1f", fdir.X(), fdir.Y());
      d.ShowGrid(fz,  1, 0, 2, "wt = %4.1f", -fdir.Z());
      d.StringBelow("Z = %5.1f", fdir.Z() + z0);

      // maintain pose
      arm->ArmConfig(a0);
      eb->Issue();
      jms_sleep(ms);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
}


// Pose arm by pushing on hand

void CBanzaiDoc::OnForceDraghand()
{
  jhcEliArm *arm = &(eb->arm);
  jhcMatrix fdir(4), mv(4);
  double z0 = -10.0, xyth = 4.0, zth = 4.0;
  double xymv = 2.0, zmv = 2.0;
  int ms = 33;

  // start up robot
  d.Clear(1, "Drag arm ...");
  d.ResetGrid(3, 320, 50);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;

  // change to starting angle
  d.ClearGrid(0, 0);
  d.StringGrid(0, 0, "Assuming neutral pose ...");
  eb->InitPose();

  // loop over selected set of frames  
  eb->Beep();
  try
  {
    while (d.AnyHit() == 0)
    {
      // get current sensor values
      if (eb->Update(0, 0) <= 0)
        break;
      arm->ForceVect(fdir, z0);

      // move in direction of pull
      mv.Zero();
      if (fabs(fdir.X()) > xyth)
        mv.SetX((fdir.X() > 0.0) ? xymv : -xymv);
      if (fabs(fdir.Y()) > xyth)
        mv.SetY((fdir.Y() > 0.0) ? xymv : -xymv);
      if (fabs(fdir.Z()) > zth)
        mv.SetZ((fdir.Z() > 0.0) ? zmv : -zmv);
        
      // list forces
      d.ClearGrid(0, 0);
      d.StringGrid(0, 0, "Move arm  X = %3.1f  :  Y = %3.1f  :  Z = %3.1f", fdir.X(), fdir.Y(), fdir.Z());

      // maintain pose
      arm->ShiftTarget(mv, 0.5);
      eb->Issue();
      jms_sleep(ms);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
}


// Steer base by pulling on hand

void CBanzaiDoc::OnForceDragrobot()
{
  jhcEliArm *arm = &(eb->arm);
  jhcEliLift *lift = &(eb->lift);
  jhcEliBase *base = &(eb->base);
  jhcMatrix a0(7), fdir(4);
  double rrate, trate, zrate, dz, trans, rot;
  double sh = 15.0, elb = 30.0, z0 = -10.0, xyth = 4.0, zth = 4.0;
  double xyrng = 32.0, zrng = 48.0, mv = 6.0, turn = 15.0, elev = 1.0;
  int ms = 33;

  // start up robot
  d.Clear(1, "Drag robot ...");
  d.ResetGrid(3, 320, 50);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;

  // change to starting angle
  d.ClearGrid(0, 0);
  d.StringGrid(0, 0, "Pointing arm forward ...");
  eb->InitPose();
  arm->ArmConfig(a0);
  a0.VSet(0, sh);
  a0.VSet(1, elb);
  arm->SetConfig(a0);

  // loop over selected set of frames  
  eb->Beep();
  try
  {
    while (d.AnyHit() == 0)
    {
      // get current sensor values
      if (eb->Update(0, 0) <= 0)
        break;
      arm->ForceVect(fdir, z0);

      // move in direction of pull
      dz = 0.0;

      // turn proportional to pull
      rot = ((fdir.X() > 0.0) ? -turn : turn);
      rrate = fabs(fdir.X()) / xyrng;

      // drive proportional to pull
      trans = ((fdir.Y() > 0.0) ? mv : -mv);
      trate = fabs(fdir.Y()) / xyrng;

      // lift proportional to pull (with deadband and mode switching)
      dz = 0.0;
      zrate = 1.0;
      if ((fabs(fdir.Z()) > zth) && (fabs(fdir.X()) <= xyth) && (fabs(fdir.Y()) <= xyth))
      {
        dz = ((fdir.Z() > 0.0) ? elev : -elev);
        zrate = (fabs(fdir.Z()) - zth) / zrng;
      }
        
      // list forces
      d.ClearGrid(0, 0);
      d.StringGrid(0, 0, "Move robot  X = %3.1f  :  Y = %3.1f  :  Z = %3.1f", fdir.X(), fdir.Y(), fdir.Z());

      // maintain pose
      lift->LiftShift(dz, 0.5);
      base->TurnTarget(rot, rrate);
      base->MoveTarget(trans, trate);
      eb->Issue();
      jms_sleep(ms);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
}


/////////////////////////////////////////////////////////////////////////////
//                           Animation Functions                           //
/////////////////////////////////////////////////////////////////////////////

// Set all arm servos to passive

void CBanzaiDoc::OnArmLimp()
{
  d.Clear(1, "Setting arm passive ...");
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;
  (eb->arm).Limp();
  d.StatusText("Stopped.");  
  Tell("Arm is now limp"); 
}


// Assume default pose for all actuators

void CBanzaiDoc::OnAnimationNeutral()
{
  // start up robot
  d.Clear(1, "Neutral pose ...");
  d.ResetGrid(3, 640, 200);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;

  // change to starting angle
  d.ClearGrid(0, 0);
  d.StringGrid(0, 0, "Moving to pose ...");
  eb->InitPose();
  d.StatusText("Stopped.");  
  Tell("Robot in neutral pose"); 
}


// Show simple evidence of power on status

void CBanzaiDoc::OnAnimationIdle()
{
  double wid, mid = 1.5, dev = 0.4, tol = 0.2, rate = 0.2;  
  double tilt, tilt0 = -40.0, nod = 10.0, nrate = 0.2;
  int state = 0, ms = 33;

  // start up robot
  d.Clear(1, "Idle animation ...");
  d.ResetGrid(3, 640, 200);
  d.StringGrid(0, 0, "Initializing robot ...");  
  if (eb->Reset(1) <= 0)
    if (AskNot("Problem with robot hardware. Continue?") <= 0)
      return;
  eb->InitPose();

  // loop over selected set of frames  
  try
  {
    while (d.AnyHit() == 0)
    {
      // get current sensor values
      d.StringGrid(0, 0, "Breathing  --  Click left to exit ...");
      if (eb->Update(0, 0) <= 0)
        break;

      if (state <= 0)
      {
        // actually open the gripper
        wid = mid + dev;
        tilt = tilt0 + nod;
        (eb->arm).WidthTarget(wid, rate, 5);
        (eb->neck).TiltTarget(tilt, nrate, 5);
        if ((eb->arm).WidthErr(wid) < tol)
          state = 1;
      }
      else 
      {
        // actually close gripper
        wid = mid - dev;
        tilt = tilt0;
        (eb->arm).WidthTarget(wid, rate, 5);
        (eb->neck).TiltTarget(tilt, nrate, 5);
        if ((eb->arm).WidthErr(wid) < tol)
          state = 0;
      }

      // send command
      eb->Issue();
      jms_sleep(ms);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
  d.Clear();
}


/////////////////////////////////////////////////////////////////////////////
//                         Linguistic Interaction                          //
/////////////////////////////////////////////////////////////////////////////

// parameters for overall interaction

int CBanzaiDoc::interact_params (const char *fname)
{
  jhcParam *ps = &ips;
  int ok;

  ps->SetTag("banzai_opt", 0);
  ps->NextSpec4( &rob, 0, "Body connected");
  ps->NextSpec4( &cam, 0, "Camera available");
  ps->NextSpec4( &mic, 0, "Speech input (2 = attn)");  
  ps->NextSpec4( &spk, 0, "Read output always");
  ps->Skip(2);

  ps->NextSpec4( &(ec.acc), 0, "Accumulate knowledge");
  ps->NextSpec4( &fsave,    0, "Update face models");
  ok = ps->LoadDefs(fname);
  ps->RevertAll();
  return ok;
}


// Configure for voice, camera, etc.

void CBanzaiDoc::OnDemoDemooptions()
{
	jhcPickVals dlg;

  dlg.EditParams(ips); 
}


// Force a robot hardware reset

void CBanzaiDoc::OnDemoResetrobot()
{
  if ((cam > 0) && (ChkStream() > 0))
    (ec.body).BindVideo(&v);
  else
    (ec.body).BindVideo(NULL);
  ec.Reset(0, 2);
}


// Read successive inputs from a text file

void CBanzaiDoc::OnDemoTextfile()
{
  jhcString sel, test;
  CFileDialog dlg(TRUE);
  HWND me = GetForegroundWindow();
  FILE *f;
  char in[200] = "";

  // select file to read 
  sprintf_s(test.ch, "%s\\test\\trial.tst", cwd);
  test.C2W();
  (dlg.m_ofn).lpstrFile = test.Txt();
  (dlg.m_ofn).lpstrFilter = _T("Test Files\0*.tst\0Text Files\0*.txt\0All Files (*.*)\0*.*\0");
  if (dlg.DoModal() != IDOK)
    return;

  // get first non-blank line
  sel.Set((dlg.m_ofn).lpstrFile);    
  if (fopen_s(&f, sel.ch, "r") != 0)
    return;
  while (strlen(in) <= 1)
    if (fgets(in, 200, f) == NULL)
    {
      fclose(f);
      return;
    }

  // possibly check for video
  if ((cam > 0) && (ChkStream() > 0))
    (ec.body).BindVideo(&v);
  else
    (ec.body).BindVideo(NULL);

  // reset all required components
  jprintf_open();
  if (ec.Reset(spk, 0) <= 0)
    return;
  chat.Reset(0);
  chat.Inject(in);

  // announce start and input mode
  d.Clear(1, "File input (ESC to quit) ...");

  // keep taking sentences until ESC
  try
  {
    while (chat.Interact() >= 0)
    {
      // check for keystroke input 
      if (ec.Accept(chat.Get(in), chat.Done()))
      {
        // get next line from file
        *in = '\0';
        while (fgets(in, 200, f) != NULL)
          if (strlen(in) > 1)
          {
            chat.Inject(in);
            break;
          }
      }

      // compute response
      if (ec.Respond() <= 0)
        break;

      // show interaction
      if ((ec.body).NewFrame())
        d.ShowGrid((ec.body).View(), 0, 0, 0, "Robot view");
      chat.Post(ec.NewInput(), 1);
      chat.Post(ec.NewOutput());
    }
  }
  catch (...){Tell("Unexpected exit!");}

  // cleanup
  jprintf("\n:::::::::::::::::::::::::::::::::::::\n");
  ec.PrintMem();
  ec.Done();
  jprintf("Done.\n\n");
  fclose(f);
  jprintf("Think %3.1f Hz, Sense %3.1f Hz\n", ec.Thinking(), ec.Sensing()); 
  jprintf_close();

  // window configuration
  d.StatusText("Stopped."); 
  chat.Mute();
  SetForegroundWindow(me);
}


// Send commands and provide advice using dialog
// *** STANDARD DEMO ***

void CBanzaiDoc::OnDemoInteractive()
{
  HWND me = GetForegroundWindow();
  char in[200];
  jhcImg col(640, 480, 3);
  int src = ((mic > 0) ? (mic + 1) : spk);

  // possibly check for video
  if ((cam > 0) && (ChkStream() > 0))
    (ec.body).BindVideo(&v);
  else
    (ec.body).BindVideo(NULL);

  // reset all required components
  jprintf_open();
  d.StatusText("Connecting to robot ...");
  if (ec.Reset(src, rob) <= 0)
  {
    Complain("Robot not functioning properly");
    d.StatusText("Failed");
    return;
  }
  ec.SetPeople("VIPs.txt");
  chat.Reset(mic);

  // announce start and input mode
  if (mic > 0)
  {
    d.Clear(1, "Voice input (ESC to quit) ...");
    SetForegroundWindow(GetConsoleWindow());
  }
  else
  {
    d.Clear(1, "Text input (ESC to quit) ...");
    SetForegroundWindow(chat);
  }

  // keep taking sentences until ESC
  try
  {
    while (chat.Interact() >= 0)
    {
      // get inputs and compute response
      if (mic > 0)
        ec.Accept(NULL, chat.Done());
      else
        ec.Accept(chat.Get(in), chat.Done());
      if (ec.Respond() <= 0)
        break;

      // show interaction
      if ((ec.body).NewFrame())
        d.ShowGrid((ec.rwi).MarkUp(), 0, 0, 0, "Robot view  --  %s", (ec.rwi).Watching());
      chat.Post(ec.NewInput(), 1);
      chat.Post(ec.NewOutput());
    }
  }
  catch (...){Tell("Unexpected exit!");}

  // cleanup
  jprintf("\n:::::::::::::::::::::::::::::::::::::\n");
  ec.PrintMem();
  ec.Done(fsave);
  jprintf("Done.\n\n");
  jprintf("Think %3.1f Hz, Sense %3.1f Hz\n", ec.Thinking(), ec.Sensing()); 
  jprintf_close();

  // window configuration
  d.StatusText("Stopped."); 
  chat.Mute();
  SetForegroundWindow(me);
}


/////////////////////////////////////////////////////////////////////////////
//                               Heads & Faces                             //
/////////////////////////////////////////////////////////////////////////////

// What human activities to innately watch

void CBanzaiDoc::OnParametersWatching()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.rwi).wps); 
}


// Parameters controlling details of watching behaviors

void CBanzaiDoc::OnParametersOrienting()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.rwi).ops); 
}


// Adjust time delays and ranges of idle activities

void CBanzaiDoc::OnParametersTargettime()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.rwi).ips); 
}


// When to alert about the presense of people

void CBanzaiDoc::OnPeopleSocialevents()
{
	jhcPickVals dlg;

  dlg.EditParams((ec.soc).eps); 
}


// Supply an external mugshot for some person

void CBanzaiDoc::OnAttentionEnrollphoto()
{
  jhcFRecoDLL *fr = &(((ec.rwi).fn).fr);
  jhcFFindOCV *ff = &(((ec.rwi).fn).ff);
  char person[80] = "";
  jhcString sel, init, idir(cwd);
  CFileDialog dlg(TRUE);
  jhcPickString pick;
  jhcName fname;
  jhcImgIO jio;
  jhcImg mug, mug4;
  jhcRoi det;
  jhcImg *src;
  FILE *out;
  const jhcFaceVect *inst = NULL;

  // get ID to use
  if (pick.EditString(person, 0, "Person name") <= 0)
    return;
  if (*person == '\0')
    return;
  sprintf_s(init.ch, "%s.bmp", person);
  init.C2W(); 

  // get sample image containing face
  (dlg.m_ofn).lpstrInitialDir = idir.Txt();
  (dlg.m_ofn).lpstrFilter = _T("Image Files\0*.bmp;*.jpg\0All Files (*.*)\0*.*\0");
  (dlg.m_ofn).lpstrFile = init.Txt();
  if (dlg.DoModal() != IDOK)
    return;
  sel.Set((dlg.m_ofn).lpstrFile);
  if (jio.LoadResize(mug, sel.ch) <= 0)
    return;
  fname.ParseName(sel.ch);

  // find biggest face and try to enroll it
  ff->Reset();
  fr->Reset();
  src = Image4(mug4, mug);
  if (ff->FindBest(det, *src, 20, 400, 0.0) > 0)
  {
    inst = fr->Enroll(person, *src, det);
    RectEmpty(*src, det, 3, 255, 0, 255);
  }
  res.Clone(*src);
  sprintf_s(rname, "enroll_box.bmp");

  // show results
  d.Clear();
  d.ShowGrid(*src, 0, 0, 0, fname.Base());
  if (inst == NULL)
    return;
  d.ShowGrid(inst->thumb, 0, 1, 0, person);

  // update database
  fr->SaveDude(person);
  if ((ec.vip).Canonical(person) == NULL)
    if (Ask("Add to VIP list?") > 0)
      if (fopen_s(&out, "VIPs.txt", "a") == 0)
      {
        fprintf(out, "%s\n", person);
        fclose(out);
      }
}


// Take a picture of some particular person

void CBanzaiDoc::OnAttentionEnrolllive()
{
  jhcFRecoDLL *fr = &(((ec.rwi).fn).fr);
  jhcFFindOCV *ff = &(((ec.rwi).fn).ff);
  char person[80] = "";
  jhcPickString pick;
  jhcImg now, mark;
  jhcRoi box;
  FILE *out;
  const jhcFaceVect *inst = NULL;
  int ok;

  // check video
  if (!v.Valid())
  {
    Complain("No video source selected");
    return;
  }
  v.SizeFor(now);
  ff->Reset();
  fr->Reset();

  // get ID to use
  if (pick.EditString(person, 0, "Person name") <= 0)
    return;
  if (*person == '\0')
    return;

  // loop over selected set of frames  
  d.Clear(1, "Enroll live ...");
  v.Rewind(FASTVID);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get next frame from source
      if (v.Get(now) < 1)
        break;

      // run face finder 
      ok = ff->FindBest(box, now, 20, 400, 0.0);
 
      // draw box around face
      mark.Clone(now);
      if (ok > 0)
        RectEmpty(mark, box, 5, -3);

      // show frame on screen
      d.ShowGrid(mark, 0, 0, 0, "%s  --  Hit any to capture", person);
    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  d.StatusText("Stopped.");  
  res.Clone(mark);
  sprintf_s(rname, "enroll_box.bmp");

  // actually enroll face (no box) as instance for person
  d.ShowGrid(mark, 0, 0, 0, "%s  %s", person, ((ok <= 0) ? "--  No face detected!" : ""));
  if (ok <= 0)
    return;
  if ((inst = fr->Enroll(person, now, box)) == NULL)
    return;
  d.ShowGrid(inst->thumb, 0, 1, 0, "Enrolled");

  // update database
  fr->SaveDude(person);
  if ((ec.vip).Canonical(person) == NULL)
    if (Ask("Add to VIP list?") > 0)
      if (fopen_s(&out, "VIPs.txt", "a") == 0)
      {
        fprintf(out, "%s\n", person);
        fclose(out);
      }
}


// Continuously aim Kinect at closest detected head

void CBanzaiDoc::OnDepthTrackhead()
{
  jhcStare3D *s3 = &((ec.rwi).s3);
  jhcImg map, col;
  jhcMatrix cam(4), dir(4), targ(4);
  double pan = 0.0, tilt = 0.0, dist = 0.0, side = 50.0, turn = 30.0;
  int index, id;

  // make sure video is working
  if (!ChkStream())
    return;

  // initialize robot system
  if (ec.Reset(0, 1) < 2)
    return;

  // loop over selected set of frames  
  d.Clear(1, "Head track ...");
  v.Rewind(1);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get new sensor data and analyze it
      if ((ec.rwi).Update() <= 0)
        break;

      // if no head then return to default
      if ((index = s3->Closest()) < 0)
        (eb->neck).GazeTarget(0.0, -15.0);    
      else
      {
        // find target angles
        (eb->neck).HeadPose(cam, dir, (eb->lift).Height());
        s3->Head(targ, index);
        cam.PanTilt3(pan, tilt, targ);
        pan -= 90.0;
        dist = targ.PlaneVec3();

        // move robot head and possibly feet
        (eb->neck).GazeTarget(pan, tilt);
        if (pan > side)
          (eb->base).TurnTarget(turn);
        else if (pan < -side)
          (eb->base).TurnTarget(-turn);
      }

      // make pretty pictures
      map.Clone(s3->map);
      col.Clone(eb->Color());
      s3->CamZone(map, 0);
      if (index >= 0)
      {
        id = s3->PersonID(index);
        s3->ShowID(map, id);
        s3->ShowIDCam(col, id);
      }

      // send any commands and start collecting next input
      (ec.rwi).Issue();

      // show pair and overhead map
      d.ShowGrid(map, 0, 0, 2, "Overhead map");
      if (index >= 0)
        d.ShowGrid(col, 0, 1, 0, "Selected head (%+d %+d) @ %3.1f in\n", ROUND(pan), ROUND(tilt), dist);
      else
        d.ShowGrid(col, 0, 1, 0, "No heads");

    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  ec.Done();
  d.StatusText("Stopped.");  
  FalseClone(res, map);
  sprintf_s(rname, "%s_pmap.bmp", v.FrameName());
}


// Beeps if user is gazing at robot

void CBanzaiDoc::OnDepthPersonmap()
{
  jhcStare3D *s3 = &((ec.rwi).s3);
  jhcFaceName *fn = &((ec.rwi).fn);
  jhcEliBase *b = (ec.rwi).base;
  jhcImg map, col;
  jhcMatrix pos(4), dir(4);
  const jhcBodyData *p;
  int i;

  // make sure video is working
  if (!ChkStream())
    return;

  // initialize robot system
  if (ec.Reset(0, 1) < 2)
    return;
  ec.SetPeople("VIPs.txt");
  (eb->neck).Limp();

  // loop over selected set of frames  
  d.Clear(1, "Head gaze ...");
  v.Rewind(1);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get new sensor data and analyze it
      if ((ec.rwi).Update() <= 0)
        break;

      // announce eye contact (GazeMax better than FrontMax)
      if (fn->GazeMax() >= 1)
      {
        b->ForceLED(1);
        PlaySound(TEXT("beep.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_LOOP);
      }
      else
      {
        b->ForceLED(0);
        PlaySound(NULL, NULL, SND_ASYNC);
      }

      // see if new person found or new face example added
      if ((i = fn->JustNamed()) >= 0)
      {
        p = s3->GetPerson(i);
        jprintf("Just determined person %d is %s\n", i, p->tag);
      }
      if ((i = fn->JustUpdated()) >= 0)
      {
        p = s3->GetPerson(i);
        jprintf("+ Added new image for %s\n", p->tag);
      }   

      // make pretty pictures
      map.Clone(s3->map);
      s3->CamLoc(map, 0);
      s3->AllHeads(map);
      fn->AllGaze(map);
      col.Clone(eb->Color());
      s3->HeadsCam(col, 0, 1, 0, 8.0, 3);
      fn->FacesCam(col);

      // send any commands and start collecting next input
      (ec.rwi).Issue();

      // show pair and overhead map
      d.ShowGrid(map, 0, 0, 2, "Overhead gaze angle");
      d.ShowGrid(col, 0, 1, 0, "Faces wrt heads");

    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  b->ForceLED(0);
  PlaySound(NULL, NULL, SND_ASYNC);
  ec.Done();
  d.StatusText("Stopped."); 
  res.Clone(col); 
  sprintf_s(rname, "%s_heads.bmp", v.FrameName());
}


// Determine which person is talking

void CBanzaiDoc::OnPeopleSpeaking()
{
  jhcStare3D *s3 = &((ec.rwi).s3);
  jhcSpeaker *tk = &((ec.rwi).tk);
  jhcImg map, col;
  jhcMatrix pos(4), dir(4);
  int spk;

  // make sure video is working
  if (!ChkStream())
    return;

  // initialize robot system
  if (ec.Reset(2, 1) < 2)
    return;
  ec.SetPeople("VIPs.txt");
  (eb->neck).Limp();

  // loop over selected set of frames  
  d.Clear(1, "Sound source ...");
  v.Rewind(1);
  try
  {
    while (!d.LoopHit(v.StepTime()))
    {
      // get new sensor data and analyze it
      ec.UpdateSpeech();
      if ((ec.rwi).Update(ec.SpeechRC()) <= 0)
        break;
      spk = tk->Speaking();

      // make pretty overhead picture
      map.Clone(s3->map);
      s3->CamLoc(map, 0);
      s3->AllHeads(map);
      if (spk >= 0)
        s3->ShowID(map, spk);
      if (ec.SpeechRC() > 0)
        tk->SoundMap(map);

      // make pretty front picture
      col.Clone(eb->Color());
      s3->HeadsCam(col);
      if (spk >= 0)
        s3->ShowIDCam(col, spk);
      if (ec.SpeechRC() > 0)
        tk->SoundCam(col, 0, 0, 2);
      tk->SoundCam(col);

      // send any commands and start collecting next input
      (ec.rwi).Issue();

      // show pair and overhead map
      d.ShowGrid(map, 0, 0, 2, "Overhead direction  --  speaker = ");
      d.ShowGrid(col, 0, 1, 0, "Speaker and others  --  watching %s", (ec.rwi).Watching());

    }
  }
  catch (...){Tell("Unexpected exit!");}
  v.Prefetch(0);
  ec.Done();
  d.StatusText("Stopped."); 
  FalseClone(res, map); 
  sprintf_s(rname, "%s_beam.bmp", v.FrameName());
}


/////////////////////////////////////////////////////////////////////////////
//                              Test Functions                             //
/////////////////////////////////////////////////////////////////////////////

// test function for current fragment of code

void CBanzaiDoc::OnUtilitiesTest()
{

}



