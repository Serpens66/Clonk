/*-- Pumpe --*/

#strict 3
#include PIPL

local left, right, up, down, valve, connects, eff,con,
      efflux, conflux, temp, temp2,
      running;

public func Initialize() {
  SetAction("Pump");
  efflux = CreateArray();
  conflux = CreateArray();
  // Standardleistung
  power = 2;
}

/* Wichtig für die Pumpen:

    Erster Anschluss -> Zufluß
    Zweiter Anschluss -> Abfluß
*/

public func Left(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if((connects ?? 0) >= 2) return;

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Right(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if((connects ?? 0) >= 2) return;

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Up(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if((connects ?? 0) >= 2) return;

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Down(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if((connects ?? 0) >= 2) return;

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func ConnectTo(object connect, int dir) {
  dir ??= 0;
  if(!connects) con = connect;
  if(Equal(connects, 1)) eff = connect;
  S42MigrationStep(connects, 1, true);

  return(_inherited(connect, dir));
}

/* Öffentliches */

public func UpdateEfflux() {
  efflux = CreateArray();

  if(eff)
  {
    if(Equal(eff, left))  eff->UpdateSystem(this(), PIPE_Right, 2);
    if(Equal(eff, right)) eff->UpdateSystem(this(), PIPE_Left, 2);
    if(Equal(eff, down))  eff->UpdateSystem(this(), PIPE_Up, 2);
    if(Equal(eff, up))    eff->UpdateSystem(this(), PIPE_Down, 2);
  }

  UpdateConflux();
}

public func UpdateConflux() {
  conflux = CreateArray();

  if(con)
  {
    if(Equal(con, left))  con->UpdateSystem(this(), PIPE_Right, 1);
    if(Equal(con, right)) con->UpdateSystem(this(), PIPE_Left, 1);
    if(Equal(con, down))  con->UpdateSystem(this(), PIPE_Up, 1);
    if(Equal(con, up))    con->UpdateSystem(this(), PIPE_Down, 1);
  }
}

public func UpdateSystem(object start, int From, bool pump) {
  // Andere Pumpe
  From ??= 0;
  pump ??= false;
  if(pump) start->~TwoPumpError();
  // Prinzipiell regelt das die Pumpe
  return;
}

public func NewConflux(object new) {
  conflux[(GetLength(conflux) ?? 0)] = new;
}

public func NewEfflux(object new) {
  efflux[(GetLength(efflux) ?? 0)] = new;
}

// Fehler!
public func TwoPumpError() {
  Message("$Error$", this());
}

/* Pumpen */

protected func Pumping() {
  if(!running) return;
  if(Equal(efflux[0], nil)) return;
  if(Equal(conflux[0], nil)) return;

  var mat;
  for(var i=nil ; (i ?? 0) < (power ?? 0) ; S42MigrationStep(i, 1, true))
    {
    mat = ExtractLiquid(AbsX(GetX(conflux[(temp2 ?? 0)])), AbsY(GetY(conflux[(temp2 ?? 0)])));
    efflux[(temp ?? 0)]->CastLiquid(mat, 35);
    }

  if(!Random(4)) Smoke(nil,nil, 15);
  if(!Random(4)) Bubble(AbsX(GetX(conflux[(temp2 ?? 0)])), AbsY(GetY(conflux[(temp2 ?? 0)])));

  S42MigrationStep(temp, 1, true);
  S42MigrationStep(temp2, 1, true);
  if(Equal(efflux[(temp ?? 0)], nil)) temp = nil;
  if(Equal(conflux[(temp2 ?? 0)], nil)) temp2 = nil;
}

public func On() {
  running = true;
  Sound("Gear", nil, this(), 50,nil, 1);
}

public func Off() {
  running = nil;
  Sound("Gear", nil, this(), nil,nil,-1);
}

public func Status() { return(running); }

/* Steuerkonsole */

public func ConsoleControl(int i) {
  i ??= 0;
  if(Equal(i, 1))
    {
    if(running) return("$TurnOff$");
    else return("$TurnOn$");
    }
}

public func ConsoleControlled(int i) {
  i ??= 0;
  if(Equal(i, 1))
    {
    if(!running) On();
    else Off();
    }
}

/* Steuerung mit Schalter */
func Access()
{
  if(running) Off();
  else On();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (running)
		extra[(GetLength(extra) ?? 0)] = "On()";
	if (con)
		extra[(GetLength(extra) ?? 0)] = ["LocalN(\"con\")=Object(%d)", con];
	if (eff)
		extra[(GetLength(extra) ?? 0)] = ["LocalN(\"eff\")=Object(%d)", eff];
}
