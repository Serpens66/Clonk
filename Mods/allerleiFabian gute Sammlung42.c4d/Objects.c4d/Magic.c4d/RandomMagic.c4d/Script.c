#strict 3

local pAimingClonk, pOriginalCaster;

public func Activate(object pCaster, object pRealcaster)
{
  var idDef,idMagic,pMagic;
  Sound("Magic1");
  // Zauber wählen
  idMagic = GetRandomSpellID();
  if(Equal(idMagic, GetID())) return(S42MigrationFirst(Message("$TxtNothing$!",pCaster), RemoveObject()));
  pMagic=CreateObject(idMagic,nil,nil,GetOwner());
  Message(GetName(pMagic),pCaster);
  // Zauberobjekt ist this() für eventuelle Zielaufrufe
  pOriginalCaster = pCaster;
  if (!pRealcaster) pRealcaster = pCaster;
  // Objekt verzögert entfernen, falls noch gezielt wird
  SetAction("WannaRemove", pRealcaster);
  // Zauberausrichtung
  SetDir(GetDir(pRealcaster));
  // Zaubern
  if (pMagic->Activate(this(),pRealcaster)) return(true);
  // Zauberfehlschlag
  return(S42MigrationFirst(Message("$TxtNothing$!",pCaster), RemoveObject()));
}

private func GetRandomSpellID()
  {
  var idDef, idMagic, i;
  while(idDef=GetDefinition(S42MigrationStep(i, 1, true),C4D_Magic))
    if(!Random(i)) idMagic=idDef;
  return (idMagic);
  }

protected func WannaRemove() {
  // manche Zauber hängen gerne Effekte an den Caster, oder lassen ihn zielen
  if(And(!GetEffectCount(nil,this()), !pAimingClonk)) RemoveObject();
}

protected func AttachTargetLost() {
  // Clonk ging im WannaRemove verloren: Weg mit dem Zufallszauber
  RemoveObject();
}

public func SpellAiming(object pSpell, object pAimingClnk)
  {
  // Clonk zielt: Zauber vorerst nicht entfernen
  pAimingClonk = pAimingClnk;
  // Originalcaste rkönnte diesen Aufruf auch gebrauchen
  if (pOriginalCaster) pOriginalCaster->~SpellAiming(pSpell, pAimingClnk);
  }
  
public func SpellSucceeded(id idSpell, object pAimingClnk)
  {
  // Clonk hat fertig gezaubert: Magie für Zufallszauber trainieren und weg
  if (pOriginalCaster) pOriginalCaster->~SpellSucceeded(GetID());
  pAimingClonk = nil;
  return(true);
  }
  
public func SpellFailed(id idSpell, object pAimingClnk)
  {
  // Zauber beim Zielen abgebrochen: Magie für Zufallszauber restaurieren
  if (pOriginalCaster) pOriginalCaster->~SpellFailed(GetID());
  pAimingClonk = nil;
  return(true);
  }

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("644"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
