// jhcSocial.h : interface to ELI people tracking kernel for ALIA system
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

#ifndef _JHCSOCIAL_
/* CPPDOC_BEGIN_EXCLUDE */
#define _JHCSOCIAL_
/* CPPDOC_END_EXCLUDE */

#include "jhcGlobal.h"

#include "Reasoning/jhcAliaNote.h"     // common audio
#include "Semantic/jhcAliaDesc.h" 

#include "Data/jhcParam.h"             // common video

#include "Action/jhcTimedFcns.h"       // common robot
#include "Eli/jhcEliGrok.h"       


//= Interface to ELI people tracking kernel for ALIA system.
//   DO: allows user to briefely "look" at someone, or continuously "watch" someone
//       also accepts the commands "come here", "approach X", and "follow X"
//  CHK: can determine if a particular someone is visible 
// FIND: can tell who is currently visible and who is closest
// NOTE: spontaneously volunteers "I see X" for people on the face reco VIP list
//       and produces the event "X is close" even if no name is known

class jhcSocial : public jhcTimedFcns
{
// PRIVATE MEMBER VARIABLES
private:
  // link to hardware
  jhcEliGrok *rwi;

  // reported events
  jhcAliaNote *rpt;

  // event state
  int hwin, told;


// PUBLIC MEMBER VARIABLES
public:
  // controls diagnostic messages
  int dbg;                   

  // attention parameters
  jhcParam aps;
  double pnear, ltol, lquit;

  // motion parameters 
  jhcParam mps;
  double cozy, aquit, ideal, worry, ttime, orient, atime, ftime;


// PUBLIC MEMBER FUNCTIONS
public:
  // creation and initialization
  ~jhcSocial (); 
  jhcSocial ();
  void Platform (jhcEliGrok *robot);

  // processing parameter bundles 
  int Defaults (const char *fname =NULL);
  int SaveVals (const char *fname) const;


// PRIVATE MEMBER FUNCTIONS
private:
  // processing parameters
  int attn_params (const char *fname);
  int move_params (const char *fname);

  // overridden virtuals
  void local_reset (jhcAliaNote *top);
  void local_volunteer ();
  int local_start (const jhcAliaDesc *desc, int i);
  int local_status (const jhcAliaDesc *desc, int i);

  // reported events
  void vip_seen ();
  void vip_close ();
  void person_prop (int i, const char *prop) const;
  void add_name (jhcAliaDesc *n, const char *name) const;

  // looking at/for people
  JCMD_DEF(soc_look);
  int get_dude (const jhcAliaDesc *agt) const;

  // moving toward people
  JCMD_DEF(soc_approach);
  JCMD_DEF(soc_follow);


};


#endif  // once




