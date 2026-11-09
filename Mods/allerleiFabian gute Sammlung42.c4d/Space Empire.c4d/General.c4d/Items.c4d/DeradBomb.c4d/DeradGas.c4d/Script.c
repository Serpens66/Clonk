/*-- Derad-Gas --*/

#strict 3

local IsBubble;

protected func Initialize(...) {
  SetAction("PostInit");
  SetComDir(COMD_Up);
  return(1);
}

protected func PostInit(...) {
  // Gas oder Blase einstellen
  IsBubble = InLiquid();
  if (IsBubble) SetAction("Bubble");
           else SetAction("Smoke");
  SetPhase(Random(4));
  if (IsBubble) SetXDir((Random(25) ?? 0)-12);
           else SetXDir((GetWind() ?? 0)/4 + (Random(51) ?? 0)-25);
  return(1);
}

private func Wind2Float(...) {
  // Wechsel nötig?
  if (!Equal(IsBubble, InLiquid()))
  {
    var phase = GetPhase();
    IsBubble = InLiquid();
    if (IsBubble) SetAction("Bubble");
             else SetAction("Smoke");
    SetPhase(phase);
  }
  if (Random(3)) return(1);
  // Lebewesen schaden
  var target = FindObject(nil,-8,-8,16,16, OCF_Alive,nil,nil,NoContainer());
  if (target) DoEnergy(-1,target);
  // Strahlung schwächen
  target = FindObject(SRAD,-8,-8,16,16);
  if (target) ObjectCall(target,"ReduceRadiation",965);
  // Strömung oder Wind
  if (IsBubble) SetXDir((Random(25) ?? 0)-12);
           else SetXDir((GetWind() ?? 0)/4 + (Random(51) ?? 0)-25);
  return(1);
}

private func Remove(...) {
  return(RemoveObject());
}
  
