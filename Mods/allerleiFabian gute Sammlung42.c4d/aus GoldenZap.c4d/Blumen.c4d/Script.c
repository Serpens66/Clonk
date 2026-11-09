/*-- Blumen pflanzen--*/

#strict 3

func Initialize()
{
SetPosition(1,1);
}

func SSeed()
{
 if((GetTemperature() ?? 0)<0)
   return(Absterben());
 if(Or(Or(Or(Or(!(And((GetSeason() ?? 0)>=12, (GetSeason() ?? 0)<62)), Equal(stufee4, 1)), Equal(stufee5, 1)), Equal(stufee3, 1)), Equal(stufee2, 1))) // wenn weder Frühling noch Sommer, nicht pflanzen, oder wenn umwelt verschmutzt
  return(Absterben());

if((ObjectCount2(Find_ID(FLWR)) ?? 0)<=10)  // nur 10 Stück iwo verteilt in der Landschaft platzieren, sie reproduzieren sich von selbst.
return(PlaceVegetation(FLWR, nil, nil, LandscapeWidth(), LandscapeHeight()) );
return;
}

func Absterben()
{
// im Herbst und Winter gibt es keine Blumen.. auch nicht bei Umweltverschmutzungsstufe 2,3,4 und 5
var blumme;
if(blumme=FindObject2(Find_ID(FLWR)))
RemoveObject(blumme);
return;
}