/*-- Schnelles Spiel --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}


// Standartauswahl?

public func IsChoosen()
{
  return SetPlrExtraData(GetPlayerByIndex(),"RdW_Settings_SPDG",true);
}

public func IsNotChoosen()
{
  return SetPlrExtraData(GetPlayerByIndex(),"RdW_Settings_SPDG",false);
}

/* Beschleunigen */  
  
private func Accelerating()
{
  // Bauende Clonks suchen
  var pClonk;
  for(pClonk in FindObjects(Find_Action("Build")))
    if (pClonk->~IsClonk())
    {
      // Gebäude aufstocken
      pClonk->GetActionTarget()->SetCon(Min(pClonk->GetActionTarget()->GetCon() + 1, 100));
      //Log("Accelerating: %s + %d", pClonk->GetActionTarget()->GetName(), Acceleration);
    }
}
