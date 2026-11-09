/*-- König --*/

#strict 3

#include KNIG // Der König ist ein Ritter

local iLastSpell; // Letzter Spruchindex - wird im Zaubermenü wieder ausgewählt
local szKingdomName; // Name des Königreiches seiner Hoheit (für Spezialszenarien)

local idCurSpell;
local iCurPhase;

/* Hat nur ein Portrait */

public func StdPortraitCount() { return(1); }

/* Zauberkraft geben (Aufgerufen vom Aufwertungszauber) */

public func MagicEnergy()
{
  // Normalerweise wird die Physical schon im Redefine2 gesetzt - dies ist ein Workaround für Aufwertungen von Clonks,
  // die kein #include CLNK im Script haben
  if (!GetPhysical("Magic")) SetPhysical("Magic", GetPhysical("Magic", nil, nil, GetID()), PHYS_Temporary);
  // Und gleich auffüllen
  for (var i = nil; (i ?? 0) < 10; S42MigrationStep(i, 1, true))
    DoMagicEnergy(10);
  return(1);
}
 
/* Kontextmenü */  
  
public func ContextMagic(pCaller)
{
  [$CtxMagicDesc$|Image=MCMS|Condition=ReadyToMagic]
  // Nicht bereit
  if (!ReadyToMagic()) return(nil);
  // Clonk soll anhalten
  SetComDir(COMD_Stop, pCaller);
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=nil; else iExtra=3; }
  CreateMenu(MCMS, nil, nil, iExtra, "$MnuNoSpells$", GetMagicEnergy());
  var idSpell, i;
  while (idSpell = GetPlrMagic(GetOwner(), nil, S42MigrationStep(i, 1, true)))
    AddMenuItem("$MnuCast$: %s", "DoMagic", idSpell);
  // Letzten Spruch auswählen
  SelectMenuItem(iLastSpell);
  return(1);
}

protected func OnMenuSelection(iMenuIndex)
{
  // Nur Zaubermenü: Menüindex speichern (Für Wiederauswahl beim Schließen und Öffnen des Menüs)
  if (Equal(GetMenu(), MCMS)) iLastSpell = iMenuIndex;
  return;
}

/* Zaubern */  

// Der König kann immer zaubern
public func ReadyToMagic()  { return(1); }
  
protected func DoMagic(idSpell)
{
  // Nicht bereit
  if (!ReadyToMagic()) return;

  // Nicht genug Zauberenergie
  if(!CheckMana(idSpell))
  {
    Sound("Error");
    Message("$MsgNotEnoughEnergy$",this());
    return;
  }
  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell,this()))
  {
    Sound("Error");
    Message("$MsgNotEnoughIngredients$",this(),GetNeededAlcStr(idSpell,this()));
    return;
  }

  if(IsConvertReady() )
  {
    idCurSpell = idSpell;
    SetMagicAction();
  }
  else
    ExecMagic(idSpell);
}
  

/* Status */

public func & GetKingdomName() { return (szKingdomName); }


// Zur Sicherheit, damit Überladungen keinen Fehler beim Starten geben
protected func Scaling()
{
  var szDesiredAction;
  if ((GetYDir() ?? 0) > 0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (!SEqual(GetAction(), szDesiredAction)) SetAction(szDesiredAction);
  return(1);   
}

protected func CheckStuck()
{                   
  // Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if((Abs(GetYDir()) ?? 0) < 5)
    if(GBackSolid(nil, 3))
      SetPosition(GetX(), (GetY() ?? 0) + 1);
}

/* Inhalt prüfen: Kristalle verarbeiten */

protected func Collection2(pItem)
{
  if (FindContents(CRYS)) ConvertCrystal();
  return(1);
}

/* Energiegewinnung */

private func ConvertCrystal(pClonk)
{
  if(!IsConvertReady())   return(nil);
  
  var crystal = FindContents(CRYS);
  if(!crystal) return(nil);
  
  if(!crystal->~Apply(this()) )
    if(DoMagicEnergy(+10) )
      RemoveObject(crystal);
    else
      return(nil);
  
  // Clonk soll anhalten
  SetComDir(COMD_Stop,pClonk);

  SetMagicAction();
  Sound("Electric");
  return(1);
}

private func Riding()
{
  ConvertCrystal();
  return(_inherited());
}

protected func AfterMagic()
{
  // Callback nach Magie-Animation: Nach weiteren Kristallen suchen
  if (FindContents(CRYS)) ConvertCrystal();

  idCurSpell = nil;
  iCurPhase = nil;

  return(1);
}
  
private func AfterMagicJump()
{
  if(And(idCurSpell, !!SEqual(GetAction(), "Walk")))
  {
    SetAction("Magic");
    SetPhase(iCurPhase);
  }
  else AfterMagic();
}

public func LightningStrike()
{
  // Wenn der Clonk tot ist, zerstört ihn der erste Blitz
  if (!!SEqual(GetAction(), "Dead"))
  {
    CastObjects(SPRK, 5, 20);
    CreateParticle("MSpark", nil,nil, 1000, RGBa(50,50,255,150));
    return(RemoveObject());
  }
  // Clonk soll anhalten
  SetComDir(COMD_Stop);
  // Energiegewinnung
  var iEnergyGain = 10;
  // Extra-Energiegewinnung durch die Krone?
  if (FindContents(_CRN)) S42MigrationAssign(iEnergyGain, "+=", 10);
  while (S42MigrationStep(iEnergyGain, -1, true)) DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion
  if(IsConvertReady() ) SetMagicAction();
  return(1);
}
  
public func IsConvertReady()
{
  // Im Laufen und Reiten können Kristalle umgewandelt werden
  return(Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Ride")), !!SEqual(GetAction(), "RideStill")));
}

/* Kann keine Rüstung tragen */

public func WearArmor(pArmor)
{
  Sound("KnightFailure");
  Message("$MsgCantWearArmor$", this());
  return(nil);
}

private func SetMagicAction()
{ 
  // Zaubern im Sprung
  if(!!SEqual(GetAction(), "Jump")) return(SetAction("JumpMagic") );
  
  // Aktion setzen
  if(!IsRiding() ) return(SetAction("Magic") );
  // Beritten? Aufs Zaubern mögen wir dennoch nicht verzichten.
  else SetAction("RideMagic");
}

private func CheckMagic()
{
  // Nur in Phase 5 zaubern
  iCurPhase = GetPhase();
  if(!Equal(iCurPhase, 5)) return;
  
  // Kein Zauber? :[
  if(!idCurSpell) return;
 
  // Zauberspruch ausführen
  ExecMagic(idCurSpell);
}

private func ResetMagic()
{
  if(Equal(GetAction(), "Magic")) SetAction("Walk");
  if(Equal(GetAction(), "JumpMagic")) SetAction("Jump");
  if(Equal(GetAction(), "RideMagic")) SetAction("Ride");
}

private func ExecMagic(idSpell)
{ 
  // Zauberobjekt erschaffen und aktivieren
  if (!CreateObject(idSpell,nil,nil,GetOwner())->~Activate(this()))
    // Zauber fehlgeschlagen
    return(ResetMagic() );

  // Zauberenergie abziehen
  DoMagicEnergy(-(Value(idSpell) ?? 0));

  // alchemische Zutaten reduzieren
  if(ObjectCount(ALCO)) ReduceAlchem(idSpell,this());

  // Aktuelle Zauberkraft trainieren
  if (GetPhysical("Magic",1))
    SetPhysical("Magic", Min((GetPhysical("Magic") ?? 0)+(Value(idSpell) ?? 0), 100000), nil);

  // Fertig
  return(1);
}

