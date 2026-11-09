/*-- Scheinwerfer --*/

#strict 3

#include BOB1 // Gebäude-Hilfsobjekt

local mlight, pLight, basement, manager;
local rot, gruen, blau,r,g,b,a, intensi,nunHSL, farbt,saett,hellig,h,s,l,standr,standg,standb,standa;


protected func Initialize()
{
if ((GetEnergy() ?? 0) > 30)
{  pLight = CreateObject(FLHH, nil,nil, GetOwner());
	pLight -> Activate(this());}
  manager=CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
// standardwerte für das Licht
standr=235;
standg=180;
standb=60;
standa=30;
  return(1);
}





public func SwitchLights() {
	if (pLight ) {RemoveObject(pLight);TurnLightOff();}
//if (FindObject(LGC2)) {RemoveObject(FindObject(LGC2));}
        else { Energy();};
}

func Energy()
{
if ((GetEnergy() ?? 0) > 30)
{ DoEnergy(-5); Log("energie da"); Sound("Click");  pLight = CreateObject(FLHH, nil,nil, GetOwner());
	pLight -> Activate(this());}
 else   {Log("keine energie da");Sound("Error");};
return(1);
}

func Energyweg()
{
if (pLight) {Log("keine energie da");RemoveObject(pLight);}
}




private func CheckNight()
{
  if (And(!mlight, LightNeeded))
  TurnLightOn();
  if (And(mlight, !LightNeeded))
  TurnLightOff();
}

private func TurnLightOn()
{
if(pLight) 
 {
if(!rot) rot=standr;
r=rot;
if(!gruen) gruen=standg;
g=gruen;
if(!blau) blau=standb;
b=blau;
if(!intensi) intensi=standa;
a=intensi;
r=BoundBy(r,nil,255);
g=BoundBy(g,nil,255);
b=BoundBy(b,nil,255);
a=BoundBy(a,nil,255);
  var iColor;
if(!nunHSL)  iColor = RGBa(r,g,b,a);
if(nunHSL) {
if(!farbt) farbt=27; 
h=farbt;
if(!saett) saett=195;
s=saett;
if(!hellig) hellig=39;
l=hellig;
h=BoundBy(h,nil,255);
s=BoundBy(s,nil,255);
l=BoundBy(l,nil,255);
iColor = HSLa(h,s,l,a);
           }
  mlight = AddLight(2200, iColor, this());
  return(1);
 }
}

//farbt,saett,hellig

func LichtAnpassen(iWert1,iWert2,iWert3,iWert4,iWert5,iWert6,iWert7,iWert8,iWert9,iWert10)
{
//Standardwerte hier nochmal
if(!rot) rot=standr;
r=rot;
if(!gruen) gruen=standg;
g=gruen;
if(!blau) blau=standb;
b=blau;
if(!intensi) intensi=standa;
a=intensi;
// die Standardfarbe in HSL
if(!farbt) farbt=27; 
h=farbt;
if(!saett) saett=195;
s=saett;
if(!hellig) hellig=39;
l=hellig;

//auf Standard zurücksetzen, wenn alle drei RGB Werte 1000
if(And(And(Equal(iWert1, 1000), Equal(iWert2, 1000)), Equal(iWert3, 1000)))
{nunHSL=nil;rot=standr;gruen=standg;blau=standb;intensi=standa;if(mlight) {RemoveObject(mlight);TurnLightOn();}return;}

if(iWert1) {nunHSL=nil;rot=iWert1;}
if(iWert2) {nunHSL=nil;gruen=iWert2;}
if(iWert3) {nunHSL=nil;blau=iWert3;}
// intensität wird nur hinzuaddiert
if(!intensi) {intensi=a;}
if(iWert4) {S42MigrationAssign(intensi, "+=", (iWert4 ?? 0));intensi=BoundBy(intensi,1,255);Message("Durchsichtigkeit: %v",this(),intensi);}
if(!rot) {rot=r;}
if(!gruen) {gruen=g;}
if(!blau) {blau=b;}
if(iWert5) {nunHSL=1;farbt=iWert5;} 
if(iWert6) {nunHSL=1;saett=iWert6;}
if(iWert7) {nunHSL=1;hellig=iWert7;}
if(iWert8) {nunHSL=1;S42MigrationAssign(farbt, "+=", (iWert8 ?? 0));farbt=BoundBy(farbt,1,255);Message("Farbton: %v",this(),farbt);}
if(iWert9) {nunHSL=1;S42MigrationAssign(saett, "+=", (iWert9 ?? 0));saett=BoundBy(saett,1,255);Message("Sättigung: %v",this(),saett);}
if(iWert10) {nunHSL=1;S42MigrationAssign(hellig, "+=", (iWert10 ?? 0));hellig=BoundBy(hellig,1,255);Message("Helligkeit: %v",this(),hellig);}

rot=BoundBy(rot,nil,255);
gruen=BoundBy(gruen,nil,255);
blau=BoundBy(blau,nil,255);

//damit es aktualisiert wird:
if(mlight) {RemoveObject(mlight);TurnLightOn();}
}

// zum verstellen des Lichts (farbe und intensität)
func IsLightObject() {return(1);}


private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}







protected func Construction()
{
  basement = CreateObject(BS06,nil,+8,GetOwner());
}

protected func ControlLeft()
{
  if(pLight) pLight->TurnLeft();
}

protected func ControlRight()
{
  if(pLight) pLight->TurnRight();
}

protected func ControlUp()
{
  if(pLight) pLight->Stop();
}

public func ControlDigDouble()
{
SwitchLights();
}

protected func ControlDown()
{
ShowInfoBar(IBRE,this(),GetEnergy(),100,10,1);
}



// Scriptaufruf

public func SetRotation(int iR, int iOwn)
{
  iR ??= 0;
  iOwn ??= 0;
  iR = BoundBy(iR, -90, 90);
  SetR(iOwn);

  if(pLight) pLight->SetR((iR ?? 0)+(GetR() ?? 0));
  if(pLight) pLight->RotateLight();
}

// Kein Kran darf mich schnappen!
public func NoCrane() { return(1); }


public func Completion()
{
  SetPlrViewRange(FogViewRange(),this(),1); 
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(50); 
}

protected func Destruction()
{
PreDestruction();
if (pLight ) {RemoveObject(pLight);}
  if (basement) RemoveObject(basement); 
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  return(inherited());
}



/* Energieüberprüfung */

public func CheckEnergy()
{
  // Energieverbrauch
  if (Or(!FindObject(ENRG), FindObject(NELI))) DoEnergy(100);
if (pLight ) DoEnergy(-50);
if ((GetEnergy() ?? 0) < 50) {Energyweg();};
  return(1);
}








public func EnoughEnergy() { return((GetEnergy() ?? 0) >= 5); }
public func IsEnergyConsumer() { return(1); }
