/*-- Zauberturm --*/

#strict 3

#include DOOR
#include BAS2



// Initialisierung
protected func Initialize() 
{
  SetAction("Ready");
  // Nach rechts ausgerichtet, wenn in der linken Landschaftshälfte
  if ((GetX() ?? 0) < (LandscapeWidth() ?? 0)/2)
    SetDir(DIR_Right);
  return(1);
}

// Beim Einsammeln von Objekten
protected func Collection2() 
{
  // Inhalt prüfen
  ContentsCheck();
  return(1);
}

// Inhalt prüfen: Kristalle verarbeiten
protected func ContentsCheck() 
{
  if (FindContents(CRYS)) 
    ConvertCrystal();
  return(1);
}

/* Energiegewinnung */

private func ConvertCrystal() 
{
  if (!IsConvertReady()) return(nil);
  var crystal = FindContents(CRYS);
  if (!crystal) return(nil);
  
  if (!crystal->~Apply(this()) )
    if(DoMagicEnergy(+10) )
      RemoveObject(crystal);
    else
      return(nil);

  SetAction("Energize");
  Sound("Electric");
  return(1);
}

public func LightningStrike() 
{
  // Nur wenn fertig gebaut
  if ((GetCon() ?? 0) < 100) return(nil);
  // Energiegewinnung
  for(var i = nil; (i ?? 0) < 10; S42MigrationStep( i, 1))
    DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion
  if (!SEqual(GetAction(), "Ready")) return(nil);
  SetAction("Energize");
  return(1);
}

protected func RefillMagic() {  // alle 100 Frames
  // Nur wenn fertig gebaut
  if ((GetCon() ?? 0) < 100) return(nil);
  DoMagicEnergy(+1);
}

/* Zaubern */

public func DoMagicCall(object caster, id spell_id, int direction)
{
  direction ??= 0;
  DoMagic(spell_id, caster, direction);
}
 	 
protected func DoMagic(id spell_id, object caster, int direction)
{
  direction ??= 0;
  if (!caster) return;

  // Zaubernden Clonk in den Turm bewegen, wenn er nicht drin ist
  if (!Equal(caster->Contained(), this()))
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, nil,nil, "DoMagicCall");
    AddCommand(caster, "Enter", this());
    return(1);
  }

  // Nicht bereit: etwas warten, dann erneut versuchen
  if (!SEqual(GetAction(), "Ready"))
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, nil,nil, "DoMagicCall");
    AddCommand(caster, "Wait", nil,nil,nil,nil,nil, 10);
    return(1);
  }

  // Richtung setzen
  if ((direction ?? 0) > 0)
    AimRight(caster);
  else
    AimLeft(caster);
  
  // Nicht genug Zauberenergie
  if (!CheckMana(spell_id))
  {
    Sound("Error");
    Message("$TxtNotEnoughEnergy$", this());
    return;
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(spell_id, this(),caster))
  {
    Sound("Error");
    Message("$TxtNotEnoughIngredients$",this(),GetNeededAlcStr(spell_id,this(),caster));
    return;
  }
  
  // Zauberobjekt erschaffen und aktivieren
  if (CreateObject(spell_id, nil, nil, GetController(caster))->~Activate(this(), caster))
  {
    // Energie abziehen
    DoMagicEnergy(-(Value(spell_id) ?? 0));

    // alchemische Zutaten abziehen
    if(ObjectCount(ALCO)) ReduceAlchem(spell_id,this(),caster);

    // Zauberkrafttraining sowie szenarienspezifische Aufrufe
    OnClonkSucceededSpell(spell_id, caster);
      
    // Aktion setzen
    SetAction("Magic");
  }
  // Zauberobjekterschaffung fehlgeschlagen
  else 
  {
    SetAction("Ready");
    Sound("Error");
    return(nil);
  }
  
  // Fertig
  return(1);
}

public func SpellFailed(id idSpell, object pByClonk)
{
  // Zauber abgebrochen:
  // Magieenergie zurückgeben
  DoMagicEnergy(Value(idSpell));
  // Alchemische Zutaten könnten aus dem Clonk oder dem Turm stammen: Dem Turm geben, wenn möglich
  if(ObjectCount(ALCO))
    if (GetAlchemBag()) IncreaseAlchem(idSpell); else IncreaseAlchem(idSpell, pByClonk);
  // k
  return(true);
}

/* Zaubern */

public func SelectSpell(object caller, int direction)
{
  // Forschung abbrechen
  direction ??= 0;
  CancelResearch();
  // Menü erzeugen und mit den Zaubersprüchen des Turmbesitzers füllen
  // Gehört der Turm keinem gülötigen Besitzer, stattdessen Zauber des Clonks nehmen
  OpenEmptySpellMenu(MCMS, caller, this, this, "$TxtNoSpells$");
  var spell_num;
  var spell_id;
  var iPlr = GetOwner(); if (!GetPlayerName(iPlr)) iPlr = GetOwner(caller);
  while (spell_id = GetPlrMagic(iPlr, nil, S42MigrationStep(spell_num, 1, true)))
    if(!spell_id->~NoTowerMagic() )
      {
      //AddMenuItem("$TxtDoMagic$", "DoMagic", spell_id, caller, 0, caller);
      var strCommand = Format("Object(%d)->DoMagic(%i,Object(%d),%d)", ObjectNumber(this()), spell_id, ObjectNumber(caller), direction);
      AddMenuItem("$TxtDoMagic$", strCommand, spell_id, caller, nil, caller);
      }
  // Zauber erforschbar
  if (And(ObjectCount(WTRS), (Equal(caller->Contained(), this()))))
  {
    // Neue Forschung starten
    AddMenuItem("$TxtResearchs$", "ResearchMenu", RSR6, caller, nil, caller);
    // Laufende Forschung fortsetzen
    if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, caller, nil, caller);
  }
  return(1);
}

public func AimLeft() 
{
  if (Equal(GetDir(), DIR_Left)) return(nil);
  SetDir(DIR_Left);
  Sound("Lever1");
  return(1);
}

public func AimRight() 
{
  if (Equal(GetDir(), DIR_Right)) return(nil);
  SetDir(DIR_Right);
  Sound("Lever1");
  return(1);
}

/* Türsteuerung */

protected func ActivateEntrance() 
{
  if (!SEqual(GetAction(), "Ready")) return(1);
  SetAction("OpenDoor");  
  return(1);
}

private func MagicCloseEntrance() 
{
  SetEntrance(nil);
  return(1);
}

/* Steuerung */

public func ContainedLeft(object pCaller) 
{
  [$TxtAimspellsleft$]
  CancelResearch();
  AimLeft(pCaller);
  SelectSpell(pCaller, nil);
  return(1);
}

public func ContainedRight(object pCaller) 
{
  [$TxtAimspellsright$]
  CancelResearch();
  AimRight(pCaller);
  SelectSpell(pCaller, 1);
  return(1);
}
  
public func ContainedDown(object pClonk) 
{
  [$TxtCancelresearch$|Image=RSR2]
  CancelResearch();
  return(nil);
}

/* Status */

public func IsConvertReady() { return(!!SEqual(GetAction(), "Ready")); }

/* Forschen */

local target;
local progress;

local restime; // Benötigte Gesamtzeit zum Forschen

protected func ResearchMenu(idImage, pClonk) 
{
  // Auswahlmenü erzeugen
  CreateMenu(idImage, pClonk, this(), nil, "$TxtNothingLeft$");
  // Alle geladenen Sprüche überprüfen
  var i,idDef,iPlr=GetOwner(pClonk);
  while (idDef = GetDefinition(S42MigrationStep(i, 1, true), C4D_Magic))
    // Dem Spieler noch nicht bekannt
    if (!(GetPlrMagic(iPlr,idDef))) {
      // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden
      var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
      fHasBase = Or(!idBase, GetPlrMagic(iPlr,idBase)); j=1;
      // Erweiterte Forschungsgrundlagen
      while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", S42MigrationStep(j, 1))))
        if (!GetPlrMagic(iPlr,idBase)) fHasBase=nil;
      // Ist schon durch etwas anderes erforschbar? (Zugbrücke rechts -> Zugbrücke links etc.)
      var link;
      if (link = DefinitionCall(idDef, "GetResearchLink"))
        if (GetName(nil, link)) // Linkdefinition auch tatsächlich geladen?
          if(!GetPlrKnowledge(iPlr, link) ) // Linkdefinition noch nicht erforscht? (passiert durch SetPlrKnowledge-Aufrufe oder Szenariovorgaben)
            continue;
      // Menüeintrag hinzufügen
      if (fHasBase) AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, nil, pClonk);
    }
  return(1);  
}

public func SelectResearch(def, clonk)
{
  // Anfangen
  AddCommand(clonk, "Call", this(), def, nil, nil, nil, "StartResearch", 10, 1);
  // Vorher ins Labor gehen
  if (!Equal(Contained(clonk), this()))
  {
    AddCommand(clonk, "Wait", nil, nil, nil, nil, 40);
    AddCommand(clonk, "Enter", this());
  } 
}
  
private func CancelResearch()
{
  if (WildcardMatch(GetAction(), "Research*")) 
    SetAction("Ready");
}

protected func StartResearch(clonk, def)
{
  // Spieler hat diesen Bauplan schon
  if (GetPlrMagic(clonk->GetOwner(), def))
  {
    Message("$TxtAlreadyDeveloped$", this(), GetName(nil, def));
    Sound("Error");
    target = nil;
    return nil;
  }
  // Bisherige Forschung abbrechen
  CancelResearch();
  // nur, wenn ansonsten inaktiv oder schon am Forschen
  if (!SEqual(GetAction(), "Ready")) return(nil);
  // Besitzer anpassen (der Besitzer des forschenden Clonks erhält den Bauplan)
  SetOwner(GetOwner(clonk));
  // Neues Forschungsziel: Fortschritt zurücksetzen
  if (!Equal(def, target)) progress = nil;
  // Forschungsziel und Gesamtzeit setzen
  restime = DefinitionCall(target=def, "GetResearchTime", this(), clonk);
  // Forschungsaktion starten
  SetAction("Research");
  return (1);
}

private func Researching()
{
  // Kein Ziel
  if (!target) return(nil);
  // Effekte TODO: Sparks
  //if (Random(3)) Smoke(5+8*Random(2),-23,3+Random(8));
  // Fortschritt
  var Process = Min(S42MigrationStep(progress, 1)*4 / (restime ?? 0), 3);
  Process = Format("Research%d", Process);
  if (!SEqual(Process, GetAction())) SetAction(Process);
  // Forschung abbrechen, wenn irgendein Clonk im Inneren das Gebäude verlassen möchte
  var pObj;
  while (pObj = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, nil,nil, this(), pObj))
    if (!!SEqual(pObj->GetCommand(), "Exit"))
      return(CancelResearch());    
  // Weitermachen
  if (!restime) restime=100;
  if ((progress ?? 0) < (restime ?? 0)) return(1);
  // Forschung abgeschlossen
  Sound("ResearchDone");
  // Dem Spieler den neuen Bauplan verpassen
  SetPlrMagic(GetOwner(), target);  
  // Alle Clonks suchen, die ein Menü für dieses Labor geöffnet haben - noch nicht implementierbar, da kein GetMenuTarget()...
  /*var pClonk;
  while (pClonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, 0, pClonk))
    if ((pClonk->GetMenu() == RSR1) && (pClonk->GetMenuTarget() == this())
      // Menü neu öffnen, damit Forschung-Fortsetzen-Einträge aktualisiert werden
      ResearchMenu(pClonk);*/  
  // Gelinkte Forschungsziele sind automatisch auch erforscht
  // (Linke Zugbrücke -> Rechte Zugbrücke etc.)
  var def_num, def;
  while (def = GetDefinition(S42MigrationStep(def_num, 1, true)))
  {
    var link;
    if (link = DefinitionCall(def, "GetResearchLink"))
      if (Equal(link, target))
        SetPlrMagic(GetOwner(), def);
  }
  // Nachricht ausgeben
  Message("$Txtsdeveloped$", this(), GetName(nil, target));
  // Forschung beenden
  CancelResearch();
  target = nil;
  // Fertig
  return(1);  
}

/* Kontext-Zaubern */

public func ContextResearch(clonk) 
{
  [$TxtResearch$|Image=RSR6|Condition=CanResearch]
  return (ResearchMenu (RSR6, clonk));
}

public func ContextMagicLeft(object caller)
{
  [$TxtAimspellsleft$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, nil);
}

public func ContextMagicRight(object caller)
{
  [$TxtAimspellsright$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, 1);
}

/* Status */

protected func CanResearch() 
{
  return (And(ObjectCount(WTRS), (GetCon () ?? 0) >= 100));
}

public func CanMagic(object caller)
{
  var iPlr = GetOwner(); 
  if (!GetPlayerName(iPlr)) iPlr = caller->GetOwner();
  return(!Equal(GetPlrMagic(caller->GetOwner()), nil));
}
