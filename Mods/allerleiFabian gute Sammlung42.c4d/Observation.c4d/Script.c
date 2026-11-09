/*-- Beobachtungsturm --*/

#strict 3
#include DOOR

// Local(0) = Der Clonk im Turm
// Local(1) = Nachricht an/aus
// Local(8) = Der Niedergang
// Local(9) = Das Fundament

/* Fundament (Local 9) und Niedergang (Local 8) */

protected func Initialize(...) {
 SetLocal(8,CreateObject(AB7U,nil,-18,GetOwner()));
 SetLocal(1,1);
 return(1);
}

protected func Construction(...) {
 SetLocal(9,CreateObject(BAS2,nil,+8));
 return(1);
}
 
/* Türsteuerung */

protected func ActivateEntrance(...) {
 if (Hostile(GetOwner(Par(nil)),GetOwner())) return(Message("%s verfeindet.|Eingang versperrt!", this(), GetPlayerName(GetOwner())));
 if (ActIdle()) SetAction("OpenDoor");
 return(1);
}

/* Menü */

protected func ContainedUp(...) {
 SetLocal(nil, Par());
 CreateMenu(BT7U, Local(nil));
 AddMenuItem("%s auf die Plattform bringen", "TransferClonk", GetID(Local(nil)), Local(nil));
 AddMenuItem("Objekte auf die Plattform bringen", "Transfer", RSR3, Local(nil));
 if (Equal(Local(1), 1)) AddMenuItem("Signal ausstellen", "SignalOff", BT7U, Local(nil));
 if (Equal(Local(1), nil)) AddMenuItem("Signal einstellen", "SignalOn", BT7U, Local(nil));
 while (SetVar(nil, FindObject(nil,-275,-125,550,250,OCF_CrewMember,nil,nil,nil, Var(nil)) ))
  if (Hostile(GetOwner(Var(nil)),GetOwner(this())))
   AddMenuItem("gesichteten Gegner beobachten", "Search", nil, Local(nil), nil, Var(nil));
 return(1);
}

/* Clonk auf die Plattform versetzen */

protected func TransferClonk(...) {
 Exit(Local(nil), nil,-18,nil, nil);
 return(1);
}

/* Objekte auf die Plattform versetzen */

func Transfer(...) {
 // Menu erzeugen
 CreateMenu(RSR3,Local(nil),this(),nil,"Beobachtungsturm ist leer",nil,nil,nil );
 // Menueinträge
 SetVar(3,-1);
 while(And(
   LessThan(
     Var(nil),
     ContentsCount()
   ),
   Not(IncVar(3))
 ))
   if(S42MigrationFirst(Not(BitAnd(     GetCategory( Contents(Var(nil)) ),     C4D_Living   )),
       SetVar(1,GetID( Contents(Var(nil)) )),
       SetVar(2,ContentsCount(Var(1))),
       SetVar(nil,Sum( Var(nil),Var(2) )),
       SetVar(3,-1)   ))
     AddMenuItem(
       "%s auf die Plattform bringen",
       "MoveOut",
       Var(1),
       Local(nil),
       Var(2),
       Local(nil)
     );
 SelectMenuItem(Par(1),Local(nil));
 return(1);
}

func MoveOut(...) {
 if(Not(Equal( Contained(Par(1)),this() ))) return;
 SetVar(nil,-1);
 SetVar(1,Contents());
 SetVar(2,nil);
 while(And(
   SetVar(3,Contents(IncVar())),
   Not(Equal( GetID(Var(3)),Par() ))
 ))
   if(Not(BitAnd( GetCategory(Var(3)),C4D_Living )))
     if(Not(Equal( GetID(Var(3)),Var(1) )))
       And(
         SetVar(1,GetID(Var(3))),
         IncVar(2)
       );
  if(Not(BitAnd( GetCategory(FindContents( Par() )), C4D_Living )))
   SetVar(4,-2);
 Exit(FindContents(Par()),nil,-18);
  if(And(
   Not( FindContents(Par()) ),
   Not( Contents( Sub(Var(),Var(9),-1) ) )
 ))
   DecVar(2);
  Transfer(Par(1),Var(2));
 return(1);
}

/* Signal an/ausstellen */

func SignalOff(...) {
 Local(1) = nil;
 Message("Du wirst nun|nicht mehr|informiert wenn|dich jemand angreift!", this());
 Sound("Click");
 return(1);
}

func SignalOn(...) {
 Local(1) = 1;
 Message("Du wirst nun|wieder informiert wenn|dich jemand angreift!", this());
 Sound("Click");
 return(1);
}

/* in der nähe gesichteten Gegner beobachten */

func Search(...) {
 SetPlrView(GetOwner(), Par(1));
 return(1);
}

/* Signal */

func Signal(...) {
 // Als erstes wird gekuckt ob überhaupt das Signal gesendet werden soll
 if (Equal(Local(1), nil)) return(1);
 // Prüft ob ein Feind die durch den Turm gekennzeichnete Grenze überschreitet
 while (SetVar(nil, FindObject(nil,nil,Sub(nil,GetY()),20,LandscapeHeight(),OCF_CrewMember,nil,nil,nil, Var(nil)) ))
  if (Hostile(GetOwner(Var(nil)),GetOwner(this())))
   And(Sound("Trumpet", 1), Message("Spieler %s|greift uns an!",GetCursor(GetOwner()),GetPlayerName(GetOwner(Var(nil)))));
 return(1);
}