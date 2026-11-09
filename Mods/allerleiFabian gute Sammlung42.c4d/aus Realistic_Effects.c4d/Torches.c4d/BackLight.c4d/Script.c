/*-- Hintergrundlicht --*/

#strict

local pLight, bOn,mlight;
local rot, gruen, blau,r,g,b,a, intensi,nunHSL, farbt,saett,hellig,h,s,l,standr,standg,standb,standa;

func Initialize() 
{
// standardwerte für das Licht
standr=235;
standg=230;
standb=230;
standa=50;
}


public func LightSize() { return(100); }
public func Light() 
{
	return(pLight); 
}

protected func Completion() {
FitToTop();
SetCategory(C4D_StaticBack());
	CreateLight();
	TurnOn();
  SetPlrViewRange(FogViewRange(),this(),1); 
	return(1);
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(200);
}


public func FitToTop()
{
  //Das Gestell passt sich selbst mal etwas besser zur Decke an.
  var x = GetX(), y = GetY() -20;
  
    SetPosition(x, y);
  
  return(1);
}


protected func CreateLight()
{
	pLight = AddLightAmbience(LightSize());
}

private func CheckNight()
{
  if (!mlight && LightNeeded)  TurnLightOn();
  if (mlight && !LightNeeded)  TurnLightOff();
}


private func TurnLightOn()
{
if(!rot) rot=standr;
r=rot;
if(!gruen) gruen=standg;
g=gruen;
if(!blau) blau=standb;
b=blau;
if(!intensi) intensi=standa;
a=intensi;
r=BoundBy(r,0,255);
g=BoundBy(g,0,255);
b=BoundBy(b,0,255);
a=BoundBy(a,0,255);
  var iColor;
if(!nunHSL)  iColor = RGBa(r,g,b,a);
if(nunHSL) {
if(!farbt) farbt=255; 
h=farbt;
if(!saett) saett=27;
s=saett;
if(!hellig) hellig=219;
l=hellig;
h=BoundBy(h,0,255);
s=BoundBy(s,0,255);
l=BoundBy(l,0,255);
iColor = HSLa(h,s,l,a);
           }
  mlight = AddLight(1000, iColor, this());
  return(1);
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
if(!farbt) farbt=255; 
h=farbt;
if(!saett) saett=27;
s=saett;
if(!hellig) hellig=219;
l=hellig;

//auf Standard zurücksetzen, wenn alle drei RGB Werte 1000
if(iWert1==1000 && iWert2==1000 && iWert3==1000)
{nunHSL=0;rot=standr;gruen=standg;blau=standb;intensi=standa;if(mlight) {RemoveObject(mlight);TurnLightOn();}return();}

if(iWert1) {nunHSL=0;rot=iWert1;}
if(iWert2) {nunHSL=0;gruen=iWert2;}
if(iWert3) {nunHSL=0;blau=iWert3;}
// intensität wird nur hinzuaddiert
if(!intensi) {intensi=a;}
if(iWert4) {intensi+=iWert4;intensi=BoundBy(intensi,1,255);Message("Durchsichtigkeit: %v",this(),intensi);}
if(!rot) {rot=r;}
if(!gruen) {gruen=g;}
if(!blau) {blau=b;}
if(iWert5) {nunHSL=1;farbt=iWert5;} 
if(iWert6) {nunHSL=1;saett=iWert6;}
if(iWert7) {nunHSL=1;hellig=iWert7;}
if(iWert8) {nunHSL=1;farbt+=iWert8;farbt=BoundBy(farbt,1,255);Message("Farbton: %v",this(),farbt);}
if(iWert9) {nunHSL=1;saett+=iWert9;saett=BoundBy(saett,1,255);Message("Sättigung: %v",this(),saett);}
if(iWert10) {nunHSL=1;hellig+=iWert10;hellig=BoundBy(hellig,1,255);Message("Helligkeit: %v",this(),hellig);}

rot=BoundBy(rot,0,255);
gruen=BoundBy(gruen,0,255);
blau=BoundBy(blau,0,255);

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


public func TurnOn() {
	if(EMPShocked()) return();
	bOn = true;
	SetAction("On");
	if(Light())
		Light()->TurnOn();
}

public func TurnOff() {
	if(!SetAction("Off"))
		SetAction("Idle");
	bOn = false;
	if(Light())
		Light()->TurnOff();
}

public func Switch() {
	if(GetAction() S= "On")
		TurnOff();
	else
		TurnOn();
}

public func IsLamp() { return(true); }
public func IsMachine() { return(true); }

public func EMPShock()
{
	TurnOff();
	EMPShockEffect(800+Random(200));
}

public func EMPShockEnd() {
	if(bOn) {
		SetAction("On");
		if(Light()) Light()->TurnOn();
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() ne "On")
		extra[GetLength(extra)] = "TurnOff()";
}
