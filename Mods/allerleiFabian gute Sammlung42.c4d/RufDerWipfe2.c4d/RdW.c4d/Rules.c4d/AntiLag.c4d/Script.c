// Anti-Lag
#strict

public func Initialize()
{
  GameCallEx("AntiLagConfig");
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

global func IsAntiLag()
{
  return ObjectCount(ATLG);
}

// Standartauswahl?

func IsDefaultRule() {return IsNetwork()||GetPlrExtraData(GetPlayerByIndex(),"RdW_Settings_ATLG");}

public func IsChoosen()
{
  return SetPlrExtraData(GetPlayerByIndex(),"RdW_Settings_ATLG",true);
}

public func IsNotChoosen()
{
  return SetPlrExtraData(GetPlayerByIndex(),"RdW_Settings_ATLG",false);
}
