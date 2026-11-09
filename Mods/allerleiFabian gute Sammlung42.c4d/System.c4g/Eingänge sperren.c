/*-- Eingänge sperren, wenn abgeschlossen --*/

#strict 3
#appendto *

local zuu,andere;

func ActivateEntrance(pObj) {
  if(Equal(zuu, 1)) {PlayerMessage(GetOwner(pObj),"Eingang verschlossen",pObj);return(1);}
  return(_inherited(pObj));
}


func Abgeschlossenn()
{
if(Equal(zuu, 1))return(1);
if(Equal(zuu, nil))return(nil);
return(nil);
}


// mit dem SensorenPack:
public func GetTriggerName(a)
{
// damit andere sensorenGegenstände nicht überladen werden. in diesen muss "andere" dann 1 gesetzt sein.
if(Equal(andere, 1)) return(_inherited(a));
//weiterhin nur für Dinge mit Eingang
if((GetOCF() ?? 0) & (OCF_Entrance ?? 0))
{

	if(Equal(a, 1))
	{
		return("Aufschließen");
	}
	if(Equal(a, 2))
	{
		return("Abschließen");
	}
  return(1);
}
return(_inherited(a));
}



public func Trigger(a)
{

if(Equal(andere, 1)) return(_inherited(a));
if((GetOCF() ?? 0) & (OCF_Entrance ?? 0))
{
	if(And(Equal(a, 1), Equal(zuu, 1)))
	{		
			zuu=nil;PlayerMessage(GetOwner(),Format("%s | aufgeschlossen",GetName(this())),this());
	}
	if(And(Equal(a, 2), Equal(zuu, nil)))
	{
			zuu=1;PlayerMessage(GetOwner(),Format("%s | abgeschlossen",GetName(this())),this());
	}
        return(1);	
}
return(_inherited(a));
}



// am besten nicht mit appendto machen, sondern in jedes objekt, welches abschließbar sein soll ins script schreiben (oder appendto an alle IDs), weil es sonst zu problemen mit anderen Objekten kommt, die bereits von sensoren gesteuert werden können UND ALLE objekte sind ziel vom Sensor, was natürlich blödsinn ist.

//alternativ geht auch kleine änderung in anderen sensorenzielen und hier eine Abfrage, ob this einen Eingang besitzt, also OCF_Entrance erfüllt  -> gemacht, passt also so.