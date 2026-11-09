/*-- Derad-Gas --*/

#strict

local IsBubble;

protected Initialize:
  SetAction("PostInit");
  SetComDir(COMD_Up());
  return(1);

protected PostInit:
  // Gas oder Blase einstellen
  IsBubble = InLiquid();
  if (IsBubble) SetAction("Bubble");
           else SetAction("Smoke");
  SetPhase(Random(4));
  if (IsBubble) SetXDir(Random(25)-12);
           else SetXDir(GetWind()/4 + Random(51)-25);
  return(1);

private Wind2Float:
  // Wechsel nötig?
  if (IsBubble != InLiquid())
  {
    var phase = GetPhase();
    IsBubble = InLiquid();
    if (IsBubble) SetAction("Bubble");
             else SetAction("Smoke");
    SetPhase(phase);
  }
  if (Random(3)) return(1);
  // Lebewesen schaden
  var target = FindObject(0,-8,-8,16,16, OCF_Alive(),0,0,NoContainer());
  if (target) DoEnergy(-1,target);
  // Strahlung schwächen
  target = FindObject(SRAD,-8,-8,16,16);
  if (target) ObjectCall(target,"ReduceRadiation",965);
  // Strömung oder Wind
  if (IsBubble) SetXDir(Random(25)-12);
           else SetXDir(GetWind()/4 + Random(51)-25);
  return(1);

private Remove:
  return(RemoveObject());
  
