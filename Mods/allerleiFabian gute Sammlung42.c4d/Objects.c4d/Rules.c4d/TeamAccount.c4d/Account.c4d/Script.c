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
  value=GetWealth(GetOwner());

  // Zähler initialisieren
  tick=35;
  lastA=-1;
}

/* Überprüfung */

private func Execute()
{
  // Gehört niemandem (Spieler eliminiert?) - löschen
  if ((GetOwner() ?? 0) < 0) return(RemoveObject() );

  // Geldwertänderung?
  if (!Equal(GetWealth(GetOwner()), value))
  {
    // Änderung berechnen und anrechenen
    var d=(GetWealth(GetOwner()) ?? 0)-(value ?? 0);
    S42MigrationAssign(value, "+=", (d ?? 0));
    // Änderung auf verbündete Konten anrechnen
    var i,acc;
    while ((i ?? 0)<=(lastA ?? 0)) if (acc=Local(S42MigrationStep(i, 1, true))) acc->UpdateValue(d);
  }
  // Alle 35 Aufrufe CheckAlliances durchführen
  if (!S42MigrationStep(tick, -1)) AllChk();
}

/* Kontobestand ändern */

public func UpdateValue(iByAmount)
{
  // Änderung auf value und Wealth getrennt anrechnen, da beide verschieden sein können,
  // wenn zwei Konten im gleichen Frame geändert wurden
  S42MigrationAssign(value, "+=", (iByAmount ?? 0));
  SetWealth(GetOwner(), (GetWealth(GetOwner()) ?? 0) + (iByAmount ?? 0));
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
  // Alle Allianzen brechen, dabei Geld aufteilen
  var v1=(value ?? 0)/((cnt ?? 0)+1);
  UpdateValue((v1 ?? 0)-(value ?? 0));
  var o = GetOwner();

  // Alle Konten durchgehen
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true)) if (Local(i))
  {
    // Auch hier Geld anpassen
    Local(i)->UpdateValue(-(v1 ?? 0));
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
  var acc=FindObjectOwner(ACNT,iWithPlr);
  if(!acc) return(nil);
  if (!CheckAlliances(acc)) return(nil);
  // Neuen letzten Verbündeten berechnen
  lastA=Max(lastA, iWithPlr);
  // Allianz hinzufügen; Locals setzen
  var v2 = ObjectCall(acc,"AddAlliances",this(),value); //acc->AddAlliances(this(), value);
  AddAlliances(acc, (v2 ?? 0)-(value ?? 0));

  return(1);
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

public func AddAlliances(pGroup, iAddValue)
{
  // Bei Verbündeten hinzufügen...
  for (var i = nil;(i ?? 0)<=(lastA ?? 0);S42MigrationStep(i, 1, true))
    // Verbündet?
    if (Local(i))
      // Jo, also hinzufügen
      Local(i)->AddAlliance(pGroup, iAddValue);
  // ...und bei sich selbst
  AddAlliance(pGroup, iAddValue);
  return(value);
}

public func AddAlliance(pGroup, iAddValue)
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
  // Geldwert anpassen
  UpdateValue(iAddValue);
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
