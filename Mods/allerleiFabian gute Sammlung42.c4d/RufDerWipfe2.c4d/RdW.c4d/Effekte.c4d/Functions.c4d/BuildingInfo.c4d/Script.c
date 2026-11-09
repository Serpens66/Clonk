#strict

public func ContextBuildingInfo(pClonk)
{
  [Gebäude-Info|Image=_ZF9]
  var szString=Format("{{%i}}%s|%s%d/%d HP %d/%d Energie",GetID(),GetName(),GetExtraInfo(),GetMaxDamage()-GetDamage(),GetMaxDamage(),GetEnergy(),GetPhysical("Energy")/1000);
  MessageWindow(szString, GetOwner(pClonk));
}

public func GetExtraInfo()
{
  return "";
}

public func GetMaxDamage()
{
  return GetDefCoreVal("BlastIncinerate","DefCore",GetID());
}