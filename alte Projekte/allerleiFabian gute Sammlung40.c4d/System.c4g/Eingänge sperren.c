/*-- Eingänge sperren, wenn abgeschlossen --*/

#strict
#appendto *

local zuu,andere;

func ActivateEntrance(pObj) {
  if(zuu==1) {PlayerMessage(GetOwner(pObj),"Eingang verschlossen",pObj);return(1);}
  return(_inherited(pObj));
}


func Abgeschlossenn()
{
if(zuu==1)return(1);
if(zuu==0)return(0);
return(0);
}


// mit dem SensorenPack:
public func GetTriggerName(a)
{
// damit andere sensorenGegenstände nicht überladen werden. in diesen muss "andere" dann 1 gesetzt sein.
if(andere==1) return(_inherited(a));
//weiterhin nur für Dinge mit Eingang
if(GetOCF() & OCF_Entrance())
{

	if(a==1)
	{
		return("Aufschließen");
	}
	if(a==2)
	{
		return("Abschließen");
	}
  return(1);
}
return(_inherited(a));
}



public func Trigger(a)
{

if(andere==1) return(_inherited(a));
if(GetOCF() & OCF_Entrance())
{
	if(a==1 && zuu==1)
	{		
			zuu=0;PlayerMessage(GetOwner(),Format("%s | aufgeschlossen",GetName(this())),this());
	}
	if(a==2 && zuu==0)
	{
			zuu=1;PlayerMessage(GetOwner(),Format("%s | abgeschlossen",GetName(this())),this());
	}
        return(1);	
}
return(_inherited(a));
}



// am besten nicht mit appendto machen, sondern in jedes objekt, welches abschließbar sein soll ins script schreiben (oder appendto an alle IDs), weil es sonst zu problemen mit anderen Objekten kommt, die bereits von sensoren gesteuert werden können UND ALLE objekte sind ziel vom Sensor, was natürlich blödsinn ist.

//alternativ geht auch kleine änderung in anderen sensorenzielen und hier eine Abfrage, ob this einen Eingang besitzt, also OCF_Entrance erfüllt  -> gemacht, passt also so.