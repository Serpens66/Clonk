/*-- Kartenprojektor --*/

#strict 3

local iNew,Active; // Nächste Phase

func Completion() {
  SetAction("Move");
	SetPhase(Random(20));
  return(1);
}

func Move()
{
  if(!!SEqual(GetAction(), "Stand")) return;
	var iPhase=GetPhase();
	if( !Equal(iPhase, iNew))
		SetPhase(BoundBy(iNew, (iPhase ?? 0)-1, (iPhase ?? 0)+1)); // Noch nicht da
	else if(!Random(50)) // Hinbewegt: Irgendwann neues Ziel suchen. 
		iNew=Random(20);
	CreateParticle("PSpark", RandomX(-26,22), RandomX(-18,9), nil, nil, (Random(16) ?? 0)+15, RGB(200,255,255));
}


protected func ControlThrow(clonk,a,b,c,d,f)
{
        if(Active)
        {
          Active = nil;
          TurnOff();
        }
        else
        {
          Active = 1;
          TurnOn();
        }
}


public func TurnOff()
{
  var phase = GetPhase();
  SetAction("Stand");
  SetPhase(phase);
}
public func TurnOn()
{
  var phase = GetPhase();
  SetAction("Move");
  SetPhase(phase);
}

public func IsMachine() { return(1); }
public func EMPShock() { return(TurnOff()); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (!!SEqual(GetAction(), "Stand")) {
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Stand\")";
		extra[(GetLength(extra) ?? 0)] = Format("SetPhase(%d)", GetPhase());
	}
}
