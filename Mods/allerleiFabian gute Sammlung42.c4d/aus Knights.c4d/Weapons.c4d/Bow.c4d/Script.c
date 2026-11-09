/*-- Bogen --*/

#strict 3

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$|Condition=ShowInContextMenu]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(nil);
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire())
  {
    if(!SEqual(pClonk->GetProcedure(), "SCALE")) return(nil);
    // Beim Klettern nur nach Doppelstop ablegen
    if(!GetPlrDownDouble(pClonk->GetOwner()))
      return(1);
    else
      return(nil);
  }
  // Träger: neue Zielaktion setzen
  if (!pClonk->~LoadBow(1))
  {
    if (!pClonk->~AimBow()) 
      return;
    SetPhase(7, pClonk);
  }
  // Erfolg
  return(1);
}

/* Spezialsteuerung durch Träger */
public func ControlThrow(pClonk)
{
  // Träger hat gezielt
  if (IsAiming())
    { Fire(pClonk); return(1); }
  // Bogen einsetzen
  return(Activate(pClonk));
}

protected func ControlConf(int conf)
{
  conf ??= 0;
  var rVal;
  var pClonk = Contained();
  if(!pClonk) return(nil);

  if((conf ?? 0) < 0)
  {
    if(pClonk->~BowControlUp(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(pClonk->AimStdConf(conf));
    else
      return(nil);
  }
  else
  {
    if(pClonk->~BowControlDown(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(pClonk->AimStdConf(conf));
    else
      return(nil);
  }
}

public func ControlUp(pClonk)
{
  return(AimUp(pClonk, 5, "ControlConf"));
}

public func ControlDown(pClonk)
{
  return(AimDown(pClonk, 5, "ControlConf"));
}

public func ControlUpdate(pClonk, int comdir)
{
  comdir ??= 0;
  AimUpdate(pClonk, comdir, 5, "ControlConf");
}

public func ControlLeft(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Left, pClonk);
    return(1);
  }
  // Keine Spezialsteuerung
  return(nil);
}

public func ControlRight(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Right, pClonk);
    return(1);
  }
  // Keine Spezialsteuerung
  return(nil);
}

public func ControlRightDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming())
  {
    pClonk->StopAiming();
    SetComDir(COMD_Right, pClonk);
  }
  // Keine Spezialsteuerung
  return(nil);
}

public func ControlLeftDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming())
  {
    pClonk->StopAiming();
    SetComDir(COMD_Left, pClonk);
  }
  // Keine Spezialsteuerung
  return(nil);
}

/* Wird gerade vom Träger gezielt? */
public func IsAiming() 
{ 
  if(!Contained()) return(nil);
    return (Contained()->~IsAiming()); 
}

/* Hardgecodet, für alte Ritter */
private func GetBowOutOld(pClonk, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir)
{
  var iOutDir; if (Equal(GetDir(pClonk), DIR_Left)) iOutDir=-1; else iOutDir=1;
  var iOutPhase = GetPhase(pClonk);
  // X-Austritt  
  iOutX = (iOutDir ?? 0)*(iOutPhase ?? 0)*2;
  // Y-Austritt
  iOutY = (iOutPhase ?? 0)+3; // + Random(9)-4;
  // R-Austritt
  iOutR = (iOutX ?? 0) * 13;
  // XDir
  iOutXDir = (iOutDir ?? 0)*(iOutPhase ?? 0)*3+2;
  // YDir
  if (Equal(iOutPhase, 5)) iOutYDir=10; else iOutYDir = ((iOutPhase ?? 0)-3)*3;
  // Größerer Abstand
  S42MigrationAssign(iOutX, "+=", (iOutXDir ?? 0)*2);
  S42MigrationAssign(iOutY, "+=", (iOutYDir ?? 0)*2);
  // RDir
  iOutRDir = (iOutDir ?? 0)*3;
}
public func GetBowOutAngle(pClonk, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  // Zufallsabweichung
  if(fDeviate) { S42MigrationAssign(iAngle, "+=", (Random(11) ?? 0) - 5); }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 70); 
  iOutYDir = -(Cos(iAngle, 70) ?? 0);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25); 
  iOutY = -(Cos(iAngle, 25) ?? 0);
  // Pfeildrehung 
  iOutR = iAngle; 
  if((iAngle ?? 0) > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return(1);
} 

/* Pfeil abfeuern */
public func Fire()
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return(nil);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(nil); }
  // Unvollständig?
  if (~(GetOCF() ?? 0) & (OCF_Fullcon ?? 0)) return(nil);
  // Noch beim Nachladen
  if (!!SEqual(GetAction(), "Reload")) return(nil);
  // Pfeil vom Träger holen
  var pArrow = pClonk->~GetArrow();
  if (!pArrow) return(nil);
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, nil, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetBowAimAngle();
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, nil,nil,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  // Sicht verfolgen
  if (!Equal(GetPlrView(GetController(pClonk)), 2))
    SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Bow");
  // Nachladen
  SetAction("Reload");
  pClonk->~LoadBow();
  // Fertig!
  return(1);
}

/* Pfeil abfeuern */
public func FireAngle(iAngle)
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return(nil);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(nil); }
  // Unvollständig?
  if (~(GetOCF() ?? 0) & (OCF_Fullcon ?? 0)) return(nil);
  // Noch beim Nachladen
  if (!!SEqual(GetAction(), "Reload")) return(nil);
  // Pfeil vom Träger holen
  var pArrow = Contained()->~GetArrow();
  if (!pArrow) return;
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(Contained(), nil, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutAngle(Contained(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, nil,nil,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
    // Sound
  Sound("Bow");  
  // Nachladen
  SetAction("Reload");
  Contained()->~LoadBow();
  return(1);
}


/* Anzünden: Verbrannte Aktion zur Markierung, dass er unbenutzbar ist */

protected func Incineration()
 {
  //Einfärben für Menüs
  SetClrModulation(RGB(50,50,50),this());
  SetAction("Burned");
  return(1);
 }

/* "Bogen einsetzen" wird nicht im Kontextmenue angezeigt */
private func ShowInContextMenu()
{
 return(nil);
}

// Ist ein Bogen
public func IsBow() { return(1); }

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }
