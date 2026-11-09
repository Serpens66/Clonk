#strict 3

local state;

func Initialize()
{
  DrawMaterialQuad ("Granite",(GetX() ?? 0)-8,GetY()     ,(GetX() ?? 0)+8,GetY()     ,(GetX() ?? 0)+8,(GetY() ?? 0)+8 ,(GetX() ?? 0)-8,(GetY() ?? 0)+8 ,true);
  DrawMaterialQuad ("Granite",(GetX() ?? 0)-8,(GetY() ?? 0)-22-8,(GetX() ?? 0)+8,(GetY() ?? 0)-22-8,(GetX() ?? 0)+8,(GetY() ?? 0)-22,(GetX() ?? 0)-8,(GetY() ?? 0)-22,true);
  state = nil;
  Opened();
  return(1);
}

public func Open() // Aufmachen
{
  var phase = GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  StopSound();
  CheckSolidMask();
  state = 1;
}

public func Close() // Zumachen
{
  var phase = 12-(GetPhase() ?? 0);
  SetAction("GateClose");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Closed()
{
  state = nil;
  Stop();
}

public func Stop() // Sofort stoppen
{
  var phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  StopSound();
  CheckSolidMask();
}

private func StopSound() { Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(4*(GetPhase() ?? 0), 23, 4, 22, nil, nil);
}

//Target

public func GetTriggerName(a)
{
	if(Equal(a, 1))
	{
		return("OPEN");
	}
	if(Equal(a, 2))
	{
		return("CLOSE");
	}
	if(Equal(a, 3))
	{
		return("STOP");
	}
  return(1);
}

public func Trigger(a)
{
	if(Equal(a, 1))
	{
		if(Equal(state, nil))
			Open();
	}
	if(Equal(a, 2))
	{
		if(Equal(state, 1))
			Close();
	}
	if(Equal(a, 3))
	{
		Stop();
	}
        return(1);	
}
