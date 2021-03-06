// jhcGraphlet.h : a collection of specific semantic nodes and links
//
// Written by Jonathan H. Connell, jconnell@alum.mit.edu
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright 2017-2020 IBM Corporation
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

#ifndef _JHCGRAPHLET_
/* CPPDOC_BEGIN_EXCLUDE */
#define _JHCGRAPHLET_
/* CPPDOC_END_EXCLUDE */

#include "jhcGlobal.h"

#include <stdio.h>                     // needed for FILE
#include <string.h>

#include "Semantic/jhcBindings.h"      // common audio
#include "Semantic/jhcNetNode.h"       
#include "Semantic/jhcNodeList.h"


//= A collection of specific semantic nodes with links.
// this is just a grouping, not necessarily the nodes themselves
// most important node in collection is called "main" (always the first)
// graphlets can only by read using jhcNodePool and BuildIn
// <pre>
// For example to encode "Jim, who was not very fat, saw the big dog":
//
//  jhcNodePool p;
//  jhcGraphlet g;
//  jhcNetNode *jim, *fat, *dog, *ako, *big, *act;
//
//  p.BuildIn(&g);
//  jim = p.MakeNode("obj"); 
//  p.AddProp(jim, "tag", "Jim");
//  fat = p.AddProp(jim, "hq", "fat", 1);
//  fat->SetBelief(0.70);
//  p.AddProp(fat, "mod", "very");
//
//  dog = p.MakeNode("obj");
//  ako = p.AddProp(dog, "ako", "dog");
//  ako->SetBelief(0.879);
//  big = p.AddProp(dog, "hq", "big");
//  big->AddArg("wrt", ako);
//  big->SetBelief(0.69);
//
//  act = p.MakeNode("act", "saw");
//  act->AddArg("agt", jim);
//  act->AddArg("obj", dog);
//  act->SetBelief(1.0);
//  g.SetMain(act);
//
// g.Print() is relation-focussed and yields:
//
//    act-8 -lex-   saw
//          -agt--> obj-1
//          -obj--> obj-5
//    tag-2 -lex-   Jim
//          -tag--> obj-1
//     hq-3 -lex-   fat
//          -neg-   1
//          -blf-   0.7000
//          -hq---> obj-1
//    mod-4 -lex-   very
//          -mod-->  hq-3
//    ako-6 -lex-   dog
//          -blf-   0.8790
//          -ako--> obj-5
//     hq-7 -lex-   big
//          -blf-   0.6900
//          -hq---> obj-5
//          -wrt--> ako-6
//
// </pre>

class jhcGraphlet : public jhcNodeList
{
// PRIVATE MEMBER VARIABLES
private:
  static const int gmax = 50;        /** Maximum nodes in local graph. */

  // basic description
  jhcNetNode *desc[gmax];
  int ni;

  // empty string
  char blank[1];


// PUBLIC MEMBER FUNCTIONS
public:
  // creation and initialization
  ~jhcGraphlet ();
  jhcGraphlet ();
  int MaxItems () const {return gmax;}
  void Clear () {ni = 0;}

  // description access
  bool Full () const {return(ni >= gmax);}
  int NumItems () const {return ni;}
  jhcNetNode *Item (int i) const
    {return(((i >= 0) && (i < ni)) ? desc[i] : NULL);}
  jhcNetNode *Main () const    {return Item(0);}
  const char *MainTag () const {return((ni <= 0) ? blank : Main()->Tag());}
    
  // list access (overrides virtual)
  jhcNetNode *NextNode (const jhcNetNode *prev =NULL) const;
  int Length () const {return NumItems();}
  bool InList (const jhcNetNode *n) const;

  // configuration
  void Copy (const jhcGraphlet& ref);
  int CopyBind (const jhcGraphlet& ref, const jhcBindings& sub);
  jhcNetNode *AddItem (jhcNetNode *item);
  jhcNetNode *SetMain (jhcNetNode *main);
  int ReplaceMain (jhcNetNode *main);
  jhcNetNode *MainLast ();
  jhcNetNode *MainProp ();
  bool InDesc (const jhcNetNode *item) const
    {return InList(item);}
  int ActualizeAll (int ver) const;

  // writing functions
  int Save (FILE *out, int lvl =0, int detail =0) const;
  int Print (int lvl =0, int detail =0) const
    {return Save(stdout, lvl, detail);}


// PRIVATE MEMBER FUNCTIONS
private:
  // configuration 
  bool in_list (const jhcNetNode *n, int rng =0) const;

  // writing functions
  int save_props (FILE *out, int lvl, int kmax, int nmax, int rmax);


};


#endif  // once




