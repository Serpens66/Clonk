/*-- Space Launch Gebäude mit Geschützen --*/

#strict
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
		return("kein Waffengeschütz");
	}
	if(a==4)
	{
		return("Laser-Geschütz");
	}
	if(a==5)
	{
		return("Maschinengewehr-Geschütz");
	}
	if(a==6)
	{
		return("Vulcan-Geschütz");
	}
	if(a==7)
	{
		return("Elektrowaffe-Geschütz");
	}
	if(a==8)
	{
		return("Raketenwerfer-Geschütz");
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
        return(1);	
}
