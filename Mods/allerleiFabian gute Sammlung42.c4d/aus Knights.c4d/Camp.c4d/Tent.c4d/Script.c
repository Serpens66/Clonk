/*-- Zelt --*/

#strict 3

local flag;    // Flagge
local flagOwn; // Besitzer



/* Initialisierung */

public func Initialize()
{
  // Eingang aufmachen
  SetEntrance(1);
}

/* Aufbau */

public func BeginCon()
{
  // Zelt wird aufgebaut: Nicht betretbar
  SetEntrance(nil);
  // Animation & Effekt setzen
  SetAction("Con");
  AddEffect("IntCon", this(), 1, 1, this() );
}

protected func FxIntConTimer(object target, int number)
{
  // Arbeiter suchen
  number ??= 0;
  var workers = ObjectCount(nil, nil,nil,nil,nil, OCF_CrewMember, "Push", target);
  // Keine Arbeiter: wieder zusammenfallen
  if(And(!workers, (GetActTime() ?? 0) > 5))
  {
    Pack();
    return(-1);
  }
  // Maximal drei Arbeiter
  if((workers ?? 0) > 3) workers = 3;
  // Aufbauen
  var iLastPhase = GetPhase();
  S42MigrationAssign(EffectVar(nil, target, number), "+=", (workers ?? 0));
  if ((EffectVar(nil, target, number) ?? 0) > 450) // Türöffnen geht schnell
    S42MigrationAssign(EffectVar(nil, target, number), "+=", 20);
  SetPhase((EffectVar(nil, target, number) ?? 0) * 20 / ((GetMass() ?? 0) * 3 / 2) );
  // Geräusch  
  if (!Equal(iLastPhase, GetPhase()))
    if ((EffectVar(nil, target, number) ?? 0) < 450)
      Sound("TentPacking*");
  // Fertig
  if((EffectVar(nil, target, number) ?? 0) >= (GetMass() ?? 0) * 3 / 2)
  {
    ConComplete();
    return(-1);
  }
}

/* Aufbau fertig */

private func ConComplete()
{
  // Eingang oeffnen
  SetEntrance(1);
  // Feedback fuer Spieler: Ist der Bau fertig laesst der Clonk los
  var obj;
  while(obj = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, "Push", this(), nil, obj) )
    AddCommand(obj, "UnGrab");
  SetAction("Idle");
}

/* Kann man das Zelt einpacken? */

public func CanPack()
{
  // Inhalt prüfen
  while(Contents())
  {
    // Tote Clonks rauswerfen
    if (And((GetOCF(Contents()) ?? 0) & (OCF_Living ?? 0), !GetAlive(Contents())))
      Exit(Contents());
    // Sonst nicht
    else
      return(nil);
  }
  return(true);
}

/* Zelt einpacken */

public func Pack()
{
  
  // Geht es ueberhaupt?
  if(!CanPack() )
  {
    // Meldung + Sound
    Message("$MsgTentMustBeEmpty$", this());
    Sound("Error");
    return(nil);
  }
  
  // Eingang dicht machen
  SetEntrance(nil);
  
  // Attachte Objekte (Flaggen, etc.) loswerden
  var obj;
  while(obj = FindObject(nil, nil,nil,nil,nil, nil, nil, this(), nil, obj) )
    if(!!SEqual(obj->GetProcedure(), "ATTACH"))
    {   
      obj->SetAction("Idle");
      flag = GetID(obj);       // Speichern
      flagOwn = GetOwner(obj); // Speichern
      RemoveObject(obj);
    }
  
  // Zu setzende Phase bestimmen, falls das Zelt
  // mitten im Aufbau wieder eingepackt werden muss
  var phase = nil;
  if(!!SEqual(GetAction(), "Con")) phase = 19 - (GetPhase() ?? 0);
  
  // Action aendern
  SetAction("Pack");
  SetPhase(phase);
  
  // Akustik
  Sound("TentPack");
}

protected func Damage()
{
  if(Equal(GetEntrance(this()), nil))
  {
    // Flagge hat den Schaden überlebt
    var iyOut; if (!GetR()) iyOut = (GetCon() ?? 0)/5;
    CreateObject(flag, nil, iyOut, flagOwn);
  }
}
    

/* Einpacken fertig */

private func PackComplete()
{
  // Zeltpaket erzeugen
  var iyOut; if (!GetR()) iyOut = (GetCon() ?? 0)/5;
  CreateObject(TENP, nil, iyOut, GetOwner());
  CreateObject(flag, nil, iyOut, flagOwn);     // und wiederherstellen!
  // Zelt loeschen
  RemoveObject();
}

/* Steuerung: Einpacken */

protected func ControlDigDouble()
{
  [$CtrlPack$]  
  // Kann nicht während des Auf- oder Abbaus eingepackt werden
  if (!(!!SEqual(GetAction(), "Idle")))
    return(nil);
  // Wieder einpacken
  Pack();
  return(1);
}

protected func SellTo() { return(TENP); }
