/*-- Gebäudemanager --*/

#strict 3

local target, light, symbol;    // Objektpointer
local lx, ly, lSize, lColor;    // Licht: Vertices, Größe, Farbe
local energycheck;              // Energie überprüfen?
// für Farbänderung der Lichter übers contextmenü
local rot, gruen, blau,r,g,b,a, intensi,nunHSL, farbt,saett,hellig,h,s,l,standr,standg,standb,standa;



/* Initialisierung */

public func Init(par_target, nrg_check)
{
  // evlt besser fuer nrg_check ->~IsEnergyConsumer() aufrufen?
  // Aufruf vom Zielobjekt
  target = par_target;
  SetAction("Attach", target);
  energycheck = nrg_check;
  return(1);
}

public func InitLight(lightX, lightY, lightsize, lightcolor)
{
  // Licht aktivieren
  lx = lightX;
  ly = lightY;
  lSize = lightsize;
  lColor = lightcolor;
// standardwerte für das Licht
standr=240;
standg=240;
standb=255;
standa=55;
  return(1);
}


protected func Destruction()
{
  LightOff();
  SymbolOff();
  return(1);
}


/* Timer-Call */

private func Check()
{
  // Spielregel Energieverbrauch
  if (!FindObject(ENRG))
    DoEnergy(1000, target);
  // EMP
  var emp = HasEMP(target);
  if (emp) DoEnergy(-100, target);
  // Energieüberprüfung
  if (energycheck)
  {
    var needed = !(target->EnoughEnergy());
    if (And(!symbol, needed))  SymbolOn();
    if (And(symbol, !needed))  SymbolOff();
  }
  // Licht ein/aus, wenn nötig
  if (Or(symbol, emp))
    return(LightOff());
  if (And(And(And(!light, LightNeeded), !symbol), !emp))  LightOn();
  if (And(light, !LightNeeded))  LightOff();
  return(1);
}


/* Licht */

private func LightOn()
{
  // Gebäude hat kein Licht
  if (And(!lx, !ly)) return(nil);

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
if(!farbt) farbt=160; 
h=farbt;
if(!saett) saett=240;
s=saett;
if(!hellig) hellig=233;
l=hellig;
h=BoundBy(h,nil,255);
s=BoundBy(s,nil,255);
l=BoundBy(l,nil,255);
iColor = HSLa(h,s,l,a);
           }
  light = AddLight(700, iColor, this());
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
if(!farbt) farbt=160; 
h=farbt;
if(!saett) saett=240;
s=saett;
if(!hellig) hellig=233;
l=hellig;

//auf Standard zurücksetzen, wenn alle drei RGB Werte 1000
if(And(And(Equal(iWert1, 1000), Equal(iWert2, 1000)), Equal(iWert3, 1000)))
{nunHSL=nil;rot=standr;gruen=standg;blau=standb;intensi=standa;if(light) {RemoveObject(light);LightOn();}return;}

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
if(light) {RemoveObject(light);LightOn();}
}

// zum verstellen des Lichts (farbe und intensität)
func IsLightObject() {return(1);}



private func LightOff()
{
  if (light) RemoveObject(light);
  return(1);
}


/* Symbol */

private func SymbolOn()
{
  symbol = CreateObject(SNOE);
  symbol->SetTarget(target);
  return(1);
}

private func SymbolOff()
{
  if (symbol) RemoveObject(symbol);
  return(1);
}

