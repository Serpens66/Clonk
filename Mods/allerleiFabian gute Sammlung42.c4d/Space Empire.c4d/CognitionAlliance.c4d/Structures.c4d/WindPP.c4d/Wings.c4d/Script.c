/*-- Propellerflügel --*/

#strict 3


protected func Destruction()
{
  // MotionBlur-Effekt löschen
  var obj;
  while (obj = FindObject(CWPF, nil,nil,nil,nil, nil,"Fade",this()))
    RemoveObject(obj);
}

public func Init(plant)
{
  SetAction("Turn",plant);
  return(1);
}

protected func CheckWind()
{
  var plant = GetActionTarget();
  SetRDir((GetWind() ?? 0)/4);
  if (HasEMP(plant)) return(1);
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(plant)),plant);
  DoEnergy(Abs((GetRDir() ?? 0)*4),plant);
  return(1);
}

private func CreateFading()
{
  // MotionBlur-Effekt erzeugen
  CreateObject(CWPF,nil,nil,GetOwner())->Init(this(), GetR());
}

