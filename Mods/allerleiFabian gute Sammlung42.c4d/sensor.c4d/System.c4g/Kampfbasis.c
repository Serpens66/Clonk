/*-- Gebäude mit Geschützen --*/

#strict 3
#appendto CBBS //Kampfbasis

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
if(Equal(zahl, 1)) {ChangeWeapon(SYNW,nil);return(1);} //Links: keine Waffe
if(And(Equal(zahl, 2), GetPlrKnowledge(GetOwner(),CMG1))) {ChangeWeapon(CMG1,nil);return(1);}  //Links: Minigun-Geschütz
if(And(Equal(zahl, 3), GetPlrKnowledge(GetOwner(),CPG1))) {ChangeWeapon(CPG1,nil);return(1);}  //Links: Plasma-Geschütz
if(And(Equal(zahl, 4), GetPlrKnowledge(GetOwner(),CPG2))) {ChangeWeapon(CPG2,nil);return(1);}  //Links: Twin-Plasma-Geschütz
if(Equal(zahl, 5)) {ChangeWeapon(SYNW,1);return(1);}  //Rechts: keine Waffe
if(And(Equal(zahl, 6), GetPlrKnowledge(GetOwner(),CMG1))) {ChangeWeapon(CMG1,1);return(1);}  //Rechts: Minigun-Geschütz
if(And(Equal(zahl, 7), GetPlrKnowledge(GetOwner(),CPG1))) {ChangeWeapon(CPG1,1);return(1);}  //Rechts: Plasma-Geschütz
if(And(Equal(zahl, 8), GetPlrKnowledge(GetOwner(),CPG2))) {ChangeWeapon(CPG2,1);return(1);}  //Rechts: Twin-Plasma-Geschütz
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
		return("Links: kein Waffengeschütz");
	}

	if(Equal(a, 4))
	{
		return("Links: Minigun-Geschütz");
	}

	if(Equal(a, 5))
	{
		return("Links: Plasma-Geschütz");
	}

        if(Equal(a, 6))
	{
	        return("Links: Twin-Plasma-Geschütz");
        }

	if(Equal(a, 7))
	{
		return("Rechts: kein Waffengeschütz");
	}

        if(Equal(a, 8))
	{
		return("Rechts: Minigun-Geschütz");
	}

	if(Equal(a, 9))
	{
		return("Rechts: Plasma-Geschütz");
	}

	if(Equal(a, 10))
	{
		return("Rechts: Twin-Plasma-Geschütz");
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
	if(Equal(a, 7))
	{
		SensorenenWaffenwechsel(5);
	}
	if(Equal(a, 8))
	{
		SensorenenWaffenwechsel(6);
	}
	if(Equal(a, 9))
	{
		SensorenenWaffenwechsel(7);
	}
	if(Equal(a, 10))
	{
		SensorenenWaffenwechsel(8);
	}
        return(1);	
}
