#strict

local state;

func Initialize()
{
  DrawMaterialQuad ("Granite",GetX()-8,GetY()     ,GetX()+8,GetY()     ,GetX()+8,GetY()+8 ,GetX()-8,GetY()+8 ,true);
  DrawMaterialQuad ("Granite",GetX()-8,GetY()-22-8,GetX()+8,GetY()-22-8,GetX()+8,GetY()-22,GetX()-8,GetY()-22,true);
  state = 0;
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
  var phase = 12-GetPhase();
  SetAction("GateClose");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Closed()
{
  state = 0;
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
  SetSolidMask(4*GetPhase(), 23, 4, 22, 0, 0);
}

//Target

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("OPEN");
	}
	if(a==2)
	{
		return("CLOSE");
	}
	if(a==3)
	{
		return("STOP");
	}
  return(1);
}

public func Trigger(a)
{
	if(a==1)
	{
		if(state == 0)
			Open();
	}
	if(a==2)
	{
		if(state == 1)
			Close();
	}
	if(a==3)
	{
		Stop();
	}
        return(1);	
}
