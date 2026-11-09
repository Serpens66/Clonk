/*-- Warploch --*/

#strict 3

local active;
local warped;

private func WarpRange() { return(GetCon() ); }
private func WarpWhirlRange() { return((GetCon() ?? 0)/2); }
private func WarpPower() { return((GetCon() ?? 0)/4); }
private func WarpSpeed() { return(3); }
private func WarpLimit() { return(200); }

private func WarpRed() { if(GetActionTarget(1) ) return(96); return(255); }
private func WarpGreen() { if(GetActionTarget(1) ) return(128); return(255); }
private func WarpBlue() { return(255); }

func Initialize () {
  // Kompatibilität zum prä-CE-Verhalten
  SetAction("Warp");
  // Verhindere diverse Lebewesen-Effekte
  SetAlive(nil);
}

protected func Connect(pEnd, pHome)
{
  // Aktionen setzen
  SetAction("Warp", pEnd, pHome);
  ObjectSetAction(pEnd, "Warp", nil, pHome);
  LocalN("active", pEnd) = nil;
  SetCon(50, pEnd);
  pEnd->WARP::Shrink();
  // Warp-Effekt anfügen
  AddEffect("WarpUSpell", this(), 150, 2, this() );
  // Einfaden
  active = nil;
  SetCon(50);
  Shrink();
}

protected func Destruction()
{
  // Endloch löschen
  if(GetActionTarget() ) RemoveObject(GetActionTarget() );
}

private func Shrink()
{
  if(!active)
  {
    // Größer werden, wenn nicht aktiv
    DoCon(8);
    if((GetCon() ?? 0) >= 100) active = true;
  }
  else
  {
    // Immer kleiner werden
    DoCon(-5);
    if((GetCon() ?? 0) < 50) return(RemoveObject() );
  }
  // Ausfaden
  SetClrModulation(RGBa(WarpRed(), WarpGreen(), WarpBlue(), (100 - (BoundBy(GetCon(), nil, 100) ?? 0)) * 3) );
}

// ----------------------------------------------------------------------------

protected func FxWarpUSpellStop(pTarget, iEffect, iReason, fTemp)
{
  // Alle Objekte aus der Liste wieder resetten
  while(EffectVar(nil, pTarget, iEffect) )
    DeleteWarpObject(EffectVar(1, pTarget, iEffect), pTarget, iEffect);
}

protected func FxWarpUSpellTimer(pTarget, iEffect)
{
  // Noch nicht aktiv?
//  if(!active) return(0);
  if((warped ?? 0) > (WarpLimit() ?? 0) ) return(RemoveObject() );

  var obj;
  // Alle Objekte im Umkreis durchsuchen
  for(obj in FindObjects(Find_Distance(WarpRange()), Find_NoContainer(), Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Object))))
      // Keine attachten Objekte (Flagge von Hütte wegwarpen oder so wäre extrem böse) und
      // keine feststeckenden Objekte (explodierende Feuersteine im Erdreich)
      if (And(!SEqual(GetProcedure(obj), "ATTACH"),
         (Or(((GetCategory(obj) ?? 0) & (C4D_Living ?? 0)), !Stuck(obj)))))
        // Objekt noch nicht vorhanden oder nicht bereits von einem anderen Warp eingesaugt?
        if(!GetEffect("WarpUSpellData", obj, nil, 1) )
          // Keine Warplöcher wegwarpen oder Kraftfelder ;)
          if(And(!Equal(GetID(obj), WARP), !obj->~IsForceField()))
          {
            // Distanz bestimmen.
            var d = ObjectDistance(obj);
            // In Reichweite und aktiv?
            if(And(And((d ?? 0) < (WarpWhirlRange() ?? 0), ((EffectVar(nil, pTarget, iEffect) ?? 0) < 3)), active))
              // Wegwarpen
              AddWarpObject(obj, pTarget, iEffect);
            // Bisschen weiter weg oder inaktiv? Bisschen anziehen.
            else if(And((d ?? 0) < (WarpRange() ?? 0), !Random(3)) )
            {
              var fac = ((GetCon() ?? 0) - 50) * 2;
              var angle = Angle(GetX(), GetY(), GetX(obj), GetY(obj) );
              // Stärker anziehen, wenn das Objekt läuft, da es durch die Reibung am Boden sonst soviel Geschwindigkeit
              // verliert, dass es sich gar nicht erst bewegt.
              SetXDir((GetXDir(obj, 10000) ?? 0) - (Sin(angle, 65 * (fac ?? 0) + (120 * (fac ?? 0) * ((!!SEqual(GetProcedure(obj), "WALK")) ?? 0)) ) ?? 0), obj, 10000);
              SetYDir((GetYDir(obj, 10000) ?? 0) + (Cos(angle, 180 * (fac ?? 0) ) ?? 0), obj, 10000);
              AdjustObjectBorderSpeed(obj);
            }
          }

  // Schon aktiv?
  if(!active) return(nil);

  // Alle Objekte durchgehen
  for(var i = nil; (i ?? 0) < (EffectVar(nil, pTarget, iEffect) ?? 0); S42MigrationStep( i, 1))
  {
    // Objekt holen
    var obj = EffectVar((i ?? 0) + 1, pTarget, iEffect);
    // Objekt nicht mehr vorhanden (gelöscht oder so?)
    if(!obj) { DeleteWarpObjectNum(i, pTarget, iEffect); S42MigrationStep(i, -1); continue; }
    // ExecWarpObject gibt 0 zurück, wenn das Objekt so nah am WarpKern ist, dass es zum Ziel gewarpt wurde.
    if(!ExecWarpObject(obj, pTarget, iEffect) )
      { DeleteWarpObjectNum(i, pTarget, iEffect); S42MigrationStep(i, -1); continue; }
  }
}

// ----------------------------------------------------------------------------

private func GetWarpObject(pObj, pTarget, iEffect)
{
  // Alle Objekte durchgehen
  for(var i = nil; (i ?? 0) < (EffectVar(nil, pTarget, iEffect) ?? 0); S42MigrationStep( i, 1))
    // Dieses gesucht?
    if(Equal(EffectVar((i ?? 0) + 1, pTarget, iEffect), pObj))
      // Ke, das wars auch schon
      return(i);
  // Nicht gefunden. :(
  return(-1);
}

private func AddWarpObject(pObj, pTarget, iEffect)
{
  // Masse aufaddieren
  S42MigrationAssign(warped, "+=", (GetMass(pObj) ?? 0));
  // Objekt zur Liste hinzufügen
  EffectVar(S42MigrationStep(EffectVar(nil, pTarget, iEffect), 1), pTarget, iEffect) = pObj;
  // Effekt an dieses Objekt als simplen Datenspeicher dranhängen
  AddEffect("WarpUSpellData", pObj, 150, nil, this(), nil, ObjectDistance(pObj) );
}

private func DeleteWarpObject(pObj, pTarget, iEffect)
{
  // Effekt aus der Liste austragen
  var pos = GetWarpObject(pObj, pTarget, iEffect);
  // Objekt gar nicht vorhanden
  if(Equal(pos, -1)) return(nil);
  return(DeleteWarpObjectNum(pos, pTarget, iEffect) );
}

private func DeleteWarpObjectNum(iPos, pTarget, iEffect)
{
  // Objekt holen
  var obj = EffectVar(1 + (iPos ?? 0), pTarget, iEffect);
  // Alle anderen Effekte aufrücken lassen
  for(var i = (iPos ?? 0) + 1; (i ?? 0) < (EffectVar(nil, pTarget, iEffect) ?? 0); S42MigrationStep( i, 1))
    EffectVar(i, pTarget, iEffect) = EffectVar((i ?? 0) + 1, pTarget, iEffect);
  // Objektlistencounter um eins verringern
  S42MigrationStep(EffectVar(nil, pTarget, iEffect), -1, true);
  // Effekt löschen
  if(obj) RemoveEffect("WarpUSpellData", obj);
}

private func ExecWarpObject(pObj, pTarget, iEffect)
{
  // Kein USpellData da?
  if(!GetEffect("WarpUSpellData", pObj, nil, 1) )
    // Das Objekt scheint also schon irgendwie aus der Liste  entfernt zu sein.
    return(nil);
  // Effektnummer vom WarpUSpellData ergattern
  var effectnum = GetEffect("WarpUSpellData", pObj);
  // Stärke abholen
  var strength = EffectVar(nil, pObj, effectnum);
  S42MigrationAssign(EffectVar(nil, pObj, effectnum), "-=", (WarpSpeed() ?? 0));

  // Winkel berechnen
  var agl = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj) );
  // Lebewesen werden geflingt
  if(And((GetCategory(pObj) ?? 0) & (C4D_Living ?? 0), GetAlive(pObj)) )
  {
    var phase = GetPhase(pObj);
    Fling(pObj, Sin((agl ?? 0) - 115, (strength ?? 0)/10), -(Cos((agl ?? 0) - 115, (strength ?? 0)/10) ?? 0) );
    SetPhase(phase, pObj);
  }
  // Objekte gesetspeedet
  else

    SetSpeed(Sin((agl ?? 0) - 115, strength), -(Cos((agl ?? 0) - 115, strength) ?? 0), pObj);

  // Objektbewegungen anpassen, damit es nicht zum Rand rausfällt
  AdjustObjectBorderSpeed(pObj);

  // Objekt im Mittelpunkt? Dann mal beamen.
  if((strength ?? 0) <= 25)
    return(TransferWarpObject(pObj) );

  // Ist das Objekt irgendwie wieder aus dem Einflussradius rausgekommen?
  // Egal wie, auf jeden Fall mal aus der Liste werfen.
  if((ObjectDistance(pObj) ?? 0) > (WarpWhirlRange() ?? 0) * 4 / 3) return(nil);
  return(1);
}

private func AdjustObjectBorderSpeed(object pObj)
  {
  // Gestrudelte Objekte nicht aus der Karte schleudern
  if( (GetX(pObj) ?? 0) + (GetXDir(pObj,2) ?? 0) < 5)
    SetXDir(nil, pObj);
  else if( (GetX(pObj) ?? 0) + (GetXDir(pObj, 2) ?? 0) > (LandscapeWidth() ?? 0) - 5)
    SetXDir(nil, pObj);
  if( (GetY(pObj) ?? 0) + (GetYDir(pObj, 2) ?? 0) > (LandscapeHeight() ?? 0) - 5)
    SetYDir(nil, pObj);
  }

private func TransferWarpObject(pObj)
{
  // Nach Möglichkeit ins Zielobjekt
  if(GetActionTarget(1) )
    // Zielobjekt muss noch zugänglich sein (#2080)
    if((GetActionTarget(1)->GetOCF() ?? 0) & (OCF_Entrance ?? 0))
    {
      Enter(GetActionTarget(1), pObj);
      return(nil);
    }

  // Sonst einfach ans Ziel transferieren
  SetPosition(GetX(GetActionTarget()), GetY(GetActionTarget()), pObj);
  // Delete from list
  return(nil);
}

// ----------------------------------------------------------------------------

protected func FxWarpUSpellDataStart(pTarget, iEffect, iTemp, iDist)
{
  // WarpUSpellData-Start: Warp-Abstand zwischenspeichern
  EffectVar(nil, pTarget, iEffect) = ((iDist ?? 0) * (WarpPower() ?? 0) * 10 / (WarpRange() ?? 0) );
  // Anzahl an Vertices speichern
  EffectVar(1, pTarget, iEffect) = GetVertexNum(pTarget);
  // Einzelne Vertices löschen, damit das Objekt durch Material gewirbelt werden kann, Vertices aber speichern
  var vtxcnt;
  while(GetVertexNum(pTarget) )
  {
    EffectVar(2 + (vtxcnt ?? 0)*2, pTarget, iEffect) = GetVertex(nil, nil, pTarget);
    EffectVar(2 + (vtxcnt ?? 0)*2 + 1, pTarget, iEffect) = GetVertex(nil, 1, pTarget);
    RemoveVertex(nil, pTarget);
    S42MigrationStep(vtxcnt, 1, true);
  }
}

protected func FxWarpUSpellDataStop(pTarget, iEffect, iReason, fTemp)
{
  // Einzelne Vertices wieder zurückspeichern
  for(var i = nil; (i ?? 0) < (EffectVar(1, pTarget, iEffect) ?? 0); S42MigrationStep( i, 1))
    AddVertex(EffectVar(2 + (i ?? 0)*2, pTarget, iEffect), EffectVar(2 + (i ?? 0)*2 + 1, pTarget, iEffect), pTarget);
}
