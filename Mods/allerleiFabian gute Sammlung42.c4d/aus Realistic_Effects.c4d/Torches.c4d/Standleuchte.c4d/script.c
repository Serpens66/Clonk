/*-- Standlampe --*/

#strict 3


local mlight, basement;
local rot, gruen, blau,r,g,b,a, intensi,nunHSL, farbt,saett,hellig,h,s,l,standr,standg,standb,standa;

func Initialize() 
{
  var iF = 170,
      iS = 240,
      iH = 240;
  SetColorDw(HSL(iF, iS, iH));

// standardwerte für das Licht
standr=235;
standg=180;
standb=60;
standa=20;
}

protected func Construction()
{
  basement = CreateObject(BS06,nil,+8,GetOwner());
}


public func Entrance()
{
  TurnLightOff();
}

public func Departure()
{
  if (And(!mlight, LightNeeded))  TurnLightOn();
}

private func CheckNight()
{
  if (And(!mlight, LightNeeded))  {TurnLightOn();SetAction("BURN");}
  if (And(mlight, !LightNeeded))  {TurnLightOff();SetAction("Idle");}
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
  mlight = AddLight(1400, iColor, this());
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


// muss, wenn es mit strom laufen soll, noch geändert werden, sodass nur Sicht, wenn auch angeschaltet
public func Completion()
{
  SetPlrViewRange(FogViewRange()); 
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(200); 
}

protected func Destruction()
{
  if (basement) RemoveObject(basement); 
  return(_inherited());
}
