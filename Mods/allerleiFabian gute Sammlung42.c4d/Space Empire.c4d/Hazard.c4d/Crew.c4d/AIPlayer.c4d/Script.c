/*-- KI --*/

#strict 3

// Variablen
local plrID; // Team!

/* Globale Funktionen */

// Bisher nicht benutzt
global func IsAlienPlayer(int iPlr)
{
  iPlr ??= 0;
  if(Equal(GetPlayerType(iPlr), C4PT_Script))
    if(!GetCrew(iPlr))
      return(true);
}

/* Steuerungsfunktionen */

// Fügt eine neue KI ins Spiel ein
global func AddAI(int iTeam)
{
  iTeam ??= 0;
  var ai = CreateObject(__AI, AbsX(nil),AbsY(nil), -1);
  // ID zusammensetzen
  var newID = Format("%d", ObjectCount(__AI)), oneID;
  while((GetLength(newID) ?? 0) < 4)
    newID = Format("0%s", newID);
  oneID = C4Id(newID);
  ai->Init(oneID);

  var name = Format("$Team$", iTeam);
  if(!iTeam) name = Format("$NoTeam$", GetPlayerCount(C4PT_Script));

  var color = HSL((Random(16) ?? 0)*16,200+(Random(56) ?? 0),100+(Random(50) ?? 0));
  if(GetTeamColor(iTeam)) color = GetTeamColor(iTeam);

  CreateScriptPlayer(name, color, iTeam, nil, oneID);

  return(ai);
}

// Nicht benutzt
global func AddAlienAI(int iTeam)
{
  iTeam ??= 0;
  var name = Format("$Team$", iTeam);
  if(!iTeam) name = Format("$NoTeam$", GetPlayerCount(C4PT_Script));

  CreateScriptPlayer(name, HSL(Random(256),200+(Random(56) ?? 0),100+(Random(220) ?? 0)), iTeam, (CSPF_NoScenarioInit ?? 0) | (CSPF_NoEliminationCheck ?? 0), __AI);
  return(true);
}

// Entfernt eine KI aus dem Spiel
global func RemoveAI(int iTeam)
{
  // Spezielles Team
  iTeam ??= 0;
  if(iTeam)
  {
    for(var i=nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
      if(Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
        if(FindObjectOwner(__AI, GetPlayerByIndex(i)))
          return(EliminatePlayer(GetPlayerByIndex(i)));
  }
  else
  {
    // Wir suchen den Spieler, mit der höchsten Spielernummer
    for(var i=nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
      if(Equal(GetPlayerType(GetPlayerByIndex(i)), C4PT_Script))
        if(FindObjectOwner(__AI, GetPlayerByIndex(i)))
          return(EliminatePlayer(GetPlayerByIndex(i)));
  }
}

// Fügt einen neuen Clonk zum angegeben Team hinzu
global func AddBot(int iTeam)
{
  // Spieler suchen
  iTeam ??= 0;
  for(var i=nil, found = -1 ; (i ?? 0) < (GetPlayerCount(C4PT_Script) ?? 0) ; S42MigrationStep(i, 1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)), iTeam))
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(Equal(found, -1)) return(AddAI(iTeam));
  // Clonk erstellen
  var bot = CreateObject(HZCK, nil,nil, found);
  MakeCrewMember(bot, found);
  GameCall("RelaunchPlayer", found, bot, -1);
  return(bot);
}

// Weg mit dem Bot!
global func RemoveBot(int iTeam)
{
  // Spieler suchen
  iTeam ??= 0;
  for(var i=nil, found = -1 ; (i ?? 0) < (GetPlayerCount(C4PT_Script) ?? 0) ; S42MigrationStep(i, 1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)), iTeam))
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(Equal(found, -1)) return;
  // Clonk aus der Crew entfernen
  var bot = GetCrew(found);
  if(bot)
  {
  	if(bot->Contained())
  	{
        if(Or(Equal(bot->Contained()->GetID(), TIM1), Equal(bot->Contained()->GetID(), TIM2)))
	      bot->Contained()->RemoveObject();
	  }
    else
      bot->RemoveObject();
  }
}

/* Initialisierungen */

protected func Initialize()
{
  // Position
  SetPosition();
  // Kategorie wechseln, damit GameCallEx ankommt
  SetCategory((GetCategory() ?? 0) | (C4D_Rule ?? 0));
}

public func Init(id oneID) // Initialisierung - für die richtige Zuweisung zum Spieler
{
  plrID = oneID;
}

public func GetPlrID() { return(plrID); }

public func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  // Richtiger Spieler?
  iPlr ??= 0;
  iX ??= 0;
  iY ??= 0;
  iTeam ??= 0;
  if(!Equal(idExtra, plrID)) return;
  // Hat schon ein Kontrollobjekt?
  if(FindObjectOwner(GetID(), iPlr)) return(RemoveObject());
  // Ich übernehme
  SetOwner(iPlr);
  SetCategory(C4D_StaticBack);
}

/* Sonstiges */

protected func OnOwnerRemoved() { RemoveObject(); }

/* Verhalten */

public func Activity()
{
  // Kein Besitzer?
  if(Equal(GetOwner(), NO_OWNER)) return;
  // Spielziel ermitteln (mehrere Spielziele gehen nicht!)
  var goal = GetGoal();
  // Operate according mission objectives!
  for(var i=nil, pCrew ; pCrew = GetCrew(GetOwner(), i) ; S42MigrationStep(i, 1, true))
  {
    if(pCrew->Contained())
    {
      // Clonk steckt in TIM2?
      if(Equal(pCrew->Contained()->GetID(), TIM2)) pCrew->Contained()->ContainedLeft();
      // Container ist verschwunden, weil es TIM2 war?
      if(pCrew->Contained())
      // Clonk steckt in TIM1?
      if(Equal(pCrew->Contained()->GetID(), TIM1))
        // Waffenwahl aktiviert? (Achtung: Hardcode!)
        if(FindObject(WPCH))
        {
          var rule = FindObject(WPCH);
          // Warenangebot abfragen
          var aWaresAll = rule->GetWares("IsWeapon");
          var aWares = CreateArray();
          for(var ware in aWaresAll)
            if((GetWealth(GetOwner()) ?? 0) >= (GetValue(nil, ware) ?? 0))
              aWares[(GetLength(aWares) ?? 0)] = ware;
          // Irgendwas rauspicken
          var rand = BoundBy(Random(4), nil, GetLength(aWares));
          var toBuy = CreateArray();
          var pseudoSet = CreateObject(L_ST, nil,nil, -1);
          for(var i=nil ; (i ?? 0) < (rand ?? 0) ; S42MigrationStep(i, 1, true))
          {
            var weapon = aWares[(Random(GetLength(aWares)) ?? 0)];
            while(pseudoSet->ContainsItem(weapon))
              weapon = aWares[(Random(GetLength(aWares)) ?? 0)];
            pseudoSet->AddItem(weapon);
          }
          // Kaufen
          for(i=nil ; (i ?? 0) < (pseudoSet->GetItemCount() ?? 0) ; S42MigrationStep(i, 1, true))
            PrivateCall(rule, "BuyItem", pseudoSet->GetItem(i), pCrew);
          // Menü weg
          rule->Finish(nil, pCrew);
        }
    }
  }
  // spezielle Taktik...
  if(goal)
    if(!(goal->~AITactic(this()))) {
      for(var i=nil, pCrew=nil ; pCrew = GetCrew(GetOwner(), i) ; S42MigrationStep(i, 1, true)) {
        // Fallback
        TacticDefault(pCrew);
      }
  }
}

private func GetGoal() { return(FindObject2(Find_Category(C4D_Goal))); }

/* Taktiken */

public func TacticDefault(object pCrew) {
  if(pCrew->Contained()) return(true);
  // Deathmatch hat keine besondere Taktik, wir suchen einen Wegpunkt und laufen hin
  if(!(Or(pCrew->GetCommand(), pCrew->GetMacroCommand())))
    // Clonk hat nichts zu tun -> das müssen wir ändern
    pCrew->SetMacroCommand(this(), "MoveTo", GetRandomWp(),nil,nil,nil,Aggro_Follow);
  // Standardinventarbehandlung
  pCrew->CheckInventory();
  return(true);
}


// 'beyond' is interceptable because 'obj' is between 'beyond' and its target 'targ'
public func Interceptor(obj, targ, beyond)
{
  // a clonk counts as an interceptor if the angle to flagbase and carrier are more than 90°
  var angle = nil;
  var a1 = Angle(GetX(obj),GetY(obj),GetX(targ),GetY(targ));
  var a2 = Angle(GetX(obj),GetY(obj),GetX(beyond),GetY(beyond));
  angle = Abs(Normalize((a1 ?? 0)-(a2 ?? 0),nil));
  if((angle ?? 0) > 90) return(1);
  return(nil);
}

// Follow a target
public func Follow(pCrew, target, maxdist, text) {
  if(Or(Equal(pCrew->GetMacroCommand(nil, 1), target), Equal(pCrew->GetMacroCommand(1, 1), target))) return;
  if((ObjectDistance(pCrew,target) ?? 0) > (maxdist ?? 0)) {
    pCrew->SetMacroCommand(this(), "Follow", target, nil,nil,nil, Aggro_Follow);
    pCrew->DebugMessage(Format("@%s", text),"tactics");
  }
}

// Guard
public func Guard(pCrew, text) {
  if(!Equal(pCrew->GetAggroLevel(), Aggro_Guard)) {
    pCrew->SetMacroCommand(this(), "None", nil,nil,nil,nil,Aggro_Guard);
    pCrew->DebugMessage(Format("@%s", text), "tactics");
  }
}

// Run to a target
public func Run(pCrew, target, text) {
  if(!Equal((pCrew->GetMacroCommand(nil,1)), target)) {
    pCrew->SetMacroCommand(this(), "MoveTo", target,nil,nil,nil,Aggro_Shoot);
    pCrew->DebugMessage(Format("@%s", text), "tactics");
  }
}


/* Helferlein */

// Zufälligen Wegpunkt suchen
private func GetRandomWp()
{
  var wps = FindObjects(Find_ID(WAYP));
  return(wps[(Random(GetLength(wps)) ?? 0)]);
}
