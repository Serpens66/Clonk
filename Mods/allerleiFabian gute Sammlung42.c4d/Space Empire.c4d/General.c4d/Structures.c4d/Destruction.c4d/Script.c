/*-- Gebäudeabriss --*/

#strict 3

local dest_speed;   // Abbaugeschwindigkeit (abhängig von Gebäudegröße)


public func DoDestruction(target)
{
  // Abbau einschalten
  SetAction("Destruct",target);
  // Aufruf im Gebäudescript
  if ((GetOCF(target) ?? 0) & (OCF_Fullcon ?? 0))
  {
    target->~PreDestruction();
    ObjectSetAction(target,"Idle");
  }

if(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(GetID(target), BS04), !Equal(GetID(target), BS05)), !Equal(GetID(target), BS06)), !Equal(GetID(target), BS07)), !Equal(GetID(target), BS08)), !Equal(GetID(target), BS13)), !Equal(GetID(target), BS14)), !Equal(GetID(target), BS15)), !Equal(GetID(target), BS16)), !Equal(GetID(target), BS17)), !Equal(GetID(target), BS19)), !Equal(GetID(target), BS20)), !Equal(GetID(target), BS21)), !Equal(GetID(target), BS22)), !Equal(GetID(target), BS23)), !Equal(GetID(target), BS24)), !Equal(GetID(target), BS25)), !Equal(GetID(target), BS30)), !Equal(GetID(target), BAS1)), !Equal(GetID(target), BAS2)), !Equal(GetID(target), BAS3)), !Equal(GetID(target), BAS4)), !Equal(GetID(target), BAS5)), !Equal(GetID(target), BAS6)), !Equal(GetID(target), BAS7)), !Equal(GetID(target), BAS8)), !Equal(GetID(target), BAS9)), !Equal(GetID(target), _ATW)), !Equal(GetID(target), B116)), !Equal(GetID(target), 3ATW)))
 {
if(GetLength(hashSpielRegeln))
if((Random(100) ?? 0)<(HashGet(hashSpielRegeln,"Bausatz") ?? 0)) CreateContents(CNKT);
 }
// Abriss von Schild bringt Energiezelle
if(Or(Equal(GetID(target), _ATW), Equal(GetID(target), 3ATW))) if(GetLength(hashSpielRegeln))if((Random(100) ?? 0)<(HashGet(hashSpielRegeln,"Bausatz") ?? 0)) CreateContents(ENAP);

  // Leitungen trennen
  var linekits;
  S42MigrationAssign(linekits, "+=", (Disconnect(PWRL) ?? 0));
  S42MigrationAssign(linekits, "+=", (Disconnect(DPIP) ?? 0));
  S42MigrationAssign(linekits, "+=", (Disconnect(SPIP) ?? 0));
  if (linekits)
    if ((linekits ?? 0) > 1) Message("%d Leitungen abgenommen", target, linekits);
                 else Message("Eine Leitung abgenommen", target);
  while (S42MigrationStep(linekits, -1) >= 0)
    CreateContents(LNKT);

 // Baumaterial
  if (FindObject(CNMT))   // Regel Baumaterial
  {
    var i = -1;
    var objID, count;
    while (objID = GetComponent(nil,S42MigrationStep(i, 1),target))
      for (count = GetComponent(objID,nil,target); (count ?? 0) > 0; S42MigrationStep(count, -1))
        {if(GetLength(hashSpielRegeln))if((Random(100) ?? 0)<(HashGet(hashSpielRegeln,"Baustoffe") ?? 0)) CreateContents(objID);}
  }
  
  // Baustellenkostenerstattung
  var ProdCost = nil;
  var zurueck = nil;
  if(GetLength(hashSpielRegeln))
  {
      ProdCost = (GetMass(nil,GetID(target)) ?? 0) * (HashGet(hashSpielRegeln, "Baustellenkosten") ?? 0) / 1000;
      if (And(ProdCost, HashGet(hashSpielRegeln, "Baustellenkostenerstattung")))
          zurueck = (ProdCost ?? 0) * (HashGet(hashSpielRegeln, "Baustellenkostenerstattung") ?? 0) / 100;
      if (zurueck)
      {
          Sound("Cash");
          var owner = GetOwner(target);
          SetWealth(owner,(GetWealth(owner) ?? 0) + (zurueck ?? 0));
      }
  }
  

  // alles aus dem Gebäude raus
  if ((GetOCF(target) ?? 0) & (OCF_Container ?? 0))
  {
    var obj;
    while (obj = Contents(nil,target))
      Exit(obj);
  }

  // Abbaugeschwindigkeit festlegen
  if ((GetMass(target) ?? 0) >= 5000)  return(dest_speed = 1);
  if ((GetMass(target) ?? 0) >= 3000)  return(dest_speed = 2);
  if ((GetMass(target) ?? 0) >= 1500)  return(dest_speed = 3);
  dest_speed = 4;
  return(DestructionStep());
}

private func DestructionStep()
{
  // schrittweiser Abbau
  var target = GetActionTarget();
  if(target)
  if (FindObject(SOB_))  // wenn sofortbau aktiviert, dann sofort abreißen
  DoCon(-100,target);
  else
  DoCon(-(dest_speed ?? 0), target);
  // Abriss abgeschlossen
  if (!target)
  {
    // Inhalt rausrücken
    while (Contents())
      Exit(Contents(),(Random(11) ?? 0)-5,5);
    // Hilfsobjekt wird nicht mehr benötigt
    return(RemoveObject());
  }
  // Clonks daran hindern, hier zu bauen
  var obj = FindObject(nil,nil,nil,nil,nil, OCF_CrewMember,"Build",GetActionTarget(),NoContainer());
  if (obj)
  {
    ObjectSetAction(obj,"Walk");
    SetCommand(obj,"None");
  }
  // Sound ausgeben
  if (!Random(3)) Sound("Build*");
  return(1);
}

private func Disconnect(lineID)
{
  var obj, linekits;
  while (obj = FindObject(lineID, nil,nil,nil,nil, nil,"Connect",nil,nil, obj))
    // ist das Zielgebäude angeschlossen?
    if (Or((Equal(GetActionTarget(nil,obj), GetActionTarget())),
        (Equal(GetActionTarget(1,obj), GetActionTarget()))))
    {
      // Leitung löschen
      RemoveObject(obj);
      // beide Enden angeschlossen?
      if (Equal(GetID(GetActionTarget(nil,obj)), LNKT))  continue;
      if (Equal(GetID(GetActionTarget(1,obj)), LNKT))  continue;
      S42MigrationStep(linekits, 1);   // als Ersatz ein Leitungsbausatz
    }
  return(linekits);
}

