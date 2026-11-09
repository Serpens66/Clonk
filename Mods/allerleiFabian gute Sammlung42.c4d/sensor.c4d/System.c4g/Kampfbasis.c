/*-- Gebäude mit Geschützen --*/

#strict
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
if(zahl==1) {ChangeWeapon(SYNW,0);return(1);} //Links: keine Waffe
if(zahl==2 && GetPlrKnowledge(GetOwner(),CMG1)) {ChangeWeapon(CMG1,0);return(1);}  //Links: Minigun-Geschütz
if(zahl==3 && GetPlrKnowledge(GetOwner(),CPG1)) {ChangeWeapon(CPG1,0);return(1);}  //Links: Plasma-Geschütz
if(zahl==4 && GetPlrKnowledge(GetOwner(),CPG2)) {ChangeWeapon(CPG2,0);return(1);}  //Links: Twin-Plasma-Geschütz
if(zahl==5) {ChangeWeapon(SYNW,1);return(1);}  //Rechts: keine Waffe
if(zahl==6 && GetPlrKnowledge(GetOwner(),CMG1)) {ChangeWeapon(CMG1,1);return(1);}  //Rechts: Minigun-Geschütz
if(zahl==7 && GetPlrKnowledge(GetOwner(),CPG1)) {ChangeWeapon(CPG1,1);return(1);}  //Rechts: Plasma-Geschütz
if(zahl==8 && GetPlrKnowledge(GetOwner(),CPG2)) {ChangeWeapon(CPG2,1);return(1);}  //Rechts: Twin-Plasma-Geschütz
PlayerMessage(GetOwner(),Format("Für Geschütz auf %s fehlt die Forschung",this()),this());return(1);
}

//Target

public func GetTriggerName(a)
{

	if(a==1)
	{
		return("Abschließen");
	}

	if(a==2)
	{
		return("Aufschließen");
	}

	if(a==3)
	{
		return("Links: kein Waffengeschütz");
	}

	if(a==4)
	{
		return("Links: Minigun-Geschütz");
	}

	if(a==5)
	{
		return("Links: Plasma-Geschütz");
	}

        if(a==6)
	{
	        return("Links: Twin-Plasma-Geschütz");
        }

	if(a==7)
	{
		return("Rechts: kein Waffengeschütz");
	}

 	if(a==8)
	{
		return("Rechts: Minigun-Geschütz");
	}

	if(a==9)
	{
		return("Rechts: Plasma-Geschütz");
	}

	if(a==10)
	{
		return("Rechts: Twin-Plasma-Geschütz");
	}

  return(1);
}

public func Trigger(a)
{
	if(a==1)
	{
			zuu=1;
	}
	if(a==2)
	{
			zuu=0;
	}
	if(a==3)
	{
		SensorenenWaffenwechsel(1);
	}
	if(a==4)
	{
		SensorenenWaffenwechsel(2);
	}
	if(a==5)
	{
		SensorenenWaffenwechsel(3);
	}
	if(a==6)
	{
		SensorenenWaffenwechsel(4);
	}
	if(a==7)
	{
		SensorenenWaffenwechsel(5);
	}
	if(a==8)
	{
		SensorenenWaffenwechsel(6);
	}
	if(a==9)
	{
		SensorenenWaffenwechsel(7);
	}
	if(a==10)
	{
		SensorenenWaffenwechsel(8);
	}
        return(1);	
}
