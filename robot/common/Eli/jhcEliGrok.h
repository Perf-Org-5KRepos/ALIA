// jhcEliGrok.h : post-processed sensors and innate behaviors for ELI robot
//
// Written by Jonathan H. Connell, jconnell@alum.mit.edu
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 IBM Corporation
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

#ifndef _JHCELIGROK_
/* CPPDOC_BEGIN_EXCLUDE */
#define _JHCELIGROK_
/* CPPDOC_END_EXCLUDE */

#include "jhcGlobal.h"

#include "Body/jhcEliBody.h"           // common robot
#include "People/jhcFaceName.h"
#include "People/jhcSpeaker.h"
#include "People/jhcStare3D.h"

#include "Body/jhcBackgRWI.h"


//= Post-processed sensors and innate behaviors for ELI robot.
// holds basic body control and sensors as well as follow-on processing modules
// generally processing belongs here while links to FCNs are in a kernel class
// allows attachment of different versions of body but assumes not shared

class jhcEliGrok : public jhcBackgRWI
{
// PRIVATE MEMBER VARIABLES
private:
  jhcImg mark;
  UL32 tnow;
  int seen;

  // target watching control
  char wtarg[9][20];
  jhcMatrix src;
  double prand, trand;
  int twin, gwin, fwin, hwin, seek, delay;
  UL32 twait, swait, gwait, fwait, hwait, rwait, idle;


// PUBLIC MEMBER VARIABLES
public:
  // robot and subcomponents
  jhcEliBody *body;                    // allow physical or simulator
  jhcEliBase *base;
  jhcEliNeck *neck;
  jhcEliLift *lift;
  jhcEliArm  *arm;
  jhcDirMic  *mic;

  // processing elements
  jhcStare3D s3;                       // head finder using depth
  jhcFaceName fn;                      // face ID and gaze for heads
  jhcSpeaker tk;                       // sound location vs head

  // watching behaviors bids
  jhcParam wps;
  int freeze, speak, sound, close, stare, face, twitch, neutral;

  // self-orientation parameters
  jhcParam ops;
  double bored, edge, hnear, hfar, dwell, side, tfix;
  int fmin;

  // idle activity parameters
  jhcParam ips;
  double center, aim, relax, rdev, pdist, pht;
  

// PUBLIC MEMBER FUNCTIONS
public:
  // creation and initialization
  ~jhcEliGrok ();
  jhcEliGrok ();
  void BindBody (jhcEliBody *b);
  const jhcImg *MarkUp () const {return &mark;}
  const char *Watching () const;

  // processing parameter bundles 
  int Defaults (const char *fname =NULL);
  int LoadCfg (const char *fname =NULL);
  int SaveVals (const char *fname);
  int SaveCfg (const char *fname) const;

  // main functions
  void Reset ();
  int Update (int voice =0, UL32 resume =0);
  void Stop ();


// PRIVATE MEMBER FUNCTIONS
private:
  // creation and initialization
  void clr_ptrs ();
  
  // processing parameters
  int watch_params (const char *fname);
  int sound_params (const char *fname);
  int orient_params (const char *fname);
  int idle_params (const char *fname);

  // interaction overrrides
  void body_update ();
  void body_issue ();

  // innate behaviors
  void cmd_freeze ();
  void watch_talker ();
  void gaze_sound ();
  void gaze_stare ();
  void gaze_face ();
  void watch_closest ();
  void gaze_random ();
  void head_neutral ();
  void set_target (int& targ, UL32& timer, int i, int th =0) const;
  void orient_toward (const jhcMatrix *targ, int bid);

  // debugging graphics
  void interest_img ();


};


#endif  // once




