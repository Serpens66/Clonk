/* Scorpion Beam */

#strict
local pilot, counter;

func Initialize()
{
  counter=0;
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
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
  else FakeExplode(60,0,0,60);
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
  counter++;
  if(counter>150) return(Hit());
  Sparks (2, RGB(220,80,180), RandomX(-5,5), RandomX(-3,3));
  SetYDir(150);
  return(1);
}