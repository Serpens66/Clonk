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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
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
  counter++;
  if(counter>100) Hit();
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
  counter++;
  if(counter>100) Hit();
  Sparks (2, RGB(220,80,180), RandomX(-5,5), RandomX(-3,3));
  SetYDir(150);
  return(1);
}