#strict 3

local usage;

/* TimerCall */

private func Check()
{

 var clonk = Contained();

 if((usage ?? 0) > 75)
 {
  Message("Werkzeug verbraucht", clonk);
  RemoveObject();
 }

 //Hat ein lebender Clonk dieses Gerät in Händen?
 if(!((GetOCF(clonk) ?? 0) & (OCF_Alive ?? 0))) return(nil);

 //Baut?
 if(!!SEqual(GetAction(clonk), "Build"))
 {
  var building=GetActionTarget(nil,clonk);
  DoCon(3,building);
  S42MigrationStep(usage, 1, true);
 }

return(1);
}

func Hit() {Sound("RockHit*");}

public func CalcValue()
{
 var basic = GetValue(nil,GetID());
 return((basic ?? 0) - (basic ?? 0)*(usage ?? 0)/75);
}

// Am Amboss...

func IsAnvilProduct() {return(1);}