/* Aufwertung */

#strict 3

public func Activate(target1, target2)
  {
  var clonk = target1;
  if (target2) clonk = target2; 

  // Zauberndes Objekt (evtl. zaubernder Clonk draußen) aufwerten
  return(Revaluation(clonk, target1));
  }

public func Revaluation(target, target1)
  {
  // Aufwertungsziel nach Komboobjekt bestimmen
  var idRevaluationTarget = SCLK, id2;
  var i, obj;
  var contained = target->Contained();
  if (contained)
    if (id2 = contained->~GetRevaluationCombo(target))
      { obj = contained; idRevaluationTarget = id2; }
  while (obj = Contents(S42MigrationStep(i, 1, true), target))
    if (id2 = obj->~GetRevaluationCombo(target))
      { idRevaluationTarget = id2; break; }
      
  // Keine Aufwertung zum gleichen Clonktyp
  if (Or(Equal(idRevaluationTarget, GetID(target)), (And(target->~IsSorcerer(), Equal(idRevaluationTarget, SCLK)))))
    return(NoRevaluation(target, target1, Format("$MsgIsRev$", GetName(target), GetName(nil, GetID(target)))));
  // Aufwertung möglich? (z.B. nicht vom König zum Ritter)
  if (idRevaluationTarget->~IsRevaluationBlocked(target))
    return(NoRevaluation(target, target1, Format("$MsgBlocked$", GetName(target), GetName(nil, GetID(target)))));
    
  // Effekt
  Sound("Magic1");
  // Umwandlung
  if (!target->~Redefine2(idRevaluationTarget))
    if (!target->~Redefine(idRevaluationTarget))
      if ((GetOCF(target) ?? 0) & (OCF_Alive ?? 0))
        {
        ChangeDef(idRevaluationTarget, target);
        ObjectSetAction(target, "Walk");
        }
      else return(S42MigrationFirst(nil, RemoveObject()));
  // Komboobjekt verbrauchen
  if (obj) RemoveObject(obj, true);
  // Clonkspezifische Aktionen nach Umwandlung (z.B. Zauberenergie für Magier)
  if (!target->~OnRevaluation())
    // Zur Abwärtskompatibilität mit alten Magiern
    if (Equal(idRevaluationTarget, SCLK)) target->~Magicenergy();
  RemoveObject(); 
  return(1);
  }

public func NoRevaluation(target, target1, szErrorMessage)
  {
  // Stein zu Gold verwandeln?
  var obj;
  if (obj = FindContents(GOLD, target)) {
    var width = 2*(GetDefCoreVal("Width", "DefCore", GetID(target1)) ?? 0);
    var height = 2*(GetDefCoreVal("Height", "DefCore", GetID(target1)) ?? 0);
    var convert_count = RandomX(5, 10);
    while (S42MigrationStep(convert_count, -1, true)) {
      var rock;
      // Stein in der Nähe?
      if (rock = FindObject(ROCK, -(width ?? 0), -(height ?? 0), (width ?? 0)*2, (height ?? 0)*2, OCF_InFree)) {
        // Zu Gold "aufwerten"
        ChangeDef(GOLD, rock);
        // Effekte!
        if (IsNewgfx())
          CastParticles("MSpark", RandomX(2, 4), RandomX(16, 20),
            AbsX(GetX(rock)), AbsY(GetY(rock)),
            5*2, 5*4, RGB(244, 220, 24), RGB(224, 148, 12)); 
        else
          CastObjects(SPRK, RandomX(1, 3), RandomX(8, 10),
            AbsX(GetX(rock)), AbsY(GetY(rock)));
      }
      // Sonst sofort Abbruch
      else break;
    }
    Sound("Magic1");
    RemoveObject(obj);
    return(1);
  }

  // Zauber fehlgeschlagen
  else {
    Sound("Error", nil, nil, 100, (GetOwner() ?? 0)+1);
    PlayerMessage(GetOwner(), szErrorMessage, target);
    RemoveObject(); 
    return(nil);
  }
}
