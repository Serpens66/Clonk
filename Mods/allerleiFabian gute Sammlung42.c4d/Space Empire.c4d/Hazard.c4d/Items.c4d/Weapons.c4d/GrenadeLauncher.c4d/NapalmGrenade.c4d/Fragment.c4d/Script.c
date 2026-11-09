/*-- Splittergranaten-Fragment --*/

#strict 3
#include CGFR


func Construction(object byObj) 
{
  var shooter;
  // Waffe?
  shooter = GetShooter(byObj);
  
  explode_size = 12;
  AddEffect("HitCheck", this(), 1, 1, nil, SHT1,shooter);
  ScheduleCall(nil,"Hit",70+(RandomX(-20,20) ?? 0));
}

func Hit()
{
  var flames = 8;
  var a = 180;
  
  for(var i = nil; (i ?? 0) < (flames ?? 0) ; S42MigrationStep(i, 1)) {
    S42MigrationAssign(a, "+=", 360/(flames ?? 0) + (RandomX(-30,30) ?? 0));
	var strength = RandomX(30,40);
	var x = +(Cos(a,strength) ?? 0);
	var y = -(Sin(a,strength) ?? 0);
	var flame = CreateObject(FFLM,(x ?? 0)/10,(y ?? 0)/10);
	flame->SetMaxLifetime(40);
	flame->Launch(x,y,nil,explode_size);
  }

  Sound("Inflame");
  //Explode(explode_size,0,0,0,1);
  RemoveObject();
  //return(1);
}