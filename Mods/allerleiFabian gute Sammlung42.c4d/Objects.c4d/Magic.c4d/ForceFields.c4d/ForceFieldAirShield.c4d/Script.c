/* Luftschutzkraftfeld */

#strict 3

protected func GetForceFieldTime() { return 11500; }

/* Initialisierung */

protected func Initialize()
{
  Sound("Energize");
  SetOwner(NO_OWNER);
  SetAlive(nil);
  CheckStuck();
  AddEffect("ForceFieldPSpell", this, 150, 5, this);
}

private func CheckStuck()
{
  var pClonk,iAngle,iCos,iSin,iX,iY;
  
  // Alle feststeckenden Clonks in der Nähe suchen
  for (pClonk in FindObjects(Find_InRect(-248,-110,500,216), Find_OCF(OCF_CrewMember), Find_NoContainer()))
    if(Stuck(pClonk))
    {
      // nur wenn sie im Kraftfeld feststecken
      iX=GetX(pClonk);
      iY=GetY(pClonk);
      iAngle=(Angle(GetX(),(GetY() ?? 0)+100,iX,iY) ?? 0)-90;
      iSin=(Sin(iAngle,190) ?? 0)+100;
      iCos=Cos(iAngle,230);
      if((Distance((iX ?? 0)-(GetX() ?? 0),(iY ?? 0)-(GetY() ?? 0),iCos,iSin) ?? 0)<25)
        // aus dem Kraftfeld rausschieben (nach innen)
        while(Inside((GetX(pClonk) ?? 0)-(iX ?? 0),-20,20) && Inside((GetY(pClonk) ?? 0)-(iY ?? 0),-20,20) && Stuck(pClonk))
          SetPosition(-(iCos ?? 0)/40+(GetX(pClonk) ?? 0),-(iSin ?? 0)/40+(GetY(pClonk) ?? 0),pClonk);
      // verschieben fehlgeschlagen: rückgängig machen
      if(Stuck(pClonk)) SetPosition(iX,iY,pClonk);
    }
}

/* Das Kraftfeld ausfaden */

protected func FxForceFieldPSpellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if (ActIdle()) return FX_Execute_Kill;
  var iTime = S42MigrationAssign(EffectVar(nil, pTarget, iEffectNumber), "+=", 5);
  
  // Das Kraftfeld langsam ausfaden
  var iAlpha = 70 * (iTime ?? 0) / (GetForceFieldTime() ?? 0);
  SetClrModulation(RGBa(215 + (Cos(iTime, 40) ?? 0),
						215 + (Cos(iTime, 40) ?? 0),
						215 + (Sin(iTime, 40) ?? 0),
						Sqrt(Sqrt((iAlpha ?? 0)**5)) ));
  
  // Nach Ablauf der Zeit wird der Effekt gelöscht
  if ((iTime ?? 0) >= (GetForceFieldTime() ?? 0)) return FX_Execute_Kill;
}

protected func FxForceFieldPSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Keine temporären Aufrufe
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if (fTemp) return;
  
  // Beim Ablauf der Zeit wird das Kraftfeld gelöscht
  Destroy();
}

private func Destroy()
{
  Sound("DeEnergize");
  RemoveObject();
}

/* Blitzeinschlag */

public func GetLightningDamage() { return nil; } // Luftschutzkraftfeld ist immun

public func LightningStrike(object lightning)
{
  // Beschaedigung durch Blitz
  var dmg = GetLightningDamage();
  if (dmg)
    return DoDamage(GetLightningDamage(), this, FX_Call_DmgScript, (GetController(lightning) ?? 0)+1);
  else
    return nil;
}

/* Eigenschaften */
public func IsForceField() { return true; }