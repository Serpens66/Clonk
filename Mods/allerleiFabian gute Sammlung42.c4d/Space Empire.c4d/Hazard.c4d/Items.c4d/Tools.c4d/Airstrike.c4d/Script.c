/*-- Airstrike --*/

#strict 3

//wurde der Airstrike aktiviert?
local launched;

//Someone set up us the bomb
func Activate(pObject) {
    SetOwner(GetController(pObject),this());
      if(!PathFree(GetX(),GetY(),GetX(),nil)) {return(Message("$RecievingBlocked$",this()));}
    Exit(this());
    //Oh, we get Signal
    Sound("Morse");
    
    //All your base..
    launched = true;
    SetVisibility((VIS_Allies ?? 0)+(VIS_Owner ?? 0),this());
    SetAction("Waiting");
    SetDir(GetDir(pObject));
    
    ScheduleCall(this(),"CallSupport",50);
	return(1);
}

//..are belong to us
func CallSupport()
{
  Sound("Siren",1);
  ScheduleCall(this(),"Boom",20);
}

//You are on the way to destruction
func Boom()
{
  var dir = (GetDir() ?? 0)*2-1;
  Schedule(Format("CreateObject(ABMB,RandomX(-100,100)-100*%d,AbsY(-25),%d)->Init(%d)",dir,GetOwner(),dir),30,15,this());
  FadeOut(this());
}

func RejectEntrance()
{
  return(launched);
}

// KI-Behandlung
protected func AI_Inventory(object pClonk)
{
  // Nur benutzen, wenn es geht
  if(!PathFree(GetX(),GetY(),GetX(),nil)) return;
  // Benutzen (verzögert)
  ScheduleCall(this, "Activate", 1, nil, pClonk);
  // übernehmen wir
  return(1);
}
