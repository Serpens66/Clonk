/*-- Blumen pflanzen--*/

#strict 

func Initialize()
{
SetPosition(1,1);
}

func SSeed()
{
 if(GetTemperature()<0)
   return(Absterben());
 if(!(GetSeason()>=12 && GetSeason()<62) || stufee4==1 || stufee5==1 || stufee3==1 || stufee2==1) // wenn weder Frühling noch Sommer, nicht pflanzen, oder wenn umwelt verschmutzt
  return(Absterben());

if(ObjectCount2(Find_ID(FLWR))<=10)  // nur 10 Stück iwo verteilt in der Landschaft platzieren, sie reproduzieren sich von selbst.
return(PlaceVegetation(FLWR, 0, 0, LandscapeWidth(), LandscapeHeight()) );
return();
}

func Absterben()
{
// im Herbst und Winter gibt es keine Blumen.. auch nicht bei Umweltverschmutzungsstufe 2,3,4 und 5
var blumme;
if(blumme=FindObject2(Find_ID(FLWR)))
RemoveObject(blumme);
return();
}