/*-- Lehmklumpen --*/

#strict 3

public func Initialize()
{
  // Erst alle Komponenten löschen
  var i, id;
  while(id = GetComponent(nil, S42MigrationStep(i, 1, true), nil, GetID()) )
    SetComponent(id, nil, this() );
  SetComponent(ILOA, 3, this() );
}

public func BridgeMaterial() 
{
  return(Material("Earth"));
}

public func BridgeTime() { return 100; } // Brückenbau für 100 Frames

public func IsMovingClonkBridge() { return true; } // Wenn true, bewegt sich der Clonk mit dem Brückenbau

protected func Hit() 
{
  Sound("RockHit*");
  return(1);
}

public func Activate(caller) 
{
  [$CtrlActivateDesc$|Image=LMMS]
  // Clonk soll anhalten
  SetComDir(COMD_Stop, caller);
  // Nur, wenn der Clonk hier bauen kann
  if (!CheckCanBridgeHere(caller)) return;
  // Menü öffnen
  CreateMenu(LMMS, caller, this());
  AddMenuItem("$MnuWallLeft$", "MenuWall", LMM1, caller, nil, COMD_Left, nil, 2, 5);
  AddMenuItem("$MnuWallLeftDiag$", "MenuWall", LMM1, caller, nil, COMD_UpLeft, nil, 2, 6);
  AddMenuItem("$MnuWallStraight$", "MenuWall", LMM1, caller, nil, COMD_Up, nil, 2, 7);
  AddMenuItem("$MnuWallRightDiag$", "MenuWall", LMM1, caller, nil, COMD_UpRight, nil, 2, 8);
  AddMenuItem("$MnuWallRight$", "MenuWall", LMM1, caller, nil, COMD_Right, nil, 2, 9);
  AddMenuItem("$MnuArrowLeft$", "MenuBridge", LMM1, caller, nil, COMD_Left, nil, 2, nil);
  AddMenuItem("$MnuArrowLeftDiag$", "MenuBridge", LMM1, caller, nil, COMD_UpLeft, nil, 2, 1);
  AddMenuItem("$MnuArrowStraight$", "MenuBridge", LMM1, caller, nil, COMD_Up, nil, 2, 2);
  AddMenuItem("$MnuArrowRightDiag$", "MenuBridge", LMM1, caller, nil, COMD_UpRight, nil, 2, 3);
  AddMenuItem("$MnuArrowRight$", "MenuBridge", LMM1, caller, nil, COMD_Right, nil, 2, 4);
  // Vorauswahl Senkrecht
  SelectMenuItem(7, caller);
  return(1);
}

protected func MenuWall(id icon, int comdir, bool enterall)
{
  comdir ??= 0;
  enterall ??= false;
  return(StartBridge(Contained(), comdir, true, enterall));  
}

protected func MenuBridge(id icon, int comdir, bool enterall)
{  
  comdir ??= 0;
  enterall ??= false;
  return(StartBridge(Contained(), comdir, nil, enterall));
}

private func CanBridgeHere(object clonk)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (!SEqual(GetAction(clonk), "Walk")) return nil;
  if (Contained(clonk)) return nil;
  // Brücke OK
  return true;
}

private func CheckCanBridgeHere(object clonk)
{
  // Brücke OK?
  if (CanBridgeHere(clonk)) return true;
  // Brücke nicht OK: Nachricht
  PlayerMessage(GetController(clonk), "$TxtCantBuild$", clonk, GetName(clonk));
  Sound("Error", nil, clonk, 100, (GetController(clonk) ?? 0)+1);
  return nil;
}

public func StartBridge(object clonk, int comdir, bool wall, bool alternate)
{
  // Nur, wenn der Clonk hier bauen kann
  comdir ??= 0;
  wall ??= false;
  alternate ??= false;
  if (!CheckCanBridgeHere(clonk)) return;
  // Evtl. weitere Aktionen durchführen
  this()->~OnBridge();
  // Aktion setzen
  ObjectSetAction(clonk, "Bridge");
  // Richtung setzen
  clonk->SetComDir(comdir);
  var moveclonk = IsMovingClonkBridge();
  if (And(wall, Equal(comdir, COMD_Up))) moveclonk = nil; // Decke über Clonk: Bewegung ist hier etwas blöd, weil sie die Decke ungleichmäßig macht
  if (alternate) moveclonk = !moveclonk;
  // Brückendaten setzen
  SetBridgeActionData(BridgeTime(), moveclonk, wall, BridgeMaterial(), clonk);
  // Material verbrauchen
  RemoveObject(); 
  // Fertig
  return(1);
}       


func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Forschung */

public func GetResearchBase() { return(MLLT); }
