/*-- Propellerflügel --*/

#strict


protected func Destruction()
{
  // MotionBlur-Effekt löschen
  var obj;
  while (obj = FindObject(CWPF, 0,0,0,0, 0,"Fade",this()))
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
  SetRDir(GetWind()/4);
  if (HasEMP(plant)) return(1);
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(plant)),plant);
  DoEnergy(Abs(GetRDir()*4),plant);  
  return(1);
}

private func CreateFading()
{
  // MotionBlur-Effekt erzeugen
  CreateObject(CWPF,0,0,GetOwner())->Init(this(), GetR());
}

