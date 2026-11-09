#strict 3




func Initialize() 
{
  SetPosition();
	var id;
	Local()=1;
	if (id=FindObject(GetID()))
	  { S42MigrationAssign(Local(), "+=", (Local(nil,id) ?? 0));  }
	SetAction("Wait");
}


func Initialized() {

//SetGamma(RGB(Local()*6,Local()*6,Local()*6),RGB(128+Local()*2,128+Local()*2,128+Local()*2),RGB(255-Local(),255-Local(),255-Local()));

if(!FindObject(NB3L)) // wenn kein "Nebelerzeuger alle x Frames", dann bewegt sich der Nebel nicht, weshalb er überall verteilt wird.
  {
var i;
while((i ?? 0)<((Local() ?? 0)*15*(Sqrt((LandscapeWidth() ?? 0)*(LandscapeHeight() ?? 0)) ?? 0)/800))
	{S42MigrationStep(i, 1, true); CreateParticle("Nebel",(Random((LandscapeWidth() ?? 0)+150) ?? 0)-(GetX() ?? 0),(Random((LandscapeHeight() ?? 0)+150) ?? 0)-(GetY() ?? 0),nil,nil,RandomX(1000,1500), RGBa(nil,nil,nil,Random(30)));}
return(1);
  }

  //Nebel auf Landschaft plazieren
  for(var i = (LandscapeWidth() ?? 0)/10; (i ?? 0) > 0; S42MigrationStep(i, -1, true))
   DoFogg((i ?? 0)*10);
}

protected func Timerr()
{
  if(!FindObject(NB3L)) return(1); // wenn kein "Nebelerzeuger alle x Frames", dann bewegt sich der Nebel nicht, muss also auch nicht neu erstellt werden
  if((GetWind(nil,nil,1) ?? 0)<0)  // wenn windrichtung nach links, dann am rechten Rand erzeugen.
  DoFogg(LandscapeWidth());
  if((GetWind(nil,nil,1) ?? 0)>0)
  DoFogg();
}

protected func DoFogg(x)
{
  //Atmosphärischen Nebel an den Kartenrändern erstellen
  var grey = 155+(Random(100) ?? 0);
 
if(And((GetWind(nil,nil,1) ?? 0)<10, (GetWind(nil,nil,1) ?? 0)>-10))  // wenn wind sehr langsam, dann deutlich seltener neue Partikel erstellen.
{if(!Random(200*10))  // lieber nochmal 2 mal langsamer, dafür dichtere nebelobjekte
  CreateParticle ("Nebel", x, Random(LandscapeHeight()), (GetWind(nil,nil,1) ?? 0)/10, nil, RandomX(1000,1500), RGBa(grey,grey,grey,Random(10))); return;}
var erstellen = 140-(Abs(GetWind(nil,nil,1)) ?? 0); // je schneller der Wind (bis maximal 100), desto mehr partikel werden erzeugt.
  if(!Random(((erstellen ?? 0)/4)*6))
  CreateParticle ("Nebel", x, Random(LandscapeHeight()), (GetWind(nil,nil,1) ?? 0)/10, nil, RandomX(1000,1500), RGBa(grey,grey,grey,Random(10)));
}



func Destruction()
{
//ResetGamma();
ClearParticles("Nebel");
}