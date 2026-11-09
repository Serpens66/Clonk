#strict
// Sichtradius Objekt fuer Teamsicht

local AktuelleSichtweite;

func Initialize() {
 return(1);
 }
 
func TeamViewRangethisObject(Range) // is called from teamviewer object to give every view object a different viewrange
{
    // Log(Format("Sicht auf %v setzen",Range));
    if(AktuelleSichtweite!=Range)
    {
        AktuelleSichtweite = Range;
        SetPlrViewRange(Range,this(),1);
    }
}

func ReturnAktuelleSicht()
{
    return(AktuelleSichtweite);
}

func AttachTo(object pObject) {
 SetAction("Shine",pObject);
 return(1);
 }
 
public func AttachTargetLost()
{
  RemoveObject();
}


func Check() {
  if(!GetActionTarget()) return(RemoveObject()); 
 if(GetAction(GetActionTarget()) eq "Dead") return(RemoveObject()); 
 } 
 