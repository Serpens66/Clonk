/*-- Beobachtungsturm --*/

#strict
#include DOOR

// Local(0) = Der Clonk im Turm
// Local(1) = Nachricht an/aus
// Local(8) = Der Niedergang
// Local(9) = Das Fundament

/* Fundament (Local 9) und Niedergang (Local 8) */

protected Initialize:
 SetLocal(8,CreateObject(AB7U,0,-18,GetOwner()));
 SetLocal(1,1);
 return(1);

protected Construction:
 SetLocal(9,CreateObject(BAS2,0,+8));
 return(1);
 
/* Türsteuerung */

protected ActivateEntrance:
 if (Hostile(GetOwner(Par(0)),GetOwner())) return(Message("%s verfeindet.|Eingang versperrt!", this(), GetPlayerName(GetOwner())));
 if (ActIdle()) SetAction("OpenDoor");
 return(1);

/* Menü */

protected ContainedUp:
 SetLocal(0, Par());
 CreateMenu(BT7U, Local(0));
 AddMenuItem("%s auf die Plattform bringen", "TransferClonk", GetID(Local(0)), Local(0));
 AddMenuItem("Objekte auf die Plattform bringen", "Transfer", RSR3, Local(0));
 if (Local(1)==1) AddMenuItem("Signal ausstellen", "SignalOff", BT7U, Local(0));
 if (Local(1)==0) AddMenuItem("Signal einstellen", "SignalOn", BT7U, Local(0));
 while (SetVar(0, FindObject(0,-275,-125,550,250,OCF_CrewMember(),0,0,0, Var(0)) ))
  if (Hostile(GetOwner(Var(0)),GetOwner(this())))
   AddMenuItem("gesichteten Gegner beobachten", "Search", 0, Local(0), 0, Var(0));
 return(1);

/* Clonk auf die Plattform versetzen */

protected TransferClonk:
 Exit(Local(0), 0,-18,0, 0);  
 return(1);

/* Objekte auf die Plattform versetzen */

Transfer:
 // Menu erzeugen
 CreateMenu(RSR3,Local(0),this(),0,"Beobachtungsturm ist leer",0,0,0 );
 // Menueinträge
 SetVar(3,-1);
 while(And(
   LessThan(
     Var(0),
     ContentsCount()
   ),
   Not(IncVar(3))
 ))
   if(Not(BitAnd(     GetCategory( Contents(Var(0)) ),     C4D_Living()   )),
       SetVar(1,GetID( Contents(Var(0)) )),
       SetVar(2,ContentsCount(Var(1))),
       SetVar(0,Sum( Var(0),Var(2) )),
       SetVar(3,-1)   )
     AddMenuItem(
       "%s auf die Plattform bringen",
       "MoveOut",
       Var(1),
       Local(0),
       Var(2),
       Local(0)
     );
 SelectMenuItem(Par(1),Local(0));
 return(1);

MoveOut:
 if(Not(Equal( Contained(Par(1)),this() ))) return();
 SetVar(0,-1);
 SetVar(1,Contents());
 SetVar(2,0);
 while(And(
   SetVar(3,Contents(IncVar())),
   Not(Equal( GetID(Var(3)),Par() ))
 ))
   if(Not(BitAnd( GetCategory(Var(3)),C4D_Living() )))
     if(Not(Equal( GetID(Var(3)),Var(1) )))
       And(
         SetVar(1,GetID(Var(3))),
         IncVar(2)
       );
  if(Not(BitAnd( GetCategory(FindContents( Par() )), C4D_Living() )))
   SetVar(4,-2);
 Exit(FindContents(Par()),0,-18);
  if(And(
   Not( FindContents(Par()) ),
   Not( Contents( Sub(Var(),Var(9),-1) ) )
 ))
   DecVar(2);
  Transfer(Par(1),Var(2));
 return(1);

/* Signal an/ausstellen */

SignalOff:
 Local(1) = 0;
 Message("Du wirst nun|nicht mehr|informiert wenn|dich jemand angreift!", this());
 Sound("Click");
 return(1);

SignalOn:
 Local(1) = 1;
 Message("Du wirst nun|wieder informiert wenn|dich jemand angreift!", this());
 Sound("Click");
 return(1);

/* in der nähe gesichteten Gegner beobachten */

Search:
 SetPlrView(GetOwner(), Par(1));
 return(1);

/* Signal */

Signal:
 // Als erstes wird gekuckt ob überhaupt das Signal gesendet werden soll
 if (Local(1)==0) return(1);
 // Prüft ob ein Feind die durch den Turm gekennzeichnete Grenze überschreitet
 while (SetVar(0, FindObject(0,0,Sub(0,GetY()),20,LandscapeHeight(),OCF_CrewMember(),0,0,0, Var(0)) ))
  if (Hostile(GetOwner(Var(0)),GetOwner(this())))  
   Sound("Trumpet", 1)&&Message("Spieler %s|greift uns an!",GetCursor(GetOwner()),GetPlayerName(GetOwner(Var(0))));
 return(1);