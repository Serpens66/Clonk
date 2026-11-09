#strict




func Initialize() 
{
  SetPosition();
	var id;
	Local()=1;
	if (id=FindObject(GetID()))
	  { Local()+=Local(0,id);  }
	SetAction("Wait");
}


func Initialized() {

//SetGamma(RGB(Local()*6,Local()*6,Local()*6),RGB(128+Local()*2,128+Local()*2,128+Local()*2),RGB(255-Local(),255-Local(),255-Local()));

if(!FindObject(NB3L)) // wenn kein "Nebelerzeuger alle x Frames", dann bewegt sich der Nebel nicht, weshalb er überall verteilt wird.
  {
var i;
while(i<(Local()*15*Sqrt(LandscapeWidth()*LandscapeHeight())/800)) 
	{i++; CreateParticle("Nebel",Random(LandscapeWidth()+150)-GetX(),Random(LandscapeHeight()+150)-GetY(),0,0,RandomX(1000,1500), RGBa(0,0,0,Random(30)));}
return(1);
  }

  //Nebel auf Landschaft plazieren
  for(var i = LandscapeWidth()/10; i > 0; i--)
   DoFogg(i*10);
}

protected func Timerr()
{
  if(!FindObject(NB3L)) return(1); // wenn kein "Nebelerzeuger alle x Frames", dann bewegt sich der Nebel nicht, muss also auch nicht neu erstellt werden
  if(GetWind(0,0,1)<0)  // wenn windrichtung nach links, dann am rechten Rand erzeugen.
  DoFogg(LandscapeWidth());
  if(GetWind(0,0,1)>0)
  DoFogg();
}

protected func DoFogg(x)
{
  //Atmosphärischen Nebel an den Kartenrändern erstellen
  var grey = 155+Random(100);
 
if(GetWind(0,0,1)<10 && GetWind(0,0,1)>-10)  // wenn wind sehr langsam, dann deutlich seltener neue Partikel erstellen.
{if(!Random(200*10))  // lieber nochmal 2 mal langsamer, dafür dichtere nebelobjekte
  CreateParticle ("Nebel", x, Random(LandscapeHeight()), GetWind(0,0,1)/10, 0, RandomX(1000,1500), RGBa(grey,grey,grey,Random(10))); return();}
var erstellen = 140-Abs(GetWind(0,0,1)); // je schneller der Wind (bis maximal 100), desto mehr partikel werden erzeugt.
  if(!Random((erstellen/4)*6))
  CreateParticle ("Nebel", x, Random(LandscapeHeight()), GetWind(0,0,1)/10, 0, RandomX(1000,1500), RGBa(grey,grey,grey,Random(10))); 
}



func Destruction()
{
//ResetGamma();
ClearParticles("Nebel");
}