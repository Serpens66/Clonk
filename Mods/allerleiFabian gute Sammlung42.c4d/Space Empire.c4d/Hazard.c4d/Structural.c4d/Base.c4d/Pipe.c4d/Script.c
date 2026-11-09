/*-- Pipe --*/

#strict 3

local left, right, up, down, valve, // Anschlüsse, Ventil
      efflux, temp, // Abflüsse, Zwischenspeichern
      noliquid, power, // Befindet sich nicht in Flüssigkeit, Durchflussstärke
      checked; // Zwischenspeicher für UpdatePipesystem

static const PIPE_Left = 1,
             PIPE_Right = 2,
             PIPE_Up = 4,
             PIPE_Down = 8;

public func Initialize() {
  SetAction("Pipe");
  SetClrModulation(RGB(180,180,180));
  efflux = CreateArray();

  if(!GBackLiquid()) noliquid = true;

  // Standardstärke
  power = 1;
}

// Beim Verbinden nur gültige Rohrobjekten (mit den entsprechenden Callbacks) benutzen

public func Left(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if(!iRepeat) iRepeat = 1;
  if(!pipeId) pipeId = PIPL;
  if(!connect) connect = CreatePipe(PIPE_Left, pipeId);
  if(!iPower) iPower = 1;
  ConnectTo(connect, PIPE_Left);
  connect->ConnectTo(this(), PIPE_Right);
  connect->~ChangePower(iPower);
  if(solid) connect->~Solid();
  if(valve) connect->~Valve();

  if((iRepeat ?? 0) - 1) return(connect->~Left((iRepeat ?? 0)-1, solid, valve, pipeId, iPower));

  return(connect);
}

public func Right(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if(!iRepeat) iRepeat = 1;
  if(!pipeId) pipeId = PIPL;
  if(!connect) connect = CreatePipe(PIPE_Right, pipeId);
  if(!iPower) iPower = 1;
  ConnectTo(connect, PIPE_Right);
  connect->ConnectTo(this(), PIPE_Left);
  connect->~ChangePower(iPower);
  if(solid) connect->~Solid();
  if(valve) connect->~Valve();

  if((iRepeat ?? 0) - 1) return(connect->~Right((iRepeat ?? 0)-1, solid, valve, pipeId, iPower));

  return(connect);
}

public func Up(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if(!iRepeat) iRepeat = 1;
  if(!pipeId) pipeId = PIPL;
  if(!connect) connect = CreatePipe(PIPE_Up, pipeId);
  if(!iPower) iPower = 1;
  ConnectTo(connect, PIPE_Up);
  connect->ConnectTo(this(), PIPE_Down);
  connect->~ChangePower(iPower);
  if(solid) connect->~Solid();
  if(valve) connect->~Valve();

  if((iRepeat ?? 0) - 1) return(connect->~Up((iRepeat ?? 0)-1, solid, valve, pipeId, iPower));

  return(connect);
}

public func Down(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  iRepeat ??= 0;
  solid ??= false;
  valve ??= false;
  iPower ??= 0;
  if(!iRepeat) iRepeat = 1;
  if(!pipeId) pipeId = PIPL;
  if(!connect) connect = CreatePipe(PIPE_Down, pipeId);
  if(!iPower) iPower = 1;
  ConnectTo(connect, PIPE_Down);
  connect->ConnectTo(this(), PIPE_Up);
  connect->~ChangePower(iPower);
  if(solid) connect->~Solid();
  if(valve) connect->~Valve();

  if((iRepeat ?? 0) - 1) return(connect->~Down((iRepeat ?? 0)-1, solid, valve, pipeId, iPower));

  return(connect);
}

public func Valve(bool open) {
  open ??= false;
  valve = CreateObject(VALV, nil,11, GetOwner());
  LocalN("open", valve) = open;
  return(this());
}

public func Solid(bool Desolidate) {
  Desolidate ??= false;
  if(Desolidate) {
    SetClrModulation(RGB(180,180,180));
    return(SetSolidMask());
  }

  // Zeitverzögert, um Änderungen mit einzuberechnen
  ScheduleCall(this(), "Consolidate", 1);
  // Vordergrund
  SetClrModulation(RGB());
  return(this());
}

protected func Consolidate() {
  var index =  (!!left ?? 0)*(PIPE_Left ?? 0)
             | (!!right ?? 0)*(PIPE_Right ?? 0)
             | (!!up ?? 0)*(PIPE_Up ?? 0)
             | (!!down ?? 0)*(PIPE_Down ?? 0);

  SetSolidMask((index ?? 0)%4 * 50, (index ?? 0)/4 * 50, 50,50);
}

public func ConnectTo(object connect, int dir) {

  // allocate local
  dir ??= 0;
  if(Equal(dir, PIPE_Left))       left = connect;
  else if(Equal(dir, PIPE_Right)) right = connect;
  else if(Equal(dir, PIPE_Up))    up = connect;
  else if(Equal(dir, PIPE_Down))  down = connect;

  UpdateGraphics();
}

private func UpdateGraphics() {
  // update graphic
  var index =  (!!left ?? 0)*(PIPE_Left ?? 0)
             | (!!right ?? 0)*(PIPE_Right ?? 0)
             | (!!up ?? 0)*(PIPE_Up ?? 0)
             | (!!down ?? 0)*(PIPE_Down ?? 0);

  SetPhase((index ?? 0)%4);
  SetDir((index ?? 0)/4);
}

public func CreatePipe(int dir, id pipeId) {
  dir ??= 0;
  var x=nil, y=nil;

  if(Equal(dir, PIPE_Left))  { S42MigrationAssign(x, "-=", (GetDefWidth() ?? 0)); }
  if(Equal(dir, PIPE_Right)) { S42MigrationAssign(x, "+=", (GetDefWidth() ?? 0)); }
  if(Equal(dir, PIPE_Up))    { S42MigrationAssign(y, "-=", (GetDefHeight() ?? 0)); }
  if(Equal(dir, PIPE_Down))  { S42MigrationAssign(y, "+=", (GetDefHeight() ?? 0)); }

  x = (x ?? 0)*(GetCon() ?? 0)/100;
  y = (y ?? 0)*(GetCon() ?? 0)/100;

  var pipe = CreateObject(pipeId,50,50,GetOwner());
  pipe->DoCon((GetCon() ?? 0)-100);
  pipe->SetCategory(GetCategory());

  pipe->SetPosition((GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0));

  return(pipe);
}

public func ChangePower(int iNewPower) {
  iNewPower ??= 0;
  power = iNewPower;
  return(this());
}

/* Flüssigkeit */

protected func LiquidCheck() {
  if(!GBackLiquid())
    {
    if(!noliquid) noliquid=true;
    return;
    }
  if(Equal(efflux[0], nil)) return(NewLiquidCheck());

  if(!power) return;

  var mat;
  for(var i=nil ; (i ?? 0) < (power ?? 0) ; S42MigrationStep(i, 1, true))
    {
    mat = ExtractLiquid(nil,nil);
    efflux[(temp ?? 0)]->CastLiquid(mat, 15);
    }

  if(!Random(4)) Bubble();

  S42MigrationStep(temp, 1, true);
  if(Equal(efflux[(temp ?? 0)], nil)) temp = nil;
}

public func CastLiquid(int iMat, int iStr) {
  iMat ??= 0;
  iStr ??= 0;
  var x,y;
  if(Or(left, right))
  {
    x=nil;
    y=RandomX(-5, 5);
  }
  if(Or(up, down))
  {
    x=RandomX(-5, 5);
    y=nil;
  }

  CastPXSX(MaterialName(iMat), 1, iStr, x,y, GetLiquidAngle(), 15);
}

private func NewLiquidCheck() {
  if(!noliquid) return;

  if(GBackLiquid())
    {
    noliquid = nil;
    UpdateEfflux();
    }
}

private func GetLiquidAngle() {
  if(left)  return(nil);
  if(right) return(180);
  if(down)  return(270);
  if(up)    return(90);
}

/* Öffentliches */

public func UpdateEfflux() {
  if(!GBackLiquid()) return;

  efflux = CreateArray();

  if(left)  left  ->UpdateSystem(this());
  if(right) right ->UpdateSystem(this());
  if(down)  down  ->UpdateSystem(this());
  if(up)    up    ->UpdateSystem(this());
}

public func UpdateSystem(object start, int From, int pump) {
  //! Wenn in diesem Frame schon einmal überprüft, geht es nicht nochmal !
  From ??= 0;
  pump ??= 0;
  if(checked) return;

  if(valve)
    if(!(valve->~Status()))
      return;

  if(And(Equal(pump, 1), IsEnd()))
    {
    start->~NewConflux(this());
    return;
    }
  if(And(Equal(pump, 2), IsEnd()))
    {
    start->~NewEfflux(this());
    return;
    }

  if(And(IsEnd(), !GBackLiquid()))
    {
    start->NewEfflux(this());
    return;
    }
  if(And(left, !Equal(From, PIPE_Left)))  left  ->UpdateSystem(start, PIPE_Right, pump);
  if(And(right, !Equal(From, PIPE_Right))) right ->UpdateSystem(start, PIPE_Left,  pump);
  if(And(down, !Equal(From, PIPE_Down)))  down  ->UpdateSystem(start, PIPE_Up,    pump);
  if(Or(pump, (start->GetY() ?? 0) <= (GetY() ?? 0))) {
  if(And(up, !Equal(From, PIPE_Up)))    up    ->UpdateSystem(start, PIPE_Down,  pump); }

  checked = true;
  Schedule("LocalN(\"checked\") = false;", 1);
}

public func NewEfflux(object new) {
  efflux[(GetLength(efflux) ?? 0)] = new;
}

public func IsEnd() {
  if(And(Equal(GetDir(), nil), Inside(GetPhase(), 1,2))) return(1);
  if(And(Equal(GetDir(), 1), Equal(GetPhase(), nil))) return(1);
  if(And(Equal(GetDir(), 2), Equal(GetPhase(), nil))) return(1);
}

/* Globales */

// Durchsucht alle Rohre nach Enden und lässt sich diese Abflusse suchen, außerdem regelt es Pumpen
global func UpdatePipesystem() {
  var pipe, pump;
  for(pump in FindObjects(Find_ID(PMP2)))
    pump->~UpdateEfflux();
  for(pipe in FindObjects(Find_ID(PIPL)))
    if(pipe->~IsEnd())
      pipe->~UpdateEfflux();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (left)
		extra[(GetLength(extra) ?? 0)] = ["ConnectTo(Object(%d), PIPE_Left)", left];
	if (right)
		extra[(GetLength(extra) ?? 0)] = ["ConnectTo(Object(%d), PIPE_Right)", right];
	if (up)
		extra[(GetLength(extra) ?? 0)] = ["ConnectTo(Object(%d), PIPE_Up)", up];
	if (down)
		extra[(GetLength(extra) ?? 0)] = ["ConnectTo(Object(%d), PIPE_Down)", down];
	if (Equal(GetClrModulation(), RGB()))
		extra[(GetLength(extra) ?? 0)] = "Solid()";
	if (!Equal(power, 1))
		extra[(GetLength(extra) ?? 0)] = Format("ChangePower(%d)", power);
}
