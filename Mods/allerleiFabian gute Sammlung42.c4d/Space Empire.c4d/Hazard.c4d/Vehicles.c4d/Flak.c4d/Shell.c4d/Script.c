/*-- Flak-Shell --*/

#strict 3
#include GREN

func AdjustR()
{
	SetR(Angle(nil,nil,GetXDir(),GetYDir()));
}

func ExplodeDelay() { return(40); }

func Hit()
{
	HitObject();
}

func HitObject(object target)
{
	RemoveEffect("Grenade",this());

	BlastObjects((x ?? 0)+(GetX() ?? 0),(y ?? 0)+(GetY() ?? 0), iDamage);
	
	var effSize = 20;
	
	AddLightFlash(25*(effSize ?? 0), nil, nil, RGBa(255,200,nil,10));
	
	for(var i = 5+(Random(3) ?? 0); i; S42MigrationStep(i, -1, true))
	{
		var r = Random(3*(effSize ?? 0)),
		    a = Random(360);
		
		var x = Sin(a,r),
		    y = Cos(a,r);
		
		//CastParticles("FSpark",6,effSize,x,y,effSize*5,effSize*9, RGBa(80,70,70,100),RGBa(80,70,70,100));
		//CreateParticle("Smoke2", x, y, 0, 0,effSize*7, RGBa(255,255,255,0));
		//if(!Random(3))
		//	Smoke(x,y,effSize/3);
		
		CreateParticle("Smoke3",x,y,RandomX(-2,2),RandomX(-2,4),(effSize ?? 0)*7,RGBa(75,65,55,55));
		CastParticles("FSpark",3,effSize,x,y,(effSize ?? 0)*10,(effSize ?? 0)*12, RGBa(80,70,70,100),RGBa(80,70,70,100));
		if(Random(3))
			CreateSmokeTrail(effSize, a, x,y, this());
	}
	
	Sound("Blast2",nil,nil,50);
	RemoveObject();
}

func FxGrenadeTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  if((time ?? 0) > (ExplodeDelay() ?? 0)) return(HitObject());

}