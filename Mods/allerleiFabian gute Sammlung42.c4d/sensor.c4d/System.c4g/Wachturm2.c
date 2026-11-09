/*-- Gebäude mit Geschützen --*/

#strict
#appendto CGDT //Wachturm bzw. GuardTower

//dadurch können die Waffengeschütze auf den Gebäuden per Sensoren gesteuert werden.


public func SensorenenWaffenwechsel(zahl)
{
if(zahl==1) {ChangeWeapon(SYNW,0);return(1);} //keine Waffe
if(zahl==2 && GetPlrKnowledge(GetOwner(),SLSW)) {ChangeWeapon(SLSW,0);return(1);} //Laser
if(zahl==3 && GetPlrKnowledge(GetOwner(),STW1)) {ChangeWeapon(STW1,0);return(1);} //Maschinenegewehr
if(zahl==4 && GetPlrKnowledge(GetOwner(),STW4)) {ChangeWeapon(STW4,0);return(1);} //Vulcan
if(zahl==5 && GetPlrKnowledge(GetOwner(),STW3)) {ChangeWeapon(STW3,0);return(1);} //Elektrowaffe
if(zahl==6 && GetPlrKnowledge(GetOwner(),STW2)) {ChangeWeapon(STW2,0);return(1);} //Raketenwerfer

if(zahl==1) {ChangeWeapon(SYNW,1);return(1);} //keine Waffe
if(zahl==2 && GetPlrKnowledge(GetOwner(),SLSW)) {ChangeWeapon(SLSW,1);return(1);} //Laser
if(zahl==3 && GetPlrKnowledge(GetOwner(),STW1)) {ChangeWeapon(STW1,1);return(1);} //Maschinenegewehr
if(zahl==4 && GetPlrKnowledge(GetOwner(),STW4)) {ChangeWeapon(STW4,1);return(1);} //Vulcan
if(zahl==5 && GetPlrKnowledge(GetOwner(),STW3)) {ChangeWeapon(STW3,1);return(1);} //Elektrowaffe
if(zahl==6 && GetPlrKnowledge(GetOwner(),STW2)) {ChangeWeapon(STW2,1);return(1);} //Raketenwerfer

PlayerMessage(GetOwner(),Format("Für Geschütz auf %s fehlt die Forschung",this()),this());return(1);
}


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles, geht darum, dass in diesen Gebäude nicht nur auf und abschließen sein soll, sondern auch anderes
andere=1;
return(_inherited(a,b,c,d,e,f,g));
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
		return("Links: Laser-Geschütz");
	}
	if(a==5)
	{
		return("Links: Maschinengewehr-Geschütz");
	}
	if(a==6)
	{
		return("Links: Vulcan-Geschütz");
	}
	if(a==7)
	{
		return("Links: Elektrowaffe-Geschütz");
	}
	if(a==8)
	{
		return("Links: Raketenwerfer-Geschütz");
	}
	if(a==9)
	{
		return("Rechts: kein Waffengeschütz");
	}
	if(a==10)
	{
		return("Rechts: Laser-Geschütz");
	}
	if(a==11)
	{
		return("Rechts: Maschinengewehr-Geschütz");
	}
	if(a==12)
	{
		return("Rechts: Vulcan-Geschütz");
	}
	if(a==13)
	{
		return("Rechts: Elektrowaffe-Geschütz");
	}
	if(a==14)
	{
		return("Rechts: Raketenwerfer-Geschütz");
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
	if(a==11)
	{
		SensorenenWaffenwechsel(9);
	}
	if(a==12)
	{
		SensorenenWaffenwechsel(10);
	}
	if(a==13)
	{
		SensorenenWaffenwechsel(11);
	}
	if(a==14)
	{
		SensorenenWaffenwechsel(12);
	}
        return(1);	
}
