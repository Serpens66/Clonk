#strict

local usage;

/* TimerCall */

private func Check()
{

 var clonk = Contained();

 if(usage > 75)
 {
  Message("Werkzeug verbraucht", clonk);
  RemoveObject();
 }

 //Hat ein lebender Clonk dieses Gerät in Händen?
 if(!(GetOCF(clonk) & OCF_Alive())) return(0);

 //Baut?
 if(GetAction(clonk) eq "Build")
 {
  var building=GetActionTarget(0,clonk);
  DoCon(3,building);
  usage++;
 }

return(1);
}

func Hit() {Sound("RockHit*");}

public func CalcValue()
{
 var basic = GetValue(0,GetID());
 return(basic - basic*usage/75);
}

// Am Amboss...

func IsAnvilProduct() {return(1);}