/*-- Konto --*/

#strict 3

local value; // Geld auf dem Konto
local tick;  // Zähler
local lastA; // Größte Spielernummer der Verbündeten
local cnt;   // Anzahl Verbündeter

// Local(n): Konto von Spieler n, wenn befreundet

/* Zerstörung */

protected func Destruction()
{
  // Befreundete Accounts benachrichtigen
  var i,acc;
  while ((i ?? 0)<=(lastA ?? 0))
    if (acc=Local(S42MigrationStep(i, 1, true)))
      acc->AllianceDie(this() );
}

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  var idDef;
  for(var i=nil; idDef = GetHomebaseMaterial (GetOwner(), nil, i, C4D_Object); S42MigrationStep(i, 1, true))
  {
    // Geldwertänderung?
    SetComponent(idDef, GetHomebaseMaterial(GetOwner(), idDef));
  }

  // Zähler initialisieren
  tick=35;
  lastA=-1;
}

/* Überprüfung */

private func Execute()
{
  // Gehört niemandem (Spieler eliminiert?) - löschen
  if ((GetOwner() ?? 0) < 0) return(RemoveObject() );

  var idDef;
  for(var i=nil; idDef = GetHomebaseMaterial (GetOwner(), nil, i, C4D_Object); S42MigrationStep(i, 1, true))
  {
    // Geldwertänderung?
    if (!Equal(GetHomebaseMaterial(GetOwner(), idDef), GetComponent(idDef)))
    {
      // Änderung berechnen und anrechenen
      var d=(GetHomebaseMaterial(GetOwner(), idDef) ?? 0)-(GetComponent(idDef) ?? 0);
      SetComponent(idDef, (GetComponent(idDef) ?? 0)+(d ?? 0));
      // Änderung auf verbündete Konten anrechnen
      var j,acc;
      while ((j ?? 0)<=(lastA ?? 0)) if (acc=Local(S42MigrationStep(j, 1, true))) acc->UpdateValue(d, idDef);
    }
  }
  // Alle 35 Aufrufe CheckAlliances durchführen
  if (!S42MigrationStep(tick, -1)) AllChk();
}

/* Kontobestand ändern */

public func UpdateValue(iByAmount, idDef)
{
  if(!idDef) return;
  // Änderung auf value und Wealth getrennt anrechnen, da beidDefe verschieden sein können,
  // wenn zwei Konten im gleichen Frame geändert wurden
  SetComponent(idDef, (GetComponent(idDef) ?? 0)+(iByAmount ?? 0));
  DoHomebaseMaterial(GetOwner(), idDef, iByAmount); 
}

/* Allianzänderungen prüfen und Konten entsprechend umkonfigurieren */

private func AllChk()
{
  // Zähler zurücksetzen
  tick=35;
  // Prüfen, ob Allianzen gebrochen wurden
  var i,o = GetOwner();
  // Alle Konten der Teammitglieder durchgehen
  for (;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true))
    // Konto vorhanden und verfeindet?
    if(And(Local(i), Hostile(o,i)))
      // Konto löschen
      BreakAlliance(i);

  // Prüfen, ob neue Allianzen geschlossen wurden
  for(i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    // Spieler ermitteln
    var plr = GetPlayerByIndex(i);
    // Nicht verfeindet und noch kein Konto reserviert?
    if(And(!Hostile(o,plr), !Local(plr)))
      // Neues Konto erstellen
      CreateAlliance(plr);
  }
}

/* Konto aus den Teamkonten löschen */

private func BreakAlliance(iPlr)
{
  var idDef;
  for(var i=nil; idDef = GetHomebaseMaterial (GetOwner(), nil, i, C4D_Object); S42MigrationStep(i, 1, true))
  {
    // Alle Allianzen brechen, dabei Geld aufteilen
    var v1=(GetComponent(idDef) ?? 0)/((cnt ?? 0)+1);
    UpdateValue((v1 ?? 0)-(GetComponent(idDef) ?? 0), idDef);
    var o = GetOwner();

    // Alle Konten durchgehen
    for (var j = nil;(j ?? 0)<=(lastA ?? 0);S42MigrationStep(j, 1, true)) if (Local(j))
    {
      // Auch hier Geld anpassen
      Local(j)->UpdateValue(-(v1 ?? 0), idDef);
    }
  }

  // Alle Konten durchgehen
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true)) if (Local(i))
  {
    // Und diesen Spieler aus der Allianz löschen
    Local(i)->RemoveAlliance(o);
    // Uns selbst natürlich auch löschen :)
    RemoveAlliance(i);
  }
  
  cnt=nil;
  lastA=-1;
}

/* Neuen Spieler in die Allianz aufnehmen */

private func CreateAlliance(iWithPlr)
{
  // Verbündete Spieler überprüfen: Alle müssen verbündet sein
  var acc=FindObjectOwner(BACN,iWithPlr);
  if(!acc) return(nil);
  if (!CheckAlliances(acc)) return(nil);
  // Neuen letzten Verbündeten berechnen
  lastA=Max(lastA, iWithPlr);
  // Allianz hinzufügen; Locals setzen
  var idDef;
  for(var i=nil; idDef = GetHomebaseMaterial (GetOwner(), nil, i, C4D_Object); S42MigrationStep(i, 1, true))
  {  
    var v2 = ObjectCall(acc,"AllianceAddMaterials",GetComponent(idDef), idDef);
    AllianceAddMaterials((v2 ?? 0)-(GetComponent(idDef) ?? 0), idDef);
  }
  for(var i=nil; idDef = GetHomebaseMaterial (iWithPlr, nil, i, C4D_Object); S42MigrationStep(i, 1, true))
  {
    if(GetHomebaseMaterial(GetOwner(), idDef)) continue; // Wurde schon behandelt
    var v2 = ObjectCall(acc,"AllianceAddMaterials",GetComponent(idDef), idDef);
    AllianceAddMaterials((v2 ?? 0)-(GetComponent(idDef) ?? 0), idDef);
  }
  var v2 = ObjectCall(acc,"AddAlliances",this()); //acc->AddAlliances(this(), value);
  AddAlliances(acc);
  return(1);
}

public func AllianceAddMaterials(iAddValue, idDef)
{
  // Bei Verbündeten hinzufügen...
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true))
    // Verbündet?
    if (Local(i))
      // Jo, also hinzufügen
      Local(i)->UpdateValue(iAddValue, idDef);
  // ...und bei sich selbst
  UpdateValue(iAddValue, idDef);
  return(GetComponent(idDef));
}

/* Prüfen, ob dieser Spieler mit der Allianzgruppe pWith verbündet ist */

private func CheckAlliances(pWith)
{
  // Allianzen für alle verbündeten Konten prüfen...
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true)) if (Local(i))
    if (!CheckAlliance(Local(i), pWith)) return(nil);
  // ...und für sich selbst
  return(CheckAlliance(this(), pWith));
}

/* Prüfen, ob der Account pAcc mit pWith verbündet ist */

public func CheckAlliance(pAcc, pWith)
{
  // Besitzer von pAcc ermitteln 
  var o = GetOwner(pAcc);
  // Jeden Spieler durchgehn
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Konto für diesen Spieler in pWith vorhanden?
    if(Local(plr, pWith) )
      // Verfeindet?
      if(Hostile(o, plr) )
        // Dann war's wohl nix.
        return(nil);
  }
  // Zuletzt noch prüfen, ob der Spieler mit pWith selbst verbündet ist.
  return(!Hostile(o, GetOwner(pWith)) );
}

public func AddAlliances(pGroup)
{
  // Bei Verbündeten hinzufügen...
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true))
    // Verbündet?
    if (Local(i))
      // Jo, also hinzufügen
      Local(i)->AddAlliance(pGroup);
  // ...und bei sich selbst
  AddAlliance(pGroup);
  return;
}

public func AddAlliance(pGroup)
{
  // Locals setzen
  var o = GetOwner(pGroup);
  // Jeden SPieler durchgehn
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Verbündet?
    if(Local(plr, pGroup) )
    {
      // Sind wie das selbst?
      if(Or(Local(plr), Equal(plr, GetOwner())) ) continue;
      // Konto hinzufügen
      Local(plr) = Local(plr, pGroup);
      // Neue höchste Spielernummer in diesem Team ermitteln
      lastA = Max(lastA, plr);
      // Diese Allianz hat ein Mitglied mehr
      S42MigrationStep(cnt, 1);
    }
  }
  
  // Auch hier neue Allianz hinzufügen
  Local(o)=pGroup;
  // Und neue höchste Spielernummer setzen
  lastA=Max(lastA, o);
  S42MigrationStep(cnt, 1);
}

/*  Allianz stirbt: Geldwert belassen und Zähler verringern */

public func AllianceDie()
{
  return(S42MigrationStep(cnt, -1));
}

public func RemoveAlliance(iAlliance)
{
  // Allianz entfernen; Zähler verringern
  if (!Local(iAlliance)) return(nil);
  Local(iAlliance) = nil;
  S42MigrationStep(cnt, -1);
  while (And(!Local(lastA), lastA)) S42MigrationStep(lastA, -1);
  return(1);
}


/* Info */

public func GetTeam()
{
  // Kein Team? :'(
  if (!cnt) return;
  // Alle Allianzmitglieder speichern
  var j,k;
  for (var i = nil; (i ?? 0) <= (lastA ?? 0); S42MigrationStep(i, 1, true))
    if (Local(i))
      Var(S42MigrationStep(j, 1, true)) = GetPlayerName(i);

  // Erstes Mitglied holen
  var rs = Var(nil);

  // Und alle weiteren mit Komma getrennt dranhängen
  while (S42MigrationStep(k, 1) < (j ?? 0))
    if (Var(k))
      rs = Format("%s, %s", rs, Var(k));
      
  return(rs);
}
