/* Scorpion Beam */

#strict 3
local pilot, counter;

func Initialize()
{
  counter=nil;
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

protected func Hit()
{
  Sound("BlasterFI",this());
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  BlastObjects(GetX(), GetY(), 40);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(60);
  else FakeExplode(60,nil,nil,60);
  return(1);
}

func Down()
{
  SetAction("Down");
  return(1);
}

func TravelDown()
{
  if(FindObject2(Find_Distance(5),Find_Func("IsBulletTarget")) )
  return(Hit());
  S42MigrationStep(counter, 1, true);
  if((counter ?? 0)>150) return(Hit());
  Sparks (2, RGB(220,80,180), RandomX(-5,5), RandomX(-3,3));
  SetYDir(150);
  return(1);
}