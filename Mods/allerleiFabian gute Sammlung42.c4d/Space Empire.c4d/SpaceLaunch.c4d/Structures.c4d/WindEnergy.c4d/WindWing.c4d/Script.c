/*-- Windrad --*/

#strict 3


/* Initialisierung */

protected func Completion()
{
  SetR(122);
}

protected func Destruction()
{
  // MotionBlur-Effekt löschen
  var obj;
  while (obj = FindObject(SWWF, nil,nil,nil,nil, nil,"Fade",this()))
    RemoveObject(obj);
}

public func Init(windenergy)
{
  // wird von der Windkraftanlage aufgerufen
  SetAction("Turn", windenergy);
}


/* Action-Calls */

protected func Wind2Turn()
{
  var windenergy = GetActionTarget();
  // EMP-Einwirkung?
  if (FindObject(SEMP, nil,nil,nil,nil, nil,"Active",windenergy))
  {
    SetRDir(nil);
    DoEnergy(-100, windenergy);
    return(1);
  }
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(windenergy)),windenergy);
  // Energie erzeugen
  DoEnergy(Abs((GetRDir() ?? 0)*3), windenergy);
  SetRDir((GetWind() ?? 0)/4);
  // Update für Energieanzeige
  ShowInfoBar(IBRE, windenergy, Abs(GetWind()),100, nil,1);
}

private func CreateFading()
{
  // MotionBlur-Effekt erzeugen
  CreateObject(SWWF,nil,nil,GetOwner())->Init(this(), GetR());
}

