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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  AddSpawnEffect(pilot,RGBa(220,80,180,100));
  RemoveObject();
  return(pilot);
}


func Up()
{
  SetAction("Up");
  return(pilot);
}

func TravelUp()
{
  S42MigrationStep(counter, 1, true);
  if((counter ?? 0)>100) Hit();
  Sparks (2, RGB(220,80,180), RandomX(-5,5), RandomX(-3,3));
  SetYDir(-60);
  return(1);
}

func Down()
{
  SetAction("Down");
  return(1);
}

func TravelDown()
{
  S42MigrationStep(counter, 1, true);
  if((counter ?? 0)>100) Hit();
  Sparks (2, RGB(220,80,180), RandomX(-5,5), RandomX(-3,3));
  SetYDir(150);
  return(1);
}