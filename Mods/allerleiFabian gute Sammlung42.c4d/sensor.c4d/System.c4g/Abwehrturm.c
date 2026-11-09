/*-- Gebäude mit Geschützen --*/

#strict 3
#appendto CDFT //Abwehrturm

//dadurch können die Waffengeschütze auf den Gebäuden per Sensoren gesteuert werden.


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles, geht darum, dass in diesen Gebäude nicht nur auf und abschließen sein soll, sondern auch anderes
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

// Für die Sensoren die Waffenwechselfunktionen:
public func SensorenenWaffenwechsel(zahl)
{
if(Equal(zahl, 1)) {SelectWeapon(SYNW);return(1);} //keine Waffe
if(And(Equal(zahl, 2), GetPlrKnowledge(GetOwner(),CMG1))) {SelectWeapon(CMG1);return(1);} //Minigun-Geschütz
if(And(Equal(zahl, 3), GetPlrKnowledge(GetOwner(),CPG1))) {SelectWeapon(CPG1);return(1);}  //Plasma-Geschütz
if(And(Equal(zahl, 4), GetPlrKnowledge(GetOwner(),CPG2))) {SelectWeapon(CPG2);return(1);}  //Twin-Plasma-Geschütz
PlayerMessage(GetOwner(),Format("Für Geschütz auf %s fehlt die Forschung",this()),this());return(1);
}

//Target

public func GetTriggerName(a)
{

	if(Equal(a, 1))
	{
		return("Abschließen");
	}

	if(Equal(a, 2))
	{
		return("Aufschließen");
	}

	if(Equal(a, 3))
	{
		return("kein Waffengeschütz");
	}

	if(Equal(a, 4))
	{
		return("Minigun-Geschütz");
	}

	if(Equal(a, 5))
	{
		return("Plasma-Geschütz");
	}

        if(Equal(a, 6))
	{
	        return("Twin-Plasma-Geschütz");
        }

  return(1);
}

public func Trigger(a)
{
	if(Equal(a, 1))
	{
			zuu=1;
	}
	if(Equal(a, 2))
	{
			zuu=nil;
	}
	if(Equal(a, 3))
	{
		SensorenenWaffenwechsel(1);
	}
	if(Equal(a, 4))
	{
		SensorenenWaffenwechsel(2);
	}
	if(Equal(a, 5))
	{
		SensorenenWaffenwechsel(3);
	}
	if(Equal(a, 6))
	{
		SensorenenWaffenwechsel(4);
	}
        return(1);	
}
