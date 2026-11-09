/*-- Gebäude mit Geschützen --*/

#strict 3
#appendto CGDT //Wachturm bzw. GuardTower

//dadurch können die Waffengeschütze auf den Gebäuden per Sensoren gesteuert werden.


public func SensorenenWaffenwechsel(zahl)
{
if(Equal(zahl, 1)) {ChangeWeapon(SYNW,nil);return(1);} //keine Waffe
if(And(Equal(zahl, 2), GetPlrKnowledge(GetOwner(),SLSW))) {ChangeWeapon(SLSW,nil);return(1);} //Laser
if(And(Equal(zahl, 3), GetPlrKnowledge(GetOwner(),STW1))) {ChangeWeapon(STW1,nil);return(1);} //Maschinenegewehr
if(And(Equal(zahl, 4), GetPlrKnowledge(GetOwner(),STW4))) {ChangeWeapon(STW4,nil);return(1);} //Vulcan
if(And(Equal(zahl, 5), GetPlrKnowledge(GetOwner(),STW3))) {ChangeWeapon(STW3,nil);return(1);} //Elektrowaffe
if(And(Equal(zahl, 6), GetPlrKnowledge(GetOwner(),STW2))) {ChangeWeapon(STW2,nil);return(1);} //Raketenwerfer

if(Equal(zahl, 1)) {ChangeWeapon(SYNW,1);return(1);} //keine Waffe
if(And(Equal(zahl, 2), GetPlrKnowledge(GetOwner(),SLSW))) {ChangeWeapon(SLSW,1);return(1);} //Laser
if(And(Equal(zahl, 3), GetPlrKnowledge(GetOwner(),STW1))) {ChangeWeapon(STW1,1);return(1);} //Maschinenegewehr
if(And(Equal(zahl, 4), GetPlrKnowledge(GetOwner(),STW4))) {ChangeWeapon(STW4,1);return(1);} //Vulcan
if(And(Equal(zahl, 5), GetPlrKnowledge(GetOwner(),STW3))) {ChangeWeapon(STW3,1);return(1);} //Elektrowaffe
if(And(Equal(zahl, 6), GetPlrKnowledge(GetOwner(),STW2))) {ChangeWeapon(STW2,1);return(1);} //Raketenwerfer

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
		return("Links: Laser-Geschütz");
	}
	if(Equal(a, 5))
	{
		return("Links: Maschinengewehr-Geschütz");
	}
	if(Equal(a, 6))
	{
		return("Links: Vulcan-Geschütz");
	}
	if(Equal(a, 7))
	{
		return("Links: Elektrowaffe-Geschütz");
	}
	if(Equal(a, 8))
	{
		return("Links: Raketenwerfer-Geschütz");
	}
	if(Equal(a, 9))
	{
		return("Rechts: kein Waffengeschütz");
	}
	if(Equal(a, 10))
	{
		return("Rechts: Laser-Geschütz");
	}
	if(Equal(a, 11))
	{
		return("Rechts: Maschinengewehr-Geschütz");
	}
	if(Equal(a, 12))
	{
		return("Rechts: Vulcan-Geschütz");
	}
	if(Equal(a, 13))
	{
		return("Rechts: Elektrowaffe-Geschütz");
	}
	if(Equal(a, 14))
	{
		return("Rechts: Raketenwerfer-Geschütz");
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
	if(Equal(a, 11))
	{
		SensorenenWaffenwechsel(9);
	}
	if(Equal(a, 12))
	{
		SensorenenWaffenwechsel(10);
	}
	if(Equal(a, 13))
	{
		SensorenenWaffenwechsel(11);
	}
	if(Equal(a, 14))
	{
		SensorenenWaffenwechsel(12);
	}
        return(1);	
}
