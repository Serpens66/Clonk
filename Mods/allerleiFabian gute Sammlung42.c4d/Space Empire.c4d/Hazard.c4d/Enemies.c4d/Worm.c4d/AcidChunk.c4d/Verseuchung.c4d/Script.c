/*-- Verseuchung --*/

#strict 3

local fade;

func Initialize()
{
  SetAction("Be");
  SetPhase(Random(6));
}

func Check()
{
	for(var enemy in FindTargets(this(),40)) {
		if(enemy->GetAlive()) {
			var abzug = (fade ?? 0)/50;
			if(!Random(8))DoDmg(Max(nil,4-(abzug ?? 0)),DMG_Bio,enemy);
		}
	}
	
	if((GetActTime() ?? 0) > 200)
		DoFade(5);
}

func OnHit(int dmg, int type, object from)
{
	dmg ??= 0;
	type ??= 0;
	if(Equal(type, DMG_Fire))
		DoFade(5);
	if(Equal(type, DMG_Explosion))
		DoFade(25);
}

func DoFade(int change)
{
	change ??= 0;
	S42MigrationAssign(fade, "+=", (change ?? 0));
	
	if((fade ?? 0) > 250)
		RemoveObject();
	
	SetClrModulation(RGBa(255,255,255,fade));
}

public func IsBulletTarget(id ID)  // nur von feuer zerstörbar
{
	if(Or(Equal(ID, FFLM), Equal(ID, FLMB)))
		return(true);
	return(nil);
}
