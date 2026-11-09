/*-- Splittergranaten-Fragment --*/

#strict 3

local explode_size;

func Construction(object byObj) 
{
  var shooter;
  // Waffe?
  shooter = GetShooter(byObj);
  
  explode_size = 12;
  AddEffect("HitCheck", this(), 1, 1, nil, SHT1,shooter);
  ScheduleCall(nil,"Hit",30+(RandomX(-20,20) ?? 0));
}

public func SetExplodeSize(int iExplodeSize) { iExplodeSize ??= 0;
    explode_size = iExplodeSize; }

func Hit()
{
  BlastObjects(nil,nil, (explode_size ?? 0)/2, Contained());
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(explode_size,nil,nil,nil,1);
 else FakeExplode(explode_size,nil,nil,explode_size);
}
func HitObject() { Hit(); }

func GrenadeSmoke()
{
  var iX = -(GetXDir() ?? 0)/6;
  var iY = -(GetYDir() ?? 0)/6;
  var iSize = 10+(RandomX(120,200) ?? 0);

  CreateParticle("Smoke2",iX,iY,RandomX(-4,4),RandomX(-3,5),iSize,RGBa(75,65,55,55));
  if(!Random(4))  
    CreateParticle("Blast",iX,iY,nil,nil,(iSize ?? 0)*2/3,RGBa(250,100+(Random(100) ?? 0),100,160));

}
