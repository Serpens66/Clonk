/*-- Schnee-Effekt --*/

#strict 3

// local frequency;
// static const SNOW_MAX_SIZE = 100, SNOW_MIN_SIZE = 10;


protected func Initialize() {
  // if(!IsNewgfx()) RemoveObject();
  SetPosition(nil,nil);
  // frequency = 10;
}

func Snowing() {  // jetzt direkt in RdW Wolkenskripten drin
  // if(GetTemperature() > 0) return(0);
  // var grav = GetGravity();
  // var speed = 25;
  // var dir = BoundBy(GetWind(),-1,1);
  // PushParticles("Snow",dir*Random(4));
  // for(var x; x < frequency; x++) {
    // var size = RandomX(SNOW_MIN_SIZE,SNOW_MAX_SIZE);
    // var xdir = BoundBy(GetWind(),-speed,speed)+RandomX(-5,5),
        // ydir = (size/2*GetGravity())/100;
    // var offset = LandscapeHeight()*2/3;
    // CreateParticle("Snow",Random(LandscapeWidth()+offset)-offset/2-offset/2*dir,0,xdir,ydir,size,0);
  // }
  return(1);
}
  
func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

