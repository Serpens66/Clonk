#strict

local color1,color2,color3,target1,target2,target3,speed,count,ramp;


//Die Funktion zum Aufrufen
global func NewGamma(clr1,clr2,clr3,rampindex,spd) {
  var fader=FindFader(rampindex);
  if(!fader) fader=CreateFader(rampindex);
  fader->_GVC::StartFading(clr1,clr2,clr3,spd);
  return(1);} 

//nun auch mˆglich, da die Farbwerte eh alle gespeichert werden
global func GetGamma(select,rampindex) {
  var fader=FindFader(rampindex);
  if(!fader) {
	if(select==0) return(0);
	if(select==1) return(8355711);
	if(select==2) return(1677721);}
  return(fader->_GVC::GetFading(select));} 

//¸berladene Funktionen, damit der Gammawert gespeichert werden kann
global func SetGamma(clr1,clr2,clr3,rampindex) {
  var fader=FindFader(rampindex);
  if(!fader) fader=CreateFader(rampindex);
  fader->_GVC::SaveFading(clr1,clr2,clr3);
  return(_inherited(clr1,clr2,clr3,rampindex));}

global func ResetGamma(rampindex) {
  var fader=FindFader(rampindex);
  if(!fader) fader=CreateFader(rampindex);
  fader->_GVC::SaveFading(0,8355711,1677721);
  return(_inherited(rampindex));}


//Wird gespeichert, damit er nicht beim ersten Faden das ziel sofort erreicht
protected func Initialize() {SaveFading(0,8355711,1677721);}

//F‰ngt an zu faden
public func StartFading(clr1,clr2,clr3,spd) {
  color1=target1;
  color2=target2;
  color3=target3;
  target1=BoundBy(clr1,0,16777215);
  target2=BoundBy(clr2,0,16777215);
  target3=BoundBy(clr3,0,16777215);
  speed=BoundBy(spd,1,25500);
  }

//Speichert die Gammawerte (und stoppt den Vorgang)
public func SaveFading(clr1,clr2,clr3) {
  color1=BoundBy(clr1,0,16777215);
  color2=BoundBy(clr2,0,16777215);
  color3=BoundBy(clr3,0,16777215);
  target1=BoundBy(clr1,0,16777215);
  target2=BoundBy(clr2,0,16777215);
  target3=BoundBy(clr3,0,16777215);}

public func GetFading(select) {return(LocalN(Format("color%d",select+1)));}

//Timer
protected func GmVal() {
  if(color1==target1) if(color2==target2) if(color3==target3) return();
  if(--count>0) return();
  SetGamma(FadeRGB(0),FadeRGB(1),FadeRGB(2),ramp);
  count=100/speed;
  }


//Fadet das Gamma eines RGBs (is ne Zusammenfassung, quasi), auﬂerdem ‰ndert es den Gammawert
private func FadeRGB(dml) { return(LocalN(Format("color%d",dml+1))=RGB(Fade(0,dml),Fade(1,dml),Fade(2,dml))); }

//Fadet das Gamma (einzelne Farbwerte)
private func Fade(rgb,dml) {return(BoundBy(GetRGBColor(rgb,dml)+FadeDistance(rgb,dml),0,255));}

//einfach nur ne Formel, die einen Wert zwischen -3 und +3 berechnet
private func FadeDistance(rgb,dml)  {return(BoundBy(GetRGBTarget(rgb,dml)-GetRGBColor(rgb,dml),-1,1)*(BoundBy(speed,100,25500)/100)*Sqrt(Sqrt(Abs(GetRGBColor(rgb,dml)-GetRGBTarget(rgb,dml)))));}

//Gibt den aktuellen Farbwert (rgb) des jeweiligen Channels aus (dml)
private func GetRGBColor(rgb,dml) {return(GetRGBValue(LocalN(Format("color%d",dml+1)),rgb+1));}

//Gibt den zu erstrebenden Farbwert (rgb) des jeweiligen Channels aus (dml)
private func GetRGBTarget(rgb,dml) {return(GetRGBValue(LocalN(Format("target%d",dml+1)),rgb+1));}


//Eine Funktion, um einen Fader des angegebenen rampindex zu finden
global func FindFader(rampindex) {
  var fader;
  while(fader=FindObject(_GVC,0,0,0,0,0,0,0,0,fader))
    if(LocalN("ramp",fader)==rampindex) return(fader);}

//Eine Funktion, um einen Fader des angegebenen rampindex zu erstellen
global func CreateFader(rampindex) {
  var fader=CreateObject(_GVC,AbsX(50),AbsY(50),-1);
  LocalN("ramp",fader)=rampindex;
  return(fader);}