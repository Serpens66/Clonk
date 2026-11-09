/*-- Space Launch Gebäude mit Geschützen --*/

#strict 3
#appendto SMBS //Hauptbasis
#appendto STOW //Wachturm

//dadurch können die Waffengeschütze auf den Gebäuden per Sensoren gesteuert werden.


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
		return("kein Waffengeschütz");
	}
	if(Equal(a, 4))
	{
		return("Laser-Geschütz");
	}
	if(Equal(a, 5))
	{
		return("Maschinengewehr-Geschütz");
	}
	if(Equal(a, 6))
	{
		return("Vulcan-Geschütz");
	}
	if(Equal(a, 7))
	{
		return("Elektrowaffe-Geschütz");
	}
	if(Equal(a, 8))
	{
		return("Raketenwerfer-Geschütz");
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
        return(1);	
}
