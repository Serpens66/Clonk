/*-- Schalter --*/

#strict

local pGate;
local dir,wand;

protected func Initialize()
{
  if (FindObject(HN0D,-110,-140,20,140))
    pGate = FindObject(HN0D,-110,-140,20,140);
  else if (FindObject(HN0D,90,-140,20,140))
    pGate = FindObject(HN0D,90,-140,20,140);
  SetAction("Right");
  dir = 2;
  return(1);
}

func Set(hangar)
{
wand = hangar;
}

protected func ControlLeft(object pClonk)
{
  if (dir == 2 && pGate)
  {
    Sound("Click");
    SetAction("Left");
    ObjectSetAction(pGate,"Open");
    SetPhase(GetPhase(pGate),pGate);
    dir = 1;
  }
//Wenn Tor zerstˆrt, ist es wieder aufbaubar
if(!pGate)
{
  // Rohstoffe gebraucht gebraucht
  if (!FindContents(ROCK) && !FindContents(CTIT))
    return(Message("Benˆtigt|1x Stein|1x Titan",this()));
  if (!FindContents(CTIT))
    return(Message("Benˆtigt noch|1x Titan",this()));
  if (!FindContents(ROCK))
    return(Message("Benˆtigt noch|1x Stein",this()));
  // beides verbrauchen
  RemoveObject(FindContents(CTIT));
  RemoveObject(FindContents(ROCK));
  // nochmal SetModules von der Hangarwand aufrufen, wird in wand gemacht mit timer
wand->~SetModules();
RemoveObject();
}
  return(1);
}

protected func ControlRight(object pClonk)
{
  if (dir == 1 && pGate)
  {
    Sound("Click");
    SetAction("Right");
    ObjectSetAction(pGate,"Close");
    SetPhase(GetPhase(pGate),pGate);
    dir = 2;
  }
//Wenn Tor zerstˆrt, ist es wieder aufbaubar
if(!pGate)
{
  // Rohstoffe gebraucht gebraucht
  if (!FindContents(ROCK) && !FindContents(CTIT))
    return(Message("Benˆtigt|1x Stein|1x Titan",this()));
  if (!FindContents(CTIT))
    return(Message("Benˆtigt noch|1x Titan",this()));
  if (!FindContents(ROCK))
    return(Message("Benˆtigt noch|1x Stein",this()));
  // beides verbrauchen
  RemoveObject(FindContents(CTIT));
  RemoveObject(FindContents(ROCK));
  // nochmal SetModules von der Hangarwand aufrufen, wird in wand gemacht mit timer
wand->~SetModules();
RemoveObject();
}
  return(1);
}


// auﬂer Baumaterial nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=ROCK && def!=CTIT) return(1); }