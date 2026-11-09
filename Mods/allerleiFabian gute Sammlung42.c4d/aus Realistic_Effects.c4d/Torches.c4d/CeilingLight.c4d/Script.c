/*-- Deckenlicht --*/

#strict
#include BLG3

func CreateLight() {
  pLight = AddLightAmbience(55);
  pLight->ChangeGraphics("Half");
  pLight->SetR(180);
  return(1);
}

func Construction()
{
SetPosition(GetX(),GetY()-5);
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
  mlight = AddLight(800, iColor, this());
  return(1);
}


protected func Completion() {
  //Ausrichten
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
  var x = GetX(), y = GetY() - GetObjHeight() / 2;
  
  while(! GBackSolid(0, -8) )
  {
    y -= 1;
    
    //Endlosschleife vermeiden
    if(y < 0) return(1);
    
    SetPosition(x, y);
  }
  return(1);
}